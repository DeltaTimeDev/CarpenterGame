// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MachineBase.h"
#include "ShapingMachine.generated.h"


UCLASS()
class CARPENTER_API AShapingMachine : public AMachineBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShapingMachine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UFUNCTION(BlueprintCallable, Category="Product")
	void SpawnProduct();

	UFUNCTION()
	void OnRequestForChangeSelection(UProduct* SelectedProduct);

	UFUNCTION()
	void OnCreateProductCommand(UProduct* CreateProduct);

	UFUNCTION()
	void OnSelectedProductIdChanged(FGuid NewCurrentSelectedProductId);

	UFUNCTION()
	void SetSelectedProductOnServer(FGuid SelectedProductID);

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProductActor> ProductClass;

private:

	 UPROPERTY(EditAnywhere, Category="Components")
	 UStaticMeshComponent* WidgetHolderMeshComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	class UCarpenterInteractableWidgetComp* ProductCatalogWidgetComponent;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<class UProductCatalogWidget> ProductCatalogWidgetClass;

	FName SpawnSocketName = TEXT("SpawnSocket"); 

	UPROPERTY()
	UProductCatalogWidget* ProductCatalogWidgetInstance;

	UPROPERTY(ReplicatedUsing= OnSelectedProductIdChanged, VisibleAnywhere)
	FGuid CurrentSelectedProductID;

	UPROPERTY()
	class ACarpenterPlayerController* LocalCarpenterPlayerController;

	UFUNCTION(NetMulticast,Reliable)
	void MulticastShapingEffect();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShapingEffect();
};
