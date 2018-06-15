#pragma once
#include <GLFW/glfw3.h>
#include <filesystem>


class Native
{
public:
	Native(GLFWwindow* _window);
	std::filesystem::path openModelDialog();
	std::filesystem::path openSceneDialog();
	std::filesystem::path saveSceneDialog();

protected:
	std::filesystem::path invokeNativeOpenSaveDialog(const wchar_t* _filter, bool _save);

	GLFWwindow* m_window;
};
