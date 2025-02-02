#include <raylib.h>

#include <iostream>

#include "utils.hpp"
#include "node.cpp"
#include "slider.cpp"


#define NODE_VEL_FACTOR 1.5f

#define NODE_COUNT 100

#define NODE_RADIUS     5.0f
#define NODE_RADIUS_VAR 2.5f

#define CONNECTION_WIDTH     2.0f
#define CONNECTION_DISTANCE  150.0f
#define CONNECTION_MAX_ALPHA 255

#define SLIDER_THICKNESS 20.0f


constexpr float SLIDER_RADIUS = SLIDER_THICKNESS / 2.0f;


struct Settings_t
{
    unsigned char NodeMaxAlpha = 255;
} Settings;


int main()
{
    const Vector2 screenResolution = {1300.0f, 900.0f};
    Vector2 nodeAreaResolution     = {900.0f, 800.0f};
    Vector2 sliderAreaResolution   = {900.0f, 800.0f};

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

    std::vector<Node> nodes     = {};
    std::vector<Slider> sliders = {};
    sliders.push_back(Slider(nodeAreaResolution.x + 35, 20, 200, SLIDER_THICKNESS, 0, 255));

    // Add nodes to vector
    for (int i = 0; i < NODE_COUNT; ++i) {
        float radius = NODE_RADIUS + GetRandomValue(0, NODE_RADIUS_VAR);

        nodes.push_back(Node{
            .pos = Vector2{static_cast<float>(GetRandomValue(radius, nodeAreaResolution.x - radius - 1)), static_cast<float>(GetRandomValue(radius, nodeAreaResolution.y - radius - 1))},

            .vel = Vector2{static_cast<float>(GetRandomValue(-100, 100)) * NODE_VEL_FACTOR / 100.0f, static_cast<float>(GetRandomValue(-100, 100)) * NODE_VEL_FACTOR / 100.0f},

            .radius = radius,

            .color = Color{static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), 255}
        });
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(5, 5);

        int connectionCount = 0;

        Settings.NodeMaxAlpha = sliders[0].getValue();

        // Update + draw nodes
        for (size_t i = 0; i < nodes.size(); ++i) {
            Node& current = nodes[i];

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
            SetShaderValue(circleShader, GetShaderLocation(circleShader, "radius"), &current.radius, SHADER_UNIFORM_FLOAT);
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

        for (Slider& slider : sliders) {
            slider.update(mousePos);
            slider.render(texture);
        }

        DrawText(TextFormat("Node Count: %i", static_cast<int>(nodes.size())), 5, nodeAreaResolution.y + 5, 20, WHITE);
        DrawText(TextFormat("Node Connection Count: %i", connectionCount), 5, nodeAreaResolution.y + 25, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
