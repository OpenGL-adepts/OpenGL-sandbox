#pragma once

#include <vector>
#include <string>
#include <imgui.h>
#include <algorithm>


// ImGui helpers
struct Gui
{
	static bool combo(const std::string& _label, int& _currentItem, const std::vector<std::string>& _items);
};
