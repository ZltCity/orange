#include <iostream>

#include <fmt/format.h>

#include "application.hpp"

namespace orange
{

bool Editor::OnInit()
{
	try
	{
		wxApp::OnInit();

		(new MainFrame())->Show(true);

		return true;
	}
	catch (const std::exception &ex)
	{
		std::cout << fmt::format("Some shit happened:\n\t{}.\nShutting down..", ex.what()) << std::endl;

		return false;
	}
}

} // namespace orange

wxIMPLEMENT_APP(orange::Editor);
