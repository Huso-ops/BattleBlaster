// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 1000.0f;
	ProjectileMovementComponent->MaxSpeed = 1000.0f;


	TrailParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailParticles"));
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{	
	AActor* MyOwner = GetOwner();
	const UWorld* World = GetWorld();

	const bool bShouldApplyEffects = IsValid(MyOwner) && IsValid(OtherActor) && OtherActor != MyOwner && OtherActor != this && World;

	if (bShouldApplyEffects)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, UDamageType::StaticClass());

		const FVector CurrentActorLocation = GetActorLocation();

		if (HitParticles)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, HitParticles, CurrentActorLocation, GetActorRotation());
		}

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(World, HitSound, CurrentActorLocation);
		}

		if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0); PlayerController && HitCameraShakeClass)
		{
			PlayerController->ClientStartCameraShake(HitCameraShakeClass);
		}
	}

	Destroy();
}

