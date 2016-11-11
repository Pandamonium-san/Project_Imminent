// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "HeavyCaveDoor.h"


// Sets default values
AHeavyCaveDoor::AHeavyCaveDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	DoorMesh->SetupAttachment(RootComponent);
	DoorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	DoorMesh->SetVisibility(true);
}

// Called when the game starts or when spawned
void AHeavyCaveDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHeavyCaveDoor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AHeavyCaveDoor::OpenDoor()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("HCD OpenDoor"));
	DoorMesh->SetVisibility(false);
	DoorMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

