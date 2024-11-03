// Fill out your copyright notice in the Description page of Project Settings.


#include "EnableCollisionNotifyState.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectNL/Character/BaseCharacter.h"
#include "ProjectNL/Component/EquipComponent/EquipComponent.h"
#include "ProjectNL/Weapon/BaseWeapon.h"

void UEnableCollisionNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
    UE_LOG(LogTemp, Log, TEXT("Notify Begin"));

    // 애니메이션 시작 시 이전 프레임의 소켓 위치 초기화
    PrevStartLocation = FVector::ZeroVector;
    PrevEndLocation = FVector::ZeroVector;
}

void UEnableCollisionNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime)
{
    if (AActor* Owner = MeshComp->GetOwner())
    {
        MakeTriangleTrace(Owner);
    }
}

void UEnableCollisionNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (AActor* Owner = MeshComp->GetOwner())
    {
      {
            HitActors.Reset();
        }
    }
    UE_LOG(LogTemp, Log, TEXT("Notify End"));
}

FVector BezierCurve(const FVector& P0, const FVector& P1, const FVector& P2, float t)
{
    // t는 곡선에서의 위치를 나타내며, 0 <= t <= 1
    float t2 = t * t;                  // t^2 계산
    float oneMinusT = 1.0f - t;        // (1 - t) 계산

    // 2차 베지어 곡선 공식: B(t) = (1 - t)^2 * P0 + 2 * (1 - t) * t * P1 + t^2 * P2
    FVector point = (oneMinusT * oneMinusT) * P0;  // (1 - t)^2 * P0: 시작점에서 곡선이 시작하게 만듦
    point += 2 * oneMinusT * t * P1;               // 2 * (1 - t) * t * P1: 제어점의 영향을 적용하여 곡선의 형태를 조정
    point += t2 * P2;                              // t^2 * P2: 끝점에 도달하게 만듦
    
    return point;
}
void UEnableCollisionNotifyState::MakeTriangleTrace(AActor* Owner)
{
    
    ABaseCharacter* BaseCharacter= Cast<ABaseCharacter>(Owner);
    UEquipComponent* EquipComponent{};
    if(BaseCharacter)
    {
        EquipComponent= BaseCharacter->GetEquipComponent();
    }
    ABaseWeapon* MainWeapon{};
    if(EquipComponent)
        MainWeapon=EquipComponent->GetMainWeapon();
    if (MainWeapon)
    {
        USkeletalMeshComponent* SwordMesh = MainWeapon->GetWeaponSkeleton();

        if (SwordMesh && SwordMesh->DoesSocketExist(FName("SwordBoneStart")) && SwordMesh->DoesSocketExist(FName("SwordBoneEnd")))
        {
            FVector CurrentStartLocation = SwordMesh->GetSocketLocation(FName("SwordBoneStart"));
            FVector CurrentEndLocation = SwordMesh->GetSocketLocation(FName("SwordBoneEnd"));

            if (PrevStartLocation == FVector::ZeroVector && PrevEndLocation == FVector::ZeroVector)
            {
                PrevStartLocation = CurrentStartLocation;
                PrevEndLocation = CurrentEndLocation;
                return;
            }

            // 충돌 검사할 액터들을 담을 배열
            TArray<FHitResult> HitResults;

            // Start와 End 위치의 중간 지점 계산
            FVector PrevMiddleLocation = (PrevStartLocation + PrevEndLocation) / 2;
            FVector CurrentMiddleLocation = (CurrentStartLocation + CurrentEndLocation) / 2;


            // 보간을 위해 이전 지점과 현재 지점 사이의 거리 계산
            float DistanceStart = FVector::Dist(PrevStartLocation, CurrentStartLocation);
            float DistanceEnd = FVector::Dist(PrevEndLocation, CurrentEndLocation);

            FVector EndDirection = (CurrentEndLocation - PrevEndLocation).GetSafeNormal();
            FVector StartDirection = (CurrentStartLocation - PrevStartLocation).GetSafeNormal();
            float DotProduct = FVector::DotProduct(EndDirection, StartDirection);
            float AngleRadians = FMath::Acos(DotProduct);

            // 라디안 값을 도 단위로 변환
            float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
            float AngleRatio =  (AngleDegrees)/45;
  
            // 보간할 간격(거리가 일정 이상일 경우 보간 수행)
            float InterpolationStep = 5.0f; // 간격을 설정 

            float NumStepsStart = ((DistanceStart/10) + AngleRatio);
            float NumStepsEnd = ((DistanceEnd /10)+ AngleRatio);
            int32 NumSteps = FMath::CeilToInt(DistanceStart / InterpolationStep);
          
          
            // 2차 베지어 곡선을 위한 제어점 계산
            FVector ControlPointStart = (PrevStartLocation + CurrentStartLocation) / 2 ; // 임의로 위로 올린 제어점
            FVector ControlPointEnd = (PrevEndLocation + CurrentEndLocation) / 2 ;
            FVector ControlPointDirection = (ControlPointStart - ControlPointEnd).GetSafeNormal();
            ControlPointStart=ControlPointStart+ControlPointDirection*-NumStepsStart;
            ControlPointEnd=ControlPointEnd+ControlPointDirection*-NumStepsEnd;
            //NumStepsStart=10;
            // 보간을 이용한 중간 지점들을 생성하여 충돌 처리 
            FVector InterPrevStartLocation;
            FVector InterPrevEndLocation;
            for (int32 i = 1; i <= NumSteps; i++)
            {
                float Alpha = i / static_cast<float>(NumSteps);
      
                FVector InterpolatedStart = BezierCurve(PrevStartLocation, ControlPointStart, CurrentStartLocation, Alpha);
                FVector InterpolatedEnd=BezierCurve(PrevEndLocation,ControlPointEnd,CurrentEndLocation,Alpha);
                FVector InterpolatedMiddle = (InterpolatedStart + InterpolatedEnd) / 2;
                // 2차 베지어 곡선을 위한 제어점 계산
                if (InterPrevStartLocation != FVector::ZeroVector && InterPrevStartLocation != FVector::ZeroVector)
                {
                    FVector InterPrevMiddelLocation=(InterPrevStartLocation+InterPrevEndLocation)/2;
                    // 첫 번째 삼각형: PrevStart -> CurrentStart -> PrevEnd
                    PerformTriangleTrace(Owner, InterPrevStartLocation, InterpolatedStart, InterPrevMiddelLocation, HitResults);

                    // 두 번째 삼각형: CurrentStart -> CurrentEnd -> PrevEnd
                    PerformTriangleTrace(Owner, InterpolatedStart, InterpolatedMiddle, InterPrevMiddelLocation, HitResults);

                    // 추가 삼각형 1: 이전 Start, 현재 Start, 이전 Middle을 연결하는 삼각형
                    PerformTriangleTrace(Owner, InterPrevMiddelLocation, InterpolatedMiddle, InterPrevEndLocation, HitResults);

                    // 추가 삼각형 2: 현재 Start, 현재 Middle, 이전 Middle을 연결하는 삼각형
                    PerformTriangleTrace(Owner, InterpolatedMiddle, InterPrevEndLocation, InterpolatedMiddle, HitResults);
                }
                

                // 보간된 지점에서 삼각형 충돌 처리
                    PerformTriangleTrace(Owner, InterpolatedStart, InterpolatedEnd, PrevEndLocation, HitResults);
                
                    // 디버그 라인 그리기
                    DrawDebugLine(Owner->GetWorld(), InterpolatedStart, InterpolatedEnd, FColor::Yellow, false, 2.0f);
                  //  //디버그 스피어 그리기 
                    DrawDebugSphere(Owner->GetWorld(), InterpolatedStart, 2.0f, 12, FColor::Yellow, false, 2.0f); // 현재 프레임 시작지점
                    DrawDebugSphere(Owner->GetWorld(), InterpolatedEnd, 2.0f, 12, FColor::Yellow, false, 2.0f); // 현재 프레임 끝지점
                    InterPrevStartLocation=InterpolatedStart;
                     InterPrevStartLocation=InterpolatedEnd;
               
            }
            
            // 첫 번째 삼각형: PrevStart -> CurrentStart -> PrevMiddle
            PerformTriangleTrace(Owner, PrevStartLocation, CurrentStartLocation, PrevMiddleLocation, HitResults);

            // 두 번째 삼각형: CurrentStart -> CurrentMiddle -> PrevMiddle
            PerformTriangleTrace(Owner, CurrentStartLocation, CurrentMiddleLocation, PrevMiddleLocation, HitResults);

            // 추가 삼각형 1: PrevMiddle -> CurrentMiddle -> PrevEnd
            PerformTriangleTrace(Owner, PrevMiddleLocation, CurrentMiddleLocation, PrevEndLocation, HitResults);

            // 추가 삼각형 2: CurrentStartLocation -> PrevEnd -> CurrentEnd
            PerformTriangleTrace(Owner, CurrentMiddleLocation, PrevEndLocation, CurrentEndLocation, HitResults);


            // 각 충돌된 액터에 대해 처리
            for (const FHitResult& Hit : HitResults)
            {
                if (AActor* HitActor = Hit.GetActor())
                {
                    if (!HitActors.Contains(HitActor))
                    {
                        HitActors.Add(HitActor);
                        ABaseCharacter* Character=Cast<ABaseCharacter>(HitActor);
                        // 적에게 충돌 시 효과 적용
                        UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Character);
                        if(ASC)
                        {
                            float PlayRate = 1.0f;
                            // ISLInteractionInterface* HitIslInteractionInterface = Cast<ISLInteractionInterface>(
                            //     HitActor);
                            // HitIslInteractionInterface->NotifyHitInteraction(Hit.ImpactPoint);
                        }
                        if (ASC && AttackDamageEffect)
                        {
                            
                            FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
                            EffectContext.AddSourceObject(Owner);

                            FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(AttackDamageEffect, 1.0f, EffectContext);
                            if (SpecHandle.IsValid())
                            {
                                ASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), ASC);
                            }
                            // 충돌 지점 시각화
                            DrawDebugSphere(Owner->GetWorld(), Hit.ImpactPoint, 10, 12, FColor::Yellow, false, 1.0f);
                        }

                       
                    }
                }
            }

            // 디버그 라인 그리기
             DrawDebugLine(Owner->GetWorld(), PrevStartLocation, CurrentStartLocation, FColor::Blue, false, 2.0f);
             DrawDebugLine(Owner->GetWorld(), PrevEndLocation, CurrentEndLocation, FColor::Blue, false, 2.0f);
             DrawDebugLine(Owner->GetWorld(), PrevStartLocation, PrevEndLocation, FColor::Blue, false, 2.0f);
             DrawDebugLine(Owner->GetWorld(), CurrentStartLocation, CurrentEndLocation, FColor::Blue, false, 2.0f);
            //
            // // // 추가 디버그 라인: Middle 지점 연결
            // // DrawDebugLine(Owner->GetWorld(), PrevMiddleLocation, CurrentMiddleLocation, FColor::Blue, false, 2.0f);
            // // DrawDebugLine(Owner->GetWorld(), CurrentStartLocation, PrevMiddleLocation, FColor::Blue, false, 2.0f);
            // // DrawDebugLine(Owner->GetWorld(), CurrentMiddleLocation, PrevEndLocation, FColor::Blue, false, 2.0f);
            //
             DrawDebugSphere(Owner->GetWorld(), PrevStartLocation, 2.3f, 12, FColor::Red, false, 2.0f); // 현재 프레임 시작지점
             DrawDebugSphere(Owner->GetWorld(), PrevEndLocation, 2.3f, 12, FColor::Red, false, 2.0f); // 현재 프레임 끝지점
             DrawDebugSphere(Owner->GetWorld(), CurrentStartLocation, 2.3f, 12, FColor::Red, false, 2.0f); // 현재 프레임 시작지점
             DrawDebugSphere(Owner->GetWorld(), CurrentEndLocation, 2.3f, 12, FColor::Red, false, 2.0f); // 현재 프레임 끝지점
            // 현재 프레임 위치를 이전 프레임 위치로 업데이트
            PrevStartLocation = CurrentStartLocation;
            PrevEndLocation = CurrentEndLocation;
         
        }
    }
}
void UEnableCollisionNotifyState::PerformTriangleTrace(AActor* Owner, FVector Point1, FVector Point2, FVector Point3, TArray<FHitResult>& OutHitResults)
{
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Owner);

    // 각 변을 라인 트레이스로 검사
    Owner->GetWorld()->LineTraceMultiByChannel(OutHitResults, Point1, Point2, ECC_WorldDynamic, CollisionParams);
    Owner->GetWorld()->LineTraceMultiByChannel(OutHitResults, Point2, Point3, ECC_WorldDynamic, CollisionParams);
    Owner->GetWorld()->LineTraceMultiByChannel(OutHitResults, Point3, Point1, ECC_WorldDynamic, CollisionParams);
}
