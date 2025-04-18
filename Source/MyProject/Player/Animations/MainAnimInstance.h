// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainAnimInstance.generated.h"


class UCharacterTrajectoryComponent;
/**
 * 
 */
UCLASS()
class MYPROJECT_API UMainAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement")
	float CharacterMovement;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement")
	UCharacterTrajectoryComponent* CharacterTrajectory;

private:
	
};
