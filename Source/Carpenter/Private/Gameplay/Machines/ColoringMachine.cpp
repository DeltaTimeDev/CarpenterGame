// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Machines/ColoringMachine.h"

#include "Components/BoxComponent.h"
#include "Framework/CarpenterPlayerController.h"
#include "Gameplay/Actors/PaintCan.h"
#include "Gameplay/Actors/ProductActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AColoringMachine::AColoringMachine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PaintingArea = CreateDefaultSubobject<UBoxComponent>(TEXT("PaintingArea"));
	PaintingArea->SetupAttachment(RootComponent);
	PaintingArea->SetHiddenInGame(false);

	RedPaintCan = CreateDefaultSubobject<UPaintCan>(TEXT("RedPaintCan"));
	RedPaintCan->SetupAttachment(RootComponent);	

	GreenPaintCan = CreateDefaultSubobject<UPaintCan>(TEXT("GreenPaintCan"));
	GreenPaintCan->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AColoringMachine::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		MarangozPlayerController = Cast<ACarpenterPlayerController>(PlayerController);
	}
	
	RedPaintCan->SetColor(FLinearColor(1,0,0));
	GreenPaintCan->SetColor(FLinearColor(0,1,0));

	RedPaintCan->OnPaintCanSelected.AddDynamic(this, &AColoringMachine::OnPaintCanSelected);
	GreenPaintCan->OnPaintCanSelected.AddDynamic(this, &AColoringMachine::OnPaintCanSelected);	
}

void AColoringMachine::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	RedPaintCan->OnPaintCanSelected.RemoveDynamic(this, &AColoringMachine::OnPaintCanSelected);
	GreenPaintCan->OnPaintCanSelected.RemoveDynamic(this, &AColoringMachine::OnPaintCanSelected);
}

void AColoringMachine::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RedPaintCan->SetColor(FLinearColor(1,0,0));
	GreenPaintCan->SetColor(FLinearColor(0,1,0));
}

void AColoringMachine::OnPaintCanSelected(FLinearColor Color)
{
	if (HasAuthority())
	{
		PaintProductInArea(Color);

		MulticastPaintEffect(Color);
	}
}

void AColoringMachine::PaintProductInArea(FLinearColor Color)
{
	TArray<AActor*> OverlappingActors;
	PaintingArea->GetOverlappingActors(OverlappingActors);

	for (AActor* OverlappingActor : OverlappingActors)
	{
		if (AProductActor* Product = Cast<AProductActor>(OverlappingActor))
		{
			Product->Paint(Color);
		}
	}
}

void AColoringMachine::MulticastPaintEffect_Implementation(FLinearColor Color)
{
	PaintEffect(Color);
}


