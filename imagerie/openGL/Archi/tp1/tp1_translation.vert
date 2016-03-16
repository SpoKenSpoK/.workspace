//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//Vertex Shader tres simple : translation (1, 1, 0)

void main(void) {

uniform float chaos;

gl_FrontColor = gl_Color;	//renvoie la couleur de la face avant sur les sommets dans le pipeline

vec4 point = gl_Vertex;
point.x += 1.0;
point.y += 1.0;
//ou point.xy + = vec4(1.0, 1.0);
gl_Position = gl_ModelViewProjectionMatrix * point;

}
