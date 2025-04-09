#define GL_SILENCE_DEPRECATION

#include <SFML/Graphics.hpp>
#include <GLUT/glut.h>

// Структура для хранения трансформаций объекта
struct Transform {
    float posX, posY, posZ;
    float scaleX, scaleY, scaleZ;
    float rotationX, rotationY, rotationZ;

    Transform() : posX(0), posY(0), posZ(0), scaleX(1), scaleY(1), scaleZ(1), rotationX(0), rotationY(0), rotationZ(0) {}
};

Transform cubeTransform;
Transform pyramidTransform;
int selectedObject = 0; // 0 - куб, 1 - пирамида

float cameraX = 0, cameraY = 0, cameraZ = 5; // Начальные координаты камеры
float fieldOfView = 45.0f; // Угол обзора камеры

void setCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0); // Позиционируем камеру
}

void setPerspective() {
    glMatrixMode(GL_PROJECTION); // Переключаемся на матрицу проекции
    glLoadIdentity(); // Сбрасываем матрицу
    gluPerspective(fieldOfView, 2000.0f / 1500.0f, 0.1f, 100.0f);  // Устанавливаем перспективу
}

void applyTransform(Transform& t, float init_pos) {
    glTranslatef(t.posX + init_pos, t.posY, t.posZ);
    glRotatef(t.rotationX, 1, 0, 0);
    glRotatef(t.rotationY, 0, 1, 0);
    glRotatef(t.rotationZ, 0, 0, 1);
    glScalef(t.scaleX, t.scaleY, t.scaleZ);
}


void drawCube() {
    glPushMatrix(); // Сохраняем текущую матрицу
    applyTransform(cubeTransform, -0.5f);
    glBegin(GL_QUADS);

    // Цветные грани куба
    glColor3f(1.0f, 0.0f, 0.0f);     // Передняя
    glVertex3f(-0.2f, -0.2f, 0.2f);
    glVertex3f(0.2f, -0.2f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);
    glVertex3f(-0.2f, 0.2f, 0.2f);

    glColor3f(1.0f, 1.0f, 0.0f);     // Передняя
    glVertex3f(-0.2f, -0.2f, -0.2f);
    glVertex3f(-0.2f, 0.2f, 0.2f);
    glVertex3f(0.2f, 0.2f, -0.2f);
    glVertex3f(0.2f, -0.2f, -0.2f);

    glColor3f(0.0f, 0.0f, 1.0f);     // Левый бок
    glVertex3f(-0.2f, -0.2f, -0.2f);
    glVertex3f(-0.2f, -0.2f, 0.2f);
    glVertex3f(-0.2f, 0.2f, 0.2f);
    glVertex3f(-0.2f, 0.2f, -0.2f);

    glColor3f(1.0f, 0.0f, 1.0f);     // Правый бок
    glVertex3f(0.2f, -0.2f, -0.2f);
    glVertex3f(0.2f, 0.2f, -0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);
    glVertex3f(0.2f, -0.2f, 0.2f);

    glColor3f(0.0f, 1.0f, 0.0f);     // Верхняя
    glVertex3f(-0.2f, 0.2f, -0.2f);
    glVertex3f(-0.2f, 0.2f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);
    glVertex3f(0.2f, 0.2f, -0.2f);

    glColor3f(1.0f, 0.5f, 0.0f);     // Нижняя
    glVertex3f(-0.2f, -0.2f, -0.2f);
    glVertex3f(0.2f, -0.2f, -0.2f);
    glVertex3f(0.2f, -0.2f, 0.2f);
    glVertex3f(-0.2f, -0.2f, 0.2f);

    glEnd();
    glPopMatrix(); // Восстанавливаем сохраненную матрицу
}

void drawPyramid() {
    glPushMatrix();
    applyTransform(pyramidTransform, 0.5f);
    glBegin(GL_TRIANGLES);

    // Цветные грани пирамиды
    glColor3f(1.0f, 0.0f, 0.0f);     // Передняя грань
    glVertex3f(0.0f, 0.2f, 0.0f);
    glVertex3f(-0.2f, -0.2f, 0.2f);
    glVertex3f(0.2f, -0.2f, 0.2f);

    glColor3f(0.0f, 1.0f, 0.0f);     // Правая грань
    glVertex3f(0.0f, 0.2f, 0.0f);
    glVertex3f(0.2f, -0.2f, 0.2f);
    glVertex3f(0.2f, -0.2f, -0.2f);

    glColor3f(0.0f, 0.0f, 1.0f);     // Задняя грань
    glVertex3f(0.0f, 0.2f, 0.0f);
    glVertex3f(0.2f, -0.2f, -0.2f);
    glVertex3f(-0.2f, -0.2f, -0.2f);

    glColor3f(1.0f, 1.0f, 0.0f);     // Левая грань
    glVertex3f(0.0f, 0.2f, 0.0f);
    glVertex3f(-0.2f, -0.2f, -0.2f);
    glVertex3f(-0.2f, -0.2f, 0.2f);

    glEnd();
    glPopMatrix();
}

// Инициализация OpenGL параметров
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Установка цвета фона (черный)
    glClearDepth(1.0f); // Установка глубины для буфера глубины
    glEnable(GL_DEPTH_TEST); // Включение теста глубины для корректного отображения 3D-объектов
    glDepthFunc(GL_LEQUAL); // Установка функции глубины
    glShadeModel(GL_SMOOTH); // Гладкая заливка поверхностей
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Установка высокого качества перспективы
}

void handleInput(float deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        if (selectedObject == 0) cubeTransform.posY += 0.1f * deltaTime;
        else pyramidTransform.posY += 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        if (selectedObject == 0) cubeTransform.posY -= 0.1f* deltaTime;
        else pyramidTransform.posY -= 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        if (selectedObject == 0) cubeTransform.posX -= 0.1f* deltaTime;
        else pyramidTransform.posX -= 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        if (selectedObject == 0) cubeTransform.posX += 0.1f* deltaTime;
        else pyramidTransform.posX += 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        if (selectedObject == 0) cubeTransform.scaleY += 0.1f* deltaTime;
        else pyramidTransform.scaleY += 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        if (selectedObject == 0) cubeTransform.scaleY -= 0.1f* deltaTime;
        else pyramidTransform.scaleY -= 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        if (selectedObject == 0) cubeTransform.scaleX += 0.1f* deltaTime;
        else pyramidTransform.scaleX += 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
        if (selectedObject == 0) cubeTransform.scaleX -= 0.1f* deltaTime;
        else pyramidTransform.scaleX -= 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        if (selectedObject == 0) cubeTransform.scaleZ += 0.1f* deltaTime;
        else pyramidTransform.scaleZ += 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)) {
        if (selectedObject == 0) cubeTransform.scaleZ -= 0.1f* deltaTime;
        else pyramidTransform.scaleZ -= 0.1f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        if (selectedObject == 0) cubeTransform.rotationY -= 5.0f* deltaTime;
        else pyramidTransform.rotationY -= 5.0f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        if (selectedObject == 0) cubeTransform.rotationY += 5.0f* deltaTime;
        else pyramidTransform.rotationY += 5.0f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)) {
        if (selectedObject ==  0) cubeTransform.rotationX -= 5.0f* deltaTime;
        else pyramidTransform.rotationX -= 5.0f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)) {
        if (selectedObject == 0) cubeTransform.rotationX += 5.0f* deltaTime;
        else pyramidTransform.rotationX += 5.0f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        if (selectedObject == 0) cubeTransform.rotationZ -= 5.0f* deltaTime;
        else pyramidTransform.rotationZ -= 5.0f* deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
        if (selectedObject == 0) cubeTransform.rotationZ += 5.0f* deltaTime;
        else pyramidTransform.rotationZ += 5.0f* deltaTime;
    }
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    sf::RenderWindow window(sf::VideoMode(2000, 1500), "3D Scene", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);

    initGL();

    // Загрузка шрифта
    sf::Font font;
    if (!font.loadFromFile("/Users/maxim/Library/Fonts/MesloLGS NF Regular.ttf")) {
        return -1; // Обработка ошибки загрузки шрифта
    }

    // Текст для отображения выбранного объекта
    sf::Text selectedObjectText;
    selectedObjectText.setFont(font);
    selectedObjectText.setCharacterSize(24);
    selectedObjectText.setFillColor(sf::Color::White);
    selectedObjectText.setPosition(10, 10);

    // Создание кнопок
    sf::RectangleShape cubeButton(sf::Vector2f(200, 75));
    cubeButton.setPosition(10, 200);
    cubeButton.setFillColor(sf::Color::White);

    sf::RectangleShape pyramidButton(sf::Vector2f(200, 75));
    pyramidButton.setPosition(10, 100);
    pyramidButton.setFillColor(sf::Color::White);

    // Текст для кнопок
    sf::Text cubeButtonText;
    cubeButtonText.setFont(font);
    cubeButtonText.setString("Select Cube ");
    cubeButtonText.setCharacterSize(20);
    cubeButtonText.setFillColor(sf::Color::Black);
    cubeButtonText.setPosition(10, 200);

    sf::Text pyramidButtonText;
    pyramidButtonText.setFont(font);
    pyramidButtonText.setString("Select Pyramid");
    pyramidButtonText.setCharacterSize(20);
    pyramidButtonText.setFillColor(sf::Color::Black);
    pyramidButtonText.setPosition(10, 100);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Обработка клика мыши
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    // Проверка, была ли нажата кнопка для куба
                    if (cubeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        selectedObject = 0;
                    }
                    // Проверка, была ли нажата кнопка для пирамиды
                    else if (pyramidButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        selectedObject = 1;
                    }
                }
            }
        }

        float deltaTime = clock.restart().asSeconds(); // для интерполяции трнасформаций

        handleInput(deltaTime*10); // Обработка ввода

        // Обновление текста в зависимости от выбранного объекта
        if (selectedObject == 0) {
            selectedObjectText.setString("Selected Object: Cube");
        } else {
            selectedObjectText.setString("Selected Object: Pyramid");
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setPerspective();
        setCamera();
        drawCube();
        drawPyramid();

        // Отрисовка интерфейса
        window.pushGLStates();   // Сохраняем состояния OpenGL для отрисовки SFML
        window.draw(selectedObjectText);
        window.draw(cubeButton);
        window.draw(cubeButtonText);
        window.draw(pyramidButton);
        window.draw(pyramidButtonText);
        window.popGLStates();    // Восстанавливаем состояния OpenGL

        window.display();
    }

    return 0;
}