#include <raylib.h>
#include <raymath.h>

#include "node.hpp"


#define NODE_MAX_VEL 100.0f

#define NODE_COUNT 100

#define NODE_RADIUS          5.0f
#define NODE_RADIUS_VAR      2.5f

#define CONNECTION_WIDTH     2.0f
#define CONNECTION_DISTANCE  200.0f
#define CONNECTION_MAX_ALPHA 255


int main()
{
    Vector2 screenResolution = {1200.0f, 800.0f};
    Vector2 nodeAreaResolution = {1000.0f, 800.0f};

    InitWindow(static_cast<int>(screenResolution.x), static_cast<int>(screenResolution.y), "Graph Thingy");
    SetTargetFPS(60);

    std::vector<Node> nodes = {};

    for (int i = 0; i < NODE_COUNT; ++i) {
        float radius = NODE_RADIUS + GetRandomValue(0, NODE_RADIUS_VAR);

        nodes.push_back(Node{
            .pos = Vector2{static_cast<float>(GetRandomValue(radius, nodeAreaResolution.x - radius - 1)), static_cast<float>(GetRandomValue(radius, nodeAreaResolution.y - radius - 1))},

            .vel = Vector2{static_cast<float>(GetRandomValue(-100, 100)) / 100.0f, static_cast<float>(GetRandomValue(-100, 100)) / 100.0f},

            .radius = radius,

            .color = Color{static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), static_cast<unsigned char>(GetRandomValue(0, 255)), 255}
        });
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(5, 5);

        for (size_t i = 0; i < nodes.size(); ++i) {
            Node& current = nodes[i];

            for (size_t j = i + 1; j < nodes.size(); ++j) {
                const Node& other = nodes[j];

                const float dist = Vector2Distance(current.pos, other.pos);

                // Draw connections
                if (dist <= CONNECTION_DISTANCE) {
                    Color color = Color{
                        static_cast<unsigned char>((static_cast<int>(current.color.r) + static_cast<int>(other.color.r)) * 0.5f),
                        static_cast<unsigned char>((static_cast<int>(current.color.g) + static_cast<int>(other.color.g)) * 0.5f),
                        static_cast<unsigned char>((static_cast<int>(current.color.b) + static_cast<int>(other.color.b)) * 0.5f),
                        static_cast<unsigned char>(CONNECTION_MAX_ALPHA - (dist / CONNECTION_DISTANCE) * CONNECTION_MAX_ALPHA)
                    };

                    DrawLineEx(current.pos, other.pos, CONNECTION_WIDTH, color);
                }
            }

            // Draw circle
            DrawCircleV(current.pos, current.radius, current.color);
            current.updatePos(nodeAreaResolution);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
