// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "KeyObject.h"


// Sets default values
AKeyObject::AKeyObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AKeyObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeyObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AKeyObject::PickUp()
{
	//play sound or what evs
	//this->BeginDestroy();
}

