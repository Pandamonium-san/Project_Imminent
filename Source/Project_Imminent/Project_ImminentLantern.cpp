// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "Project_ImminentLantern.h"


// Sets default values
AProject_ImminentLantern::AProject_ImminentLantern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	HandleMeshWithSocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleMeshWithSocket"));
	HandleMeshWithSocket->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanternMesh"));
	LanternMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	LightSource = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightSource"));
	LightSource->AttachToComponent(LanternMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ForwardSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("ForwardSpotLight"));
	ForwardSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
	SpotLightArray.Add(ForwardSpotLight);

	LeftSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("LeftSpotLight"));
	LeftSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
	SpotLightArray.Add(LeftSpotLight);


	RightSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("RightSpotLight"));
	RightSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
	SpotLightArray.Add(RightSpotLight);


	BackSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("BackSpotLight"));
	BackSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
	SpotLightArray.Add(BackSpotLight);

	MaxIntensity = 5000.0f;
	IntensityConsumptionRate = 100.0f;
	Intensity = MaxIntensity;

}

// Called when the game starts or when spawned
void AProject_ImminentLantern::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProject_ImminentLantern::Tick( float DeltaTime )
{
	if (Intensity >= 1)
		Intensity -= IntensityConsumptionRate;


	Super::Tick( DeltaTime );
}

void AProject_ImminentLantern::ResetIntensity()
{
	for (int32 i = 0; i < SpotLightArray.Num(); i++)
		SpotLightArray[i]->Intensity = MaxIntensity;
}


