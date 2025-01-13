#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Starflux.generated.h"


// 움직임 모드 정의
UENUM(BlueprintType)
enum class EStarfluxState : uint8
{
    Pop,        // 위로 올라오는 상태
    Floating,   // 부유 상태
    Absorbing   // 흡수 상태
};

UCLASS(Blueprintable)
class EXOQUEST_API AStarflux : public AActor
{
    GENERATED_BODY()

public:
    // 생성자
    AStarflux();

protected:
    // 초기화 함수
    virtual void BeginPlay() override;

public:
    // 매 프레임 호출 함수
    virtual void Tick(float DeltaTime) override;

protected:
    // ======= 충돌체 & 메쉬 =======

    // 충돌체
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
    class UCapsuleComponent* capsuleComp;

    // 메쉬
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MeshComp")
    class UStaticMeshComponent* meshComp;

    // ======= 애니메이션 관련 변수 =======

    // 현재 움직임 상태
    EStarfluxState CurrentState = EStarfluxState::Pop;

    // 시작 위치
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Motion")
    FVector StartPos;

    // 주기
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Motion")
    float Amplitude{ 15.f };

    // 회전 속도
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Motion")
    float RotationSpeed{ 1.f };

    // 애니메이션 진행 시간
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Motion")
    float ElapsedTime{ 0.f };

    // 팝업 효과 활성화 여부
    bool bIsPopping;

    // 시작 위치 아래 지점
    FVector StartBelowLocation;

    // ======= 아이템 관련 변수 =======

    // 아이템 이름
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName{ TEXT("StarFlux") };

    // 아이템 수량
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemNum{ 1 };

    // ======= 타이머 및 근처 감지 관련 =======

    // 타이머 핸들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer", meta = (AllowPrivateAccess = "true"))
    FTimerHandle DistanceCheckTimerHandle;

    // 체크 간격
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
    float CheckInterval = 1.f;

    // 캐릭터 근처 여부
    bool IsCharacterNearby = false;

    // 흡수 가능 여부
    bool bCanAbsorb = true;

    // 경과 시간 추적
    float TimeSinceStart = 0.0f;

    // 캐릭터 참조
    class ACharacterBase* character;

    // 캐릭터와 아이템 거리
    float distanceWithCharacter{ 100.f };

    float MoveToCharacterSpeed{ 7.f };

    float AbsorptionDistance{ 500.f };

    // ======= 멤버 함수 =======

public:
    // 팝업 애니메이션
    UFUNCTION()
    void Pop(float DeltaTime);

    // 부유 효과
    UFUNCTION()
    void Floating(float DeltaTime);

    // 충돌 이벤트 처리
    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // 근처 캐릭터 체크
    void CheckForNearbyCharacters();

    // 캐릭터 흡수 처리
    void AbsorbToCharacter(float DeltaTime);
};
