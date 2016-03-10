uniform sampler2D colorMap;
uniform float seuil;

void main(void)
{
	vec4 tmp = texture2D(colorMap, gl_TexCoord[0].st);
	float grey = 0.299 * tmp.r + 0.587 * tmp.g + 0.114 * tmp.b;
	if(grey > seuil)
		tmp.rgb = vec3(1.0);
	else
		tmp.rgb = vec3(0.0);
		
	gl_FragColor = tmp;
}
