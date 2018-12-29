//
// Created by mesmers on 18-12-28.
//

#ifndef HIGHOPENGL_CARMERA_H
#define HIGHOPENGL_CARMERA_H

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

using namespace glm;

const float PITCH = 0.0f;
const float YAW = -90.0f;
const float SPEED = 2.5f;
const float SENSITITY = 0.1f;
const float ZOOM = 45.0f;

enum CameraMovement {
    FORWARD,
    DOWN,
    LEFT,
    RIGHT
};

class Camera {
public:
    vec3 cameraPos;
    vec3 cameraFront;
    vec3 cameraUp;
    vec3 cameraRight;

    vec3 worldUp;

    float pitch;
    float yaw;
    float speed;
    float sensitity;
    float zoom;

    Camera(vec3 position = vec3(0, 0, 3), vec3 front = vec3(0, 0, -1)) :
            worldUp(0, 1, 0),
            pitch(PITCH),
            yaw(YAW),
            speed(SPEED),
            sensitity(SENSITITY),
            zoom(ZOOM) {
        cameraPos = position;
        cameraFront = front;
        updateAxis();
    }

    mat4 getViewMatrix() {
        return lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }

    void moveKey(CameraMovement movement, double deltaTime) {
        float offset = speed * deltaTime;
        if (movement == FORWARD) {
//            cameraPos += vec3(0, offset, 0);
            cameraPos += cameraFront * offset;
        }
        if (movement == DOWN) {
//            cameraPos -= vec3(0, offset, 0);
            cameraPos -= cameraFront * offset;
        }
        if (movement == LEFT) {
//            cameraPos -= vec3(offset, 0, 0);
            cameraPos -= cameraRight * offset;
        }
        if (movement == RIGHT) {
//            cameraPos += vec3(offset, 0, 0);
            cameraPos += cameraRight * offset;
        }
    }

    void moveMouse(double xOffset, double yOffset) {
        xOffset *= sensitity;
        yOffset *= sensitity;
        yaw += xOffset;
        pitch += yOffset;

        if (pitch > 89) {
            pitch = 89;
        }
        if (pitch < -89) {
            pitch = -89;
        }
        updateAxis();
    }

    void scrollMouse(float yOffset) {
        if (zoom >= 1 && zoom <= 45) {
            zoom += yOffset;
        }
        if (zoom <= 1) {
            zoom = 1;
        }
        if (zoom >= 45) {
            zoom = 45;
        }
    }


private:
    void updateAxis() {
        vec3 front;
        front.x = cos(radians(pitch)) * cos(radians(yaw));
        front.y = sin(radians((pitch)));
        front.z = cos(radians(pitch)) * sin(radians(yaw));
        cameraFront = normalize(front);
        cameraRight = normalize(cross(cameraFront, worldUp));
        cameraUp = normalize(cross(cameraRight, cameraFront));
    }
};

#endif //HIGHOPENGL_CARMERA_H
