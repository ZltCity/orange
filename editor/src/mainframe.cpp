#include <wx/mstream.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>

#include <fstream>

#include <embedded/resources.hpp>

#include <stb_image.h>

#include "mainframe.hpp"
#include "viewport.hpp"

namespace orange
{

const wxSize MainFrame::defaultIconSize = {16, 16};

wxBitmap createImage(orange::embedded::ResourceID resourceID)
{
	const auto resource = orange::embedded::getResource(resourceID);

	wxInitAllImageHandlers();
	//	auto stream = std::fstream("C:\\Users\\Roman\\Projects\\orange\\bin\\test.png", std::ios::binary |
	// std::ios::out);
	//
	//	for (auto byte : resource)
	//	{
	//		stream.put(byte);
	//	}
	//	auto width = int {}, height = int {}, channels = int {};
	//	const auto data = stbi_load_from_memory(
	//		reinterpret_cast<const stbi_uc *>(resource.data()), static_cast<int>(resource.size()), &width, &height,
	//		&channels, 4);
	auto stream = wxMemoryInputStream(resource.data(), resource.size());

	return wxBitmap(stream);
	//	return wxBitmap(reinterpret_cast<const char *>(data), width, height);
	//		return {};
}

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Orange editor")
{
	createMenuBar();
	createToolBar();
	createStatusBar();
	createViewport();

	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::createMenuBar()
{
	auto menuFile = new wxMenu;

	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	auto menuHelp = new wxMenu;

	menuHelp->Append(wxID_ABOUT);

	auto menuBar = new wxMenuBar;

	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);
}

void MainFrame::createToolBar()
{
	using namespace orange::embedded;

	auto iconMove = getResource(ResourceID::ICON_MOVE_SVG);
	auto iconSelectTile = getResource(ResourceID::ICON_SELECT_TILE_SVG);
	auto iconSelectRectangle = getResource(ResourceID::ICON_SELECT_REGION_SVG);
	auto toolbar = wxFrame::CreateToolBar();

	toolbar->AddTool(1, {}, wxBitmapBundle::FromSVG(iconMove.data(), iconMove.size(), defaultIconSize), "Move");
	toolbar->AddTool(
		2, {}, wxBitmapBundle::FromSVG(iconSelectTile.data(), iconSelectTile.size(), defaultIconSize), "Select tile");
	toolbar->AddTool(
		3, {}, wxBitmapBundle::FromSVG(iconSelectRectangle.data(), iconSelectRectangle.size(), defaultIconSize),
		"Select region");
	toolbar->AddSeparator();
	toolbar->Realize();
}

void MainFrame::createStatusBar()
{
	auto statusBar = CreateStatusBar();

	SetStatusText("Welcome to Orange editor..");
}

void MainFrame::createViewport()
{
	auto sizer = new wxBoxSizer(wxHORIZONTAL);
	auto viewport = new Viewport(this);

	sizer->Add(viewport, 1, wxEXPAND);

	SetSizer(sizer);
	SetAutoLayout(true);
}

void MainFrame::OnExit(wxCommandEvent &event)
{
	Close(true);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
	wxMessageBox("About placeholder", "About Orange", wxOK | wxICON_INFORMATION);
}

} // namespace orange
