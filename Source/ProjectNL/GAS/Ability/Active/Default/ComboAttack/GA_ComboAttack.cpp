#include "GA_ComboAttack.h"

//#include "AT_HeavyAttack.h"
#include "AnimNotify/ComboAttackEndNotify.h"
#include "ProjectNL/GAS/Ability/Utility/PlayMontageWithEvent.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/GAS/Ability/Active/Default/ComboAttack/AnimNotify/ComboAttackNotifyState.h"
#include"Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectNL/Helper/GameplayTagHelper.h"


UGA_ComboAttack::UGA_ComboAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	ComboClearCooldown = 0;
}

bool UGA_ComboAttack::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayTagContainer* SourceTags,
                                         const FGameplayTagContainer* TargetTags,
                                         FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags
	                               , OptionalRelevantTags))
	{
		return false;
	}
	return true;
}


void UGA_ComboAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle
                                      , const FGameplayAbilityActorInfo* ActorInfo
                                      , const FGameplayAbilityActivationInfo
                                      ActivationInfo
                                      , const FGameplayEventData* TriggerEventData)
{
	// 입력이 눌린 시각 기록
	InputPressedTime = FDateTime::Now();;

	HeavyAttackPerform = false;
	ComboAttackPerform = false;

	UE_LOG(LogTemp, Display, TEXT("Test Combo ComboAttackTask!!"));
	CurrentCharacter = Cast<ABaseCharacter>(
			GetAvatarActorFromActorInfo());
}

void UGA_ComboAttack::HandleComboNotifyStart(const EHandEquipStatus AttackHand)
{
	// TODO: 공격 시 line-trace 관련 코드 여기에 추가하면 좋다.
	UE_LOG(LogTemp, Display, TEXT("Test Combo Notify Start"))
}

void UGA_ComboAttack::HandleComboNotifyEnd(const EHandEquipStatus AttackHand)
{
	UE_LOG(LogTemp, Display, TEXT("Test Combo Notify End"))
	// TODO: 공격 line-trace 탐색 중단을 선언 하는 코드 추가
}

void UGA_ComboAttack::HandleComboEndNotify()
{
	ClearDelegate();
	ComboIndex = ComboIndex == MaxCombo - 1 ? 0 : ComboIndex + 1;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
	           , false);
}


void UGA_ComboAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ClearDelegate();
	GetWorld()->GetTimerManager().SetTimer(ComboClearTimerHandle, FTimerDelegate::CreateLambda([&]
	{
		ComboIndex = 0;
	}), ComboClearCooldown, false);
	UE_LOG(LogTemp, Display, TEXT("Test Combo OnCompleted"));
}

void UGA_ComboAttack::OnCompletedAbility(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(CurrentCharacter->GetController()))
	{
		PlayerController->SetIgnoreMoveInput(false);
		//PlayerController->SetIgnoreLookInput(true);
		UE_LOG(LogTemp, Warning, TEXT("캐릭터 입력이 활성화 되었습니다."));
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
	           , true);
}

void UGA_ComboAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	ClearDelegate();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true
	           , true);
	UE_LOG(LogTemp, Display, TEXT("Test Combo onCanceled"));
}

void UGA_ComboAttack::ClearDelegate()
{
	if (ComboAttackNotifyState)
	{
		ComboAttackNotifyState->OnNotifiedBegin.RemoveAll(this);
		ComboAttackNotifyState->OnNotifiedEnd.RemoveAll(this);
	}

	if (ComboAttackEndNotify)
	{
		ComboAttackEndNotify->OnNotified.RemoveAll(this);
	}
}

void UGA_ComboAttack::EndAbility(const FGameplayAbilitySpecHandle Handle
                                 , const FGameplayAbilityActorInfo* ActorInfo
                                 , const FGameplayAbilityActivationInfo
                                 ActivationInfo, bool bReplicateEndAbility
                                 , bool bWasCancelled)
{
	if (APlayerController* PlayerController = Cast<APlayerController>(CurrentCharacter->GetController()))
	{
		PlayerController->SetIgnoreMoveInput(false);
		//PlayerController->SetIgnoreLookInput(true);
		UE_LOG(LogTemp, Warning, TEXT("캐릭터 입력이 활성화 되었습니다."));
	}
		
	UE_LOG(LogTemp, Display, TEXT("Test Combo EndAbility"));
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
	                  , bWasCancelled);
	
	// 캐릭터 입력 비활성화

}

void UGA_ComboAttack::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                   const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputPressed(Handle, ActorInfo, ActivationInfo);

	UE_LOG(LogTemp, Display, TEXT("Test Combo InputPressed"));
}

void UGA_ComboAttack::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
                                    const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);
	const FTimespan HoldDuration = FDateTime::Now() - InputPressedTime;
	UAbilitySystemComponent* AbilitySystem = GetAbilitySystemComponentFromActorInfo();
	if(!AbilitySystem)
	{
		return;
	}
	// 태그에 따라 조건 분기
	if (AbilitySystem->HasMatchingGameplayTag(NlGameplayTags::Status_Movement_Falling))
	{
		ExecuteJumpAttack();
	}
	else
	{
		if (HoldDuration.GetTotalSeconds() < 0.8)
		{
			ExecuteComboAttack();
		}
		else
		{
			ExecuteHeavyAttack();
		}
		UE_LOG(LogTemp, Display, TEXT("Test Combo InputReleased"));
	}
}

void UGA_ComboAttack::ExecuteHeavyAttack()
{
	ComboIndex = 0;
	ClearDelegate();
	const TObjectPtr<UAnimMontage> HeavyAttack = CurrentCharacter->
	                                             GetEquipComponent()->GetHeavyAttackAnim();

	if (IsValid(HeavyAttackTask))
	{
		HeavyAttackTask->EndTask();
		HeavyAttackTask = nullptr;
	}

	HeavyAttackTask = UPlayMontageWithEvent::InitialEvent(this, NAME_None
	                                                      , HeavyAttack
	                                                      , FGameplayTagContainer());
	HeavyAttackTask->OnCancelled.AddDynamic(this, &UGA_ComboAttack::OnCancelled);
	HeavyAttackTask->OnBlendOut.AddDynamic(this, &UGA_ComboAttack::OnCompletedAbility);
	HeavyAttackTask->OnCompleted.AddDynamic(this, &UGA_ComboAttack::OnCompletedAbility);
	HeavyAttackTask->OnInterrupted.AddDynamic(this, &UGA_ComboAttack::OnCompletedAbility);
	HeavyAttackTask->ReadyForActivation();
}

void UGA_ComboAttack::ExecuteComboAttack()
{
	if (CurrentCharacter)
	{
		if (IsValid(ComboAttackTask))
		{
			ComboAttackTask->EndTask();
			ComboAttackTask = nullptr;
		}

		const TArray<TObjectPtr<UAnimMontage>> ComboAttack = CurrentCharacter->
		                                                     GetEquipComponent()->GetComboAttackAnim();
		MaxCombo = ComboAttack.Num();

		if (!IsValid(ComboAttack[ComboIndex]))
		{
			UE_LOG(LogTemp, Error, TEXT("Fail to Load Combo Animation Try Again"));
			ComboIndex = 0;
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo
			           , true, true);
		}

		SetCurrentMontage(ComboAttack[ComboIndex]);

		// TODO: 메인 서브 선택해서 Delegate 실행되게끔 처리
		// TODO: 매번마다 Delegate를 연결하는 방식이 아닌, 최소한의 연결 방식을 활용하고 싶으나
		// 당장 나은 방식을 찾지는 못함. 별도의 트리거 변수를 사용한다면 가능할 수는 있으나
		// 코드가 난잡해지는 문제가 있을 수 있어 정확하게 효과적인지 비교는 해봐야함

		for (FAnimNotifyEvent NotifyEvent : GetCurrentMontage()->Notifies)
		{
			if (const TObjectPtr<UComboAttackEndNotify> AttackEndNotify = Cast<UComboAttackEndNotify>(
				NotifyEvent.Notify))
			{
				ComboAttackEndNotify = AttackEndNotify;
				ComboAttackEndNotify->OnNotified.AddDynamic(this, &ThisClass::HandleComboEndNotify);
			}


			if (const TObjectPtr<UComboAttackNotifyState> AttackNotifyState = Cast<UComboAttackNotifyState>(
				NotifyEvent.NotifyStateClass))
			{
				ComboAttackNotifyState = AttackNotifyState;
				ComboAttackNotifyState->OnNotifiedBegin.AddDynamic(
					this, &ThisClass::HandleComboNotifyStart);
				ComboAttackNotifyState->OnNotifiedEnd.AddDynamic(
					this, &ThisClass::HandleComboNotifyEnd);
			}
		}

		ComboAttackTask = UPlayMontageWithEvent::InitialEvent(this, NAME_None
		                                                      , GetCurrentMontage()
		                                                      , FGameplayTagContainer());
		ComboAttackTask->OnCancelled.AddDynamic(this, &UGA_ComboAttack::OnCancelled);
		ComboAttackTask->OnBlendOut.AddDynamic(this, &UGA_ComboAttack::OnCompleted);
		ComboAttackTask->OnCompleted.AddDynamic(this, &UGA_ComboAttack::OnCompleted);
		ComboAttackTask->OnInterrupted.AddDynamic(this, &UGA_ComboAttack::OnCompletedAbility);

		ComboAttackTask->ReadyForActivation();
	}
}

void UGA_ComboAttack::ExecuteJumpAttack()
{
    // 캐릭터 유효성 체크
    if (!CurrentCharacter)
    {
        UE_LOG(LogTemp, Error, TEXT("ExecuteJumpAttack: CurrentCharacter가 null입니다."));
        return;
    }

    // 레이캐스트로 바닥 거리 체크
    const FVector CharacterLocation = CurrentCharacter->GetActorLocation();
    const FVector TraceStart = CharacterLocation;
    const FVector TraceEnd = CharacterLocation - FVector(0, 0, 1000.0f); // 레이 길이 (1000 유닛)
    FHitResult HitResult;

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(CurrentCharacter); // 캐릭터를 레이캐스트에서 제외

    // 레이캐스트 실행
    const bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        TraceStart,
        TraceEnd,
        ECC_Visibility, // 바닥 감지용 충돌 채널
        CollisionParams
    );

    if (bHit)
    {
        const FVector HitLocation = HitResult.ImpactPoint; // 바닥에 맞은 지점
        const float DistanceToGround = (CharacterLocation - HitLocation).Size(); // 바닥과의 거리 계산

        // 디버그 스피어 표시 (히트 위치)
        DrawDebugSphere(GetWorld(), HitLocation, 20.0f, 12, FColor::Red, false, 2.0f); // 20.0f 반지름의 빨간색 스피어

        // 디버그 라인 표시 (캐릭터 위치 -> 바닥 히트 위치)
        DrawDebugLine(GetWorld(), TraceStart, HitLocation, FColor::Green, false, 2.0f, 0, 1.0f);

        // 거리 디버그 메시지 출력
        UE_LOG(LogTemp, Warning, TEXT("ExecuteJumpAttack: 바닥까지 거리 = %.2f"), DistanceToGround);

        // 최소 거리 조건 확인
        const float MinDistanceToAttack = 130.0f; // 공격 허용 최소 거리 (130 유닛)
        if (DistanceToGround < MinDistanceToAttack)
        {
            UE_LOG(LogTemp, Warning, TEXT("ExecuteJumpAttack: 바닥까지의 거리가 너무 가까워 공격을 실행할 수 없습니다."));
            return;
        }
    }
    else
    {
        // 바닥 감지 실패 처리
        UE_LOG(LogTemp, Warning, TEXT("ExecuteJumpAttack: 바닥이 감지되지 않았습니다. 공격 실행이 취소됩니다."));

        // 디버그 라인 표시 (바닥 미감지 시)
        DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 2.0f, 0, 1.0f);
        return;
    }

	// 캐릭터 입력 비활성화
	if (APlayerController* PlayerController = Cast<APlayerController>(CurrentCharacter->GetController()))
	{
    	PlayerController->SetIgnoreMoveInput(true);
    	//PlayerController->SetIgnoreLookInput(true);
    	UE_LOG(LogTemp, Warning, TEXT("ExecuteJumpAttack: 캐릭터 입력이 차단되었습니다."));
		// 점프 차단
		CurrentCharacter->StopJumping();
	}

    // 점프 공격 실행
    ComboIndex = 0;
    ClearDelegate();

    const TObjectPtr<UAnimMontage> JumpAttackAnim = CurrentCharacter->
        GetEquipComponent()->GetJumpAttackAnim();

    if (IsValid(JumpAttackTask))
    {
        JumpAttackTask->EndTask();
        JumpAttackTask = nullptr;
    }

    JumpAttackTask = UPlayMontageWithEvent::InitialEvent(this, NAME_None,
                                                         JumpAttackAnim,
                                                         FGameplayTagContainer());
    JumpAttackTask->OnCancelled.AddDynamic(this, &UGA_ComboAttack::OnCancelled);
    JumpAttackTask->OnBlendOut.AddDynamic(this, &UGA_ComboAttack::OnCompletedAbility);
    JumpAttackTask->OnCompleted.AddDynamic(this, &UGA_ComboAttack::OnCompletedAbility);
    JumpAttackTask->OnInterrupted.AddDynamic(this, &UGA_ComboAttack::OnCompletedAbility);
    JumpAttackTask->ReadyForActivation();
}
