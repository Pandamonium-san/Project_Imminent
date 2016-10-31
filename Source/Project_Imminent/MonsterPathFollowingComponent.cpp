// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "MonsterPathFollowingComponent.h"
#include "Project_Imminent/UNavArea_Jump.h"
#include "EngineGlobals.h"

UMonsterPathFollowingComponent::UMonsterPathFollowingComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UMonsterPathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);

	if (CharacterMoveComp != NULL)
	{
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];

		if (FNavAreaHelper::HasJumpFlag(SegmentStart))
		{
			//jump! well... fly-in-straight-line
			CharacterMoveComp->SetMovementMode(MOVE_Flying);
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
		}
		else
		{
			//walk
			CharacterMoveComp->SetMovementMode(MOVE_Walking);
		}
	}
}

void UMonsterPathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
	Super::SetMovementComponent(MoveComp);

	CharacterMoveComp = Cast<UCharacterMovementComponent>(MovementComp);
}

