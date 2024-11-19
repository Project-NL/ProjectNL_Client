#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AT_PutDamageWithEvent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPutDamageWithEventSignature, float, ReceivedDamage);

UCLASS()
class PROJECTNL_API UAT_PutDamageWithEvent : public UAbilityTask
{
	GENERATED_BODY()
	
	
public:
	UPROPERTY()
	FPutDamageWithEventSignature OnDamage;
	
	static UAT_PutDamageWithEvent* InitialEvent(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;

	virtual void OnDestroy(bool bInOwnerFinished) override;

	UFUNCTION()
	void OnDamageReceived(float ReceivedDamage);
};
