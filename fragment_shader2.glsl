#version 330

out vec4 FragColor;

in vec4 outcol;


void main()
{
	gl_FragColor  = vec4(outcol.r, outcol.g,outcol.b, outcol.a);
} 
