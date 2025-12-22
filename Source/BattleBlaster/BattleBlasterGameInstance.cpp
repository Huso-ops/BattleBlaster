// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameInstance.h"
#include "Kismet/GameplayStatics.h"

void UBattleBlasterGameInstance::LoadLevel(const int32& Index)
{
	if (Index > 0 && Index <= MaxLevelIndex) 
	{
		CurrentLevelIndex = Index;

		FString LevelName = FString::Printf(TEXT("Level_%d"), Index);

		UGameplayStatics::OpenLevel(GetWorld(), *LevelName);
	}

}

void UBattleBlasterGameInstance::LoadNextLevel()
{
	if (CurrentLevelIndex < MaxLevelIndex)
	{
		LoadLevel(CurrentLevelIndex + 1);
	} 
	else 
	{
		RestartGame();
	}
}

void UBattleBlasterGameInstance::RestartCurrentLevel()
{
	LoadLevel(CurrentLevelIndex);
}


void UBattleBlasterGameInstance::RestartGame()
{
	LoadLevel(1);
}
