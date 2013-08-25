#include "..\stdafx.h"

#pragma once

#include "..\Logic\Types.h"
#include "..\Logic\Functions.h"
#include "..\HGE\hgecolor.h"

enum Phis { 
	acceleration,
	hp_max,
	saturation, 
	stamina, 
	fertility,
	live_time,

	energy_max, 
	energy_mature,
	energy_hungry,

	reproduction_cost,
	reproduction_time,
	reproduction_pause,

	end_of_phis
};

enum Soc {
	max_speed,
	rand_way,
	partner,
	cohesion_partner,
	separation_partner,
	alignment_partner,
	enemy,
	cohesion_enemy,
	separation_enemy,
	alignment_enemy,

	end_of_soc
};

class FOV {
protected:
	Type_eye _type;
public:
	int type() {return _type;};
	virtual float angle() = 0;
	virtual float height() = 0;
	virtual float width() = 0;

	virtual FOV* mutation(float coef, Mode_mutation mode) = 0;
};

class FOV_Tri : public FOV {
private:
	float _angle;
	float _height;
	float _width;
public:
	FOV_Tri():
		_angle(0),
		_height(0),
		_width(0) {};
	FOV_Tri(float ang, float hgt, float wdt):
		_angle(ang),
		_height(hgt),
		_width(wdt)
	{_type = TRIANGLE;};
		
	float angle()	{return _angle;};
	float height()	{return _height;};
	float width()	{return _width;};

	FOV* mutation(float coef, Mode_mutation mode) {
		FOV_Tri result;
		if (mode == ONE) {
			int seed = func::randi(0, 2);
			switch(seed) {
			case 0: result._angle = func::getVariation(_angle, coef); break;
			case 1: result._height = func::getVariation(_height, coef); break;
			case 2: result._width = func::getVariation(_width, coef); break;
			}
		} else if (mode == ALL || mode == HALF) {
			result._angle = func::getVariation(_angle, coef);
			result._height = func::getVariation(_height, coef);
			result._width = func::getVariation(_width, coef); 
		}
		return new FOV_Tri(result);
	};
	FOV_Tri hibrid(FOV_Tri eye) {
		FOV_Tri result;
		result._angle = (_angle + eye._angle)/2;
		result._height = (_height + eye._height)/2;
		result._width = (_width + eye._width)/2;
		return result;
	};
};

class FOV_Rad : public FOV {
private:
	float _height;
public:
	FOV_Rad(): 
	  _height(0) {};
	FOV_Rad(float hgt):
		_height(hgt)
	{_type = RADIUS;};

	float angle()	{return 0;};
	float height()	{return _height;};
	float width()	{return 0;};
	void setHeight(float hgt) {_height = hgt;};

	FOV* mutation(float coef, Mode_mutation mode) {
		FOV_Rad result;
		func::getVariation(_height, coef);
		return new FOV_Rad(result);
	};
	FOV_Rad hibrid(FOV_Rad eye) {
		FOV_Rad result;
		result._height = (_height + eye._height)/2;
		return result;
	};
};


class GeneticCode {
public:
	std::vector <float> phis;
	std::vector <std::vector <float> > soc;

	DWORD color;
	FOV_Rad radialEye;
	std::vector <FOV_Tri> eyes;
	Mode_feeding diet;
public:
	GeneticCode() {
		color=0xFFFFFFFF;

		phis.resize(Phis::end_of_phis, 0);

		std::vector <float> one_of_soc;
		one_of_soc.resize(Soc::end_of_soc, 0);
		soc.resize(Individ_status::end_of_status, one_of_soc);
	};

	GeneticCode hibridization(GeneticCode person, Mode_hibrid mode);
	GeneticCode mutation(float coef, Mode_mutation mode);
	bool save(std::string path);
	bool load(std::string path);
};
