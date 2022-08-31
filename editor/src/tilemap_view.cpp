#include <stdexcept>

#include <orange/gl/utils.hpp>
#include <stb_image.h>

#include "tilemap_view.hpp"

namespace orange
{

BEGIN_EVENT_TABLE(TileMapView, wxGLCanvas)
EVT_SIZE(TileMapView::resized)
EVT_PAINT(TileMapView::render)
EVT_MOTION(TileMapView::mouseMoved)
EVT_MOUSEWHEEL(TileMapView::mouseWheelMoved)
EVT_LEFT_DOWN(TileMapView::mouseDown)
EVT_LEFT_UP(TileMapView::mouseReleased)
EVT_RIGHT_DOWN(TileMapView::rightClick)
EVT_LEAVE_WINDOW(TileMapView::mouseLeftWindow)
EVT_KEY_DOWN(TileMapView::keyPressed)
EVT_KEY_UP(TileMapView::keyReleased)
END_EVENT_TABLE()

TileMapView::TileMapView(wxFrame *parent)
	: wxGLCanvas(parent, wxID_ANY, contextAttributes), context(std::make_shared<wxGLContext>(this)), scaleFactor(10.0f)
{
	wxGLCanvas::SetCurrent(*context);

	if (!gladLoadGL())
	{
		throw std::runtime_error("Could not load GLAD.");
	}

	assetManager = std::make_shared<orange::AssetManager>("./assets");
	renderer = std::make_shared<orange::Renderer>(assetManager);

	resetTileMap();
}

void TileMapView::resized(wxSizeEvent &evt)
{
	const auto newSize = evt.GetSize();

	screenSize = glm::ivec2 {newSize.GetWidth(), newSize.GetHeight()};

	gl::invoke(glViewport, 0, 0, screenSize.x, screenSize.y);
}

void TileMapView::render(wxPaintEvent &evt)
{
	if (!IsShown())
		return;

	wxGLCanvas::SetCurrent(*context);
	wxPaintDC(this);

	gl::invoke(glClearColor, 0.5f, 0.6f, 0.5f, 1.0f);
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

void TileMapView::mouseMoved(wxMouseEvent &event)
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

void TileMapView::mouseWheelMoved(wxMouseEvent &event)
{
	scaleFactor += static_cast<float>(event.GetWheelRotation()) * 0.005f;

	wxWindow::Refresh();
}

void TileMapView::mouseDown(wxMouseEvent &event)
{
}

void TileMapView::mouseReleased(wxMouseEvent &event)
{
}

void TileMapView::rightClick(wxMouseEvent &event)
{
}

void TileMapView::mouseLeftWindow(wxMouseEvent &event)
{
}

void TileMapView::keyPressed(wxKeyEvent &event)
{
}

void TileMapView::keyReleased(wxKeyEvent &event)
{
}

void TileMapView::resetTileMap()
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
