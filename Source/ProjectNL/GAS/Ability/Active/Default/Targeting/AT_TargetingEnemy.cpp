// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectNL/GAS/Ability/Active/Default/Targeting/AT_TargetingEnemy.h"

#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectNL/Character/Enemy/EnemyCharacter.h"
#include "ProjectNL/Character/Player/PlayerCharacter.h"
#include "ProjectNL/Component/CameraComponent/PlayerCameraComponent.h"
#include "ProjectNL/Component/CameraComponent/PlayerSpringArmComponent.h"
#include "ProjectNL/GAS/Attribute/BaseAttributeSet.h"
#include "ProjectNL/Helper/GameplayTagHelper.h"

UAT_TargetingEnemy* UAT_TargetingEnemy::InitialEvent(UGameplayAbility* OwningAbility)
{
	UAT_TargetingEnemy* NewEvent = NewAbilityTask<UAT_TargetingEnemy>(
		OwningAbility);

	return NewEvent;
}

void UAT_TargetingEnemy::Activate()
{
	Super::Activate();
	bTickingTask = true;
	OriginalCharacterSpeed = Cast<ACharacter>(GetAvatarActor())->GetCharacterMovement()->MaxWalkSpeed;

	SaveCameraSettings(GetAvatarActor()->FindComponentByClass<UPlayerCameraComponent>());//카메라의 초기 세팅을 저장해준다 복구할 때 사용
	SaveSpringArmSettings(GetAvatarActor()->FindComponentByClass<UPlayerSpringArmComponent>()); 
	TargetNearestEnemy();
}


void UAT_TargetingEnemy::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (NearestEnemy && NearestEnemyCheck)
	{
		// 먼저, 플레이어 캐릭터의 AbilitySystemComponent에서 현재 활성화된 GameplayTag들을 가져옵니다.
		if (APlayerCharacter* PlayerChar = Cast<APlayerCharacter>(GetAvatarActor()))
		{
			if (UAbilitySystemComponent* ASC = PlayerChar->GetAbilitySystemComponent())
			{
				FGameplayTagContainer ActiveTags;
				ASC->GetOwnedGameplayTags(ActiveTags);

				//static const FGameplayTag DodgeTag = NlGameplayTags::Status_Sprint;
				for (const FGameplayTag& Tag : ActiveTags)
				{
					//UE_LOG(LogTemp, Log, TEXT("Active Tag: %s"), *Tag.ToString());
				}
				// "Status_dodge" 태그 활성화 여부 체크
				if (ActiveTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Sprint")))||ActiveTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Dodge")))||
					ActiveTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Status.Roll"))))
				{
					UE_LOG(LogTemp, Log, TEXT("Status_dodge 태그 활성화됨 -> PlayerContollerRotation 실행"));
					
				}
				else
				{
					
					LockOnTarget(NearestEnemy);
					UE_LOG(LogTemp, Log, TEXT("Status_dodge 태그 비활성화됨 -> PlayerContollerRotation 실행"));
				}	
			}
		}
		PlayerContollerRotation(DeltaTime);
		// LockOnTarget과 CameraRotation은 기존처럼 항상 실행
		CameraRotation(DeltaTime);
	}
	else
	{
		BacktoSquareOne(DeltaTime);
	}
}

void UAT_TargetingEnemy::TargetNearestEnemy()
{
	NearestEnemyCheck = !NearestEnemyCheck;
	NearestEnemy = FindNearestTarget();
	if (!NearestEnemyCheck)
	{
		//ReleaseLockOnTarget(); //어빌리티를 종료시킵니다.
	}
	else if (NearestEnemyCheck && NearestEnemy) //클릭과 타겟 캐릭터 상대방이 둘다 있을경우
	{
		APlayerController* PlayerController = Cast<
			APlayerController>(Cast<ACharacter>(GetAvatarActor())->GetController());
		PlayerController->SetIgnoreLookInput(true); // 회전 입력 비활성화
		Cast<ACharacter>(GetAvatarActor())->GetCharacterMovement()->MaxWalkSpeed = 250.0f; //속도 감소
	}
}


AActor* UAT_TargetingEnemy::FindNearestTarget() const
{
	FVector PlayerLocation = GetAvatarActor()->GetActorLocation();
	AActor* nearestEnemy = nullptr;
	float NearestDistance = TargetingRange;

	// 구체 반경 정의
	float SphereRadius = TargetingRange;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetAvatarActor()); // 플레이어 캐릭터는 무시
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;

	// 충돌 형상(구체) 정의
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SphereRadius);

	// 겹치는 액터들을 저장할 배열
	TArray<FOverlapResult> OverlapResults;

	UWorld* World = GetWorld();
	if (World)
	{
		// 구체 겹치기 수행
		bool bOverlapped = World->OverlapMultiByChannel(
			OverlapResults,
			PlayerLocation,
			FQuat::Identity,
			ECC_Pawn, // 적들이 Pawns이라고 가정; 필요에 따라 조정
			CollisionShape,
			QueryParams
		);

		// 겹친 액터가 있는 경우
		if (bOverlapped)
		{
			// 결과를 순회하면서 가장 가까운 적 찾기
			for (const FOverlapResult& Result : OverlapResults)
			{
				AEnemyCharacter* OverlappedActor = Cast<AEnemyCharacter>(Result.GetActor());
				if (OverlappedActor && OverlappedActor != GetAvatarActor())
				{
					float Distance = FVector::Dist(PlayerLocation, OverlappedActor->GetActorLocation());
					if (Distance < NearestDistance)
					{
						NearestDistance = Distance;
						nearestEnemy = OverlappedActor;
					}
				}
			}
		}
		// 디버그 스피어 그리기
		DrawDebugSphere(World, PlayerLocation, SphereRadius, 24, FColor::Green, false, 5.0f);

		if (nearestEnemy)
		{
			// 가장 가까운 적의 위치에 디버그 스피어 그리기
			DrawDebugSphere(World, nearestEnemy->GetActorLocation(), 50.0f, 24, FColor::Blue, false, 5.0f);
		}
	}


	return nearestEnemy;
}
void UAT_TargetingEnemy::PlayerContollerRotation(float DeltaTime)
{
	AActor* OwnerActor = GetAvatarActor();
	APlayerController* PlayerController = Cast<
		APlayerController>(Cast<ACharacter>(OwnerActor)->GetController());
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		OwnerActor->GetActorLocation(), NearestEnemy->GetActorLocation());
	const FRotator InterpRotation = UKismetMathLibrary::RInterpTo(PlayerController->GetControlRotation(),
																  LookAtRotation, DeltaTime, 10.f);


	PlayerController->SetControlRotation(FRotator(InterpRotation.Pitch, InterpRotation.Yaw,
												  PlayerController->GetControlRotation().Roll));

}

bool UAT_TargetingEnemy::BacktoSquareOne(float DeltaTime)
{
	
	AActor* AvatarActor = GetAvatarActor();
	RestoreCameraRotation(AvatarActor->FindComponentByClass<UPlayerCameraComponent>(),DeltaTime);
	//RestoreSpringArmRotation(AvatarActor->FindComponentByClass<UPlayerSpringArmComponent>(),DeltaTime);
	


	return true;
}

void UAT_TargetingEnemy::CameraRotation(float DeltaTime)
{
	AActor* OwnerActor = GetAvatarActor();
	APlayerController* PlayerController = Cast<
		APlayerController>(Cast<ACharacter>(OwnerActor)->GetController());
	const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(
		OwnerActor->GetActorLocation(), NearestEnemy->GetActorLocation());
	const FRotator InterpRotation = UKismetMathLibrary::RInterpTo(PlayerController->GetControlRotation(),
																  LookAtRotation, DeltaTime, 10.f);
	UPlayerSpringArmComponent* SpringArm = OwnerActor->FindComponentByClass<UPlayerSpringArmComponent>();
	if (SpringArm && CurrentTarget)
	{
		// 타겟과의 거리 계산
		FVector CameraLocation = SpringArm->GetComponentLocation();
		FVector TargetLocation = CurrentTarget->GetActorLocation();
		float Distance = FVector::Dist(CameraLocation, TargetLocation);

		// 거리에 따라 Pitch 조정 (거리 조정 상수 설정 가능)
		float MaxDistance = 700.0f; // 최대 거리 (원하는 대로 설정 가능)
		float MinPitch = -18.5f; // 최소 Pitch 각도
		float MaxPitch = -2.5f; // 최대 Pitch 각도

		// 거리를 비율로 계산하여 Pitch 각도 보간
		float NormalizedDistance = FMath::Clamp(Distance / MaxDistance, 0.0f, 1.0f);
		float DynamicPitch = FMath::Lerp(MinPitch, MaxPitch, NormalizedDistance);

		// 계산된 DynamicPitch로 회전 설정
		FRotator TargetRotation(DynamicPitch, InterpRotation.Yaw, 0.0f);
		SpringArm->SetWorldRotation(TargetRotation);

		SpringArm->TargetArmLength = 400.0f; // 카메라와 캐릭터 간 거리
		SpringArm->bUsePawnControlRotation = false; // 플레이어 컨트롤러의 회전 사용 안함
		SpringArm->bEnableCameraLag = true; // 카메라 랙 활성화
		SpringArm->CameraLagSpeed = 1.3f; // 카메라 랙 속도
		SpringArm->CameraLagMaxDistance = 100.0f;
	}
	UPlayerCameraComponent* Camera = OwnerActor->FindComponentByClass<UPlayerCameraComponent>();
	if (Camera && CurrentTarget)
	{
		FRotator CurrentCameraRotation = Camera->GetComponentRotation();
	
		// 타겟의 위치를 바라보는 회전 계산
		FVector TargetLocation = CurrentTarget->GetActorLocation();
		FVector CameraLocation = Camera->GetComponentLocation();
		FRotator CameraLookAtRotation = UKismetMathLibrary::FindLookAtRotation(CameraLocation, TargetLocation);
	
		// 보간하여 카메라 회전 설정
		FRotator Rotation = FMath::RInterpTo(CurrentCameraRotation, CameraLookAtRotation, DeltaTime, 20.0f);
	
		Camera->SetWorldRotation(Rotation);
	}
}

void UAT_TargetingEnemy::ReleaseLockOnTarget()
{
	NearestEnemyCheck = false;
	AActor* OwnerActor = GetAvatarActor();
	//캐릭터 속도를 다시 높임
	float BaseSpeed=Cast<UBaseAttributeSet>(Cast<APlayerCharacter>(GetAvatarActor())->GetAbilitySystemComponent()->GetAttributeSet(UBaseAttributeSet::StaticClass()))->GetMovementSpeed();
	Cast<ACharacter>(GetAvatarActor())->GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
	APlayerController* PlayerController = Cast<APlayerController>(Cast<ACharacter>(OwnerActor)->GetController());
	if(PlayerController)
	{
		// 플레이어 컨트롤러의 회전 입력을 다시 활성화
		PlayerController->SetIgnoreLookInput(false);
		// 플레이어 컨트롤러의 뷰 타겟을 플레이어로 설정 (필요시)
		PlayerController->SetViewTarget(OwnerActor);
	}
	if(UPlayerSpringArmComponent* playerspringarm=GetAvatarActor()->FindComponentByClass<UPlayerSpringArmComponent>())
	{
	//	RestoreSpringArmSettings(playerspringarm);
	}
	//RestoreCameraSettings(GetAvatarActor()->FindComponentByClass<UPlayerCameraComponent>());
	OnCanceled.Broadcast();
}

void UAT_TargetingEnemy::LockOnTarget(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
	if (CurrentTarget)
	{
		AActor* OwnerActor = GetAvatarActor();
		if (OwnerActor)
		{
			FVector TargetLocation = CurrentTarget->GetActorLocation();
			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(
				OwnerActor->GetActorLocation(), TargetLocation);
			NewRotation.Pitch = 0.0f;
			OwnerActor->SetActorRotation(NewRotation);
			UWorld* World = GetWorld();
			if (CurrentTarget && World)
			{
				// 가장 가까운 적의 위치에 디버그 스피어 그리기
				DrawDebugSphere(World, CurrentTarget->GetActorLocation(), 50.0f, 24, FColor::Blue, false, 0.1f);
				DrawDebugLine(World, GetAvatarActor()->GetActorLocation(), CurrentTarget->GetActorLocation(),
				              FColor::Yellow,
				              false, 0.01f, 0, 2.0f);
			}
		}
	}
}

void UAT_TargetingEnemy::SaveSpringArmSettings(USpringArmComponent* SpringArm)
{
	if (SpringArm)
	{
		SavedRotation = SpringArm->GetRelativeRotation();
		SavedLocation = SpringArm->GetRelativeLocation();
		SavedTargetArmLength = SpringArm->TargetArmLength;
		bSavedUsePawnControlRotation = SpringArm->bUsePawnControlRotation;
		bSavedInheritPitch = SpringArm->bInheritPitch;
		bSavedInheritYaw = SpringArm->bInheritYaw;
		bSavedInheritRoll = SpringArm->bInheritRoll;
		bSavedEnableCameraLag = SpringArm->bEnableCameraLag;

		// 3. 회전 설정
		//    - 단순히 캐릭터를 ‘정면’에서 바라보는 것이 아니라
		//      캐릭터 ‘뒤’에서 바라보려면 캐릭터의 Yaw에 180도 더해주는 식을 자주 사용합니다.
		SavedSpringArmWorldRotation = GetAvatarActor()->GetActorRotation();
		SavedSpringArmWorldRotation.Yaw += 180.0f; // 뒤에서 바라보도록 Yaw를 180도 추가
	}
}

void UAT_TargetingEnemy::SaveCameraSettings(UPlayerCameraComponent* CameraComponent)
{
	// 1. 타겟(캐릭터)의 위치를 구한다.
	FVector TargetLocation = GetAvatarActor()->GetActorLocation();
	FVector ForwardVector =  GetAvatarActor()->GetActorForwardVector();

	// 1) 카메라 위치 설정
	//    - 캐릭터를 바라보는 방향의 반대(-ForwardVector)로 400만큼 뒤로 이동
	//    - Z축(Up)으로 200만큼 올립니다.
	 SavedCameraWorldLocation = TargetLocation 
		- (ForwardVector * 400.0f) // 뒤로 400
		+ FVector(0.0f, 0.0f, 200.0f); // 위로 200
	SavedCameraWorldRotation = UKismetMathLibrary::FindLookAtRotation(
	  SavedCameraWorldLocation, 
	  TargetLocation
  );
	
}


void UAT_TargetingEnemy::RestoreSpringArmSettings(USpringArmComponent* SpringArm)
{
	if (SpringArm)
	{
		SpringArm->TargetArmLength = SavedTargetArmLength;
		SpringArm->bUsePawnControlRotation = bSavedUsePawnControlRotation;
		SpringArm->bInheritPitch = bSavedInheritPitch;
		SpringArm->bInheritYaw = bSavedInheritYaw;
		SpringArm->bInheritRoll = bSavedInheritRoll;
		SpringArm->bEnableCameraLag = true;
		SpringArm->SetRelativeRotation(SavedRotation);
		SpringArm->CameraLagMaxDistance = 50.0f;
	//	SpringArm->SetRelativeLocation(SavedLocation);
	}
}
void UAT_TargetingEnemy::RestoreCameraRotation(UPlayerCameraComponent* CameraComponent, float DeltaTime)
{
	AActor* AvatarActor = GetAvatarActor();
	if (!AvatarActor || !CameraComponent)
	{
		return;
	}
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	// if (PC)
	// {
	// 	// 카메라가 보간되는 동안 마우스 Look 입력을 무시
	// 	PC->SetIgnoreLookInput(true);
	// }

	// 1. 캐릭터 위치 / Forward Vector 구하기
	FVector TargetLocation = AvatarActor->GetActorLocation();
	FVector ForwardVector = AvatarActor->GetActorForwardVector();

	// 2. 목표 카메라 위치(DesiredLocation) 계산
	//    캐릭터 바라보는 반대 방향(-ForwardVector)으로 400 뒤, 위로 200
	FVector DesiredLocation = TargetLocation
						   - (ForwardVector * 400.0f)
						   + FVector(0.0f, 0.0f, 200.0f);

	// 3. 카메라 현재 위치 / 목표 위치 보간(VInterpTo)
	FVector CurrentLocation = CameraComponent->GetComponentLocation();
	FVector InterpLocation  = FMath::VInterpTo(
		CurrentLocation,    // 시작
		SavedCameraWorldLocation,    // 목표
		DeltaTime,          // 델타타임
		5.0f                // 보간 속도(값 조절 가능)
	);
	CameraComponent->SetWorldLocation(InterpLocation);

	// 4. 목표 회전(DesiredRotation) 계산(캐릭터를 바라보는 방향)
	FRotator CurrentRotation = CameraComponent->GetComponentRotation();
	FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(
		CurrentLocation, // 카메라(보간된) 위치
		TargetLocation  // 캐릭터 위치
	);

	// 5. 회전도 보간(RInterpTo)
	FRotator InterpRotation = FMath::RInterpTo(
		CurrentRotation,  // 시작 회전
		DesiredRotation,  // 목표 회전
		DeltaTime,
		5.0f              // 보간 속도(값 조절 가능)
	);
	CameraComponent->SetWorldRotation(DesiredRotation);

	
	// 1) FRotator::Equals 사용
	//    두 회전이 (Yaw, Pitch, Roll) 각각 오차 1e-3(0.001) 이내면 같은 회전으로 봄
	if ( InterpLocation.Equals(SavedCameraWorldLocation, 0.8f))
	{
		UE_LOG(LogTemp, Log, TEXT("InterpLocation 성공적"));
		ReleaseLockOnTarget();
	}
}

void UAT_TargetingEnemy::RestoreSpringArmRotation(USpringArmComponent* SpringArm,float DeltaTime)
{
	// 캐릭터의 위치와 Forward Vector를 구합니다.
	AActor* AvatarActor = GetAvatarActor();

	USpringArmComponent* SpringArmComp = AvatarActor->FindComponentByClass<USpringArmComponent>();
	if (!SpringArmComp)
	{
		return ;
	}

	// 2. SpringArm 길과 Offset 설정
	//    - 캐릭터 뒤로 400, 위로 200 지점
	SpringArmComp->TargetArmLength = 400.0f;                  // 뒤로 400
	SpringArmComp->SocketOffset    = FVector(0.f, 0.f, 200.f); // 위로 200

	

	// 3-1. 부드러운 회전을 원한다면 보간(RInterpTo) 사용
	FRotator CurrentRotation = SpringArmComp->GetComponentRotation();
	FRotator SpringInterpRotation     = FMath::RInterpTo(CurrentRotation, SavedSpringArmWorldRotation, DeltaTime, 5.0f);
	SpringArmComp->SetWorldRotation(SpringInterpRotation);
}