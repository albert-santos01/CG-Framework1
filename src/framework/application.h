/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the application, is in charge of creating the data, getting the user input, process the update and render.
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include "includes.h"
#include "framework.h"

#include "image.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "material.h"
#include "light.h"

class Application
{
public:


	//window
	SDL_Window* window;
	float window_width;
	float window_height;

	float time;
	int select;

	//keyboard state
	const Uint8* keystate;

	//mouse state
	int mouse_state; //tells which buttons are pressed
	Vector2 mouse_position; //last mouse position
	Vector2 mouse_delta; //mouse movement in the last frame

	//constructor
	Application(const char* caption, int width, int height);

	//main methods
	void init( void );
	void render( void );
	void update( double dt );

	//methods for events
	void onKeyPressed( SDL_KeyboardEvent event );
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp( SDL_MouseButtonEvent event );

	//other methods to control the app
	void setWindowSize(int width, int height) {
		glViewport( 0,0, width, height );
		this->window_width = width;
		this->window_height = height;
	}

	Vector2 getWindowSize()
	{
		int w,h;
		SDL_GetWindowSize(window,&w,&h);
		return Vector2(w,h);
	}

	void start();
};

class Entity {
public:
	Mesh* mesh;
	Material* material;
	Matrix44 model;
	
	Entity(Mesh* msh, Material* mat);

	void set_entity(Shader* shader);

	void set_position(int x, int y, int z);

};

#endif 
