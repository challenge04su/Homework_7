#include "MyPawn.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	CollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComp"));
	SetRootComponent(CollisionComp);
	CollisionComp->SetSimulatePhysics(false);
	CollisionComp->SetCollisionProfileName(TEXT("Pawn"));

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	VerticalVelocity = 0.f;
	jumpSpeed = 600.f;
	Gravity = 980.f;
	isGround = false;
	groundMoveSpeed = 600.f;
	airControlRatio = 0.4f;
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isGround)
	{
		VerticalVelocity -= Gravity * DeltaTime;
	}

	FVector MoveDelta = FVector(0.f, 0.f, VerticalVelocity * DeltaTime);
	AddActorWorldOffset(MoveDelta, true);

	CheckGround();
}

void AMyPawn::CheckGround()
{
	float CapsuleHalfHeight = CollisionComp->GetScaledCapsuleHalfHeight();

	FVector Start = GetActorLocation() - FVector(0.f, 0.f, CapsuleHalfHeight - 2.f);
	FVector End = Start - FVector(0.f, 0.f, 10.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);
	
	if (bHit && VerticalVelocity <= 0.f)
	{
		if (!isGround)
		{
			isGround = true;
			VerticalVelocity = 0.f;
		}
	}
	else
	{
		isGround = false;
	}
}

void AMyPawn::HandleMove(const FVector2D& MoveInput)
{
	if (MoveInput.IsNearlyZero())
		return;

	float Speed = isGround
		? groundMoveSpeed
		: groundMoveSpeed * airControlRatio;

	FVector MoveDir =
		GetActorForwardVector() * MoveInput.X +
		GetActorRightVector() * MoveInput.Y;

	MoveDir.Normalize();

	AddActorWorldOffset(MoveDir * Speed * GetWorld()->GetDeltaSeconds(), true);
}

void AMyPawn::HandleLook(const FVector2D& LookInput)
{
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void AMyPawn::HandleStartJump()
{
	if (!isGround)
	{
		return;
	}

	VerticalVelocity = jumpSpeed;
	isGround = false;
}

void AMyPawn::HandleStopJump()
{
	if (VerticalVelocity > 0.f)
	{
		VerticalVelocity *= 0.3f;
	}
}

