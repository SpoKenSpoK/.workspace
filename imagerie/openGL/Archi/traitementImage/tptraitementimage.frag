//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//Fragment Shader pour le traitement d'images

#define KERNEL_SIZE 9

// Gaussien (flou) !/16 !
// 1 2 1
// 2 4 2
// 1 2 1

// Laplacien (contours)
// 0   1   0
// 1  -4   1
// 0   1   0

//Emboss
// 2   0   0
// 0  -1   0
// 0   0   -1

//Sharpness
// -1   -1   -1
// -1    9   -1
// -1   -1   -1

float kernel[KERNEL_SIZE];

uniform sampler2D colorMap;

const float step_w = 1.0 / 512.0;	//taille de l'image=512, attention en dur !
const float step_h = 1.0 / 512.0;

vec2 offset[KERNEL_SIZE];

void main(void)
{

//le noyau de la convolution, i.e. le masque appliqué aux pixels
//pour l'instant laplacien
kernel[0] = -1.0;
kernel[1] = -1.0;
kernel[2] = -1.0;
kernel[3] = -1.0;
kernel[4] = 9.0;
kernel[5] = -1.0;
kernel[6] = -1.0;
kernel[7] = -1.0;
kernel[8] = -1.0;

//comment sont définis les voisins du pixel courant
offset[0] = vec2(-step_w, -step_h);
offset[1] = vec2(0.0, -step_h);
offset[2] = vec2(step_w, -step_h);
offset[3] = vec2(-step_w, 0.0);
offset[4] = vec2(0.0, 0.0);
offset[5] = vec2(step_w, 0.0);
offset[6] = vec2(-step_w, step_h);
offset[7] = vec2(0.0, step_h);
offset[8] = vec2(step_w, step_h);


	int i = 0;
	vec4 sum = vec4(0.0);

	if(gl_TexCoord[0].s < 0.498) //on n'effectue les opérations que sur les pixels dans la moitié gauche de l'image
	{
		for( i = 0; i < KERNEL_SIZE; i++ )
		{
			vec4 tmp = texture2D(colorMap, gl_TexCoord[0].st + offset[i]);
			//float luminosite = 0.299 * tmp.r + 0.587 * tmp.g + 0.114 * tmp.b;

			//tmp.rgb = vec3(luminosite);

			sum += tmp * kernel[i];
		}
	}
	else if( gl_TexCoord[0].s > 0.502 ) //sur la partie droite, on ne fait rien
	{
		sum = texture2D(colorMap, gl_TexCoord[0].xy);
	}
	else
	{
	sum = vec4(1.0, 1.0, 0.0, 1.0); //au milieu on met des pixels rouges
	}

	gl_FragColor = sum;
}
