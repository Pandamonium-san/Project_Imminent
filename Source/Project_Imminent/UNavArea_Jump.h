// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/Navigation/NavAreas/NavArea.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "UNavArea_Jump.generated.h"

/**
 *  Tutorial: https://wiki.unrealengine.com/Unreal_Engine_AI_Tutorial_-_1_-_Making_AI_Jump_as_a_Part_of_Path_Following
 */
UENUM()
namespace ENavAreaFlag
{
	//up to 15 values
	enum Type
	{
		Default,
		Jump
		//And what not
	};
}

namespace FNavAreaHelper
{
	FORCEINLINE bool IsSet(uint16 Flags, ENavAreaFlag::Type Bit) { return (Flags & (1 << Bit)) != 0; }
	FORCEINLINE void Set(uint16 Flags, ENavAreaFlag::Type Bit) { Flags |= (1 << Bit); }

	FORCEINLINE bool IsNavLink(const FNavPathPoint& PathVert) { return (FNavMeshNodeFlags(PathVert.Flags).PathFlags & RECAST_STRAIGHTPATH_OFFMESH_CONNECTION) != 0; }
	FORCEINLINE bool HasJumpFlag(const FNavPathPoint& PathVert) { return IsSet(FNavMeshNodeFlags(PathVert.Flags).AreaFlags, ENavAreaFlag::Jump); }

}


UCLASS()
class PROJECT_IMMINENT_API UUNavArea_Jump : public UNavArea
{
	GENERATED_UCLASS_BODY()

};
