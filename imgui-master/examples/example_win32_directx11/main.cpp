
#include "main.h"


//using namespace KeyAuth;
//
//std::string name = "Ost_PERSONAL"; // application name. right above the blurred text aka the secret on the licenses tab among other tabs
//std::string ownerid = "PFa3QPrl49"; // ownerid, found in account settings. click your profile picture on top right of dashboard and then account settings.
//std::string secret = "a82f04c64df053385f3033202d40a76ac8d90840067b658366cd749805c07b29"; // app secret, the blurred text on licenses tab and other tabs
//std::string version = "2.0"; // leave alone unless you've changed version on website
//
//api KeyAuthApp(name, ownerid, secret, version);

class c_datos {
public:

    char Username[255] = "";
    char Password[255] = "";
    char license[255] = "";
    char Key[255] = "";


};

inline c_datos datos;


void ToggleClickability(bool clickable)
{
    if (!hwnd) return;

    LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    
    if (clickable)
    {
        // Hacer la ventana clickable y visible
        exStyle = exStyle & ~WS_EX_TRANSPARENT;
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);
        
        // Asegurar que la ventana esté en primer plano
        SetForegroundWindow(hwnd);
        SetFocus(hwnd);
        
        // Asegurar opacidad completa
        SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
        
        // Mostrar ventana
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);
    }
    else
    {
        // Hacer la ventana transparente a clics (dejar pasar clics al juego)
        exStyle = exStyle | WS_EX_TRANSPARENT;
        SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);
        
        // Mantener opacidad completa pero transparente a clics
        SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    }
    
    // Forzar redibujado de la ventana
    RedrawWindow(hwnd, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME);
}


//barra de progreso login

void DrawCircularProgressBar(float progress, float thickness, ImU32 color, const char* label = "Cargando...", ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;


    ImVec2 region_size = ImVec2(250, 250);


    ImVec2 avail = ImGui::GetContentRegionAvail();


    ImVec2 offset = ImVec2((avail.x - region_size.x) * 0.5f, (avail.y - region_size.y) * 1.5f);
    ImGui::SetCursorPos(offset);


    ImGui::InvisibleButton("circle_area", region_size);
    ImVec2 window_pos = ImGui::GetItemRectMin();

    float max_radius = 120.0f;
    float radius = std::min(
        (std::min(region_size.x, region_size.y) - thickness * 2.0f) / 2.0f,
        max_radius
    );


    ImVec2 center = ImVec2(window_pos.x + region_size.x * 0.5f,
        window_pos.y + region_size.y * 0.5f);

    const int num_segments = 100;
    float angle_start = -IM_PI / 2.0f;
    float angle_end = angle_start + (2.0f * IM_PI) * std::clamp(progress, 0.0f, 1.0f);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->PathClear();

    for (int i = 0; i <= num_segments; ++i)
    {
        float a = angle_start + ((float)i / (float)num_segments) * (angle_end - angle_start);
        draw_list->PathLineTo(ImVec2(center.x + cosf(a) * radius, center.y + sinf(a) * radius));
    }

    draw_list->PathStroke(color, false, thickness);


    ImVec2 text_size = ImGui::CalcTextSize(label);
    ImVec2 text_pos = ImVec2(center.x - text_size.x * 0.5f, center.y - text_size.y * 0.5f);

    draw_list->AddText(text_pos, ImGui::GetColorU32(textColor), label);
}



void SetupImGuiStyle()
{
    // Modern style by LousyBook-01 from ImThemes
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.3f;
    style.WindowPadding = ImVec2(10.1f, 10.1f);
    style.WindowRounding = 10.3f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(20.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.ChildRounding = 8.2f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 10.7f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(20.0f, 1.5f);
    style.FrameRounding = 4.8f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(9.7f, 5.3f);
    style.ItemInnerSpacing = ImVec2(5.4f, 9.3f);
    style.CellPadding = ImVec2(7.9f, 2.0f);
    style.IndentSpacing = 10.7f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 12.1f;
    style.ScrollbarRounding = 20.0f;
    style.GrabMinSize = 10.0f;
    style.GrabRounding = 4.6f;
    style.TabRounding = 4.0f;
    style.TabBorderSize = 0.0f;
   
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(1.0f, 1.0f, 1.0f, 0.3991416f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.039215688f, 0.039215688f, 0.039215688f, 0.94f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(0.050980393f, 0.050980393f, 0.050980393f, 0.94f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.42745098f, 0.42745098f, 0.49803922f, 0.5f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.42060083f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.14117648f, 0.14117648f, 0.14117648f, 0.4f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.23137255f, 0.23137255f, 0.23137255f, 0.86266094f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.09411765f, 0.09411765f, 0.09411765f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.2918455f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.13725491f, 0.13725491f, 0.13725491f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.019607844f, 0.019607844f, 0.019607844f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.30980393f, 0.30980393f, 0.30980393f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40784314f, 0.40784314f, 0.40784314f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.50980395f, 0.50980395f, 0.50980395f, 1.0f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.98039216f, 0.25882354f, 0.25882354f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25882354f, 0.5882353f, 0.98039216f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.0f, 0.0f, 0.0f, 0.5793991f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.09803922f, 0.09803922f, 0.09803922f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.23137255f, 0.23137255f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.0f, 0.0f, 0.0f, 0.4549356f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.18039216f, 0.18039216f, 0.18039216f, 0.8f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.9764706f, 0.25882354f, 0.25882354f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803922f, 0.4f, 0.7490196f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803922f, 0.4f, 0.7490196f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.2f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25882354f, 0.5882353f, 0.9764706f, 0.95f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.105882354f, 0.105882354f, 0.105882354f, 1.0f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(1.0f, 0.3647059f, 0.6745098f, 0.8f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(1.0f, 0.22352941f, 0.22352941f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.10980392f, 0.16862746f, 0.23921569f, 0.9724f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.13333334f, 0.25882354f, 0.42352942f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.42745098f, 0.34901962f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.21568628f, 0.21568628f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.21568628f, 0.69803923f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(1.0f, 0.23529412f, 0.23529412f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0f, 0.31764707f, 0.31764707f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0f, 0.5647059f, 0.5647059f, 0.36909872f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.7254902f, 0.3372549f, 1.0f, 0.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 0.27450982f, 0.27450982f, 0.111588f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9764706f, 0.25882354f, 0.25882354f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.9f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.0f, 0.0f, 0.0f, 0.64377683f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.46781117f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.73390555f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.7982833f);
}

// Main code
int maindll()
{

    //KeyAuthApp.init();
    // Make process DPI aware and obtain main monitor scale
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);


    WNDCLASSEXW wc = { sizeof(WNDCLASSEXW) };
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
    wc.lpszMenuName = nullptr;
    wc.lpszClassName = L"uwu";
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassExW(&wc);

    hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TOOLWINDOW,
        wc.lpszClassName, nullptr,
        WS_POPUP,
        0, 0, width, height,
        nullptr, nullptr, wc.hInstance, nullptr
    );

    SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);

    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }


    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
    style.FontScaleDpi = main_scale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);



    //stilos para el panel iconos y imagines

  
    // AGREGAR FUENTES
    Inter_S = io.Fonts->AddFontFromMemoryTTF(&Inter_Semmi, sizeof Inter_Semmi, 14.5f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    font::fuentezada = io.Fonts->AddFontFromMemoryTTF(&Inter_Semmi, sizeof(Inter_Semmi), 10.5f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Inter_S_1 = io.Fonts->AddFontFromMemoryTTF(&Inter_Semmi, sizeof Inter_Semmi, 16.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Icon_Arrow = io.Fonts->AddFontFromMemoryTTF(&Arrow, sizeof Arrow, 6.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    ImFont* Nev1 = io.Fonts->AddFontFromMemoryTTF(&Nevan, sizeof Nevan, 25, NULL, io.Fonts->GetGlyphRangesCyrillic());





    if (image2::main == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, main_icon, sizeof(main_icon), &info, pump, &image2::main, 0);
    if (image2::gless == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, glass_icon, sizeof(glass_icon), &info, pump, &image2::gless, 0);
    if (image2::help == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, help_icon, sizeof(help_icon), &info, pump, &image2::help, 0);
    if (image2::settings == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, settings_icon, sizeof(settings_icon), &info, pump, &image2::settings, 0);
    if (image2::panel == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, world_icon, sizeof(world_icon), &info, pump, &image2::panel, 0);
    if (image2::other == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, other_icon, sizeof(other_icon), &info, pump, &image2::other, 0);
    if (image2::save == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, save_icon, sizeof(save_icon), &info, pump, &image2::save, 0);
    if (image2::login111 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logintaboxcy666666, sizeof(logintaboxcy666666), &info, pump, &image2::login111, 0);
    if (image2::Register111 == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, registertaboxcy666666, sizeof(registertaboxcy666666), &info, pump, &image2::Register111, 0);
    if (logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logotype2, sizeof(logotype2), &info, pump, &logo, 0);
    if (fu1l == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, net2, sizeof(net2), &info, pump, &fu1l, 0);
    if (image2::rayo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, rayoo, sizeof(rayoo), &info, pump, &image2::rayo, 0);
    if (image2::flecha == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, flechaa, sizeof(flechaa), &info, pump, &image2::flecha, 0);
    if (image2::ancla == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, anclaa, sizeof(anclaa), &info, pump, &image2::ancla, 0);
    if (image2::fastfire == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, fastfiree, sizeof(fastfiree), &info, pump, &image2::fastfire, 0);
    if (image2::tpenemy == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, tpenemyy, sizeof(tpenemyy), &info, pump, &image2::tpenemy, 0);
    if (image2::nieve == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, nievee, sizeof(nievee), &info, pump, &image2::nieve, 0);
    if (image2::ghost == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, ghosst, sizeof(ghosst), &info, pump, &image2::ghost, 0);
    if (image2::ffimagen == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, ffimagen, sizeof(ffimagen), &info, pump, &image2::ffimagen, 0);
    if (image2::ffimagenMAX == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, ffimagenMAX, sizeof(ffimagenMAX), &info, pump, &image2::ffimagenMAX, 0);
    if (image2::ffimagenTELA == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, ffimagenTELA, sizeof(ffimagenTELA), &info, pump, &image2::ffimagenTELA, 0);




    //lamar el boton togle antes del while loop
    ToggleClickability(isClickable);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.f, 0.f, 0.f, 0.f);

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }



        // aqui llamamos el keybi tecla

        static int KeyHide = VK_INSERT;
        if (GetAsyncKeyState(KeyHide) & 1)
        {
            // Toggle basado en el estado actual de Main_Window para evitar desincronización
            // Forzar sincronización completa para asegurar que el panel siempre funcione
            // INDEPENDIENTE del estado de la partida
            if (!Main_Window)
            {
                // Mostrar panel
                Main_Window = true;
                Verified = true;
                isClickable = true;
                ToggleClickability(true);
            }
            else
            {
                // Ocultar panel
                Main_Window = false;
                Verified = false;
                isClickable = false;
                ToggleClickability(false);
            }


        }

        // Tecla DELETE para cerrar panel y matar emulador
        static int KeyKill = VK_DELETE;
        if (GetAsyncKeyState(KeyKill) & 1)
        {
            // Cerrar panel
            Main_Window = false;
            Verified = false;
            isClickable = false;
            ToggleClickability(isClickable);
            EnabledEsp = false;

            // Matar proceso del emulador si existe
            if (hdPlayerWindow != NULL)
            {
                DWORD processId = 0;
                GetWindowThreadProcessId(hdPlayerWindow, &processId);
                if (processId != 0)
                {
                    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processId);
                    if (hProcess != NULL)
                    {
                        TerminateProcess(hProcess, 0);
                        CloseHandle(hProcess);
                    }
                }
                CloseHandle(hdPlayerWindow);
                hdPlayerWindow = NULL;
            }

            // Salir de la aplicación
            done = true;
            PostMessage(hwnd, WM_QUIT, 0, 0);
        }



        //// Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (EnabledEsp)
        {

            ESP_line();
        }

        // HUD de Keybinds siempre activo (incluso en lobby)
        DrawKeybindsHUD();
        
        // FOV siempre activo (incluso en lobby)
        DrawFOV();

        SetupImGuiStyle();
        ImGuiContext& g = *GImGui;
        ImGuiStyle* style = &ImGui::GetStyle();

        if (Main_Window)
        {

            if (show_login && !loading)
            {
                
                ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
                if (ImGui::Begin("login", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
                {

                    if (login)
                    {

                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                        ImGui::SetCursorPos(ImVec2(250, 150));
                        ImGui::BeginChild("ta1", ImVec2(420, 300), false);
                        {
                           
                            ImGui::InputText("usuario", datos.Username, sizeof(datos.Username));
                            ImGui::InputText("password", datos.Password, sizeof(datos.Password));
                            ImGui::SetCursorPos(ImVec2(60, 70));
                            if(ImGui::Button("Login", ImVec2(150, 30)))
                            {
                               
                             
                                //if (!KeyAuthApp.login(datos.Username, datos.Password))
                                //{

                                //    MessageBoxA(NULL, "error", NULL, NULL);
                                //}
                                //else {



                                //}
                                  

                                loading = true;
                                progress1 = 0.0f;
                               
                               
                            }

                            ImGui::SetCursorPos(ImVec2(60, 105));
                            if (ImGui::Button("Registrate", ImVec2(150, 30)))
                            {
                                registrard = true;
                                login = false;
                            }

                        }ImGui::EndChild();
                        ImGui::PopStyleColor();

                       

                    }


                    if(registrard)
                    {


                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                        ImGui::SetCursorPos(ImVec2(250, 150));
                        ImGui::BeginChild("ta1", ImVec2(420, 300), false);
                        {
                           
                            ImGui::InputText("usuario", datos.Username, sizeof(datos.Username));
                            ImGui::InputText("password", datos.Password, sizeof(datos.Password));
                            ImGui::InputText("licencia", datos.Key, sizeof(datos.Key));

                            ImGui::SetCursorPos(ImVec2(60, 90));
                            if (ImGui::Button("Registrar", ImVec2(150, 30)))
                            {

                                //if (!KeyAuthApp.regstr(datos.Username, datos.Password,datos.Key))
                                //{

                                //    MessageBoxA(NULL, "error", NULL, NULL);
                                //}
                                //else {


                                //}

                                registrard = false;
                                login = true;
                            }

                          
                        }ImGui::EndChild();
                        ImGui::PopStyleColor();


                    }



                }ImGui::End();

            }


            else if (loading && !authenticed)
            {
                // Renderizado ImGui
                if (ImGui::Begin("login", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
                {
                    ImGui::SetCursorPos(ImVec2(0, 160));
                    ImVec4 textoColor = ImVec4(0.3f, 0.6f, 1.0f, 1.0f);

                    DrawCircularProgressBar(progress1, 6.0f, ImGui::GetColorU32(c::main_coCQlor1), "Cargando...", textoColor);


                    ImGui::Dummy(ImVec2(0, 0));

                    progress1 += load_speed * ImGui::GetIO().DeltaTime;
                    if (progress1 >= 1.0f)
                    {
                        loading = false;
                        authenticed = true;
                        show_login = false;

                        // Activar ADB automáticamente
                        initOffsets(0);
                        std::thread([]() {
                            hdPlayerWindow = INJECTESPADB();
                            while (EspADB != "The Abd is active!");
                            EnabledEsp = true;
                            }).detach();
                    }


                }
                ImGui::End();

            }



            if (authenticed)
            {

                


                ImGui::SetNextWindowSize(ImVec2(1200, 650), ImGuiCond_Always);
                // Hacer el panel transparente para ver la ventana de atrás
                ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.7f)); // Alpha 0.7 = 70% de opacidad (30% transparente)
                if (ImGui::Begin("panel principal", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
                {

                    auto draw = ImGui::GetWindowDrawList();
                    auto background_list1 = ImGui::GetBackgroundDrawList();
                    ImVec2 pos = ImGui::GetWindowPos();
                    ImVec2 size = ImGui::GetWindowSize();
                    float splitX = size.x * 0.09f;
                    float WIDTH3 = 1200;
                    float HEIGHT3 = 650;
                    ImVec2 top_left1 = pos;
                    ImVec2 bottom_right1 = ImVec2(pos.x + WIDTH3, pos.y + HEIGHT3);
                    float rounding13313 = c::rounding;


                    AddShadowRect(background_list1, top_left1, bottom_right1, ImColor(color_edit4.x, color_edit4.y, color_edit4.z, color_edit4.w), 0.f, ImVec2(0, 0), rounding13313);


                    draw->AddRectFilled(pos, ImVec2(pos.x + splitX + 1.0f, pos.y + size.y), ImColor(15, 15, 20, 245), 8, ImDrawFlags_RoundCornersLeft);
                    draw->AddRectFilled(ImVec2(pos.x + splitX, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), ImColor(20, 20, 25, 255), 8, ImDrawFlags_RoundCornersRight);
                    
                    // Borde sutil y elegante
                    ImGui::GetBackgroundDrawList()->AddRect(pos, pos + ImVec2(1200, 650), ImColor(60, 60, 80, 100), 8, 0, 1.0f);
                    
                    // Línea separadora vertical elegante
                    draw->AddLine(
                        ImVec2(pos.x + splitX, pos.y + 20),
                        ImVec2(pos.x + splitX, pos.y + size.y - 20),
                        ImColor(80, 80, 100, 150),
                        1.0f
                    );

                    // Header profesional sin logo
                    ImGui::PushFont(Inter_S_1);
                    const char* header_text = "CHEAT PANEL";
                    ImVec2 header_size = ImGui::CalcTextSize(header_text);
                    ImVec2 header_pos = ImVec2(
                        pos.x + (splitX - header_size.x) * 0.5f,
                        pos.y + 35.0f
                    );
                    // Texto blanco brillante para máxima visibilidad
                    draw->AddText(header_pos, IM_COL32(255, 255, 255, 255), header_text);
                    ImGui::PopFont();


                    tab_alpha = ImLerp(tab_alpha, (page == active_tab) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
                    if (tab_alpha < 0.01f && tab_add < 0.01f)
                        active_tab = page;

                    tab_alpha_1 = ImLerp(tab_alpha_1, (SkinsPage == active_tab_1) ? 1.f : 0.f, 22.f * ImGui::GetIO().DeltaTime);
                    if (tab_alpha_1 < 0.01f && tab_add_1 < 0.01f)
                        active_tab_1 = SkinsPage;

                    tab_alpha_2 = ImLerp(tab_alpha_2, (page_switch == active_tab_2) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
                    if (tab_alpha_2 < 0.01f && tab_add_2 < 0.01f)
                        active_tab_2 = page_switch;

                    const auto& p = ImGui::GetWindowPos();
                    const ImVec2& region = ImGui::GetContentRegionMax();

                    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 80, p.y + region.y), ImGui::GetColorU32(c::child_rect), 12.f, ImDrawFlags_RoundCornersLeft);

                    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 80, p.y + region.y), ImGui::GetColorU32(c::child_rect), 12.f, ImDrawFlags_RoundCornersLeft);



                    /////////////////////////////////////////////////////////////////////////////////////////



                    //tab
                    ImGui::BeginGroup();
                    {
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 35));
                        ImGui::SetCursorPos(ImVec2(16, 100));
                        ImGui::BeginGroup();
                        {
                            if (ImGui::Tab("mains", (ImTextureID)image2::main, 0 == page, ImVec2(30, 30), ImVec2(17, 17))) page = 0;
                           /* if (ImGui::Tab("glass", (ImTextureID)image2::save, 1 == page, ImVec2(30, 30), ImVec2(17, 17))) page = 1;*/
                            if (ImGui::Tab("help", (ImTextureID)image2::gless, 2 == page, ImVec2(30, 30), ImVec2(17, 13))) page = 2;
                            /*if (ImGui::Tab("help2", (ImTextureID)image2::rayo, 3 == page, ImVec2(30, 30), ImVec2(17, 17))) page = 3;*/
                           /* if (ImGui::Tab("help3", (ImTextureID)image2::nieve, 4 == page, ImVec2(30, 30), ImVec2(17, 17))) page = 4;*/
                           /* if (ImGui::Tab("help4", (ImTextureID)image2::panel, 5 == page, ImVec2(30, 30), ImVec2(17, 17))) page = 5;*/
                            if (ImGui::Tab("settings", (ImTextureID)image2::settings, 6 == page, ImVec2(30, 30), ImVec2(17, 17))) page = 6;
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleVar();
                    }
                    ImGui::EndGroup();

                    auto DrawGroupBox = [&](const char* title, ImVec2 pos, ImVec2 size)
                        {
                            ImDrawList* draw = ImGui::GetWindowDrawList();
                            ImVec2 windowPos = ImGui::GetWindowPos();

                            ImVec2 p1 = ImVec2(windowPos.x + pos.x, windowPos.y + pos.y);
                            ImVec2 p2 = ImVec2(p1.x + size.x, p1.y + size.y);

                            // Fondo oscuro suave y estilizado
                            draw->AddRectFilled(p1, p2, IM_COL32(15, 15, 20, 230), 8.f);

                            // Borde sutil brillante
                            draw->AddRect(p1, p2, IM_COL32(60, 60, 80, 150), 8.f, 0, 1.0f);

                            ImVec2 titleP1 = ImVec2(p1.x, p1.y);
                            ImVec2 titleP2 = ImVec2(p2.x, p1.y + 35.f);
                            // Header del grupo
                            draw->AddRectFilled(titleP1, titleP2, IM_COL32(25, 25, 35, 255), 8.f, ImDrawFlags_RoundCornersTop);

                            // Línea separadora elegante (acento azul)
                            draw->AddLine(
                                ImVec2(p1.x, p1.y + 35.f),
                                ImVec2(p2.x, p1.y + 35.f),
                                IM_COL32(40, 120, 255, 180), 1.5f
                            );

                            // Texto del título más visible y brillante
                            draw->AddText(
                                ImVec2(p1.x + 15.f, p1.y + 10.f),
                                IM_COL32(255, 255, 255, 255),
                                title
                            );
                        };

                    auto BeginGroupBox = [&](const char* title, const char* id, ImVec2 pos, ImVec2 size)
                        {
                            DrawGroupBox(title, pos, size);  // dibuja el marco

                            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
                            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
                            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
                            ImGui::SetCursorPos(ImVec2(pos.x + 15, pos.y + 40));
                            ImGui::BeginChild(id, ImVec2(size.x - 30, size.y - 50), false);
                        };

                    auto EndGroupBox = [&]()
                        {
                            ImGui::EndChild();
                            ImGui::PopStyleVar();
                            ImGui::PopStyleColor(2);
                        };


                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha* style->Alpha);
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 12));
                    {

                        if (active_tab == 0)
                        {
                            BeginGroupBox("AIMBOT", "tab1", ImVec2(180, 99), ImVec2(480, 280));
                            {
                                ImGui::SetCursorPosX(10);

                                ImGui::Checkbox2("AIMBOT RED", &AimbotRotation);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Aimbot Silent", &aimSilentEnabled);
                                ImGui::SetCursorPosX(10);
                                ImGui::Combo("Bone Aim", &aimbot_bone_combo, aimbot_bone_list, IM_ARRAYSIZE(aimbot_bone_list));
                                ImGui::Checkbox2("Aim Fov", &ShowFov);
                                ImGui::Checkbox2("Info Box", &ESPInfoBox);
                                ImGui::Checkbox2("NoRecoil", &NoRecoil);
                                ImGui::Checkbox2("Pull Player", &PullPlayer);
                                if (PullPlayer) {
                                    ImGui::SetCursorPosX(10);
                                    ImGui::Combo("Pull Bone", &SelectedPullBone, PullBoneOptions, IM_ARRAYSIZE(PullBoneOptions));
                                }
                              /*  ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("IgnoreKnocked", &IgnoreKnocked);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Aim Fov", &ShowFov);

                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Fast Reload", &FastReload);*/


                            }EndGroupBox();


                            BeginGroupBox("Keybind", "tab3", ImVec2(680, 99), ImVec2(480, 280));
                            {

                                ImGui::SetCursorPosX(10);
                                ImGui::Combo("Bones", &SelectedPullBone, PullBoneOptions, IM_ARRAYSIZE(PullBoneOptions), 180);


                            }
                            EndGroupBox();



                            BeginGroupBox("Extras", "tab2", ImVec2(180, 400), ImVec2(980, 210));
                            {


                                ImGui::SetCursorPosX(10);
                                ImGui::SliderInt("Fov", &FovAll, 0, 1000, "%d");
                                /* ImGui::SliderFloat("Aim fov", &fov, 0.0f, 1000.0f, "%.0f");*/
                                ImGui::SetCursorPosX(10);
                                ImGui::SliderInt("Tiros al Cuello", &neckShotsCount, 0, 5, "%d");
                               /* ImGui::SetCursorPosX(10);
                                ImGui::SliderInt("Velocida Silent", &rojo, 0, 1000, "%d ms");
                                ImGui::SetCursorPosX(10);
                                ImGui::SliderInt("Smoothness", &g_silentSmooth, 0, 50, "%d ms");*/
                                ImGui::SetCursorPosX(10);
                                ImGui::Separator();

                                ImGui::Dummy(ImVec2(0, 8)); // ← espacio al final
                            }
                            EndGroupBox();

                        }

                        if (active_tab == 2)
                        {
                            BeginGroupBox("Esp line", "tab1", ImVec2(180, 99), ImVec2(480, 280));
                            {

                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("ESP LINE v2", &EspLineZ);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Box", &ESPBox);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Fill Box", &ESPFillBox);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Name", &ESPNameZ);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Skeleton", &ESPBones);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Health", &ESPHealth);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp HealthMAX", &ESPHealthMAX);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Weapon Icon", &EspWeaponsICON);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Weapon Text", &EspWeaponsTex);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Wukong", &ShowInvis);
                                ImGui::SetCursorPosX(10);
                                ImGui::Separator();
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("ESP Line Center", &ESPLineCenter);
                                if (ImGui::IsItemClicked()) {
                                    EspLineZ = false;
                                    ESPLineBottom = false;
                                }
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("ESP Line Bottom", &ESPLineBottom);
                                if (ImGui::IsItemClicked()) {
                                    EspLineZ = false;
                                    ESPLineCenter = false;
                                }
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("ESP Distance", &ESPDisctanc);
                                ImGui::SetCursorPosX(10);
                                ImGui::Separator();
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Box 3D", &EspBox3D);
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Esp Radar 2D", &EspRadar2D);
                                ImGui::SetCursorPosX(10);
                                ImGui::Separator();
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("ESP Crosshair", &ESPCrosshair);
                                if (ESPCrosshair) {
                                    ImGui::SetCursorPosX(10);
                                    ImGui::Combo("Crosshair Style", &ESPCrosshairStyle, "Cross\0Circle\0Dot\0");
                                    ImGui::SetCursorPosX(10);
                                    ImGui::SliderFloat("Crosshair Size", &ESPCrosshairSize, 5.0f, 30.0f, "%.1f");
                                    ImGui::SetCursorPosX(10);
                                    ImGui::ColorEdit4("Crosshair Color", (float*)&ESPCrosshairColor);
                                }

                            }EndGroupBox();

                            BeginGroupBox("Chams", "tab3", ImVec2(680, 99), ImVec2(480, 280));
                            {

                           
                            



                            }
                            EndGroupBox();


                            BeginGroupBox("colores", "tab2", ImVec2(180, 400), ImVec2(980, 210));
                            {

                                ImGui::ColorEdit4("panel", (float*)&color_edit4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                              /*  ImGui::ColorEdit4("Esp Box", (float*)&boxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                                ImGui::ColorEdit4("EspFillBox", (float*)&fillBoxColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                                ImGui::ColorEdit4("Esp Name", (float*)&namecolor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);

                                ImGui::ColorEdit4("Esp Skeleton", (float*)&bonesColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                                ImGui::ColorEdit4("Esp Health", (float*)&healthColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                                ImGui::ColorEdit4("Wukong & Orion", (float*)&invisibleColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);*/
                                ImGui::Dummy(ImVec2(0, 8));
                            }
                            EndGroupBox();

                        }
                        if (active_tab == 6)
                        {

                            BeginGroupBox("Settings", "tab1", ImVec2(180, 99), ImVec2(480, 280));
                            {
                               /* ImGui::Keybind("Ocultar panel", "", &UpCheckx);
                                ImGui::Checkbox2("Activar particulas", &aimbotpro1);*/
                                /*ImGui::Checkbox("Mode Streamer", &aimbotpro1);*/
                            }
                            EndGroupBox();

                            BeginGroupBox("Config", "tab3", ImVec2(680, 99), ImVec2(480, 280));
                            {


                            }
                            EndGroupBox();

                            BeginGroupBox("colores del panel", "tab2", ImVec2(180, 400), ImVec2(980, 210));
                            {

                                ImGui::ColorEdit4("Color Barra Menu", (float*)&color_edit4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                                /*ImGui::ColorEdit4("Color Boton Activado", (float*)&c::black, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                                ImGui::ColorEdit4("Color Encabezado Arriba", (float*)&c::main_color2, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);*/
                                ImGui::Dummy(ImVec2(0, 8)); // ← espacio al final
                            }
                            EndGroupBox();




                        }

                      


                    }ImGui::PopStyleVar(2);




                }ImGui::End();
                ImGui::PopStyleColor(); // Restaurar el color de fondo original

            }

           
        }


        

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { 0.f, 0.f, 0.f, 0.f };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

//llamarlo como dll o exe

bool IsProcessAdmin()
{
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(
        &NtAuthority,
        2,
        SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS,
        0, 0, 0, 0, 0, 0,
        &adminGroup))
    {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }

    return isAdmin == TRUE;
}

#ifdef _WIN64 
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hinstDLL);


        if (!IsProcessAdmin())
        {
            MessageBoxA(
                NULL,
                "ERROR:\n\nEl emulador NO se esta ejecutando como ADMINISTRADOR.\n\n"
                "Cierra el emulador y vuelvelo a ejecutar como administrador.", "",
                MB_ICONERROR | MB_OK
            );

            // ❌ Cancela la carga de la DLL
            return FALSE;
        }


        hThread1 = (HANDLE)_beginthreadex(
            nullptr,
            0,
            (_beginthreadex_proc_type)maindll,
            nullptr,
            0,
            nullptr
        );



        if (!hThread1)
            return FALSE;

        break;
    }

    case DLL_PROCESS_DETACH:
    {
        if (hCurrentUIThread) {
            WaitForSingleObject(hCurrentUIThread, INFINITE);
            CloseHandle(hCurrentUIThread);
        }

        if (hThread1) {
            WaitForSingleObject(hThread1, INFINITE);
            CloseHandle(hThread1);
        }

        break;
    }
    }
    return TRUE;
}
#endif


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    maindll();
    return 0;
}


// Helper functions



bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // <-- cambiado
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0 };

    HRESULT res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED)
        res = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_WARP, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    // Transparencia DWM
    MARGINS margins = { -1, -1, -1, -1 };
    DwmExtendFrameIntoClientArea(hWnd, &margins);

    DWM_BLURBEHIND bb = { 0 };
    bb.dwFlags = DWM_BB_ENABLE;
    bb.fEnable = TRUE;
    bb.hRgnBlur = NULL;
    DwmEnableBlurBehindWindow(hWnd, &bb);

    CreateRenderTarget();
    return true;
}


void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}