#include "Camera.h"
#include <iostream>

using namespace std;

std::shared_ptr<Camera> Camera::playerCamera = NULL;

std::shared_ptr<Camera> Camera::debugCamera = NULL;

Camera::Camera()
{
    xPos = 0.0f;
    yPos = 0.0f;
    xPrev = 0.0f;
    yPrev = 0.0f;
    theta = 0.0f;
    phi = 0.0f;
    camera_position = vec3(0.0f, 0.6f, 3.0f);
    camera_front = vec3(0.0f, 0.0f, -1.0f);
    camera_up = vec3(0.0f, 1.0f, 0.0f);
    keys[1024] = { false };
}

/**
Sets the camera to be used in the callback calls
*/
void Camera::set_player_camera(std::shared_ptr<Camera> camera)
{
    playerCamera = camera;
}


/**
Call the glfwSetCursorPosCallback to the window and this function.
*/
void Camera::player_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - playerCamera->xPrev;
    float yoffset = playerCamera->yPrev - ypos;
    playerCamera->xPrev = xpos;
    playerCamera->yPrev = ypos;

    float sensitivity = 0.000001f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    playerCamera->theta += xoffset;
    playerCamera->phi += yoffset;

    if (playerCamera->phi > MaxVerticalAngle)
        playerCamera->phi = MaxVerticalAngle;
    if (playerCamera->phi < -MaxVerticalAngle)
        playerCamera->phi = -MaxVerticalAngle;

    vec3 front;
    front.x = cos(radians(playerCamera->theta)) * cos(radians(playerCamera->phi));
    front.y = sin(radians(playerCamera->phi));
    front.z = sin(radians(playerCamera->theta)) * cos(radians(playerCamera->phi));
    playerCamera->camera_front = normalize(front);
}

/**
Call the glfwSetKeyCallback to the window and this function.
*/
void Camera::player_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
        playerCamera->keys[key] = true;
    else if (action == GLFW_RELEASE)
        playerCamera->keys[key] = false;
}

/**
Sets the debug camera to be used in the callback calls
*/
void Camera::set_debug_camera(std::shared_ptr<Camera> camera)
{
    debugCamera = camera;
}


/**
Call the glfwSetCursorPosCallback to the window and this function.
*/
void Camera::debug_mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    GLfloat xoffset = xpos - debugCamera->xPrev;
    GLfloat yoffset = debugCamera->yPrev - ypos;
    debugCamera->xPrev = xpos;
    debugCamera->yPrev = ypos;

    GLfloat sensitivity = 0.1;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    debugCamera->theta += xoffset;
    debugCamera->phi += yoffset;

    if (debugCamera->phi > MaxVerticalAngle)
        debugCamera->phi = MaxVerticalAngle;
    if (debugCamera->phi < -MaxVerticalAngle)
        debugCamera->phi = -MaxVerticalAngle;

    vec3 front;
    front.x = cos(radians(debugCamera->theta)) * cos(radians(debugCamera->phi));
    front.y = sin(radians(debugCamera->phi));
    front.z = sin(radians(debugCamera->theta)) * cos(radians(debugCamera->phi));
    debugCamera->camera_front = normalize(front);
}

/**
Call the glfwSetKeyCallback to the window and this function.
*/
void Camera::debug_key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)
        debugCamera->keys[key] = true;
    else if (action == GLFW_RELEASE)
        debugCamera->keys[key] = false;
}

/**
Call this at the end of the draw loop to update for strafing.
*/
void Camera::movement()
{
    float cameraSpeed = 0.2f;
    if (keys[GLFW_KEY_W])
        camera_position += cameraSpeed * vec3(camera_front.x, 0.f, camera_front.z);
    if (keys[GLFW_KEY_S])
        camera_position -= cameraSpeed * vec3(camera_front.x, 0.f, camera_front.z);
    if (keys[GLFW_KEY_A])
        camera_position -= normalize(cross(camera_front, camera_up)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        camera_position += normalize(cross(camera_front, camera_up)) * cameraSpeed;
}

mat4 Camera::get_view_matrix()
{
    return lookAt(camera_position, camera_position + camera_front, camera_up);
}