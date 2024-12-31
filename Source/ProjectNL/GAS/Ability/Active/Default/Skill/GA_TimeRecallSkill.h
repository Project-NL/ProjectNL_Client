// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/GAS/Ability/Utility/BaseInputTriggerAbility.h"
#include "GA_TimeRecallSkill.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNL_API UGA_TimeRecallSkill : public UBaseInputTriggerAbility
{
	GENERATED_BODY()
public:
	UGA_TimeRecallSkill(const FObjectInitializer& ObjectInitializer);

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
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
	void OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData);
	
	UFUNCTION()
	void OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
	void PlayActionAnimation();
	void PerformRecall(float DeltaTime);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TimeRecall")
	TSubclassOf<class AGhostTrail> GhostTrailActor;
	/** 기록된 Transform을 역재생하기(시간 역행) 시작하는 함수 */

public:
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GAS")
	TObjectPtr<UAnimMontage> SkillAnimMontage;
private:
	UPROPERTY()
	TObjectPtr<class UPlayMontageWithEvent> PlayMontageTask;


};
