// Copyright Epic Games, Inc. All Rights Reserved.


#include "Framework/CarpenterCharacter.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/WidgetInteractionComponent.h"
#include "Engine/LocalPlayer.h"
#include "Gameplay/Interfaces/GrabbableInterface.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ACarpenterCharacter

ACarpenterCharacter::ACarpenterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	WidgetInteractionComponent = CreateDefaultSubobject<
		UWidgetInteractionComponent>(TEXT("WidgetInteractionComponent"));
	WidgetInteractionComponent->InteractionDistance = 500;
	WidgetInteractionComponent->bShowDebug = true;
	WidgetInteractionComponent->SetupAttachment(FirstPersonCameraComponent);

	PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	PhysicsHandle->SetIsReplicated(true);
}

//////////////////////////////////////////////////////////////////////////// Input

void ACarpenterCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ACarpenterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACarpenterCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACarpenterCharacter::Look);

		// Interaction
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Started, this,
		                                   &ACarpenterCharacter::InteractionStart);
		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Completed, this,
		                                   &ACarpenterCharacter::InteractionEnd);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void ACarpenterCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ACarpenterCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACarpenterCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACarpenterCharacter::InteractionStart(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("InteractionStart"));

	if (USceneComponent* HitComponent = LineTraceForHitComponent())
	{
		IInteractableInterface* LastInteractedInterface = GetInteractionInterface(HitComponent);

		if (LastInteractedInterface)
		{
			LastInteractedComponent = HitComponent;

			if (LastInteractedInterface->bInteractLocally)
			{
				LastInteractedInterface->InteractStart(this);
			}
			else
			{
				ServerInteractionStart(HitComponent);
			}
		}
	}
}

void ACarpenterCharacter::InteractionEnd(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Error, TEXT("InteractionEnd"));

	if (IsValid(LastInteractedComponent))
	{
		IInteractableInterface* LastInteractedInterface = GetInteractionInterface(LastInteractedComponent);
		
		if (LastInteractedInterface->bInteractLocally)
		{
			LastInteractedInterface->InteractEnd(this);
		}
		else
		{
			ServerInteractionEnd(LastInteractedComponent);
		}
	}
}


USceneComponent* ACarpenterCharacter::LineTraceForHitComponent()
{
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector End = Start + (ForwardVector * 500.f);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
	return HitResult.GetComponent();
}

void ACarpenterCharacter::Grab(AActor* ActorToGrab)
{
	if (!ActorToGrab) return;

	if (IGrabbableInterface* GrabbableInterface = Cast<IGrabbableInterface>(ActorToGrab))
	{
		UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(ActorToGrab->GetRootComponent());
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			PhysicsHandle->GrabComponentAtLocation(
				PrimComp,
				NAME_None,
				ActorToGrab->GetActorLocation() + ActorToGrab->GetActorUpVector() * 5
			);
		}
	}
}

void ACarpenterCharacter::Release(AActor* ActorToGrab)
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}

void ACarpenterCharacter::UpdateGrab()
{
	FVector Start;
	FRotator Rot;
	Controller->GetPlayerViewPoint(Start, Rot);

	FVector TargetLocation = Start + Rot.Vector() * 100.0f;
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(TargetLocation);
	}
}

void ACarpenterCharacter::ServerInteractionStart_Implementation(class USceneComponent* InteractableActorComponent)
{
	GetInteractionInterface(InteractableActorComponent)->InteractStart(this);
}

void ACarpenterCharacter::ServerInteractionEnd_Implementation(class USceneComponent* InteractableActorComponent)
{
	GetInteractionInterface(InteractableActorComponent)->InteractEnd(this);
}

IInteractableInterface* ACarpenterCharacter::GetInteractionInterface(USceneComponent* ComponentToCast)
{
	if (IInteractableInterface* InteractableComponent = Cast<IInteractableInterface>(ComponentToCast))
	{
		return InteractableComponent;
	}
	else if (IInteractableInterface* InteractableActor = Cast<IInteractableInterface>(ComponentToCast->GetOwner()))
	{
		return InteractableActor;
	}
	return nullptr;
}
