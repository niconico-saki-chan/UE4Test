// Fill out your copyright notice in the Description page of Project Settings.

#include "MUSubLevelManager.h"
#include "MUSubLevelLoader.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMUSubLevelManager::AMUSubLevelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMUSubLevelManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMUSubLevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMUSubLevelManager::LoadSubLevelStreaming(FName LevelName, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad)
{
	ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(this->GetWorld(), LevelName);
	if (!LevelStreaming)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not found this level streaming. Name is (%s)."));
		return;
	}
	AMUSubLevelLoader* Loader = this->GetLoader(LevelName);
	Loader->Load(bMakeVisibleAfterLoad, bShouldBlockOnLoad);
}

void AMUSubLevelManager::UnloadSubLevelStreaming(FName LevelName, float Delay)
{
	ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(this->GetWorld(), LevelName);
	if (!LevelStreaming)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not found this level streaming. Name is (%s)."));
		return;
	}
	AMUSubLevelLoader* Loader = this->GetLoader(LevelName);
	Loader->Unload(Delay);
}

AMUSubLevelLoader* AMUSubLevelManager::GetLoader(FName LevelName)
{
	AMUSubLevelLoader* Loader = this->SubLevelLoaders.FindRef(LevelName);
	if (!Loader)
	{
		FActorSpawnParameters SpawnParams;
		Loader = this->GetWorld()->SpawnActor<AMUSubLevelLoader>(SpawnParams);
		this->SubLevelLoaders.Add(LevelName, Loader);
		TFunction<void(FMUSubLevelLoadCallback*)> LoadedCallback = [&](FMUSubLevelLoadCallback* Level) {
			UE_LOG(LogTemp, Warning, TEXT("Loaded"));
		};
		TFunction<void(FMUSubLevelLoadCallback*)> UnloadedCallback = [&](FMUSubLevelLoadCallback* Level) {
			UE_LOG(LogTemp, Warning, TEXT("Unloaded"));
		};
		Loader->Setup(LevelName, LoadedCallback, UnloadedCallback);
	}
	return Loader;
}
