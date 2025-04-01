#include "Test/AMeshMorphTool.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Rendering/PositionVertexBuffer.h"
#include "Rendering/StaticMeshVertexBuffer.h"
#include "Rendering/ColorVertexBuffer.h"
#include "StaticMeshResources.h"

AAMeshMorphTool::AAMeshMorphTool()
{
	PrimaryActorTick.bCanEverTick = true;
	ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	ProcMesh->bUseAsyncCooking = false;
	RootComponent = ProcMesh;
}

void AAMeshMorphTool::BeginPlay()
{
	Super::BeginPlay();
	InitMeshes();
}

void AAMeshMorphTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InterpAlpha = FMath::Clamp(InterpAlpha + DeltaTime * 0.2f, 0.f, 1.f);
	InterpolateMeshes();
}

void AAMeshMorphTool::InitMeshes()
{
	ExtractMeshData(MeshA, DataA);
	ExtractMeshData(MeshB, DataB);

	if (DataA.Verts.Num() < DataB.Verts.Num())
	{
		EqualizeVertices(DataA, DataB);
	}
	else if (DataB.Verts.Num() < DataA.Verts.Num())
	{
		EqualizeVertices(DataB, DataA);
	}

	InterpData = DataA;
	ProcMesh->CreateMeshSection_LinearColor(0, InterpData.Verts, InterpData.Tris, InterpData.Normals, InterpData.UVs, InterpData.Colors, TArray<FProcMeshTangent>(), true);
}

void AAMeshMorphTool::InterpolateMeshes()
{
	if (DataA.Verts.Num() != DataB.Verts.Num()) return;
	InterpData.Verts.SetNum(DataA.Verts.Num());

	for (int32 i = 0; i < DataA.Verts.Num(); ++i)
	{
		InterpData.Verts[i] = FMath::Lerp(DataA.Verts[i], DataB.Verts[i], InterpAlpha);
	}

	ProcMesh->UpdateMeshSection_LinearColor(0, InterpData.Verts, InterpData.Normals, InterpData.UVs, InterpData.Colors, TArray<FProcMeshTangent>());
}

void AAMeshMorphTool::ExtractMeshData(UStaticMesh* Mesh, FMeshData& OutData)
{
	if (!Mesh) return;
	FStaticMeshLODResources& LOD = Mesh->GetRenderData()->LODResources[0];
	const FPositionVertexBuffer& PosBuffer = LOD.VertexBuffers.PositionVertexBuffer;
	const FStaticMeshVertexBuffer& VertBuffer = LOD.VertexBuffers.StaticMeshVertexBuffer;
	const FColorVertexBuffer& ColorBuffer = LOD.VertexBuffers.ColorVertexBuffer;

	int32 VertexCount = PosBuffer.GetNumVertices();
	OutData.Verts.SetNum(VertexCount);
	OutData.Normals.SetNum(VertexCount);
	OutData.UVs.SetNum(VertexCount);
	OutData.Colors.SetNum(VertexCount);

	for (int32 i = 0; i < VertexCount; ++i)
	{
		OutData.Verts[i] = FVector(PosBuffer.VertexPosition(i));
		OutData.Normals[i] = FVector(VertBuffer.VertexTangentZ(i));
		OutData.UVs[i] = FVector2D(VertBuffer.GetVertexUV(i, 0));
		OutData.Colors[i] = ColorBuffer.GetNumVertices() > 0 ? FLinearColor(ColorBuffer.VertexColor(i)) : FLinearColor::Black;
	}

	int32 IndexCount = LOD.IndexBuffer.GetNumIndices();
	OutData.Tris.SetNum(IndexCount);
	for (int32 i = 0; i < IndexCount; ++i)
	{
		OutData.Tris[i] = LOD.IndexBuffer.GetIndex(i);
	}
}

void AAMeshMorphTool::EqualizeVertices(FMeshData& A, const FMeshData& B)
{
	int32 Diff = B.Verts.Num() - A.Verts.Num();
	if (Diff <= 0) return;

	int32 NumTrisToAdd = Diff / 3;
	for (int32 i = 0; i < NumTrisToAdd; ++i)
	{
		int32 TriStartIndex = UKismetMathLibrary::RandomIntegerInRange(0, A.Tris.Num() / 3 - 1) * 3;
		int32 Index0 = A.Tris[TriStartIndex];
		int32 Index1 = A.Tris[TriStartIndex + 1];
		int32 Index2 = A.Tris[TriStartIndex + 2];

		FVector V0 = A.Verts[Index0];
		FVector V1 = A.Verts[Index1];
		FVector V2 = A.Verts[Index2];

		FVector N0 = A.Normals[Index0];
		FVector N1 = A.Normals[Index1];
		FVector N2 = A.Normals[Index2];

		FVector2D UV0 = A.UVs[Index0];
		FVector2D UV1 = A.UVs[Index1];
		FVector2D UV2 = A.UVs[Index2];

		FLinearColor C0 = A.Colors[Index0];
		FLinearColor C1 = A.Colors[Index1];
		FLinearColor C2 = A.Colors[Index2];

		int32 NewIndexStart = A.Verts.Num();
		A.Tris.Add(NewIndexStart);
		A.Tris.Add(NewIndexStart + 1);
		A.Tris.Add(NewIndexStart + 2);

		A.Verts.Add(V0);
		A.Verts.Add(V1);
		A.Verts.Add(V2);

		A.Normals.Add(N0);
		A.Normals.Add(N1);
		A.Normals.Add(N2);

		A.UVs.Add(UV0);
		A.UVs.Add(UV1);
		A.UVs.Add(UV2);

		A.Colors.Add(C0);
		A.Colors.Add(C1);
		A.Colors.Add(C2);
	}
}