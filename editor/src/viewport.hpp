#pragma once

#include <memory>

#include <orange/renderer.hpp>

#include <wx/glcanvas.h>
#include <wx/wx.h>

namespace orange
{

class Viewport : public wxGLCanvas
{
public:
	explicit Viewport(wxFrame *parent);

	void resized(wxSizeEvent &evt);

	void render(wxPaintEvent &evt);

	void mouseMoved(wxMouseEvent &event);
	void mouseWheelMoved(wxMouseEvent &event);
	void mouseDown(wxMouseEvent &event);
	void mouseReleased(wxMouseEvent &event);
	void rightClick(wxMouseEvent &event);
	void mouseLeftWindow(wxMouseEvent &event);
	void keyPressed(wxKeyEvent &event);
	void keyReleased(wxKeyEvent &event);

	DECLARE_EVENT_TABLE()

private:
	constexpr static int contextAttributes[] = {WX_GL_CORE_PROFILE, WX_GL_MAJOR_VERSION, 4, WX_GL_MINOR_VERSION, 6};

	void resetTileMap();

	std::shared_ptr<wxGLContext> context;
	std::shared_ptr<orange::AssetManager> assetManager;
	std::shared_ptr<orange::Renderer> renderer;

	wxPoint lastMousePosition;

	glm::ivec2 screenSize;
	glm::vec2 cameraPosition;
	float scaleFactor;
};

} // namespace orange
