// Fill out your copyright notice in the Description page of Project Settings.

#include "Project_Imminent.h"
#include "Monster_Checkpoint.h"


// Sets default values
AMonster_Checkpoint::AMonster_Checkpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	id = "monster_default";
}

// Called when the game starts or when spawned
void AMonster_Checkpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonster_Checkpoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

