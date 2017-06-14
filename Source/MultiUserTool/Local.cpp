// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapperModule.h"
#include "Funciones.h"
#include "Neuron.h"
#include "MyPaperSpriteActor.h"
#include "CodeGeneratedNeuron.h"
#include "LightControls.h"
#include "MyHUD.h"
//#include "MyPlayerController.h"
//#include "MyPawn.h"
#define SKIP_EXTERNALS
#include "Local.h"
#undef  SKIP_EXTERNALS


HUD_CONTROL *CONTROLS = nullptr;
HUD_CONTROL *NEURON_SLIDERS = nullptr;
HUD_CONTROL *FOREST_SLIDERS = nullptr;
HUD_CONTROL *LIGHTING_SLIDERS = nullptr;
HUD_INTERFACE *INTERFACES = nullptr;
TArray<HUD_CONTROL *> CONTROL_REGISTER;
int NUM_CONTROLS = 0;

float GZOOM = 1.0f;
float GFOV = 60.0f;
float AZM = 0.0f;
float ELV = -45.0f;
float DIS = 1500.0f;
float FPS = 0.0f;
float GDT=0.016f;

//CAMARA PRINCIPAL
FVector CAM_POSITION = FVector(0, 0, 0);
FVector CAM_CX = I;
FVector CAM_CY = J; 
FVector CAM_CZ = K;
float   CAM_FOV=GFOV;

float SCREEN_WIDTH = 1300.0f;
float SCREEN_HEIGHT = 572.0f;
float OX = SCREEN_WIDTH / 2;
float OY = SCREEN_HEIGHT / 2;

//UCanvas *GCANVAS=nullptr;

//float SLIDER[10];

int SELECTED_CONTROL = 0;


float GFMX = 0.0f;
float GFMY = 0.0f;
float GMWHEEL = 0.0f;

bool GLBD = false;
bool GRBD = false;
bool GMBD = false;
bool TECLAS[256];
bool UGLBD = false;
bool UGRBD = false;
bool UGMBD = false;
bool UTECLAS[256];

AMyHUD *GHUD = nullptr;
UCanvas *GCANVAS = nullptr;

UTexture2D *TEXTURA_BOTON = nullptr;
UTexture2D *TEXTURA_TRACKER = nullptr;
UTexture2D *TEXTURA_SLIDER = nullptr;

bool SHOW_HUD_INFO = true;
bool SHOW_HUD_INTERFACE = true;
bool SHOW_WIRES = false;




int NNEUR = 0;
NEURON_REF *NEURONAS[256];
FVector NEURONS_POS[256];
int MAX_NEURONAS = 0;
int SELECTED_NEURON = -1;
int NUM_NEURONAS = 0;
ANeuron **BOSQUE_NEURONAS = nullptr;
AMyPaperSpriteActor **BOSQUE_SPRITE_NEURONAS = nullptr;
int NUM_SPRITE_NEURONAS = 0;



UTexture2D *LoadTextura(FString pngfile)//char *fich)
{
	//TEXTURA *T = NULL;
	//FString pngfile = "myimage.png";
	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	// Note: PNG format.  Other formats are supported
	IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
	UTexture2D *mytex;
	TArray<uint8> RawFileData;// = (uint8 *)malloc(1920 * 1080 * 3);
	if (FFileHelper::LoadFileToArray(RawFileData, *pngfile))
	{
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
		{
			const TArray<uint8>* UncompressedBGRA = NULL;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{
				mytex = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);
				void* TextureData = mytex->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
				mytex->PlatformData->Mips[0].BulkData.Unlock();
				// Update the rendering resource from data.
				mytex->UpdateResource();
				return mytex;
			}
		}
	}
	return nullptr;
}







Local::Local()
{
}

Local::~Local()
{
}
