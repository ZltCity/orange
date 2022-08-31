#include <wx/sizer.h>

#include "mainframe.hpp"
#include "tilemap_view.hpp"

namespace orange
{

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY, "Orange editor")
{
	wxMenu *menuFile = new wxMenu;
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuHelp, "&Help");

	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("Welcome to Orange editor..");

	auto sizer = new wxBoxSizer(wxHORIZONTAL);
	auto tileMapView = new TileMapView(this);

	sizer->Add(tileMapView, 1, wxEXPAND);
	SetSizer(sizer);
	SetAutoLayout(true);

	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
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
