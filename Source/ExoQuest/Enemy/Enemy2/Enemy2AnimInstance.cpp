#include "Enemy/Enemy2/Enemy2AnimInstance.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "Components/CapsuleComponent.h"  // UCapsuleComponent 정의 추가


void UEnemy2AnimInstance::SetAnimState(EEnemyState newState)
{
    animState = newState;
}

void UEnemy2AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);
    enemyOwner = Cast<AEnemy2>(TryGetPawnOwner()); // ← 이거 반드시 필요!
}


void UEnemy2AnimInstance::PlayHitMontage()
{
    if (HitMontage)
    {
        Montage_Play(HitMontage);
        UE_LOG(LogTemp, Warning, TEXT("Playing Hit Montage"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HitMontage is NULL!! Did you assign it in AnimBP?"));
    }
}
