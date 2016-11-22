// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "SteelDoorTrigger.h"
#include "VerticalDoor.h"
#include "UnrealEd.h"
#include "EngineUtils.h"





void USteelDoorTrigger::TriggerEvent_Implementation()
{

	for (TActorIterator<AVerticalDoor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		//AHeavyCaveDoor *Mesh = *ActorItr;

		ActorItr->ActivateDoor();
		ActorItr->PointLight->SetVisibility(true);
		break;
	}

#ifdef UE_BUILD_DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "SteelDoorTrigger activated.");
#endif
}