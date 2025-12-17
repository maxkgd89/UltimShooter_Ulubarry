// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterCharacter.h"
#include "Kismet/KismetMathLibrary.h"

//native initialize animation
UShooterAnimInstance::UShooterAnimInstance()
{
 return;
}

void UShooterAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
    //from here we get 1. velocity speed 2. is accelerating 3. movement offset yaw to feed to the animation blueprint
    if (ShooterCharacter == nullptr)
    {
        ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
    }

    if (ShooterCharacter)
    {
        //Get the lateral speed of the character from velocity
        FVector Velocity = ShooterCharacter->GetVelocity();
        Velocity.Z = 0;
        Speed = Velocity.Size();

        //Is the character accelerating
        if (ShooterCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
        {
            bIsAccelerating = true;
        }
        else
        {
            bIsAccelerating = false;
        }

        //Calculate movement offset yaw
        FRotator AimRotation = ShooterCharacter->GetBaseAimRotation();
        FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ShooterCharacter->GetVelocity());
        MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

        //Set last movement offset yaw
        //Calculate LastMovementOffsetYaw: Check if ShooterCharacter->GetVelocity().Size() is greater than zero. If it is, update LastMovementOffsetYaw to equal MovementOffsetYaw
        if (ShooterCharacter->GetVelocity().Size() > 0.f)
        {
            LastMovementOffsetYaw = MovementOffsetYaw;
        }
    }
}
    //native initialize animation

    void UShooterAnimInstance::NativeInitializeAnimation()
    {
        ShooterCharacter = Cast<AShooterCharacter>(TryGetPawnOwner());
    }


