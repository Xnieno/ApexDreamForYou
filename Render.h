#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_win32.h"
#include "Imgui/imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>

#include <iostream>
#include <Windows.h>

// Data
static ID3D11Device* g_pd3dDevice = nullptr;
static ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
static IDXGISwapChain* g_pSwapChain = nullptr;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "Imgui/imgui_internal.h"

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")
#include "background.h"
#include "user_circle.h"
#include "inter.h"
#include "ico_pack.h"
#include "Imgui/color.h"
using namespace std;

void CustomStyleColor();

void Particles();

int Render();