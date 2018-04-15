// Fill out your copyright notice in the Description page of Project Settings.

#include "MULevelManager.h"
#include "MUSubLevelManager.h"
#include "MUGameInstance.h"

UMULevelManager* UMULevelManager::GetLevelManager()
{
	return UMUGameInstance::LevelManager;
}

void UMULevelManager::LoadSubLevelStreaming(UObject* WorldContextObject, FName LevelName, bool bMakeVisibleAfterLoad, bool bShouldBlockOnLoad)
{
	if (!this->SubLevelManager)
	{
		this->SubLevelManager = WorldContextObject->GetWorld()->SpawnActor<AMUSubLevelManager>();
	}
	this->SubLevelManager->LoadSubLevelStreaming(LevelName, bMakeVisibleAfterLoad, bShouldBlockOnLoad);
}

void UMULevelManager::UnloadSubLevelStreaming(UObject* WorldContextObject, FName LevelName, float Delay)
{
	if (!this->SubLevelManager)
	{
		this->SubLevelManager = WorldContextObject->GetWorld()->SpawnActor<AMUSubLevelManager>();
	}
	this->SubLevelManager->UnloadSubLevelStreaming(LevelName, Delay);
}


//TWeakPtr<AMUSubLevelManager> UMULevelManager::GetSubLevelManager()
AMUSubLevelManager* UMULevelManager::GetSubLevelManager()
{
	return nullptr;
}


