// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "BridgeTrigger.h"
#include "Bridge.h"



void UBridgeTrigger::TriggerEvent_Implementation()
{
	for (TActorIterator<ABridge> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		//ABridge *bridge = *ActorItr;
		ActorItr->ActivateBridge();
	
		
	}
}

