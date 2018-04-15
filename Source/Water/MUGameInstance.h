// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "MUGameInstance.generated.h"

class UMULevelManager;

UCLASS()
class WATER_API UMUGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public:
	static UMULevelManager* LevelManager;

public:
	UFUNCTION(BlueprintCallable)
	void Setup();
};
