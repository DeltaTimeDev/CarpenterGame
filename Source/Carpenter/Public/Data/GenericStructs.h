// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GenericStructs.generated.h"



USTRUCT(BlueprintType)
struct CARPENTER_API FProductData
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Product")
	FGuid TypeID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	FString Name = "None";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	UTexture2D* Image = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	float RawMaterialCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	float Price = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	UStaticMesh* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	FLinearColor Color = FLinearColor::White;

	FProductData() 
		: TypeID(FGuid::NewGuid())
	{}
};

USTRUCT(BlueprintType)
struct FReplicatedProductData
{
	GENERATED_BODY()

	FReplicatedProductData(){}

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Product")
	FGuid TypeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(VisibleAnywhere)
	UStaticMesh* Mesh;
};

UCLASS(Blueprintable)
class CARPENTER_API UProduct : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Product")
	FProductData Data;
};

UCLASS(BlueprintType)
class CARPENTER_API UProductDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	TArray<FProductData> Products;
};


UCLASS()
class CARPENTER_API UGenericStructs : public UObject
{
	GENERATED_BODY()
};
