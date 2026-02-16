#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;     // <--- NOWY
layout (location = 2) in vec2 aTexCoord;   // <--- ZMIENIONA LOKACJA

// Stare uniformy do budowy Macierzy Modelu
uniform vec3 uMove;         // Translacja
uniform mat4 uTransform;    // Rotacja/Skala

uniform vec4 uColor;

// Nowe uniformy 3D
uniform mat4 uView;         
uniform mat4 uProjection;   

// Parametry oœwietlenia - NOWE
uniform vec3 lightPos;      // Pozycja œwiat³a (np. 0, 0, 1.5 - to co kamera)
uniform vec3 viewPos;       // Pozycja kamery (np. 0, 0, 1.5)

out vec4 fragColor;
out vec2 TexCoord;
out vec3 Normal;            // <--- NOWY: Normalna w przestrzeni œwiata
out vec3 FragPos;           // <--- NOWY: Pozycja fragmentu w przestrzeni œwiata

void main()
{
    // 1. Obliczenie Macierzy Modelu (uTransform jest rotacj¹)
    mat4 model = mat4(1.0);
    // Macierz Modelu to (Model Rotation) * (Model Translation) 
    // Mamy Translation i Rotation osobno, wiêc obliczmy Model.
    // U¿ywamy uTransform jako ROTACJÊ i dopiero dodajemy TRANSLACJÊ na koniec
    vec4 rotatedPos = uTransform * vec4(aPos, 1.0);
    vec3 worldPos_3d = rotatedPos.xyz + uMove;

    // Pozycja wierzcho³ka w przestrzeni œwiata (gotowa do u¿ycia w fs.glsl)
    FragPos = worldPos_3d;

    // Przekszta³cenie normalnej (tylko przez rotacjê, bez translacji)
    // UWAGA: Nie jest potrzebna macierz normalnych (Inverse Transpose), 
    // jeœli macierz Modelu jest tylko macierz¹ Model_Rotacyjna (uTransform) 
    // i skalowanie jest jednolite.
    Normal = normalize( mat3(uTransform) * aNormal );


    // 3. Transformacja 3D: gl_Position = P * V * M * aPos
    // M = Translation * Rotation (jeœli liczone macierz¹, u nas rêcznie)
    gl_Position = uProjection * uView * vec4(FragPos, 1.0);

    fragColor = uColor;
    TexCoord = aTexCoord; 
}