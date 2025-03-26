// LeaveMeAlone Game by Netologiya. All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LMAHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LEAVEMEALONE_API ULMAHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULMAHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const { return Health; }


protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged; 

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;
	bool AddHealth(float NewHealth);

private:
	float Health = 0.0f;

	UFUNCTION()
	void OnTakeAnyDamage(
		AActor* DamagedActor, float Damage, 
		const class UDamageType* DamageType, 
		class AController* InstigatedBy, 
		AActor* DamageCauser);


	bool IsHealthFull() const;

};
