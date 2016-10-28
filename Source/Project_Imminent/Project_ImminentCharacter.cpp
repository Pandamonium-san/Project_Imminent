// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Project_Imminent.h"
#include "Project_ImminentCharacter.h"
//#include "Project_ImminentProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "TriggerComponent.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AProject_ImminentCharacter

AProject_ImminentCharacter::AProject_ImminentCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	HandleMeshWithSocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshWithSocketForLantern"));
	HandleMeshWithSocket->SetupAttachment(GetCapsuleComponent());

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->Hand = EControllerHand::Right;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);


	RunSpeedFactor = 1.5f;
	MaxStamina = 50.0f;
	StaminaConsumptionRate = 10.0f;
	StaminaRegenerationRate = 10.0f;
	Stamina = MaxStamina;
	ExhaustionLimit = 20.f;
	WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	RunSpeed = WalkSpeed * RunSpeedFactor;
	bExhausted = false;


	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AProject_ImminentCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}


//////////////////////////////////////////////////////////////////////////
// Input

void AProject_ImminentCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
  PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AProject_ImminentCharacter::Interact);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AProject_ImminentCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AProject_ImminentCharacter::StopRun);
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AProject_ImminentCharacter::OnResetVR);

	PlayerInputComponent->BindAxis("MoveForward", this, &AProject_ImminentCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AProject_ImminentCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AProject_ImminentCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AProject_ImminentCharacter::LookUpAtRate);
}

void AProject_ImminentCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Check if stamina should be consumed. 
	if (bRunning && !bExhausted)
		Stamina -= StaminaConsumptionRate * DeltaTime;
	//Checks if stamina should be regenerated. 
	if (!bRunning && Stamina < MaxStamina)
		Stamina += StaminaRegenerationRate * DeltaTime;	

	//Checks if stamina has been exhausted and stops the player from running
	if (Stamina <= 1.0f)
	{
		bExhausted = true;
		StopRun();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Exhausted!"));
	}

	//Checks if stamina has been recovered to the exhaustionlimit and if so removes exhaustion.
	if (bExhausted)
	{
		if (Stamina >= ExhaustionLimit)
			bExhausted = false;
	}
}

void AProject_ImminentCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}


void AProject_ImminentCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AProject_ImminentCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AProject_ImminentCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AProject_ImminentCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AProject_ImminentCharacter::Interact()
{
  FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Interact_Trace")), true, this);
  TraceParams.bTraceComplex = true;
  TraceParams.bTraceAsyncScene = true;
  TraceParams.bReturnPhysicalMaterial = false;
  TraceParams.AddIgnoredActor(this);

  //Re-initialize hit info
  FHitResult Hit(ForceInit);

  FVector Start = FirstPersonCameraComponent->GetComponentLocation();
  FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * InteractRange;
  //call GetWorld() from within an actor extending class
  GetWorld()->LineTraceSingleByChannel(
    Hit,        //result
    Start,    //start
    End, //end
    ECC_Visibility, //collision channel
    TraceParams
  );

#ifdef UE_BUILD_DEBUG
  DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 1, 0, 1);
#endif
  if (Hit.Actor != NULL)
  {
    TArray<UTriggerComponent*> comps;
    Hit.Actor->GetComponents(comps);
    for (int i = 0; i < comps.Num(); ++i)
    {
      UTriggerComponent* thisComp = Cast<UTriggerComponent>(comps[i]);
      if (thisComp)
      {
        thisComp->TriggerEvent();
      }
    }
#ifdef UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Hit");
#endif
  }
}

void AProject_ImminentCharacter::Run()
{
	//Checks if the player can run: Have stamina and is not exhausted, if so sets the walkspeed to runspeed.
		if (Stamina > 0 && !bExhausted)
		{
			GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
			bRunning = true;
		}		
}
void AProject_ImminentCharacter::StopRun()
{
	//Stops the character from runnin and returns the walkspeed to default.
		bRunning = false;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}
