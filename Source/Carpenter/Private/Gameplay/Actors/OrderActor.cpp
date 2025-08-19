// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Actors/OrderActor.h"

#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "Gameplay/Actors/ProductActor.h"
#include "Gameplay/Libraries/ProductFunctionLibrary.h"
#include "Gameplay/Subsystems/MoneySubsystem.h"
#include "Net/UnrealNetwork.h"

class AProductActor;
// Sets default values
AOrderActor::AOrderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;
	
	AcceptingArea = CreateDefaultSubobject<UBoxComponent>(TEXT("PaintingArea"));
	AcceptingArea->SetupAttachment(RootComponent);
	AcceptingArea->SetHiddenInGame(false);
	

	TextRenderComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComponent"));
	TextRenderComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AOrderActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AcceptingArea->OnComponentBeginOverlap.AddDynamic(this, &AOrderActor::OnOverlapBegin);
	}
	UpdateVisual();
}

void AOrderActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOrderActor, Order)
}

// Called every frame
void AOrderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AOrderActor::SetOrder(FOrder InOrder)
{
	Order = InOrder;
	UpdateVisual();
}

int32 AOrderActor::GetEarnAmount(FReplicatedProductData Data)
{
	float OriginalPrice = UProductFunctionLibrary::GetPriceOfProduct(this,Data.TypeID);
	
	if (Data.TypeID == Order.TypeID)
	{
		if (Data.Color == Order.Color)
		{
			return OriginalPrice;
		}
		else
		{
			return OriginalPrice/2;
		}
	}
	else
	{
		return 0;		
	}
}

void AOrderActor::OnOrderChanged(FOrder NewOrder)
{
	UpdateVisual();
}

void AOrderActor::Activate(bool Value)
{
	TextRenderComponent->SetVisibility(Value);
	AcceptingArea->SetVisibility(Value);
}

void AOrderActor::UpdateVisual()
{
	Activate(Order.IsActive);
	FString Text = ColorNameMap[Order.Color];
	Text.Append(" ").Append(Order.Name);
	TextRenderComponent->SetText(FText::FromString(Text));
}

void AOrderActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UMoneySubsystem* MoneySubsystem = Cast<UMoneySubsystem>(GetGameInstance()->GetSubsystem<UMoneySubsystem>()))
	{
		if (AProductActor* ProductActor = Cast<AProductActor>(OtherActor))
		{
			FReplicatedProductData Data = ProductActor->Data;

			int32 EarnAmount = GetEarnAmount(Data);
			MoneySubsystem->Earn(EarnAmount);
			MulticastMoneyEffect(EarnAmount);

			ProductActor->Destroy();
			Order.IsActive = false;
			SetOrder(Order);
		}
	}
}

void AOrderActor::MulticastMoneyEffect_Implementation(int32 MoneyCount)
{
	MoneyEffect(MoneyCount);
}

