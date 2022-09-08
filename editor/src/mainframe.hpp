#pragma once

#include <wx/wx.h>

namespace orange
{

class MainFrame : public wxFrame
{
public:
	MainFrame();

	static const wxSize defaultIconSize;

private:
	void createMenuBar();
	void createToolBar();
	void createStatusBar();
	void createViewport();

	void OnExit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
};

} // namespace orange
