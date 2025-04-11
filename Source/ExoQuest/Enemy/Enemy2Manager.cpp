#include "Enemy2Manager.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Enemy2/Enemy2.h"
#include "ObjectPool/ActorPoolManager.h"

AEnemy2Manager::AEnemy2Manager()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AEnemy2Manager::BeginPlay()
{
    Super::BeginPlay();

    auto Pool = Cast<AActorPoolManager>(
        UGameplayStatics::GetActorOfClass(GetWorld(), AActorPoolManager::StaticClass()));

    if (!Pool || !EnemyClass) return;

    for (int i = 0; i < 10; ++i)
    {
        FVector Origin = GetActorLocation();
        FVector RandomOffset = FMath::VRand() * FMath::FRandRange(500.f, SpawnRadius);
        FVector SpawnLocation = Origin + RandomOffset;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        AEnemy2* Enemy = Pool->SpawnActorFromPool<AEnemy2>(EnemyClass, SpawnLocation, SpawnRotation);
        if (Enemy)
        {
            AliveEnemies.Add(Enemy);

            Enemy->SpawnDefaultController(); // AIController 부착
            Enemy->OnDestroyed.AddDynamic(this, &AEnemy2Manager::OnEnemyDestroyed);
        }
    }
}

void AEnemy2Manager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    SpawnTimer += DeltaTime;
    if (SpawnTimer >= SpawnInterval)
    {
        TrySpawnEnemy();
        SpawnTimer = 0.f;
    }
}

void AEnemy2Manager::TrySpawnEnemy()
{
    if (!EnemyClass) return;

    if (AliveEnemies.Num() >= MaxEnemyCount)
        return;

    auto Pool = Cast<AActorPoolManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AActorPoolManager::StaticClass()));
    if (!Pool) return;

    FVector Origin = GetActorLocation();
    FVector RandomOffset = FMath::VRand() * FMath::FRandRange(500.f, SpawnRadius);
    FVector SpawnLocation = Origin + RandomOffset;
    FRotator SpawnRotation = FRotator::ZeroRotator;

    AEnemy2* Enemy = Pool->SpawnActorFromPool<AEnemy2>(EnemyClass, SpawnLocation, SpawnRotation);
    if (Enemy)
    {
        AliveEnemies.Add(Enemy);

        // Enemy에게 매니저 자신 전달 (Unregister 위해)
        Enemy->SpawnDefaultController(); // AIController 보장

        // 아래처럼 Enemy에 Manager 참조 넘겨도 됨
        Enemy->OnDestroyed.AddDynamic(this, &AEnemy2Manager::OnEnemyDestroyed);
    }
}

void AEnemy2Manager::OnEnemyDestroyed(AActor* DestroyedActor)
{
    AEnemy2* Enemy = Cast<AEnemy2>(DestroyedActor);
    if (Enemy)
    {
        UnregisterEnemy(Enemy);
    }
}

void AEnemy2Manager::UnregisterEnemy(AEnemy2* Enemy)
{
    AliveEnemies.Remove(Enemy);
}