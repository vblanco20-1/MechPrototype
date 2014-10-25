// Fill out your copyright notice in the Description page of Project Settings.

#include "Mech.h"
#include "MechPlayerController.h"
#include "MechCharacter.h"
#include "Engine.h"
//#include "GameplayFramework/EnginePrivate.h"
//#include "IHeadMountedDisplay.h"
//#include "GameFramework/h"
AMechPlayerController::AMechPlayerController(const class FPostConstructInitializeProperties& PCIP)
	: Super(PCIP)
{

}



void AMechPlayerController::UpdateRotation(float DeltaTime)
{
	Super::UpdateRotation(DeltaTime);
	////// Calculate Delta to be applied on ViewRotation
	//FRotator DeltaRot(RotationInput);

	//FRotator ViewRotation = GetControlRotation();

	//if (PlayerCameraManager)
	//{
	//	PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);
	//}

	//if (!PlayerCameraManager || !PlayerCameraManager->bFollowHmdOrientation)
	//{
	//	if (IsLocalPlayerController() && GEngine->HMDDevice.IsValid() && GEngine->HMDDevice->IsHeadTrackingAllowed())
	//	{
	//		GEngine->HMDDevice->ApplyHmdRotation(this, ViewRotation);
	//	}
	//}

	//SetControlRotation(ViewRotation);

	//APawn* const P = GetPawnOrSpectator();
	//if (P)
	//{
	//	P->FaceRotation(ViewRotation, DeltaTime);
	//}
}
