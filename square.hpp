#ifndef SQUARE_HPP
#define SQUARE_HPP

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Square
{
public:
    enum DIRECTION
    {
        DIR_RIGHT = 1,
        DIR_LEFT = -1,
        DIR_UP = 2,
        DIR_DOWN = -2
    };

    Square(float x, float y, float length);

    glm::vec3 getPosition();                // zwracamy kopiê pozycji
    void setPosition(float x, float y, float z);  // setter DODANY

    glm::vec4 getColor();
    void setColor(const glm::vec4& color);

    void move();
    DIRECTION getDirection();
    void setDirection(DIRECTION direction);

private:
    glm::vec4 m_Color;
    glm::vec3 m_Position;
    float m_Length;
    DIRECTION m_Direction;
};

#endif
