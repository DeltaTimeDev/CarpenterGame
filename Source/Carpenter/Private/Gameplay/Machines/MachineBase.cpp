// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Machines/MachineBase.h"

// Sets default values
AMachineBase::AMachineBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	MachineMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MachineMeshComponent"));
	MachineMeshComponent->SetupAttachment(RootComponent);
}


