// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "Funciones.h"

#include "Funciones.h"

int *imalloc(int n) { return (int *)malloc(n * sizeof(int)); }

int max(int a, int b)
{
	if (a > b) return a;
	return b;
}

int min(int a, int b)
{
	if (a < b) return a;
	return b;
}


/*
float angle = FMath::DegreesToRadians(360.0f / segments);
float sinA = FMath::Sin(angle);
float cosA = FMath::Cos(angle);
*/

float SIN(float a)
{
	return FMath::Sin(FMath::DegreesToRadians(a));
}

float COS(float a)
{
	return FMath::Cos(FMath::DegreesToRadians(a));
}

float TAN(float a)
{
	return FMath::Tan(FMath::DegreesToRadians(a));
}

float ASIN(float s) { return FMath::RadiansToDegrees(asin(s)); }
float ACOS(float s) { return FMath::RadiansToDegrees(acos(s)); }
float ATAN(float s) { return FMath::RadiansToDegrees(atan(s)); }
float ATAN2(float s,float c) { return FMath::RadiansToDegrees(atan2(s,c)); }



FVector cross(FVector A, FVector B)
{
	FVector C = FVector(A.Y*B.Z - A.Z*B.Y, A.Z*B.X - A.X*B.Z, A.Y*B.Z - A.Z*B.Y);
	//C.CrossProduct(A, B);
	return C;
}

float dot(FVector A, FVector B)
{
	//FVector C;
	//C.DotProduct(A, B);
	return (A.X*B.X + A.Y*B.Y + A.Z*B.Z);
}

float mod(FVector V)
{
	return FMath::Sqrt(V.X*V.X + V.Y*V.Y + V.Z*V.Z);
}

FVector versor(FVector A)
{
	/*
	FVector V = A;
	V.Normalize(0.00001f);
	return V;
	*/
	float m = mod(A);
	if (m > 0.0f) {
		return FVector(A.X / m, A.Y / m, A.Z / m);
	}
	return FVector();
}

FVector versor(float Azm, float Elv)
{
	return FVector(COS(Elv)*COS(Azm), COS(Elv)*SIN(Azm), SIN(Elv));
}

/*
FVector versor(FVector V)
{
float mod = sqrtf(V.X*V.X + V.Y*V.Y + V.Z*V.Z);
return FVector(V.X / mod, V.Y / mod, V.Z / mod);
}
*/

FVector versor(float x, float y, float z)
{
	FVector V = FVector(x, y, z);
	V.Normalize(0.00001f);
	return V;
}


float frand()
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return r;
}

float sfrand()
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return 2.0f*r - 1.0f;
}

FVector RandomVector(float sz)
{
	return FVector(sz*sfrand(), sz*sfrand(), sz*sfrand());
}

FRotator RandomRotator(float sz)
{
	return FRotator(sz*sfrand(), sz*sfrand(), sz*sfrand());
}

float frand(float rmin, float rmax)
{
	float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return rmin + r*(rmax - rmin);
}


FVector RandDir()
{
	//float azm = frand(0, 360);
	//float elv = frand(-180, 180);
	return versor(RandomVector(1));// FVector(COS(elv)*SIN(azm), COS(elv)*COS(azm), SIN(elv));
}

FVector RandomNormalDir(FVector U)
{
	FVector V = RandDir();
	//A*Ux+B*Uy+C*Uz=0

	float u = dot(V, U);
	FVector T = V - U*u;//COMPONENTE 
	return versor(T);
}

FVector RanDir(FVector U, float Amin,float Amax)
{
	FVector T = RandomNormalDir(U);
	float a = frand(Amin, Amax);
	return versor(U*COS(a) + T*SIN(a));
}


FVector Bezier(FVector* p, float t)
{
	float   t2, t3;
	FVector a, b, c;
	FVector result;
	c = 3.0 * (p[1] - p[0]);
	b = 3.0 * (p[2] - p[1]) - c;
	a = p[3] - p[0] - c - b;
	t2 = t * t;
	t3 = t2 * t;
	result = (a * t3) + (b * t2) + (c * t) + p[0];
	return result;
}

float DXY(FVector &A, FVector &B)
{
	float dx = A.X - B.X;
	float dy = A.Y - B.Y;
	return sqrt(dx*dx + dy*dy);
}

FVector ucross(FVector A, FVector B)
{
	return versor(cross(A, B));
}



char *ToChar(const TCHAR *txt)
{
	size_t newsize = wcslen(txt) + 1;
	char * nstring = new char[newsize];
	size_t convertedChars = 0;
	wcstombs_s(&convertedChars, nstring, newsize, txt, _TRUNCATE);
	return nstring;
}

TCHAR *ToTCHAR(char *txt)
{
	size_t newsize = strlen(txt) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	// Convert char* string to a wchar_t* string.
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, txt, _TRUNCATE);
	return wcstring;
}

Funciones::Funciones()
{
}

Funciones::~Funciones()
{
}
