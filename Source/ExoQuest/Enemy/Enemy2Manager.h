#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy2Manager.generated.h"

UCLASS()
class EXOQUEST_API AEnemy2Manager : public AActor
{
    GENERATED_BODY()

public:
    AEnemy2Manager();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    void TrySpawnEnemy();

public:
    // 등록된 살아있는 적들
    UPROPERTY()
    TArray<class AEnemy2*> AliveEnemies;

    // 스폰할 적 클래스
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<class AEnemy2> EnemyClass;

    // 최대 스폰 수
    UPROPERTY(EditAnywhere, Category = "Spawning")
    int32 MaxEnemyCount = 10;

    // 스폰 반경
    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnRadius = 2000.f;

    // 스폰 간격
    UPROPERTY(EditAnywhere, Category = "Spawning")
    float SpawnInterval = 5.f;

private:
    float SpawnTimer = 0.f;

public:
    // 적 사망 시 호출됨
    UFUNCTION()
    void UnregisterEnemy(class AEnemy2* Enemy);
    UFUNCTION()
    void OnEnemyDestroyed(AActor* DestroyedActor);
};
