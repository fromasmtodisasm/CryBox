#pragma once

#include <BlackBox/Render/ITechnique.hpp>
#include <BlackBox/Render/ScreenShader.hpp>
#include <BlackBox/IPostProcessor.hpp>
#include <BlackBox/Quad.hpp>

class FrameBufferObject;
struct ICVar;

class HdrTechnique : public ITechnique, public IPostProcessor
{
public:
  HdrTechnique();
  ~HdrTechnique();

  // Inherited via ITechnique
  /////////////////////////////////////////////////
  virtual bool Init(Scene* scene, FrameBufferObject* renderTarget) override;

  virtual bool OnRenderPass(int pass) override;

  virtual int GetFrame() override;

  virtual bool PreRenderPass() override;

  virtual void PostRenderPass() override;
  /////////////////////////////////////////////////
  virtual void Do(unsigned int texture) override;
private:
  bool HdrPass();
	void BloomPass();
  void createShader();

private:
  ITechnique *shadowMapping;
  CBaseShaderProgram* m_ScreenShader;
  CBaseShaderProgram* m_BlurShader;
  //ScreenShader *m_ScreenShader;
	Quad m_ScreenQuad;
  FrameBufferObject* hdrBuffer;
  FrameBufferObject* pingPongBuffer[2];
  Scene* m_Scene;

  ICVar* exposure;
  ICVar* enabled;
	ICVar* bloom;
	ICVar* bloomThreshold;

  bool inited = false;
	bool pingpong = false;

  IRender* render;
};