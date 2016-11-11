// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HeavyCaveDoor.generated.h"

UCLASS()
class PROJECT_IMMINENT_API AHeavyCaveDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeavyCaveDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* DoorMesh; 

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	USceneComponent* RootComponent;

	UFUNCTION(BlueprintCallable, Category = "DoorOpen")
	void OpenDoor();
	
};
