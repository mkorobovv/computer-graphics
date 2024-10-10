#include <SFML/Graphics.hpp>

#include <cmath>

#define HEXAGON_COUNT_POINTS 6
#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 900

#define FPS 60

// Функция интерполяции (линейная интерполяция) для изменения формы
sf::Vector2f linear_interpolation(const sf::Vector2f& start, const sf::Vector2f& end, float t)
{
    return start + t * (end - start);
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Hexagon Animation");

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(FPS);

    // Задаем начальную и конечную позиции шестиугольника
    sf::Vector2f initial_position(150, 750);
    sf::Vector2f final_position(1150, 150);

    // Переменная времени для интерполяции
    float time = 0.0f;
    float speed = 0.01f;  // Скорость движения

    // Флаг, чтобы определить направление интерполяции (вперед/назад)
    bool forward = true;

    sf::ConvexShape hexagon;
    hexagon.setPointCount(HEXAGON_COUNT_POINTS);

    // Определяем размеры шестиугольника
    float radius = 50.0f; // Радиус шестиугольника
    for (size_t i = 0; i < HEXAGON_COUNT_POINTS; i++)
    {
        float angle = 2 * M_PI * i / HEXAGON_COUNT_POINTS; // Углы для шестиугольника
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        hexagon.setPoint(i, sf::Vector2f(x, y));
    }

    hexagon.setFillColor(sf::Color::Red);
    hexagon.setOutlineThickness(5);
    hexagon.setOutlineColor(sf::Color::Black);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(sf::Color::White);

        // Вычисляем текущую позицию шестиугольника
        sf::Vector2f current_position = linear_interpolation(initial_position, final_position, time);

        // Применяем трансформацию к шестиугольнику
        hexagon.setPosition(current_position);

        float scale = 1.f + .5f * sin(time * M_PI); // Используем синус для плавного изменения масштаба
        hexagon.setScale(scale, scale);

        float angle = 2 * M_PI * time / HEXAGON_COUNT_POINTS;
        hexagon.rotate(angle);

        window.draw(hexagon);

        // Обновление интерполяции
        if (forward) {
            time += speed;

            if (time >= 1.0f) forward = false; // Начинаем движение обратно
        } else {
            time -= speed;

            if (time <= 0.0f) forward = true; // Начинаем движение вперед
        }

        window.display();
    }

    return 0;
}