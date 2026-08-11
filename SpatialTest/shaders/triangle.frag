#version 430

in vec4 AreaColor;
in vec2 TexCoord;

uniform sampler2D textureData;

void main() {
	gl_FragColor = texture(textureData, TexCoord) * AreaColor;
}