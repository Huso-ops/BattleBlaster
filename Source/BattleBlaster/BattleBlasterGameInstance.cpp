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
		IncreaseTowerDifficultyPerLevel();
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
	SetTowerDifficultyReset();
	LoadLevel(1);
}

std::tuple<float, float, float> UBattleBlasterGameInstance::GetTowerDifficulty() const
{
	return std::make_tuple(TowerDamagePerLevel, TowerFireRatePerLevel, TowerFireRangeLevel);
}


void UBattleBlasterGameInstance::IncreaseTowerDifficultyPerLevel()
{

	TowerDamagePerLevel += 10;
	TowerFireRatePerLevel -= 1;
	TowerFireRangeLevel += 100;
}

void UBattleBlasterGameInstance::SetTowerDifficultyReset()
{
	TowerDamagePerLevel = 0;
	TowerFireRatePerLevel = 0;
	TowerFireRangeLevel = 0;
}
