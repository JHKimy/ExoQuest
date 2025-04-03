#include "Enemy/Enemy1AnimInstance.h"
#include "Enemy/Enemy1.h"
#include "Components/CapsuleComponent.h"  // UCapsuleComponent 정의 추가


void UEnemy1AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
    enemyOwner = Cast<AEnemy1>(TryGetPawnOwner()); // ← 이거 반드시 필요!
}

void UEnemy1AnimInstance::OnEndAttackAnimation()
{
	bAttackPlay = false;
}

void UEnemy1AnimInstance::AnimNotify_E1AttackStart()
{
    if (enemyOwner)
    {
        enemyOwner->AttackCollision->SetGenerateOverlapEvents(true);
    }
    enemyOwner->bCanDamage = true;

}

void UEnemy1AnimInstance::AnimNotify_E1AttackEnd()
{
    if (enemyOwner)
    {
        enemyOwner->AttackCollision->SetGenerateOverlapEvents(false);
    }
    enemyOwner->bCanDamage = false;
}

void UEnemy1AnimInstance::PlayHitMontage()
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
