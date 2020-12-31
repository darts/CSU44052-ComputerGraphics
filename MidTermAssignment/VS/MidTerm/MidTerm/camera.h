#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class Camera
{
public:
    enum class Camera_Directions {
        FORWARDS,
        BACKWARDS,
        LEFT,
        RIGHT
    };

    const float MAX_ZOOM = 1.0f;
    const float MIN_ZOOM = 50.0f;

    //positional information
    glm::vec3 camera_position; //this can be edited on instantiation
    //Since we are using a right hand coordinate system:
    //positive y axis is up, positive x axis is to the right, positive z axis is backwards
    glm::vec3 z_axis_vector = glm::vec3(0.0f, 0.0f, -1.0f), global_vertical = glm::vec3(0.0f, 1.0f, 0.0f), y_axis_vector, x_axis_vector; //these are currently preset

    //angle information, yaw = left/right, pitch=up/down
    //facing towards the object, horizontally
    float camera_yaw = -90.0f, camera_pitch = 0.0f; 

    //default movement characteristics
    float movement_speed = 10.0f, mouse_speed = 0.05f;

    //default zoom level
    float camera_zoom = MIN_ZOOM;

    // constructor with vectors
    Camera(glm::vec3 starting_position = glm::vec3(0.0f))
    {
        camera_position = starting_position;
        updateCameraVectors();
    }

    //generate the view matrix using glm lookAt()
    glm::mat4 GenerateCameraViewMatrix()
    {
        return glm::lookAt(camera_position, camera_position + z_axis_vector, y_axis_vector);
    }

    //handles key presses
    //including the time thing makes movement smoother
    //otherwise movement speed is seriously affected by the frame time
    void HandleKeyPress(Camera_Directions direction, float time_diff)
    {
        //std::cout << time_diff << std::endl;
        float normalised_speed = movement_speed * time_diff;
        if (direction == Camera_Directions::FORWARDS)
            camera_position += z_axis_vector * normalised_speed;
        if (direction == Camera_Directions::BACKWARDS)
            camera_position -= z_axis_vector * normalised_speed;
        if (direction == Camera_Directions::LEFT)
            camera_position -= x_axis_vector * normalised_speed;
        if (direction == Camera_Directions::RIGHT)
            camera_position += x_axis_vector * normalised_speed;
    }

    //handle mouse movement
    void HandleMouseMove(float mouse_move_x, float mouse_move_y)
    {
        camera_yaw += (mouse_move_x * mouse_speed);//turn laterally
        camera_pitch += (mouse_move_y * mouse_speed);//turn vertically 

        updateCameraVectors();
    }

    //handle mouse scrolling for zoom
    void HandleMouseScroll(float scroll_y)
    {
        camera_zoom -= scroll_y;
        if (camera_zoom > MIN_ZOOM)//don't zoom out too far
            camera_zoom = MIN_ZOOM;
        if (camera_zoom < MAX_ZOOM)//don't zoom in too far
            camera_zoom = MAX_ZOOM;
    }

private:
    //update the direction vectors
    void updateCameraVectors()
    {
        // calculate the new z_axis_vector so we know what is in front of us
        glm::vec3 front;
        front.x = cos(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
        front.y = sin(glm::radians(camera_pitch));
        front.z = sin(glm::radians(camera_yaw)) * cos(glm::radians(camera_pitch));
        z_axis_vector = glm::normalize(front);
        // also re-calculate the x_axis_vector and y_axis_vector
        x_axis_vector = glm::normalize(glm::cross(z_axis_vector, global_vertical));
        y_axis_vector = glm::normalize(glm::cross(x_axis_vector, z_axis_vector));
    }
};
