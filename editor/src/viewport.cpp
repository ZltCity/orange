#include <stdexcept>

#include <orange/gl/utils.hpp>
#include <stb_image.h>

#include "viewport.hpp"

namespace orange
{

BEGIN_EVENT_TABLE(Viewport, wxGLCanvas)
EVT_SIZE(Viewport::resized)
EVT_PAINT(Viewport::render)
EVT_MOTION(Viewport::mouseMoved)
EVT_MOUSEWHEEL(Viewport::mouseWheelMoved)
EVT_LEFT_DOWN(Viewport::mouseDown)
EVT_LEFT_UP(Viewport::mouseReleased)
EVT_RIGHT_DOWN(Viewport::rightClick)
EVT_LEAVE_WINDOW(Viewport::mouseLeftWindow)
EVT_KEY_DOWN(Viewport::keyPressed)
EVT_KEY_UP(Viewport::keyReleased)
END_EVENT_TABLE()

Viewport::Viewport(wxFrame *parent)
	: wxGLCanvas(parent, wxID_ANY, contextAttributes), context(std::make_shared<wxGLContext>(this)), scaleFactor(10.0f)
{
}

void Viewport::resized(wxSizeEvent &evt)
{
	const auto newSize = evt.GetSize();

	screenSize = glm::ivec2 {newSize.GetWidth(), newSize.GetHeight()};
}

void Viewport::render(wxPaintEvent &evt)
{
	wxGLCanvas::SetCurrent(*context);

	if (renderer == nullptr)
	{
		initRenderer();
	}

	gl::invoke(glViewport, 0, 0, ToPhys(screenSize.x), ToPhys(screenSize.y));
	gl::invoke(glClearColor, 0.0f, 0.0f, 0.0f, 1.0f);
	gl::invoke(glClear, GL_COLOR_BUFFER_BIT);

	try
	{
		renderer->renderTileMap(0, screenSize, cameraPosition, scaleFactor);
	}
	catch (const std::exception &ex)
	{
		wxMessageBox(ex.what(), "Error occurred..", wxOK | wxICON_ERROR);
		abort();
	}

	gl::invoke(glFlush);

	SwapBuffers();
}

void Viewport::mouseMoved(wxMouseEvent &event)
{
	const auto mousePosition = event.GetPosition();

	if (event.ButtonIsDown(wxMouseButton::wxMOUSE_BTN_LEFT))
	{
		const auto delta = mousePosition - lastMousePosition;

		cameraPosition += glm::vec2 {-delta.x, delta.y} * 5.0f;

		wxWindow::Refresh();
	}

	lastMousePosition = mousePosition;
}

void Viewport::mouseWheelMoved(wxMouseEvent &event)
{
	scaleFactor += static_cast<float>(event.GetWheelRotation()) * 0.005f;

	wxWindow::Refresh();
}

void Viewport::mouseDown(wxMouseEvent &event)
{
}

void Viewport::mouseReleased(wxMouseEvent &event)
{
}

void Viewport::rightClick(wxMouseEvent &event)
{
}

void Viewport::mouseLeftWindow(wxMouseEvent &event)
{
}

void Viewport::keyPressed(wxKeyEvent &event)
{
}

void Viewport::keyReleased(wxKeyEvent &event)
{
}

void Viewport::initRenderer()
{
	if (!gladLoadGL())
	{
		throw std::runtime_error("Could not load GLAD.");
	}

	assetManager = std::make_shared<orange::AssetManager>("./assets");
	renderer = std::make_shared<orange::Renderer>(assetManager);

	resetTileMap();
}

void Viewport::resetTileMap()
{
	int tileMapWidth {}, tileMapHeight {}, tileMapBpp {};
	const auto tileMapData = stbi_load("assets/images/test-tilemap.png", &tileMapWidth, &tileMapHeight, &tileMapBpp, 4);

	if (tileMapData == nullptr)
	{
		throw std::runtime_error("Could not load tile map.");
	}

	auto tileMap = TileMap(tileMapWidth, tileMapHeight);

	for (uint32_t y = 0; y < tileMapHeight; ++y)
	{
		for (uint32_t x = 0; x < tileMapWidth; ++x)
		{
			tileMap.setTile(x, y, Tile {reinterpret_cast<const uint32_t *>(tileMapData)[x + y * tileMapWidth]});
		}
	}

	renderer->setTileMap(0, tileMap);
}

} // namespace orange
