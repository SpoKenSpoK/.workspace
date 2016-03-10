//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//Vertex Shader tres simple : translation (1, 1, 0)
//uniform float cputime;

uniform float chaos;


void main(void) {

gl_FrontColor = gl_Color;	//renvoie la couleur de la face avant sur les sommets dans le pipeline
// gl_Position = ftransform();

vec4 v = vec4(gl_Vertex);

	v.z = sin(5.0*v.x + gputime)*0.5;

	gl_Position = gl_ModelViewProjectionMatrix * v;

 /* Translation
vec4 point = gl_Vertex;
point.x = point.x + 1.0;
point.y = point.y + 1.0;

gl_Position = gl_ModelViewProjectionMatrix * point;
*/
}
