// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TriggerComponent.h"
#include "GameFramework/Actor.h"
#include "Switch.generated.h"

UCLASS()
class PROJECT_IMMINENT_API ASwitch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitch();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	USceneComponent* RootComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Avaiability)
	bool bEnabled;
		
};
