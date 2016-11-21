// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "Monster.h"
#include "Project_ImminentCharacter.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightKillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LightsOutSphere"));
	PlayerKillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PlayerKillSphere"));
	PlayerKillSphere->SetupAttachment(GetCapsuleComponent());
	PlayerKillSphere->InitSphereRadius(400);
	PlayerKillSphere->bGenerateOverlapEvents = true;


	LightKillSphere->InitSphereRadius(400);
	LightKillSphere->SetupAttachment(GetCapsuleComponent());
	LightKillSphere->bGenerateOverlapEvents = true;

	LightKillSphere->OnComponentBeginOverlap.AddDynamic(this, &AMonster::OnOverlapBeginLight);


	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	FlySpeed = GetCharacterMovement()->MaxFlySpeed;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

void AMonster::OnOverlapBeginLight(class UPrimitiveComponent* OverlappingComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AInteractLight>(OtherActor))
	{
		AInteractLight *interactLight = Cast<AInteractLight>(OtherActor);
		interactLight->light->SetIntensity(0);
	}
}