// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Libraries/ProductFunctionLibrary.h"

#include "Data/GenericStructs.h"
#include "Gameplay/Subsystems/ProductSubsystem.h"

class UProductSubsystem* UProductFunctionLibrary::GetProductSubsystem(UObject* Context)
{
	if (Context)
	{
		return Cast<UProductSubsystem>(Context->GetWorld()->GetGameInstance()->GetSubsystem<UProductSubsystem>());
	}
	return nullptr;
}

FProductData UProductFunctionLibrary::GetProductData(UObject* Context, FGuid ProductTypeID)
{
	for (auto Product : GetProductSubsystem(Context)->ProductDataAsset->Products)
	{
		if (Product.TypeID == ProductTypeID)
		{
			return Product;
		}
	}
	return FProductData();
}

FReplicatedProductData UProductFunctionLibrary::GetProductReplicatedData(UObject* Context, FGuid ProductTypeID)
{
	FProductData ProductData = GetProductData(Context, ProductTypeID);

	FReplicatedProductData ReplicatedProductData;
	ReplicatedProductData.TypeID = ProductData.TypeID;
	ReplicatedProductData.Color = ProductData.Color;
	ReplicatedProductData.Mesh = ProductData.Mesh;
	return ReplicatedProductData;
}

float UProductFunctionLibrary::GetPriceOfProduct(UObject* Context, FGuid ProductTypeID)
{
	return GetProductData(Context, ProductTypeID).Price;
}

float UProductFunctionLibrary::GetCostOfProduct(UObject* Context, FGuid ProductTypeID)
{
	return GetProductData(Context, ProductTypeID).RawMaterialCost;
}

FProductData UProductFunctionLibrary::GetRandomProduct(UObject* Context)
{
	return GetProductSubsystem(Context)->GetRandomProduct();
}

class UProductDataAsset* UProductFunctionLibrary::GetProductDataAsset(UObject* Context)
{
	return GetProductSubsystem(Context)->ProductDataAsset;
}
