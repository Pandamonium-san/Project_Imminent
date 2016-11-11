// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TriggerComponent.generated.h"


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_IMMINENT_API UTriggerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTriggerComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

  // Determines whether the player can activate this trigger by interacting with it.
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interact")
    bool UserInteractable;
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger")
    FString DebugText;
  // Generic trigger event. Activated if player interacts with this actor and interactable is enabled. Can be activated from another script.
  UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="TriggerComponent")
    void TriggerEvent();
  virtual void TriggerEvent_Implementation();
	
};
