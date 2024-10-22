#define GL_SILENCE_DEPRECATION

#include <SFML/Graphics.hpp>
#include <GLUT/glut.h>
#include <cmath>
#include <iostream>

#define CAMERA_SPEED 0.01f
#define ZOOM_SPEED 0.05f

// Управление камеройad
float cameraX = 0, cameraY = 0, cameraZ = 5;
float fieldOfView = 45.0f;

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
    gluLookAt(cameraX, cameraY, cameraZ, 0, 0, 0, 0, 1, 0);
}

void calculatePerspective(float fovY, float aspect, float zNear, float zFar) {
    float fH = tan(fovY / 360 * M_PI) * zNear;
    float fW = fH * aspect;

    glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

void setPerspective() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    calculatePerspective(fieldOfView, 1500.0f / 1200.0f, 0.1f, 100.0f);
}

void drawCube() {
    glPushMatrix();
    glTranslatef(-2.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);

    // Передняя грань
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Задняя грань
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Левый бок
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    // Правый бок
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // Верхняя грань
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    // Нижняя грань
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    glEnd();
    glPopMatrix();
}

void drawPyramid() {
    glPushMatrix();
    glTranslatef(2.0f, 0.0f, 0.0f);
    glBegin(GL_TRIANGLES);

    // Передняя грань
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f( 0.0f,  1.0f,  0.0f); // Вершина
    glVertex3f(-1.0f, -1.0f,  1.0f); // Левый нижний угол
    glVertex3f( 1.0f, -1.0f,  1.0f); // Правый нижний угол

    // Правая грань
    glVertex3f( 0.0f,  1.0f,  0.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Задняя грань
    glVertex3f( 0.0f,  1.0f,  0.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);

    // Левая грань
    glVertex3f( 0.0f,  1.0f,  0.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    glEnd();

    // Отрисовка нижней грани (квадрат)
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glEnd();
    glPopMatrix();
}



void drawSphere() {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -3.0f); // Смещение сферы
    glColor3f(1.0f, 1.0f, 1.0f);
    GLUquadric* quadric = gluNewQuadric();
    gluSphere(quadric, 1.0f, 30, 30);
    gluDeleteQuadric(quadric);
    glPopMatrix();
}

int main() {
    sf::ContextSettings settings;
    settings.depthBits = 24;
    sf::RenderWindow window(sf::VideoMode(1500, 1200), "3D Scene", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(false);

    glEnable(GL_DEPTH_TEST);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        handleInput(); // Обрабатываем ввод
        handleZoom();

        // Очищаем экран
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Устанавливаем камеру и перспективу
        setPerspective();
        setCamera();

        // Отрисовываем объекты
        drawCube();
        drawPyramid();
        drawSphere();

        // Обновляем окно
        window.display();
    }

    return 0;
}
