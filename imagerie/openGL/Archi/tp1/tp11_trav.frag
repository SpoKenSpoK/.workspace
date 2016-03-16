//R. RAFFIN, IUT Aix-Marseille, département Informatique, site d'Arles
//M4104c, ARCHI IN
//romain.raffin[AT]univ-amu.fr

//Fragment Shader tres simple

void main (void) {
// vec4 vectcolor = gl_Color;
// float luminance = 0.299*vectcolor.r + 0.587*vectcolor.g + 0.114*vectcolor.b;
 //Y = 0,299 R + 0,587 G + 0,114 B;
// gl_FragColor.rb = 0.0;
// gl_FragColor.rgb = luminance;

gl_FragColor = gl_Color;	//renvoie la couleur du pipeline
// 	gl_FragColor = vec4(0.0,0.0,1.0,1.0);		//modif de tous les pixels (bleu)
}
