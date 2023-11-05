#version 330 core

uniform sampler2D tex;
uniform vec2 resolution;

void main() {
	gl_FragColor = texture(tex, gl_FragCoord.xy / resolution);
}