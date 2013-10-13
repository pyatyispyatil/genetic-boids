#include "Main_inclusion.h"

#pragma comment (lib, "hge.lib")
#pragma comment (lib, "hgehelp.lib")

#pragma once

#include "GUI\Gui_graphic_area.h"
#include "Logic\Environment.h"

#include "GUI\Gui_win_manager.h"



#define POP_A 100
#define POP_G 40

HGE *hge=0;

Environment env(400, 400);
EditorState	state;

GUI_win_manager *winManager;
GraphicArea		*display;

hgeFont			*fnt;
Pixel			*sliderTexture;
hgeSprite		*testSpr;
hgeQuad			rightBar;

#include "GUI_structure.h"

float			timer = 0;
float			mp_x = 0;
float			mp_y = 0;

void			InitEditor();
void			DoneEditor();
void			InitEnvironment();
void			addIndivid(Vector <int> p, Mode_feeding diet); 

bool FrameFunc() {
	float		dt=hge->Timer_GetDelta();
	hge->Input_GetMousePos(&mp_x, &mp_y);

	state.mp.x = mp_x;
	state.mp.y = mp_y;

	if (state.play) {
		timer+=dt;
		if (timer>0.1) {
			env.step();
			timer=0;
		}
	}
	
	display->Clear();
	std::map <long long int, Individ_Proto*> ::iterator p = env.population.begin();
	while (p != env.population.end()) {
		if (p->second->dna.diet == GETERO) {
			if (p->second->gender == MALE)
				(*display)(p->second->pos) = 0xFF990000;
			if (p->second->gender == FEMALE)
				(*display)(p->second->pos) = 0xFFCC0000;
		} else if (p->second->dna.diet == AUTO) {
			if (p->second->gender == MALE)
				(*display)(p->second->pos) = 0xFF009900;
			if (p->second->gender == FEMALE)
				(*display)(p->second->pos) = 0xFF00CC00;
		}
		p++;
	}

	CheckKeys();
	winManager->Update(dt, state.mp.x, state.mp.y);
	
	if (selectInd->ID != 0) 
		(*display)(selectInd->pos) = 0xFF1111DD;

	display->Update();

	return state.down;
}


bool RenderFunc()
{
	hge->Gfx_Clear(0);
	hge->Gfx_BeginScene();

	display->Render();

	if (state.play) {
		std::vector <std::vector < Vector <int>>> polygons;
		//����������� ����� �������� ���� c ���������� �������� �� �������
		std::map <long long int, Individ_Proto*> ::iterator p = env.population.begin();
		while (p != env.population.end()) {
			//std::vector <FOV_Tri> ::iterator e = p->second.dna.eyes.begin();		
			//while (e != p->second.dna.eyes.end()) {
			//	std::vector < Vector <double> >  poly_to_arr_doub;
			//	std::vector < Vector <int> >  poly_to_arr_int;

			//	//������� ���-�� � ���� ������. � ����, ����� �� ��� �����.
			//	poly_to_arr_doub = e->getPolygon();
			//	for(int i=0; i<3; i++ ) {
			//		poly_to_arr_doub[i].rotate(p->second.way.getDeg()-M_PI/2);
			//		poly_to_arr_doub[i]+=p->second.pos.toDouble();
			//		poly_to_arr_int.push_back(poly_to_arr_doub[i].toInt());
			//	}
			//	polygons.push_back(poly_to_arr_int);
			//	e++;
			//}

			if (display->checkVisiblity(p->second->pos.x, p->second->pos.y)) {
				Cell c = (*display)[p->second->pos.x + p->second->pos.y*env.W()];
				Vector <double> start, end;
				start = c.getCenterPos()-display->getPos().toDouble();
				end = start + p->second->way*20*zoom;
				hge->Gfx_RenderLine(start.x, start.y, end.x, end.y, 0xAA00AA00);
			}
			p++;
		}
	}

	//display->RenderInfo(&polygons);

	//hge->Gfx_RenderQuad(&rightBar);

	winManager->Render();

	hge->Gfx_EndScene();

	return false;
}


void addIndivid(Vector <int> p, Mode_feeding diet) {
	GeneticCode g;

	g.radialEye.setHeight(0);
	g.eyes.push_back(FOV_Tri(0, 30, 20));
	g.diet=diet;
	g.color = 0xFF000000;
	for (int i=0; i<end_of_status; i++) {
		if (diet==AUTO) {
			g.soc[i][max_speed] = 2;
			g.soc[i][libido] = 1;
			g.soc[i][rand_way] = 1;
			g.soc[i][partner] = 1;
			g.soc[i][cohesion_partner] = 1;
			g.soc[i][separation_partner] = 0.2;
			g.soc[i][alignment_partner] = 0.1;
			g.soc[i][enemy] = 2;
			g.soc[i][cohesion_enemy] = -1;
			g.soc[i][separation_enemy] = 1;
			g.soc[i][alignment_enemy] = 1;


			g.phis[acceleration] = 0.2;
			g.phis[hp_max] = 50;
			g.phis[saturation] = 10;
			g.phis[consumption] = 2;
			g.phis[fertility] = 0;
			g.phis[live_time] = 600;

			g.phis[energy_max] = 800;
			g.phis[energy_mature] = 300;
			g.phis[energy_hungry] = 200;

			g.phis[reproduction_cost] = 150;
			g.phis[reproduction_time] = 15;
			g.phis[reproduction_pause] = 100;
		}
		if (diet==GETERO) {
			g.soc[i][max_speed] = 2;
			g.soc[i][libido] = 1;
			g.soc[i][rand_way] = 1;
			g.soc[i][partner] = 1;
			g.soc[i][cohesion_partner] = 1;
			g.soc[i][separation_partner] = 0.5;
			g.soc[i][alignment_partner] = 1.5;
			g.soc[i][enemy] = 3;
			g.soc[i][cohesion_enemy] = 1;
			g.soc[i][separation_enemy] = -1;
			g.soc[i][alignment_enemy] = 0;


			g.phis[acceleration] = 0.2;
			g.phis[hp_max] = 50;
			g.phis[saturation] = 10;
			g.phis[consumption] = 2;
			g.phis[fertility] = 0;
			g.phis[live_time] = 600;

			g.phis[energy_max] = 600;
			g.phis[energy_mature] = 300;
			g.phis[energy_hungry] = 200;

			g.phis[reproduction_cost] = 150;
			g.phis[reproduction_time] = 15;
			g.phis[reproduction_pause] = 100;
		}
	}
	g.soc[0][libido] = 0;
	g.soc[2][libido] = 0;
	g.color = 0xFF009900;
	env.addIndivid(CreateIndivid(p.round(), g));
}

void InitEnvironment() {
	env.setMutation(0, 0, 0, 0, ONE);
	Vector <int> p;

	for(int i=0; i<POP_A; i++) {
		p.x=func::randi(0, env.W()-2);
		p.y=func::randi(0, env.H()-2); 
		addIndivid(p, AUTO);
	}
	for(int i=0; i<POP_G; i++) {
		p.x=func::randi(0, env.W()-2);
		p.y=func::randi(0, env.H()-2); 
		addIndivid(p, GETERO);
	}

	env.step();
	display->Update();
}

void InitEditor() {
	rightBar.v[0].x=600;
	rightBar.v[0].y=0;
	rightBar.v[1].x=800;
	rightBar.v[1].y=0;
	rightBar.v[2].x=800;
	rightBar.v[2].y=600;
	rightBar.v[3].x=600;
	rightBar.v[3].y=800;

	rightBar.v[0].col=
		rightBar.v[1].col=
		rightBar.v[2].col=
		rightBar.v[3].col=0xFF000000;
	rightBar.v[0].z=
		rightBar.v[1].z=
		rightBar.v[2].z=
		rightBar.v[3].z=0.5f;
	rightBar.tex = NULL;
	rightBar.blend=BLEND_COLORMUL | BLEND_ALPHABLEND | BLEND_NOZWRITE;

	fnt = new hgeFont("123.fnt");
	fnt->SetScale(0.5);


	winManager = new GUI_win_manager();
	CreateWinManager();

	display = new GraphicArea(env.W(), env.H(), 800, 600);
}

void DoneEditor() {

}






int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	hge = hgeCreate(HGE_VERSION);

	hge->System_SetState(HGE_INIFILE, "SimpleLi.ini");
	hge->System_SetState(HGE_LOGFILE, "SimpleLi.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_DONTSUSPEND, true);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "SimpleLi");
	hge->System_SetState(HGE_WINDOWED, 1);
	hge->System_SetState(HGE_HIDEMOUSE, 0);
	hge->System_SetState(HGE_FPS, 100);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);
	hge->System_SetState(HGE_USESOUND, false);

	if(hge->System_Initiate()) {
		InitEditor();
		InitEnvironment();
		hge->System_Start();
		DoneEditor();
	} else 
		MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

	hge->System_Shutdown();
	hge->Release();
	return 0;
}