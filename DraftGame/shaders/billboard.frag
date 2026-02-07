#version 410

in vec2 TexCoords;

uniform sampler3D uTextureData;

void main() {
	gl_FragColor = texture2D(uTextureData, TexCoords);

	if (gl_FragColor.r == 0
	&&	gl_FragColor.g == 0 
	&&	gl_FragColor.b == 0) discard;
}