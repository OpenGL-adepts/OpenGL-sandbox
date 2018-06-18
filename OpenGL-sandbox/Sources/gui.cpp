#include "gui.hpp"


//static
bool Gui::combo(const std::string& _label, int& _currentItem, const std::vector<std::string>& _items)
{
	std::vector<const char*> itemsCstr;
	itemsCstr.resize(_items.size());
	std::transform(_items.begin(), _items.end(), itemsCstr.begin(), [](const std::string& _str) { return _str.c_str(); });
	return ImGui::Combo(_label.c_str(), &_currentItem, itemsCstr.data(), itemsCstr.size());
}
