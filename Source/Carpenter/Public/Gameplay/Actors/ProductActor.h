// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GenericStructs.h"
#include "GameFramework/Actor.h"
#include "Gameplay/Interfaces/GrabbableInterface.h"
#include "Gameplay/Interfaces/InteractableInterface.h"
#include "ProductActor.generated.h"



UCLASS()
class CARPENTER_API AProductActor : public AActor, public IGrabbableInterface, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProductActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	class UBoxComponent* RootBoxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComponent;
	

	/// There are for client side smoothing STARTs
	UPROPERTY(Replicated)
	FVector TargetLocation;

	UPROPERTY(Replicated)
	FRotator TargetRotation;

	void ReplicateMovementWithSmoothing(float DeltaTime);
	/// There are for client side smoothing ENDs


	UPROPERTY(ReplicatedUsing=OnRep_DataChanged)
	FReplicatedProductData Data;
	
	UFUNCTION()
	void OnRep_DataChanged(FReplicatedProductData NewData);
	
	void UpdateVisual();
	
	void SetProductData(FReplicatedProductData InData);

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	

	void SetColor(FLinearColor NewColor);
	void Paint(FLinearColor NewColor);

	UPROPERTY(EditAnywhere)
	FName GrabSocketName;
	virtual FVector GetGrabPoint() override;

	UPROPERTY()
	ACarpenterCharacter* GrabbedBy = nullptr;
	bool bIsGrabbed = false;

	virtual void InteractStart(class ACarpenterCharacter* CarpenterCharacter) override;
	virtual void InteractEnd(class ACarpenterCharacter* CarpenterCharacter) override;

};
