// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
	SpringArmComp->SocketOffset = FVector(0.f,50.f,50.f);

	//Set bUseControllerRotationPitch, bUseControllerRotationYaw, and bUseControllerRotationRoll to false

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	//Access the Character Movement Component: Set bOrientRotationToMovement to true

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f,0.f);
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
 
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
		
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AShooterCharacter::FireWeapon);
	}





}

void AShooterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        
        // get right vector 
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement 
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);	
	}
}

void AShooterCharacter::Look(const FInputActionValue& Value)
{
	 // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(-LookAxisVector.Y);
    }
}

void AShooterCharacter::FireWeapon(const FInputActionValue& Value)
{
	//print to log
	UE_LOG(LogTemp, Warning, TEXT("FireWeapon called"));
	// Try and play the sound if specified
	if (FireSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
	const USkeletalMeshSocket* BarrelSocket = GetMesh()->GetSocketByName("SMG_Barrel");
	//print eulog if the socket is found
	UE_LOG(LogTemp, Warning, TEXT("BarrelSocket: %s"), BarrelSocket ? TEXT("Found") : TEXT("Not Found"));

	
	
	if (BarrelSocket)
	{

			const FTransform SocketTransform = BarrelSocket->GetSocketTransform(GetMesh());
			if (MuzzleFlash)
			
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
			}

			FVector2D ViewportSize;
			if (GEngine && GEngine->GameViewport)
				{GEngine->GameViewport->GetViewportSize(ViewportSize);
				}
			FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
			FVector CrosshairWorldPosition;
			FVector CrosshairWorldDirection;

			bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
				UGameplayStatics::GetPlayerController(this, 0),
				CrosshairLocation,
				CrosshairWorldPosition,
				CrosshairWorldDirection
			);

			if (bScreenToWorld)
			{
				FHitResult ScreenTraceHit;
				const FVector Start = CrosshairWorldPosition;
				const FVector End = Start + CrosshairWorldDirection * 50000.f;
				FVector BeamEndPoint = End;
				GetWorld()->LineTraceSingleByChannel(
					ScreenTraceHit,
					Start,
					End,
					ECollisionChannel::ECC_Visibility);
				if (ScreenTraceHit.bBlockingHit)
				{
					BeamEndPoint = ScreenTraceHit.Location;
					if (ImpactParticles)
					{
						UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(), 
						ImpactParticles, 
						ScreenTraceHit.Location, 
						ScreenTraceHit.ImpactNormal.Rotation());	
				}
				if (BeamParticles)
				{
					UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(), 
						BeamParticles, 
						SocketTransform);
					if (Beam)
					{
						Beam->SetVectorParameter(FName("Target"), BeamEndPoint);
					}
				}
				}
			}

	}
	//play firing montage
	if (HipFireMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			//HipFireMontage is the montage to play
			AnimInstance->Montage_Play(HipFireMontage);
			//StartFire is montage section name
			AnimInstance->Montage_JumpToSection(FName("StartFire"), HipFireMontage);
		}
	}

}


//Define bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation) in ShooterCharacter.cpp
bool AShooterCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation)
{
	return true;
}