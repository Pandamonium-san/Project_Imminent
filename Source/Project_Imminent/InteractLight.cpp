// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "InteractLight.h"


// Sets default values
AInteractLight::AInteractLight()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	light = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	light->SetupAttachment(RootComponent);
	light->SetIntensity(4000);
	light->SetLightColor(FLinearColor(280, 50, 50), true);
	light->SetSourceRadius(400);
	light->SetAttenuationRadius(500);
	
	hitbox = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	hitbox->InitSphereRadius(10);
	hitbox->SetupAttachment(RootComponent);
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

