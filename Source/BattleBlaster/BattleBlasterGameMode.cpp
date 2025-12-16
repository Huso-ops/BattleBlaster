// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"

void ABattleBlasterGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATower::StaticClass(), Actors);
	TowerCount = Actors.Num();

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		Tank = Cast<ATank>(PlayerPawn);
			
		if (!IsValid(Tank))
		{
			UE_LOG(LogTemp, Warning, TEXT("Game Couldn't Find The Tank Actor"));

			return;
		}
	}

	TArray<ATower*> Towers;
	Towers.Reserve(TowerCount);

	for (AActor* Actor : Actors)
	{
		if (ATower* Tower = Cast<ATower>(Actor))
		{
			Tower->Tank = Tank;

			Towers.Add(Tower);
		}
	}
}
