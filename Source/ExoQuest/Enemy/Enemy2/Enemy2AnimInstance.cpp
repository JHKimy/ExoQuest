#include "Enemy/Enemy2/Enemy2AnimInstance.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "Components/CapsuleComponent.h"  // UCapsuleComponent 정의 추가
#include "Enemy2AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

//void UEnemy2AnimInstance::SetIdleActionType(int num)
//{
//    IdleActionType =Cast<EIdleActionType> num;
//}

void UEnemy2AnimInstance::SetAnimState(EEnemyState newState)
{
    animState = newState;
}

void UEnemy2AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    enemyOwner = Cast<AEnemy2>(TryGetPawnOwner()); // ← 이거 반드시 필요!

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
