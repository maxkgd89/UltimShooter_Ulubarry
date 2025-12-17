// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

#include "ShooterAnimInstance.generated.h"

class AShooterCharacter;
/*
 */
UCLASS()
class ULTIMSHOOTER_API UShooterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

private:
	//Reference to the owning shooter character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShooterAnimInstance", meta = (AllowPrivateAccess = "true"))
	AShooterCharacter* ShooterCharacter;

	//float speed of the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShooterAnimInstance", meta = (AllowPrivateAccess = "true"))
	float Speed;

	//bIsAccelerating
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShooterAnimInstance", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	//MovementOffsetYaw and LastMovementOffsetYaw
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShooterAnimInstance", meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ShooterAnimInstance", meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;


public:
	UShooterAnimInstance();

	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	virtual void NativeInitializeAnimation() override;


};