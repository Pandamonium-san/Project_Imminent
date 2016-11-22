// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerComponent.h"
#include "SteelDoorTrigger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_IMMINENT_API USteelDoorTrigger : public UTriggerComponent
{
	GENERATED_BODY()
	
		virtual void TriggerEvent_Implementation() override;

	
	
};
