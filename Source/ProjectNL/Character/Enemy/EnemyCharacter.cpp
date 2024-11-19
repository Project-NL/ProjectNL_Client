// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/Character/Enemy/EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "ProjectNL/GAS/Attribute/PlayerAttributeSet.h"
#include "ProjectNL/GAS/NLAbilitySystemComponent.h"
#include "ProjectNL/Player/BasePlayerState.h"


AEnemyCharacter::AEnemyCharacter()
{
	AbilitySystemComponent = CreateDefaultSubobject<UNLAbilitySystemComponent>(
	"Ability System Component");
	PlayerAttributeSet = CreateDefaultSubobject<UPlayerAttributeSet>(TEXT("AttributeSet"));
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if(AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
		PlayerAttributeSet->InitBaseAttribute();

		//AbilitySystemComponent->InitializeAbilitySystem(InitializeData);
		
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			PlayerAttributeSet->GetMovementSpeedAttribute()).AddUObject(
			this, &ThisClass::MovementSpeedChanged);
		
		Initialize();
	}
}

void AEnemyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

