// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "MechCharacter.generated.h"

UCLASS(config=Game)
class AMechCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	///** Pawn mesh: 1st person view (arms; seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	//TSubobjectPtr<class USkeletalMeshComponent> Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	TSubobjectPtr<class UCameraComponent> FirstPersonCameraComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
		
	virtual FRotator GetViewRotation() const;

	virtual void Tick(float DeltaSeconds);

	float CurrentTurnInput;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Torso)
	FVector2D CurrentTorsoTwist;
	
	FVector2D TorsoInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Torso)
	float TorsoYawTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Torso)
	float TorsoPitchTurnRate;

	//returns total rotation for the pilot in the torso
	UFUNCTION(BlueprintCallable, Category = Torso)
	virtual FRotator GetTorsoWorldRotation();
	UFUNCTION(BlueprintCallable, Category = Torso)
	void AddTorsoPitchInput(float Val);
	UFUNCTION(BlueprintCallable, Category = Torso)
	void AddTorsoYawInput(float Val);
	UFUNCTION(BlueprintCallable, Category = Torso)
		virtual FRotator GetTorsoRotation();


	void SetTorsoTwist(FVector2D newTwist);
protected:

	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// End of APawn interface
};

