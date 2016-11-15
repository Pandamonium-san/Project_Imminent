// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "InteractLight.generated.h"

UCLASS()
class PROJECT_IMMINENT_API AInteractLight : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractLight();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
		UPointLightComponent* light;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CollisionBox)
		USphereComponent* hitbox;

};
