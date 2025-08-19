// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/GenericStructs.h"
#include "GameFramework/Actor.h"
#include "OrderActor.generated.h"

USTRUCT(BlueprintType)
struct CARPENTER_API FOrder
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Product")
	FGuid TypeID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	FString Name = "None";
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	FLinearColor Color = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Product")
	bool IsActive = false;

};

UCLASS()
class CARPENTER_API AOrderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrderActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class USceneComponent* RootComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	class UBoxComponent* AcceptingArea;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UTextRenderComponent* TextRenderComponent;

	UPROPERTY(ReplicatedUsing= OnOrderChanged)
	FOrder Order;

	int32 GetEarnAmount(FReplicatedProductData Data);
	
	UFUNCTION()
	void OnOrderChanged(FOrder NewOrder);
	
	void SetOrder(FOrder InOrder);

	void Activate(bool Value);

	void UpdateVisual();

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast,Reliable)
	void MulticastMoneyEffect(int32 MoneyCount);

	UFUNCTION(BlueprintImplementableEvent)
	void MoneyEffect(int32 MoneyCount);
	
	/*string corresponding of color */
	TMap<FLinearColor, FString> ColorNameMap = {
		{ FLinearColor::Red, "Red" },
		{ FLinearColor::Green, "Green" },
		{ FLinearColor::Blue, "Blue" },
		{ FLinearColor::Black, "Black" },
		{ FLinearColor::White, "White" },
		{ FLinearColor::Yellow, "Yellow" }
	};
};



