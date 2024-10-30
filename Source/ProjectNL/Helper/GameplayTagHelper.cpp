#include "GameplayTagHelper.h"
#include "NativeGameplayTags.h"

namespace NlGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Combat, "Ability", "Ability 관련 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Category, "Category", "특정 Actor의 카테고리 관련 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Category_Entity, "Category.Entity", "특정 Actor의 카테고리가 엔티티(살아있는 생물체)인 경우");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State, "State", "특정 액터의 상태에 대한 최상위 트리 태그");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Idle, "State.Idle", "특정 액터의 상태가 어떠한 동작하지 않는 대기 상태인 경우 태그");
}