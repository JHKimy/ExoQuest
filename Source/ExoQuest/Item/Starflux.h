#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Starflux.generated.h"


// ������ ��� ����
UENUM(BlueprintType)
enum class EStarfluxState : uint8
{
    Pop,        // ���� �ö���� ����
    Floating,   // ���� ����
    Absorbing   // ��� ����
};

UCLASS(Blueprintable)
class EXOQUEST_API AStarflux : public AActor
{
    GENERATED_BODY()

public:
    // ������
    AStarflux();

protected:
    // �ʱ�ȭ �Լ�
    virtual void BeginPlay() override;

public:
    // �� ������ ȣ�� �Լ�
    virtual void Tick(float DeltaTime) override;

protected:
    // ======= �浹ü & �޽� =======

    // �浹ü
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* capsuleComp;

    // �޽�
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeshComp")
    class UStaticMeshComponent* meshComp;

    // ======= �ִϸ��̼� ���� ���� =======

    // ���� ������ ����
    EStarfluxState CurrentState = EStarfluxState::Pop;

    // ���� ��ġ
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Motion")
    FVector StartPos;

    // �ֱ�
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Motion")
    float Amplitude{ 15.f };

    // ȸ�� �ӵ�
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Motion")
    float RotationSpeed{ 1.f };

    // �ִϸ��̼� ���� �ð�
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Motion")
    float ElapsedTime{ 0.f };

    // �˾� ȿ�� Ȱ��ȭ ����
    bool bIsPopping;

    // ���� ��ġ �Ʒ� ����
    FVector StartBelowLocation;

    // ======= ������ ���� ���� =======

    // ������ �̸�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName{ TEXT("StarFlux") };

    // ������ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemNum{ 1 };

    // ======= Ÿ�̸� �� ��ó ���� ���� =======

    // Ÿ�̸� �ڵ�
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (AllowPrivateAccess = "true"))
    FTimerHandle DistanceCheckTimerHandle;

    // üũ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float CheckInterval = 1.f;

    // ĳ���� ��ó ����
    bool IsCharacterNearby = false;

    // ��� ���� ����
    bool bCanAbsorb = true;

    // ��� �ð� ����
    float TimeSinceStart = 0.0f;

    // ĳ���� ����
    class ACharacterBase* character;

    // ĳ���Ϳ� ������ �Ÿ�
    float distanceWithCharacter{ 100.f };

    float MoveToCharacterSpeed{ 7.f };

    float AbsorptionDistance{ 500.f };

    // ======= ��� �Լ� =======

public:
    // �˾� �ִϸ��̼�
    UFUNCTION()
    void Pop(float DeltaTime);

    // ���� ȿ��
    UFUNCTION()
    void Floating(float DeltaTime);

    // �浹 �̺�Ʈ ó��
    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // ��ó ĳ���� üũ
    void CheckForNearbyCharacters();

    // ĳ���� ��� ó��
    void AbsorbToCharacter(float DeltaTime);
};
