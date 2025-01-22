#include "node.hpp"


void Node::updatePos()
{
    Vector2 newPos = { pos.x + vel.x, pos.y + vel.y };

    if (newPos.x < radius || newPos.x >= screenRes->x - radius)
        vel.x = -vel.x;

    if (newPos.y < radius || newPos.y >= screenRes->y - radius)
        vel.y = -vel.y;

    pos.x += vel.x;
    pos.y += vel.y;
}

void Node::push(const Vector2& mousePos)
{
    float dist        = Vector2Dist(pos, mousePos);
    float maxDistance = 300.0f;

    if (dist < maxDistance) {
        const float forceMult = 1.0f - Max(0.0f, dist / maxDistance);
        const float force     = 600.0f * forceMult * forceMult; // Base force * factor

        pos.x = Clamp(pos.x + (pos.x - mousePos.x) * force / (dist * dist), radius, screenRes->x - radius);
        pos.y = Clamp(pos.y + (pos.y - mousePos.y) * force / (dist * dist), radius, screenRes->y - radius);
    }
}