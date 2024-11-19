#include "BaseCharacter.h"
#include "ProjectNL/Helper/EnumHelper.h"

#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/GAS/Attribute/BaseAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectNL/Helper/GameplayTagHelper.h"


ABaseCharacter::ABaseCharacter()
{
	EntityType = EEntityCategory::Undefined;
	EquipComponent = CreateDefaultSubobject<UEquipComponent>("Equip Component");
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	// 태그 컨테이너 선언
	// if(!GetCharacterMovement())
	// {
	// 	return;
	// }
	// // Check if the character is falling
	
}

void ABaseCharacter::Initialize()
{
	if (AbilitySystemComponent)
	{
		if (GetEquipComponent()->GetIsFirstEquipWeapon())
		{
			AbilitySystemComponent->AddLooseGameplayTag(NlGameplayTags::Status_Combat);
		}
	}
}

void ABaseCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if(AbilitySystemComponent)
	{
		if (GetCharacterMovement()->IsFalling())
		{
			// 떨어지는 상태: Falling 태그 추가, Grounded 태그 제거
			if (!AbilitySystemComponent->HasMatchingGameplayTag(NlGameplayTags::Status_Movement_Falling))
			{
				AbilitySystemComponent->AddLooseGameplayTag(NlGameplayTags::Status_Movement_Falling);
			}
			AbilitySystemComponent->RemoveLooseGameplayTag(NlGameplayTags::Status_Movement_Grounded);
		}
		else
		{
			// 땅에 있는 상태: Grounded 태그 추가, Falling 태그 제거
			if (!AbilitySystemComponent->HasMatchingGameplayTag(NlGameplayTags::Status_Movement_Grounded))
			{
				AbilitySystemComponent->AddLooseGameplayTag(NlGameplayTags::Status_Movement_Grounded);
			}
			AbilitySystemComponent->RemoveLooseGameplayTag(NlGameplayTags::Status_Movement_Falling);
		}
	}
}


void ABaseCharacter::Server_ApplyGameplayEffectToSelf_Implementation(
	TSubclassOf<UGameplayEffect> Effect, const uint32 Level)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		Server_ApplyGameplayEffectToSelf(Effect);
		return;
	}

	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	AbilitySystemComponent->ApplyGameplayEffectToSelf(
		Effect.GetDefaultObject(), Level
		, AbilitySystemComponent->MakeEffectContext());
}

void
ABaseCharacter::Server_RemoveActiveGameplayEffectBySourceEffect_Implementation(
	TSubclassOf<UGameplayEffect> Effect)
{
	if (GetLocalRole() != ROLE_Authority)
	{
		Server_ApplyGameplayEffectToSelf(Effect);
		return;
	}

	if (!IsValid(AbilitySystemComponent))
	{
		return;
	}

	AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(
		Effect, AbilitySystemComponent);
}

void ABaseCharacter::MovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	const float MovementSpeed = Data.NewValue;

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}