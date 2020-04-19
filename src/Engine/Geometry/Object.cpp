#include <BlackBox/Renderer/Object.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Geometry/ObjLoader.hpp>
#include <BlackBox/Renderer/Renderer.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/Material.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

#include <fstream>
#include <iostream>
#include <cctype>
#include <cstdio>
#include <sstream>
#include <memory>

using namespace std;
int Object::refs = 0;

Object::Object() : m_transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)), velocity(glm::vec3(0))
{
}

Object::Object(MeshList mesh) : m_Mesh(mesh)
{
}

Object::Object(const Object* obj) :
  m_transform(obj->m_transform.position, obj->m_transform.rotation, obj->m_transform.scale),
  m_Mesh(obj->m_Mesh), m_Shader(obj->m_Shader),
  velocity(glm::vec3(0)),
  m_path(obj->m_path),
  type(obj->type)
{
  refs++;
}

void Object::draw(SRenderParams& renderParams) {
  DEBUG_GROUP(__FUNCTION__);
  glm::mat3 NormalMatrix(1.0);

	add_uniform(renderParams, "NormalMatrix", glm::mat3(glm::transpose(glm::inverse(getTransform()))));
	auto &p = m_Material->program;
	p->Use();

  for (auto& mesh : *m_Mesh)
  {
		for (auto& uv : renderParams.uniforms)
		{
			switch (uv.type)
			{
			case UniformValue::Type::FLOAT_VAL:
				p->Uniform(uv.val.f, uv.name.data());
				break;
			case UniformValue::Type::INT_VAL:
				p->Uniform(uv.val.i, uv.name.data());
				break;
			case UniformValue::Type::V1_VAL:
				p->Uniform(uv.val.v1, uv.name.data());
				break;
			case UniformValue::Type::V2_VAL:
				p->Uniform(uv.val.v2, uv.name.data());
				break;
			case UniformValue::Type::V3_VAL:
				p->Uniform(uv.val.v3, uv.name.data());
				break;
			case UniformValue::Type::V4_VAL:
				p->Uniform(uv.val.v4, uv.name.data());
				break;
			case UniformValue::Type::M2_VAL:
				p->Uniform(uv.val.m2, uv.name.data());
				break;
			case UniformValue::Type::M3_VAL:
				p->Uniform(uv.val.m3, uv.name.data());
				break;
			case UniformValue::Type::M4_VAL:
				p->Uniform(uv.val.m4, uv.name.data());
				break;
			case UniformValue::Type::Samp_VAL:
				p->Uniform(static_cast<const ITexture*>(uv.val.t), uv.name.data());
				break;
			case UniformValue::Type::CAMERA_VAL:
			{
				CCamera *c = uv.val.c;
				p->Uniform(c->getViewMatrix(), "view");
				p->Uniform(c->getProjectionMatrix(), "projection");
				p->Uniform(c->getProjectionMatrix(), "viewPos");
				break;
			}
				break;
			default:
				break;
			}
		}
		m_Material->apply(this);
		gEnv->pRenderer->DrawBuffer(mesh.m_Verts, nullptr, 0, 0, static_cast<int>(RenderPrimitive::TRIANGLES));
  }
}

glm::mat4 Object::getTransform()
{
  glm::mat4x4 translate(1.0f), rotate(1.0f), scale(1.0f);
  scale = glm::scale(scale, m_transform.scale);
  translate = glm::translate(translate, m_transform.position);
  rotate = glm::rotate(rotate, glm::radians(m_transform.rotation.x), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
  rotate = glm::rotate(rotate, glm::radians(m_transform.rotation.y), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
  rotate = glm::rotate(rotate, glm::radians(m_transform.rotation.z), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
  return translate * rotate * scale;
}

void Object::updateVectors()
{
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
  front.y = sin(glm::radians(this->Pitch));
  front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
  this->Front = glm::normalize(front);
  // Also re-calculate the Right and Up vector
  this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

bool Object::visible()
{
  return m_visible;
}

void Object::setVisibility(bool v)
{
  m_visible = v;
}

void Object::update(float deltatime)
{
  /*
  if (m_transform.position.y < 0)
    velocity.y = - velocity.y*friction;
  m_transform.position += velocity * deltatime;
  */
  updateVectors();
}

void Object::setTexture(Texture* texture, const char* type)
{
  m_Material->setTexture(texture, type);
}

Object Object::operator=(Object& that)
{
  Object obj;
  obj.m_Mesh = that.m_Mesh;
  return obj;
}

Object* Object::clone()
{
  Object* obj = new Object;
  obj->m_Mesh = this->m_Mesh;
  return obj;
}

Material* Object::getMaterial()
{
  return m_Material;
}

void Object::setMaterial(Material* material)
{
  if (m_Material != nullptr)
    delete m_Material;
  m_Material = material;
}

void Object::setRenderMode(int mode)
{
  m_RenderMode = mode;
}

int Object::getRenderMode()
{
  return m_RenderMode;
}

void Object::rotateX(float angle)
{
  m_transform.rotation.x = angle;
}

void Object::rotateY(float angle)
{
  m_transform.rotation.y = angle;
}

void Object::rotateZ(float angle)
{
  m_transform.rotation.z = angle;
}

void Object::SetScriptObject(IScriptObject* pObject)
{
  m_pScript = pObject;
}

IScriptObject* Object::GetScriptObject()
{
  return m_pScript;
}

void Object::move(Movement direction) {
  GLfloat velocity = this->MovementSpeed;
  if (direction == FORWARD)
    this->m_transform.position += glm::vec3(this->Front.x, this->Front.y, this->Front.z) * velocity;
  if (direction == BACKWARD)
    this->m_transform.position -= glm::vec3(this->Front.x, this->Front.y, this->Front.z) * velocity;
  if (direction == LEFT)
    this->m_transform.position -= this->Right * velocity;
  if (direction == RIGHT)
    this->m_transform.position += this->Right * velocity;
  if (direction == UP)
    this->m_transform.position += this->Up * velocity;
  if (direction == DOWN)
    this->m_transform.position -= this->Up * velocity;
}

void Object::move(glm::vec3 v)
{
  this->m_transform.position += v;
}

void Object::moveTo(glm::vec3 v)
{
  m_transform.position = v;
}

void Object::rotate(float angle, glm::vec3 v) {
  if (v.x != 0.0) m_transform.rotation.x = angle;
  if (v.y != 0.0) m_transform.rotation.y = angle;
  if (v.z != 0.0) m_transform.rotation.z = angle;
}

void Object::scale(glm::vec3 v)
{
  m_transform.scale = v;
}

Object* Object::load(string path)
{
  Object* obj = nullptr;
  MeshList mesh;
  CVertexBuffer* vb = nullptr;
  VerteciesInfo vertecies;
  BoundingBox bb;
  ObjLoader OBJ;

  if (!OBJ.load(path.c_str(), vertecies, bb))
    return nullptr;

  if (!gEnv->IsDedicated())
  {
		vb = gEnv->pRenderer->CreateBuffer(vertecies.data.size(), VERTEX_FORMAT_P3F_N_TEX2F, ("model: " + path).c_str());
		gEnv->pRenderer->UpdateBuffer(vb, vertecies.data.data(), static_cast<int>(vertecies.data.size()), false);
  }
  mesh = std::make_shared<std::vector<Mesh>>();
  Mesh _mesh(vb);
  _mesh.bb = bb;
  mesh->push_back(_mesh);
  obj = new Object();
  obj->m_Mesh = mesh;
  obj->m_path = path;
  return obj;
}

Transform::Transform()
{
}
