// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "Project_ImminentLantern.h"
#include "Project_ImminentCharacter.h"
#include "Engine.h"


// Sets default values
AProject_ImminentLantern::AProject_ImminentLantern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	HandleMesh= CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleMesh"));
	HandleMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanternMesh"));
	LanternMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	LanternMesh->SetSimulatePhysics(true);

	LightSource = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightSource"));
	LightSource->AttachToComponent(LanternMesh, FAttachmentTransformRules::KeepRelativeTransform);

	ForwardSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("ForwardSpotLight"));
	ForwardSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
	SpotLightArray.Add(ForwardSpotLight);

	LeftSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("LeftSpotLight"));
	LeftSpotLight->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	LeftSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
	SpotLightArray.Add(LeftSpotLight);	
	
	RightSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("RightSpotLight"));
	RightSpotLight->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	RightSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
	SpotLightArray.Add(RightSpotLight);

	BackSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("BackSpotLight"));
	BackSpotLight->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	BackSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
	SpotLightArray.Add(BackSpotLight);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProject_ImminentLantern::OnOverlapBegin);
	CollisionComponent->AttachToComponent(LanternMesh, FAttachmentTransformRules::KeepRelativeTransform);

	//PhysicsConstraint = NewObject<UPhysicsConstraintComponent>(this);

	for (int32 i = 0; i < SpotLightArray.Num(); i++)
	{
		SpotLightArray[i]->LightColor.R = 230.0f;
		SpotLightArray[i]->LightColor.G = 179.0f;
		SpotLightArray[i]->LightColor.B = 111.0f;
		SpotLightArray[i]->OuterConeAngle = 80.0f;
		SpotLightArray[i]->AttenuationRadius = 2000.0f;
	}
	
	
	MaxIntensity = 5000.0f;
	IntensityConsumptionRate = 100.0f;
	Intensity = MaxIntensity;
	bAttached = false;

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

void AProject_ImminentLantern::OnOverlapBegin(class UPrimitiveComponent* OverlappingComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bAttached)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));

		if (Cast<AProject_ImminentCharacter>(OtherActor))
		{
			AProject_ImminentCharacter* other = Cast<AProject_ImminentCharacter>(OtherActor);
			HandleMesh->AttachToComponent(other->HandleMeshWithSocket, FAttachmentTransformRules::KeepRelativeTransform);
			bAttached = true;
		}
	}
	
}


