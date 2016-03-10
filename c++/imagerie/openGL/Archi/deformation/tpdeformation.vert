//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//vertex shader pour la déformation complexe

//les valeurs que l'on reçoit du CPU
uniform vec3 Uv3centre_deformation;
uniform vec3 Uv3vecteur_deformation;
uniform float Ufrayon_deformation;

//une valeur que l'on souhaitera faire passer au fragment shader...
varying vec4 VFactif;


void main(void)
{
vec4 point = gl_Vertex;

	//on calcule la distance du centre de la déformation au vertex (point) courant
	float d = distance(vec4(Uv3centre_deformation, 1.0), point);	//distance est une fonction de GLSL (cf. mémo sur le site)

	//si la distance ci-dessus est inférieure au rayon d'action de la contrainte
	//on déforme de manière linéaire (max=centre, min=rayon)
	if (d < Ufrayon_deformation) {
		point.xyz = point.xyz + (1.0 - (d / Ufrayon_deformation)) * Uv3vecteur_deformation.xyz;
		VFactif = vec4(1.0,0.0,0.0,1.0);
	}
	else
		VFactif = vec4(0.0,0.0,1.0,1.0);

	//et on projette ce point
	gl_Position = gl_ModelViewProjectionMatrix * point;
}
