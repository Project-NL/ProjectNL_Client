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
				ManaBar->InitializePercent(PlayerAttributeSet->GetMana(), PlayerAttributeSet->GetMaxMana());
				StaminaBar->InitializePercent(PlayerAttributeSet->GetStamina(), PlayerAttributeSet->GetMaxStamina());
				
				ASC->GetGameplayAttributeValueChangeDelegate(
					PlayerAttributeSet->GetHealthAttribute()).AddUObject(
					this, &ThisClass::HandleCurrentHealthChanged);
				ASC->GetGameplayAttributeValueChangeDelegate(
					PlayerAttributeSet->GetMaxHealthAttribute()).AddUObject(
					this, &ThisClass::HandleMaxHealthChanged);
				ASC->GetGameplayAttributeValueChangeDelegate(
					PlayerAttributeSet->GetManaAttribute()).AddUObject(
					this, &ThisClass::HandleCurrentManaChanged);
				ASC->GetGameplayAttributeValueChangeDelegate(
					PlayerAttributeSet->GetMaxManaAttribute()).AddUObject(
					this, &ThisClass::HandleMaxManaChanged);
				ASC->GetGameplayAttributeValueChangeDelegate(
					PlayerAttributeSet->GetStaminaAttribute()).AddUObject(
					this, &ThisClass::HandleCurrentStaminaChanged);
				ASC->GetGameplayAttributeValueChangeDelegate(
					PlayerAttributeSet->GetMaxStaminaAttribute()).AddUObject(
					this, &ThisClass::HandleMaxStaminaChanged);
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

void UPlayerStatus::HandleCurrentManaChanged(const FOnAttributeChangeData& Data)
{
	ManaBar->SetCurrentValue(Data.NewValue);
}

void UPlayerStatus::HandleMaxManaChanged(const FOnAttributeChangeData& Data)
{
	ManaBar->SetMaxValue(Data.NewValue);
}

void UPlayerStatus::HandleCurrentStaminaChanged(const FOnAttributeChangeData& Data)
{
	StaminaBar->SetCurrentValue(Data.NewValue);
}

void UPlayerStatus::HandleMaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	StaminaBar->SetMaxValue(Data.NewValue);
}

