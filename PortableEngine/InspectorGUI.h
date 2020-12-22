#pragma once
#include "Transform.h"
#include "Renderer.h"
#include "misc_components.h"
#ifdef _WIN64
#include "DIrectXRenderer.h"
#endif

void ComponentGUI(Transform& t);
void ComponentGUI(GLRenderer& r);
void ComponentGUI(Rotator& r);
#ifdef _WIN64
void ComponentGUI(DirectXRenderer& dxr);
#endif