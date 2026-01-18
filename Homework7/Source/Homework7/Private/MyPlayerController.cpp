#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MyPawn.h"
#include "InputActionValue.h"

AMyPlayerController::AMyPlayerController()

	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	JumpAction(nullptr),
	LookAction(nullptr)
{}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EI = Cast<UEnhancedInputComponent>(InputComponent))
	{
		if (MoveAction)
		{
			EI->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Move);
		}

		if (JumpAction)
		{
			EI->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyPlayerController::StartJump);
			EI->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayerController::StopJump);
		}

		if (LookAction)
		{
			EI->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Look);
		}
	}
}

void AMyPlayerController::Move(const FInputActionValue& value)
{
	if (AMyPawn* MyPawn = Cast<AMyPawn>(GetPawn()))
	{
		MyPawn->HandleMove(value.Get<FVector2D>());
	}
}

void AMyPlayerController::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		if (AMyPawn* ControlledPawn = Cast<AMyPawn>(GetPawn()))
		{
			ControlledPawn->HandleStartJump();
		}
	}
}

void AMyPlayerController::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		if (AMyPawn* ControlledPawn = Cast<AMyPawn>(GetPawn()))
		{
			ControlledPawn->HandleStopJump();
		}
	}
}

void AMyPlayerController::Look(const FInputActionValue& value)
{
	if (AMyPawn* MyPawn = Cast<AMyPawn>(GetPawn()))
	{
		MyPawn->HandleLook(value.Get<FVector2D>());
	}
}