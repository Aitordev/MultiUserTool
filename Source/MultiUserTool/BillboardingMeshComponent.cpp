// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
//#include "CustomMeshComponentPluginPrivatePCH.h"
#include "CustomMeshComponent.h"
#include "DynamicMeshBuilder.h"
#include "MaterialShared.h"
#include "EngineGlobals.h"
#include "RenderUtils.h"
#include "SceneManagement.h"
#include "LocalVertexFactory.h"
#include "Engine/Engine.h"
#include "BillboardingMeshComponent.h"

/*


struct VertexIn
{
	float3 PosW  : POSITION;
	float2 SizeW : SIZE;
};

[StructLayout(LayoutKind.Sequential)]
public struct TreePointSprite {
public Vector3 Pos;
public Vector2 Size;

public static readonly int Stride = Marshal.SizeOf(typeof(TreePointSprite));
}
public static class InputLayoutDescriptions {
// other inputLayouts omitted...
public static readonly InputElement[] TreePointSprite = {
new InputElement("POSITION", 0, Format.R32G32B32_Float, 0, 0, InputClassification.PerVertexData, 0 ),
new InputElement("SIZE", 0, Format.R32G32_Float, 12,0,InputClassification.PerVertexData, 0)
};
}

cbuffer cbPerFrame
{
DirectionalLight gDirLights[3];
float3 gEyePosW;

float  gFogStart;
float  gFogRange;
float4 gFogColor;
};

cbuffer cbPerObject
{
float4x4 gViewProj;
Material gMaterial;
};

cbuffer cbFixed
{
//
// Compute texture coordinates to stretch texture over quad.
//

float2 gTexC[4] =
{
float2(0.0f, 1.0f),
float2(0.0f, 0.0f),
float2(1.0f, 1.0f),
float2(1.0f, 0.0f)
};
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2DArray gTreeMapArray;

SamplerState samLinear
{
Filter   = MIN_MAG_MIP_LINEAR;
AddressU = CLAMP;
AddressV = CLAMP;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	// Just pass data over to geometry shader.
	vout.CenterW = vin.PosW;
	vout.SizeW   = vin.SizeW;

	return vout;
}

// We expand each point into a quad (4 vertices), so the maximum number of vertices
// we output per geometry shader invocation is 4.
[maxvertexcount(4)]
void GS(point VertexOut gin[1],
uint primID : SV_PrimitiveID,
inout TriangleStream<GeoOut> triStream)
{
//
// Compute the local coordinate system of the sprite relative to the world
// space such that the billboard is aligned with the y-axis and faces the eye.
//

float3 up = float3(0.0f, 1.0f, 0.0f);
float3 look = gEyePosW - gin[0].CenterW;
look.y = 0.0f; // y-axis aligned, so project to xz-plane
look = normalize(look);
float3 right = cross(up, look);

//
// Compute triangle strip vertices (quad) in world space.
//
float halfWidth  = 0.5f*gin[0].SizeW.x;
float halfHeight = 0.5f*gin[0].SizeW.y;

float4 v[4];
v[0] = float4(gin[0].CenterW + halfWidth*right - halfHeight*up, 1.0f);
v[1] = float4(gin[0].CenterW + halfWidth*right + halfHeight*up, 1.0f);
v[2] = float4(gin[0].CenterW - halfWidth*right - halfHeight*up, 1.0f);
v[3] = float4(gin[0].CenterW - halfWidth*right + halfHeight*up, 1.0f);

//
// Transform quad vertices to world space and output
// them as a triangle strip.
//
GeoOut gout;
[unroll]
for(int i = 0; i < 4; ++i)
{
gout.PosH     = mul(v[i], gViewProj);
gout.PosW     = v[i].xyz;
gout.NormalW  = look;
gout.Tex      = gTexC[i];
gout.PrimID   = primID;

triStream.Append(gout);
}
}

float4 PS(GeoOut pin, uniform int gLightCount, uniform bool gUseTexure, uniform bool gAlphaClip, uniform bool gFogEnabled) : SV_Target
{
	// snip...
	if(gUseTexure)
	{
	// Sample texture.
	float3 uvw = float3(pin.Tex, pin.PrimID%4);
	texColor = gTreeMapArray.Sample( samLinear, uvw );

	if(gAlphaClip)
	{
		// Discard pixel if texture alpha < 0.05.  Note that we do this
		// test as soon as possible so that we can potentially exit the shader
		// early, thereby skipping the rest of the shader code.
		clip(texColor.a - 0.05f);
	}
}
// snip...
}
*/



class FPrimitiveDrawInterface;
class FMaterialRenderProxy;


struct FMeshVertex
{
	FMeshVertex() {}
	FMeshVertex(const FVector& InPosition,const FVector2D& InTcoord) :
		Position(InPosition),
		TextureCoordinate(InTcoord),
		TangentX(FVector(1, 0, 0)),
		TangentZ(FVector(0, 0, 1)),
		Color(FColor(255, 255, 255))
	{
		// basis determinant default to +1.0
		TangentZ.Vector.W = 255;
	}

	FMeshVertex(const FVector& InPosition, const FVector& InTangentX, const FVector& InTangentZ, const FVector2D& InTexCoord, const FColor& InColor) :
		Position(InPosition),
		TextureCoordinate(InTexCoord),
		TangentX(InTangentX),
		TangentZ(InTangentZ),
		Color(InColor)
	{
		// basis determinant default to +1.0
		TangentZ.Vector.W = 255;
	}

	void SetTangents(const FVector& InTangentX, const FVector& InTangentY, const FVector& InTangentZ)
	{
		TangentX = InTangentX;
		TangentZ = InTangentZ;
		// store determinant of basis in w component of normal vector
		TangentZ.Vector.W = GetBasisDeterminantSign(InTangentX, InTangentY, InTangentZ) < 0.0f ? 0 : 255;
	}

	FVector GetTangentY()
	{
		return (FVector(TangentZ) ^ FVector(TangentX)) * ((float)TangentZ.Vector.W / 127.5f - 1.0f);
	};

	FVector Position;
	FVector2D TextureCoordinate;
	FPackedNormal TangentX;
	FPackedNormal TangentZ;
	FColor Color;
};

/**
* A utility used to construct dynamically generated meshes, and render them to a FPrimitiveDrawInterface.
* Note: This is meant to be easy to use, not fast.  It moves the data around more than necessary, and requires dynamically allocating RHI
* resources.  Exercise caution.
*/
class FMeshBuilder
{
public:

	/** Initialization constructor. */
	ENGINE_API FMeshBuilder();

	/** Destructor. */
	ENGINE_API ~FMeshBuilder();

	/** Adds a vertex to the mesh. */
	ENGINE_API int32 AddVertex(
		const FVector& InPosition,
		const FVector2D& InTextureCoordinate,
		const FVector& InTangentX,
		const FVector& InTangentY,
		const FVector& InTangentZ,
		const FColor& InColor
	);

	/** Adds a vertex to the mesh. */
	ENGINE_API int32 AddVertex(const FMeshVertex &InVertex);

	/** Adds a triangle to the mesh. */
	ENGINE_API void AddTriangle(int32 V0, int32 V1, int32 V2);

	/** Adds many vertices to the mesh. */
	ENGINE_API int32 AddVertices(const TArray<FMeshVertex> &InVertices);

	/** Add many indices to the mesh. */
	ENGINE_API void AddTriangles(const TArray<int32> &InIndices);

	/** Adds a mesh of what's been built so far to the collector. */
	ENGINE_API void GetMesh(const FMatrix& LocalToWorld, const FMaterialRenderProxy* MaterialRenderProxy, uint8 DepthPriorityGroup, bool bDisableBackfaceCulling, bool bReceivesDecals, int32 ViewIndex, FMeshElementCollector& Collector);
	ENGINE_API void GetMesh(const FMatrix& LocalToWorld, const FMaterialRenderProxy* MaterialRenderProxy, uint8 DepthPriorityGroup, bool bDisableBackfaceCulling, bool bReceivesDecals, bool bUseSelectionOutline, int32 ViewIndex,
		FMeshElementCollector& Collector, HHitProxy* HitProxy);

	/**
	* Draws the mesh to the given primitive draw interface.
	* @param PDI - The primitive draw interface to draw the mesh on.
	* @param LocalToWorld - The local to world transform to apply to the vertices of the mesh.
	* @param FMaterialRenderProxy - The material instance to render on the mesh.
	* @param DepthPriorityGroup - The depth priority group to render the mesh in.
	*/
	ENGINE_API void Draw(FPrimitiveDrawInterface* PDI, const FMatrix& LocalToWorld, const FMaterialRenderProxy* MaterialRenderProxy, uint8 DepthPriorityGroup, bool bDisableBackfaceCulling = false, bool bReceivesDecals = true);

private:
	class FMeshIndexBuffer* IndexBuffer;
	class FMeshVertexBuffer* VertexBuffer;
};

/*
virtual void * RHILockVertexBuffer
(
	FVertexBufferRHIParamRef VertexBuffer,
	uint32 Offset,
	uint32 SizeRHI,
	EResourceLockMode LockMode
)
*/



/** Vertex Buffer */
class FMeshVertexBuffer : public FVertexBuffer
{
public:
	TArray<FMeshVertex> Vertices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		void* VertexBufferData = nullptr;
		VertexBufferRHI = RHICreateAndLockVertexBuffer(Vertices.Num() * sizeof(FMeshVertex), BUF_Dynamic, CreateInfo, VertexBufferData);

		// Copy the vertex data into the vertex buffer.		
		FMemory::Memcpy(VertexBufferData, Vertices.GetData(), Vertices.Num() * sizeof(FMeshVertex));
		RHIUnlockVertexBuffer(VertexBufferRHI);
	}

	void Update(FMeshVertex *vertex_data,int nv)
	{
		FMeshVertex *V = (FMeshVertex *)RHILockVertexBuffer(VertexBufferRHI, 0, nv * sizeof(FMeshVertex), EResourceLockMode::RLM_WriteOnly);
			//COPIAMOS
		memcpy(V, vertex_data, nv * sizeof(FMeshVertex));

		RHIUnlockVertexBuffer(VertexBufferRHI);
	}

};

/** Index Buffer */
class FMeshIndexBuffer : public FIndexBuffer
{
public:
	TArray<int32> Indices;

	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		void* Buffer = nullptr;
		IndexBufferRHI = RHICreateAndLockIndexBuffer(sizeof(int32), Indices.Num() * sizeof(int32), BUF_Dynamic, CreateInfo, Buffer);

		// Write the indices to the index buffer.		
		FMemory::Memcpy(Buffer, Indices.GetData(), Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(IndexBufferRHI);
	}
};

/** Vertex Factory */

class FMeshVertexFactory : public FLocalVertexFactory
{
public:

	FMeshVertexFactory()
	{}

	/** Init function that should only be called on render thread. */
	void Init_RenderThread(const FMeshVertexBuffer* VertexBuffer)
	{
		check(IsInRenderingThread());

		DataType NewData;
		NewData.PositionComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Position, VET_Float3);
		NewData.TextureCoordinates.Add(
			FVertexStreamComponent(VertexBuffer, STRUCT_OFFSET(FDynamicMeshVertex, TextureCoordinate), sizeof(FDynamicMeshVertex), VET_Float2)
		);
		NewData.TangentBasisComponents[0] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentX, VET_PackedNormal);
		NewData.TangentBasisComponents[1] = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, TangentZ, VET_PackedNormal);
		NewData.ColorComponent = STRUCTMEMBER_VERTEXSTREAMCOMPONENT(VertexBuffer, FDynamicMeshVertex, Color, VET_Color);

		SetData(NewData);
	}

	/** Initialization */
	void Init(const FMeshVertexBuffer* VertexBuffer)
	{
		if (IsInRenderingThread())
		{
			Init_RenderThread(VertexBuffer);
		}
		else
		{
			ENQUEUE_UNIQUE_RENDER_COMMAND_TWOPARAMETER(
				InitMeshVertexFactory,
				FMeshVertexFactory*, VertexFactory, this,
				const FMeshVertexBuffer*, VertexBuffer, VertexBuffer,
				{
					VertexFactory->Init_RenderThread(VertexBuffer);
				});
		}
	}
};

/** Scene proxy */
class FBillboardingMeshSceneProxy : public FPrimitiveSceneProxy
{
public:

	FBillboardingMeshSceneProxy(UBillboardingMeshComponent* Component)
		: FPrimitiveSceneProxy(Component)
		, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
	{
		const FColor VertexColor(255, 255, 255);

		// Add each triangle to the vertex/index buffer
		for (int TriIdx = 0; TriIdx<Component->MeshTris.Num(); TriIdx++)
		{
			FMeshTriangle& Tri = Component->MeshTris[TriIdx];

			const FVector Edge01 = (Tri.Vertex1 - Tri.Vertex0);
			const FVector Edge02 = (Tri.Vertex2 - Tri.Vertex0);

			const FVector TangentX = Edge01.GetSafeNormal();
			const FVector TangentZ = (Edge02 ^ Edge01).GetSafeNormal();
			const FVector TangentY = (TangentX ^ TangentZ).GetSafeNormal();

			FMeshVertex Vert0;
			Vert0.Position = Tri.Vertex0;
			Vert0.Color = VertexColor;
			Vert0.SetTangents(TangentX, TangentY, TangentZ);
			int32 VIndex = VertexBuffer.Vertices.Add(Vert0);
			IndexBuffer.Indices.Add(VIndex);

			FMeshVertex Vert1;
			Vert1.Position = Tri.Vertex1;
			Vert1.Color = VertexColor;
			Vert1.SetTangents(TangentX, TangentY, TangentZ);
			VIndex = VertexBuffer.Vertices.Add(Vert1);
			IndexBuffer.Indices.Add(VIndex);

			FMeshVertex Vert2;
			Vert2.Position = Tri.Vertex2;
			Vert2.Color = VertexColor;
			Vert2.SetTangents(TangentX, TangentY, TangentZ);
			VIndex = VertexBuffer.Vertices.Add(Vert2);
			IndexBuffer.Indices.Add(VIndex);
		}

		// Init vertex factory
		VertexFactory.Init(&VertexBuffer);

		// Enqueue initialization of render resource
		BeginInitResource(&VertexBuffer);
		BeginInitResource(&IndexBuffer);
		BeginInitResource(&VertexFactory);

		// Grab material
		Material = Component->GetMaterial(0);
		if (Material == NULL)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
	}

	virtual ~FBillboardingMeshSceneProxy()
	{
		VertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_BillboardingMeshSceneProxy_GetDynamicMeshElements);

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy(IsSelected()) : NULL,
			FLinearColor(0, 0.5f, 1.f)
		);

		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

		FMaterialRenderProxy* MaterialProxy = NULL;
		if (bWireframe)
		{
			MaterialProxy = WireframeMaterialInstance;
		}
		else
		{
			MaterialProxy = Material->GetRenderProxy(IsSelected());
		}

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];
				// Draw the mesh.
				FMeshBatch& Mesh = Collector.AllocateMesh();
				FMeshBatchElement& BatchElement = Mesh.Elements[0];
				BatchElement.IndexBuffer = &IndexBuffer;
				Mesh.bWireframe = bWireframe;
				Mesh.VertexFactory = &VertexFactory;
				Mesh.MaterialRenderProxy = MaterialProxy;
				BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());
				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = IndexBuffer.Indices.Num() / 3;
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = VertexBuffer.Vertices.Num() - 1;
				Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
				Mesh.Type = PT_TriangleList;
				Mesh.DepthPriorityGroup = SDPG_World;
				Mesh.bCanApplyViewModeOverrides = false;
				Collector.AddMesh(ViewIndex, Mesh);
			}
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) 
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;
		Result.bRenderInMainPass = ShouldRenderInMainPass();
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		return Result;
	}

	virtual bool CanBeOccluded() const override
	{
		return !MaterialRelevance.bDisableDepthTest;
	}

	virtual uint32 GetMemoryFootprint(void) const override { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

private:

	UMaterialInterface* Material;
	FMeshVertexBuffer VertexBuffer;
	FMeshIndexBuffer IndexBuffer;
	FMeshVertexFactory VertexFactory;

	FMaterialRelevance MaterialRelevance;
};

//////////////////////////////////////////////////////////////////////////

/*
UBillboardingMeshComponent::UBillboardingMeshComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
}
*/

UBillboardingMeshComponent::UBillboardingMeshComponent():Super()
	//: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	//SetCollisionProfileName(UCollisionProfile::BlockAllDynamic_ProfileName);
}

bool UBillboardingMeshComponent::SetMeshTriangles(const TArray<FMeshTriangle>& Triangles)
{
	MeshTris = Triangles;

	// Need to recreate scene proxy to send it over
	MarkRenderStateDirty();

	return true;
}

void UBillboardingMeshComponent::AddMeshTriangles(const TArray<FMeshTriangle>& Triangles)
{
	MeshTris.Append(Triangles);

	// Need to recreate scene proxy to send it over
	MarkRenderStateDirty();
}

void  UBillboardingMeshComponent::ClearMeshTriangles()
{
	MeshTris.Reset();

	// Need to recreate scene proxy to send it over
	MarkRenderStateDirty();
}


FPrimitiveSceneProxy* UBillboardingMeshComponent::CreateSceneProxy()
{
	FPrimitiveSceneProxy* Proxy = NULL;
	if (MeshTris.Num() > 0)
	{
		Proxy = new FBillboardingMeshSceneProxy(this);
	}
	return Proxy;
}

int32 UBillboardingMeshComponent::GetNumMaterials() const
{
	return 1;
}


FBoxSphereBounds UBillboardingMeshComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds NewBounds;
	NewBounds.Origin = FVector::ZeroVector;
	NewBounds.BoxExtent = FVector(HALF_WORLD_MAX, HALF_WORLD_MAX, HALF_WORLD_MAX);
	NewBounds.SphereRadius = FMath::Sqrt(3.0f * FMath::Square(HALF_WORLD_MAX));
	return NewBounds;
}






