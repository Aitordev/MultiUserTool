// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
struct FColor;
struct FVector;
struct FRotator;

class FString;
class UCanvas;
class AMyHUD;
class ANeuron;
class ACodeGeneratedNeuron;

//CONTROLES
class HUD_CONTROL {
public:
	int id;
	int tipo;
	int xmin, ymin, xmax, ymax, w, h,dxt,dyt;
	char texto[256];
	FString text;
	HUD_CONTROL *next;
	//HUD_CONTROL();
	bool IsMouseInside(void);
	void Render(UCanvas *Canvas);
	void ReSize(float neww, float newh);
};

class HUD_INTERFACE {
public:
	FString Name;
	HUD_CONTROL *Controls;
	int xmin, ymin, xmax, ymax, w, h;
	HUD_INTERFACE *next;
	//variables 
};



class HUD_MENU : public HUD_CONTROL {
public:
	bool open;
	int Xmin, Xmax, Ymin, Ymax;
	HUD_CONTROL *opciones;
	char Dir[256];
	FString Path;

	//HUD_MENU();
	void AddOpcion(char *texto);

};

class HUD_BUTTON : public HUD_CONTROL {
public:
};

class HUD_SLIDER : public HUD_CONTROL {
public:
	float *Bind;
	float valor;
	float vmin;
	float vmax;
	float pos;
	void Update(void);
	//void Render(void);
	void Render(UCanvas *Canvas);

};

class NEURON_REF {
public:
	ANeuron *Neurona;
	ACodeGeneratedNeuron *NeuronaGenerada;
	FVector Pos;
	FRotator Rot;
	float Radio;
	NEURON_REF(ANeuron *neurona, FVector pos, FRotator rot);
	NEURON_REF(ACodeGeneratedNeuron *neurona, FVector pos, FRotator rot);
};


HUD_SLIDER *AddSlider(char *txt, int x, int y, int w, int h, float *bind, float vi, float vmin, float vmax);
HUD_CONTROL *AddControl(char *txt, int x, int y, int w, int h);
void CreateDirMenu(char *menu, char *dir);






/**
 * 
 */
class MULTIUSERTOOL_API LightControls
{
public:
	LightControls();
	~LightControls();
};
