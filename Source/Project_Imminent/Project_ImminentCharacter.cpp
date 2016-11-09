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
#include "Project_ImminentLantern.h"
#include "EngineUtils.h"

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

  ArmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmMesh"));
  ArmMesh->SetupAttachment(GetCapsuleComponent());

  HandleMeshWithSocket = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshWithSocketForLantern"));
  HandleMeshWithSocket->AttachToComponent(ArmMesh, FAttachmentTransformRules::KeepRelativeTransform);


  // Create VR Controllers.
  R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
  R_MotionController->Hand = EControllerHand::Right;
  R_MotionController->SetupAttachment(RootComponent);
  L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
  L_MotionController->SetupAttachment(RootComponent);

  PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("GrabHandle"));

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
  PlayerInputComponent->BindAction("Interact", IE_Released, this, &AProject_ImminentCharacter::Release);
  PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AProject_ImminentCharacter::Run);
  PlayerInputComponent->BindAction("Run", IE_Released, this, &AProject_ImminentCharacter::StopRun);
  PlayerInputComponent->BindAction("ChargeLantern", IE_Pressed, this, &AProject_ImminentCharacter::RechargeLantern);

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

  PlayerInputComponent->BindAxis("MoveItemAway", this, &AProject_ImminentCharacter::MoveItemAway);
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

  //Checks if stamina has been recovered to the exhaustion limit and if so removes exhaustion.
  if (bExhausted)
  {
    if (Stamina >= ExhaustionLimit)
      bExhausted = false;
  }

  DoLineTrace();

  // Sets grabbed component location
  if (PhysicsHandle->GrabbedComponent != NULL)
  {
    float distanceFromTarget;
    FVector targetLocation = FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * ItemDistance;
    PhysicsHandle->SetTargetLocation(targetLocation);
    if (PhysicsHandle->GrabbedBoneName == NAME_None)
    {
      FRotator targetRotation = itemInitRot + FRotator(0, FirstPersonCameraComponent->GetComponentRotation().Yaw - pawnInitRot.Yaw, 0);
      PhysicsHandle->GrabbedComponent->SetWorldRotation(targetRotation);
      distanceFromTarget = FVector::Dist(targetLocation, PhysicsHandle->GrabbedComponent->GetComponentLocation());
    }
    else
    {
      distanceFromTarget = FVector::Dist(targetLocation, PhysicsHandle->GrabbedComponent->GetBodyInstance(PhysicsHandle->GrabbedBoneName)->GetCOMPosition());
    }
    if (distanceFromTarget > MaxHoldDistance)
      Release();

    // Line Trace to prevent player from standing on held object
    FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Interact_Trace")), true, this);
    TraceParams.bTraceComplex = true;
    TraceParams.bTraceAsyncScene = true;
    TraceParams.bReturnPhysicalMaterial = false;
    TraceParams.AddIgnoredActor(this);

    FHitResult Hit(ForceInit);

    FVector Start = FirstPersonCameraComponent->GetComponentLocation();
    FVector End = Start + FVector(0, 0, -InteractRange - 100);
    GetWorld()->LineTraceSingleByChannel(
      Hit,
      Start,
      End,
      ECC_Visibility,
      TraceParams
    );
    if (Hit.GetComponent() == PhysicsHandle->GrabbedComponent)
      Release();
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 0.1f, 0, 1);
    Start = GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, -100.0f) - GetCapsuleComponent()->GetForwardVector() * 60;
    End = Start + GetCapsuleComponent()->GetForwardVector() * 120;
    GetWorld()->LineTraceSingleByChannel(
      Hit,
      Start,
      End,
      ECC_Visibility,
      TraceParams
    );
    if (Hit.GetComponent() == PhysicsHandle->GrabbedComponent)
      Release();
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 0.1f, 0, 1);

    Start = GetCapsuleComponent()->GetComponentLocation() + FVector(0, 0, -100.0f) - GetCapsuleComponent()->GetRightVector() * 60;
    End = Start + GetCapsuleComponent()->GetRightVector() * 120;
    GetWorld()->LineTraceSingleByChannel(
      Hit,
      Start,
      End,
      ECC_Visibility,
      TraceParams
    );
    if (Hit.GetComponent() == PhysicsHandle->GrabbedComponent)
      Release();
    DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, 0.1f, 0, 1);
  }
}

void AProject_ImminentCharacter::DoLineTrace()
{
  FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Interact_Trace")), true, this);
  TraceParams.bTraceComplex = true;
  TraceParams.bTraceAsyncScene = true;
  TraceParams.bReturnPhysicalMaterial = false;
  TraceParams.AddIgnoredActor(this);

  FVector Start = FirstPersonCameraComponent->GetComponentLocation();
  FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * InteractRange;
  GetWorld()->LineTraceSingleByChannel(
    this->HitResult,
    Start,
    End,
    ECC_Visibility,
    TraceParams
  );
}
void AProject_ImminentCharacter::OnResetVR()
{
  UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AProject_ImminentCharacter::RechargeLantern()
{
  for (TObjectIterator<AProject_ImminentLantern> Itr; Itr; ++Itr)
  {
    // Access the subclass instance with the * or -> operators.
    AProject_ImminentLantern *Component = *Itr;
    Component->ResetIntensity();
  }
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
  if (HitResult.Actor == NULL)
    return;

  // Check if trigger
  TArray<UTriggerComponent*> comps;
  HitResult.Actor->GetComponents(comps);
  for (int i = 0; i < comps.Num(); ++i)
  {
    if (comps[i]->UserInteractable)
      comps[i]->TriggerEvent();
  }

  // Check if physics
  if (!(HitResult.Component->IsSimulatingPhysics(HitResult.BoneName) || HitResult.Component->IsSimulatingPhysics()) ||
    HitResult.Component->GetMass() > MaxGrabMass)
    return;
#ifdef UE_BUILD_DEBUG
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Grab");
#endif
  PhysicsHandle->InterpolationSpeed = InterpolationSpeed;
  if(HitResult.BoneName == NAME_None)
    PhysicsHandle->GrabComponent(HitResult.GetComponent(), HitResult.BoneName, HitResult.GetComponent()->GetComponentLocation(), false);
  else
    PhysicsHandle->GrabComponent(HitResult.GetComponent(), HitResult.BoneName, HitResult.Location + (HitResult.TraceEnd - HitResult.Location) / 2, false);
  GrabbedItem = HitResult.GetComponent();
  pawnInitRot = FirstPersonCameraComponent->GetComponentRotation();
  itemInitRot = HitResult.Component->GetComponentRotation();
  itemInitAngDamp = HitResult.Component->GetAngularDamping();
  ItemDistance = InitItemDistance;
  //HitResult.Component->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
  HitResult.Component->SetAngularDamping(10);
}

void AProject_ImminentCharacter::MoveItemAway(float Val)
{
  if (Val != 0.0f)
  {
#ifdef UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Changed item distance");
#endif
    ItemDistance += 10 * Val;
    ItemDistance = FMath::Clamp(ItemDistance, MinItemDistance, MaxItemDistance);
  }
}

void AProject_ImminentCharacter::Release()
{
  if (PhysicsHandle->GrabbedComponent != NULL)
  {
    //PhysicsHandle->GrabbedComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    PhysicsHandle->GrabbedComponent->SetAngularDamping(itemInitAngDamp);
    PhysicsHandle->GrabbedComponent->WakeRigidBody();
    GrabbedItem = NULL;
    PhysicsHandle->ReleaseComponent();
#ifdef UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Release");
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
