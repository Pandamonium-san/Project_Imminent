// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "UNavArea_Jump.h"


UUNavArea_Jump::UUNavArea_Jump(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	FNavAreaHelper::Set(AreaFlags, ENavAreaFlag::Jump);
}



