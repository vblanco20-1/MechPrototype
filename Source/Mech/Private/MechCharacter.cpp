// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#include "Mech.h"
#include "MechCharacter.h"
#include "MechProjectile.h"
#include "Animation/AnimInstance.h"
#include "Engine.h"
//#include "GameplayFramework/EnginePrivate.h"
#include "IHeadMountedDisplay.h"

//////////////////////////////////////////////////////////////////////////
// AMechCharacter

AMechCharacter::AMechCharacter(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{
	// Set size for collision capsule
	CapsuleComponent->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = PCIP.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = Mesh;
	FirstPersonCameraComponent->AttachSocketName = FName("ViewPoint");
	//FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;



	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMechCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	InputComponent->BindAction("Fire", IE_Pressed, this, &AMechCharacter::OnFire);


	InputComponent->BindAxis("MoveForward", this, &AMechCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AMechCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AMechCharacter::TurnAtRate);
	//InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AMechCharacter::LookUpAtRate);
}

void AMechCharacter::OnFire()
{
	//// try and fire a projectile
	//if (ProjectileClass != NULL)
	//{
	//	const FRotator SpawnRotation = GetControlRotation();
	//	// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
	//	const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(GunOffset);

	//	UWorld* const World = GetWorld();
	//	if (World != NULL)
	//	{
	//		// spawn the projectile at the muzzle
	//		World->SpawnActor<AMechProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
	//	}
	//}

	//// try and play the sound if specified
	//if (FireSound != NULL)
	//{
	//	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	//}

	//// try and play a firing animation if specified
	//if(FireAnimation != NULL)
	//{
	//	// Get the animation object for the arms mesh
	//	UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
	//	if(AnimInstance != NULL)
	//	{
	//		AnimInstance->Montage_Play(FireAnimation, 1.f);
	//	}
	//}

}



void AMechCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMechCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMechCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	CurrentTurnInput = Rate;

}

void AMechCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	//AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	AddTorsoPitchInput(Rate);
}

FRotator AMechCharacter::GetViewRotation() const
{
	return  Super::GetViewRotation();
}

void AMechCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// Torso Rotation	
	CurrentTorsoTwist += TorsoInput * DeltaSeconds;
	CurrentTorsoTwist.X = FMath::Clamp(CurrentTorsoTwist.X, -1.f, 1.f);
	CurrentTorsoTwist.Y = FMath::Clamp(CurrentTorsoTwist.Y, -1.f, 1.f);

	// General Rotation
	float DeltaYaw = CurrentTurnInput * BaseTurnRate * DeltaSeconds;
	//rotate the pawn
	FRotator PawnRot = GetActorRotation();
	PawnRot.Yaw += DeltaYaw;
	SetActorRotation(PawnRot);

	//Set controller rotation from the torso rotation
	FRotator ControlRot = GetTorsoWorldRotation();

	// add Oculus Rift offset to the controller rotation
	if (GetController()->IsLocalPlayerController() && GEngine->HMDDevice.IsValid() && GEngine->HMDDevice->IsHeadTrackingAllowed())
	{
		FQuat HMDRot;
		FVector HMDPos;
		GEngine->HMDDevice->GetCurrentOrientationAndPosition(HMDRot, HMDPos);
		ControlRot += HMDRot.Rotator();
	}	
	GetController()->SetControlRotation(ControlRot);	
}

void AMechCharacter::AddTorsoPitchInput(float Val)
{
	TorsoInput.Y = Val;
}

void AMechCharacter::AddTorsoYawInput(float Val)
{
	TorsoInput.X = Val;
}

void AMechCharacter::SetTorsoTwist(FVector2D newTwist)
{
	CurrentTorsoTwist = newTwist;
}

FRotator AMechCharacter::GetTorsoWorldRotation()
{
	FRotator OffsetRotation;
	OffsetRotation.Pitch = CurrentTorsoTwist.Y * 90;
	OffsetRotation.Yaw = CurrentTorsoTwist.X * 90;
	FRotator FinalRotation = OffsetRotation + GetActorRotation();
	return FinalRotation;
}

FRotator AMechCharacter::GetTorsoRotation()
{
	FRotator OffsetRotation;

	OffsetRotation.Pitch = CurrentTorsoTwist.Y * 90;
	OffsetRotation.Yaw = CurrentTorsoTwist.X * 90;

	return OffsetRotation;
}
