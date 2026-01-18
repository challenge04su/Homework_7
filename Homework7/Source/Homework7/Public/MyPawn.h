#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyPawn.generated.h"

struct FInputActionValue;

UCLASS()
class HOMEWORK7_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();

	void HandleMove(const FVector2D& MoveInput);
	void HandleLook(const FVector2D& LookInput);
	void HandleStartJump();
	void HandleStopJump();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void CheckGround();

	bool isGround;
	float VerticalVelocity;

	UPROPERTY(EditAnywhere)
	float jumpSpeed;
	UPROPERTY(EditAnywhere)
	float groundMoveSpeed;
	UPROPERTY(EditAnywhere)
	float Gravity;
	UPROPERTY(EditAnywhere)
	float airControlRatio;

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* MovementComp;
};
