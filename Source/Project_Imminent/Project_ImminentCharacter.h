// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Project_ImminentCharacter.generated.h"

class UInputComponent;

UCLASS(config=Game)
class AProject_ImminentCharacter : public ACharacter
{
	GENERATED_BODY()

	///** Pawn mesh: 1st person view (arms; seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	//class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

	/* WalkSpeed is derived from CharaterMovementComponent MaxWalkSpeed*/
	float WalkSpeed;

	/* Is based on WalkSpeed and RunSpeedFactor and decides how fast the player runs*/
	float RunSpeed;

	/* Stamina decides if the player can run or not*/
	float Stamina;

	/* Is set to true if the player has exhausted all of the stamina and triggers a delay before running is available again */
	bool bExhausted;

public:
	AProject_ImminentCharacter();

	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;


  /** How far away the player can interact with something. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
    float InteractRange = 300;

	/* Mesh with socket that will be used to attach the lantern*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* HandleMeshWithSocket;

	/* The factor of which the WalkSpeed will be multiplied inorder to calculate the RunSpeed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float RunSpeedFactor;

	/* The limit for the stamina to reach before the player can run again */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float ExhaustionLimit;

	/* The maximum amount of stamina */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float MaxStamina;
	
	/* The rate of which stamina is consumed while running */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float StaminaConsumptionRate;

	 /* The rate of which stamina is recovered while not running */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
	float StaminaRegenerationRate;

	/* Is set to true if the player is running */
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool bRunning;

protected:
	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/* Handles logic when player starts running */
	void Run();

	/* Handles logic whan player stops running */
	void StopRun();
	
  /** Does a line trace in camera direction and activates it if it is user interactable. */
  void Interact();
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:

	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

