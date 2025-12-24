// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleBlasterGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Tank.h"
#include "Tower.h"
#include "BattleBlasterGameInstance.h"
#include "CoreMinimal.h"

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
	
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			BattleBlasterGameInstance = Cast<UBattleBlasterGameInstance>(GameInstance);
		}

		for (AActor* Actor : Actors)
		{
			if (ATower* Tower = Cast<ATower>(Actor))
			{
				Tower->Tank = Tank;
				
				if (BattleBlasterGameInstance)
				{
					const auto [TowerDamagePerLevel, TowerFireRatePerLevel, TowerFireRangeLevel] = BattleBlasterGameInstance->GetTowerDifficulty();

					Tower->Damage += TowerDamagePerLevel;
					Tower->FireRate += TowerFireRatePerLevel;
					Tower->FireRange += TowerFireRangeLevel;
				}
			}
		}
	}

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0); IsValid(PlayerController))
	{
		ScreenMessageWidget = CreateWidget<UScreenMessage>(PlayerController, ScreenMessageClass);

		if (IsValid(ScreenMessageWidget))
		{
			ScreenMessageWidget->AddToPlayerScreen();
			ScreenMessageWidget->SetScreenMessage("Get Ready!!!");
		}
	}

	CountDownSeconds = CountdownDelay;

	GetWorldTimerManager().SetTimer(CountDownTimerHandle, this, &ABattleBlasterGameMode::OnCountdownTimerTimeout, 1.0f, true);
}

void ABattleBlasterGameMode::OnCountdownTimerTimeout()
{
	if (CountDownSeconds <= 0) 
	{
		ScreenMessageWidget->SetScreenMessage("Go");

		Tank->SetInputPlayerEnabled(true);

		ScreenMessageWidget->SetVisibility(ESlateVisibility::Hidden);

		GetWorldTimerManager().ClearTimer(CountDownTimerHandle);

		return;
	}
	
	ScreenMessageWidget->SetScreenMessage(FString::Printf(TEXT("Game will start in %d"), CountDownSeconds));

	CountDownSeconds -= 1;
}

void ABattleBlasterGameMode::ActorDied(AActor* DeadActor)
{
	bool bIsGameOver{ false };

	if (Tank == DeadActor)
	{
		// Tank dead
		Tank->HandleDesctruction();

		bIsGameOver = true;
	} 
	else
	{
		if (ATower* DeadTower = Cast<ATower>(DeadActor); IsValid(DeadTower))
		{
			DeadTower->HandleDesctruction();

			TowerCount--;

			if (TowerCount == 0)
			{
				bIsGameOver = true;
				bIsVictory = true;
			}
		}
	}

	if (bIsGameOver)
	{
		const FString GameOverMessage = bIsVictory ? "Victory!" : "Defeat!";

		ScreenMessageWidget->SetScreenMessage(GameOverMessage);

		ScreenMessageWidget->SetVisibility(ESlateVisibility::Visible);

		FTimerHandle GameOverTimerHandle;
		GetWorldTimerManager().SetTimer(GameOverTimerHandle, this, &ABattleBlasterGameMode::OnGameOverTimerTimeOut, GameOverDelay, false);
	}
}

void ABattleBlasterGameMode::OnGameOverTimerTimeOut()
{
	if (!IsValid(BattleBlasterGameInstance))
	{
		return;
	}

	if (bIsVictory)
	{
		BattleBlasterGameInstance->LoadNextLevel();
	}
	else 
	{
		BattleBlasterGameInstance->RestartCurrentLevel();
	}
}
