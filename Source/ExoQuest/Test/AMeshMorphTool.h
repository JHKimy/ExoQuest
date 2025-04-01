#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "AMeshMorphTool.generated.h"

USTRUCT(BlueprintType)
struct FMeshData
{
	GENERATED_BODY()

	TArray<FVector> Verts;
	TArray<int32> Tris;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FLinearColor> Colors;
};

UCLASS()
class EXOQUEST_API AAMeshMorphTool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAMeshMorphTool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    /** 시작 메쉬 (StaticMesh A) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MorphMesh")
    UStaticMesh* MeshA;

    /** 끝 메쉬 (StaticMesh B) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MorphMesh")
    UStaticMesh* MeshB;

    /** 보간 값 (0 ~ 1 사이 슬라이더로 조절) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MorphMesh", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float InterpAlpha = 0.f;

    /** 초기화 함수 (BeginPlay에서 호출) */
    UFUNCTION(BlueprintCallable, Category = "MorphMesh")
    void InitMeshes();

    /** 보간 함수 (Tick에서 호출 또는 슬라이더 이벤트로 호출) */
    UFUNCTION(BlueprintCallable, Category = "MorphMesh")
    void InterpolateMeshes();

private:
    /** StaticMesh의 정점 데이터를 추출 */
    void ExtractMeshData(UStaticMesh* Mesh, FMeshData& OutData);

    /** 정점 수가 다를 경우 부족한 쪽을 채움 */
    void EqualizeVertices(FMeshData& A, const FMeshData& B);

private:
    FMeshData DataA;
    FMeshData DataB;
    FMeshData InterpData;

    UPROPERTY(VisibleAnywhere)
    UProceduralMeshComponent* ProcMesh;
};
