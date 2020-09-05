#pragma once
#include <string>
class Core {
protected:
	// Size of the window's client area
	unsigned int width;
	unsigned int height;

	// Does our window currently have focus?
	// Helpful if we want to pause while not the active window
	bool hasFocus;

	virtual std::string GetExePath();
	virtual std::wstring GetExePath_Wide();

	virtual std::string GetFullPathTo(std::string relativeFilePath);
	virtual std::wstring GetFullPathTo_Wide(std::wstring relativeFilePath);

public:
	virtual long InitWindow() = 0;
};