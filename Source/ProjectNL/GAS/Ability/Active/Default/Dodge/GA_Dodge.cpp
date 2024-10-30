// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/GAS/Ability/Active/Default/Dodge/GA_Dodge.h"

#include "Kismet/KismetMathLibrary.h"
#include "ProjectNL/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

UGA_Dodge::UGA_Dodge(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void UGA_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	PlayDirectionalDodgeAnimation();
}

void UGA_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_Dodge::PlayDirectionalDodgeAnimation()
{

	ABaseCharacter* CurrentCharacter = Cast<ABaseCharacter>(
		GetAvatarActorFromActorInfo());
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(CurrentCharacter);

	FVector Velocity = CurrentCharacter->GetVelocity(); // 캐릭터의 속도 벡터를 가져옴
	FRotator ActorRotation = CurrentCharacter->GetActorRotation(); // 캐릭터의 현재 회전 값을 가져옴

	// 이동 방향을 계산 (CalculateDirection 함수 사용)
	float Yaw = UKismetMathLibrary::NormalizedDeltaRotator(Velocity.Rotation(), ActorRotation).Yaw;

	// 방향 인덱스 결정 (8방향)
	int32 DirectionIndex{};
	if (Yaw >= -22.5f && Yaw <= 22.5f)
	{
		DirectionIndex = 0; // 앞
	}
	else if (Yaw > 22.5f && Yaw <= 67.5f)
	{
		DirectionIndex = 1; // 앞 오른쪽
	}
	else if (Yaw > 67.5f && Yaw <= 112.5f)
	{
		DirectionIndex = 2; // 오른쪽
	}
	else if (Yaw > 112.5f && Yaw <= 157.5f)
	{
		DirectionIndex = 3; // 뒤 오른쪽
	}
	else if (Yaw > 157.5f || Yaw <= -157.5f)
	{
		DirectionIndex = 4; // 뒤
	}
	else if (Yaw > -157.5f && Yaw <= -112.5f)
	{
		DirectionIndex = 5; // 뒤 왼쪽
	}
	else if (Yaw > -112.5f && Yaw <= -67.5f)
	{
		DirectionIndex = 6; // 왼쪽
	}
	else if (Yaw > -67.5f && Yaw <= -22.5f)
	{
		DirectionIndex = 7; // 앞 왼쪽
	}

	FGameplayTagContainer ActiveTags;
	TargetASC->GetOwnedGameplayTags(ActiveTags);

	// 조건에 따라 애니메이션 실행
	Task = UPlayMontageWithEvent::InitialEvent(this, NAME_None
																						, DodgeAnimMontage[DirectionIndex]
																						, FGameplayTagContainer());
	Task->OnCancelled.AddDynamic(this, &UGA_Dodge::OnCancelled);
	Task->OnInterrupted.AddDynamic(this, &UGA_Dodge::OnCancelled);
	Task->OnBlendOut.AddDynamic(this, &UGA_Dodge::OnCancelled);
	Task->OnCompleted.AddDynamic(this, &UGA_Dodge::OnCompleted);
	Task->ReadyForActivation();
}

void UGA_Dodge::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
					, false);
}

void UGA_Dodge::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
					, false);
}

void UGA_Dodge::ClearDelegate()
{
	
}

