#include "Utils/Il2cpp/lazy_importer.hpp"
#include "Utils/bytes_font.h"
#include "Cheat/Hooks/detours/detours.h"


extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WndProcHook(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (show && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	static auto is_down = true;
	static auto is_clicked = false;

	if (GetAsyncKeyState(VK_INSERT))
	{
		is_clicked = false;
		is_down = true;
	}
	else if (!GetAsyncKeyState(VK_INSERT) && is_down)
	{
		is_clicked = true;
		is_down = false;
	}
	else
	{
		is_clicked = false;
		is_down = false;
	}

	if (is_clicked)
	{
		show = !show;
	}
	if (GetAsyncKeyState(vars::keys::locktarget) & 1) {
		vars::combat::lock_target = !vars::combat::lock_target;
	}
	auto pressed_buttons = false;
	auto pressed_menu_key = uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONUP || uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONUP || uMsg == WM_MOUSEWHEEL;

	if (!pressed_menu_key)
		pressed_buttons = true;
	if (!pressed_buttons && show && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;
	return CallWindowProc(original_windowproc, hWnd, uMsg, wParam, lParam);
}
HRESULT resize_hook(IDXGISwapChain* swapChain, UINT bufferCount, UINT width, UINT height, DXGI_FORMAT newFormat, UINT swapChainFlags) {

	if (render_target_view) render_target_view->Release( );

	if (immediate_context) immediate_context->Release( );
	if (device) device->Release( );

	device = nullptr;

	return resize_original(swapChain, bufferCount, width, height, newFormat, swapChainFlags);
}

#include <Windows.h>
#include <string>
#include <iostream>
#include <Wininet.h>
#include <string>
#pragma comment(lib, "wininet.lib")
using namespace std;

HRESULT present_hook(IDXGISwapChain* swapChain, UINT SyncInterval, UINT Flags) {
	if (!device) {
		swapChain->GetDevice(__uuidof(device), reinterpret_cast<PVOID*>(&device));
		device->GetImmediateContext(&immediate_context);
		ID3D11Texture2D* renderTarget = nullptr;
		swapChain->GetBuffer(0, __uuidof(renderTarget), reinterpret_cast<PVOID*>(&renderTarget));
		device->CreateRenderTargetView(renderTarget, nullptr, &render_target_view);
		renderTarget->Release( );
		ImGui::CreateContext( );
		ImGui_ImplWin32_Init(game_window);
		ImGui_ImplDX11_Init(device, immediate_context);
		//OxyRender::InitRender(swapChain);
		//ImGui::StyleColorsDark();
		/*menu::style();*/
		ImFontConfig icons_config;
		ImGuiIO& io = ImGui::GetIO();
		ImFontConfig cfg;
		cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags::ImGuiFreeTypeBuilderFlags_MonoHinting | ImGuiFreeTypeBuilderFlags_Monochrome; //отключает антиалиасинг и дает строгий алгоритм хинта

		ImFontConfig font_config;

	static const ImWchar ranges[ ] = {

        0x0020, 0x00FF, // Basic Latin + Latin Supplement
        0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
        0x2DE0, 0x2DFF, // Cyrillic Extended-A
        0xA640, 0xA69F, // Cyrillic Extended-B
        0xE000, 0xE226, // icons
        0,
    };

	    io.Fonts->AddFontFromMemoryTTF((void*)font, sizeof(font), 13.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
		io.Fonts->AddFontFromMemoryTTF(icons_binary, sizeof icons_binary, 15, &font_config, ranges);
		io.Fonts->AddFontFromMemoryTTF((void*)Glyphter, sizeof(Glyphter), 13.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());

		font1 = io.Fonts->AddFontFromMemoryTTF((void*)font, sizeof(font), 13.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
		esp_font = io.Fonts->AddFontFromFileTTF(("C:\\Windows\\Fonts\\Verdana.ttf"), 12, &cfg, io.Fonts->GetGlyphRangesCyrillic());
		indShandow = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 30, NULL, io.Fonts->GetGlyphRangesCyrillic());
		pixel_s = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 13, NULL, io.Fonts->GetGlyphRangesCyrillic());
		calibri_ = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 13, NULL, io.Fonts->GetGlyphRangesCyrillic());
		fMain = io.Fonts->AddFontFromMemoryTTF(calibri, sizeof(calibri), 15, NULL, io.Fonts->GetGlyphRangesCyrillic());
		fSmall = io.Fonts->AddFontFromMemoryTTF(calibri, sizeof(calibri), 13, NULL, io.Fonts->GetGlyphRangesCyrillic());
		fName = io.Fonts->AddFontFromMemoryTTF(calibri, sizeof(calibri), 22, NULL, io.Fonts->GetGlyphRangesCyrillic());
		flogs = io.Fonts->AddFontFromMemoryTTF((void*)calibri, sizeof(calibri), 13.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());
		Verdanaboltbyser = io.Fonts->AddFontFromMemoryTTF(TopFont, sizeof(TopFont), vars::misc::fotsize, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
	}

	immediate_context->OMSetRenderTargets(1, &render_target_view, nullptr);
	immediate_context->RSGetViewports(&vps, &viewport);
	screen_size = {viewport.Width, viewport.Height};
	screen_center = {viewport.Width / 2.0f, viewport.Height / 2.0f};
	vars::stuff::ScreenHeight = viewport.Height;
	vars::stuff::ScreenWidth = viewport.Width;
	if (!vars::stuff::Panic) {
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		DrawList = ImGui::GetBackgroundDrawList();
		notify::render();
		pre_draw();

		if (show) {
			menu::DrawMenu();
		}
		
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}
	else {
		vars::misc::fov = 90.f;
		MH_DisableHook(MH_ALL_HOOKS);
	}
	return present_original(swapChain, SyncInterval, Flags);
}
void Start( ) {
	vars::stor::gBase = (DWORD64)GetModuleHandleA(("GameAssembly.dll"));
	vars::stor::uBase = (DWORD64)GetModuleHandleA(("UnityPlayer.dll"));
	il2cpp::init();

	while (!game_window) {
		EnumWindows([ ](HWND hWnd, LPARAM lParam) -> BOOL {
			DWORD pid = 0;
			GetWindowThreadProcessId(hWnd, &pid);
			if (pid == GetCurrentProcessId( )) {
				*reinterpret_cast<HWND*>(lParam) = hWnd;
				return FALSE;
			}

			return TRUE;
			}, reinterpret_cast<LPARAM>(&game_window));
	}

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	auto featureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC sd = {0};
	ZeroMemory(&sd, sizeof sd);
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = game_window;
	sd.SampleDesc.Count = 1;
	sd.Windowed = TRUE;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &featureLevel, 1, D3D11_SDK_VERSION, &sd, &swapChain, &device, nullptr, &context))) {
		MessageBoxW(0, (L"Failed to create D3D11 device and swap chain"), (L"Failure"), MB_ICONERROR);
	}
	else {
		auto table = *reinterpret_cast<PVOID**>(swapChain);
		present_original = reinterpret_cast<HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT)>(table[ 8 ]);
		resize_original = reinterpret_cast<HRESULT(__fastcall*)(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT)>(table[ 13 ]);

		context->Release( );
		device->Release( );
		swapChain->Release( );

		original_windowproc = (WNDPROC)SetWindowLongPtrW(game_window, GWLP_WNDPROC, (LONG_PTR)WndProcHook);

		DetourTransactionBegin( );
		DetourUpdateThread(GetCurrentThread( ));
		DetourAttach(&(LPVOID&)present_original, (PBYTE)present_hook);
		DetourTransactionCommit( );

		DetourTransactionBegin( );
		DetourUpdateThread(GetCurrentThread( ));
		DetourAttach(&(LPVOID&)resize_original, (PBYTE)resize_hook);
		DetourTransactionCommit( );
		
	}
}