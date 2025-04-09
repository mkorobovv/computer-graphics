#version 120

// Входные атрибуты
attribute vec3 aPos;
attribute vec3 aNormal;
attribute vec2 aTexCoords;
attribute vec3 aTangent;
attribute vec3 aBitangent;

// Передача данных во фрагментный шейдер
varying vec2 TexCoords;
varying vec3 FragPos;
varying mat3 TBN;

// Матрицы
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Преобразуем координаты вершин в мировые
    vec3 T = normalize(mat3(model) * aTangent);
    vec3 B = normalize(mat3(model) * aBitangent);
    vec3 N = normalize(mat3(model) * aNormal);

    // Матрица TBN (Tangent, Bitangent, Normal)
    TBN = mat3(T, B, N);

    // Позиция фрагмента в мировом пространстве
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Передаем текстурные координаты
    TexCoords = aTexCoords;

    // Вычисляем итоговую позицию вершины
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
