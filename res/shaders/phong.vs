//global variables from the CPU
uniform mat4 model;
uniform mat4 viewprojection;

uniform vec3 light_position;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 camera_position;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float alpha;
uniform vec3 Ia;

//vars to pass to the pixel shader
varying vec3 v_wPos;
varying vec3 v_wNormal;

//here create uniforms for all the data we need here

void main()
{	
	//convert local coordinate to world coordinates
	vec3 wPos = (model * vec4( gl_Vertex.xyz, 1.0)).xyz;
	//convert local normal to world coordinates
	vec3 wNormal = (model * vec4( gl_Normal.xyz, 0.0)).xyz;

	//pass them to the pixel shader interpolated
	v_wPos = wPos;
	v_wNormal = wNormal;


	//project the vertex by the model view projection 
	gl_Position = viewprojection * vec4(wPos,1.0); //output of the vertex shader
}