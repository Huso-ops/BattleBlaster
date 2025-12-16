// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"

// Sets default values
ABasePawn::ABasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMash"));
	BaseMesh->SetupAttachment(CapsuleComp);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMash"));
	TurretMesh->SetupAttachment(BaseMesh);
}

void ABasePawn::RotateTurret(const FVector& TargetLocation)
{
	FVector VectorToTarget = TargetLocation - TurretMesh->GetComponentLocation();

	FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f);

	FRotator InterpolatedRotation = FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), 5.0f);

	TurretMesh->SetWorldRotation(LookAtRotation);
}

