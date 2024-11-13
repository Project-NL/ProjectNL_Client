#include "AT_PutDamageWithEvent.h"

#include "ProjectNL/GAS/NLAbilitySystemComponent.h"

UAT_PutDamageWithEvent* UAT_PutDamageWithEvent::InitialEvent(UGameplayAbility* OwningAbility)
{
	UAT_PutDamageWithEvent* NewEvent = NewAbilityTask<UAT_PutDamageWithEvent>(
		OwningAbility);

	return NewEvent;
}

void UAT_PutDamageWithEvent::Activate()
{
	if (UNLAbilitySystemComponent* ASC = Cast<UNLAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->OnDamage.AddDynamic(this, &ThisClass::OnDamageReceived);
	}
}

void UAT_PutDamageWithEvent::OnDestroy(bool bInOwnerFinished)
{
	if (UNLAbilitySystemComponent* ASC = Cast<UNLAbilitySystemComponent>(AbilitySystemComponent))
	{
		ASC->OnDamage.RemoveDynamic(this, &ThisClass::OnDamageReceived);
	}
	
	Super::OnDestroy(bInOwnerFinished);
}

void UAT_PutDamageWithEvent::OnDamageReceived(float ReceivedDamage)
{
	OnDamage.Broadcast(ReceivedDamage);
}



