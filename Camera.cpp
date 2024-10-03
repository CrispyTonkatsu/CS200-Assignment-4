/**
 * File: Camera.cpp
 * Name: Edgar Jose Donoso Mansilla (e.donosomansilla)
 * Assignment: 4
 * Course: CS200
 * Term: FALL24
 */

#include "Camera.h"
#include "Affine.h"

cs200::Camera::Camera() :
    center_point(point(0, 0)), right_vector(vector(1, 0)), up_vector(vector(0, 1)), rect_width(2), rect_height(2) {}

cs200::Camera::Camera(const glm::vec4 &C, const glm::vec4 &v, float W, float H) :
    center_point(C), right_vector(vector(-v.y, v.x)), up_vector(v), rect_width(W), rect_height(H) {}

cs200::Camera &cs200::Camera::moveRight(float x) {
  center_point = center() + (x * right());
  return *this;
}
cs200::Camera &cs200::Camera::moveUp(float y) {
  center_point = center() + (y * up());
  return *this;
}

cs200::Camera &cs200::Camera::rotate(float t) { 
  glm::mat4 rotation = cs200::rotate(-t);
  right_vector = rotation * right_vector;
  up_vector = rotation * up_vector;
  return *this;
}

cs200::Camera &cs200::Camera::zoom(float f) {
  right_vector = right() * f;
  up_vector = up() * f;
  return *this;
}

glm::mat4 cs200::affineInverse(const glm::mat4 &A) {
  float determinant = 1 / (A[0][0] * A[1][1] - A[1][0] * A[0][1]);
  glm::mat4 transformation = {
      {determinant * A[1][1], -determinant * A[0][1], 0, 0},
      {-determinant * A[1][0], determinant * A[0][0], 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1},
  };
  glm::mat4 translation = {
      {1, 0, 0, 0},
      {0, 1, 0, 0},
      {0, 0, 1, 0},
      {-A[3][0], -A[3][1], 0, 1},
  };
  return (transformation * translation);
}

glm::mat4 cs200::cameraToWorld(const Camera &cam) { return {cam.right(), cam.up(), vector(0, 0), cam.center()}; }

glm::mat4 cs200::worldToCamera(const Camera &cam) { return affineInverse(cameraToWorld(cam)); }

glm::mat4 cs200::cameraToNDC(const Camera &cam) {
  return {{1 / cam.width(), 0, 0, 0}, {0, 1 / cam.height(), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};
}

glm::mat4 cs200::NDCToCamera(const Camera &cam) { return affineInverse(cameraToNDC(cam)); }
