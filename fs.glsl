#version 330 core

out vec4 FragColor;

in vec4 fragColor; 
in vec2 TexCoord; 
in vec3 Normal;    
in vec3 FragPos;   

uniform sampler2D ourTexture; 
uniform int useTexture;

// Parametry oœwietlenia - NOWE/AKTUALIZOWANE
uniform vec3 lightPos;      
uniform vec3 viewPos;      

// Parametry materia³u/odbicia lustrzanego - NOWE
uniform float specularStrength; // Si³a odbicia lustrzanego (np. 0.5)
uniform int shininess;          // Po³ysk/Ostroœæ (np. 32)

void main()
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0); // Bia³e œwiat³o
    
    // ----------------------
    // 1. Oœwietlenie Ambient (Otoczenia)
    // ----------------------
    float ambientStrength = 0.1; // Zmniejszono, aby œwiat³o kierunkowe by³o bardziej widoczne
    vec3 ambient = ambientStrength * lightColor;

    // ----------------------
    // 2. Oœwietlenie Diffuse (Rozproszone)
    // ----------------------
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // ----------------------
    // 3. Oœwietlenie Specular (Lustrzane) - DODANO
    // ----------------------
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm); // Wektor odbity
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 lighting = ambient + diffuse + specular; // Suma wszystkich komponentów
    
    vec4 objectColor;
    if(useTexture == 1)
    {
        objectColor = texture(ourTexture, TexCoord) * fragColor;
    }
    else
    {
        objectColor = fragColor;
    }

    // Ostateczny kolor fragmentu: (Tekstura/Kolor) * Oœwietlenie
    FragColor = vec4(objectColor.xyz * lighting, objectColor.a);
}