//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//Vertex Shader tres simple

uniform float chaosOne;
uniform float chaosTwo;


void main(void) {
	vec4 point = gl_Vertex;

	point.x *= chaosOne;
	point.y += chaosTwo;

	gl_Position = gl_ModelViewProjectionMatrix * point;
	// gl_Position = ftransform(); fonctionne aussi
	gl_FrontColor = gl_Color ;
	//gl_Position = ftransform () ;

}
