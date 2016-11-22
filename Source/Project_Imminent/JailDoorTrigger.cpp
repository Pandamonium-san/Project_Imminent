// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "JailDoorTrigger.h"
#include "SwingDoor.h"
#include "UnrealEd.h"
#include "EngineUtils.h"




void UJailDoorTrigger::TriggerEvent_Implementation()
{

	for (TActorIterator<ASwingDoor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		//AHeavyCaveDoor *Mesh = *ActorItr;
		if (ActorItr->bEnabled)
		{
			ActorItr->ActivateDoor();
			break;
		}
		break;		
	}

#ifdef UE_BUILD_DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "SwingDoorTrigger activated.");
#endif
}