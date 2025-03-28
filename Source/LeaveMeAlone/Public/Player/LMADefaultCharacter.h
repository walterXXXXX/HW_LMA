// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class ULMAEnduranceComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float MinArmLength = 300;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float MaxArmLength = 1400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Settings")
	float ArmChangeVelocity = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Endurance")
	ULMAEnduranceComponent* EnduranceComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	bool SprintState = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1.0", ClampMax = "5.0"))
	float SprintAccelerationRatio = 2.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MinEnduranceForSprint = 5.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float FatiguePeriod = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FatiguePointsPerTime = 5.0f;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	float YRotation = -75.0f;	// ������� ������ �� ��� Y
	float ArmLength = 1400.0f;	// ����� �������
	float FOV = 55.0f;			// ���� ������ ������

	FTimerHandle SprintTimerHandle;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void OnSprint();
	void OffSprint();
	void Sprinting();
	
	void CameraZoom(float Value);

	void OnDeath();
	void OnHealthChanged(float NewHealth);
	void OnEnduranceChanged(float NewEndurance);


	void RotationPlayerOnCursor();

};
