// Fill out your copyright notice in the Description page of Project Settings.

#include "MUGameInstance.h"
#include "MULevelManager.h"

UMULevelManager* UMUGameInstance::LevelManager;

void UMUGameInstance::Setup()
{
	UMUGameInstance::LevelManager = NewObject<UMULevelManager>();
}
