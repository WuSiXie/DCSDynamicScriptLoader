#pragma once
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <sstream>
#include <vector>
#include "networkmoduel.h"
int ImGuiIDUsed = 0;
int RequestANewID()
{
	ImGuiIDUsed++;
	return ImGuiIDUsed;
}