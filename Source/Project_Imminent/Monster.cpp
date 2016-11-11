// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "Monster.h"


// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	lightKillSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LightsOutSphere"));
	lightKillSphere->InitSphereRadius(400);
	lightKillSphere->SetupAttachment(RootComponent);

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

