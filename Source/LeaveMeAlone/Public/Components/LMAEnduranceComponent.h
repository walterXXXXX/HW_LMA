// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAEnduranceComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnduranceChanged, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAEnduranceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULMAEnduranceComponent();

	FOnEnduranceChanged OnEnduranceChanged; 

	UFUNCTION(BlueprintCallable)
	float GetEndurance() const { return Endurance; }

	UFUNCTION(BlueprintCallable)
	void OnTakeFatigue(float FatiguePoints);

	UFUNCTION(BlueprintCallable)
	void SetAllowRecovery(bool _AllowRecovery) { AllowRecovery = _AllowRecovery; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxEndurance = 100.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float RecoveryPeriod = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoveryPointsPerTime = 2.0f;

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Endurance = 0.0f;
	bool AllowRecovery = true;
	
	FTimerHandle RecoveryTimerHandle;

	void EnduranceRecovery();
		
};
