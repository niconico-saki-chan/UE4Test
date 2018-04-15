#include "MUSubLevelLoader.h"
#include "Engine/LatentActionManager.h"
#include "Engine/Engine.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "MUSubLevelManager.h"

const float AMUSubLevelLoader::DelayOfUnloading = 2.0f;

// Sets default values
AMUSubLevelLoader::AMUSubLevelLoader()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMUSubLevelLoader::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMUSubLevelLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->bIsStaringLoad)
	{
		this->bIsStaringLoad = false;
		this->LoadExe();
	}
	else if (this->bIsWaittingUnload)
	{
		if (this->RemainDurationOfUnloading > 0.0f)
		{
			this->RemainDurationOfUnloading -= DeltaTime;
		}
		if (this->RemainDurationOfUnloading <= 0.0f)
		{
			this->bIsWaittingUnload = false;
			this->UnloadExe();
		}
	}
}

bool AMUSubLevelLoader::Setup(
	FName LevelName,
	TFunction<void(FMUSubLevelLoadCallback*)> LoadedFunc /*= nullptr*/,
	TFunction<void(FMUSubLevelLoadCallback*)> UnloadedFunc /*= nullptr*/)
{
	this->LoadingLevelName = LevelName;
	this->LoadedCallback = LoadedFunc;
	this->UnloadedCallback = UnloadedFunc;
	this->TargetLevelStreaming = UGameplayStatics::GetStreamingLevel(this->GetWorld(), this->LoadingLevelName);
	if (!this->TargetLevelStreaming)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not found this level streaming. Name is (%s)."));
		return false;
	}
	return true;
}

void AMUSubLevelLoader::Load(bool bMakeVisibleAfterLoad /*= false*/, bool bShouldBlockOnLoad /*= false*/)
{
	if (!this->TargetLevelStreaming)
	{
		//存在しない場合は即返す
		this->FireLoadedCallbacks(false);
		return;
	}
	if (!this->bIsStaringLoad)
	{
		if (this->TargetLevelStreaming->IsLevelLoaded())
		{
			if (this->TargetLevelStreaming->IsLevelVisible())
			{
				//見えてはいるがすでにアンロード命令を出している場合はもう一度ロードする
				if (this->bIsUnloading)
				{
					this->bIsStaringLoad = true;
				}
			}
			else if (bMakeVisibleAfterLoad)
			{
				//可視化情報のみの変更はフラグの変更のみで大丈夫
				this->TargetLevelStreaming->bShouldBeVisible = true;
			}
		}
		else
		{
			if (!this->bIsLoading)
			{
				this->bIsStaringLoad = true;
			}
			else if (bShouldBlockOnLoad && !this->TargetLevelStreaming->bShouldBlockOnLoad)
			{
				this->bIsStaringLoad = true;
			}
		}
	}

	this->bMakeVisible = bMakeVisibleAfterLoad;
	this->bIsBlockingLoad = bShouldBlockOnLoad;

	this->CancelUnloading();
}

void AMUSubLevelLoader::Unload(float Delay /*= DelayOfUnloading*/)
{
	if (!this->TargetLevelStreaming)
	{
		//存在しない場合は即返す
		this->FireUnloadedCallbacks(false);
		return;
	}
	if (!this->bIsWaittingUnload && !this->bIsUnloading)
	{
		this->RemainDurationOfUnloading = FMath::Clamp(Delay, 0.0f, Delay);
		if (!this->TargetLevelStreaming->IsLevelLoaded())
		{
			if (this->bIsLoading)
			{
				this->bIsWaittingUnload = true;
			}
		}
		else
		{
			this->bIsWaittingUnload = true;
		}
	}
	CancelLoading();
}

void AMUSubLevelLoader::CancelLoading()
{
	this->bIsStaringLoad = false;
	this->bIsLoading = false;
	this->FireLoadedCallbacks(true);
}

void AMUSubLevelLoader::CancelUnloading()
{
	this->bIsWaittingUnload = false;
	this->bIsUnloading = false;
	this->RemainDurationOfUnloading = 0.0f;
	this->FireUnloadedCallbacks(true);
}

void AMUSubLevelLoader::LoadExe()
{
	if (this->bIsLoading && this->bIsBlockingLoad && !this->TargetLevelStreaming->bShouldBlockOnLoad)
	{
		//ブロッキングロードには途中から変更できないので全レベルストリーミングに対して行ってしまうが強制的にする。
		//TODO : できることならこのレベルについてのみ強制するようにしたい。
		UGameplayStatics::FlushLevelStreaming(this->GetWorld());
	}
	else
	{
		this->bIsLoading = true;
		FLatentActionInfo info;
		info.CallbackTarget = this;
		info.ExecutionFunction = "OnLoaded";
		UGameplayStatics::LoadStreamLevel(this->GetWorld(), this->LoadingLevelName, this->bMakeVisible, this->bIsBlockingLoad, info);
	}
}

void AMUSubLevelLoader::UnloadExe()
{
	this->RemainDurationOfUnloading = 0.0f;
	this->bIsUnloading = true;
	FLatentActionInfo info;
	info.CallbackTarget = this;
	info.ExecutionFunction = "OnUnloaded";
	UGameplayStatics::UnloadStreamLevel(this->GetWorld(), this->LoadingLevelName, info);
}

void AMUSubLevelLoader::OnLoaded()
{
	this->FireLoadedCallbacks();
	this->bIsStaringLoad = false;
	this->bIsLoading = false;
}

void AMUSubLevelLoader::OnUnloaded()
{
	this->FireUnloadedCallbacks();
	this->bIsWaittingUnload = false;
	this->bIsUnloading = false;
	this->RemainDurationOfUnloading = 0.0f;
}

void AMUSubLevelLoader::FireLoadedCallbacks(bool bIsCancel /*= false*/)
{
	if (this->LoadedCallback)
	{
		FMUSubLevelLoadCallback Callback;
		if (this->TargetLevelStreaming)
		{
			Callback = FMUSubLevelLoadCallback(
				this->TargetLevelStreaming, this->TargetLevelStreaming->IsLevelLoaded(),
				this->TargetLevelStreaming->IsLevelVisible(), bIsCancel);

		}
		else
		{
			Callback = FMUSubLevelLoadCallback();
		}
		this->LoadedCallback(&Callback);
	}
}

void AMUSubLevelLoader::FireUnloadedCallbacks(bool bIsCancel /*= false*/)
{
	if (this->UnloadedCallback)
	{
		FMUSubLevelLoadCallback Callback;
		if (this->TargetLevelStreaming)
		{
			Callback = FMUSubLevelLoadCallback(this->TargetLevelStreaming, !this->TargetLevelStreaming->IsLevelLoaded(), false, bIsCancel);
		}
		else
		{
			Callback = FMUSubLevelLoadCallback();
		}
		this->UnloadedCallback(&Callback);
	}
}
