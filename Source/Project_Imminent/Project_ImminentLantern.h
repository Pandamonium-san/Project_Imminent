// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Project_ImminentLantern.generated.h"

UCLASS()
class PROJECT_IMMINENT_API AProject_ImminentLantern : public AActor
{
	GENERATED_BODY()
private:
	float Intensity;
	TArray<USpotLightComponent*> SpotLightArray;

public:	
	// Sets default values for this actor's properties
	AProject_ImminentLantern();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	float MaxIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	float IntensityConsumptionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* LanternMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* HandleMeshWithSocket;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	USpotLightComponent* ForwardSpotLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	USpotLightComponent* RightSpotLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	USpotLightComponent* LeftSpotLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	USpotLightComponent* BackSpotLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	UPointLightComponent* LightSource;
	
	USceneComponent* RootComponent;

	UFUNCTION()
	void ResetIntensity();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	
	
};
