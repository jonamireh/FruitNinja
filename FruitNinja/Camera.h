#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <GLFW/glfw3.h>

static const float MaxVerticalAngle = 85.0f; //must be less than 90 to avoid gimbal lock

/*
// Where this is called from use this code
glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
glfwSetCursorPosCallback(window, mouse_callback);
glfwSetCursorPos(window, 0.0, 0.0);
*/

class Camera
{
private:
    // Camera Location
    double xPos, yPos, xPrev, yPrev;
    // Camera Angle
    float theta, phi;
    // Camera Vectors for lookAt
    // Key Map for Strafing
    bool keys[1024];
    // Camera pointers for callbacks
    static std::shared_ptr<Camera> playerCamera;
    static std::shared_ptr<Camera> debugCamera;
public:
    glm::vec3 camera_position;
    glm::vec3 camera_front;
    glm::vec3 camera_up;

    Camera();

    static void set_player_camera(std::shared_ptr<Camera> camera);

    static void player_mouse_callback(GLFWwindow*, double, double);

    static void player_key_callback(GLFWwindow*, int, int, int, int);

    static void set_debug_camera(std::shared_ptr<Camera> camera);

    static void debug_mouse_callback(GLFWwindow*, double, double);

    static void debug_key_callback(GLFWwindow*, int, int, int, int);

    void movement();

	glm::mat4 get_view_matrix();
};