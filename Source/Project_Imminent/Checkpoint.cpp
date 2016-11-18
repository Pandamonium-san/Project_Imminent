// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "Checkpoint.h"


// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	CheckPointHitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CheckPointHitBox->SetupAttachment(RootComponent);

	CheckpointPosition = GetActorLocation();
	CheckpointRotation = GetActorRotation();

	id = "default";

}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckpoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

