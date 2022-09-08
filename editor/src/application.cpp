#include "application.hpp"

namespace orange
{

bool Editor::OnInit()
{
	(new MainFrame())->Show(true);

	return true;
}

} // namespace orange

wxIMPLEMENT_APP(orange::Editor);
