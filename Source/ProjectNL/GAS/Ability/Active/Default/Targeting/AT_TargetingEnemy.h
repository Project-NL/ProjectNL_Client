// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ProjectNL/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectNL/Helper/UtilHelper.h"
#include "AT_TargetingEnemy.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAbilityTaskCleared);
class USpringArmComponent;
UCLASS()
class PROJECTNL_API UAT_TargetingEnemy : public UAbilityTask
{
	GENERATED_BODY()


public:
	static UAT_TargetingEnemy* InitialEvent(UGameplayAbility* OwningAbility);
	
	virtual void Activate() override;
	
	

	GETTER_SETTER(int8,NearestEnemyCheck);

	virtual void TargetNearestEnemy();

	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskCleared OnCanceled;

protected:
	virtual void TickTask(float DeltaTime) override;
	void ReleaseLockOnTarget();
	AActor* FindNearestTarget() const;
	void CameraRotation(float DeltaTime);

	void LockOnTarget(AActor* NewTarget);

	// 스프링암 설정을 저장하는 함수
	UFUNCTION(BlueprintCallable, Category = "SpringArm")
	void SaveSpringArmSettings(USpringArmComponent* SpringArm);

	// 스프링암 설정을 복원하는 함수
	UFUNCTION(BlueprintCallable, Category = "SpringArm")
	void RestoreSpringArmSettings(USpringArmComponent* SpringArm);

private:
	UPROPERTY()
	TObjectPtr<AActor> CurrentTarget{};
	
	UPROPERTY()
	TObjectPtr<AActor> NearestEnemy{};
	
	UPROPERTY(EditAnywhere, Category = "Targeting")
	float TargetingRange = 1000.0f;

	UPROPERTY()
	float OriginalCharacterSpeed{};
	UPROPERTY()
	int8 NearestEnemyCheck=false;
	
	// 저장된 스프링암 설정
	UPROPERTY()
	FRotator SavedRotation;
	UPROPERTY()
	FVector SavedLocation;
	UPROPERTY()
	float SavedTargetArmLength;
	UPROPERTY()
	int8 bSavedUsePawnControlRotation;
	UPROPERTY()
	int8 bSavedInheritPitch;
	UPROPERTY()
	int8 bSavedInheritYaw;
	UPROPERTY()
	int8 bSavedInheritRoll;
	UPROPERTY()
	int8 bSavedEnableCameraLag;
	

};
