//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//Vertex Shader tres simple : passage d'un float uniform et deformation
uniform float time;


void main(void) {

	gl_FrontColor = gl_Color;	//renvoie la couleur de la face avant sur les sommets dans le pipeline

	vec4 v = vec4(gl_Vertex);

	v.z = sin(5.0*v.x + time)*0.5;
	gl_Position = gl_ModelViewProjectionMatrix * v;
}
