// Fill out your copyright notice in the Description page of Project Settings.

#include "MultiUserTool.h"
#include "Runtime/Core/Public/HAL/FileManagerGeneric.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapper.h"
#include "Runtime/ImageWrapper/Public/Interfaces/IImageWrapperModule.h"
//#include "SMyCompoundWidget.h"
//#include "SCombo.h"
#include "Neuron.h"
#include "LightControls.h"
#include "Local.h"
#include "MyHUD.h"
#include "SlateChat.h"
#include "XPlayerState.h"

class UMaterial;
int LoadNeuronMeshes();
bool cmpt(char *a, char *b);
extern int MESH_TYPE;
int MATERIAL_TYPE = 1;

bool SHOW_NEURON_SLIDERS = false;
bool SHOW_FOREST_SLIDERS = false;
bool SHOW_LIGHTING_SLIDERS = false;

bool NEW_MATERIAL=false;
UMaterial *THE_NEW_MATERIAL=nullptr;

UMaterial * GetMaterial(int i);

HUD_CONTROL *GET_SELECTED_CONTROL();

// Colors 
const FColor AMyHUD::FColorBlack = FColor(0, 0, 0, 255);
const FColor AMyHUD::FColorRed = FColor(255, 0, 0, 255);
const FColor AMyHUD::FColorGreen = FColor(0,255, 0, 255);
const FColor AMyHUD::FColorYellow = FColor(255, 255, 0, 255);
const FColor AMyHUD::FColorBlue = FColor(0, 0, 255, 255);
const FColor AMyHUD::FColor_White = FColor(255, 255, 255, 255);
// Backgrounds 
const FLinearColor AMyHUD::LC_Black = FLinearColor(0, 0, 0, 1);
const FLinearColor AMyHUD::LC_Pink = FLinearColor(1, 0, 1, 1);
const FLinearColor AMyHUD::LC_Red = FLinearColor(1, 0, 0, 1);
const FLinearColor AMyHUD::LC_Yellow = FLinearColor(1, 1, 0, 1);

float SLIDER[10];

extern HUD_CONTROL *FOREST_SLIDERS;
extern HUD_CONTROL *LIGHTING_SLIDERS;

bool AMyHUD::LoadResources()
{
	static ConstructorHelpers::FObjectFinder<UFont>HUDFontOb(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = HUDFontOb.Object;
	FString TextureFile = "C://texturas/boton001.png";
	TEXTURA_BOTON = LoadTextura(TextureFile);
	TextureFile = "C://texturas/Slider001.png";
	TEXTURA_SLIDER = LoadTextura(TextureFile);
	TextureFile = "C://texturas/boton002.png";
	TEXTURA_TRACKER = LoadTextura(TextureFile);
	return true;
}

void AMyHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Establish the PC
	ThePC = GetOwningPlayerController();

	//How to get a ref to your custom PC
	//AYourPlayerController* YourChar = Cast<AYourPlayerController>(ThePC);

	//How to Get The Character
	//AYourCharacterClass* YourChar = Cast<AYourCharacterClass>(GetOwningPawn());

	//if (GEngine && GEngine->GameViewport) // make sure our screen is ready for the widget
	//{
	//	SAssignNew(MyUIWidget, SSlateChat).OwnerHUD(this); // add the widget and assign it to the var
	//	GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MyUIWidget.ToSharedRef()));
	//}
}

HUD_INTERFACE *AMyHUD::CreateDefaultInterface()
{
	HUD_INTERFACE *Interface = nullptr;
	int W = 150;
	int H = 40;
	int X = SCREEN_WIDTH - W;
	int Y = 0;
	
	AddControl("ESCENA", X, Y, W, H); Y += H;
	AddControl("GENERA", X, Y, W, H); Y += H;
	AddControl("ADDNEU", X, Y, W, H); Y += H;
	AddControl("LOAD", X, Y, W, H); Y += H;
	//AddControl("control 5", X, Y, W, H); Y += H;
	
	Y += 2 * H; W = 300; H = 30;

	//CreateDirMenu("scenas", "scenes");
	/*
	HUD_MENU *m=AddMenu("scenes", 0, 10, 150,40);
	m->AddOpcion("LowRes_4");
	m->AddOpcion("opc2");
	m->AddOpcion("opc3");
	m = AddMenu("menu 2", 150, 10,150, 40);
	m->AddOpcion("opc1");
	m->AddOpcion("opc2");
	m->AddOpcion("opc3");
	m->AddOpcion("opc4");
	m = AddMenu("menu 3", 300, 10, 150, 40);
	m->AddOpcion("opc1");
	m->AddOpcion("opc2");
	m->AddOpcion("opc3");
	m->AddOpcion("opc4");
	*/
	/*
	for (int i = 0; i < 10; i++) SLIDER[i] = 0.0f;
	W = 300; H = 30;
	X = SCREEN_WIDTH - W;
	Y = SCREEN_HEIGHT - 3 * H;
	AddSlider("slider 1", X, Y, W, H, &SLIDER[0], 1.0f, 0.0f, 10.0f); Y += H;
	AddSlider("slider 2", X, Y, W, H, &SLIDER[1], 1.0f, 0.0f, 10.0f); Y += H;
	AddSlider("slider 3", X, Y, W, H, &SLIDER[2], 1.0f, 0.0f, 10.0f); Y += H;
	*/
	return Interface;
}

UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My HUD")
AMyHUD::AMyHUD(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
	// Use the RobotoDistanceField font from the Engine.
	GHUD = this;
	GCANVAS = Canvas;
	if (!ButtonBackground) {
		static ConstructorHelpers::FObjectFinder<UTexture2D> Texture(TEXT("Texture'/Game/Textures/Boton_verde.Boton_verde'"));//ButtonBackground.ButtonBackground
		if (Texture.Object != NULL) {
			ButtonBackground = (UTexture2D*)Texture.Object;
		}
	}
	LoadResources();
	//CreateDefaultInterface();
}

HUD_MENU *MENU_ACTUAL = nullptr;

HUD_CONTROL *NEW_HUD_CONTROL();
HUD_SLIDER *NEW_HUD_SLIDER();
HUD_MENU *NEW_HUD_MENU();

//MACROS QUE FACILITAN LA CRACION DE INTERFACES LIGERAS
#define LINK(c) if(CONTROLS) CONTROLS=c; else ca->next=c; ca=c; 
#define OPEN_BUTTONS(x,y,w,h) { int X=x;int Y=y;int W=w; int H=h; HUD_CONTROL *c=nullptr;HUD_CONTROL *ca=nullptr;
#define ADD_HBUTTON(t) c=AddControl(t,X,Y,W,H);X+=W; LINK(c)
#define ADD_VBUTTON(t) c=AddControl(t,X,Y,W,H);Y+=H; LINK(c)
#define NEW_BUTTON(t) c=AddControl(t,X,Y,W,H);Y+=H;  LINK(c)
#define ADD_HMENU(t) { c=AddMenu(t,X,Y,W,H);Y+=H; LINK(c)
#define ADD_HDMENU(name,dir,ter) c=CreateDirMenu(name,ter,dir,X,Y,W,H);X+=W; LINK(c)
#define END_BUTTONS }

#define OPEN_MENU(menu,x,y,w,h) { int X=x;int Y=y;int W=w; int H=h;  HUD_CONTROL *ca=nullptr; HUD_CONTROL *c=AddMenu(menu,X,Y,W,H);Y+=H;  LINK(c) 
#define ADD_OPTION(t) c=AddOption(t,X,Y,W,H);Y+=H;  //LINK(c)
#define END_MENU  X=UMENU->xmax; Y=UMENU->ymin; UMENU=nullptr;}

//#define DRAW_BUTTON(b,e) DrawControl(b,e); //Y += H;
#define DRAW_CONTROL(c,e) { if(c->tipo==2) DrawMenu((HUD_MENU *)c,e); else DrawControl(c,e);}
#define RENDER_CONTROLS(C) {SELECTED_CONTROL=0; HUD_CONTROL *c=C; while(c) {if(c->IsMouseInside()) DRAW_CONTROL(c,true) else DRAW_CONTROL(c,false) c=c->next;}}

HUD_CONTROL *AMyHUD::AddControl(char *nombre, int x, int y, int w, int h)
{
	HUD_CONTROL *c = NEW_HUD_CONTROL();
	c->tipo = 0;
	c->xmin = x;
	c->ymin = y;
	c->xmax = x + w;
	c->ymax = y + h;
	c->w = w;
	c->h = h;
	c->next = NULL;
	
	sprintf(c->texto, nombre);
	c->text = FString(nombre);// ::Printf(TEXT("MENU %d"), j++);
	float szh, szv;
	Canvas->TextSize(HUDFont, c->text, szh, szv, 1.0f, 1.0f);
	c->dxt =(int)((c->w - szh) / 2.0f);
	c->dyt =(int)((c->h - szv) / 2.0f);
	if (CONTROLS) c->next = CONTROLS;
	CONTROLS = c;
	return c;
}



HUD_MENU *UMENU = nullptr;

HUD_MENU *AMyHUD::AddMenu(char *nombre, int x, int y, int w, int h)
{
	HUD_MENU *c = NEW_HUD_MENU();
	c->tipo = 2;// THUD_MENU;
	c->xmin = x;
	c->ymin = y;
	c->xmax = x + w;
	c->ymax = y + h;
	c->w = w;
	c->h = h;
	c->next = NULL;
	//c->enhance = false;
	
	c->open = false;
	c->opciones = nullptr;
	c->Xmin = c->xmin;
	c->Xmax = c->xmax;
	c->Ymin = c->ymin;
	c->Ymax = c->ymax;
	
	sprintf(c->texto, nombre);
	c->text = FString(nombre);// ::Printf(TEXT("MENU %d"), j++);
	float szh, szv;
	Canvas->TextSize(HUDFont, c->text, szh, szv, 1.0f, 1.0f);
	c->dxt = (int)((c->w - szh) / 2.0f);
	c->dyt = (int)((c->h - szv) / 2.0f);
	if (CONTROLS) c->next = CONTROLS;
	CONTROLS = c;
	UMENU = c;
	return c;
}

HUD_CONTROL *AMyHUD::AddOption(char *nombre, int x, int y, int w, int h)
{
	//HUD_CONTROL *c = AddControl(nombre, x, y, w, h);
	HUD_CONTROL *c = NEW_HUD_CONTROL();
	c->tipo = 4;
	c->xmin = x;
	c->ymin = y;
	c->xmax = x + w;
	c->ymax = y + h;
	c->w = w;
	c->h = h;
	c->next = nullptr;

	sprintf(c->texto, nombre);
	c->text = FString(nombre);// ::Printf(TEXT("MENU %d"), j++);
	float szh, szv;
	Canvas->TextSize(HUDFont, c->text, szh, szv, 1.0f, 1.0f);
	
	c->dxt = (int)((c->w - szh) / 2.0f);
	c->dyt = (int)((c->h - szv) / 2.0f);
	if (szh > w - 2 * h) w = szh + 2 * h;
	c->xmax = x + w;
	c->w = w;
	c->dxt = (int)((c->w - szh) / 2.0f);

	if (UMENU) {
		if (UMENU->opciones!=nullptr) {
			c->next = UMENU->opciones;
		}
		UMENU->opciones = c;
	}
	return c;
}

class FFileManagerGeneric;

HUD_MENU *AMyHUD::CreateDirMenu(char *menu,const char *dir,const char *ter,int X,int Y,int W,int H)
{
	TArray<FString> files;
	IFileManager& FileManager = IFileManager::Get();
	FString FinalPath = FString("C://TFG/scenes");// dir);// "C://TFG/scenes");// "C://TFG/scenes";// +"/" + "*.txt";
	FString Term = FString("*.txt");// "*.txt");// "*.txt";
	FileManager.FindFiles(files, *FinalPath, *Term);// TEXT("/Game/scenes"));
	HUD_MENU *m = AddMenu(menu, X, Y, W, H); Y += H;
	
	for (int i = 0; i < files.Num(); i++) {
		FString S = files[i];
		const TCHAR* aux = *S;
		AddOption(ToChar(aux), X, Y, W, H); Y += H;
	}

	m->Path = FinalPath;
	
	//AddOption("opcion1", X, Y, W, H); Y += H;
	//AddOption("opcion2", X, Y, W, H); Y += H;
	//AddOption("opcion3", X, Y, W, H); Y += H;
	return m;
}


HUD_SLIDER *AMyHUD::AddSlider(char *txt, int x, int y, int w, int h, float *bind, float vmin, float vmax, float vi)
{
	HUD_SLIDER *c = NEW_HUD_SLIDER();
	c->tipo = 1;
	c->xmin = x;
	c->ymin = y;
	c->xmax = x + w;
	c->ymax = y + h;
	c->w = w;
	c->h = h;
	c->Bind = bind;
	c->valor = vi;
	c->vmin = vmin;
	c->vmax = vmax;
	c->next = NULL;
	sprintf(c->texto, txt);
	c->text = FString(txt);// ::Printf(TEXT("%s"), (void *)txt);
	float szh, szv;
	Canvas->TextSize(HUDFont, c->text, szh, szv, 1.0f, 1.0f);
	
	c->dyt = (int)((c->h - szv) / 2.0f);
	c->dxt = 2 * c->dyt;// (int)((c->w - szh) / 2.0f);
	if (CONTROLS) c->next = CONTROLS;
	CONTROLS = c;
	return c;
}

void AMyHUD::DrawMenu(HUD_MENU *c, bool enhance) {
	if (c->open) enhance = true;
	XDrawTile(ButtonBackground, c->xmin, c->ymin, c->w, c->h, FColor::White);
	float escala = 1.0f;
	if (enhance) {
		DrawText(c->text, FColor::Green, c->xmin + c->dxt, c->ymin + c->dyt, HUDFont);
		c->open = true;
	}
	else {
		DrawText(c->text, FColor::White, c->xmin + c->dxt, c->ymin + c->dyt, HUDFont);
	}
	if (c->open) {
		HUD_CONTROL *opcsel = nullptr;
		HUD_CONTROL *opc = c->opciones;
		while (opc) {
			if (opc->IsMouseInside()) opcsel = opc;
			DrawControl(opc, opc->IsMouseInside());
			opc = opc->next;
		}
		if (opcsel == nullptr) {
			c->open = false;
			UMENU = nullptr;
		}
		else {
			UMENU = c;
		}
	}
}

void AMyHUD::RenderControls() 
{
	SELECTED_CONTROL=0; 
	HUD_CONTROL *c=CONTROLS; 
	while(c) {
		switch (c->tipo) {
			case 0: DrawControl(c, c->IsMouseInside()); break;
			//case 1: DrawSlider((HUD_SLIDER *)c, c->IsMouseInside()) break;
			case 2: DrawMenu((HUD_MENU *)c, c->IsMouseInside()); break;
		}
		c=c->next;
	}
}


/*
HUD_MENU *AMyHUD::AddMenu(char *nombre, int x, int y, int w, int h)
{
	HUD_CONTROL *c = AddControl(nombre, x, y, w, h);
	c->tipo = 2;//MENU
	if (CONTROLS) c->next = CONTROLS;
	CONTROLS = c;
	return c;
}
*/



void AMyHUD::BeginPlay()
{
	//LoadResources();
	//CreateDefaultInterface();
	
	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////So far only TSharedPtr<SMyUIWidget> has been created, now create the actual object.
	/////Create a SMyUIWidget on heap, our MyUIWidget shared pointer provides handle to object
	/////Widget will not self-destruct unless the HUD's SharedPtr (and all other SharedPtrs) destruct first.
	//MyUIWidget = SNew(SSlateChat).OwnerHUD(this);
	//Combo001= SNew(SCombo).OwnerHUD(this);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Pass our viewport a weak ptr to our widget
	/////Viewport's weak ptr will not give Viewport ownership of Widget
	//GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MyUIWidget.ToSharedRef()));
	//GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(MyUIWidget.ToSharedRef()));

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/////Set widget's properties as visible (sets child widget's properties recursively)
	//MyUIWidget->SetVisibility(EVisibility::Visible);
	
}

void AMyHUD::OnButtonX()
{
	GZOOM += 1;
}

extern int NUM_TRIANGLES;
void AMyHUD::RenderDebugText(int X, int Y)
{
	int ukey = 0;
	for (int i = (int)'A'; i <= (int)'Z'; i++) {
		if (TECLAS[i] == true) ukey = i;
	}
	for (int i = (int)'0'; i <= (int)'9'; i++) {
		if (TECLAS[i] == true) ukey = i;
	}
	FString zoom = FString::Printf(TEXT("ZOOM = %2.3f FOV= %2.3f FPS=%2.3f"), GZOOM, GFOV, FPS);
	FString mousea = FString::Printf(TEXT("MX=%1.3f MY=%1.3f MW=%1.3f"), GFMX, GFMY, GMWHEEL);
	FString mouse = FString::Printf(TEXT("LBD=%d RBD=%d MBD=%d"), (int)GLBD, (int)GRBD, (int)GMBD);
	FString keys = FString::Printf(TEXT("NUM TRG=%d MATERIAL=%d UKEY=%c"), NUM_TRIANGLES,MATERIAL_TYPE,(char)ukey);
	FString neurons = FString::Printf(TEXT("NUMBER OF NEURONS = %d SELECTED=%d TYPE=%d MAT=%d"), MAX_NEURONAS, SELECTED_CONTROL,MESH_TYPE,MATERIAL_TYPE);// SELECTED_NEURON);
	FString camera = FString::Printf(TEXT("DIS= %2.3f AZM=%1.3f ELV=%1.3f"), DIS, AZM, ELV);
	FString screen = FString::Printf(TEXT("W= %d H=%d"), (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT);
	int DY = 30;
	DrawText(zoom, FColor::White, X, Y, HUDFont); Y += DY;
	DrawText(mouse, FColor::White, X, Y, HUDFont); Y += DY;
	DrawText(mousea, FColor::White, X, Y, HUDFont); Y += DY;
	DrawText(camera, FColor::White, X, Y, HUDFont); Y += DY;
	DrawText(screen, FColor::White, X, Y, HUDFont); Y += DY;
	DrawText(keys, FColor::White, X, Y, HUDFont); Y += DY;
	DrawText(neurons, FColor::White, X, Y, HUDFont); Y += DY;
	//DrawText(zoom + "\n" +mouse+ "\n" + keys + "\n"+ numNeurons, FColor::White, 500, 20, HUDFont);
}



void AMyHUD::DrawCruz(int X, int Y, int sz, FColor color)
{
	Canvas->K2_DrawLine(FVector2D(X - sz, Y), FVector2D(X + sz, Y), 2, color);
	Canvas->K2_DrawLine(FVector2D(X, Y - sz), FVector2D(X, Y + sz), 2, color);
	//Canvas->K2_DrawBox(FVector2D(X-sz,Y-1), FVector2D(2*sz,2), 2);
	//Canvas->K2_DrawBox(FVector2D(X - 1, Y - sz), FVector2D(2,2*sz), 2);
}

int AMyHUD::SelectNeuron()
{
	static int uisel = -1;
	int isel = -1;
	FVector Xp;
	FVector MousePos = FVector(GFMX, GFMY, 0.0f);
	float d, dmin = 100000.0f;
	for (int i = 0; i < MAX_NEURONAS; i++) {
		Xp = Canvas->Project(NEURONAS[i]->Pos);
		d = DXY(Xp, MousePos);
		if (d < dmin) {
			dmin = d;
			isel = i;
		}
	}
	if (uisel >= 0 && uisel != isel) {
		if (NEURONAS[uisel]->Neurona) {
			//NEURONAS[uisel]->Neurona->Enhance(false);
			uisel = -1;
		}
	}
	if (isel >= 0 && isel != uisel) {
		if (NEURONAS[isel]->Neurona) {
			//NEURONAS[isel]->Neurona->Enhance(true);
			uisel = isel;
		}
	}
	return isel;
}

extern bool TRACKING_SLIDER3 = false;

void AMyHUD::UpdateInput()
{
	SCREEN_WIDTH = Canvas->SizeX;
	SCREEN_HEIGHT = Canvas->SizeY;
	OX = SCREEN_WIDTH / 2;
	OY = SCREEN_HEIGHT / 2;

	ThePC->GetMousePosition(GFMX, GFMY);
	
	GLBD = ThePC->IsInputKeyDown(EKeys::LeftMouseButton);
	GRBD = ThePC->IsInputKeyDown(EKeys::RightMouseButton);
	GMBD = ThePC->IsInputKeyDown(EKeys::MiddleMouseButton);
	float AxisValue = ThePC->GetInputAnalogKeyState(EKeys::MouseWheelAxis);
	GZOOM += 0.05f*AxisValue;

	if (ThePC->IsInputKeyDown(EKeys::Z)) GZOOM += 0.1f*GDT;
	if (ThePC->IsInputKeyDown(EKeys::X)) GZOOM -= 0.1f*GDT;
	if (GZOOM < 0.1f) GZOOM = 0.1f;
	if (GZOOM > 10.0f) GZOOM = 10.0f;

	if (GMBD && !TRACKING_SLIDER3) {
		AZM = 180.0f*(GFMX - OX) / OX;
		ELV = 90.0f*(GFMY - OY) / OY;
	}
}

void AMyHUD::DrawMainMenu()
{
	//Background
	DrawMaterialSimple(
		MaterialBackground,
		10, 10,
		256,
		512,
		1.3
	);

	//Menu Title

	//Draw buttons
	//DrawMainMenuButtons();
}






class FFileManagerGeneric;

void AMyHUD::CreateDirMenu2(char *menu, char *dir)
{
	TArray<FString> files;

	//FFileManagerGeneric *Fm=new FFileManagerGeneric();
	IFileManager& FileManager = IFileManager::Get();
	FString FinalPath = "C://TFG/scenes";// +"/" + "*.txt";
	FString Term = "*.txt";
	FileManager.FindFiles(files, *FinalPath, *Term);// TEXT("/Game/scenes"));
	//HUD_MENU *m = AddMenu(menu, 0, 0, 250, 40);
	OPEN_MENU(menu,0, 0, 200, 40)
	for (int i = 0; i < files.Num(); i++) {
		FString S = files[i];
		//const TCHAR* TheMessage = *TheString;
		const TCHAR* aux = *S;// (TCHAR *)(S.GetCharArray());
		char *opc = ToChar(aux);
		ADD_OPTION("Escenas")
		//m->AddOpcion(ToChar(aux));
	}
	END_MENU
}




void AMyHUD::ResizeControls(float neww, float newh)
{
	HUD_CONTROL *c = CONTROLS;
	while (c) {
		c->ReSize(neww, newh);
		c = c->next;
	}
}


void AMyHUD::DrawControls(void)
{
	SELECTED_CONTROL = 0;//NO SELECCIONA CONTROL
	HUD_CONTROL *c = CONTROLS;
	while (c) {
		c->Render(Canvas);
		c = c->next;
	}
}

float AMyHUD::UpdateFps(float DeltaSeconds)
{
	static int frames = 0;
	static float Ti = 0.0f;
	static float T = 0.0f;
	T += DeltaSeconds;
	frames++;
	if (T > 1.0f) {
		FPS = (float)frames / T;
		T = 0.0f;
		frames = 0;
	}
	return FPS;
}

void AMyHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	//GZOOM = MyUIWidget->Slider1->GetValue();
	//AQUI DEBERIA GESTIONAR LOS CONTROLES
	static int frames = 0;
	static int Ti = 0.0f;
	static int T = 0.0f;
	T += DeltaSeconds;
	frames++;
	if (T > 1.0f) {
		FPS = (float)frames / T;
		T = 0.0f;
		frames = 0;
	}
}

ANeuron *AMyHUD::AddNeurona()
{
	float R = 500.0f;
	FVector Pos = RandomVector(R);
	FRotator Rot = RandomRotator(90.0f);
	ANeuron *neurona = GetWorld()->SpawnActor<ANeuron>(ANeuron::StaticClass(), Pos, Rot);
	NEURONAS[MAX_NEURONAS++] = new NEURON_REF(neurona, Pos, Rot);
	return neurona;
}


int SCENE_TO_LOAD = 0;
int NEURONS_TO_SPAW = 0;
int NEURONS_TO_SHOW = 0;
int GEN_NEURONS_TO_SPAW = 0;
bool ADD_NEURON_FOREST = false;
bool ADD_BIG_NEURON_FOREST = false;
bool ADD_IMPOSTORES = false;
bool CLEAN_ALL = false;
bool FREE_NEURON_FOREST = false;
char SCENE_FILE[256];

bool AMyHUD::On(char *txt)
{
	if (SELECTED_CONTROL && ThePC->WasInputKeyJustPressed(EKeys::LeftMouseButton)) {
		if (IS_CONTROL_CLICKED(txt, true)) return true;
	}
	return false;
}

bool AMyHUD::OnMenu(char *txt)
{
	if (UMENU && cmpt(UMENU->texto,txt) && ThePC->WasInputKeyJustPressed(EKeys::LeftMouseButton)) {
		return true;
	}
	return false;
}

/*
bool AMyHUD::OnOption(char *txt)
{
	if (SELECTED_CONTROL && ThePC->WasInputKeyJustPressed(EKeys::LeftMouseButton)) {
		if (IS_CONTROL_CLICKED(txt, true)) return true;
	}
	return false;
}
*/

void AMyHUD::RenderSliders(HUD_CONTROL *S)
{
	HUD_SLIDER *s = (HUD_SLIDER *)S;
	//GLBD= ThePC->IsInputKeyDown(EKeys::LeftMouseButton);
	while (s) {
		//if (tipo == 1) {
		s->Update();
		DrawSlider(s);
		s = (HUD_SLIDER *)s->next;
	}
	//GLBD = false;
}

void AMyHUD::RenderSliders()
{
	extern HUD_CONTROL *NEURON_SLIDERS;
	extern HUD_CONTROL *FOREST_SLIDERS;
	extern HUD_CONTROL *LIGHTING_SLIDERS;
	if(SHOW_NEURON_SLIDERS)		RenderSliders(NEURON_SLIDERS);
	if(SHOW_FOREST_SLIDERS)		RenderSliders(FOREST_SLIDERS);
	if(SHOW_LIGHTING_SLIDERS)	RenderSliders(LIGHTING_SLIDERS);
	/*
	HUD_SLIDER *s=(HUD_SLIDER *)NEURON_SLIDERS;
	//GLBD= ThePC->IsInputKeyDown(EKeys::LeftMouseButton);
	while (s) {
		//if (tipo == 1) {
			s->Update();
			DrawSlider(s);
		s = (HUD_SLIDER *)s->next;
	}
	//GLBD = false;
	*/
}

HUD_CONTROL *AMyHUD::CreateNeuronSliders(int X,int Y)
{
	extern HUD_CONTROL *NEURON_SLIDERS;
	extern float NIVEL_MAXIMO;// = 4.0f;
	extern float BIFURCACIONES;// = 10.0f;
	extern float SINUSOIDAD;// = 0.5f;
	extern float ANGULO;
	extern float ESBELTED;// = 20.0f;
	extern float SIMETRIA;// = 0.5f;
	extern float DENDRITAS;// = 5.0f;
	extern float RADIO_INICIAL;// = 0.3f;
	extern float LOD;
	extern float FOREST_NUMBER;
	extern float FOREST_GAP;
	extern float LIGHT_AZM;
	extern float LIGHT_ELV;
	extern float LIGHT_INT;
	extern float LIGHT_ANG;
	extern float LIGHT_ATN;
	extern float LIGHT_RED;
	extern float LIGHT_GREEN;
	extern float LIGHT_BLUE;
	int W = 300;
	int H = 30;
	HUD_CONTROL *old = CONTROLS;
	CONTROLS = nullptr;
	AddSlider("LOD", X, Y, W, H, &LOD, 0.0f, 10.0f, 0.0f); Y += H;
	AddSlider("Nivel Max", X, Y, W, H, &NIVEL_MAXIMO, 1.0f, 5.0f, 4.0f); Y += H;
	AddSlider("Bifurcaciones", X, Y, W, H, &BIFURCACIONES, 0.0f, 20.0f, 10.0f); Y += H;
	AddSlider("Sinusoidad", X, Y, W, H, &SINUSOIDAD, 0.0f, 1.0f, 0.5f); Y += H;
	AddSlider("Angulo", X, Y, W, H, &ANGULO, 0.0f, 90.0f, 40.0f); Y += H;
	AddSlider("Esbelted", X, Y, W, H, &ESBELTED, 1.0f, 100.0f, 20.0f); Y += H;
	AddSlider("Simetria", X, Y, W, H, &SIMETRIA, 0.0f, 1.0f, 0.5f); Y += H;
	AddSlider("Dendritas", X, Y, W, H, &DENDRITAS, 1.0f, 10.0f, 5.0f); Y += H;
	AddSlider("Radio Raiz", X, Y, W, H, &RADIO_INICIAL, 0.0f, 1.0f, 0.3f); Y += H;
	NEURON_SLIDERS = CONTROLS;
	Y += H;
	CONTROLS = nullptr;
	AddSlider("Forest N", X, Y, W, H, &FOREST_NUMBER, 1.0f, 10.0f, 5.0f); Y += H;
	AddSlider("Forest Gap", X, Y, W, H, &FOREST_GAP, 100.0f, 1000.0f, 300.0f); Y += H;
	FOREST_SLIDERS = CONTROLS;
	Y += H;
	CONTROLS = nullptr;
	AddSlider("Light Azm", X, Y, W, H, &LIGHT_AZM, -180.0f, 180.0f, 0.0f); Y += H;
	AddSlider("Light Elv", X, Y, W, H, &LIGHT_ELV, -90.0f, 90.0f, 0.0f); Y += H;
	AddSlider("Light Int", X, Y, W, H, &LIGHT_INT, 0.0f, 1.0f, 0.5f); Y += H;
	AddSlider("Light Ang", X, Y, W, H, &LIGHT_ANG, 0.0f, 360.0f, 90.0f); Y += H;
	AddSlider("Light Att", X, Y, W, H, &LIGHT_ATN, 0.0f, 90.0f, 0.0f); Y += H;
	LIGHTING_SLIDERS = CONTROLS;
	CONTROLS = old;
	return NEURON_SLIDERS;
}
#define ON(p) if(On(p))
#define ON_MENU(p) if(OnMenu(p))
bool SUBDIVIDIR = false;
extern int NEURONS_TO_SHOW;
extern bool HIDE_NAVIGATOR;

void AMyHUD::DispatchActions()
{
#undef I
	bool clicked = ThePC->WasInputKeyJustPressed(EKeys::LeftMouseButton);

	ON("Show Neurona") NEURONS_TO_SHOW=1;
	ON("Add Neurona") NEURONS_TO_SPAW++;
	ON("Gen Neurona") {
		GEN_NEURONS_TO_SPAW++; 
		SHOW_NEURON_SLIDERS = true;
		SHOW_FOREST_SLIDERS = false;
		SHOW_LIGHTING_SLIDERS = false;
	}
	ON("Random Forest") {
		ADD_NEURON_FOREST = true;
		SHOW_NEURON_SLIDERS = false;
		SHOW_FOREST_SLIDERS = true;
		SHOW_LIGHTING_SLIDERS = false;
	}
	ON("Big Forest")	ADD_BIG_NEURON_FOREST = true;
	ON("Load Forest") {
		sprintf(SCENE_FILE, "%s","C://TFG/scenes/LowRes_4.txt");
		SCENE_TO_LOAD = 1;
	}
	ON("Impostores") ADD_IMPOSTORES = true;
	ON("Clean All") {
		CLEAN_ALL = true;
		SHOW_NEURON_SLIDERS = false;
		SHOW_FOREST_SLIDERS = false;
		SHOW_LIGHTING_SLIDERS = false;
	}
	ON("Show Nav") HIDE_NAVIGATOR = !HIDE_NAVIGATOR;
	ON_MENU("Neurons") {
		HUD_CONTROL *opcsel = GET_SELECTED_CONTROL();
		int j = 1;
		HUD_CONTROL *c = UMENU->opciones;
		while (c) {
			if (c == opcsel) {
				MESH_TYPE = j;
				NEURONS_TO_SHOW = 1;
			}
			j++;
			c = c->next;
		}
	}

	ON_MENU("Materials") {
		HUD_CONTROL *opcsel = GET_SELECTED_CONTROL();
		int j = 0;
		HUD_CONTROL *c = UMENU->opciones;
		while (c) {
			if (c == opcsel) {
				THE_NEW_MATERIAL = GetMaterial(j);
				NEW_MATERIAL = true;
				MATERIAL_TYPE = j;
				//NEURONS_TO_SHOW = 1;
			}
			j++;
			c = c->next;
		}
	}

	ON_MENU("Scenes") {
		HUD_CONTROL *opcsel = GET_SELECTED_CONTROL();
		if (opcsel) {
			sprintf(SCENE_FILE, "C://TFG/scenes/%s", opcsel->texto);
			SCENE_TO_LOAD = 1;
		}
	}

	
	/*if (ThePC->WasInputKeyJustPressed(EKeys::W)) {
		SHOW_WIRES = !SHOW_WIRES;
	}
	if (ThePC->WasInputKeyJustPressed(EKeys::S)) {
		if (SHOW_WIRES) SUBDIVIDIR = true;
	}*/
	if (ThePC->WasInputKeyJustPressed(EKeys::I)) {
		SHOW_HUD_INFO = !SHOW_HUD_INFO;
		SetCursorMoveOnly(false);
	}
	/*if (ThePC->WasInputKeyJustPressed(EKeys::F)) {
		FREE_NEURON_FOREST=true;
	}*/
	if (ThePC->WasInputKeyJustPressed(EKeys::C)) {
		SHOW_HUD_INTERFACE = !SHOW_HUD_INTERFACE;
	}
	if (ThePC->WasInputKeyJustPressed(EKeys::L)) {
		//LoadNeuronMeshes();
		SHOW_LIGHTING_SLIDERS = !SHOW_LIGHTING_SLIDERS;
	}

	/*
	if (ThePC->WasInputKeyJustPressed(EKeys::MidleMouseButton)) {
		CameraAzm = AZM = 180.0f*(GFMX - OX) / OX;
		CameraElv = ELV = 90.0f*(GFMY - OY) / OY;
		CameraFOV = GFOV;
	}
	*/
	if (ThePC->WasInputKeyJustPressed(EKeys::M)) {
		SHOW_HUD_INTERFACE = !SHOW_HUD_INTERFACE;
		SHOW_HUD_INFO = !SHOW_HUD_INFO;
		SetCursorMoveOnly(false);
	}
	if (ThePC->WasInputKeyJustPressed(EKeys::Enter)) {
		if (MyUIWidget.IsValid() && MyUIWidget->ChatInput.IsValid()) {
			FSlateApplication::Get().SetKeyboardFocus(MyUIWidget->ChatInput);
			// When the user presses Enter he will focus his keypresses on the chat input bar
		}
	}
}

void RenderIndexedMesh(UCanvas *canvas,int lod);

void AMyHUD::DrawHUD()
{
	//if (!ThePC) {
		//ThePC = GetOwningPlayerController();
		//AddMessageBP(2, TEXT(""), TEXT("Welcome. Press Enter to chat."), false); 
		// random Welcome message shown to the local player. To be deleted. note type 2 is system message and username is blank
		//return;
	//}

	//Multiplayer Safety Check
	if (!ThePC->PlayerInput) return; //not valid for first seconds of a multiplayer client
	// llamar a la version padre de DrawHUD
	Super::DrawHUD();
	if (!Canvas) return;
	bool RESIZE_SCREEN = false;

	// Conseguimos las dimensiones de la ventana
	FVector2D ScreenDimensions = FVector2D(Canvas->SizeX, Canvas->SizeY);
	if (SHOW_HUD_INTERFACE) {
		if (SCREEN_WIDTH != Canvas->SizeX || SCREEN_HEIGHT != Canvas->SizeY) {
			//RESIZE_SCREEN = true;
			ResizeControls(Canvas->SizeX, Canvas->SizeY);
		}
	}
	UpdateInput();
	
	if (!CONTROLS) {
			OPEN_BUTTONS(SCREEN_WIDTH - 200, 0, 200, 40)
			NEW_BUTTON("Show Neurona")
			NEW_BUTTON("Add Neurona")
			NEW_BUTTON("Gen Neurona")
			NEW_BUTTON("Random Forest")
			NEW_BUTTON("Big Forest")
			NEW_BUTTON("Load Forest")
			NEW_BUTTON("Impostores")
			NEW_BUTTON("Clean All")
			NEW_BUTTON("Show Nav")
			END_BUTTONS
			OPEN_BUTTONS(0, 0, 200, 40)
			//ADD_HDMENU("Neuron Mesh", "C://TFG/neuronas", "*.uasset");
			//ADD_HDMENU("Materials", "C://TFG/materiales", "*.uasset")
			//ADD_HDMENU("Load Forest", "C://TFG/scenes", "*.txt")
			ADD_HMENU("Neurons") 
				ADD_OPTION("neuron006")
				ADD_OPTION("neuron005")
				ADD_OPTION("neuron004")
				ADD_OPTION("neuron003")
				ADD_OPTION("neuron002")
				ADD_OPTION("neuron001")
			END_MENU
			/*ADD_HMENU("Materials")
				ADD_OPTION("material 6")
				ADD_OPTION("material 5")
				ADD_OPTION("material 3")
				ADD_OPTION("material 2")
				ADD_OPTION("material 1")
				ADD_OPTION("material 0")
			END_MENU*/
			//ADD_HDMENU("Scenes", "C://TFG/scenes", "*.txt")
			//ADD_HMENU("Render Options")
			END_BUTTONS
			CreateNeuronSliders(0, 540);
			//CreateDirMenu("escenas","C://TFG/scenes","*.txt");
		/*
		OPEN_BUTTONS(0, 0, 200, 40)
		NEW_BUTTON("Escenas")
		NEW_BUTTON("escena1")
		NEW_BUTTON("escena2")
		END_BUTTONS
		*/
	}




	//Canvas->SetDrawColor(100, 0, 0, 255);

	if (SHOW_WIRES) {
		//RENDER_NEURON_WIRES(Canvas, FColor::Green);
		if (SUBDIVIDIR) {
			RenderIndexedMesh(Canvas, 1);
			SUBDIVIDIR = false;
		}
		else {
			RenderIndexedMesh(Canvas, 0);
		}
	}
	
	if (SHOW_HUD_INTERFACE) {
		FVector Xp;
		if (MAX_NEURONAS) {
			for (int i = 0; i < MAX_NEURONAS; i++) {
				Xp = Canvas->Project(NEURONS_POS[i]);
				DrawCruz(Xp.X, Xp.Y, 20, FColor::Green);
			}
			int j = SelectNeuron();
			if (j >= 0) {
				Xp = Canvas->Project(NEURONS_POS[j]);
				DrawCruz(Xp.X, Xp.Y, 60, FColor::Yellow);
				FString txt = FString::Printf(TEXT("Xp = %2.3f Yp= %2.3f Z= %2.3f"), NEURONS_POS[0].X, NEURONS_POS[0].Y, NEURONS_POS[0].Z);
				DrawText(txt, FColor::Red, 0, 600, HUDFont);
				FString txt2 = FString::Printf(TEXT("Xp = %2.3f Yp= %2.3f Z= %2.3f"), Xp.X, Xp.Y, Xp.Z);
				DrawText(txt2, FColor::Red, 0, 630, HUDFont);
			}
		}
	}

	//Imprimir un texto de prueba en la pantalla
	if (SHOW_HUD_INFO) RenderDebugText(SCREEN_WIDTH-600,SCREEN_HEIGHT-250);
	if (SHOW_HUD_INTERFACE) {
			RENDER_CONTROLS(CONTROLS)
			RenderSliders();
	}
	int mx = (int)GFMX;
	int my = (int)GFMY;
	if (!SELECTED_CONTROL) DrawCruz(mx, my, 30, FColor::Red);
	DispatchActions();

}
/*
void AMyHUD::AddMessageBP(const int32 Type, const FString& Username, const FString& Text, const bool Replicate)
{
	if (!ThePC || !MyUIWidget.IsValid())
		return;

	FSChatMsg newmessage;
	newmessage.Init(Type, FText::FromString(Username), FText::FromString(Text)); // initialize our struct and prep the message
	if (newmessage.Type > 0)
		if (Replicate)
		{
			AXPlayerState* MyPS = Cast<AXPlayerState>(ThePC->PlayerState);
			if (MyPS)
				MyPS->UserChatRPC(newmessage); 
				// Send the complete chat message to the PlayerState so it can be replicated then displayed
		}
		else
			MyUIWidget->AddMessage(newmessage); 
			// Send a local message to this client only, no one else receives it
}*/













