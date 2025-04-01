#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugMesh.generated.h"

USTRUCT(BlueprintType)
struct FMeshDatas
{
	GENERATED_BODY()

	FMeshDatas()
		: Verts(), Tris(), Normals(), UVs(), Colors(), Sects() {}

	UPROPERTY() TArray<FVector> Verts;
	UPROPERTY() TArray<int32> Tris;
	UPROPERTY() TArray<FVector> Normals;
	UPROPERTY() TArray<FVector2D> UVs;
	UPROPERTY() TArray<FLinearColor> Colors;
	UPROPERTY() TArray<int32> Sects;

	void Clear()
	{
		Verts.Empty();
		Tris.Empty();
		Normals.Empty();
		UVs.Empty();
		Colors.Empty();
		Sects.Empty();
	}
};

// 여기 유틸리티 클래스 정의 시작
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class EXOQUEST_API UProceduralMeshUtility : public UActorComponent
{
	GENERATED_BODY()

public:

	void GetMeshDataFromStaticMesh(UStaticMesh* StaticMesh, FMeshDatas& OutData, int32 StartSectionIndex)
	{
		// 유효성 검사
		if (!StaticMesh || !StaticMesh->GetRenderData() || !StaticMesh->GetRenderData()->LODResources.IsValidIndex(0))
		{
			return;
		}

		// 데이터 초기화
		OutData.Clear();

		// LOD 0 데이터 참조
		FStaticMeshLODResources& LOD = StaticMesh->GetRenderData()->LODResources[0];

		int32 CurrentSectionIndex = StartSectionIndex;
		int32 SectionID = 0;
		int32 NewVertexCount = 0;

		// 섹션이 유효한 동안 반복
		while (LOD.Sections.IsValidIndex(CurrentSectionIndex))
		{
			// 정점 중복 방지용 맵: [정점 인덱스 → 새로운 인덱스]
			TMap<int32, int32> VertexMap;

			// 현재 섹션 정보
			const FStaticMeshSection& Section = LOD.Sections[CurrentSectionIndex];
			uint32 StartIndex = Section.FirstIndex;
			uint32 EndIndex = StartIndex + Section.NumTriangles * 3;

			FIndexArrayView IndexArray = LOD.IndexBuffer.GetArrayView();
			uint32 TotalIndices = IndexArray.Num();

			// 정점 속성 유무 판단
			bool bHasNormals = LOD.VertexBuffers.StaticMeshVertexBuffer.GetNumVertices() >= LOD.VertexBuffers.PositionVertexBuffer.GetNumVertices();
			bool bHasUVs = bHasNormals;
			bool bHasColors = LOD.VertexBuffers.ColorVertexBuffer.GetNumVertices() >= LOD.VertexBuffers.PositionVertexBuffer.GetNumVertices();

			// 삼각형을 구성하는 모든 인덱스를 순회
			for (uint32 i = StartIndex; i < EndIndex && i < TotalIndices; ++i)
			{
				int32 OriginalVertexIndex = IndexArray[i];
				int32* MappedIndex = VertexMap.Find(OriginalVertexIndex);

				int32 NewIndex = 0;

				if (MappedIndex)
				{
					// 이미 등록된 정점이면 그걸 사용
					NewIndex = *MappedIndex;
				}
				else
				{
					// 새로운 정점이면 속성 추가
					OutData.Verts.Emplace(LOD.VertexBuffers.PositionVertexBuffer.VertexPosition(OriginalVertexIndex));

					if (bHasNormals)
						OutData.Normals.Emplace(LOD.VertexBuffers.StaticMeshVertexBuffer.VertexTangentZ(OriginalVertexIndex));

					if (bHasUVs)
						OutData.UVs.Emplace(LOD.VertexBuffers.StaticMeshVertexBuffer.GetVertexUV(OriginalVertexIndex, 0));

					if (bHasColors)
						OutData.Colors.Emplace(LOD.VertexBuffers.ColorVertexBuffer.VertexColor(OriginalVertexIndex));

					OutData.Sects.Emplace(SectionID);

					NewIndex = NewVertexCount;
					VertexMap.Emplace(OriginalVertexIndex, NewVertexCount);
					++NewVertexCount;
				}

				// 삼각형 인덱스에 등록
				OutData.Tris.Emplace(NewIndex);
			}

			// 다음 섹션으로 이동
			++CurrentSectionIndex;
			++SectionID;
		}
	}
};

UCLASS()
class EXOQUEST_API ADebugMesh : public AActor
{
	GENERATED_BODY()

public:
	ADebugMesh();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	TObjectPtr<class UStaticMeshComponent> Mesh;

	UPROPERTY()
	UProceduralMeshUtility* Utility;

	void LogMeshData();

public:
	virtual void Tick(float DeltaTime) override;
};
