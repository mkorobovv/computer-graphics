#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// Ray structure representing the origin and direction
struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

// Sphere structure with an intersection method
struct Sphere {
    glm::vec3 center;
    float radius;

    bool intersect(const Ray& ray, float& t) const {
        glm::vec3 oc = ray.origin - center;
        float b = glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - c;
        if (discriminant > 0) {
            float sqrtDiscriminant = sqrt(discriminant);
            t = -b - sqrtDiscriminant;
            if (t > 0) return true;
            t = -b + sqrtDiscriminant;
            return t > 0;
        }
        return false;
    }
};

// Plane structure with an intersection method
struct Plane {
    glm::vec3 point;
    glm::vec3 normal;
    GLuint textureID;  // ID текстуры для плоскости

    bool intersect(const Ray& ray, float& t) const {
        float denom = glm::dot(normal, ray.direction);
        if (abs(denom) > 1e-6) {
            glm::vec3 p0l0 = point - ray.origin;
            t = glm::dot(p0l0, normal) / denom;
            return (t >= 0);
        }
        return false;
    }
};

// Light source structure
struct Light {
    glm::vec3 position;
    glm::vec3 color;
};

// Tracing function that handles intersection tests, shading, reflection, etc.
glm::vec3 trace(const Ray& ray, const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const Light& light, const glm::vec3& viewPos, int depth = 0) {
    if (depth > 3) return glm::vec3(0.0f);  // Prevent infinite recursion

    float closest_t = std::numeric_limits<float>::max();
    glm::vec3 closest_color = glm::vec3(0.0f);
    glm::vec3 hitPoint;
    glm::vec3 normal;
    GLuint textureID = 0;
    bool hit = false;

    // Check for intersections with spheres
    for (const Sphere& sphere : spheres) {
        float t;
        if (sphere.intersect(ray, t) && t < closest_t) {
            closest_t = t;
            hit = true;
            hitPoint = ray.origin + t * ray.direction;
            normal = glm::normalize(hitPoint - sphere.center);
            closest_color = glm::vec3(1.0f);  // White color for the sphere (no texture)
        }
    }

    // Check for intersections with planes
    for (const Plane& plane : planes) {
        float t;
        if (plane.intersect(ray, t) && t < closest_t) {
            closest_t = t;
            hit = true;
            hitPoint = ray.origin + t * ray.direction;
            normal = plane.normal;
            textureID = plane.textureID;
            closest_color = glm::vec3(1.0f);  // Default color for the plane (can be modified by texture)
        }
    }

    if (!hit) return glm::vec3(0.0f);  // If no intersection, return black (no color)

    // Texture mapping for planes only
    if (textureID != 0) {
        glm::vec2 texCoords = glm::vec2(hitPoint.x, hitPoint.z);  // Simple planar projection
        glBindTexture(GL_TEXTURE_2D, textureID);
        // Sample the texture color at texCoords (this part would require a texture sampling code).
    }

    // Compute ambient and diffuse lighting
    glm::vec3 ambient = 0.1f * closest_color;
    glm::vec3 lightDir = glm::normalize(light.position - hitPoint);
    float diff = glm::max(glm::dot(normal, lightDir), 0.0f);
    glm::vec3 diffuse = diff * closest_color;

    // Reflection logic
    glm::vec3 reflectDir = glm::normalize(glm::reflect(ray.direction, normal));
    Ray reflectRay = { hitPoint + normal * 0.001f, reflectDir };
    glm::vec3 reflectColor = trace(reflectRay, spheres, planes, light, viewPos, depth + 1);

    return ambient + diffuse + 0.5f * reflectColor;  // Combine ambient, diffuse, and reflection
}

// Function to render the scene
void renderScene(const std::vector<Sphere>& spheres, const std::vector<Plane>& planes, const Light& light, const glm::vec3& viewPos, int width, int height) {
    std::vector<glm::vec3> framebuffer(width * height);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float u = (x + 0.5f) / float(width) * 2.0f - 1.0f;
            float v = (y + 0.5f) / float(height) * 2.0f - 1.0f;

            Ray ray = { viewPos, glm::normalize(glm::vec3(u, v, -1.0f)) };

            framebuffer[y * width + x] = trace(ray, spheres, planes, light, viewPos);
        }
    }

    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POINTS);
    for (int i = 0; i < framebuffer.size(); ++i) {
        glColor3f(framebuffer[i].r, framebuffer[i].g, framebuffer[i].b);
        glVertex2f((i % width) / float(width) * 2.0f - 1.0f, (i / width) / float(height) * 2.0f - 1.0f);
    }
    glEnd();
    glFlush();
}

int main() {
    // Initialize GLFW and OpenGL context
    if (!glfwInit()) return -1;
    GLFWwindow* window = glfwCreateWindow(800, 600, "Ray Tracing with Textures and Reflections", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    // Load textures for the planes (no texture for sphere)
    // GLuint floorTexture = loadTexture("floor_texture.jpg");
    // GLuint wallTexture = loadTexture("wall_texture.jpg");

    // Create a sphere with no texture (just a white color)
    std::vector<Sphere> spheres = {
        { glm::vec3(0.0f, 0.0f, -3.0f), 1.0f }
    };

    // Create planes for the floor and wall, each with its own texture
    std::vector<Plane> planes = {
        { glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1 },
        { glm::vec3(3.0f, 0.0f, -3.0f), glm::vec3(-1.0f, 0.0f, 0.0f), 1 }
    };

    // Light source (yellow light)
    Light light = { glm::vec3(3.0f, 2.0f, -2.0f), glm::vec3(1.0f, 1.0f, 0.0f) };

    float azimuth = 0.0f;  // Angle for horizontal rotation
    float elevation = 0.2f; // Angle for vertical rotation
    float radius = 5.0f;    // Distance from the center of the scene

    while (!glfwWindowShouldClose(window)) {
        // Handle camera rotation
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) azimuth -= 0.01f;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) azimuth += 0.01f;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) elevation += 0.01f;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) elevation -= 0.01f;

        // Calculate the camera position based on azimuth and elevation
        glm::vec3 viewPos = glm::vec3(
            radius * cos(elevation) * sin(azimuth),
            radius * sin(elevation),
            radius * cos(elevation) * cos(azimuth)
        );

        renderScene(spheres, planes, light, viewPos, 800, 600);  // Render the scene

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

