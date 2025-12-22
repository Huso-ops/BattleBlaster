// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Tank.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsInFireRange())
	{
		RotateTurret(Tank->GetActorLocation(), TowerRotateSpeed);
	}
}

void ATower::CheckFireCondition()
{	
	if (IsInFireRange() && Tank->bIsAlive)
	{
		Fire();
	}
}

bool ATower::IsInFireRange()
{
	bool Result{ false };

	if (IsValid(Tank)) 
	{
		const float TowerDistanceWithTankSquared = FVector::DistSquared(GetActorLocation(), Tank->GetActorLocation());

		Result = (TowerDistanceWithTankSquared <= (FireRange * FireRange));
	}

	return Result;
}

void ATower::HandleDesctruction()
{
	Super::HandleDesctruction();

	Destroy();
}
