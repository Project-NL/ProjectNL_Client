#pragma once

class FEnumHelper
{
public:
	template <typename EnumType>
	static FString GetClassEnumKeyAsString(const EnumType EnumeratorValue)
	{
		TArray<FString> EnumKey;
		UEnum::GetValueAsString(EnumeratorValue).
			ParseIntoArray(EnumKey, TEXT("::"));

		if (EnumKey.Num() == 1)
		{
			return EnumKey[0];
		}
		return EnumKey[1];
	}
};

UENUM(BlueprintType)
enum class EEntityCategory: uint8
{
	Undefined UMETA(DisplayName = "Undefined")
	, Player UMETA(DisplayName = "Player"), Neutral UMETA(DisplayName = "Neutral")
	, Enemy UMETA(DisplayName = "Enemy"),
};

UENUM(BlueprintType)
enum class EInputIDType: uint8
{
	None UMETA(DisplayName = "None"), Jump UMETA(DisplayName = "Jump")
	, Attack UMETA(DisplayName = "Attack"), Sprint UMETA(DisplayName = "Sprint")
	, ToggleCombatMode UMETA(DisplayName = "ToggleCombatMode")
	, FixedCamera UMETA(DisplayName = "FixedCamera")
	, Skill1 UMETA(DisplayName = "Skill1"), Skill2 UMETA(DisplayName = "Skill2")
	, Skill3 UMETA(DisplayName = "Skill3"), Skill4 UMETA(DisplayName = "Skill4")};