#ifndef __CHARLES_OPENGL_CHAOS_CAMERA_H_
#define __CHARLES_OPENGL_CHAOS_CAMERA_H_

#include "opengl_chaos.h"
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CameraMovement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class CameraScene
{
public:
    // what can camera see
    float m_fov = 45.0f;
    float m_w_h_ratio = 800.0f / 600.0f;
    float m_z_near = 0.1f;
    float m_z_far = 100.0f;
public:
    CameraScene(float fov, float w_h_ratio, float m_z_near, float m_z_far);
    CameraScene(const CameraScene &other) = default;
    CameraScene() = default;
};

class Camera
{
public:
    // camera Attributes
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_world_up;
    // euler Angles
    float m_yaw;
    float m_pitch;
    // camera options
    float m_movement_speed = 2.5f;
    float m_mouse_sensitivity = 0.1f;
    float m_zoom;
    CameraScene m_cameraScene;
    Camera(CameraScene cameraScene,
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f, float pitch = 0.0f,
        float zoom = 45.0f);
    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix();
    void process_keyboard(CameraMovement direction, float delta_time);
    void process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch = true);
    void process_mouse_scroll(float y_offset);
    void scorll_control_position(float y_offset);
private:
    void update_camera_vectors();
};



#endif