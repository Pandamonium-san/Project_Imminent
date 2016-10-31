// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "UNavArea_Jump.h"


UUNavArea_Jump::UUNavArea_Jump(const class FPostConstructInitializeProperties& PCIP) : Super(PCIP)
{
	FNavAreaHelper::Set(AreaFlags, ENavAreaFlag::Jump);
}



