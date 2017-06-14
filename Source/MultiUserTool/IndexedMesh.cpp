// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "Funciones.h"
#include "IndexedMesh.h"


//UIndexedMesh::UIndexedMesh(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
UIndexedMesh::UIndexedMesh(){
	memset(this, 0, sizeof(UIndexedMesh));
	FVector O = FVector(0, 0, 0);
	float Sz = 200.0f;
	int o = AddVertex(O);
	int i = AddVertex(Sz*I);
	int j = AddVertex(Sz*J);
	int k = AddVertex(Sz*K);
	AddFace(o, i, k);
	AddFace(o, k, j);
	AddFace(o, j, i);
	AddFace(i, j, k);
}

//AÑADE UN VERTICE EN LA POSICION POS Y RETORNA SU INDICE
int UIndexedMesh::AddVertex(FVector Pos) {
	vertex[num_vertex++] = Pos;
	return num_vertex - 1;
}

//AÑADE UN LADO QUE UNE LOS VERTICES DE INDICES P y Q (pq <=> qp) Y RETORNA SU INDICE
int UIndexedMesh::AddEdge(int p, int q)
{
	for (int i = 0; i < num_edges; i++) {
		if (edges[i].p == q && edges[i].q == p) return i;
		if (edges[i].p == p && edges[i].q == q) return i;//REPETIDO
	}
	edges[num_edges++] = Edge(p, q);
	return num_edges-1;
}

//AÑADE UN TRIANGULO DE VERTICES INDEXADOS POR a,b,c (CREA LOS LADOS SI AUN NO EXISTEN)
//ADEMAS ENCOLA EN CADA VERTICE LOS TRIANGULOS ADYACENTES
int UIndexedMesh::AddFace(int a, int b, int c) {
	Face f; f.a = a; f.b = b; f.c = c;
	int idf = num_faces;
	f.A = AddEdge(a, b); f.B = AddEdge(b, c); f.C = AddEdge(c, a);
	faces[num_faces++] = f;
	LinkFaceToVertex(a, idf);
	LinkFaceToVertex(b, idf);
	LinkFaceToVertex(c, idf);
	return idf;
}


//IMPLEMENTA LA LISTA LINKADA DE TRIANGULOS ADYACENTES A UN VERTICE
void UIndexedMesh::LinkFaceToVertex(int vertex_index, int face_index)
{
	//USAMOS UNA LISTA LINKADA DE INDICES A FACES
	int last_link = vertex_faces[vertex_index];
	int pos = link_pos;
	vertex_faces[link_pos++] = face_index;
	vertex_faces[link_pos++] = last_link;
	vertex_faces[vertex_index] = pos;
}

//OBTIENE LA LISTA DE TRIANGULOS ADYACENTES A UN VERTICE
int UIndexedMesh::GetFaces(int vertex_index, int *faces)
{
	int j = 0;
	int link= vertex_faces[vertex_index];
	while (link > 0) {
		faces[j++] = vertex_faces[link];
		link= vertex_faces[link+1];
	}
	return j;
}

//EXPRESION PARA EL NIVEL 0
UIndexedMesh *UIndexedMesh::Subdividir()
{
	int a, b, c, p, q, r;
	UIndexedMesh *imesh = new UIndexedMesh();
	int *half = imalloc(num_edges);
	//COPIAMOS LOS VERTICES DE LA MALLA ORIGINAL
	for (int i = 0; i < num_vertex; i++) imesh->AddVertex(Pos(i));
	//CREAMOS UN VERTICE EN EL PUNTO MEDIO DE CADA LADO
	for (int i = 0; i < num_edges; i++) half[i] = imesh->AddVertex(HalfPos(i));
	//AHORA PODEMOS GENERAR LOS TRIANGULOS DEL NIVEL N+1 POR SUBDIVISION DE LOS DEL NIVEL ANTERIOR
	//EL NUMERO DE TRIANGULOS SE MULTIPLICA POR CUATRO
	for (int i = 0; i < num_faces; i++) {
		Face f = faces[i];
		a = f.a; b = f.b; c = f.c;
		p = half[f.A]; q = half[f.B]; r = half[f.C];
		imesh->AddFace(a, p, r);
		imesh->AddFace(p, b, q);
		imesh->AddFace(q, c, r);
		imesh->AddFace(p, q, r);
	}
	free(half);
	//LINKA LOS OBJETOS PARA PODER ACCEDER DESDE CUALQUIERA DE ELLOS AL RESTO DE NIVELES DE DETALLE
	NextLOD = imesh;
	imesh->PrevLOD = this;
	imesh->lod = lod + 1;//ESTA INVERTIDO
	return imesh;

}

FVector UIndexedMesh::Area(int face)
{
	FVector Xa = Pos(Faces[face].a);
	FVector Xb = Pos(Faces[face].b);
	FVector Xc = Pos(Faces[face].c);
	return cross(Xb - Xa, Xc - Xa);
}

void UIndexedMesh::UpdateNormals()
{
	int flist[256];
	for (int i = 0; i < num_vertex; i++) {
		FVector N = FVector(0, 0, 0);
		int n = GetFaces(i, flist);
		for (int j = 0; j < n; j++) {
			N += Area(flist[j]);//SOLO USAMOS LAS FACES DE SU NIVEL
		}
		normals[i]=versor(N);
	}
}



void UIndexedMesh::Render(UCanvas *canvas)
{
	//TArray<FVector2D> 
	FVector2D Xp[1024];
	FVector P;
	//num_vertex = 1;
	for (int i = 0; i < num_vertex; i++) {
		P = canvas->Project(vertex[i]);
		//Xp.Add(FVector2D(P.X,P.Y));
		Xp[i]=FVector2D(P.X, P.Y);
		//canvas->K2_DrawLine(FVector2D(0, 0), FVector2D(P.X, P.Y), 2.0f, FLinearColor::Red);
	}
	Edge e;
	for (int i = 0; i < num_edges; i++) {
		e = edges[i];
		canvas->K2_DrawLine(Xp[e.p], Xp[e.q], 1.0f, FLinearColor::Green);
	}
	//canvas->K2_DrawLine(FVector2D(0,0), FVector2D(1000, 1000), 2.0f, FLinearColor::Green);
}

void RenderIndexedMesh(UCanvas *canvas,int lod)
{
	static UIndexedMesh *mesh = nullptr;
	if (mesh == nullptr) {
		mesh = new UIndexedMesh();
	}
	if (lod > 0) {
		mesh = mesh->Subdividir();
	}

	mesh->Render(canvas);
}






