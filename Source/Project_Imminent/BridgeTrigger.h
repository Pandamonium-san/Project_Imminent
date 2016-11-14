// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerComponent.h"
#include "BridgeTrigger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_IMMINENT_API UBridgeTrigger : public UTriggerComponent
{
	GENERATED_BODY()
	
	
		virtual void TriggerEvent_Implementation() override;

	
};
