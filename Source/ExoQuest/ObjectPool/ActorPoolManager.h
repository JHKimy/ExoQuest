#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActorPoolManager.generated.h"

UCLASS()
class EXOQUEST_API AActorPoolManager : public AActor
{
    GENERATED_BODY()

public:
    AActorPoolManager();

    virtual void BeginPlay() override;

    // 원하는 클래스의 액터 꺼내오기
    template <typename T>
    T* SpawnActorFromPool(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation);

    // 액터 되돌리기
    void ReturnActorToPool(AActor* Actor);

private:
    // 클래스별 풀 맵
    TMap<UClass*, TArray<AActor*>> PoolMap;

    // 재사용 가능한 액터 반환
    AActor* GetInactiveActorFromPool(UClass* ActorClass);
};


template <typename T>
T* AActorPoolManager::SpawnActorFromPool(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation)
{
    if (!ActorClass) return nullptr;

    AActor* Actor = GetInactiveActorFromPool(ActorClass);
    if (!Actor)
    {
        // 새로 생성
        Actor = GetWorld()->SpawnActor<AActor>(ActorClass, Location, Rotation);
    }
    else
    {
        // 위치 재설정
        Actor->SetActorLocation(Location);
        Actor->SetActorRotation(Rotation);
    }

    // 공통 초기화
    Actor->SetActorHiddenInGame(false);
    Actor->SetActorEnableCollision(true);
    Actor->SetActorTickEnabled(true);

    return Cast<T>(Actor);
}
