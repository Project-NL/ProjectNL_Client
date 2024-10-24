#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NLAbilitySystemInitializationData.generated.h"

class UBasicAttributeSet;
class UGameplayAbility;
class UBaseInputTriggerAbility;
class UAttributeSet;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FNLAbilitySystemInitializationData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<UBasicAttributeSet>> AttributeSets;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<UBaseInputTriggerAbility>> GameplayAbilities;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayAbility> FirstNameAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayAbility> SecondNameAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TSubclassOf<UGameplayAbility> ThirdNameAbility;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffects;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FGameplayTagContainer GameplayTags;
};
