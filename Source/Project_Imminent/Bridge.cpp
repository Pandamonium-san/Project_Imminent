// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "Bridge.h"


// Sets default values
ABridge::ABridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	RotatorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatorMesh"));
	RotatorMesh->SetupAttachment(RootComponent);

	BridgeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BridgeMesh"));
	BridgeMesh->SetupAttachment(RotatorMesh);

	CurrentRotation = GetActorRotation();
	RotationSpeed = 10.0f;
	degrees = 90.0f;
	counter = 0.0f;
	ResetDelay = 7.0f;
	delay = ResetDelay;
	bReset = false;
	bActive = false;
	bShouldReset = false;

}

// Called when the game starts or when spawned
void ABridge::BeginPlay()
{
	Super::BeginPlay();
	delay = ResetDelay;
	bActive = true;
}

// Called every frame
void ABridge::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (bActive)
	{
		if (!bReset)
			RotateBridge(DeltaTime);
		else
		{
			if (delay >= 0)			
				delay -= DeltaTime;
			else
				if (bShouldReset)
					ResetBridge(DeltaTime);
		}
	}
	

}
void ABridge::ActivateBridge()
{
	bActive = true;
}

void ABridge::RotateBridge(float DeltaTime)
{
	FRotator NewRotation = GetActorRotation();


	if (counter <= degrees)
	{
		NewRotation -= FRotator(0.0f, 1 * RotationSpeed * DeltaTime, 0.0f);
		counter += 1 * RotationSpeed * DeltaTime;
		SetActorRotation(NewRotation);
		CurrentRotation = NewRotation;
	}
	else
		if (bShouldReset)
			bReset = true;
	
	

}
void ABridge::ResetBridge(float DeltaTime)
{
	FRotator NewRotation = GetActorRotation();

	if (counter >= 0)
	{
		NewRotation += FRotator(0.0f, 1 * RotationSpeed * DeltaTime, 0.0f);
		counter -= 1 * RotationSpeed * DeltaTime;
		SetActorRotation(NewRotation);
		CurrentRotation = NewRotation;
		if (!bShouldReset)
		{
			delay = ResetDelay;
			bActive = false;
		}

	}
	else
	{
		bReset = false;
		delay = ResetDelay;
		bActive = false;
	}

}
