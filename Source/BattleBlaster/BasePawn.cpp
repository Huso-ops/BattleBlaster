// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePawn.h"
#include "Kismet/GameplayStatics.h"

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

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ABasePawn::RotateTurret(const FVector& TargetLocation, const float& RotateSpeed)
{
	const FVector VectorToTarget = TargetLocation - TurretMesh->GetComponentLocation();

	const FRotator LookAtRotation = FRotator(0.0f, VectorToTarget.Rotation().Yaw, 0.0f);

	const FRotator InterpolatedRotation = FMath::RInterpTo(TurretMesh->GetComponentRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), RotateSpeed);

	//const FRotator InterpolatedRotation = FMath::RInterpConstantTo(TurretMesh->GetComponentRotation(), LookAtRotation, GetWorld()->GetDeltaSeconds(), RotateSpeed);

	TurretMesh->SetWorldRotation(InterpolatedRotation);
}

void ABasePawn::Fire()
{
	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());

	Projectile->SetOwner(this);

	//DrawDebugSphere(GetWorld(), ProjectileSpawnPoint->GetComponentLocation(), 20.0f, 20, FColor::Blue, false, 3.0f);
}

void ABasePawn::HandleDesctruction()
{
	const UWorld* World = GetWorld();
	const FVector CurrentActorLocation = GetActorLocation();

	if(DeathParticles)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, DeathParticles, CurrentActorLocation, GetActorRotation());
	}

	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(World, DeathSound, CurrentActorLocation);
	}

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0); PlayerController && DeathCameraShakeClass)
	{
		PlayerController->ClientStartCameraShake(DeathCameraShakeClass);
	}
}

