// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "VerticalDoor.generated.h"

UCLASS()
class PROJECT_IMMINENT_API AVerticalDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVerticalDoor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
		UPointLightComponent* PointLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MovementZLength;

	bool bIsActive;
	USceneComponent* RootComponent;
	FVector CurrentPosition;
	float DeltaPos;

	void ActivateDoor();
	void RaiseDoor(float DeltaTime);
};
