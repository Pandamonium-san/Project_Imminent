// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

UCLASS()
class PROJECT_IMMINENT_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CheckpointID)
	FString id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	UBoxComponent* CheckPointHitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
	USceneComponent* MonsterSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Position)
	USceneComponent* PlayerSpawn;

	USceneComponent* RootComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
