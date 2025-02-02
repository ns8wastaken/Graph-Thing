#include <raylib.h>

#include <iostream>

#include "utils.hpp"
#include "node.cpp"
#include "ui/slider.cpp"
#include "ui/slider_wrapper.cpp"


#define NODE_VEL_FACTOR 1.5f

#define NODE_COUNT 100

#define CONNECTION_WIDTH     2.0f
#define CONNECTION_DISTANCE  150.0f
#define CONNECTION_MAX_ALPHA 255

#define SLIDER_THICKNESS 20.0f


constexpr float SLIDER_RADIUS = SLIDER_THICKNESS / 2.0f;


struct Settings_t
{
    float NodeRadius           = 5.0f;
    unsigned char NodeMaxAlpha = 255;
} Settings;


int main()
{
    const Vector2 screenResolution = { 1300.0f, 900.0f };
    Vector2 nodeAreaResolution     = { 900.0f, 800.0f };
    Vector2 sliderAreaResolution   = { 900.0f, 800.0f };

    Node::screenRes    = &nodeAreaResolution;
    Slider::sliderArea = &sliderAreaResolution;

    InitWindow(static_cast<int>(screenResolution.x), static_cast<int>(screenResolution.y), "Graph Thing");
    SetTargetFPS(60);

    // Slider shader
    Shader sliderShader = LoadShader(0, "src/shaders/rounded_rect.fs");
    SetShaderValue(sliderShader, GetShaderLocation(sliderShader, "screenResolution"), &screenResolution, SHADER_UNIFORM_VEC2);
    SetShaderValue(sliderShader, GetShaderLocation(sliderShader, "radius"), &SLIDER_RADIUS, SHADER_UNIFORM_FLOAT);
    Slider::sliderShader = &sliderShader;

    // Circle shader
    Shader circleShader = LoadShader(0, "src/shaders/circle.fs");
    SetShaderValue(circleShader, GetShaderLocation(circleShader, "screenResolution"), &screenResolution, SHADER_UNIFORM_VEC2);
    Slider::circleShader = &circleShader;

    // Blank texture
    Image img       = GenImageColor(screenResolution.x, screenResolution.y, BLANK);
    Texture texture = LoadTextureFromImage(img);
    UnloadImage(img);

    std::vector<Node> nodes = {};
    SliderWrapper sliderWrapper;
    sliderWrapper.addSlider("Connection Alpha", nodeAreaResolution.x + 35.0f, 20.0f, 200.0f, SLIDER_THICKNESS, 0.0f, 255.0f, 255.0f);
    sliderWrapper.addSlider("Node Radius", nodeAreaResolution.x + 35.0f, 20.0f + 50.0f, 200.0f, SLIDER_THICKNESS, 0.0f, 10.0f, 5.0f);

    // Add nodes
    for (int i = 0; i < NODE_COUNT; ++i) {
        nodes.push_back(Node{
            .pos = Vector2{ static_cast<float>(GetRandomValue(Settings.NodeRadius, nodeAreaResolution.x - Settings.NodeRadius - 1)), static_cast<float>(GetRandomValue(Settings.NodeRadius, nodeAreaResolution.y - Settings.NodeRadius - 1)) },

            .vel = Vector2{ static_cast<float>(GetRandomValue(-100, 100)) * NODE_VEL_FACTOR / 100.0f, static_cast<float>(GetRandomValue(-100, 100)) * NODE_VEL_FACTOR / 100.0f },

            .color = Color{ static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), 255 }
        });
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(5, 5);

        int connectionCount = 0;

        Settings.NodeMaxAlpha = sliderWrapper.getSlider("Connection Alpha").getValue();
        Settings.NodeRadius   = sliderWrapper.getSlider("Node Radius").getValue();

        // Update + draw nodes
        for (size_t i = 0; i < nodes.size(); ++i) {
            Node& current = nodes[i];
            current.radius = Settings.NodeRadius;

            for (size_t j = i + 1; j < nodes.size(); ++j) {
                const Node& other = nodes[j];

                const float dist = Vector2Dist(current.pos, other.pos);

                // Draw connections
                if (dist <= CONNECTION_DISTANCE) {
                    Color color = Color{
                        static_cast<unsigned char>((static_cast<int>(current.color.r) + static_cast<int>(other.color.r)) * 0.5f),
                        static_cast<unsigned char>((static_cast<int>(current.color.g) + static_cast<int>(other.color.g)) * 0.5f),
                        static_cast<unsigned char>((static_cast<int>(current.color.b) + static_cast<int>(other.color.b)) * 0.5f),
                        static_cast<unsigned char>(Settings.NodeMaxAlpha - (dist / CONNECTION_DISTANCE) * Settings.NodeMaxAlpha)
                    };

                    DrawLineEx(current.pos, other.pos, CONNECTION_WIDTH, color);

                    ++connectionCount;
                }
            }

            // Draw circle
            SetShaderValue(circleShader, GetShaderLocation(circleShader, "position"), &current.pos, SHADER_UNIFORM_VEC2);
            SetShaderValue(circleShader, GetShaderLocation(circleShader, "radius"), &Settings.NodeRadius, SHADER_UNIFORM_FLOAT);
            BeginShaderMode(circleShader);
            DrawTexture(texture, 0, 0, current.color);
            EndShaderMode();
            current.updatePos();
        }

        const Vector2 mousePos = GetMousePosition();

        // Push nodes
        if (IsMouseButtonDown(MouseButton::MOUSE_BUTTON_LEFT)) {
            for (Node& node : nodes) {
                node.push(mousePos);
            }
        }

        sliderWrapper.updateAndRender(mousePos, texture);

        DrawText(TextFormat("Node Count: %i", static_cast<int>(nodes.size())), 5, nodeAreaResolution.y + 5, 20, WHITE);
        DrawText(TextFormat("Node Connection Count: %i", connectionCount), 5, nodeAreaResolution.y + 25, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
