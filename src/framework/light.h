#pragma once

#include "framework.h"
#include <shader.h>

//This class contains all the info about the properties of the light
class Light
{
public:

	//you can access this variables directly, do not need a setter/getter

	Vector3 position; //where is the light
	Vector3 diffuse_color; //the amount (and color) of diffuse
	Vector3 specular_color; //the amount (and color) of specular

	static std::vector <Light> lights;

	Light();
	Light(Vector3 pos, Vector3 diffuse_c, Vector3 spec_col);
	//possible method: uploads properties to shader uniforms
	void uploadToShader( Shader* shader);
	
};

