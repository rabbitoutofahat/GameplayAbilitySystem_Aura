// Copyright Druid Mechanics


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; // In multiplayer, when an entity changes on the server, that change will be replicated/sent down to all clients connected to the server 

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
