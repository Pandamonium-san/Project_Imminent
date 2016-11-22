// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "VerticalDoor.h"


// Sets default values
AVerticalDoor::AVerticalDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	StaticMesh->SetupAttachment(RootComponent);

	Speed = 10.0f;
	MovementZLength = 100.0f;
	DeltaPos = 0.0f;
	bIsActive = false;
}

// Called when the game starts or when spawned
void AVerticalDoor::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentPosition = StaticMesh->GetRelativeTransform().GetLocation();
	PointLight->SetVisibility(false);

	
}

// Called every frame
void AVerticalDoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (bIsActive)
		RaiseDoor(DeltaTime);

}

void AVerticalDoor::RaiseDoor(float DeltaTime)
{
	if (DeltaPos <= MovementZLength)
	{
		FVector NewPosition = FVector(CurrentPosition.X, CurrentPosition.Y, CurrentPosition.Z + Speed * DeltaTime);
		StaticMesh->SetRelativeLocation(NewPosition);
		CurrentPosition = NewPosition;
		DeltaPos += Speed * DeltaTime;
	}
	else
	{
		bIsActive = false;
	}
}

void AVerticalDoor::ActivateDoor()
{
	bIsActive = true;
}
