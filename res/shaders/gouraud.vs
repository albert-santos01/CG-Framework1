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
varying vec3 Ip;

//here create uniforms for all the data we need here

void main()
{	
	//convert local coordinate to world coordinates
	vec3 wPos = (model * vec4( gl_Vertex.xyz, 1.0)).xyz;
	//convert local normal to world coordinates
	vec3 wNormal = (model * vec4( gl_Normal.xyz, 0.0)).xyz;

	//pass them to the pixel shader interpolated
	v_wPos = wPos;
	//v_wNormal = wNormal;

	//in GOURAUD compute the color here and pass it to the pixel shader
	//...
	vec3 L = light_position - wPos;
	L = normalize(L);
	vec3 V = camera_position - wPos;
	V = normalize(V);
	vec3 R = wPos - light_position;
	R = normalize(R);
	R = reflect(R,wNormal);

	float L_N= dot(L,wNormal);
	L_N= clamp( L_N,0.0,1.0);

	float R_V= dot(R,V);
	R_V= clamp( R_V,0.0,1.0);
	R_V = pow(R_V, alpha);

	Ip= Ka*Ia + Kd*L_N*Id + Ks*R_V*Is;


	//project the vertex by the model view projection 
	gl_Position = viewprojection * vec4(wPos,1.0); //output of the vertex shader
}