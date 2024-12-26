#include "chaos_camera.h"
#include <iostream>
#include <format>

Camera::Camera(CameraScene cameraScene,
    glm::vec3 position,
    glm::vec3 up,
    float yaw, float pitch,
    float zoom)
{
    this->m_cameraScene = cameraScene;
	this->m_position = position;
	this->m_world_up = up;
	this->m_yaw = yaw;
	this->m_pitch = pitch;
    this->m_zoom = zoom;
    this->update_camera_vectors();
}

void Camera::update_camera_vectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
    front.y = sin(glm::radians(this->m_pitch));
    front.z = sin(glm::radians(this->m_yaw)) * cos(glm::radians(this->m_pitch));
    this->m_front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    this->m_right = glm::normalize(glm::cross(this->m_front, this->m_world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->m_up = glm::normalize(glm::cross(this->m_right, this->m_front));
}

glm::mat4 Camera::get_view_matrix()
{
    return glm::lookAt(this->m_position, this->m_position + this->m_front, this->m_up);

    //glm::mat4 translation = glm::mat4(1.0f); // Identity matrix by default
    //translation[3][0] = -this->m_position.x; // Fourth column, first row
    //translation[3][1] = -this->m_position.y;
    //translation[3][2] = -this->m_position.z;
    ////translation = glm::translate(translation, this->m_position);

    //glm::mat4 rotation = glm::mat4(1.0f);
    //rotation[0][0] = this->m_right.x;
    //rotation[1][0] = this->m_right.y;
    //rotation[2][0] = this->m_right.z;

    //rotation[0][1] = this->m_up.x;
    //rotation[1][1] = this->m_up.y;
    //rotation[2][1] = this->m_up.z;

    //rotation[0][2] = -this->m_front.x;
    //rotation[1][2] = -this->m_front.y;
    //rotation[2][2] = -this->m_front.z;

    //return rotation * translation;
}

void Camera::process_keyboard(CameraMovement direction, float delta_time)
{
    float velocity = this->m_movement_speed * delta_time;
    if (direction == CameraMovement::FORWARD)
    {
        this->m_position = this->m_position + this->m_front * velocity;
        //this->m_position.z = this->m_position.z + this->m_front.z * velocity;
    }
    if (direction == CameraMovement::BACKWARD)
    {
        this->m_position = this->m_position - this->m_front * velocity;
        //this->m_position.z = this->m_position.z - this->m_front.z * velocity;
    }
    if (direction == CameraMovement::LEFT)
    {
        this->m_position = this->m_position - this->m_right * velocity;
        //this->m_position.x = this->m_position.x - this->m_front.x * velocity;
    }
    if (direction == CameraMovement::RIGHT)
    {
        this->m_position = this->m_position + this->m_right * velocity;
        //this->m_position.x = this->m_position.x + this->m_front.x * velocity;
    }
}

void Camera::process_mouse_movement(float x_offset, float y_offset, bool constrain_pitch)
{
    x_offset *= this->m_mouse_sensitivity;
    y_offset *= this->m_mouse_sensitivity;

    this->m_yaw += x_offset;
    this->m_pitch += y_offset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrain_pitch)
    {
        if (this->m_pitch > 89.0f)
            this->m_pitch = 89.0f;
        if (this->m_pitch < -89.0f)
            this->m_pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    this->update_camera_vectors();
}

void Camera::process_mouse_scroll(float y_offset)
{
    this->m_zoom = this->m_zoom - y_offset;
    if (this->m_zoom < 1.0f)
    {
        this->m_zoom = 1.0f;
    }
    if (this->m_zoom > 89.0f)
    {
        this->m_zoom = 89.0f;
    }
}

void Camera::scorll_control_position(float y_offset)
{
    this->m_position = this->m_position + this->m_front * y_offset;
}

glm::mat4 Camera::get_projection_matrix()
{
    glm::mat4 projection;
    projection = glm::perspective(
        glm::radians(this->m_zoom),
        this->m_cameraScene.m_w_h_ratio,
        this->m_cameraScene.m_z_near,
        this->m_cameraScene.m_z_far);
    return projection;
}

CameraScene::CameraScene(float fov, float w_h_ratio, float z_near, float z_far)
{
    this->m_fov = fov;
    this->m_w_h_ratio = w_h_ratio;
    this->m_z_near = z_near;
    this->m_z_far = z_far;
}