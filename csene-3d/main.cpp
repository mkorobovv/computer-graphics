#define GL_SILENCE_DEPRECATION

#include <SFML/Graphics.hpp>
#include <GLUT/glut.h>

// Определение скоростей перемещения камеры и зума
#define CAMERA_SPEED 0.01f
#define ZOOM_SPEED 0.05f

// Параметры камеры и перспективы
float cameraX = 0, cameraY = 0, cameraZ = 5; // Начальные координаты камеры
float fieldOfView = 45.0f; // Угол обзора камеры

void handleInput() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) cameraZ -= CAMERA_SPEED; // Вперед

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) cameraZ += CAMERA_SPEED; // Назад

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) cameraX -= CAMERA_SPEED; // Влево

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) cameraX += CAMERA_SPEED; // Вправо
}

void handleZoom() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        fieldOfView = std::max(10.0f, fieldOfView - ZOOM_SPEED);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        fieldOfView = std::min(80.0f, fieldOfView + ZOOM_SPEED);
}

void setCamera() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0); // Позиционируем камеру
}

void setPerspective() {
    glMatrixMode(GL_PROJECTION); // Переключаемся на матрицу проекции
    glLoadIdentity(); // Сбрасываем матрицу
    gluPerspective(fieldOfView, 1500.0f / 1200.0f, 0.1f, 100.0f);  // Устанавливаем перспективу
}

void drawCube() {
    glPushMatrix(); // Сохраняем текущую матрицу
    glTranslatef(-2.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    // Цветные грани куба
    glColor3f(1.0f, 0.0f, 0.0f);     // Передняя
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);

    glColor3f(1.0f, 1.0f, 0.0f);     // Задняя
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glColor3f(0.0f, 0.0f, 1.0f);     // Левый бок
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);

    glColor3f(1.0f, 0.0f, 1.0f);     // Правый бок
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glColor3f(0.0f, 1.0f, 0.0f);     // Верхняя
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    glColor3f(1.0f, 0.5f, 0.0f);     // Нижняя
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
    glPopMatrix(); // Восстанавливаем сохраненную матрицу
}

void drawPyramid() {
    glPushMatrix();
    glTranslatef(2.0f, 0.0f, 0.0f);

    glBegin(GL_TRIANGLES);

    // Цветные грани пирамиды
    glColor3f(1.0f, 0.0f, 0.0f);     // Передняя грань
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    glColor3f(0.0f, 1.0f, 0.0f);     // Правая грань
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    glColor3f(0.0f, 0.0f, 1.0f);     // Задняя грань
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    glColor3f(1.0f, 1.0f, 0.0f);     // Левая грань
    glVertex3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    glEnd();
    glPopMatrix();
}

// Отрисовка сферы
void drawSphere() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -3.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    GLUquadric* quadric = gluNewQuadric();
    gluSphere(quadric, 1.0f, 30, 30);
    gluDeleteQuadric(quadric);
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

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    sf::RenderWindow window(sf::VideoMode(1500, 1200), "3D Scene", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);

    initGL();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        handleInput();
        handleZoom();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setPerspective();
        setCamera();
        drawCube();
        drawPyramid();
        drawSphere();

        window.display();
    }

    return 0;
}
