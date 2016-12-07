// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "KeyTrigger.h"
#include "SwingDoor.h"
#include "KeyObject.h"




void UKeyTrigger::TriggerEvent_Implementation()
{

	for (TActorIterator<ASwingDoor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->bEnabled = true;
		for (TActorIterator<AKeyObject> KeyItr(GetWorld()); KeyItr; ++KeyItr)
		{
			KeyItr->PickUp();
			break;
		}
		break;		
	}

#ifdef UE_BUILD_DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "KeyIsPickedUp activated.");
#endif
}