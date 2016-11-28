// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Monster.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UMotionControllerComponent* L_MotionController;

  UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  class UPhysicsHandleComponent* PhysicsHandle;

  UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
  class UPrimitiveComponent* GrabbedItem;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
  /** Hit result for line trace. Shows what the player is looking at. */
  FHitResult HitResult;

  /** Initial values before grab. */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FRotator pawnInitRot;
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    FRotator itemInitRot;
  float itemInitAngDamp;
  
	/* WalkSpeed is derived from CharaterMovementComponent MaxWalkSpeed*/
	float WalkSpeed;

	/* Is based on WalkSpeed and RunSpeedFactor and decides how fast the player runs*/
	float RunSpeed;

	/* Stamina decides if the player can run or not*/
	float Stamina;

	/* Is set to true if the player has exhausted all of the stamina and triggers a delay before running is available again */
	bool bExhausted;

	float Intensity;
	TArray<USpotLightComponent*> SpotLightArray;
	FString CurrentCheckpoint;

	TArray<FString> CheckpointArray;



public:
	AProject_ImminentCharacter();

	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<AMonster> Monster;

	void RechargeLantern();
	void StopRechargeLantern();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	float MaxIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	float IntensityConsumptionRate;

	FLinearColor NewLightColor;



	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappingComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
		USpotLightComponent* ForwardSpotLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
		USpotLightComponent* RightSpotLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
		USpotLightComponent* LeftSpotLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
		USpotLightComponent* BackSpotLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Light)
		UPointLightComponent* LightSource;




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
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interact)
    float InteractRange = 300;

  /** How far away the item will float when the player is holding it. */
  float ItemDistance = 100;
  /** Initial distance when player grabs item. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interact)
    float InitItemDistance = 100;
  /** Min distance player can hold item. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interact)
    float MinItemDistance = 50;
  /** Max distance player can hold item. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interact)
    float MaxItemDistance = 290;
  /** How far away the item can be from the target location before the item is automatically dropped. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interact)
    float MaxHoldDistance = 300;
  /** How fast the item will move towards the target location. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interact)
  float InterpolationSpeed = 4;
  /** Grab will fail if target weighs more than this amount. */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Interact)
    float MaxGrabMass = 2500;

	/* Mesh with socket that will be used to attach the lantern*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* HandleMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ArmMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Physics, meta = (AllowPrivateAccess = "true"))
	UPhysicsConstraintComponent* PhysConstraint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LanternMesh;

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

	UFUNCTION(BlueprintCallable, Category = "Death")
	void RespawnAtCheckpoint();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light)
	bool resetGuide;

protected:
  /** Line trace from camera for interact. */
  void DoLineTrace();

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
	
  /** Does a line trace in camera direction and activates it if it is user interactable, or grabs it if it is a physics object. */
  void Interact();

  /** Changes the distance of the held item. */
  void MoveItemAway(float Val);

  /** Releases the physics handle. */
  UFUNCTION(BlueprintCallable, Category="Interact")
  void Release();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface


public:

	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

