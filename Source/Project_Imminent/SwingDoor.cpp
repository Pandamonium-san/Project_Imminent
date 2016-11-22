// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "SwingDoor.h"


// Sets default values
ASwingDoor::ASwingDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	PivotPoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PivotPoint"));
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	PivotPoint->SetupAttachment(RootComponent);
	DoorMesh->SetupAttachment(PivotPoint);
	bEnabled = false;
	bIsActive = false;
	Speed = 10.0f;
	DeltaRot = 0.0f;
	DegreesToRotate = 30.0f;

}

// Called when the game starts or when spawned
void ASwingDoor::BeginPlay()
{
	Super::BeginPlay();

	CurrentRotation = GetActorRotation();

}

// Called every frame
void ASwingDoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (bIsActive)
		OpenDoor(DeltaTime);
}

void ASwingDoor::OpenDoor(float DeltaTime)
{
	if (DeltaRot <= DegreesToRotate)
	{
		FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + Speed * DeltaTime, CurrentRotation.Roll);
		SetActorRotation(NewRotation);
		CurrentRotation = NewRotation;
		DeltaRot += Speed * DeltaTime;
	}
	else
	{
		bIsActive = false;
	}
}

void ASwingDoor::ActivateDoor()
{
	bIsActive = true;
}

