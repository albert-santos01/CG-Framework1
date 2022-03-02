//this var comes from the vertex shader
//they are baricentric interpolated by pixel according to the distance to every vertex
varying vec3 v_wPos;
varying vec3 v_wNormal;

//here create uniforms for all the data we need here

uniform vec3 light_position;
uniform vec3 Id;
uniform vec3 Is;
uniform vec3 camera_position;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float alpha;
uniform vec3 Ia;

void main(){
	
	
	vec3 L = light_position - v_wPos;
	L = normalize(L);
	vec3 V = camera_position - v_wPos;
	V = normalize(V);
	vec3 R = v_wPos - light_position;
	R = normalize(R);
	R = reflect(R,v_wNormal);
	float L_N= dot(L,v_wNormal);
	L_N= clamp( L_N,0.0,1.0);
	float R_V= dot(R,V);
	R_V= clamp( R_V,0.0,1.0);
	R_V = pow(R_V, alpha);
	vec3 Ip= Ka*Ia + Kd*L_N*Id + Ks*R_V*Is;
	vec3 color = Ip;
	gl_FragColor = vec4( color, 1.0 ) * 1.0;
}
