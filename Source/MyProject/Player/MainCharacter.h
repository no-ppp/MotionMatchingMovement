// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "MainCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UMotionTrajectoryComponent;
class UCharacterTrajectoryComponent;

//ENUM of Movement States
UENUM(BlueprintType)
enum class EMovementState : uint8
{
	Walk     UMETA(DisplayName = "Walk"),
	Sprint   UMETA(DisplayName = "Sprint"),
	Crouch   UMETA(DisplayName = "Crouch")
};

UCLASS()
class MYPROJECT_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();
private:
	//Movement State
	UPROPERTY(EditAnywhere, Category = "Movement")
	TMap<EMovementState, float> MovementSpeeds = {
		{EMovementState::Walk, 300.f},
		{EMovementState::Sprint, 600.f},
		{EMovementState::Crouch, 150.f}
	};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	//Movement variables
	UPROPERTY(VisibleAnywhere)
	bool bIsSprinting = false;

	//Jump logic variables
	virtual void Jump() override;
	
	UPROPERTY(VisibleAnywhere)
	FTimerHandle JumpCooldownTimer;
	
	UPROPERTY(EditAnywhere, Category=Movement)
	float JumpColdown = 1.0f;
	
	UPROPERTY(VisibleAnywhere)
	bool bCanJumpAgain = true;


	//Camera variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	UCharacterTrajectoryComponent* CharacterTrajectory;
		
	//inputs Actions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	UInputAction* CrouchAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input)
	TSoftObjectPtr<UInputMappingContext> InputMapping;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//debug
	void DisplayDebugInfo() const;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool IsSprinting() const { return bIsSprinting; }

	UFUNCTION(BlueprintCallable)
	UCharacterTrajectoryComponent* GetCharacterTrajectory() const { return CharacterTrajectory; }

protected:
// Input Functions
	UFUNCTION(BlueprintCallable, Category = "Input")
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Sprint(const FInputActionValue& Value);
	void StopSprint(const FInputActionValue& Value);
	void Crouch(const FInputActionValue& Value);
	void CrouchStop(const FInputActionValue& Value);
};


