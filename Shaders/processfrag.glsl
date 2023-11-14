#version 330 core
uniform sampler2D sceneTex;
uniform int isVertical;
in Vertex{
    vec2 texCoord;
} IN;
out vec4 fragColor;

const float scaleFactors[7] = float[](0.05, 0.1, 0.2, 0.3, 0.2, 0.1, 0.05);

void main(void) {
    vec2 center = vec2(0.5, 0.5);

    float centerRadius = 0.2;

    fragColor = vec4(0, 0, 0, 1);

    vec2 delta = vec2(0, 0);

    if (isVertical == 1) {
        delta = dFdy(IN.texCoord);
    }
    else {
        delta = dFdx(IN.texCoord);
    }

    for (int i = 0; i < 7; i++) {
        vec2 offset = delta * (i - 3);
        vec2 sampleCoord = IN.texCoord.xy + offset;

        vec4 tmp = texture2D(sceneTex, sampleCoord);
        fragColor.rgb += tmp.rgb * scaleFactors[i];

    }
}
