// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerComponent.h"
#include "HeavyCaveDoor.h"

#include "HeavyCaveDoorTrigger.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_IMMINENT_API UHeavyCaveDoorTrigger : public UTriggerComponent
{
	GENERATED_BODY()
	
public:
	virtual void TriggerEvent_Implementation() override;
	
};
