// LeaveMeAlone Game by Netologiya. All RightsReserved.


#include "Components/LMAEnduranceComponent.h"

// Sets default values for this component's properties
ULMAEnduranceComponent::ULMAEnduranceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void ULMAEnduranceComponent::BeginPlay()
{
	Super::BeginPlay();

	Endurance = MaxEndurance;
	OnEnduranceChanged.Broadcast(Endurance);

}

void ULMAEnduranceComponent::OnTakeFatigue(float FatiguePoints) {
	Endurance = FMath::Clamp(Endurance - FatiguePoints, 0.0f, MaxEndurance);
	OnEnduranceChanged.Broadcast(Endurance);
	AActor* OwnerComponent = GetOwner();
	OwnerComponent->GetWorldTimerManager().SetTimer(RecoveryTimerHandle, this, 
		&ULMAEnduranceComponent::EnduranceRecovery, RecoveryPeriod, true);
}

void ULMAEnduranceComponent::EnduranceRecovery() {
	if (!AllowRecovery) return;
	Endurance = FMath::Clamp(Endurance + RecoveryPointsPerTime, 0.0f, MaxEndurance);
	OnEnduranceChanged.Broadcast(Endurance);
	if (Endurance == MaxEndurance) {
		AActor* OwnerComponent = GetOwner();
		OwnerComponent->GetWorldTimerManager().ClearTimer(RecoveryTimerHandle);
	}
}




