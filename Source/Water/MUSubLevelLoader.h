#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MUSubLevelLoader.generated.h"

class ULevelStreaming;
class AMUSubLevelManager;

USTRUCT()
struct FMUSubLevelLoadCallback
{
	GENERATED_USTRUCT_BODY()

	FMUSubLevelLoadCallback()
	{
	}

	FMUSubLevelLoadCallback(ULevelStreaming* Level, bool IsSuccess, bool IsVisible, bool IsCancel) :
		LevelStreaming(Level), bIsSuccess(IsSuccess), bIsVisible(IsVisible), bIsCancel(IsCancel)
	{
	}

	UPROPERTY(EditAnywhere)
	ULevelStreaming* LevelStreaming;

	UPROPERTY(EditAnywhere)
	bool bIsSuccess;

	UPROPERTY(EditAnywhere)
	bool bIsVisible;

	UPROPERTY(EditAnywhere)
	bool bIsCancel;
};

UCLASS()
class WATER_API AMUSubLevelLoader : public AActor
{
	GENERATED_BODY()

private:
	static const float DelayOfUnloading;

public:
	// Sets default values for this actor's properties
	AMUSubLevelLoader();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool Setup(FName LevelName, TFunction<void(FMUSubLevelLoadCallback*)> LoadedFunc = nullptr, TFunction<void(FMUSubLevelLoadCallback*)> UnloadedFunc = nullptr);

	void Load(bool bMakeVisibleAfterLoad = false, bool bShouldBlockOnLoad = false);

	void Unload(float Delay = DelayOfUnloading);

	void CancelLoading();

	void CancelUnloading();


private:
	void LoadExe();

	void UnloadExe();

	void OnLoaded();

	void OnUnloaded();

	void FireLoadedCallbacks(bool bIsCancel = false);

	void FireUnloadedCallbacks(bool bIsCancel = false);

private:
//	UPROPERTY(EditAnywhere)
	TFunction<void(FMUSubLevelLoadCallback*)> LoadedCallback;

//	UPROPERTY(EditAnywhere)
	TFunction<void(FMUSubLevelLoadCallback*)> UnloadedCallback;

	UPROPERTY(EditAnywhere)
	int32 UUIDCount;

	UPROPERTY(EditAnywhere)
	FName LoadingLevelName;

	UPROPERTY(EditAnywhere)
	ULevelStreaming* TargetLevelStreaming;

	UPROPERTY(EditAnywhere)
	bool bIsStaringLoad;

	UPROPERTY(EditAnywhere)
	bool bIsLoading;

	UPROPERTY(EditAnywhere)
	bool bMakeVisible;

	UPROPERTY(EditAnywhere)
	bool bIsBlockingLoad;

	UPROPERTY(EditAnywhere)
	bool bIsWaittingUnload;

	UPROPERTY(EditAnywhere)
	bool bIsUnloading;

	UPROPERTY(EditAnywhere)
	float RemainDurationOfUnloading;
};
