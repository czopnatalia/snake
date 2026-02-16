#include "square.hpp"
#include <cstdlib>
#include <ctime>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

Square::Square(float x, float y, float length)
{
    m_Position = glm::vec3(x, y, 0.0f);

    // Kolor losowy
    m_Color.r = (std::rand() % 25) / 25.0f;
    m_Color.g = (std::rand() % 25) / 25.0f;
    m_Color.b = (std::rand() % 25) / 25.0f;
    m_Color.a = 1.0f;

    m_Length = length;
    m_Direction = DIR_RIGHT;
}

glm::vec3 Square::getPosition()
{
    return m_Position; // zwracamy kopiê – BEZPIECZNIE
}

void Square::setPosition(float x, float y, float z)
{
    m_Position = glm::vec3(x, y, z);
}

glm::vec4 Square::getColor()
{
    return m_Color;
}

void Square::setColor(const glm::vec4& color)
{
    m_Color = color;
}

Square::DIRECTION Square::getDirection()
{
    return m_Direction;
}

void Square::setDirection(DIRECTION direction)
{
    m_Direction = direction;
}

void Square::move()
{
    switch (m_Direction)
    {
    case DIR_LEFT:
      m_Position -= glm::vec3(m_Length, 0.0f, 0.0f);
      break;
    case DIR_RIGHT:
      m_Position += glm::vec3(m_Length, 0.0f, 0.0f);
      break;
    case DIR_UP:
      m_Position += glm::vec3(0.0f, m_Length, 0.0f);
      break;
    case DIR_DOWN:
      m_Position -= glm::vec3(0.0f, m_Length, 0.0f);
      break;
    }
}
