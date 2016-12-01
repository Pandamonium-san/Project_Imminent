// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "InteractLight.h"
#include "Monster.generated.h"

UCLASS()
class PROJECT_IMMINENT_API AMonster : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AMonster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CollisionBox)
	USphereComponent *PlayerKillSphere;

	UFUNCTION()
	void OnOverlapBeginLight(class UPrimitiveComponent* OverlappingComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CollisionBox)
	USphereComponent *LightKillSphere;

	float WalkSpeed;
	float FlySpeed;

	
};
