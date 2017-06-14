// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "Funciones.h"
#include "Neuron.h"
#include "MyHUD.h"
#include "CodeGeneratedNeuron.h"

float NIVEL_MAXIMO = 4.0f;
float BIFURCACIONES = 10.0f;
float SINUSOIDAD = 0.5f;
float ANGULO = 40.0f;
float ESBELTED = 20.0f;
float SIMETRIA = 0.5f;
float DENDRITAS = 5.0f;
float RADIO_INICIAL = 0.3f;
float LOD = 0.0f;

class UMaterial;

UMaterial* TheMaterial1 = nullptr;
// Sets default values
ACodeGeneratedNeuron::ACodeGeneratedNeuron()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UCustomMeshComponent* mesh = CreateDefaultSubobject<UCustomMeshComponent>(TEXT("CustomMesh"));
	R = 50.0f;
	Rmax = 3000.0f;
	NumVertex = 0;
	NumTriangles = 0;
	GenerarNeurona();
	mesh->SetCustomMeshTriangles(triangles);
	if (!TheMaterial1) {
		static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/Material/NewMaterial2.NewMaterial2'"));
		if (Material.Object != NULL)
		{
			TheMaterial1 = (UMaterial*)Material.Object;
		}
	}
	mesh->SetMaterial(0, TheMaterial1);
	RootComponent = mesh;
	SetReplicates(true);
}

//CONSTRUCTOR DE LA MALLA
void ACodeGeneratedNeuron::Sample(const TArray<FVector>& points, TArray<FCustomMeshTriangle>& triangles, int segments) {
	UE_LOG(LogClass, Log, TEXT("ACodeGeneratedNeuron::Sample POINTS %d"), points.Num());

	TArray<FVector> verts;

	// precompute some trig
	float angle = FMath::DegreesToRadians(360.0f / segments);
	float sinA = FMath::Sin(angle);
	float cosA = FMath::Cos(angle);

	/*
	This implementation is rotation around the X Axis, other formulas below

	Z Axis Rotation
	x' = x*cos q - y*sin q
	y' = x*sin q + y*cos q
	z' = z

	X Axis Rotation
	y' = y*cos q - z*sin q
	z' = y*sin q + z*cos q
	x' = x

	Y Axis Rotation
	z' = z*cos q - x*sin q
	x' = z*sin q + x*cos q
	y' = y
	*/

	//Working point array, in which we keep the rotated line we draw with
	TArray<FVector> wp;
	for (int i = 0; i < points.Num(); i++) {
		wp.Add(points[i]);
	}

	// Add a first and last point on the axis to complete the triangles
	FVector p0(wp[0].X, 0, 0);
	FVector pLast(wp[wp.Num() - 1].X, 0, 0);

	FCustomMeshTriangle tri;
	//for each segment draw the triangles clockwise for normals pointing out or counterclockwise for the opposite (this here does CW)
	for (int segment = 0; segment<segments; segment++) {

		for (int i = 0; i<points.Num() - 1; i++) {
			FVector p1 = wp[i];
			FVector p2 = wp[i + 1];
			FVector p1r(p1.X, p1.Y*cosA - p1.Z*sinA, p1.Y*sinA + p1.Z*cosA);
			FVector p2r(p2.X, p2.Y*cosA - p2.Z*sinA, p2.Y*sinA + p2.Z*cosA);

			if (i == 0) {
				tri.Vertex0 = p1;
				tri.Vertex1 = p0;
				tri.Vertex2 = p1r;
				triangles.Add(tri);
			}

			tri.Vertex0 = p1;
			tri.Vertex1 = p1r;
			tri.Vertex2 = p2;
			triangles.Add(tri);

			tri.Vertex0 = p2;
			tri.Vertex1 = p1r;
			tri.Vertex2 = p2r;
			triangles.Add(tri);

			if (i == points.Num() - 2) {
				tri.Vertex0 = p2;
				tri.Vertex1 = p2r;
				tri.Vertex2 = pLast;
				triangles.Add(tri);
				wp[i + 1] = p2r;
			}

			wp[i] = p1r;
		}
	}
}

// Called when the game starts or when spawned
void ACodeGeneratedNeuron::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACodeGeneratedNeuron::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/************************************************************************
GENERADOR DE NEURONAS
*************************************************************************/

void ACodeGeneratedNeuron::Subdividir(TArray<FCustomMeshTriangle> &input_triangles, TArray<FCustomMeshTriangle> &out_triangles)
{
#define ADD_TRG(A,B,C) tri.Vertex0 = A; tri.Vertex1 = B; tri.Vertex2 = C; out_triangles.Add(tri);
	int n = input_triangles.Num();
	FVector a, b, c, p, q, r;
	FCustomMeshTriangle tri;
	//float R = 100.0f;
	for (int i = 0; i < n; i++) {
		a = input_triangles[i].Vertex0;
		b = input_triangles[i].Vertex1;
		c = input_triangles[i].Vertex2;

		p = R*versor((a + b) / 2);
		q = R*versor((b + c) / 2);
		r = R*versor((c + a) / 2);

			ADD_TRG(a, p, r)
			ADD_TRG(p, b, q)
			ADD_TRG(q, c, r)
			ADD_TRG(p, q, r)
	}
#undef ADD_TRG//(a,b,c)
}

FVector Centroide(TArray<FCustomMeshTriangle> &triangles)
{
	FVector C = FVector();
	int n = triangles.Num();
	for (int i = 0; i < n; i++) {
		C += triangles[i].Vertex0;
		C += triangles[i].Vertex1;
		C += triangles[i].Vertex2;
	}
	C /= (3 * n);
	return C;
}

void ACodeGeneratedNeuron::GenerarSoma(TArray<FCustomMeshTriangle> &triangles_out)
{
	TArray<FVector> points;
	TArray<FVector> T;
	TArray<FVector> B;
	TArray<FVector> N;
	TArray<FCustomMeshTriangle> triangles1;
	TArray<FCustomMeshTriangle> triangles2;
	//float R = 100.0f;
	FVector a = R*versor(1, 0, -1);
	FVector b = R*versor(-1, -1, -1);
	FVector c = R*versor(-1, 1, -1);
	FVector d = R*versor(0, 0, 1);
	FCustomMeshTriangle tri;
#define ADD_TRG(A,B,C) tri.Vertex0 = A; tri.Vertex1 = B; tri.Vertex2 = C; triangles1.Add(tri);
		ADD_TRG(a, c, b)
		ADD_TRG(a, b, d)
		ADD_TRG(b, c, d)
		ADD_TRG(c, a, d)
#undef ADD_TRG
		/*
		for (int i = 0; i < 4; i++) {
		Subdividir(triangles, triangles2); triangles.Empty();
		Subdividir(triangles2, triangles); triangles2.Empty();
		}
		*/
	Subdividir(triangles1, triangles2); triangles1.Empty();
	Subdividir(triangles2, triangles1); triangles2.Empty();
	Subdividir(triangles1, triangles2); triangles1.Empty();
	Subdividir(triangles2, triangles1); triangles2.Empty();
	Subdividir(triangles1, triangles); triangles1.Empty();
}


void Dendrita::Generar(FVector Xi, FVector Ui, TArray<FCustomMeshTriangle> &triangles, int nivel)
{
	//TArray<FVector> points;
	TArray<FVector> T;
	TArray<FVector> B;
	TArray<FVector> N;


	FVector Io = Ui;// RandDir();// I;// U;
	FVector Jo = RandomNormalDir(Io);// cross(K, Io);
	FVector Ko = cross(Io, Jo);
	int n = 100 - nivel * 20;
	//USAR ARCOS DE BEZIER
	FVector O, DX;
	//FVector U = FVector(1, 0, 0);
	FVector X = Xi;// FVector(0, 0, 0); //Xi;// FVector(0, 0, 0);
	float s = frand(5.0f, 10.0f);
	for (int i = 0; i < n; i++) {
		X += s*Io + Jo*(s*SIN(15.0f*(float)i));
		points.Add(X);

		T.Add(Io);
		B.Add(Jo);
		N.Add(Ko);

	}
	int n1 = n - 1;
	int n2 = n - 2;
	FVector t = Io;
	FVector k = Ko;
	FVector b = Jo;
	for (int i = 0; i < n1; i++) {
		t = versor(points[i + 1] - points[i]);
		b = versor(cross(t, k));
		k = cross(b, t);
		//T.Add(t);
		//B.Add(b);
		//N.Add(k);
		T[i] = t;
		N[i] = b;
		B[i] = k;
	}
	T[n1] = t;
	N[n1] = b;
	B[n1] = k;

	int m = 16 - nivel * 3;
	float R = 10.0f - nivel*2.0f;
	float da = 360.0f / m;
	FVector Seccion[100][32];
	FCustomMeshTriangle tri;
	float r, a;
	for (int i = 0; i < n; i++) {
		a = 0.0f;
		for (int j = 0; j < m; j++) {
			r = R*(1.0f + 0.3f*SIN(3 * i*360.0f / n));
			DX = r*(COS(a)*N[i] + SIN(a)*B[i]);//DESPLAZAMIENTO ENEL PLANO NORMAL
			O = points[i] + DX;
			Seccion[i][j] = O;
			a += da;
		}
		Seccion[i][m] = Seccion[i][0];
		if (i > 0) {

			for (int j = 0; j < m; j++) {
				tri.Vertex0 = Seccion[i - 1][j];
				tri.Vertex2 = Seccion[i - 1][j + 1];
				tri.Vertex1 = Seccion[i][j];
				triangles.Add(tri);
				tri.Vertex0 = Seccion[i - 1][j + 1];
				tri.Vertex2 = Seccion[i][j + 1];
				tri.Vertex1 = Seccion[i][j];
				triangles.Add(tri);
			}
		}
	}
	/*
	int nd = max(0, 3 - nivel);
	int j = 0;
	Dendrita *d;
	for (int i = 0; i < nd; i++) {
	j = (int)frand(n / 2, n);
	d=new Dendrita();
	d->Generar(points[j], RandomNormalDir(T[j]), triangles, nivel + 1);
	}
	*/
	//Mesh->SetCustomMeshTriangles(triangles);

}

Dendrita::Dendrita(FVector Xi, FVector Ui, int nivel)
{

}


void Dendrita::Update(void)//FVector Xi, FVector Ui, int nivel)
{
	//TArray<FVector> points;
	TArray<FCustomMeshTriangle> new_triangles;

	//USAR ARCOS DE BEZIER
	FVector O, DX;
	//FVector U = FVector(1, 0, 0);
	FVector X = Xo;// FVector(0, 0, 0); //Xi;// FVector(0, 0, 0);
	float s = frand(5.0f, 10.0f);
	//AQUI MOVEMOS EL EJE DE LA DENDRITA 
	for (int i = 0; i < n; i++) {
		X += s*Io + Jo*(s*SIN(15.0f*(float)i));
		points.Add(X);
	}
	//RECALCULAMOS LOS TRIEDROS
	int n1 = n - 1;
	FVector t = Io;
	FVector k = Ko;
	FVector b = Jo;
	for (int i = 0; i < n1; i++) {
		t = versor(points[i + 1] - points[i]);
		b = versor(cross(t, k));
		k = cross(b, t);
		T[i] = t;
		N[i] = b;
		B[i] = k;
	}
	T[n1] = t;
	N[n1] = b;
	B[n1] = k;
	float R = 10.0f - nivel*2.0f;
	float a;
	float da = 360.0f / m;
	FVector Seccion[100][32];
	float r;
	//RECALCULAMOS LOS VERTICES
	for (int i = 0; i < n; i++) {
		a = 0.0f;
		for (int j = 0; j < m; j++) {
			r = R*(1.0f + 0.3f*SIN(3 * i*360.0f / n));
			DX = r*(COS(a)*N[i] + SIN(a)*B[i]);//DESPLAZAMIENTO ENEL PLANO NORMAL
			O = points[i] + DX;
			Seccion[i][j] = O;
			a += da;
		}
		Seccion[i][m] = Seccion[i][0];

		if (i > 0) {
			FCustomMeshTriangle tri;
			for (int j = 0; j < m; j++) {
				tri.Vertex0 = Seccion[i - 1][j];
				tri.Vertex2 = Seccion[i - 1][j + 1];
				tri.Vertex1 = Seccion[i][j];
				new_triangles.Add(tri);
				tri.Vertex0 = Seccion[i - 1][j + 1];
				tri.Vertex2 = Seccion[i][j + 1];
				tri.Vertex1 = Seccion[i][j];
				new_triangles.Add(tri);
			}
		}
	}
	/*
	int nd = max(0, 3 - nivel);
	int j = 0;
	Dendrita *d;
	for (int i = 0; i < nd; i++) {
	j = (int)frand(n / 2, n);
	d=new Dendrita();
	d->Generar(points[j], RandomNormalDir(T[j]), triangles, nivel + 1);
	}
	*/
	//AQUI CAMBIARIAMOS LOS VERTICES (SINCRONIZAR, COMO?)
	//Mesh->SetCustomMeshTriangles(triangles);

}

void ACodeGeneratedNeuron::GenerarDendrita2(FVector Xi, FVector U, float L, int nivel)
{
	TArray<FVector> points;
	TArray<FVector> T;
	TArray<FVector> B;
	TArray<FVector> N;
	int n = 100;
	//USAR ARCOS DE BEZIER
	FVector O, DX;
	//CALCULO DE TANGENTES Y BINORMALES
	DX = U*L;// points[1] - points[0];
	FVector Io = U;
	FVector Jo = cross(K, Io);
	FVector Ko = cross(Io, Jo);

	L = 100.0f;
	float s = 10.0f;// L / (float)n;
	FVector X = Xi;
	for (int i = 0; i < n; i++) {
		X += s*U;// +RandDir(U)*frand(0, s / 2);
		points.Add(X);
		T.Add(Io);
		B.Add(Jo);
		N.Add(Ko);
	}


	T[0] = Io;// versor(DX);
	N[0] = Jo;// cross(K, T[0]);//A MEJORAR
	B[0] = Ko;// cross(T[0], N[0]);
	int n1 = n - 1;
	int n2 = n - 2;
	for (int i = 1; i < n1; i++) {
		T[i] = Io;// versor(points[i + 1] - points[i - 1]);//FVector(1, 0, 0);
		N[i] = Jo;// FVector(0, 0, 1);
		B[i] = Ko;// cross(T[i], N[i]);// FVector(0, 1, 0);
	}
	T[n1] = Io;// versor(points[n1] - points[n1 - 1]);
	N[n1] = Jo;// cross(K, T[n1]);//A MEJORAR
	B[n1] = Ko;// cross(T[n1], N[n1]);

	int m = 12;
	float Ri = 5.0f;// frand(0.5, 1.0f)*(L / 10);
	float a;
	float da = 360.0f / m;

	FCustomMeshTriangle tri;
	FVector Seccion[200][100];
	float r;
	for (int i = 0; i < n; i++) {
		a = 0.0f;
		for (int j = 0; j < m; j++) {
			r = Ri;// R*(1.0f + 0.3f*SIN(3 * i*360.0f / n));
			DX = r*(COS(a)*N[i] + SIN(a)*B[i]);
			O = points[i] + DX;
			Seccion[i][j] = O;// .Add(O);
			a += da;
		}
		Seccion[i][m] = Seccion[i][0];

		if (i > 0) {
			for (int j = 0; j < m; j++) {
				tri.Vertex0 = Seccion[i - 1][j];
				tri.Vertex1 = Seccion[i - 1][j + 1];
				tri.Vertex2 = Seccion[i][j];
				triangles.Add(tri);

				tri.Vertex0 = Seccion[i - 1][j + 1];
				tri.Vertex1 = Seccion[i][j + 1];
				tri.Vertex2 = Seccion[i][j];
				triangles.Add(tri);
			}
		}
	}
	//Mesh->SetCustomMeshTriangles(triangles);

}





bool GenerarTriedro(FVector U, FVector &Io, FVector &Jo, FVector &Ko)
{
	//SOLUCIONA POR EL EJE QUE DA MAYOR PRODUCTO VECTORIAL
	Io = versor(U);
	float x = mod(cross(Io, I));
	float y = mod(cross(Io, J));
	float z = mod(cross(Io, K));
	if (x >= y && x >= z) {//SOLUCIONA EN I
		Jo = ucross(Io, I);
		Ko = cross(Io, Jo);
		return true;
	}
	if (y >= x && y >= z) {//SOLUCIONA EN J
		Jo = ucross(Io, J);
		Ko = cross(Io, Jo);
		return true;
	}
	if (z >= x && z >= y) {//SOLUCIONA EN K
		Jo = ucross(Io, K);
		Ko = cross(Io, Jo);
		return true;
	}

	return false;
}



class LocalRef {
public:
	FTransform T;
	FRotator A;
	FVector O, Ex, Ey, Ez;// X,T,N,B


	LocalRef() { T = FTransform(); O = FVector(), Ex = I; Ey = J; Ez = K; }
	LocalRef(FVector X, FRotator Rot) {
		O = X; A = Rot;
		T = FTransform(A, O, FVector(1, 1, 1));
		Ex = T.GetUnitAxis(EAxis::X);
		Ey = T.GetUnitAxis(EAxis::Y);
		Ez = T.GetUnitAxis(EAxis::Z);
	}

	LocalRef(FVector X, FVector ex, FVector ey, FVector ez) {
		O = X; //A = Rot;
		T = FTransform(ex, ey, ez, O);// A, O, FVector(1, 1, 1));
		A  = T.Rotator();
		Ex = T.GetUnitAxis(EAxis::X);
		Ey = T.GetUnitAxis(EAxis::Y);
		Ez = T.GetUnitAxis(EAxis::Z);
	}

	LocalRef(FVector U, float Amin, float Amax) {
		O = FVector(0,0,0);
		Ex = RanDir(U, Amin, Amax);
		Ey = RandomNormalDir(Ex);
		Ez = cross(Ex, Ey);
		T = FTransform(Ex, Ey, Ez, O);// A, O, FVector(1, 1, 1));
		A = T.Rotator();
	}

	LocalRef(FVector U) {
		O = FVector(0, 0, 0);
		Ex = U;
		Ey = RandomNormalDir(Ex);
		Ez = cross(Ex, Ey);
		T = FTransform(Ex, Ey, Ez, O);// A, O, FVector(1, 1, 1));
		A = T.Rotator();
	}

	void MoveX(float dx, float ax, float ay, float az) {
		O += Ex*dx;
		A += FRotator(ax, ay, az);
		T = FTransform(A, O, FVector(1, 1, 1));
		Ex = T.GetUnitAxis(EAxis::X);
		Ey = T.GetUnitAxis(EAxis::Y);
		Ez = T.GetUnitAxis(EAxis::Z);
	}
	void MoveX(float dx,FRotator da) {
		O += Ex*dx;
		A += da;// FRotator(ax, ay, az);
		T = FTransform(A, O, FVector(1, 1, 1));
		Ex = T.GetUnitAxis(EAxis::X);
		Ey = T.GetUnitAxis(EAxis::Y);
		Ez = T.GetUnitAxis(EAxis::Z);
	}
	void MoveX(float dx, FVector U,float Amin,float Amax) {
		O += Ex*dx;
		Ex = RanDir(U, Amin, Amax);
		Ey = RandomNormalDir(Ex);
		Ez = cross(Ex, Ey);
		T = FTransform(Ex, Ey, Ez, O);// A, O, FVector(1, 1, 1));
		A = T.Rotator();
	}
	FVector Tpos(FVector Xr) { return T.TransformPosition(Xr); }
	FVector Tvec(FVector Vr) { return T.TransformVector(Vr); }
};

LocalRef RandomRef()
{
	float Ax = frand(-180, 180);
	float Ay = frand(-180, 180);
	float Az = frand(-180, 180);
	LocalRef Ref = LocalRef(FVector(0, 0, 0), FRotator(Ax, Ay, Az));
	return Ref;
}

LocalRef RandomRef(FVector O)
{
	float Ax = frand(-180, 180);
	float Ay = frand(-180, 180);
	float Az = frand(-180, 180);
	LocalRef Ref = LocalRef(O, FRotator(Ax, Ay, Az));
	return Ref;
}


LocalRef RandomRef(FVector X, FVector U, float Amin, float Amax)
{
	FVector Ex = RanDir(U, Amin, Amax);
	FVector Ey = RandomNormalDir(Ex);
	FVector Ez = cross(Ex, Ey);
	LocalRef Ref = LocalRef(X, Ex, Ey, Ez);
	return Ref;
}

LocalRef RandomRef(FVector X, FVector U)
{
	FVector Ex = U;// RanDir(U, Amin, Amax);
	FVector Ey = RandomNormalDir(Ex);
	FVector Ez = cross(Ex, Ey);
	LocalRef Ref = LocalRef(X, Ex, Ey, Ez);
	return Ref;
}



void ACodeGeneratedNeuron::GenerarTriangulos(int m, FVector *S, FVector *Sa)
{
	FCustomMeshTriangle tri;
	for (int j = 0; j < m; j++) {
		tri.Vertex0 = Sa[j];
		tri.Vertex1 = Sa[j + 1];
		tri.Vertex2 = S[j];
		triangles.Add(tri);
		tri.Vertex0 = Sa[j + 1];
		tri.Vertex1 = S[j + 1];
		tri.Vertex2 = S[j];
		triangles.Add(tri);
		NumTriangles += 2;
		NumVertex += 6;
	}

}

void ACodeGeneratedNeuron::GenerarSeccion(FVector *S, float r, int m, FVector O, FVector B, FVector N)
{
	float a = 0.0f;
	float da = 360.0f / m;
	//GENERACION DE SECCIONES TRANSVERSALES
	for (int j = 0; j < m; j++) {
		S[j] = O + r*(SIN(a)*B + COS(a)*N);
		a += da;
	}
}

struct VERTEX {
public:
	FVector O, T, B, N;
	float R;
	class RAMA *ramas;
	void Set(LocalRef Ref, float R) { O = Ref.O; T = Ref.Ex; B = Ref.Ey; N = Ref.Ez; R = R; }
};

struct SUBRAMA {
	LocalRef Ref;
	float R;
	float L;
	int nivel;
};

class RAMA {
public:
	float Ri, Rf, L;
	int n;
	int nivel;
	VERTEX *Vertex;
	RAMA *Ramas, *next;
	int nramas;
	SUBRAMA *SubRamas;
	RAMA(LocalRef Ref, float L, float Ri, int Nivel);
	//RAMA(FVector Xi, FRotator Ai, float L, float Ri);
	bool HayBifurcacion(float s);
	RAMA *AddRama(LocalRef Ref, float L, float Ri, int Nivel);
	int AddSubRama(LocalRef Ref, float L, float R, int nivel);
	int GenerarSubRamas(void);
	void Render(UCanvas *Canvas, FColor color);
};

bool RAMA::HayBifurcacion(float s)
{
	if (nivel == 4) return false;
	if (s < 0.5f) return false;
	float r = frand(0.5f, 1.0f);
	if (r > s) return true;
	return false;
}



int RAMA::GenerarSubRamas(void)
{
	SUBRAMA *S;
	for (int i = 0; i < nramas; i++) {
		S = &SubRamas[i];
		RAMA *Rama = AddRama(S->Ref, S->L, S->R, S->nivel);
		//if (Ramas) Rama->next = Ramas;Ramas = Rama;
	}
	return nramas;
}

RAMA *RAMA::AddRama(LocalRef Ref, float L, float Ri, int Nivel)
{
	Ref.MoveX(0.0f, frand(-25, 25), frand(-15, 15), frand(-15, 15));
	RAMA *Rama = new RAMA(Ref, L, Ri, Nivel + 1);
	if (Ramas) Rama->next = Ramas; Ramas = Rama;
	Rama->GenerarSubRamas();
	return Rama;
}


int RAMA::AddSubRama(LocalRef Ref, float L, float R, int nivel)
{
	SUBRAMA *S = &SubRamas[nramas];
	S->Ref = Ref;
	S->L = L;
	S->R = R;
	S->nivel = nivel;
	nramas++;
	return nramas;
}
//RAMA::RAMA(FVector Xi, FRotator Ai, float L, float Ri)
RAMA::RAMA(LocalRef Refi, float Longitud, float Radio, int Nivel)
{
	Ramas = nullptr;
	next = nullptr;
	nivel = Nivel;
	L = Longitud;
	Ri = Radio;
	Rf = 0.3f*Ri;
	nramas = 0;
	int max_ramas = (10 - Nivel);
	SubRamas = (SUBRAMA *)malloc(max_ramas * sizeof(SUBRAMA));
	memset(SubRamas, 0, max_ramas * sizeof(SUBRAMA));

	LocalRef Ref = Refi;
	FVector X = Ref.O;

	n = (int)(10.0f*L / Ri);
	if (n > 100) n = 100;
	Vertex = (VERTEX *)malloc(n * sizeof(VERTEX));
	float ds = L / (float)n;
	float t, s = 0.0f;
	float R = Ri;
	for (int i = 0; i < n; i++) {
		Vertex[i].Set(Ref, R);
		t = (float)i / (float)(n - 1);
		if (nivel == 0 && (i>n / 2) && ((i % 5) == 4) /*&& HayBifurcacion(t)*/ && nramas < max_ramas) {
			AddSubRama(Ref, L*frand(0.5f, 1.5f), R*frand(0.6f, 0.2f), nivel + 1);
			/*
			SUBRAMA *S = &SubRamas[nramas];
			S->Ref = Ref;
			S->L = L*frand(0.5f, 1.5f);
			S->R = R*frand(0.6f, 0.2f);
			S->nivel = nivel + 1;
			nramas++;
			*/
			/*
			RAMA *Rama= AddRama(Ref, L*frand(0.5f, 1.5f), R*frand(0.6f, 0.2f), nivel + 1);
			Vertex[i].ramas = Rama;
			if (Ramas) Rama->next = Ramas;
			Ramas = Rama;
			nramas++;
			*/
		}
		Ref.MoveX(ds, frand(-25, 25), frand(-15, 15), frand(-15, 15));
		s += ds; R = Ri + (Rf - Ri)*sqrt(s);
	}
	//CUANDO TERMINA LA RAMA PUEDE BIFURCAR
	if (0 && HayBifurcacion(s) && nramas < max_ramas) {
		RAMA *Rama = AddRama(Ref, L*frand(0.5f, 1.5f), R*frand(0.6f, 0.2f), nivel + 1);
		Vertex[n - 1].ramas = Rama;
		if (Ramas) Rama->next = Ramas;
		Ramas = Rama;
		nramas++;
	}
}

void RAMA::Render(UCanvas *Canvas, FColor color)
{
	FVector Xp[256];
	for (int i = 0; i < n; i++) {
		Xp[i] = Canvas->Project(Vertex[i].O);
	}
	for (int i = 0; i < n - 1; i++) {
		Canvas->K2_DrawLine(FVector2D(Xp[i].X, Xp[i].Y), FVector2D(Xp[i + 1].X, Xp[i + 1].Y), 2, color);
	}
	RAMA *r = Ramas;
	while (r) {
		r->Render(Canvas, color);
		r = r->next;
	}
}




void RENDER_NEURON_WIRES(UCanvas *Canvas, FColor color)
{
	static RAMA *Ramas = nullptr;
	if (!Ramas) {
		Ramas = new RAMA(RandomRef(), frand(200, 800), frand(20, 80), 0);
		Ramas->GenerarSubRamas();
		//for(int i=0;i<5;i++) Ramas->AddRama(RandomRef(), frand(200, 800), frand(20, 60), 0);
	}
	Ramas->Render(Canvas, color);
}


int ACodeGeneratedNeuron::GenerarRama(FVector Xi, FRotator Ai, float L, float Ri)
{
	VERTEX Vertex[256];
	LocalRef Ref = LocalRef(Xi, Ai);
	FVector X = Xi;
	float Rf = 0.3f*Ri;
	int n = (int)(L / Ri);
	float ds = L / (float)n;
	float s = 0.0f;
	for (int i = 0; i < n; i++) {
		Vertex[i].O = X;
		Vertex[i].T = Ref.Ex;
		Vertex[i].B = Ref.Ey;
		Vertex[i].N = Ref.Ez;
		Vertex[i].R = Ri + (Rf - Ri)*sqrt(s);//DEBERIAMOS GENERAR LA DISTRIBUCION DE RADIOS
											 //ESTE GENERADOS DE AVANCE DEBERIA SER MAS COMPLEJO
		Ref.MoveX(ds, frand(-25, 25), frand(-15, 15), frand(-15, 15));
		X = X + Vertex[i].T * ds;
		s += ds;
	}
	return n;
}

/*
extern float NIVEL_MAXIMO;// = 4.0f;
extern float BIFURCACIONES;// = 10.0f;
extern float SINUSOIDAD;// = 0.5f;
extern float ESBELTED;// = 20.0f;
extern float SIMETRIA;// = 0.5f;
extern float DENDRITAS;// = 5.0f;
extern float RADIO_INICIAL;// = 0.3f;
*/

void ACodeGeneratedNeuron::GenerarDendrita(FVector Xi, FVector Ui, TArray<FCustomMeshTriangle> &triangles, int nivel)
{
	FVector points[256];
	FVector T[256];
	FVector B[256];
	FVector N[256];
	LocalRef Refs[256];

	//CONSTRUIMOS UNA REFERENCIA
	float factor = 1.0f / (float)(nivel + 1);
	float Ax = frand(-180, 180);
	float Ay = frand(-180, 180);
	float Az = frand(-180, 180);
	LocalRef Ref = LocalRef(Xi, FRotator(Ax, Ay, Az));
	if (nivel > 0) {
		Ref = RandomRef(Xi,Ui, 10.0f, 40.0f);
	}
	//USAR ARCOS DE BEZIER
	float Lmax = factor*1000.0f;// -nivel * 200.0f;
	float Lmin = Lmax / 2.0f;
	float L = frand(Lmin, Lmax);
	int n = 100 - nivel * 20;
	float ds = L / n;// frand(5.0f, 10.0f);
	FVector DX;
	FVector X = Xi;
	for (int i = 0; i < n; i++) {
		points[i] = X;
		T[i] = Ref.Ex;
		B[i] = Ref.Ey;
		N[i] = Ref.Ez;
		Refs[i] = Ref;
		Ref.MoveX(ds, SINUSOIDAD*frand(-25, 25), SINUSOIDAD*frand(-15, 15), SINUSOIDAD*frand(-15, 15));
		X = X + T[i] * ds;
	}
	//GENERACION DE LA MALLA

	int m = (int)fmax(3.0f, factor*(32.0f - nivel * 3.0f));
	float Ri = RADIO_INICIAL*factor*frand(0.2f*R, 0.5f*R);
	float Rf = 0.3f*Ri;
	float da = 360.0f / m;
	FVector Seccion[100][33];
	float r, a;
	//FCustomMeshTriangle tri;
	float s = 0.0f;
	ds = 1.0f / n;
	for (int i = 0; i < n; i++) {
		a = 0.0f;
		r = Ri + (Rf - Ri)*sqrt(s);
		GenerarSeccion(Seccion[i], r, m, points[i], B[i], N[i]);
		s += ds;
		Seccion[i][m] = Seccion[i][0];//CERRAMOS LA SECCION
		if (i > 0) {
			GenerarTriangulos(m, Seccion[i], Seccion[i - 1]);
		}
	}
	//RAMIFICACIONES
	int nd = max(0, ((int)DENDRITAS) - nivel);
	int j = 0;
	FVector U;
	for (int i = 0; i < nd; i++) {
		j = (int)frand(n / 3, n);
		U = versor(T[j] + RandomNormalDir(T[j]));//SALEN A 45º
		//GenerarDendrita(points[j], U, triangles, nivel + 1);
		GenerarDendrita(Refs[j], triangles, nivel + 1);
	}
}


void ACodeGeneratedNeuron::GenerarDendrita(LocalRef Refi, TArray<FCustomMeshTriangle> &triangles2, int nivel)
{
	FVector points[256];
	FVector T[256];
	FVector B[256];
	FVector N[256];
	LocalRef Refs[256];

	//CONSTRUIMOS UNA REFERENCIA
	float factor = 1.0f / (float)(nivel + 1);
	float Ax = frand(-90, 90);
	float Ay = frand(-90, 90);
	float Az = frand(-90, 90);
	LocalRef Ref = Refi;// LocalRef(Refi.O, FRotator(Ax, Ay, Az));
	if (nivel > 0) {
		Ref = Refi;
		//Ref.MoveX(0.0f, ANGULO*frand(-45, 45), ANGULO*frand(-15, 15), ANGULO*frand(-15, 15));
		Ref.MoveX(0.0f, Ref.Ex, ANGULO / 2, ANGULO);
		if (dot(Refi.Ex, Ref.Ex) < COS(45)) return;
	}
	FVector Xi = Ref.O;
	//USAR ARCOS DE BEZIER
	float Lmax = factor*1000.0f;// -nivel * 200.0f;
	float Lmin = Lmax / 2.0f;
	float L = frand(Lmin, Lmax);
	int n = 100 - nivel * 20;
	float ds = L / n;// frand(5.0f, 10.0f);
	FVector DX;
	FVector X = Xi;
	for (int i = 0; i < n; i++) {
		points[i] = X;
		T[i] = Ref.Ex;
		B[i] = Ref.Ey;
		N[i] = Ref.Ez;
		Refs[i] = Ref;
		Ref.MoveX(ds, SINUSOIDAD*frand(-25, 25), SINUSOIDAD*frand(-15, 15), SINUSOIDAD*frand(-15, 15));
		X = X + T[i] * ds;
	}
	//GENERACION DE LA MALLA

	int m = (int)fmax(3.0f, factor*(32.0f - nivel * 3.0f));
	float Ri = RADIO_INICIAL*factor*frand(0.5f*R, 0.2f*R);
	float Rf = 0.3f*Ri;
	float da = 360.0f / m;
	FVector Seccion[100][33];
	float r, a;
	//FCustomMeshTriangle tri;
	float s = 0.0f;
	ds = 1.0f / n;
	for (int i = 0; i < n; i++) {
		a = 0.0f;
		r = Ri + (Rf - Ri)*sqrt(s);
		GenerarSeccion(Seccion[i], r, m, points[i], B[i], N[i]);

		s += ds;
		Seccion[i][m] = Seccion[i][0];//CERRAMOS LA SECCION
		if (i > 0) {
			GenerarTriangulos(m, Seccion[i], Seccion[i - 1]);
		}
	}
	//RAMIFICACIONES
	int nd = max(0, ((int)DENDRITAS) - nivel);
	int j = 0;
	FVector U;
	for (int i = 0; i < nd; i++) {
		j = (int)frand(0.3f*n,0.8f*n);
		U = versor(T[j] + RandomNormalDir(T[j]));//SALEN A 45º
		//GenerarDendrita(points[j], U, triangles, nivel + 1);
		GenerarDendrita(Refs[j], triangles, nivel + 1);
	}
}




/************************************************************************
EJEMPLO DE GENERACION DE UNA MALLA PROCEDURAL RANDOMIZADA
*************************************************************************/

void ACodeGeneratedNeuron::GenerarSomaNew()
{
	int nd = (int)DENDRITAS;
	FVector U,Xi[32];
	float R = 50.0f;
	float Rmin = R;
	float Rmax = 2 * R;
	LocalRef Ref[32];
	for (int i = 0; i < nd; i++) {
		U = RandDir();
		Xi[i] = frand(Rmin, Rmax)*U;
		Ref[i] = RandomRef(Xi[i], U);
	}
	//CREAMOS TRIANGULOS
	

}

float GRADOS(float a)
{
	return (180.0f*a / 3.141692f);
}

void GetAzmElv(FVector U, float *azm, float *elv)
{
	*elv = GRADOS(asin(U.Z));
	*azm = GRADOS(atan2(U.Y, U.X));
}

FVector Ajustar(FVector X)
{
	float R = mod(X);
	float Rmin = R / 2;
	FVector V = versor(X);
	float azm, elv;
	GetAzmElv(V, &azm, &elv);
	float da = (azm - 56.0f)/30.0f;
	float de = (elv - 45.0f)/30.0f;
	R = R / (0.2f + da*da + de*de);
	if (R < Rmin) R = Rmin;
	//if (fabs(elv) < 45.0f) R = 3.0f*R;
	return V*R;
}

int NUM_TRIANGLES = 0;

FVector Ajustar(FVector X, FVector *U, int n)
{
	FVector V, W,Ur;
	float ca,x,r,R = mod(X);
	R *= 0.4f;
	V = versor(X);
	float a,amin =90.0f;
	FVector T = V;
	for (int j = 0; j < n; j++) {
		ca = dot(U[j], V);
		if (ca > 0.0f) {
			a = GRADOS(acos(ca));// asin(mod(cross(U[j], V))));
			if (a < 45.0f) {
				x = ca;
				r = SIN(a);
				x = 1 / (r+0.001f);
				Ur = versor(V - U[j]*ca);
				if (x > 3.6) x = 3.6;
				return R*(U[j] * x + Ur*r);
			}
		}
	}
	return V*R;
}

void ACodeGeneratedNeuron::AjustarSoma(LocalRef *Refs,int n)
{
	FVector X,V,W;
	FVector U[16];
	for (int i = 0; i < n; i++) {
		U[i] = Refs[i].Ex;
	}
	
	int nt = triangles.Num();
	NUM_TRIANGLES = nt;
	FCustomMeshTriangle tri;
	for (int i = 0; i < nt; i++) {
		tri = triangles[i];
		tri.Vertex0 = Ajustar(tri.Vertex0,U, n);
		tri.Vertex1 = Ajustar(tri.Vertex1, U, n);
		tri.Vertex2 = Ajustar(tri.Vertex2, U, n);
		triangles[i] = tri;
	}
}

void ACodeGeneratedNeuron::GenerarNeurona(void)
{
	GenerarSoma(triangles);//GENERA UN NUCLEO POR SUBDIVISON DE UN TETRAEDRO
	Centro = Centroide(triangles);
	int nd = (int)DENDRITAS;// 4;
	FVector Xi = FVector(0, 0, 0);
	FVector U = K;// FVector(0, 0, 1);
				  //GenerarDendrita(Xi, U, triangles, 0);
	float Ax = frand(-180, 180);
	float Ay = frand(-180, 180);
	float Az = frand(-180, 180);
	LocalRef Ref = LocalRef(FVector(0,0,0), FRotator(Ax, Ay, Az));
	LocalRef Refs[16];
	for (int i = 0; i < nd; i++) {
		//U = RandDir();
		//GenerarDendrita(Xi, U, triangles, 0);
		Ax = frand(-180, 180);
		Ay = frand(-180, 180);
		Az = frand(-180, 180);
		Refs[i] = LocalRef(FVector(0, 0, 0), FRotator(Ax, Ay, Az));
	}
	nd = 0;
	Refs[nd++] = LocalRef(I);
	Refs[nd++] = LocalRef(J);
	Refs[nd++] = LocalRef(K);
	Refs[nd++] = LocalRef(-I);
	Refs[nd++] = LocalRef(-J);
	Refs[nd++] = LocalRef(-K);
	AjustarSoma(Refs, nd);
	for (int i = 0; i < nd; i++) {
		Refs[i].MoveX(1.5f*R, FRotator(0, 0, 0));
		GenerarDendrita(Refs[i], triangles, 0);
	}
}





/************************************************************************
EJEMPLO DE GENERACION DE UNA MALLA PROCEDURAL RANDOMIZADA
*************************************************************************/
/*
void ACodeGeneratedNeuron::GenerarNeurona(void)
{
	GenerarSoma(triangles);//GENERA UN NUCLEO POR SUBDIVISON DE UN TETRAEDRO
	Centro = Centroide(triangles);
	int nd = 4;
	FVector Xi = FVector(0, 0, 0);
	FVector U = K;// FVector(0, 0, 1);
				  //GenerarDendrita(Xi, U, triangles, 0);
	for (int i = 0; i < nd; i++) {
		U = RandDir();
		GenerarDendrita(Xi, U, triangles, 0);
	}
}
*/



/*
// Sets default values
ACodeGeneratedNeuron::ACodeGeneratedNeuron()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACodeGeneratedNeuron::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACodeGeneratedNeuron::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}
*/

