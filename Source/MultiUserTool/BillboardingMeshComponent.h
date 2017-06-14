// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/MeshComponent.h"
#include "BillboardingMeshComponent.generated.h"

//struct FVector;
//USTRUCT(BlueprintType)
struct FMeshTriangle
{
	//GENERATED_USTRUCT_BODY()

		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Triangle)
		FVector Vertex0;

		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Triangle)
		FVector Vertex1;

		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Triangle)
		FVector Vertex2;

		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Triangle)
		FVector2D UV0;

		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Triangle)
		FVector2D UV1;

		//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Triangle)
		FVector2D UV2;
};
/**
 * 
 */

//UCLASS(hidecategories = (Object, LOD, Physics, Collision), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)

UCLASS()
class MULTIUSERTOOL_API UBillboardingMeshComponent : public UMeshComponent
{
	GENERATED_BODY()
public:
		//GENERATED_UCLASS_BODY()
		//UBillboardingMeshComponent(const FObjectInitializer& ObjectInitializer);
		UBillboardingMeshComponent();
		/** Set the geometry to use on this triangle mesh */
		//UFUNCTION(BlueprintCallable, Category = "Components|CustomMesh")
		bool SetMeshTriangles(const TArray<FMeshTriangle>& Triangles);

	/** Add to the geometry to use on this triangle mesh.  This may cause an allocation.  Use SetCustomMeshTriangles() instead when possible to reduce allocations. */
	//UFUNCTION(BlueprintCallable, Category = "Components|CustomMesh")
		void AddMeshTriangles(const TArray<FMeshTriangle>& Triangles);

	/** Removes all geometry from this triangle mesh.  Does not deallocate memory, allowing new geometry to reuse the existing allocation. */
	//UFUNCTION(BlueprintCallable, Category = "Components|CustomMesh")
		void ClearMeshTriangles();

private:

	// Begin UPrimitiveComponent interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	// End UPrimitiveComponent interface.

	// Begin UMeshComponent interface.
	virtual int32 GetNumMaterials() const override;
	// End UMeshComponent interface.

	// Begin USceneComponent interface.
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	// Begin USceneComponent interface.

	/** */
	TArray<FMeshTriangle> MeshTris;
	

	friend class FBillboardingMeshSceneProxy;
	
	
	
	
};
