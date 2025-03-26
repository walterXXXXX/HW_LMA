// LeaveMeAlone Game by Netologiya. All RightsReserved.

#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Components/LMAHealthComponent.h"
#include "Components/LMAEnduranceComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);

	EnduranceComponent = CreateDefaultSubobject<ULMAEnduranceComponent>("EnduranceComponent");

}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (CursorMaterial) {
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), 
			CursorMaterial, CursorSize, FVector(0));
	}

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);
	
	OnEnduranceChanged(EnduranceComponent->GetEndurance());
	EnduranceComponent->OnEnduranceChanged.AddUObject(this, &ALMADefaultCharacter::OnEnduranceChanged);
}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!(HealthComponent->IsDead())) {
		RotationPlayerOnCursor();
	}
	if ((GetLastMovementInputVector().X == 0) || (GetLastMovementInputVector().Y == 0))
		OffSprint();
}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("CameraZoom", this, &ALMADefaultCharacter::CameraZoom);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ALMADefaultCharacter::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ALMADefaultCharacter::OffSprint);
}

void ALMADefaultCharacter::MoveForward(float Value) {
	AddMovementInput(GetActorForwardVector(), Value);
}

void ALMADefaultCharacter::MoveRight(float Value) {
	AddMovementInput(GetActorRightVector(), Value);
}

void ALMADefaultCharacter::OnSprint() {
	if (!SprintState)
		GetCharacterMovement()->MaxWalkSpeed *= SprintAccelerationRatio;
	SprintState = true;
	EnduranceComponent->SetAllowRecovery(false);
	GetWorldTimerManager().SetTimer(SprintTimerHandle, this, 
		&ALMADefaultCharacter::Sprinting, FatiguePeriod, true);
}

void ALMADefaultCharacter::OffSprint() {
	if (SprintState)
		GetCharacterMovement()->MaxWalkSpeed /= SprintAccelerationRatio;
	SprintState = false;
	EnduranceComponent->SetAllowRecovery(true);
	GetWorldTimerManager().ClearTimer(SprintTimerHandle);
}

void ALMADefaultCharacter::Sprinting() {
	EnduranceComponent->OnTakeFatigue(FatiguePointsPerTime);
	if (EnduranceComponent->GetEndurance() <= MinEnduranceForSprint)
		OffSprint();
}


void ALMADefaultCharacter::CameraZoom(float Value) {
	ArmLength += Value * ArmChangeVelocity;
	if (ArmLength < MinArmLength)
		ArmLength = MinArmLength;
	if (ArmLength > MaxArmLength)
		ArmLength = MaxArmLength;
	SpringArmComponent->TargetArmLength = ArmLength;
}

void ALMADefaultCharacter::OnDeath() {
	CurrentCursor->DestroyRenderState_Concurrent();
	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);
	if (Controller) {
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMADefaultCharacter::OnHealthChanged(float NewHealth) {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
		FString::Printf(TEXT("Health = %f"), NewHealth));
}

void ALMADefaultCharacter::OnEnduranceChanged(float NewEndurance) {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
		FString::Printf(TEXT("Endurance = %f"), NewEndurance));
}

void ALMADefaultCharacter::RotationPlayerOnCursor() {
	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(ResultHit.Location);
		}
	}
}

