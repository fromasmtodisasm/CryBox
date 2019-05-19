#pragma once
#include <memory>
#include <BlackBox/Texture.hpp>
#include <glm/glm.hpp>

class CShader;
class CShaderProgram;
class Object;
class CCamera;

struct Material
{
  Texture
  *diffuse,
  *specular,
  *bump,
  *normal,
  *mask;
  float shininess;
  glm::vec3 diffuseColor;

  CShaderProgram *program;
  std::shared_ptr<std::string> name;
  bool hasTexture = false;

  Material() : 
  diffuse(nullptr),
  specular(nullptr),
  bump(nullptr),
  normal(nullptr),
  mask(nullptr),
  diffuseColor({0,0,0}),
  shininess(64.0f),
  program(nullptr)
  {

  }
  void apply(Object *object, CCamera *camera);
  void setTexture(Texture *texture, const char *type);
private:
  void activeTexture(uint32_t block, const char *uniform, Texture *texture);
};