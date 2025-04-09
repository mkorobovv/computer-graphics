#version 120

// Входные данные от вершинного шейдера
varying vec2 TexCoords;
varying vec3 FragPos;
varying mat3 TBN;

// Текстуры
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

// Параметры освещения
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    // Извлекаем нормаль из карты нормалей
    vec3 normal = texture2D(normalMap, TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0); // Перевод из диапазона [0,1] в [-1,1]

    // Преобразуем нормаль в мировое пространство
    normal = normalize(TBN * normal);

    // Вычисляем направления света и наблюдателя
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Расчет отраженного света (Blinn-Phong)
    float diff = max(dot(normal, lightDir), 0.0);

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0); // Блеск (спекулярность)

    // Получаем цвет из текстуры
    vec3 color = texture2D(diffuseMap, TexCoords).rgb;

    // Итоговый цвет
    vec3 ambient = 0.1 * color;
    vec3 diffuse = diff * color;
    vec3 specular = spec * vec3(0.5);

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}
