#pragma once
#include <BlackBox/Renderer/BaseRenderer.hpp>

struct IWindow;

typedef int glContextAttribute;

enum AttributeType
{
  DEBUG,
  RELEASE,
  CORE
};

struct Hardware
{
  const GLubyte* unknown_desc;
  Hardware()
    :
      vendor(unknown_desc),
      render(unknown_desc),
      version(unknown_desc),
      glsl_version(unknown_desc)
  {

  }
  const GLubyte* vendor;
  const GLubyte* render;
  const GLubyte* version;
  const GLubyte* glsl_version;
};

struct SVertexPoolEntry
{
	bool free;
	CVertexBuffer* vertexBuffer;
};

class GLRenderer :
  public CRenderer,
  public IConsoleVarSink,
  public IInputEventListener
{
public:
  GLRenderer(ISystem* engine);
  ~GLRenderer();

public:
  // Inherited via IRenderer
  virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr) override;

  virtual bool ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) override;

  virtual void Release() override;

  virtual void BeginFrame(void) override;

  virtual void Update(void) override;

  virtual void GetViewport(int* x, int* y, int* width, int* height) override;

  virtual void SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) override;

  virtual void SetScissor(int x = 0, int y = 0, int width = 0, int height = 0) override;

  virtual void Draw3dBBox(const Vec3& mins, const Vec3& maxs) override;

  virtual void SetCamera(const CCamera& cam) override;

  virtual const CCamera& GetCamera() override;

  virtual bool ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) override;

  virtual void ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

  virtual void Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info) override;

  virtual int SetPolygonMode(int mode) override;

  virtual int GetWidth() override;

  virtual int GetHeight() override;

  virtual void ScreenShot(const char* filename = nullptr) override;

  virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) override;

  virtual void DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a) override;

  virtual void PrintLine(const char* szText, SDrawTextInfo& info) override;

  virtual void SetState(State state, bool enable) override;
  // Inherited via IRenderer
  virtual void DrawFullScreenImage(int texture_id) override;

  virtual void SetCullMode(CullMode mode = CullMode::BACK) override;

  virtual bool DeleteContext(WIN_HWND hWnd) override;

  virtual bool CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX = 1, int SSY = 1) override;

  virtual bool SetCurrentContext(WIN_HWND hWnd) override;

  virtual void MakeMainContextActive() override;

  virtual WIN_HWND GetCurrentContextHWND() override;

  virtual bool IsCurrentContextMainVP() override;

  virtual int GetCurrentContextViewportHeight() const override;

  virtual int GetCurrentContextViewportWidth() const override;

  virtual void SetClearColor(const Vec3& vColor) override;

  virtual void ClearDepthBuffer() override;

  virtual void ClearColorBuffer(const Vec3 vColor) override;

  // Inherited via IConsoleVarSink
  virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;
  // Inherited via IInputEventListener
  virtual bool OnInputEvent(const SInputEvent& event) override;
private:
  void glInit();
  void fillSates();
  void InitConsoleVariables();
  void InitConsoleCommands();
  void printHardware();

private:
  IWindow* m_Window = nullptr;
  ISystem* m_pSystem = nullptr;

  bool is_fullscreen = false;
  Vec4 m_viewPort;
  unsigned int cbpp = 0;
  int zbpp = 0;
  int sbits = 0;

  //============
  const GLuint majorVersion = 4;
  const GLuint minorVersion = 3;
  const int antialiassing = 8;
	std::shared_ptr<OpenglDebuger> glDebug;
  //============
  CCamera* m_Camera = nullptr;
  Quad* m_ScreenQuad = nullptr;
  // Shaders
  BaseShaderProgramRef m_ScreenShader;
#if defined(_DEBUG) || defined(GL_DEBUG) || !defined(NDEBUG)
  bool isDebug = true;
#else
  bool isDebug = false;
#endif
#if defined(_DEBUG) || defined(GL_DEBUG)
  glContextAttribute glContextType = AttributeType::DEBUG;
#else
  glContextAttribute glContextType = AttributeType::CORE;
#endif
  // DEBUG
  ICVar* translateImageX = nullptr;
  ICVar* translateImageY = nullptr;

  ICVar* scaleImageX = nullptr;
  ICVar* scaleImageY = nullptr;

  ICVar* needTranslate = nullptr;
  ICVar* needFlipY = nullptr;

  ICVar* r_debug = nullptr;
  ICVar* test_proj = nullptr;
  ICVar* render_via_viewport = nullptr;

  ICVar* cam_width;
  ICVar* cam_height;

  std::map<State, GLenum> stateMap;

  HWND m_HWND;
  void* context;

  Vec3 m_clearColor;
  float m_clearDepth;

  int m_CurrentTarget;

  Hardware m_Hardware;

	std::vector<SVertexPoolEntry> m_VertexBufferPool;
	int m_FrameID = 0;

  // Inherited via IRenderer
  virtual void SetRenderTarget(int nHandle) override;

  // Inherited via CRenderer
  virtual IShader* Sh_Load(ShaderDesc const& desc) override;

  // Inherited via CRenderer
  virtual void DrawFullscreenQuad() override;
  virtual int GetFrameID(bool bIncludeRecursiveCalls = true) override;

	// Inherited via CRenderer
	virtual void Set2DMode(bool enable, int ortox, int ortoy) override;

	virtual CVertexBuffer* CreateBuffer(int vertexcount, int vertexformat, const char* szSource, bool bDynamic = false) override;

	virtual void ReleaseBuffer(CVertexBuffer* bufptr) override;

	virtual void DrawBuffer(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start = 0, int vert_stop = 0, CMatInfo* mi = NULL) override;

	virtual void UpdateBuffer(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs = 0, int Type = 0) override;

	virtual void CreateIndexBuffer(SVertexStream* dest, const void* src, int indexcount) override;

	virtual void UpdateIndexBuffer(SVertexStream* dest, const void* src, int indexcount, bool bUnLock = true) override;

	virtual void ReleaseIndexBuffer(SVertexStream* dest) override;

};
