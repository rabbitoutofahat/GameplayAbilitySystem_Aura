// Copyright Druid Mechanics


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; // In multiplayer, when an entity changes on the server, that change will be replicated/sent down to all clients connected to the server 

}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{

	Super::PlayerTick(DeltaTime);

	CursorTrace();

}

void AAuraPlayerController::CursorTrace()
{

	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;
	
	// Cast REMOVED, see TScriptInterface wrapper in the .h file
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor(); 

	/**
	* Line trace from cursor. There are several scenarios:
	* A. LastActor is null and ThisActor is null
	*    - Do nothing
	* B. LastActor is null but ThisActor is valid
	*    - Highlight ThisActor
	* C. LastActor is valid and ThisActor is null
	*    - UnHighlight LastActor
	* D. Both actors are valid, but LastActor != ThisActor
	*    - UnHighlight LastActor, and Highlight ThisActor
	* E. Both actors are valid, and are the same actor
	*    - Do nothing
	*/

	if (LastActor == nullptr)
	{

		if (ThisActor != nullptr)
		{
			// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A - do nothing
		}
	}
	else // LastActor is valid
	{

		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // Both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}
}

void AAuraPlayerController::BeginPlay()
{

	Super::BeginPlay();
    check(AuraContext); // Check is going to halt execution if the condition pointed to by AuraContext. AuraContext will be evaluated as 'false' if it hasn't been set yet

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()); // Used to add mapping contexts
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData; // Enables use of input from keyboard + input, and can use input to affect UI such as widgets
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false); // As soon as cursor is captured in viewport, we will not hide the cursor
	SetInputMode(InputModeData);

}

void AAuraPlayerController::SetupInputComponent()
{

	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent); // Will crash if this cast fails

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);

}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{

	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>(); // Input action value in the form of a 2D vector
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Gives us the normalised forward vector that corresponds to the yaw rotation vector
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{

		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);

	}

}


