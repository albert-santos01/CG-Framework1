#include "light.h"




Light::Light()
{
	position.set(50, 50, 0);
	diffuse_color.set(0.6f,0.6f,0.6f);
	specular_color.set(0.6, 0.6, 0.6);
}
Light::Light(Vector3 pos, Vector3 diffuse_c, Vector3 spec_col){
	position = pos;
	diffuse_color = diffuse_c;
	specular_color = spec_col;
	//position.set(50, 50, 0);
	//diffuse_color.set(0.6f,0.6f,0.6f);
	//specular_color.set(0.6, 0.6, 0.6);
}
void Light::uploadToShader(Shader* shader){
	shader->setVector3("light_position", position);
	shader->setVector3("Id", diffuse_color);
	shader->setVector3("Is", specular_color);
}

