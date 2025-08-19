// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GenericStructs.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProductFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CARPENTER_API UProductFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static class UProductSubsystem* GetProductSubsystem(UObject* Context);
	static FProductData GetProductData(UObject* Context, FGuid ProductTypeID);
	static FReplicatedProductData GetProductReplicatedData(UObject* Context, FGuid ProductTypeID);
	static float GetPriceOfProduct(UObject* Context, FGuid ProductTypeID);
	static float GetCostOfProduct(UObject* Context, FGuid ProductTypeID);
	static FProductData GetRandomProduct(UObject* Context);
	static class UProductDataAsset* GetProductDataAsset(UObject* Context);

};
