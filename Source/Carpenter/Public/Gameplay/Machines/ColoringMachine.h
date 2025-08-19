// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MachineBase.h"
#include "ColoringMachine.generated.h"



UCLASS()
class CARPENTER_API AColoringMachine : public AMachineBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AColoringMachine();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	UFUNCTION()
	void OnPaintCanSelected(FLinearColor Color);

	void PaintProductInArea(FLinearColor Color);

	UFUNCTION(NetMulticast,Reliable)
	void MulticastPaintEffect(FLinearColor Color);

	UFUNCTION(BlueprintImplementableEvent)
	void PaintEffect(FLinearColor Color);

	UPROPERTY(EditAnywhere)
	TArray<class AProductActor*> OverlappingProducts;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category="Components")
	class UBoxComponent* PaintingArea;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UPaintCan* RedPaintCan;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UPaintCan* GreenPaintCan;

	UPROPERTY()
	class ACarpenterPlayerController* MarangozPlayerController;

};
