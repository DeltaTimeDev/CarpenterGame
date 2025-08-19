// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Machines/ShapingMachine.h"

#include "Components/WidgetComponent.h"
#include "Framework/CarpenterPlayerController.h"
#include "Gameplay/Actors/ProductActor.h"
#include "Gameplay/Libraries/ProductFunctionLibrary.h"
#include "Gameplay/Subsystems/MoneySubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Widgets/ProductCatalogWidget.h"
#include "Widgets/CarpenterInteractableWidgetComp.h"

// Sets default values
AShapingMachine::AShapingMachine()
{
	PrimaryActorTick.bCanEverTick = false;


	WidgetHolderMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WidgetHolderMeshComponent"));
	WidgetHolderMeshComponent->SetupAttachment(RootComponent);

	ProductCatalogWidgetComponent = CreateDefaultSubobject<UCarpenterInteractableWidgetComp>(TEXT("ProductCatalogWidgetComponent"));
	ProductCatalogWidgetComponent->SetupAttachment(WidgetHolderMeshComponent, "WidgetSocket");
}

void AShapingMachine::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		LocalCarpenterPlayerController = Cast<ACarpenterPlayerController>(PlayerController);
	}


	ProductCatalogWidgetInstance = Cast<UProductCatalogWidget>(ProductCatalogWidgetComponent->GetUserWidgetObject());
	if (ProductCatalogWidgetInstance)
	{
		ProductCatalogWidgetInstance->OnRequestForChangeSelection.AddDynamic(this, &AShapingMachine::OnRequestForChangeSelection);
		ProductCatalogWidgetInstance->OnCreateCommand.AddDynamic(this, &AShapingMachine::OnCreateProductCommand);
		

		ProductCatalogWidgetInstance->SetProductList(UProductFunctionLibrary::GetProductDataAsset(this)->Products);

	}
}

void AShapingMachine::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShapingMachine, CurrentSelectedProductID);
}

void AShapingMachine::SpawnProduct()
{
	FReplicatedProductData CurrentSelectedProductData = UProductFunctionLibrary::GetProductReplicatedData(this,CurrentSelectedProductID);

	if (MachineMeshComponent->DoesSocketExist(SpawnSocketName))
	{
		const FTransform SpawnTransform = MachineMeshComponent->GetSocketTransform(SpawnSocketName);

		if (AProductActor* ProductActor = GetWorld()->SpawnActorDeferred<AProductActor>(
			ProductClass,
			SpawnTransform,
			nullptr, // Owner
			nullptr, // Instigator
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn
		))
		{
			ProductActor->SetProductData(CurrentSelectedProductData);
			UGameplayStatics::FinishSpawningActor(ProductActor, SpawnTransform);
		}

		if (UMoneySubsystem* MoneySubsystem = Cast<UMoneySubsystem>(GetGameInstance()->GetSubsystem<UMoneySubsystem>()))
		{
			float Cost = UProductFunctionLibrary::GetCostOfProduct(this,CurrentSelectedProductID);
			MoneySubsystem->Spend(Cost);
		}

		MulticastShapingEffect();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No product selected or socket not found."));
	}
}

void AShapingMachine::OnRequestForChangeSelection(UProduct* SelectedProduct)
{
	if (IsValid(LocalCarpenterPlayerController))
	{
		LocalCarpenterPlayerController->ServerSetProductShapingMachine(this, SelectedProduct->Data.TypeID);
	}
}

void AShapingMachine::OnSelectedProductIdChanged(FGuid NewCurrentSelectedProductId)
{
	ProductCatalogWidgetInstance->SelectProductByID(CurrentSelectedProductID);
}

void AShapingMachine::SetSelectedProductOnServer(FGuid SelectedProductID)
{
	CurrentSelectedProductID = SelectedProductID;
	OnSelectedProductIdChanged(SelectedProductID);
}

void AShapingMachine::MulticastShapingEffect_Implementation()
{
	ShapingEffect();
}


void AShapingMachine::OnCreateProductCommand(UProduct* CreateProduct)
{
	if (IsValid(LocalCarpenterPlayerController))
	{
		LocalCarpenterPlayerController->ServerSpawnProduct(this);
	}
}
