#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthItem.generated.h"

UCLASS()
class EXOQUEST_API AHealthItem : public AActor
{
    GENERATED_BODY()

public:
    // ======================================
    // ������ �� �⺻ �Լ�
    AHealthItem();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    // ======================================
    // ������Ʈ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* meshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    class UCapsuleComponent* capsuleCollision;

    // ======================================
    // �ִϸ��̼� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float RiseSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float RiseHeight = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
    float MoveToCharacterSpeed = 5.0f;

    // ======================================
    // ������ ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (AllowPrivateAccess = "true"))
    int32 ItemNum;

    // ======================================
    // ĳ���� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
    bool IsCharacterNearby = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
    class ACharacterBase* character;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character", meta = (AllowPrivateAccess = "true"))
    float CheckInterval = 1.0f;

    // ======================================
    // Ÿ�̸� �� ��ġ ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (AllowPrivateAccess = "true"))
    FVector InitialLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (AllowPrivateAccess = "true"))
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position", meta = (AllowPrivateAccess = "true"))
    bool HasReachedTarget = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (AllowPrivateAccess = "true"))
    FTimerHandle DistanceCheckTimerHandle;

    // ======================================
    UFUNCTION()
    void CheckForNearbyCharacters();

    UFUNCTION()
    void AbsorbToCharacter();

    UFUNCTION()
    void OnBeginOverlap(
        UPrimitiveComponent* OverlappedComponent,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFormSweep,
        const FHitResult& SweepResult);
};
