// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProceduralMeshComponent.h"
//#include "IndexedMesh.generated.h"

/**
 * 
 */

	
	
	
	

class VertexPNT {
public:
	FVector		Pos;
	FVector		Normal;
	FVector2D	Tcoord;
};

struct Edge {
//public:
	int p, q;//inf
	int r, l;
	Edge() { p = q = r = l = 0; }
	Edge(int a, int b) { p = a; q = b; r = l = 0; }
};

struct Face {
	int a, b, c;//indices de los vertices
	int A, B, C;//indices de los lados
};

struct LinkFace {
	int face;
	int next;
	LinkFace() { face = next = 0; }
	LinkFace(int f, int n) { face = f; next = n; }
};


/*
UCLASS()
class MULTIUSERTOOL_API UIndexedMesh : public UProceduralMeshComponent
{
	GENERATED_UCLASS_BODY()
	*/
class MULTIUSERTOOL_API UIndexedMesh 
{
public:
	FVector vertex[1024];
	FVector normals[1024];
	Edge	edges[1024];
	Face	faces[1024];
	int     vertex_faces[10240];

	TArray<FVector> Vertex;
	TArray<FVector> Normals;
	TArray<Edge> Edges;
	TArray<Face> Faces;
	TArray<TArray<int>> VertexFaces;
	TArray<TArray<int>> VertexEdges;
	int num_faces;
	int num_vertex;
	int num_edges;
	int link_pos;
	int lod;
	UIndexedMesh *NextLOD;
	UIndexedMesh *PrevLOD;
	//int nlod;
	UIndexedMesh();
	//UIndexedMesh(const FObjectInitializer& ObjectInitializer);
	int AddVertex(FVector Pos);// { TArray<int> a; VertexFaces.Add(a); TArray<int> e; VertexEdges.Add(e); return Vertex.Add(Pos); }
	int AddEdge(int p, int q);// { Edge e = Edge(p, q);  return Edges.Add(e); }
	int AddFace(int p, int q, int r);
	void LinkFaceToVertex(int vertex_index, int face_index);
	UIndexedMesh * Subdividir();
	FVector Pos(int i) { return vertex[i]; }
	FVector HalfPos(int e) { return (Pos(edges[e].p) + Pos(edges[e].q)) / 2.0f; }
	void UpdateNormals();
	int  GetFaces(int vertex_index, int *faces);
	FVector Area(int face);
	void Render(UCanvas *canvas);
};

