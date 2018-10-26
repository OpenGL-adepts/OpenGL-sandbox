#include "native.hpp"


Native::Native(GLFWwindow* _window)
	: m_window(_window)
{
}


std::filesystem::path Native::openModelDialog()
{
	auto filter =
		L"3D models (*.FBX;*.DAE;*.GLTF;*.GLB;*.BLEND;*.3DS;*.ASE;*.OBJ;*.IFC;*.XGL;*.ZGL;*.PLY;*.DXF;*.LWO;*.LWS;*.LXO;*.STL;*.X;*.AC;*.MS3D;*.COB;*.SCN)\0*.FBX;*.DAE;*.GLTF;*.GLB;*.BLEND;*.3DS;*.ASE;*.OBJ;*.IFC;*.XGL;*.ZGL;*.PLY;*.DXF;*.LWO;*.LWS;*.LXO;*.STL;*.X;*.AC;*.MS3D;*.COB;*.SCN\0"
		L"Autodesk (*.FBX)\0*.FBX\0"
		L"Collada (*.DAE)\0*.DAE\0"
		L"glTF (*.GLTF;*.GLB)\0*.GLTF;*.GLB\0"
		L"Blender 3D (*.BLEND)\0*.BLEND\0"
		L"3ds Max 3DS (*.3DS)\0*.3DS\0"
		L"3ds Max ASE (*.ASE)\0*.ASE\0"
		L"Wavefront object (*.OBJ)\0*.OBJ\0"
		L"Industry Foundation Classes (IFC/Step) (*.IFC)\0*.IFC\0"
		L"XGL (*.XGL;*.ZGL)\0*.XGL;*.ZGL\0"
		L"Stanford Polygon Library (*.PLY)\0*.PLY\0"
		L"AutoCAD DXF (*.DXF)\0*.DXF\0"
		L"LightWave (*.LWO;*.LWS)\0*.LWO;*.LWS\0"
		L"Modo (*.LXO)\0*.LXO\0"
		L"Stereolithography (*.STL)\0*.STL\0"
		L"DirectX X (*.X)\0*.X\0"
		L"AC3D (*.AC)\0*.AC\0"
		L"Milkshape 3D (*.ms3d)\0*.ms3d\0"
		L"TrueSpace (*.COB;*.SCN)\0*.COB;*.SCN\0"
		L"All files (*.*)\0*.*\0";

	return invokeNativeOpenSaveDialog(filter, false);
}


std::filesystem::path Native::openTextureDialog()
{
	auto filter =
		L"Image file (*.PNG;*.JPG;*.TGA)\0*.PNG;*.JPG;*.TGA\0"
		L"All files (*.*)\0*.*\0";

	return invokeNativeOpenSaveDialog(filter, false);
}


std::filesystem::path Native::openSceneDialog()
{
	auto filter =
		L"OpenGL sandbox scene (*.JSON)\0*.JSON\0"
		L"All files (*.*)\0*.*\0";

	return invokeNativeOpenSaveDialog(filter, false);
}


std::filesystem::path Native::saveSceneDialog()
{
	auto filter =
		L"OpenGL sandbox scene (*.JSON)\0*.JSON\0"
		L"All files (*.*)\0*.*\0";

	return invokeNativeOpenSaveDialog(filter, true);
}


std::filesystem::path Native::invokeNativeOpenSaveDialog(const wchar_t* _filter, bool _save)
{
	OPENFILENAMEW ofnw;
	memset(&ofnw, 0, sizeof ofnw);
	ofnw.lStructSize = sizeof ofnw;
	ofnw.hwndOwner = glfwGetWin32Window(m_window);
	ofnw.Flags = OFN_FILEMUSTEXIST;
	ofnw.lpstrFilter = _filter;

	wchar_t buffer[MAX_PATH] = {};
	ofnw.lpstrFile = buffer;
	ofnw.nMaxFile = MAX_PATH;
	ofnw.lpstrDefExt = (LPCWSTR)"json";

	if(_save ? GetSaveFileNameW(&ofnw) : GetOpenFileNameW(&ofnw))
		return buffer;

	return {};
}
