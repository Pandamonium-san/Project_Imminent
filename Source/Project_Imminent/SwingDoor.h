// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "SwingDoor.generated.h"

UCLASS()
class PROJECT_IMMINENT_API ASwingDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwingDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* PivotPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float DegreesToRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Speed;

	USceneComponent* RootComponent;
	FRotator CurrentRotation;
	bool bEnabled;
	float DeltaRot;
	bool bIsActive;
	void OpenDoor(float DeltaTime);
	void ActivateDoor();
};
