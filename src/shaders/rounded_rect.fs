#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec2 screenResolution;
uniform vec4 rectangle;
uniform float radius;

out vec4 finalColor;

float RectSDF(vec2 pos, vec2 size, float radius)
{
    vec2 dist = abs(pos) - size + vec2(radius);
    return min(max(dist.x, dist.y), 0.0) + length(max(dist, 0.0)) - radius;
}

void main()
{
    // Center position of the frame
    vec2 centerPos = rectangle.xy + rectangle.zw / 2.0;

    // Screenspace position [0; screenSize[
    vec2 ssp = fragTexCoord * screenResolution;

    float dist = RectSDF(ssp - centerPos, rectangle.zw / 2.0, radius);

    vec4 backgroundColor = vec4(0.0);

    finalColor = mix(fragColor, backgroundColor, smoothstep(-1.0, 1.0, dist));
}