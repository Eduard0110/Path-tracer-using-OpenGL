#version 330

uniform sampler2D tex;
uniform bool horizontal;
uniform int uMaxLod;

uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{   
    int level = uMaxLod;
    vec2 uv = gl_FragCoord.xy / textureSize(tex, 0);
    vec3 result = vec3(0.0);

    while (level >= 0) {
        vec2 tex_offset = 1.0 / textureSize(tex, level); // gets size of single texel
        result += textureLod(tex, uv, level).rgb * weight[0];


        if (horizontal)
        {
            for (int i = 1; i < 5; ++i)
            {
                result += textureLod(tex, uv + vec2(tex_offset.x * i, 0.0), level).rgb * weight[i];
                result += textureLod(tex, uv - vec2(tex_offset.x * i, 0.0), level).rgb * weight[i];
            }
        }
        else
        {
            for (int i = 1; i < 5; ++i)
            {
                result += textureLod(tex, uv + vec2(0.0, tex_offset.y * i), level).rgb * weight[i];
                result += textureLod(tex, uv - vec2(0.0, tex_offset.y * i), level).rgb * weight[i];
            }
        }

        level--;
    }

    vec3 fColor = vec3(result) / (uMaxLod + 1);
    gl_FragColor = vec4(fColor, 1.0);
}