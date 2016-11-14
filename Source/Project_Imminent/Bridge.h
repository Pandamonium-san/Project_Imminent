// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Bridge.generated.h"

UCLASS()
class PROJECT_IMMINENT_API ABridge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABridge();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* BridgeMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* RotatorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
	float RotationSpeed;

	FRotator CurrentRotation;
	float counter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
	float degrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
	float ResetDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
	bool bActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rotation)
	bool bShouldReset;

	USceneComponent* RootComponent;

	UFUNCTION(BlueprintCallable, Category = Bridge)
	void ActivateBridge();

	
	void RotateBridge(float DeltaTime);

	void ResetBridge(float DeltaTime);

	bool bReset;
	float delay;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
