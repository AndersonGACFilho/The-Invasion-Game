#include "entities/Entity.h"

bool Entity::intersects(const Entity& other) const
{
    return m_x + m_edgeX > other.m_x - other.m_edgeX
        && m_x - m_edgeX < other.m_x + other.m_edgeX
        && m_y + m_edgeY > other.m_y - other.m_edgeY
        && m_y - m_edgeY < other.m_y + other.m_edgeY;
}
