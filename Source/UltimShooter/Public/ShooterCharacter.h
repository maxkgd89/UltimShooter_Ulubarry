// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "inputActionValue.h"
#include "ShooterCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class ULTIMSHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    /** Movement Functions */
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

	// Optional: For turn rates if you want to mimic old behavior
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float BaseTurnRate = 45.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    float BaseLookUpRate = 45.f;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

};
