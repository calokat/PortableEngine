#pragma once
#include "Transform.h"
#include "GLRenderer.h"
#include "misc_components.h"
#include "lights.h"
#ifdef _WIN64
#include "DIrectXRenderer.h"
#endif
void ComponentGUI(const Name& n);
void ComponentGUI(Transform& t);
void ComponentGUI(GLRenderer& r);
void ComponentGUI(Rotator& r); 
void ComponentGUI(PointLight& pl);
void ComponentGUI(DirectionalLight& dl);
#ifdef _WIN64
void ComponentGUI(DirectXRenderer& dxr);
#endif