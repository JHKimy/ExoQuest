#include "Enemy/Enemy2/Enemy2AnimInstance.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "Components/CapsuleComponent.h"  // UCapsuleComponent 정의 추가
#include "Enemy2AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/CharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy/Weapon/EnemyRifle.h"

//void UEnemy2AnimInstance::SetIdleActionType(int num)
//{
//    IdleActionType =Cast<EIdleActionType> num;
//}

void UEnemy2AnimInstance::SetAnimState(EEnemyState newState)
{
    animState = newState;
}

void UEnemy2AnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    APawn* OwnerPawn = TryGetPawnOwner();
    if (OwnerPawn)
    {
        enemyOwner = Cast<AEnemy2>(OwnerPawn);
    }
}

void UEnemy2AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    //// Enemy 캐릭터를 AnimInstance에서 가져오기
    //if (!enemyOwner)
    //{
    //    enemyOwner = Cast<AEnemy2>(TryGetPawnOwner());
    //}    
    //// 방어 코드 추가!
    //if (!enemyOwner || !enemyOwner->target)
    //{
    //    return;
    //}
    // 
    // 
    // 
    // 
    //
    //  
    if (!enemyOwner || !enemyOwner->target) return;
    // 1. Muzzle 위치
    FVector MuzzleLocation = enemyOwner->EnemyRifle->GetMuzzle()->GetComponentLocation();

    // 2. 타겟 위치
    FVector TargetLocation = enemyOwner->target->GetActorLocation();

    // 3. Muzzle에서 Target을 바라보는 회전값 계산
    FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TargetLocation);

    // 4. 총의 현재 회전 기준과 비교하려면...
    // FRotator GunRot = enemyOwner->rifle->GetActorRotation();
    // FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(LookRot, GunRot);

    // 5. 직접 Pitch만 사용할 경우
    AimPitch = LookRot.Pitch;
    UE_LOG(LogTemp, Warning, TEXT("AimPitch: %.2f"), AimPitch);

    //SetEnableIK();

    //AEnemy2AIController* AIController = Cast<AEnemy2AIController>(enemyOwner->GetController());
    //if (!AIController) return;

    //UBlackboardComponent* BB = AIController->GetBlackboardComponent();
    //if (!BB) return;

    //// IdleIndex 값을 가져와서 Enum으로 변환
    //int32 Index = BB->GetValueAsInt("IdleIndex");
    //IdleActionType = static_cast<EIdleActionType>(Index);


}


void UEnemy2AnimInstance::PlayHitMontage()
{
    if (HitMontage)
    {
        Montage_Play(HitMontage,2.f);
        //UE_LOG(LogTemp, Warning, TEXT("Playing Hit Montage"));
    }
    else
    {
        //UE_LOG(LogTemp, Error, TEXT("HitMontage is NULL!! Did you assign it in AnimBP?"));
    }
}

void UEnemy2AnimInstance::PlayFireMontage()
{
    if (FireMontage)
    {
        Montage_Play(FireMontage);
    }
}

//void UEnemy2AnimInstance::SetEnableIK()
//{
//    if (IdleActionType == EIdleActionType::Stretching) {
//        bEnableIK = false;
//    }
//    else {
//        true;
//    }
//    //bEnableIK = (IdleActionType != EIdleActionType::Stretching);
//}

void UEnemy2AnimInstance::SetLeftHandIKTransform(const FTransform& InTransform)
{
    LeftHandEffector = InTransform.GetLocation();
    LeftHandRotation = InTransform.Rotator();
}
