#version 330 core

layout (location = 0) out vec4 FragColor;

uniform sampler2D bloomTex;
uniform sampler2D tex;
uniform vec2 resolution;

uniform float brightness;
uniform float expousure;
uniform float contrast;
uniform float bloomIntensity;

void main() {
	vec2 uv = gl_FragCoord.xy / resolution;
	vec3 col = texture(tex, uv).rgb + texture(bloomTex, uv).rgb * bloomIntensity;

	// post processing
    col.rgb += brightness;
    col.rgb = (col.rgb - 0.5) * (contrast + 1.0) + 0.5;
	col *= pow(2.0, expousure);

	FragColor = vec4(col, 1.0);
}