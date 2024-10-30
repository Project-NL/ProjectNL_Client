// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ProjectNL/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AT_TargetingEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNL_API UAT_TargetingEnemy : public UAbilityTask
{
	GENERATED_BODY()


public:
	static UAT_TargetingEnemy* InitialEvent(UGameplayAbility* OwningAbility);
	
	virtual void Activate() override;
	
	
	void ReleaseLockOnTarget();
	AActor* FindNearestTarget() const;
	void CameraRotation(float DeltaTime);

	void LockOnTarget(AActor* NewTarget);

protected:
	virtual void TickTask(float DeltaTime) override;
	virtual void TargetNearestEnemy();
private:
	UPROPERTY()
	TObjectPtr<AActor> CurrentTarget;
	
	UPROPERTY()
	TObjectPtr<AActor> NearestEnemy;
	
	UPROPERTY(EditAnywhere, Category = "Targeting")
	float TargetingRange = 1000.0f;



	int32 NearestEnemyCheck=false;
};
