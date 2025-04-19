// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterTrajectoryComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Crouch
	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 44.f;
	
	CharacterTrajectory = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("Character Trajectory"));
	
	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (Subsystem)
		{
			Subsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
		}
	}
	Super::BeginPlay();

	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DisplayDebugInfo();
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMainCharacter::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainCharacter::Move);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AMainCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMainCharacter::StopSprint);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &AMainCharacter::Crouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AMainCharacter::CrouchStop);
	}

	
}

void AMainCharacter::Move(const FInputActionValue& Value)
{
	//Get the vector
	FVector2D MoveAxis = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MoveAxis.Y);
		AddMovementInput(RightDirection, MoveAxis.X);
	}
}

void AMainCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxis = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxis.Y);
	AddControllerYawInput(LookAxis.X);
}

void AMainCharacter::Sprint(const FInputActionValue& Value)
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeeds[EMovementState::Sprint];;
}
void AMainCharacter::StopSprint(const FInputActionValue& Value)
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeeds[EMovementState::Walk];;
}

void AMainCharacter::Crouch(const FInputActionValue& Value)
{
	Super::Crouch();
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeeds[EMovementState::Crouch];
}

void AMainCharacter::CrouchStop(const FInputActionValue& Value)
{
	Super::UnCrouch();
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeeds[EMovementState::Walk];;
}

void AMainCharacter::Jump()
{
	if (bCanJumpAgain && !GetCharacterMovement()->IsFalling())
	{
		Super::Jump();
		bCanJumpAgain = false;

		float CurrentCooldown = bIsSprinting ? JumpColdown * 2.0f : JumpColdown;
        
		GetWorldTimerManager().SetTimer(
			JumpCooldownTimer,
			[this]()
			{
				bCanJumpAgain = true;
			},
			CurrentCooldown,
			false
		);
	}
}

// DEBUG
void AMainCharacter::DisplayDebugInfo() const
{
	FVector Velocity = GetVelocity();
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();
	FVector NormalizedVelocity = Velocity.GetSafeNormal2D();

	float Angle = FMath::RadiansToDegrees(FMath::Atan2(
		FVector::DotProduct(Right, NormalizedVelocity),
		FVector::DotProduct(Forward, NormalizedVelocity)
	));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,  // Key
			0.0f,  // Duration (0 = permanent)
			FColor::Green,  // Color
			FString::Printf(TEXT("Movement Direction: %.2f°"), Angle)
		);
	}
}

