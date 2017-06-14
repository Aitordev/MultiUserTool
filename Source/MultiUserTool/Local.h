// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#ifndef SKIP_EXTERNALS
#define COMPILE_EXTERNALS
#endif

#ifdef COMPILE_EXTERNALS
struct FColor;
struct FVector;
struct FRotator;

class FString;
class UCanvas;
class UTexture2D;
class AMyHUD;
class ANeuron;
class AMyPaperSpriteActor;
//class ACodeGeneratedNeuron;
class HUD_CONTROL;
class HUD_INTERFACE;
class HUD_MENU;
class HUD_SLIDER;
class NEURON_REF;



extern UCanvas *GCANVAS;
extern AMyHUD *GHUD;

extern bool SHOW_HUD_INFO;
extern bool SHOW_HUD_INTERFACE;
extern bool SHOW_WIRES;


extern HUD_INTERFACE *INTERFACES;
extern HUD_CONTROL *CONTROLS;
extern TArray<HUD_CONTROL *> CONTROL_REGISTER;
extern int NUM_CONTROLS;
extern int SELECTED_CONTROL;



//CAMERA
extern float GZOOM;
extern float GFOV;
extern float AZM;
extern float ELV;
extern float DIS;
extern float FPS;
extern float GDT;
extern FVector CAM_POSITION, CAM_CX, CAM_CY, CAM_CZ;
extern float CAM_FOV;


#define FIXED_AIM   0
#define FLIGHT		1
//SCREEN
extern float SCREEN_WIDTH;
extern float SCREEN_HEIGHT;
extern float OX;
extern float OY;

//INTERFACE KB & MOUSE
extern float GFMX, GFMY, GMWHEEL;
extern bool GLBD, GRBD, GMBD;
extern bool TECLAS[256];
extern bool UGLBD, UGRBD, UGMBD;
extern bool UTECLAS[256];


extern UTexture2D *TEXTURA_BOTON;
extern UTexture2D *TEXTURA_TRACKER;
extern UTexture2D *TEXTURA_SLIDER;

//NEURONAS
extern int NNEUR;
extern int MAX_NEURONAS;
extern NEURON_REF *NEURONAS[256];
extern FVector NEURONS_POS[256];
extern int NUM_NEURONAS;
extern ANeuron **BOSQUE_NEURONAS;
extern int SELECTED_NEURON;
extern AMyPaperSpriteActor **BOSQUE_SPRITE_NEURONAS;
extern int NUM_SPRITE_NEURONAS;


//FUNCIONES LOCALES
void RENDER_NEURON_WIRES(UCanvas *Canvas, FColor color);
bool IS_CONTROL_CLICKED(char *name, bool clicked);
bool GET_CONTROL_CLICKED(char *file, bool clicked);
bool GET_MENU_OPTION(char *path, char *opcion, bool clicked);
UTexture2D *LoadTextura(FString pngfile);

#endif


/**
 * 
 */
class MULTIUSERTOOL_API Local
{
public:
	Local();
	~Local();
};
