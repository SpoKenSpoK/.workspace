uniform sampler2D colorMap;

void main(void)
{
	vec4 tmp = texture2D(colorMap, gl_TexCoord[0].st);
	float grey = 0.299 * tmp.r + 0.587 * tmp.g + 0.114 * tmp.b;
	tmp.rgb = vec3(grey);
	gl_FragColor = tmp;
}
