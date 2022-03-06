#include "application.h"
#include "utils.h"
#include "includes.h"
#include "utils.h"



Camera* camera = NULL;
Mesh* mesh = NULL;
Shader* gouraud = NULL;
Shader* phong = NULL;
//might be useful...
Material* material = NULL;
std::vector <Light*> lights;
std::vector <Entity*> entities;

Shader* phong_shader = NULL;
Shader* gouraud_shader = NULL;
Light* light = NULL;
int r = 0;
Vector3 ambient_light(0.1, 0.2, 0.3); //here we can store the global ambient light of the scene
Vector3 backgound_color(0, 0, 0);
float angle = 0;
float angle_xz = PI * 1.5;
float angle_yz = 0;
//float angle_xz = 0;
float init_x = 0;
float init_y = 50.0;
float init_z = 50.0;
int max_entities;


Application::Application(const char* caption, int width, int height)
{
	this->window = createWindow(caption, width, height);

	// initialize attributes
	// Warning: DO NOT CREATE STUFF HERE, USE THE INIT 
	// things create here cannot access opengl
	int w,h;
	SDL_GetWindowSize(window,&w,&h);

	this->window_width = w;
	this->window_height = h;
	this->keystate = SDL_GetKeyboardState(NULL);
}

//Here we have already GL working, so we can create meshes and textures
void Application::init(void)
{
	std::cout << "initiating app..." << std::endl;
	
	//here we create a global camera and set a position and projection properties
	camera = new Camera();
	camera->lookAt(Vector3(0,50,50),Vector3(0,0,0),Vector3(0,1,0));
	printf("(%f) * cos(PI / 30) - (%f) * sin(PI / 30), 0.0, (%f)* sin(PI / 30) + (%f)* cos(PI / 30)); = (%f,%f)\n", camera->eye.x, camera->eye.z, camera->eye.x, camera->eye.z, (camera->eye.x) * cos(PI / 30) - (camera->eye.z) * sin(PI / 30), camera->eye.x * sin(PI / 30) + (camera->eye.z) * cos(PI / 30));
	camera->setPerspective(60,window_width / window_height,0.1,10000);
	r = camera->eye.distance(camera->center);
	//then we load a mesh
	mesh = new Mesh();
	if( !mesh->loadOBJ( "../res/meshes/lee.obj" ) )
		std::cout << "FILE Lee.obj NOT FOUND " << std::endl;

	//we load one or several shaders...

	//where to start:
	gouraud = Shader::Get("../res/shaders/gouraud.vs", "../res/shaders/gouraud.fs");

	phong = Shader::Get( "../res/shaders/phong.vs", "../res/shaders/phong.fs" );
	//where to start
	
	//load your Gouraud and Phong shaders here and stored them in some global variables
	//...

	//CODE HERE:
	//create a light (or several) and and some materials
	//...
	light = new Light();
	Light* light2 = new Light(Vector3(-50, -50, 0), Vector3(1.0, 0.0, 0.0), Vector3(0.2, 0.4, 0.6));
	Light* light3 = new Light(Vector3(-50, 50, 10), Vector3(0.2, 0.4, 0.6), Vector3(1.0, 1.0, 1.0));
	material = new Material();
	
	/*Matrix44 model_matrix;
	model_matrix.setIdentity();*/
	max_entities = 5;
	for (int i = 0; i < max_entities; i++) {
		entities.push_back(new Entity(mesh, material));
		entities[i]->set_position(((i % 5) - 2) * 22, ((int)(floor(i / 5)) % 3) * 22, -(int)floor(i / 15) * 22);
	}
	

	/*lights.push_back(light1);
	lights.push_back(light2);
	lights.push_back(light3);*/
	lights = {light,light2,light3};
}

//render one frame
void Application::render(void)
{
	//update eye position
	/*camera->eye.x = init_x * cos(angle_xz) - init_z * sin(angle_xz);
	camera->eye.z = -(init_x * sin(angle_xz) - init_z * cos(angle_xz));

	camera->eye.y = init_y * cos(angle_yz) - init_z * sin(angle_yz);
	camera->eye.z = init_y * sin(angle_yz) - init_z * cos(angle_yz);*/

	/*camera->eye.x = camera->eye.x * cos(angle_xz) - camera->eye.z * sin(angle_xz);
	camera->eye.z = -(camera->eye.x * sin(angle_xz) - camera->eye.z * cos(angle_xz));*/


	//update the aspect of the camera acording to the window size
	camera->aspect = window_width / window_height;
	camera->updateProjectionMatrix();
	//Get the viewprojection matrix from our camera
	Matrix44 viewprojection = camera->getViewProjectionMatrix();
	float* view = camera->view_matrix.m;

	/*if (select == 3) {
		printf("-----------------------------RENDER----------------------------\n\n\n");
		for (int i = 0; i < 16; i++) {
			printf("m[%d] = %f\t", i, view[i]);
			if (i % 4 == 3 && i != 0) {
				printf("\n");
			}
		}
		printf("\n\n");
		printf("---------------------------------------------------------------\n\n\n");
	}*/

	//set the clear color of the colorbuffer as the ambient light so it matches
	glClearColor(backgound_color.x, backgound_color.y, backgound_color.z, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear framebuffer and depth buffer 
	glEnable( GL_DEPTH_TEST ); //enable depth testing for occlusions
	glDepthFunc(GL_LEQUAL); //Z will pass if the Z is LESS or EQUAL to the Z of the pixel

	//choose a shader and enable it
	//shader->enable();
	Matrix44 model_matrix;
	model_matrix.setIdentity();
	model_matrix.translate(0,0,0); //example of translation
	model_matrix.rotate(angle, Vector3(0, 1, 0));


	if (select==1){
		gouraud->enable();
		gouraud->setMatrix44("model", model_matrix); //upload the transform matrix to the shader
		gouraud->setMatrix44("viewprojection", viewprojection); //upload viewprojection info to the shader
		gouraud->setVector3("camera_position", camera->eye);

		light->uploadToShader(gouraud);
		material->uploadToShader(gouraud);
		gouraud->setVector3("Ia", ambient_light);


		mesh->render(GL_TRIANGLES);
		//disable shader when we do not need it any more
		//shader->disable();
		gouraud->disable();
	}

	else if (select == 2) {
		phong->enable();
		phong->setMatrix44("model", model_matrix); //upload the transform matrix to the shader
		phong->setMatrix44("viewprojection", viewprojection); //upload viewprojection info to the shader
		phong->setVector3("camera_position", camera->eye);

		light->uploadToShader(phong);
		material->uploadToShader(phong);
		phong->setVector3("Ia", ambient_light);
		//do the draw call into the GPU
		mesh->render(GL_TRIANGLES);
		phong->disable();
	}

	else if (select == 3) {
		phong->enable();
		phong->setMatrix44("model", model_matrix); //upload the transform matrix to the shader
		phong->setMatrix44("viewprojection", viewprojection); //upload viewprojection info to the shader
		phong->setVector3("camera_position", camera->eye);
		phong->setVector3("Ia", ambient_light);
		
		for (Light*light:lights){
			light->uploadToShader(phong);
			material->uploadToShader(phong);
			//do the draw call into the GPU
			mesh->render(GL_TRIANGLES);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			phong->setVector3("Ia", Vector3(0,0,0));

		}
		
		glDisable(GL_BLEND);

		phong->disable();
	}

	else if (select == 4) {
		phong->enable();
		phong->setMatrix44("viewprojection", viewprojection); //upload viewprojection info to the shader
		phong->setVector3("camera_position", camera->eye);
		phong->setVector3("Ia", ambient_light);
		glDisable(GL_BLEND);
		
		for (Light* light : lights) {
			for (Entity* entity : entities) {
				light->uploadToShader(phong);
				entity->set_entity(phong);

			}
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			
			phong->setVector3("Ia", Vector3(0, 0, 0));
		}

		glDisable(GL_BLEND);

		phong->disable();
	}
	

	//CODE HERE: pass all the info needed by the shader to do the computations
	//send the material and light uniforms to the shader
	//...

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

//called after render
void Application::update(double seconds_elapsed)
{
	if (keystate[SDL_SCANCODE_SPACE]){
		for (int i = 0; i < max_entities; i++) {
			entities[i]->model.rotate(-angle, Vector3(0, 1, 0));
		}
		angle += seconds_elapsed;
		for (int i = 0; i < max_entities; i++) {
			entities[i]->model.rotate(angle, Vector3(0, 1, 0));
		}
	}

	if (keystate[SDL_SCANCODE_D])
		camera->eye = camera->eye + Vector3(1, 0, 0) * seconds_elapsed * 10.0;
	else if (keystate[SDL_SCANCODE_A])
		camera->eye = camera->eye + Vector3(-1, 0, 0) * seconds_elapsed * 10.0;
	if (keystate[SDL_SCANCODE_W])
		camera->eye = camera->eye + Vector3(0, 1, 0) * seconds_elapsed * 10.0;
	else if (keystate[SDL_SCANCODE_S])
		camera->eye = camera->eye + Vector3(0, -1, 0) * seconds_elapsed * 10.0;

	if (keystate[SDL_SCANCODE_RIGHT]){
		camera->eye = Vector3((camera->eye.x) * cos(-seconds_elapsed) - (camera->eye.z) * sin(-seconds_elapsed), camera->eye.y, camera->eye.x * sin(-seconds_elapsed) + (camera->eye.z) * cos(-seconds_elapsed));
		angle_xz += seconds_elapsed;
		printf("(%f) * cos(PI / 30) - (%f) * sin(PI / 30), 0.0, (%f)* sin(PI / 30) + (%f)* cos(PI / 30)); = (%f,%f)\n", camera->eye.x,camera->eye.z, camera->eye.x, camera->eye.z, (camera->eye.x) * cos(PI / 30) - (camera->eye.z) * sin(PI / 30), camera->eye.x * sin(PI / 30) + (camera->eye.z) * cos(PI / 30));
		//camera->eye.z = -(init_x * sin(angle_xz) - init_z * cos(angle_xz));

		//camera->eye = camera->eye + Vector3((r)*cos(angle_xz), 0.0, (r)*sin(angle_xz)) * seconds_elapsed * 10.0;
		//camera->rotate(angle_xz, Vector3(1, 0, 0));
		/*camera->view_matrix.translate(camera->eye.x + seconds_elapsed*10, 0.0, 0.0);
		printf("-------------------------------CAMARA---------------------------------------\n\n");
		printf("e.x = %f, e.y = %f, e.z = %f\n\n", camera->eye.x, camera->eye.y, camera->eye.z);
		camera->updateViewMatrix(); 
		float* view= camera->view_matrix.m;
		printf("-----------------------------UPDATE----------------------------\n\n\n");
		for (int i = 0; i < 16; i++) {
			printf("[%f]",view[i]);
			if (i%4==3 && i!=0){
				printf("\n");

			}
		}
		printf("---------------------------------------------------------------\n\n\n");*/

		
	}
	else if (keystate[SDL_SCANCODE_LEFT]) {
		camera->eye = Vector3((camera->eye.x) * cos(seconds_elapsed) - (camera->eye.z) * sin(seconds_elapsed), camera->eye.y, camera->eye.x * sin(seconds_elapsed) + (camera->eye.z) * cos(seconds_elapsed));
		angle_xz -= seconds_elapsed;
		//camera->eye= camera->eye - Vector3((r)*cos(angle_xz), 0, r*sin(angle_xz));
		//camera->rotate(angle_xz, Vector3(1, 0, 0));
		/*camera->view_matrix.translate(camera->eye.x - seconds_elapsed, 0.0, 0.0);
		camera->updateViewMatrix();*/
		//camera->eye.z = -(init_x * sin(angle_xz) - init_z * cos(angle_xz));
	}

	if (keystate[SDL_SCANCODE_UP]) {
		
		camera->eye = Vector3(camera->eye.x, camera->eye.z * sin(seconds_elapsed) + (camera->eye.y) * cos(seconds_elapsed), (camera->eye.z) * cos(seconds_elapsed) - (camera->eye.y) * sin(seconds_elapsed)); 
		angle_yz += seconds_elapsed;
	}
	else if (keystate[SDL_SCANCODE_DOWN]) {
		camera->eye = Vector3(camera->eye.x, camera->eye.z * sin(-seconds_elapsed) + (camera->eye.y) * cos(-seconds_elapsed), (camera->eye.z) * cos(-seconds_elapsed) - (camera->eye.y) * sin(-seconds_elapsed));
		angle_yz -= seconds_elapsed;
	}
}

//keyboard press event 
void Application::onKeyPressed( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: exit(0); break; //ESC key, kill the app
		case SDLK_r: 
			Shader::ReloadAll();
			break; //ESC key, kill the app

		case SDLK_1: select = 1; break;
		case SDLK_2: select = 2; break;
		case SDLK_3: select = 3; break;
		case SDLK_4: select = 4; break;
		case SDLK_TAB: 
			lights.push_back(new Light(Vector3((rand() % 88) - 44, (rand() % 44) - 22, (rand() % 10)-5), Vector3((float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX)), Vector3((float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX), (float)(rand()) / (float)(RAND_MAX))));
				break;
		case SDLK_PLUS: 
			max_entities++; 
			entities.push_back(new Entity(mesh, material));
			entities[max_entities-1]->set_position((((max_entities - 1) % 5) - 2) * 22, (((int)floor((max_entities - 1)/5)) % 3) * 22, -(int)floor((max_entities - 1) / 15) * 22);
			break;
		case SDLK_7: camera->eye = Vector3(0.0, 20.0, -20.0); break;
		case SDLK_8: camera->eye = Vector3(0.0, 20.0, 20.0); break;
		case SDLK_9: camera->eye = Vector3(0.0, -20.0, -20.0); break;

	}
}

//mouse button event
void Application::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse pressed
	{
	}

}

void Application::onMouseButtonUp( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_LEFT) //left mouse unpressed
	{

	}
}

//when the app starts
void Application::start()
{
	std::cout << "launching loop..." << std::endl;
	launchLoop(this);
}

Entity::Entity(Mesh* msh, Material* mat) {
	mesh = msh;
	material = mat;
	model.setIdentity();
}


void Entity::set_entity(Shader* shader) {
	material->uploadToShader(shader),
	shader->setMatrix44("model", model);
	mesh->render(GL_TRIANGLES);

}
void Entity::set_position(int x, int y, int z){
	model.translate(x,y,z);
	model.rotate(angle, Vector3(0, 1, 0));
}
