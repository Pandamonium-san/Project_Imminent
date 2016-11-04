// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Navigation/PathFollowingComponent.h"
#include "MonsterPathFollowingComponent.generated.h"


class UNavMovementComponent;
class UCharacterMovementComponent;
/**
 *
 */
UCLASS()
class PROJECT_IMMINENT_API UMonsterPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_UCLASS_BODY()

protected:
	//Cached UCharacterMovementComponent
	UPROPERTY(transient)
	UCharacterMovementComponent* CharacterMoveComp;

public:
	// used to detect properties of a path's segment a character is about to follow
	virtual void SetMoveSegment(int32 SegmentStartIndex) override;

	//used to cache UCharacterMovementComponent we're using in SetMoveSegment implementation
	virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;

};
