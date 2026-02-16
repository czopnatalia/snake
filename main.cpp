#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cmath>

// WYMAGANE PRZEZ ŚRODOWISKO
#include "GL/glew.h"
#include "GL/freeglut.h"

// Pliki Twojego Projektu
#include "square.hpp" 
#include "shaderLoader.h" 
#include "tekstura.h" // Dodano

// Funkcje GLM
#include "glm/vec3.hpp" 
#include "glm/vec4.hpp" 
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;

// ----------------------------------------------------
// ZMIENNE GLOBALNE GRY
// ----------------------------------------------------
GLuint g_ShaderProgram;
GLuint g_AppleTextureID;

// Nowe VAO/VBO dla geometrii 3D
unsigned int cubeVAO, cubeVBO;
unsigned int pyramidVAO, pyramidVBO; 

// NOWE dla Sfery (Jabłka)
unsigned int sphereVAO, sphereVBO;
unsigned int g_SphereVertexCount = 0; // Przechowuje liczbę wierzchołków sfery

// Zmienne 3D i Tekstura
GLuint g_SnakeTextureID; 
glm::mat4 g_View;
glm::mat4 g_Projection;
GLint g_TextureUniformLoc;

// NOWE dla Oświetlenia
glm::vec3 g_CameraPosition = glm::vec3(0.0f, 0.0f, 1.5f);
glm::vec3 g_LightPosition = glm::vec3(0.0f, 0.0f, 1.5f); // Światło w pozycji kamery

// Parametry Specular
float g_SpecularStrength = 0.5f; // Siła błysku
int g_Shininess = 32;            // Połysk/Ostrość

float length = 0.08f;
float halfLength = length / 2.0f;

// 1. Geometria Ostrosłupa (Głowa) - 18 wierzchołków (6 trójkątów * 3 wierzchołki)
float pyramidVertices[] = {
  // POZYCJA (3f)    // NORMALNA (3f)           // WSPÓŁRZ. TEKSTURY (2f)
  // Tył (Trójkąt) - Normalna: (0, 0.707, -0.707) w przybliżeniu
  -halfLength, -halfLength, -halfLength,  0.0f,  0.707f, -0.707f,  0.0f, 0.0f,
  halfLength, -halfLength, -halfLength,  0.0f,  0.707f, -0.707f,  1.0f, 0.0f,
  0.0f,        halfLength,  0.0f,        0.0f,  0.707f, -0.707f,  0.5f, 1.0f, // Wierzchołek

  // Przód (Trójkąt) - Normalna: (0, 0.707, 0.707)
  -halfLength, -halfLength,  halfLength,  0.0f,  0.707f,  0.707f,  0.0f, 0.0f,
  halfLength, -halfLength,  halfLength,  0.0f,  0.707f,  0.707f,  1.0f, 0.0f,
  0.0f,        halfLength,  0.0f,        0.0f,  0.707f,  0.707f,  0.5f, 1.0f, // Wierzchołek

  // Lewa (Trójkąt) - Normalna: (-0.707, 0.707, 0)
  -halfLength, -halfLength, -halfLength, -0.707f, 0.707f,  0.0f,  0.0f, 0.0f,
  -halfLength, -halfLength,  halfLength, -0.707f, 0.707f,  0.0f,  1.0f, 0.0f,
  0.0f,        halfLength,  0.0f,       -0.707f, 0.707f,  0.0f,  0.5f, 1.0f, // Wierzchołek

  // Prawa (Trójkąt) - Normalna: (0.707, 0.707, 0)
  halfLength, -halfLength, -halfLength,  0.707f, 0.707f,  0.0f,  0.0f, 0.0f,
  halfLength, -halfLength,  halfLength,  0.707f, 0.707f,  0.0f,  1.0f, 0.0f,
  0.0f,        halfLength,  0.0f,        0.707f, 0.707f,  0.0f,  0.5f, 1.0f, // Wierzchołek

  // Dół (Kwadrat = 2 trójkąty, Podstawa) - Normalna: (0, -1, 0)
  -halfLength, -halfLength, -halfLength,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
  halfLength, -halfLength, -halfLength,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
  halfLength, -halfLength,  halfLength,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,

  -halfLength, -halfLength, -halfLength,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
  halfLength, -halfLength,  halfLength,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
  -halfLength, -halfLength,  halfLength,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f
};

// 2. Geometria Sześcianu (Ciało i Jabłko) - 36 wierzchołków
float cubeVertices[] = {
  // POZYCJA (3f) | NORMALNA (3f) | WSPÓŁRZ. TEKSTURY (2f)
  // Tył (Normalna: 0, 0, -1)
  -halfLength, -halfLength, -halfLength,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 
  halfLength, -halfLength, -halfLength,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f, 
  halfLength,  halfLength, -halfLength,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
  halfLength,  halfLength, -halfLength,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f, 
  -halfLength,  halfLength, -halfLength,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 
  -halfLength, -halfLength, -halfLength,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
  // Przód (Normalna: 0, 0, 1)
  -halfLength, -halfLength,  halfLength,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f, 
  halfLength, -halfLength,  halfLength,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f, 
  halfLength,  halfLength,  halfLength,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
  halfLength,  halfLength,  halfLength,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f, 
  -halfLength,  halfLength,  halfLength,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f, 
  -halfLength, -halfLength,  halfLength,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
  // Lewa (Normalna: -1, 0, 0)
  -halfLength,  halfLength,  halfLength, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 
  -halfLength,  halfLength, -halfLength, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 
  -halfLength, -halfLength, -halfLength, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  -halfLength, -halfLength, -halfLength, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 
  -halfLength, -halfLength,  halfLength, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 
  -halfLength,  halfLength,  halfLength, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
  // Prawa (Normalna: 1, 0, 0)
  halfLength,  halfLength,  halfLength,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 
  halfLength,  halfLength, -halfLength,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 
  halfLength, -halfLength, -halfLength,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
  halfLength, -halfLength, -halfLength,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f, 
  halfLength, -halfLength,  halfLength,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f, 
  halfLength,  halfLength,  halfLength,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
  // Dół (Normalna: 0, -1, 0)
  -halfLength, -halfLength, -halfLength,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f, 
  halfLength, -halfLength, -halfLength,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f, 
  halfLength, -halfLength,  halfLength,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
  halfLength, -halfLength,  halfLength,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f, 
  -halfLength, -halfLength,  halfLength,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f, 
  -halfLength, -halfLength, -halfLength,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
  // Góra (Normalna: 0, 1, 0)
  -halfLength,  halfLength, -halfLength,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f, 
  halfLength,  halfLength, -halfLength,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f, 
  halfLength,  halfLength,  halfLength,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
  halfLength,  halfLength,  halfLength,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f, 
  -halfLength,  halfLength,  halfLength,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f, 
  -halfLength,  halfLength, -halfLength,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};

std::vector<Square*> snakeBlockList;

// Zmienne Globalne dla Jabłka
glm::vec3 g_FoodPosition;
glm::vec4 g_FoodColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f); // Czerwony

// ----------------------------------------------------
// PROTOTYPY FUNKCJI
// ----------------------------------------------------
void moveSnake();
void drawSnake(GLuint program);
void addTail();
void spawnFood();
bool checkCollision();
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void renderScene();
void timerFunction(int value);


// ----------------------------------------------------
// DEFINICJE LOGIKI GRY
// ----------------------------------------------------
void moveSnake()
{
  int n = (int)snakeBlockList.size();
  if (n == 0) return;

  // 1) Zachowaj stare pozycje (przed ruchem)
  std::vector<glm::vec3> oldPos;
  oldPos.reserve(n);
  for (int i = 0; i < n; ++i)
  {
    oldPos.push_back(snakeBlockList[i]->getPosition());
  }

  // 2) Skopiuj kierunki: każdy segment dostaje kierunek swojego poprzednika
  for (int i = n - 1; i > 0; --i)
  {
    snakeBlockList[i]->setDirection(snakeBlockList[i - 1]->getDirection());
  }

  // 3) Przesuń tylko głowę (move przesuwa o m_Length)
  snakeBlockList[0]->move();

  // Pomocnicza lambda do wrap-around na siatce (2D)
  auto wrap = [&](glm::vec3 p) -> glm::vec3 {
    float minPos = -1.0f + length / 2.0f;
    float maxPos = 1.0f - length / 2.0f;

    if (p.x > maxPos) p.x = minPos;
    else if (p.x < minPos) p.x = maxPos;

    if (p.y > maxPos) p.y = minPos;
    else if (p.y < minPos) p.y = maxPos;

    return p;
    };

  // 4) Zastosuj wrap do głowy (po jej ruchu)
  {
    glm::vec3 headPos = snakeBlockList[0]->getPosition();
    headPos = wrap(headPos);
    snakeBlockList[0]->setPosition(headPos.x, headPos.y, headPos.z);
  }

  // 5) Ustaw pozycje kolejnych segmentów na miejsce, gdzie był poprzednik
  for (int i = 1; i < n; ++i)
  {
    glm::vec3 target = oldPos[i - 1];
    target = wrap(target); 
    snakeBlockList[i]->setPosition(target.x, target.y, target.z);
  }
}



void addTail()
{
  int n = snakeBlockList.size();

  if (n == 0)
  {
    snakeBlockList.push_back(new Square(0.0f, 0.0f, length));
    return;
  }

  Square* last = snakeBlockList[n - 1];
  glm::vec3 pos = last->getPosition();
  Square::DIRECTION dir = last->getDirection();

  switch (dir)
  {
  case Square::DIR_RIGHT:
    pos -= glm::vec3(length, 0.0f, 0.0f);
    break;
  case Square::DIR_LEFT:
    pos += glm::vec3(length, 0.0f, 0.0f);
    break;
  case Square::DIR_UP:
    pos -= glm::vec3(0.0f, length, 0.0f);
    break;
  case Square::DIR_DOWN:
    pos += glm::vec3(0.0f, length, 0.0f);
    break;
  }

  Square* newSq = new Square(pos.x, pos.y, length);
  newSq->setDirection(dir);
  snakeBlockList.push_back(newSq);
}



bool almostEqual(float a, float b)
{
  return fabs(a - b) < 0.001f;
}


void spawnFood()
{
  float minCoord = -1.0f + length / 2.0f;

  int cells = (int)(2.0f / length); // ile pól w poziomie/ pionie

  bool collision = false;

  do {
    collision = false;

    int ix = rand() % cells;
    int iy = rand() % cells;

    float fx = minCoord + ix * length;
    float fy = minCoord + iy * length;

    g_FoodPosition = glm::vec3(fx, fy, 0.0f);

    // upewniamy się, że nie wyląduje na wężu
    for (auto block : snakeBlockList)
    {
      glm::vec3 pos = block->getPosition();
      if (almostEqual(pos.x, g_FoodPosition.x) &&
        almostEqual(pos.y, g_FoodPosition.y))
      {
        collision = true;
        break;
      }
    }

  } while (collision);
}


bool checkCollision()
{
  // Kolizja z ciałem (kod bez zmian)
  if (snakeBlockList.size() < 2) {
    return false;
  }

  glm::vec3 headPos = snakeBlockList[0]->getPosition();

  for (size_t i = 1; i < snakeBlockList.size(); ++i)
  {
    glm::vec3 bodyPos = snakeBlockList[i]->getPosition();

    if (almostEqual(headPos.x, bodyPos.x) && almostEqual(headPos.y, bodyPos.y))
    {
      return true;
    }
  }
  return false;
}

void drawSnake(GLuint program)
{
  GLint uMoveLoc = glGetUniformLocation(program, "uMove");
  GLint uColorLoc = glGetUniformLocation(program, "uColor");
  GLint uTransformLoc = glGetUniformLocation(program, "uTransform");

  // 1. AKTYWACJA TEKSTURY WĘŻA (Jednostka 0)
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, g_SnakeTextureID); 

  glm::mat4 transform = glm::mat4(1.0f);

  GLint uUseTexture = glGetUniformLocation(program, "useTexture");
  glUniform1i(uUseTexture, 1); // WĄŻ używa tekstury

  for (size_t i = 0; i < snakeBlockList.size(); i++)
  {
    Square* part = snakeBlockList[i];

    // USTAW KOLOR NA BIAŁY, ABY TEKSTURA BYŁA WIDOCZNA
    glUniform4f(uColorLoc, 1.0f, 1.0f, 1.0f, 1.0f); 

    // Rotacja dla kierunku węża (tylko w 2D, wokół Z)
    transform = glm::mat4(1.0f);
    float angle = 0.0f;
    switch (part->getDirection())
    {
    case Square::DIR_LEFT: angle = glm::radians(90.0f); break;
    case Square::DIR_RIGHT: angle = glm::radians(-90.0f); break;
    case Square::DIR_DOWN: angle = glm::radians(180.0f); break;
    default: angle = 0.0f; break;
    }
    // Obracamy wokół Z dla widoku 3D
    transform = glm::rotate(transform, angle, glm::vec3(0.0f, 0.0f, 1.0f));

    glUniformMatrix4fv(uTransformLoc, 1, GL_FALSE, &transform[0][0]);

    // Przesunięcie
    glUniform3f(uMoveLoc, part->getPosition().x, part->getPosition().y, part->getPosition().z);

    if (i == 0)
    {
      // GŁOWA – OSTROSŁUP
      glBindVertexArray(pyramidVAO);
      glDrawArrays(GL_TRIANGLES, 0, 18); 
    }
    else
    {
      // CIAŁO – SZEŚCIAN
      glBindVertexArray(cubeVAO);
      glDrawArrays(GL_TRIANGLES, 0, 36); 
    }
  }

  // DEZAKTYWACJA TEKSTURY (żeby jabłko jej nie użyło)
  glBindTexture(GL_TEXTURE_2D, 0); 

  // Zresetuj macierz do identyczności dla jabłka
  transform = glm::mat4(1.0f);
  glUniformMatrix4fv(uTransformLoc, 1, GL_FALSE, &transform[0][0]);
}


// ----------------------------------------------------
// FREEGLUT CALLBACKS
// ----------------------------------------------------

void keyboard(unsigned char key, int x, int y)
{
  if (key == 27) // ESC
  {
    for (Square* block : snakeBlockList) { delete block; }
    exit(0);
  }
  if (key == ' ') // Spacja
  {
    addTail();
  }
  glutPostRedisplay();
}

void specialKeys(int key, int x, int y)
{
  if (snakeBlockList.size() != 0)
  {
    Square* first = snakeBlockList[0];

    // Zapobiega cofaniu się węża
    switch (key)
    {
    case GLUT_KEY_LEFT:
      if (first->getDirection() != Square::DIR_RIGHT) first->setDirection(Square::DIR_LEFT);
      break;
    case GLUT_KEY_RIGHT:
      if (first->getDirection() != Square::DIR_LEFT) first->setDirection(Square::DIR_RIGHT);
      break;
    case GLUT_KEY_UP:
      if (first->getDirection() != Square::DIR_DOWN) first->setDirection(Square::DIR_UP);
      break;
    case GLUT_KEY_DOWN:
      if (first->getDirection() != Square::DIR_UP) first->setDirection(Square::DIR_DOWN);
      break;
    }
  }
}

void renderScene()
{
  // Czyść kolor i GŁĘBOKOŚĆ
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glUseProgram(g_ShaderProgram);

  // 1. ZMIENNE LOKALNE DLA UNIFORMÓW
  GLint uUseTextureLoc = glGetUniformLocation(g_ShaderProgram, "useTexture");
  GLint uColorLoc = glGetUniformLocation(g_ShaderProgram, "uColor");

  // DODANE: Pobranie lokacji uniformów dla oświetlenia specularnego
  GLint uSpecStrengthLoc = glGetUniformLocation(g_ShaderProgram, "specularStrength");
  GLint uShininessLoc = glGetUniformLocation(g_ShaderProgram, "shininess");

  // 1. Rysowanie węża
  // --- WĘŻOWI WŁĄCZAMY TEKSTURĘ ---
  //GLint uUseTextureLoc = glGetUniformLocation(g_ShaderProgram, "useTexture");
  //GLint uColorLoc = glGetUniformLocation(g_ShaderProgram, "uColor");

  glUniform1i(uUseTextureLoc, 1);  // użyj TEXTURY
  glUniform4f(uColorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // biały kolor (nie przyciemnia tekstury)
  drawSnake(g_ShaderProgram);

  // -------------------------------------------
  // 2. RYSOWANIE JABŁKA (tekstura = WYŁĄCZONA)
  // -------------------------------------------
  GLint uMoveLoc = glGetUniformLocation(g_ShaderProgram, "uMove");
  GLint uTransformLoc = glGetUniformLocation(g_ShaderProgram, "uTransform");

  // Ustawianie wyższych parametrów specularnych dla lśniącego jabłka
  glUniform1f(uSpecStrengthLoc, 1.0f); // Bardzo jasne odbicie
  glUniform1i(uShininessLoc, 256);     // Bardzo ostre (lśniące) odbicie

  glm::mat4 transform = glm::mat4(1.0f);
  glUniformMatrix4fv(uTransformLoc, 1, GL_FALSE, &transform[0][0]);

  if (g_AppleTextureID != (GLuint)-1)
  {
    glUniform1i(uUseTextureLoc, 1); // jabłko -> z teksturą
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_AppleTextureID); // Użyj tekstury jabłka
    glUniform4f(uColorLoc, 1.0f, 1.0f, 1.0f, 1.0f); // Biały kolor, aby tekstura była widoczna
  }
  else
  {
    glUniform1i(uUseTextureLoc, 0); // jabłko -> bez tekstury (w razie błędu ładowania)
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUniform4f(uColorLoc, g_FoodColor.r, g_FoodColor.g, g_FoodColor.b, g_FoodColor.a);
  }

  glUniform3f(uMoveLoc,
    g_FoodPosition.x, g_FoodPosition.y, g_FoodPosition.z);

  glBindVertexArray(sphereVAO);
  glDrawArrays(GL_TRIANGLES, 0, g_SphereVertexCount); // Użyj liczby wierzchołków sfery

  // Przywróć teksturę węża po rysowaniu, aby nie wpłynęło na `drawSnake`
  // (Choć `drawSnake` i tak ustawi ją na nowo, jest to dobra praktyka)
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, g_SnakeTextureID); 

  glutSwapBuffers();
}


void timerFunction(int value)
{
  moveSnake(); 

  // 2. KONTROLA KOLIZJI Z CIAŁEM (TYLKO TA KOLIZJA KOŃCZY GRĘ!)
  if (checkCollision())
  {
    for (Square* block : snakeBlockList) { delete block; }
    cerr << "Koniec gry! Wąż zderzył się ze sobą." << endl;
    exit(0);
  }

  // 3. KONTROLA KOLIZJI Z JABŁKIEM
  glm::vec3 headPos = snakeBlockList[0]->getPosition();
  if (almostEqual(headPos.x, g_FoodPosition.x) &&
    almostEqual(headPos.y, g_FoodPosition.y))
  {
    addTail();
    spawnFood();
  }


  glutPostRedisplay();
  glutTimerFunc(100, timerFunction, 0);
}

// Funkcja generująca wierzchołki kuli (UV Sphere)
std::vector<float> generateSphere(float radius, int segments)
{
  std::vector<float> data;
  const float PI = 3.14159265359f;

  for (int i = 0; i <= segments; ++i)
  {
    // Kąty theta dla szerokości geograficznej
    float theta = i * PI / segments;
    float sinTheta = sin(theta);
    float cosTheta = cos(theta);

    for (int j = 0; j <= segments; ++j)
    {
      // Kąty phi dla długości geograficznej
      float phi = j * 2 * PI / segments;
      float sinPhi = sin(phi);
      float cosPhi = cos(phi);

      // 1. POZYCJA (x, y, z)
      float x = cosPhi * sinTheta;
      float y = cosTheta;
      float z = sinPhi * sinTheta;

      // 2. NORMALNA (to samo co pozycja, bo promień kuli wynosi 1)
      float nx = x;
      float ny = y;
      float nz = z;

      // 3. WSPÓŁRZĘDNE TEKSTURY (u, v)
      float u = (float)j / segments;
      float v = (float)i / segments;

      // Wierzchołek 1 (Punkt A)
      // Dodaj Pozycję
      data.push_back(radius * x);
      data.push_back(radius * y);
      data.push_back(radius * z);
      // Dodaj Normalną
      data.push_back(nx);
      data.push_back(ny);
      data.push_back(nz);
      // Dodaj Współrzędne Tekstury
      data.push_back(u);
      data.push_back(v);
    }
  }

  // Konwersja siatki wierzchołków (quads) na trójkąty
  std::vector<float> finalVertices;
  for (int i = 0; i < segments; ++i)
  {
    for (int j = 0; j < segments; ++j)
    {
      int k1 = i * (segments + 1) + j;
      int k2 = i * (segments + 1) + j + 1;
      int k3 = (i + 1) * (segments + 1) + j;
      int k4 = (i + 1) * (segments + 1) + j + 1;

      // Trójkąt 1: k1, k3, k2 (Trójkąt A)
      for(int k = 0; k < 8; k++) finalVertices.push_back(data[k1 * 8 + k]);
      for(int k = 0; k < 8; k++) finalVertices.push_back(data[k3 * 8 + k]);
      for(int k = 0; k < 8; k++) finalVertices.push_back(data[k2 * 8 + k]);

      // Trójkąt 2: k2, k3, k4 (Trójkąt B)
      for(int k = 0; k < 8; k++) finalVertices.push_back(data[k2 * 8 + k]);
      for(int k = 0; k < 8; k++) finalVertices.push_back(data[k3 * 8 + k]);
      for(int k = 0; k < 8; k++) finalVertices.push_back(data[k4 * 8 + k]);
    }
  }
  return finalVertices;
}


// ----------------------------------------------------
// GŁÓWNA FUNKCJA
// ----------------------------------------------------
int main(int argc, char** argv)
{
  std::srand(std::time(nullptr));

  // ---------------------------------------
  // Inicjalizacja GLUT + OpenGL
  // ---------------------------------------
  glutInit(&argc, argv);
  glutInitContextVersion(3, 3);
  glutInitContextFlags(GLUT_CORE_PROFILE | GLUT_DEBUG);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH); // DODANO GLUT_DEPTH
  glutInitWindowSize(800, 800);
  glutCreateWindow("Snake 3D");

  GLenum err = glewInit();
  if (GLEW_OK != err) {
    fprintf(stderr, "GLEW Error: %s\n", glewGetErrorString(err));
    return 1;
  }

  // ---------------------------------------
  // ŁADOWANIE SHADERÓW (nowe pliki)
  // ---------------------------------------
  g_ShaderProgram = loadShaders("./vs.glsl", "./fs.glsl");
  if (g_ShaderProgram == 0) {
    cerr << "Blad ladowania shaderow!" << endl;
    return 1;
  }

  // ---------------------------------------
  // 3D Ustawienia
  // ---------------------------------------
  glEnable(GL_DEPTH_TEST);

  // ---------------------------------------
  // ŁADOWANIE TEKSTURY I USTAWIENIA 3D
  // ---------------------------------------
  g_SnakeTextureID = WczytajTeksture("snake.bmp"); 
  if (g_SnakeTextureID == (GLuint)-1) {
    cerr << "Blad ladowania tekstury weza!" << endl;
  }

  // NOWE: ŁADOWANIE TEKSTURY DLA JABŁKA
  g_AppleTextureID = WczytajTeksture("apple.bmp"); 
  if (g_AppleTextureID == (GLuint)-1) {
    cerr << "Blad ladowania tekstury jablka! Uzycie jednolitego koloru." << endl;
    // Jeśli ładowanie się nie powiedzie, możesz nadal użyć jednolitego koloru
    // i kontynuować, dlatego nie używamy 'return 1'
  }

  glUseProgram(g_ShaderProgram);
  // 1. MACIERZE 3D - Konfiguracja
  g_Projection = glm::perspective(glm::radians(60.0f), 800.0f / 800.0f, 0.1f, 100.0f);
  g_View = glm::lookAt(
    g_CameraPosition, 
    glm::vec3(0.0f, 0.0f, 0.0f), 
    glm::vec3(0.0f, 1.0f, 0.0f)); 

  GLint uViewLoc = glGetUniformLocation(g_ShaderProgram, "uView");
  GLint uProjLoc = glGetUniformLocation(g_ShaderProgram, "uProjection");
  glUniformMatrix4fv(uViewLoc, 1, GL_FALSE, &g_View[0][0]);
  glUniformMatrix4fv(uProjLoc, 1, GL_FALSE, &g_Projection[0][0]);

  // NOWE: Uniformy dla Oświetlenia
  GLint uViewPosLoc = glGetUniformLocation(g_ShaderProgram, "viewPos");
  GLint uLightPosLoc = glGetUniformLocation(g_ShaderProgram, "lightPos");
  glUniform3f(uViewPosLoc, g_CameraPosition.x, g_CameraPosition.y, g_CameraPosition.z);
  glUniform3f(uLightPosLoc, g_LightPosition.x, g_LightPosition.y, g_LightPosition.z);

  // NOWE: Uniformy dla Specular
  GLint uSpecStrengthLoc = glGetUniformLocation(g_ShaderProgram, "specularStrength");
  GLint uShininessLoc = glGetUniformLocation(g_ShaderProgram, "shininess");
  glUniform1f(uSpecStrengthLoc, g_SpecularStrength);
  glUniform1i(uShininessLoc, g_Shininess);

  // 2. Uniform dla tekstury
  g_TextureUniformLoc = glGetUniformLocation(g_ShaderProgram, "ourTexture");
  glUniform1i(g_TextureUniformLoc, 0); 
  glUseProgram(0);

  // ---------------------------------------
  // 1. VAO/VBO – SZEŚCIAN (CIAŁO i JABŁKO)
  // ---------------------------------------
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);

  glBindVertexArray(cubeVAO);
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

  // Atrybut 0: Pozycja (3 floaty)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Atrybut 1: Normalna (3 floaty) - Nowy atrybut!
  // Normalna ma być w lokacji 1, więc zmieniamy indeks.
  // UŻYJ LOKACJI 1 DLA NORMALNEJ, LOKACJI 2 DLA TEKSTURY!
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Atrybut 2: Współrzędne Tekstury (2 floaty)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // ---------------------------------------
  // 2. VAO/VBO – OSTROSŁUP (GŁOWA)
  // ---------------------------------------
  glGenVertexArrays(1, &pyramidVAO);
  glGenBuffers(1, &pyramidVBO);

  glBindVertexArray(pyramidVAO);
  glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), pyramidVertices, GL_STATIC_DRAW);

  // Atrybut 0: Pozycja (3 floaty)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Atrybut 1: Normalna (3 floaty) - Nowy atrybut!
  // Normalna ma być w lokacji 1, więc zmieniamy indeks.
  // UŻYJ LOKACJI 1 DLA NORMALNEJ, LOKACJI 2 DLA TEKSTURY!
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Atrybut 2: Współrzędne Tekstury (2 floaty)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // ---------------------------------------
  // 3. VAO/VBO – SFERA (JABŁKO) - NOWE
  // ---------------------------------------

  // Generowanie wierzchołków sfery (ustawiamy promień na połowę długości bloku)
  float sphereRadius = halfLength; 
  int sphereSegments = 32; // Im więcej segmentów, tym gładsza kula, ale więcej wierzchołków
  std::vector<float> sphereVertices = generateSphere(sphereRadius, sphereSegments);

  g_SphereVertexCount = sphereVertices.size() / 8; // 8 floatów na wierzchołek

  glGenVertexArrays(1, &sphereVAO);
  glGenBuffers(1, &sphereVBO);

  glBindVertexArray(sphereVAO);
  glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
  glBufferData(GL_ARRAY_BUFFER, sphereVertices.size() * sizeof(float), sphereVertices.data(), GL_STATIC_DRAW);

  // Atrybut 0: Pozycja (3 floaty)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Atrybut 1: Normalna (3 floaty)
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Atrybut 2: Współrzędne Tekstury (2 floaty)
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Oczyść po konfiguracji
  glBindVertexArray(0);

  // ---------------------------------------
  // Kolor tła
  // ---------------------------------------
  glClearColor(0.0f, 0.2f, 0.2f, 1.0f);

  // ---------------------------------------
  // INICJALIZACJA WĘŻA: GŁOWA + 1 SEGMENT
  // ---------------------------------------
  snakeBlockList.clear();

  Square* head = new Square(0.0f, 0.0f, length);
  head->setDirection(Square::DIR_RIGHT);
  head->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // Ustaw kolor na BIAŁY
  snakeBlockList.push_back(head);

  Square* body1 = new Square(-length, 0.0f, length);
  body1->setDirection(Square::DIR_RIGHT);
  body1->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)); // Ustaw kolor na BIAŁY
  snakeBlockList.push_back(body1);

  // JABŁKO
  spawnFood();

  // ---------------------------------------
  // CALLBACKS
  // ---------------------------------------
  glutDisplayFunc(renderScene);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeys);
  glutTimerFunc(100, timerFunction, 0);

  // ---------------------------------------
  // START GRY
  // ---------------------------------------
  glutMainLoop();

  glDeleteProgram(g_ShaderProgram);
  // Zwolnienie zasobów
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteBuffers(1, &cubeVBO);
  glDeleteVertexArrays(1, &pyramidVAO);
  glDeleteBuffers(1, &pyramidVBO);
  glDeleteVertexArrays(1, &sphereVAO); 
  glDeleteBuffers(1, &sphereVBO);
  for (Square* block : snakeBlockList) { delete block; }

  return 0;
}