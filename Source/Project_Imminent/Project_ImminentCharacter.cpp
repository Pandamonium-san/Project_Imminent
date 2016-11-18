// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Project_Imminent.h"
#include "Project_ImminentCharacter.h"
//#include "Project_ImminentProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Runtime/Engine/Classes/PhysicsEngine/PhysicsConstraintComponent.h"
#include "MotionControllerComponent.h"
#include "TriggerComponent.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "Checkpoint.h"
#include "Monster_Checkpoint.h"

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
  FirstPersonCameraComponent->RelativeLocation = FVector(50.0f, 1.75f, 64.f); // Position the camera
  FirstPersonCameraComponent->bUsePawnControlRotation = true;

  ArmMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArmMesh"));
  ArmMesh->SetupAttachment(GetCapsuleComponent());
  // ArmMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

  HandleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HandleMesh"));
  HandleMesh->SetupAttachment(GetCapsuleComponent());
  //HandleMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

  LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanternMesh"));
  LanternMesh->SetupAttachment(GetCapsuleComponent());
  // LanternMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
  LanternMesh->SetSimulatePhysics(true);

  CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
  CollisionComponent->SetupAttachment(GetCapsuleComponent());
  CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AProject_ImminentCharacter::OnOverlapBegin);

  NewLightColor.R = 230.0f;
  NewLightColor.G = 179.0f;
  NewLightColor.B = 111.0f;

  MaxIntensity = 5000.0f;
  IntensityConsumptionRate = 100.0f;
  Intensity = MaxIntensity;

  LightSource = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightSource"));
  LightSource->AttachToComponent(LanternMesh, FAttachmentTransformRules::KeepRelativeTransform);
  LightSource->SetLightColor(NewLightColor);

  ForwardSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("ForwardSpotLight"));
  ForwardSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
  ForwardSpotLight->SetLightColor(NewLightColor);
  SpotLightArray.Add(ForwardSpotLight);
  /*
    LeftSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("LeftSpotLight"));
    LeftSpotLight->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    LeftSpotLight->SetLightColor(NewLightColor);
    LeftSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
    SpotLightArray.Add(LeftSpotLight);*/

  RightSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("RightSpotLight"));
  RightSpotLight->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
  RightSpotLight->SetLightColor(NewLightColor);
  RightSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
  SpotLightArray.Add(RightSpotLight);

  BackSpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("BackSpotLight"));
  BackSpotLight->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
  BackSpotLight->SetLightColor(NewLightColor);
  BackSpotLight->AttachToComponent(LightSource, FAttachmentTransformRules::KeepRelativeTransform);
  SpotLightArray.Add(BackSpotLight);

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
  //WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
  //RunSpeed = WalkSpeed * RunSpeedFactor;
  bExhausted = false;

  CurrentCheckpoint = "none";
  // Uncomment the following line to turn motion controllers on by default:
  //bUsingMotionControllers = true;

  static ConstructorHelpers::FObjectFinder<UClass> MonsterFinder(TEXT("Blueprint'/Game/ImminentCPP/Blueprints/AI/Monster_BP.Monster_BP_C'"));
  Monster = MonsterFinder.Object;

}

void AProject_ImminentCharacter::BeginPlay()
{
  // Call the base class  
  Super::BeginPlay();
  WalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
  RunSpeed = WalkSpeed * RunSpeedFactor;
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
  PlayerInputComponent->BindAction("ChargeLantern", IE_Pressed, this, &AProject_ImminentCharacter::RespawnAtCheckpoint);
  PlayerInputComponent->BindAction("ChargeLantern", IE_Released, this, &AProject_ImminentCharacter::StopRechargeLantern);

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

  if (LightSource->Intensity >= 1.0f)
  {
    Intensity -= IntensityConsumptionRate * DeltaTime;
    for (int32 i = 0; i < SpotLightArray.Num(); i++)
      SpotLightArray[i]->SetIntensity(Intensity);

    LightSource->SetIntensity(Intensity);
  }

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
    if (PhysicsHandle->GrabbedBoneName == NAME_None)
    {

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
}


void AProject_ImminentCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappingComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACheckpoint>(OtherActor))
	{
		ACheckpoint* cp = Cast<ACheckpoint>(OtherActor);
		bool newCheckpoint = true;
		for (int i = 0; i < CheckpointArray.Num(); i++)
		{
			if (cp->id == CheckpointArray[i])
				newCheckpoint = false;
		}
		if (newCheckpoint)
		{
			
			CurrentCheckpoint = cp->id;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, CurrentCheckpoint);
			CheckpointArray.Add(cp->id);
		}		
	}
}

void AProject_ImminentCharacter::RespawnAtCheckpoint()
{
	UWorld* World = GetWorld();
	if (World)
	{
		for (TActorIterator<ACheckpoint> ActorItr(World); ActorItr; ++ActorItr)
		{
			if (ActorItr->id == CurrentCheckpoint)
			{
				FVector NewLocation = ActorItr->GetActorLocation();
				SetActorLocation(NewLocation);
					for (TActorIterator<AMonster_Checkpoint> MonsterItr(World); MonsterItr; ++MonsterItr)
					{			
						if (MonsterItr->id == CurrentCheckpoint)
						{
							FActorSpawnParameters SpawnParams;
							SpawnParams.Owner = this;
							SpawnParams.Instigator = Instigator;

							if (World)
								AMonster* m = World->SpawnActor<AMonster>(Monster, MonsterItr->GetActorLocation(), MonsterItr->GetActorRotation(), SpawnParams);			
							break;
						}
					}
					break;
			}
		}
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

void AProject_ImminentCharacter::StopRechargeLantern()
{

}

void AProject_ImminentCharacter::RechargeLantern()
{
  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Intensity reset"));
  Intensity = MaxIntensity;
  LightSource->SetIntensity(MaxIntensity);

  //for (int32 i = 0; i < SpotLightArray.Num()-1; i++)
  //	SpotLightArray[i]->SetIntensity(MaxIntensity);
  //for (TObjectIterator<AProject_ImminentLantern> Itr; Itr; ++Itr)
  //{
  //	// Access the subclass instance with the * or -> operators.
  //	AProject_ImminentLantern *Component = *Itr;
  //	Component->ResetIntensity();
  //	break;
  //}
}

void AProject_ImminentCharacter::MoveForward(float Value)
{
  if (Value != 0.0f)
  {
    // add movement in that direction
    if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
      AddMovementInput(GetActorForwardVector(), Value);
    else if (GetCharacterMovement()->MovementMode == (EMovementMode::MOVE_Flying | EMovementMode::MOVE_Swimming))
      AddMovementInput(FirstPersonCameraComponent->GetForwardVector(), Value);
  }
}

void AProject_ImminentCharacter::MoveRight(float Value)
{
  if (Value != 0.0f)
  {
    // add movement in that direction
    if(GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Walking)
        AddMovementInput(GetActorRightVector(), Value);
    else if (GetCharacterMovement()->MovementMode == (EMovementMode::MOVE_Flying | EMovementMode::MOVE_Swimming))
      AddMovementInput(FirstPersonCameraComponent->GetRightVector(), Value);
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
  if (!HitResult.Component->IsSimulatingPhysics(HitResult.BoneName) && !HitResult.Component->IsSimulatingPhysics())
    return;
  if (HitResult.Component->GetMass() > MaxGrabMass)
  {
#ifdef UE_BUILD_DEBUG
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Too heavy");
#endif
    return;
  }
#ifdef UE_BUILD_DEBUG
  GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Grab");
#endif
  PhysicsHandle->InterpolationSpeed = InterpolationSpeed;
  if (HitResult.BoneName == NAME_None)
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
