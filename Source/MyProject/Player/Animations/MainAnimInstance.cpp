// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyProject/Player/MainCharacter.h"


void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	if (APawn* Pawn = TryGetPawnOwner())
	{
		if (AMainCharacter* Character = Cast<AMainCharacter>(Pawn))
		{
			CharacterMovement = Character->GetCharacterMovement()->Velocity.Size();
			CharacterTrajectory = Character->GetCharacterTrajectory();
		}
	}
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (APawn* Pawn = TryGetPawnOwner())
	{
		if (AMainCharacter* Character = Cast<AMainCharacter>(Pawn))
		{
			CharacterMovement = Character->GetCharacterMovement()->Velocity.Size();
			CharacterTrajectory = Character->GetCharacterTrajectory();
		}
	}
}


