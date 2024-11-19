#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "ProjectNL/Helper/UtilHelper.h"
#include "NLAbilitySystemComponent.generated.h"

struct FInitGameplayAbilitySystem;
struct FNLAbilitySystemInitializationData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageToAbilitySystem, float, Damage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTNL_API UNLAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UNLAbilitySystemComponent();

	void InitializeAbilitySystem(
		const FNLAbilitySystemInitializationData& InitData);

	FOnDamageToAbilitySystem OnDamage;
	
	void ReceiveDamage(const float Damage) const;

	GETTER_SETTER(bool, IsInitialized)
private:
	bool IsInitialized = false;
};
