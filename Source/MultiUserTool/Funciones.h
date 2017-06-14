// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#define I FVector(1,0,0)
#define J FVector(0,1,0)
#define K FVector(0,0,1)

int *imalloc(int n);
float SIN(float a);
float COS(float a);
float TAN(float a);
float ASIN(float s);
float ACOS(float s);
float ATAN(float s);
float ATAN2(float s, float c);
FVector versor(FVector A);
FVector versor(float x, float y, float z);
FVector versor(float Azm, float Elv);
float mod(FVector V);
FVector cross(FVector A, FVector B);
FVector ucross(FVector A, FVector B);
float dot(FVector A, FVector B);
float DXY(FVector &A, FVector &B);
FVector RandDir();
FVector RanDir(FVector U, float Amin, float Amax);
float frand(float xmin, float xmax);
float frand();
float sfrand();
FVector RandomVector(float sz);
FRotator RandomRotator(float sz);
FVector RandomNormalDir(FVector U);
int max(int a, int b);
int min(int a, int b);
char *ToChar(const TCHAR *txt);
TCHAR *ToTCHAR(char *txt);




/**
 * 
 */
class MULTIUSERTOOL_API Funciones
{
public:
	Funciones();
	~Funciones();
};
