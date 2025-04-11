#include "ActorPoolManager.h"

AActorPoolManager::AActorPoolManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AActorPoolManager::BeginPlay()
{
    Super::BeginPlay();
}

AActor* AActorPoolManager::GetInactiveActorFromPool(UClass* ActorClass)
{
    if (PoolMap.Contains(ActorClass))
    {
        TArray<AActor*>& Pool = PoolMap[ActorClass];
        for (int32 i = 0; i < Pool.Num(); ++i)
        {
            AActor* Pooled = Pool[i];
            if (Pooled && !Pooled->IsActorTickEnabled())
            {
                Pool.RemoveAt(i);
                return Pooled;
            }
        }
    }

    return nullptr;
}

void AActorPoolManager::ReturnActorToPool(AActor* Actor)
{
    if (!Actor) return;

    // 상태 초기화
    Actor->SetActorHiddenInGame(true);
    Actor->SetActorEnableCollision(false);
    Actor->SetActorTickEnabled(false);

    // 풀에 추가
    PoolMap.FindOrAdd(Actor->GetClass()).Add(Actor);
}