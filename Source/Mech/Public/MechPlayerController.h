// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MechPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MECH_API AMechPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY()
	virtual	void UpdateRotation(float DeltaTime);
	
	
};
