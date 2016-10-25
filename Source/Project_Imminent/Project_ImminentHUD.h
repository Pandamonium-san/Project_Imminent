// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once 
#include "GameFramework/HUD.h"
#include "Project_ImminentHUD.generated.h"

UCLASS()
class AProject_ImminentHUD : public AHUD
{
	GENERATED_BODY()

public:
	AProject_ImminentHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

