#include "material.h"



Material::Material()
{
	ambient.set(1,1,1); //reflected ambient light
	diffuse.set(1, 1, 1); //reflected diffuse light
	specular.set(1, 1, 1); //reflected specular light
	shininess = 30.0; //glosiness coefficient (plasticity)
}

void Material::uploadToShader(Shader* shader){
	shader->setVector3("Ka", ambient);
	shader->setVector3("Kd", diffuse);
	shader->setVector3("Ks", specular);
	shader->setUniform1("alpha", shininess);
}


