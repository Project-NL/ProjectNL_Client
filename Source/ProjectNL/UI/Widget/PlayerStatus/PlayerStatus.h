#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatus.generated.h"

class UValuePercentBar;
struct FOnAttributeChangeData;

UCLASS()
class PROJECTNL_API UPlayerStatus : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void HandleCurrentHealthChanged(const FOnAttributeChangeData& Data);
	void HandleMaxHealthChanged(const FOnAttributeChangeData& Data);
	void HandleCurrentManaChanged(const FOnAttributeChangeData& Data);
	void HandleMaxManaChanged(const FOnAttributeChangeData& Data);
	void HandleCurrentStaminaChanged(const FOnAttributeChangeData& Data);
	void HandleMaxStaminaChanged(const FOnAttributeChangeData& Data);
	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UValuePercentBar> HealthBar;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UValuePercentBar> ManaBar;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UValuePercentBar> StaminaBar;
};
