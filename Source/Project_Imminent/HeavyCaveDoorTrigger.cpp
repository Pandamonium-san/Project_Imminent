// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "HeavyCaveDoorTrigger.h"
#include "HeavyCaveDoor.h"




void UHeavyCaveDoorTrigger::TriggerEvent_Implementation()
{

	for (TActorIterator<AHeavyCaveDoor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		//AHeavyCaveDoor *Mesh = *ActorItr;
		ActorItr->OpenDoor();
	}	

#ifdef UE_BUILD_DEBUG
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, "Trigger activated.");
#endif
}