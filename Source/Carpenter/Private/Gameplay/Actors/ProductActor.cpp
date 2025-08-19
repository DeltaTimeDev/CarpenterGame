// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Actors/ProductActor.h"

#include "Components/BoxComponent.h"
#include "Framework/CarpenterCharacter.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AProductActor::AProductActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBoxComp"));
	RootBoxComp->SetIsReplicated(true);
	RootComponent = RootBoxComp;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootBoxComp);

	bReplicates = true;
}



void AProductActor::OnRep_DataChanged(FReplicatedProductData NewData)
{
	UpdateVisual();
}

void AProductActor::SetProductData(FReplicatedProductData InData)
{
	Data = InData;
	UpdateVisual();
}

void AProductActor::Paint(FLinearColor NewColor)
{
	Data.Color = NewColor;
	UpdateVisual();
}

void AProductActor::UpdateVisual()
{
	MeshComponent->SetStaticMesh(Data.Mesh);
	SetColor(Data.Color);
}

void AProductActor::SetColor(FLinearColor NewColor)
{
	UMaterialInstanceDynamic* DynamicMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicMaterial)
	{
		if (NewColor != FLinearColor::White)
		{
			DynamicMaterial->SetScalarParameterValue("ApplyOverlayColor", 1);
		}
		DynamicMaterial->SetVectorParameterValue("ColorParameter", NewColor);
	}
}

// Called when the game starts or when spawned
void AProductActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		RootBoxComp->SetSimulatePhysics(true);
	}
}

void AProductActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProductActor, Data);
	DOREPLIFETIME(AProductActor, TargetLocation);
	DOREPLIFETIME(AProductActor, TargetRotation);
}


// Called every frame
void AProductActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		if (bIsGrabbed && IsValid(GrabbedBy))
		{
			GrabbedBy->UpdateGrab();
		}
	}

	ReplicateMovementWithSmoothing(DeltaTime);
}

void AProductActor::ReplicateMovementWithSmoothing(float DeltaTime)
{
	if (HasAuthority())
	{
		TargetLocation = GetActorLocation();
		TargetRotation = GetActorRotation();
	}
	else if (HasAuthority() == false)
	{
		FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLocation, DeltaTime, 10.0f);
		FRotator NewRot = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 10.0f);
		SetActorLocationAndRotation(NewLoc, NewRot);
	}
}

FVector AProductActor::GetGrabPoint()
{
	FVector WorldLocation = MeshComponent->GetSocketLocation(GrabSocketName);
	return MeshComponent->GetComponentTransform().InverseTransformPosition(WorldLocation);
}

void AProductActor::InteractStart(class ACarpenterCharacter* CarpenterCharacter)
{
	if (HasAuthority())
	{
		if (!bIsGrabbed)
		{
			bIsGrabbed = true;
			GrabbedBy = CarpenterCharacter;
			CarpenterCharacter->Grab(this);
		}
	}
}

void AProductActor::InteractEnd(class ACarpenterCharacter* CarpenterCharacter)
{
	if (HasAuthority())
	{
		if (GrabbedBy == CarpenterCharacter)
		{
			bIsGrabbed = false;
			GrabbedBy = nullptr;
			CarpenterCharacter->Release(this);
		}
	}
}

