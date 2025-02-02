#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec2 screenResolution;
uniform vec2 position;
uniform float radius;

out vec4 finalColor;

float CircleSDF(vec2 pos, float radius)
{
    return length(pos) - radius;
}

void main()
{
    // Screenspace position [0; screenSize[
    vec2 ssp = fragTexCoord * screenResolution;

    float dist = CircleSDF(ssp - position, radius);

    vec4 backgroundColor = vec4(0.0);

    finalColor = mix(fragColor, backgroundColor, smoothstep(-1.0, 1.0, dist));
}