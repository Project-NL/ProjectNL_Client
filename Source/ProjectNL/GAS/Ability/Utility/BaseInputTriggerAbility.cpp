#include "BaseInputTriggerAbility.h"

#include "ProjectNL/Helper/EnumHelper.h"
#include "AbilitySystemComponent.h"

UBaseInputTriggerAbility::UBaseInputTriggerAbility(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, bActivateAbilityInputTrigger(false)
{
	InputID = EInputIDType::None;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UBaseInputTriggerAbility::OnAvatarSet(
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);
	SetupEnhancedInputBindings(ActorInfo, Spec);
}

void UBaseInputTriggerAbility::SetupEnhancedInputBindings(
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (IsValid(ActorInfo->AvatarActor.Get()))
	{
		if (const APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
		{
			if (const AController* PawnController = AvatarPawn->GetController())
			{
				if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
					UEnhancedInputComponent>(PawnController->InputComponent.Get()))
				{
					if (UBaseInputTriggerAbility* AbilityInstance = Cast<
						UBaseInputTriggerAbility>(Spec.Ability))
					{
						if (IsValid(AbilityInstance->ActivationInputAction))
						{
							EnhancedInputComponent->BindAction(
								ActivationInputAction, ETriggerEvent::Started, AbilityInstance
								, &ThisClass::OnAbilityInputPressed, ActorInfo);

							EnhancedInputComponent->BindAction(
								ActivationInputAction, ETriggerEvent::Completed, AbilityInstance
								, &ThisClass::OnAbilityInputReleased, ActorInfo);
						}
					}
				}
			}
		}
	}
}

void UBaseInputTriggerAbility::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (const APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
	{
		if (const AController* PawnController = AvatarPawn->GetController())
		{
			// TODO: 해당 로직이 Autonomous Proxy에서 Authority로 넘어가지 않는 이슈가 있음
			// Authority가 아닌 환경에서 PawnController->InputComponent가 존재하지 않는다.
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
				UEnhancedInputComponent>(PawnController->InputComponent.Get()))
			{
				const FEnhancedInputActionEventBinding& TriggeredEventBinding =
					EnhancedInputComponent->BindAction(ActivationInputAction
																						, ETriggerEvent::Triggered, this
																						, &
																						ThisClass::OnTriggeredInputAction);

				TriggeredEventHandle = TriggeredEventBinding.GetHandle();
				CommitAbility(Handle, ActorInfo, ActivationInfo);
				return;
			}
		}
	}

	constexpr bool bReplicateCancelAbility = true;
	CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UBaseInputTriggerAbility::EndAbility(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo
	, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility
										, bWasCancelled);
	
	if (const APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
	{
		if (const AController* PawnController = AvatarPawn->GetController())
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
				UEnhancedInputComponent>(PawnController->InputComponent.Get()))
			{
				EnhancedInputComponent->RemoveBindingByHandle(TriggeredEventHandle);
			}
		}
		TriggeredEventHandle = -1;
	}
}

void UBaseInputTriggerAbility::InputReleased(
	const FGameplayAbilitySpecHandle Handle
	, const FGameplayAbilityActorInfo* ActorInfo
	, const FGameplayAbilityActivationInfo ActivationInfo)
{
	Super::InputReleased(Handle, ActorInfo, ActivationInfo);

	if (bActivateAbilityInputTrigger)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}
}


void UBaseInputTriggerAbility::OnRemoveAbility(
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	if (IsValid(ActorInfo->AvatarActor.Get()))
	{
		if (const APawn* AvatarPawn = Cast<APawn>(ActorInfo->AvatarActor.Get()))
		{
			if (const AController* PawnController = AvatarPawn->GetController())
			{
				if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
					UEnhancedInputComponent>(PawnController->InputComponent))
				{
					EnhancedInputComponent->ClearBindingsForObject(Spec.Ability.Get());
				}
			}
		}
	}

	Super::OnRemoveAbility(ActorInfo, Spec);
}

void UBaseInputTriggerAbility::OnAbilityInputPressed(
	const FGameplayAbilityActorInfo* ActorInfo)
{
	if (const ABaseCharacter* Owner = Cast<
		ABaseCharacter>(ActorInfo->AvatarActor))
	{
		Owner->GetAbilitySystemComponent()->AbilityLocalInputPressed(
			static_cast<uint32>(InputID));
	}
}

void UBaseInputTriggerAbility::OnAbilityInputReleased(
	const FGameplayAbilityActorInfo* ActorInfo)
{
	if (const ABaseCharacter* Owner = Cast<
		ABaseCharacter>(ActorInfo->AvatarActor))
	{
		Owner->GetAbilitySystemComponent()->AbilityLocalInputReleased(
			static_cast<uint32>(InputID));
	}
};

void UBaseInputTriggerAbility::OnTriggeredInputAction(
	const FInputActionValue& Value)
{
}
