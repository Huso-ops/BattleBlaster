// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ScreenMessage.h"
#include "BattleBlasterGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBLASTER_API ABattleBlasterGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UScreenMessage> ScreenMessageClass;

	UScreenMessage* ScreenMessageWidget;

	class ATank* Tank;

	int32 TowerCount = { 0 };

	bool bIsVictory{ false };

	UPROPERTY(EditAnywhere)
	float GameOverDelay = { 3.0f };

	UPROPERTY(EditAnywhere)
	int32 CountdownDelay{ 3 };

	int32 CountDownSeconds;

	FTimerHandle CountDownTimerHandle;

	void ActorDied(AActor* DeadActor);

	void OnGameOverTimerTimeOut();
	void OnCountdownTimerTimeout();
};
