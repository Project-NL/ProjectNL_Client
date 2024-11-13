#include "GE_DamageExecCalculation.h"

#include "AbilitySystemComponent.h"
#include "ProjectNL/GAS/NLAbilitySystemComponent.h"

UGE_DamageExecCalculation::UGE_DamageExecCalculation()
{
}

void UGE_DamageExecCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();
	UAbilitySystemComponent* SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();

	AActor* TargetActor = TargetAbilitySystemComponent->GetAvatarActor();
	AActor* SourceActor = TargetAbilitySystemComponent->GetAvatarActor();

	if (UNLAbilitySystemComponent* TargetASC = Cast<UNLAbilitySystemComponent>(TargetAbilitySystemComponent))
	{
		TargetASC->ReceiveDamage(DamageValue);
	}
}
