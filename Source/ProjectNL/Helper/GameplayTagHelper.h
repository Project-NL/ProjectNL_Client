#pragma once
#include "NativeGameplayTags.h"

struct FGameplayTag;

namespace NlGameplayTags
{
	PROJECTNL_API FGameplayTag FindTagByString(const FString& TagString
																						, bool bMatchPartialString = false);

	// Ability 관련 최상위 트리 태그로 하나만 들어갈 수 있음
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability)
	
	// 특정 Actor의 카테고리 관련 최상위 트리 태그로 하나만 들어갈 수 있음 (ex. 플레이어, 엔티티, 오브젝트 등)
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Category)
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Category_Entity)
	
	// 특정 액터의 행동 상태에 대한 최상위 트리 태그로 하나만 들어갈 수 있음
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State)
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Idle)
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_GrabWeapon)
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_PutWeapon)

	// 특정 액터의 현재 상황에 대해 나타내는 태그로 여러개가 들어갈 수 있음.
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status)
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Combat)
	PROJECTNL_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Status_Targeting)
}
