// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Subsystems/ProductSubsystem.h"

#include "Data/GenericStructs.h"
#include "Data/ProjectDataSettings.h"

void UProductSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadProductDataAsset();
}

void UProductSubsystem::LoadProductDataAsset()
{
	if (!ProductDataAsset)
	{
		if (const UProjectDataSettings* ProductSettings = GetDefault<UProjectDataSettings>())
		{
			ProductDataAsset = ProductSettings->Products.LoadSynchronous();
		}
	}
}

FProductData UProductSubsystem::GetRandomProduct() const
{
	if (!ProductDataAsset || ProductDataAsset->Products.Num() == 0)
	{
		return FProductData();
	}

	const int32 RandomIndex = FMath::RandRange(0, ProductDataAsset->Products.Num() - 1);
	return ProductDataAsset->Products[RandomIndex];
}
