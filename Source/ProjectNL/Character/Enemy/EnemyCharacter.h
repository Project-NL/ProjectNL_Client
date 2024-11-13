// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectNL/Character/BaseCharacter.h"

#include "EnemyCharacter.generated.h"

class UPlayerAttributeSet;

/**
 * 
 */
UCLASS()
class PROJECTNL_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AEnemyCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	TObjectPtr<UPlayerAttributeSet> PlayerAttributeSet;
};
