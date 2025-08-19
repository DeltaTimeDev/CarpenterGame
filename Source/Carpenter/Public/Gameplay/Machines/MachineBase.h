// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MachineBase.generated.h"

UCLASS()
class CARPENTER_API AMachineBase : public AActor
{
	GENERATED_BODY()

protected:	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Machine, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootComp;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Machine, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MachineMeshComponent;
	
public:	
	// Sets default values for this actor's properties
	AMachineBase();


};
