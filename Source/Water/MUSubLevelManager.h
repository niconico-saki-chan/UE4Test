// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MUSubLevelManager.generated.h"

class ULevelStreaming;
class AMUSubLevelLoader;

UCLASS()
class WATER_API AMUSubLevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMUSubLevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LoadSubLevelStreaming(FName LevelName, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad);

	void UnloadSubLevelStreaming(FName LevelName, float Delay = 2.0f);

private:
	AMUSubLevelLoader* GetLoader(FName LevelName);

	UPROPERTY(EditAnywhere)
	TMap<FName, AMUSubLevelLoader*> SubLevelLoaders;
};
