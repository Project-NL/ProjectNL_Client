#include "PlayerStatus.h"

#include "ProjectNL/Player/BasePlayerState.h"
#include "ProjectNL/GAS/NLAbilitySystemComponent.h"
#include "ProjectNL/GAS/Attribute/PlayerAttributeSet.h"
#include "ProjectNL/UI/Widget/ValuePercentbar/ValuePercentBar.h"

void UPlayerStatus::NativeConstruct()
{
	if (const TObjectPtr<ABasePlayerState> PlayerState = Cast<ABasePlayerState>(
		GetOwningPlayer()->PlayerState))
	{
		if (UNLAbilitySystemComponent* ASC = PlayerState->AbilitySystemComponent)
		{
			if (const UPlayerAttributeSet* PlayerAttributeSet = PlayerState->
				AttributeSet)
			{
				HealthBar->InitializePercent(PlayerAttributeSet->GetHealth(), PlayerAttributeSet->GetMaxHealth());
				
				ASC->GetGameplayAttributeValueChangeDelegate(
					PlayerAttributeSet->GetHealthAttribute()).AddUObject(
					this, &ThisClass::HandleCurrentHealthChanged);
				ASC->GetGameplayAttributeValueChangeDelegate(
					PlayerAttributeSet->GetMaxHealthAttribute()).AddUObject(
					this, &ThisClass::HandleMaxHealthChanged);
			}
		}
	}
}

void UPlayerStatus::HandleCurrentHealthChanged(const FOnAttributeChangeData& Data)
{
	HealthBar->SetCurrentValue(Data.NewValue);
}

void UPlayerStatus::HandleMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	HealthBar->SetMaxValue(Data.NewValue);
}

