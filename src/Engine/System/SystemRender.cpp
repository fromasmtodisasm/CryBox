#include <BlackBox/3DEngine/3DEngine.hpp>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/IWindow.hpp>
#include <BlackBox/System/System.hpp>

void CSystem::Render()
{
	if (m_env.pRenderer)
	{
		PROFILER_PUSH_CPU_MARKER("CPU RENDER", Utils::COLOR_YELLOW);
		{
			m_Render->SetState(IRenderer::State::DEPTH_TEST, true);
			m_env.p3DEngine->SetCamera(GetViewCamera());
			m_env.p3DEngine->Draw();
		}
		PROFILER_POP_CPU_MARKER();
	}
}

void CSystem::RenderBegin()
{
	if (!m_env.pRenderer)
		return;
	PROFILER_SYNC_FRAME();
	PROFILER_PUSH_CPU_MARKER("Full frame", COLOR_GRAY);
	m_Render->SetState(IRenderer::State::DEPTH_TEST, true);
	m_Render->BeginFrame();
#if ENABLE_DEBUG_GUI
	if (m_GuiManager)
	{
		m_GuiManager->NewFrame();
		m_GuiManager->ShowDemoWindow();
	}
	//m_GuiManager.ShowNodeEditor();
#endif
}

void CSystem::RenderEnd()
{
	if (!m_env.pRenderer)
		return;
	PROFILER_POP_CPU_MARKER();
	{
		//DEBUG_GROUP("DRAW_PROFILE");
		PROFILER_DRAW();
	}
	if (m_Render)
	{
		m_Render->Update();
		if (IConsole* pConsole = GetIConsole())
			pConsole->Draw();
#if ENABLE_DEBUG_GUI
		if (m_GuiManager)
			m_GuiManager->Render();
#endif
		//if (m_bIsActive)
		m_pWindow->swap();
	}
}
