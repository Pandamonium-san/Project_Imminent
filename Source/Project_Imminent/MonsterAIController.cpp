// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "MonsterAIController.h"
#include "Project_Imminent/MonsterPathFollowingComponent.h"

AMonsterAIController::AMonsterAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UMonsterPathFollowingComponent>(TEXT("PathFollowComponent")))
{

}




