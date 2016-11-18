// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "InteractLight.h"
#include "Monster.h"
#include "Engine.h"


// Sets default values
AInteractLight::AInteractLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	hitbox->InitSphereRadius(10);
	hitbox->bGenerateOverlapEvents = true;
	hitbox->SetCollisionProfileName("OverlapAll");
	hitbox->AttachTo(RootComponent);

	light = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	light->SetIntensity(4000);
	light->SetSourceRadius(400);
	light->SetAttenuationRadius(500);
	light->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AInteractLight::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractLight::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}