// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MULevelManager.generated.h"

class AMUSubLevelManager;
	
UCLASS()
class WATER_API UMULevelManager : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure)
	static UMULevelManager* GetLevelManager();

public:
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void LoadSubLevelStreaming(UObject* WorldContextObject, FName LevelName, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	void UnloadSubLevelStreaming(UObject* WorldContextObject, FName LevelName, float Delay = 2.0f);

private:
	//TWeakPtr<AMUSubLevelManager> GetSubLevelManager();
	AMUSubLevelManager* GetSubLevelManager();

	UPROPERTY(EditAnywhere)
	//TWeakPtr<AMUSubLevelManager> SubLevelManager;
	AMUSubLevelManager* SubLevelManager;

};
