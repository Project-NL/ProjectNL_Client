// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/GAS/Ability/Utility/BaseInputTriggerAbility.h"
#include "GA_Dodge.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNL_API UGA_Dodge : public UBaseInputTriggerAbility
{
	GENERATED_BODY()
public:
	UGA_Dodge(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle
															, const FGameplayAbilityActorInfo* ActorInfo
															, const FGameplayAbilityActivationInfo
															ActivationInfo
															, const FGameplayEventData*
															TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle
													, const FGameplayAbilityActorInfo* ActorInfo
													, const FGameplayAbilityActivationInfo ActivationInfo
													, bool bReplicateEndAbility
													, bool bWasCancelled) override;
	
	void PlayDirectionalDodgeAnimation();

	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);
	
	void ClearDelegate();
private:

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TArray<TObjectPtr<UAnimMontage>> DodgeAnimMontage;
	
	TObjectPtr<class UPlayMontageWithEvent> Task;
};