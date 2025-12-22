// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "InputMappingContext.h"

// Sets default values
ATank::ATank()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(Controller);

	if (!IsValid(PlayerController))
	{
		return;
	}

	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			SubSystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	SetInputPlayerEnabled(false);
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController)
	{
		FHitResult HitResult;

		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

		RotateTurret(HitResult.ImpactPoint, TowerRotateSpeed);

		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 20, FColor::Blue, false);
	}
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::MoveInput);

		EIC->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::TurnInput);

		EIC->BindAction(FireAction, ETriggerEvent::Started, this, &ATank::Fire);
	}
}

void ATank::MoveInput(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>();

	FVector DeltaLocation = FVector(Speed * InputValue * GetWorld()->GetDeltaSeconds(), 0.0f, 0.0f);

	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::TurnInput(const FInputActionValue& Value)
{
	const float InputValue = Value.Get<float>();

	FRotator DeltaRotation = FRotator(0.0f, 0.0f, 0.0f);
	DeltaRotation.Yaw = TurnRate * InputValue * GetWorld()->GetDeltaSeconds();

	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleDesctruction()
{
	Super::HandleDesctruction();

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetInputPlayerEnabled(false);

	bIsAlive = false;
}

void ATank::SetInputPlayerEnabled(const bool& Enabled)
{
	if (!IsValid(PlayerController))
	{
		return;
	}

	PlayerController->bShowMouseCursor = Enabled;

	if (Enabled)
	{
		EnableInput(PlayerController);
	} 
	else 
	{
		DisableInput(PlayerController);
	}
}

