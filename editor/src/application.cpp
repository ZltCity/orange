#include "application.hpp"

namespace orange
{

wxIMPLEMENT_APP(Editor);

bool Editor::OnInit()
{
	(new MainFrame())->Show(true);

	return true;
}

} // namespace orange
