#include "Enemy/Enemy1AnimInstance.h"

void UEnemy1AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}

void UEnemy1AnimInstance::OnEndAttackAnimation()
{
	bAttackPlay = false;
}
