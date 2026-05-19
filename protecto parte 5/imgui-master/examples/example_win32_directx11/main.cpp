#include "main.h"
#include <ctime>
#include "Esp/Weapons.h"


using namespace KeyAuth;

inline std::string name = skCrypt("Slaughtherhouse").decrypt(); 
inline std::string ownerid = skCrypt("o8bTUhtbOZ").decrypt(); 
inline std::string version = skCrypt("1.0").decrypt(); 
inline std::string url = skCrypt("https://keyauth.win/api/1.3/").decrypt();
inline std::string path = skCrypt("").decrypt();

inline KeyAuth::api KeyAuthApp(name, ownerid, version, url);

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
    exStyle = clickable ? (exStyle & ~WS_EX_TRANSPARENT) : (exStyle | WS_EX_TRANSPARENT);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle);


    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    RedrawWindow(hwnd, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME);
}


//barra de progreso login

void DrawCircularProgressBar(float progress, float thickness, ImU32 color, const char* label = "Cargando...", ImVec4 textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
{
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImVec2 region_size = ImVec2(300, 300);
    ImVec2 avail = ImGui::GetContentRegionAvail();
    ImVec2 offset = ImVec2((avail.x - region_size.x) * 0.5f, (avail.y - region_size.y) * 1.5f);
    ImGui::SetCursorPos(offset);

    ImGui::InvisibleButton("circle_area", region_size);
    ImVec2 window_pos = ImGui::GetItemRectMin();

    ImVec2 center = ImVec2(window_pos.x + region_size.x * 0.5f, window_pos.y + region_size.y * 0.5f);
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    float time = (float)ImGui::GetTime();
    
    // 1. Efecto de Brillo/Glow del Fondo
    float pulse = (sinf(time * 3.0f) * 0.5f + 0.5f);
    ImU32 glowBg = ImGui::GetColorU32(ImVec4(0.0f, 0.6f, 1.0f, 0.03f + pulse * 0.02f));
    draw_list->AddCircleFilled(center, 90.0f, glowBg, 64);
    
    // 2. Anillo de progreso principal (Neon Glow)
    float base_radius = 85.0f;
    float progress_clamped = (progress < 0.0f) ? 0.0f : ((progress > 1.0f) ? 1.0f : progress);
    float angle_start = -IM_PI / 2.0f;
    float angle_end = angle_start + (2.0f * IM_PI) * progress_clamped;
    
    if (progress_clamped > 0.01f) {
        int segments = (int)(100 * progress_clamped);
        if (segments < 3) segments = 3;
        
        // Capa 1: Glow exterior grueso
        draw_list->PathClear();
        for (int i = 0; i <= segments; ++i) {
            float a = angle_start + ((float)i / (float)segments) * (angle_end - angle_start);
            draw_list->PathLineTo(ImVec2(center.x + cosf(a) * base_radius, center.y + sinf(a) * base_radius));
        }
        draw_list->PathStroke(ImGui::GetColorU32(ImVec4(0.0f, 0.8f, 1.0f, 0.15f)), false, 8.0f);
        
        // Capa 2: Glow medio
        draw_list->PathClear();
        for (int i = 0; i <= segments; ++i) {
            float a = angle_start + ((float)i / (float)segments) * (angle_end - angle_start);
            draw_list->PathLineTo(ImVec2(center.x + cosf(a) * base_radius, center.y + sinf(a) * base_radius));
        }
        draw_list->PathStroke(ImGui::GetColorU32(ImVec4(0.0f, 0.9f, 1.0f, 0.35f)), false, 4.0f);
        
        // Capa 3: Línea central brillante (núcleo blanco/cyan)
        draw_list->PathClear();
        for (int i = 0; i <= segments; ++i) {
            float a = angle_start + ((float)i / (float)segments) * (angle_end - angle_start);
            draw_list->PathLineTo(ImVec2(center.x + cosf(a) * base_radius, center.y + sinf(a) * base_radius));
        }
        draw_list->PathStroke(ImGui::GetColorU32(ImVec4(0.9f, 1.0f, 1.0f, 1.0f)), false, 1.5f);
    }
    
    // 3. Anillos orbitales giratorios decorativos (Estilo futurista/hacker)
    float outer_radius = 100.0f;
    float rot_speed = time * 1.5f;
    int num_dashes = 12;
    float dash_len = (2.0f * IM_PI) / (float)num_dashes;
    for (int i = 0; i < num_dashes; ++i) {
        float start_a = rot_speed + i * dash_len;
        float end_a = start_a + dash_len * 0.4f;
        
        draw_list->PathClear();
        int dash_segments = 5;
        for (int j = 0; j <= dash_segments; ++j) {
            float a = start_a + ((float)j / (float)dash_segments) * (end_a - start_a);
            draw_list->PathLineTo(ImVec2(center.x + cosf(a) * outer_radius, center.y + sinf(a) * outer_radius));
        }
        draw_list->PathStroke(ImGui::GetColorU32(ImVec4(0.0f, 0.7f, 1.0f, 0.4f)), false, 1.5f);
    }
    
    float inner_radius = 70.0f;
    float rot_speed_reverse = -time * 2.2f;
    int inner_dashes = 8;
    float inner_dash_len = (2.0f * IM_PI) / (float)inner_dashes;
    for (int i = 0; i < inner_dashes; ++i) {
        float start_a = rot_speed_reverse + i * inner_dash_len;
        float end_a = start_a + inner_dash_len * 0.25f;
        
        draw_list->PathClear();
        int dash_segments = 4;
        for (int j = 0; j <= dash_segments; ++j) {
            float a = start_a + ((float)j / (float)dash_segments) * (end_a - start_a);
            draw_list->PathLineTo(ImVec2(center.x + cosf(a) * inner_radius, center.y + sinf(a) * inner_radius));
        }
        draw_list->PathStroke(ImGui::GetColorU32(ImVec4(0.0f, 0.8f, 1.0f, 0.25f)), false, 1.0f);
    }
    
    // 4. Pequeños puntos indicadores cardinales estáticos (como una brújula de puntería)
    ImU32 dot_color = ImGui::GetColorU32(ImVec4(0.0f, 0.8f, 1.0f, 0.3f));
    draw_list->AddCircleFilled(ImVec2(center.x, center.y - outer_radius), 2.0f, dot_color);
    draw_list->AddCircleFilled(ImVec2(center.x, center.y + outer_radius), 2.0f, dot_color);
    draw_list->AddCircleFilled(ImVec2(center.x - outer_radius, center.y), 2.0f, dot_color);
    draw_list->AddCircleFilled(ImVec2(center.x + outer_radius, center.y), 2.0f, dot_color);

    // 5. Porcentaje de carga
    char pct_buf[16];
    sprintf(pct_buf, "%d%%", (int)(progress_clamped * 100.0f));
    ImVec2 pct_size = ImGui::CalcTextSize(pct_buf);
    draw_list->AddText(ImVec2(center.x - pct_size.x * 0.5f, center.y - pct_size.y * 1.3f), ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, 0.9f)), pct_buf);

    // 6. Texto descriptivo con pulsación de opacidad
    ImVec2 text_size = ImGui::CalcTextSize(label);
    ImVec2 text_pos = ImVec2(center.x - text_size.x * 0.5f, center.y + text_size.y * 0.4f);
    
    float alpha = 0.5f + sinf(time * 4.0f) * 0.3f;
    ImU32 label_color = ImGui::GetColorU32(ImVec4(0.0f, 0.8f, 1.0f, alpha));
    
    draw_list->AddText(text_pos, label_color, label);
}



void SetupImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.4f;
    style.WindowPadding = ImVec2(16.0f, 16.0f);
    style.WindowRounding = 12.0f;
    style.WindowBorderSize = 1.0f;
    style.WindowMinSize = ImVec2(20.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.ChildRounding = 8.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 10.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(16.0f, 6.0f);
    style.FrameRounding = 6.0f;
    style.FrameBorderSize = 1.0f;
    style.ItemSpacing = ImVec2(12.0f, 8.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 8.0f);
    style.IndentSpacing = 20.0f;
    style.ScrollbarSize = 10.0f;
    style.ScrollbarRounding = 10.0f;
    style.GrabMinSize = 12.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 6.0f;
    style.TabBorderSize = 0.0f;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.08f, 0.98f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.09f, 0.09f, 0.12f, 0.60f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.06f, 0.06f, 0.08f, 0.98f);
    colors[ImGuiCol_Border]                 = ImVec4(0.18f, 0.18f, 0.25f, 0.50f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.11f, 0.11f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.40f, 0.15f, 0.95f, 0.40f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.06f, 0.06f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.08f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.11f, 0.11f, 0.15f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.20f, 0.20f, 0.28f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.25f, 0.25f, 0.35f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.45f, 0.20f, 0.98f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.55f, 0.25f, 0.98f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.45f, 0.20f, 0.95f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.55f, 0.25f, 0.98f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.35f, 0.15f, 0.85f, 0.80f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.45f, 0.20f, 0.95f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.55f, 0.25f, 0.98f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.15f, 0.15f, 0.22f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.25f, 0.25f, 0.35f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.40f, 0.15f, 0.95f, 0.80f);
    colors[ImGuiCol_Separator]              = ImVec4(0.18f, 0.18f, 0.25f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.40f, 0.15f, 0.95f, 0.78f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.45f, 0.20f, 0.95f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.40f, 0.15f, 0.95f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.45f, 0.20f, 0.95f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.55f, 0.25f, 0.98f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.11f, 0.11f, 0.15f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.40f, 0.15f, 0.95f, 0.80f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.45f, 0.20f, 0.95f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.15f, 0.15f, 0.20f, 1.00f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.45f, 0.20f, 0.95f, 0.45f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.45f, 0.20f, 0.95f, 0.80f);
}

// Main code
int maindll()
{

    KeyAuthApp.init();
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
    font2::fuentezada = io.Fonts->AddFontFromMemoryTTF(&Inter_Semmi, sizeof(Inter_Semmi), 10.5f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Inter_S_1 = io.Fonts->AddFontFromMemoryTTF(&Inter_Semmi, sizeof Inter_Semmi, 16.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Icon_Arrow = io.Fonts->AddFontFromMemoryTTF(&Arrow, sizeof Arrow, 6.f, NULL, io.Fonts->GetGlyphRangesCyrillic());
    Nev1 = io.Fonts->AddFontFromMemoryTTF(&Nevan, sizeof Nevan, 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
    font2::WeaponsIco = io.Fonts->AddFontFromMemoryTTF(&WIconsZ, sizeof WIconsZ, 20.0f, NULL, io.Fonts->GetGlyphRangesCyrillic());





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



        // VK_INSERT toggles the menu visibility
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            if (!Verified)
            {
                isClickable = true;
                Main_Window = true;
                ToggleClickability(isClickable);
                Verified = true;
            }
            else
            {
                isClickable = false;
                Main_Window = false;
                ToggleClickability(isClickable);
                Verified = false;
            }
        }
        
        // VK_DELETE now exits everything instantly
        if (GetAsyncKeyState(VK_DELETE) & 0x8000)
        {
            ExitProcess(0);
        }



        //// Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        if (EnabledEsp)
        {

            ESP_line();
        }

        SetupImGuiStyle();
        ImGuiContext& g = *GImGui;
        ImGuiStyle* style = &ImGui::GetStyle();

        if (Main_Window)
        {

            if (show_login && !loading)
            {
                // [ignoring loop detection]
                ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

                if (ImGui::Begin("login", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
                {
                    auto draw = ImGui::GetWindowDrawList();
                    ImVec2 pos = ImGui::GetWindowPos();
                    ImVec2 size = ImGui::GetWindowSize();

                    // 1. Fondo principal translúcido y oscuro de la ventana (efecto glassmorphism estilo Panel)
                    ImColor themeBorder = ImColor(0, 200, 255, 150);
                    draw->AddRectFilled(pos, pos + size, ImColor(10, 10, 14, 160), 8.0f);
                    draw->AddRect(pos, pos + size, themeBorder, 6.0f, 0, 1.5f);

                    // 2. Líneas procedimentales de circuito cibernético (Izquierda y Derecha)
                    ImColor circuitColor = ImColor(0, 160, 200, 70);
                    ImColor nodeColor = ImColor(0, 220, 255, 180);

                    // Circuitos de la izquierda
                    draw->AddLine(ImVec2(pos.x + 20, pos.y + 150), ImVec2(pos.x + 90, pos.y + 150), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 90, pos.y + 150), ImVec2(pos.x + 120, pos.y + 180), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 120, pos.y + 180), ImVec2(pos.x + 195, pos.y + 180), circuitColor, 1.5f);
                    draw->AddCircleFilled(ImVec2(pos.x + 195, pos.y + 180), 3.0f, nodeColor);

                    draw->AddLine(ImVec2(pos.x + 20, pos.y + 250), ImVec2(pos.x + 70, pos.y + 250), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 70, pos.y + 250), ImVec2(pos.x + 100, pos.y + 220), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 100, pos.y + 220), ImVec2(pos.x + 190, pos.y + 220), circuitColor, 1.5f);
                    draw->AddCircleFilled(ImVec2(pos.x + 190, pos.y + 220), 3.0f, nodeColor);

                    draw->AddLine(ImVec2(pos.x + 20, pos.y + 350), ImVec2(pos.x + 110, pos.y + 350), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 110, pos.y + 350), ImVec2(pos.x + 135, pos.y + 325), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 135, pos.y + 325), ImVec2(pos.x + 195, pos.y + 325), circuitColor, 1.5f);
                    draw->AddCircleFilled(ImVec2(pos.x + 195, pos.y + 325), 3.0f, nodeColor);

                    // Circuitos de la derecha
                    draw->AddLine(ImVec2(pos.x + 780, pos.y + 150), ImVec2(pos.x + 710, pos.y + 150), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 710, pos.y + 150), ImVec2(pos.x + 680, pos.y + 180), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 680, pos.y + 180), ImVec2(pos.x + 605, pos.y + 180), circuitColor, 1.5f);
                    draw->AddCircleFilled(ImVec2(pos.x + 605, pos.y + 180), 3.0f, nodeColor);

                    draw->AddLine(ImVec2(pos.x + 780, pos.y + 250), ImVec2(pos.x + 730, pos.y + 250), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 730, pos.y + 250), ImVec2(pos.x + 700, pos.y + 220), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 700, pos.y + 220), ImVec2(pos.x + 610, pos.y + 220), circuitColor, 1.5f);
                    draw->AddCircleFilled(ImVec2(pos.x + 610, pos.y + 220), 3.0f, nodeColor);

                    draw->AddLine(ImVec2(pos.x + 780, pos.y + 350), ImVec2(pos.x + 690, pos.y + 350), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 690, pos.y + 350), ImVec2(pos.x + 665, pos.y + 325), circuitColor, 1.5f);
                    draw->AddLine(ImVec2(pos.x + 665, pos.y + 325), ImVec2(pos.x + 605, pos.y + 325), circuitColor, 1.5f);
                    draw->AddCircleFilled(ImVec2(pos.x + 605, pos.y + 325), 3.0f, nodeColor);

                    // 3. Renderizar la Tarjeta de Login (Card Frame)
                    ImVec2 cardMin(pos.x + 210.0f, pos.y + 40.0f);
                    ImVec2 cardMax(pos.x + 590.0f, pos.y + 460.0f);

                    // Brillo desactivado para coincidir con el panel principal Glassmorphism
                    // Fondo interior del Card (translúcido)
                    draw->AddRectFilled(cardMin, cardMax, ImColor(10, 10, 15, 120), 12.0f);
                    // Borde principal del Card cyan
                    draw->AddRect(cardMin, cardMax, ImColor(0, 200, 255, 150), 12.0f, 0, 1.5f);

                    // 4. Dibujar Escudo y Cresta Procedimental superior (Badge)
                    ImVec2 shieldCenter(pos.x + 400.0f, pos.y + 40.0f);

                    // Polígonos para el escudo metálico superior
                    ImVec2 p[5] = {
                        ImVec2(shieldCenter.x - 30, shieldCenter.y - 25),
                        ImVec2(shieldCenter.x + 30, shieldCenter.y - 25),
                        ImVec2(shieldCenter.x + 30, shieldCenter.y + 5),
                        ImVec2(shieldCenter.x, shieldCenter.y + 25),
                        ImVec2(shieldCenter.x - 30, shieldCenter.y + 5)
                    };
                    draw->AddConvexPolyFilled(p, 5, ImColor(28, 31, 38));
                    draw->AddPolyline(p, 5, ImColor(0, 200, 255), true, 2.0f);

                    // Círculo de avatar interior
                    draw->AddCircleFilled(ImVec2(shieldCenter.x, shieldCenter.y - 2), 16.0f, ImColor(18, 20, 24));
                    draw->AddCircle(ImVec2(shieldCenter.x, shieldCenter.y - 2), 16.0f, ImColor(0, 200, 255), 0, 1.5f);

                    // Icono de Usuario (Cabeza y Hombros en cyan)
                    draw->AddCircleFilled(ImVec2(shieldCenter.x, shieldCenter.y - 6), 5.0f, ImColor(0, 220, 255));
                    draw->PathClear();
                    draw->PathArcTo(ImVec2(shieldCenter.x, shieldCenter.y + 11), 8.0f, IM_PI * 1.15f, IM_PI * 1.85f);
                    draw->PathStroke(ImColor(0, 220, 255), false, 2.5f);

                    // Corona/Cresta del dragón arriba del escudo
                    draw->PathClear();
                    draw->PathLineTo(ImVec2(shieldCenter.x - 12, shieldCenter.y - 30));
                    draw->PathLineTo(ImVec2(shieldCenter.x - 6, shieldCenter.y - 38));
                    draw->PathLineTo(ImVec2(shieldCenter.x, shieldCenter.y - 32));
                    draw->PathLineTo(ImVec2(shieldCenter.x + 6, shieldCenter.y - 38));
                    draw->PathLineTo(ImVec2(shieldCenter.x + 12, shieldCenter.y - 30));
                    draw->PathStroke(ImColor(255, 180, 0), false, 2.0f); // Dorado/Amarillo corona

                    if (login)
                    {
                        // 5. Título "INICIAR SESIÓN"
                        ImGui::PushFont(Nev1);
                        std::string titleStr = "INICIAR SESION";
                        ImVec2 titleSize = ImGui::CalcTextSize(titleStr.c_str());
                        draw->AddText(ImVec2(pos.x + 400.0f - titleSize.x * 0.5f, pos.y + 100.0f), ImColor(255, 255, 255), titleStr.c_str());
                        ImGui::PopFont();

                        // Subtítulo
                        ImGui::PushFont(Inter_S);
                        std::string subtitleStr = "Por favor, ingrese sus credenciales para";
                        std::string subtitleStr2 = "continuar.";
                        ImVec2 subSize1 = ImGui::CalcTextSize(subtitleStr.c_str());
                        ImVec2 subSize2 = ImGui::CalcTextSize(subtitleStr2.c_str());
                        draw->AddText(ImVec2(pos.x + 400.0f - subSize1.x * 0.5f, pos.y + 135.0f), ImColor(150, 155, 165), subtitleStr.c_str());
                        draw->AddText(ImVec2(pos.x + 400.0f - subSize2.x * 0.5f, pos.y + 152.0f), ImColor(150, 155, 165), subtitleStr2.c_str());
                        ImGui::PopFont();

                        // 6. Controles de Entrada (Inputs)
                        ImGui::SetCursorPos(ImVec2(240, 185));
                        ImGui::BeginChild("InputsArea", ImVec2(320, 260), false, ImGuiChildFlags_None | ImGuiWindowFlags_NoBackground);
                        {
                            ImGuiStyle& style = ImGui::GetStyle();
                            float oldRounding = style.FrameRounding;
                            ImVec2 oldPadding = style.FramePadding;

                            style.FrameRounding = 6.0f;
                            style.FramePadding = ImVec2(35.0f, 10.0f); // Espacio en la izquierda para el icono procedural

                            // --- INPUT USUARIO (Correo) ---
                            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.09f, 0.11f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.12f, 0.13f, 0.16f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.12f, 0.13f, 0.16f, 1.0f));
                            ImGui::PushItemWidth(320.0f);

                            ImGui::InputTextWithHint("##usuario", "Correo electronico", datos.Username, sizeof(datos.Username));
                            
                            // Dibujar icono de sobre procedural
                            ImVec2 userMin = ImGui::GetItemRectMin();
                            ImVec2 userMax = ImGui::GetItemRectMax();
                            float userMidY = (userMin.y + userMax.y) * 0.5f;

                            // Dibujar rectángulo del sobre
                            ImVec2 envMin(userMin.x + 12.0f, userMidY - 5.0f);
                            ImVec2 envMax(userMin.x + 26.0f, userMidY + 5.0f);
                            draw->AddRect(envMin, envMax, ImColor(0, 200, 220), 1.0f);
                            draw->AddLine(envMin, ImVec2(userMin.x + 19.0f, userMidY), ImColor(0, 200, 220));
                            draw->AddLine(ImVec2(userMin.x + 19.0f, userMidY), ImVec2(envMax.x, envMin.y), ImColor(0, 200, 220));

                            // Borde brillante si está enfocado
                            if (ImGui::IsItemActive())
                            {
                                draw->AddRect(userMin, userMax, ImColor(0, 220, 255), 6.0f, 0, 1.5f);
                            }

                            ImGui::Spacing();
                            ImGui::Spacing();

                            // --- INPUT PASSWORD (Contraseña) ---
                            static bool show_password = false;
                            ImGui::InputTextWithHint("##password", "Contrasena", datos.Password, sizeof(datos.Password), show_password ? 0 : ImGuiInputTextFlags_Password);

                            ImVec2 passMin = ImGui::GetItemRectMin();
                            ImVec2 passMax = ImGui::GetItemRectMax();
                            float passMidY = (passMin.y + passMax.y) * 0.5f;

                            // Dibujar candado procedural
                            ImVec2 lockMin(passMin.x + 13.0f, passMidY + 1.0f);
                            ImVec2 lockMax(passMin.x + 23.0f, passMidY + 7.0f);
                            draw->AddRectFilled(lockMin, lockMax, ImColor(0, 200, 220), 1.0f);
                            // Arco del candado
                            draw->PathClear();
                            draw->PathArcTo(ImVec2(passMin.x + 18.0f, passMidY + 1.0f), 3.0f, IM_PI, 0);
                            draw->PathStroke(ImColor(0, 200, 220), false, 1.2f);

                            // Botón interactivo para alternar visibilidad (icono de ojo a la derecha)
                            ImGui::SetCursorScreenPos(ImVec2(passMax.x - 32.0f, passMin.y));
                            if (ImGui::InvisibleButton("##toggle_pass", ImVec2(32.0f, passMax.y - passMin.y)))
                            {
                                show_password = !show_password;
                            }

                            // Dibujar icono de ojo procedural
                            ImVec2 eyeCenter(passMax.x - 16.0f, passMidY);
                            draw->PathClear();
                            draw->PathArcTo(ImVec2(eyeCenter.x, eyeCenter.y + 3.0f), 5.0f, -IM_PI * 0.75f, -IM_PI * 0.25f);
                            draw->PathStroke(ImColor(0, 200, 220), false, 1.2f);
                            draw->PathClear();
                            draw->PathArcTo(ImVec2(eyeCenter.x, eyeCenter.y - 3.0f), 5.0f, IM_PI * 0.25f, IM_PI * 0.75f);
                            draw->PathStroke(ImColor(0, 200, 220), false, 1.2f);
                            draw->AddCircleFilled(eyeCenter, 2.0f, ImColor(0, 220, 255));
                            if (show_password)
                            {
                                // Raya diagonal roja/cyan si el password es visible
                                draw->AddLine(ImVec2(eyeCenter.x - 5.0f, eyeCenter.y - 3.0f), ImVec2(eyeCenter.x + 5.0f, eyeCenter.y + 3.0f), ImColor(255, 100, 100), 1.5f);
                            }

                            if (ImGui::IsItemActive())
                            {
                                draw->AddRect(passMin, passMax, ImColor(0, 220, 255), 6.0f, 0, 1.5f);
                            }

                            ImGui::PopStyleColor(3);
                            ImGui::PopItemWidth();

                            // Restaurar estilos
                            style.FrameRounding = oldRounding;
                            style.FramePadding = oldPadding;

                            // --- ¿OLVIDÓ SU CONTRASEÑA? ---
                            ImGui::SetCursorPos(ImVec2(180, 108));
                            ImGui::PushFont(font2::fuentezada);
                            bool forgotHovered = false;
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.6f, 0.65f, 0.7f, 1.0f));
                            if (ImGui::Selectable("¿Olvido su contrasena?", &forgotHovered, ImGuiSelectableFlags_DontClosePopups, ImVec2(140, 15)))
                            {
                                MessageBoxA(NULL, "Por favor contacte al soporte de ᶻ̷ ᴴ𝒯𝐸𝒜𝑀.", "Recuperacion", MB_ICONINFORMATION | MB_OK);
                            }
                            ImGui::PopStyleColor();
                            ImGui::PopFont();

                            // --- BOTÓN INICIAR SESIÓN (Borde dorado / neón cepillado) ---
                            ImGui::SetCursorPos(ImVec2(0, 135));
                            ImVec2 btnMin = ImGui::GetCursorScreenPos();
                            ImVec2 btnMax = btnMin + ImVec2(320.0f, 42.0f);

                            bool btnHovered = ImGui::IsMouseHoveringRect(btnMin, btnMax);
                            bool btnClicked = btnHovered && ImGui::IsMouseClicked(0);

                            // Dibujar contorno neón degradado del botón
                            ImColor btnBorder = btnHovered ? ImColor(255, 180, 0) : ImColor(0, 200, 255);
                            draw->AddRectFilled(btnMin, btnMax, btnHovered ? ImColor(12, 45, 60) : ImColor(6, 25, 35), 6.0f);
                            draw->AddRect(btnMin, btnMax, btnBorder, 6.0f, 0, 1.5f);

                            // Detalles angulares dorados del botón al estilo sci-fi de la imagen
                            draw->AddLine(btnMin, btnMin + ImVec2(15.0f, 0), ImColor(255, 180, 0), 2.5f);
                            draw->AddLine(btnMin, btnMin + ImVec2(0, 15.0f), ImColor(255, 180, 0), 2.5f);
                            draw->AddLine(btnMax, btnMax - ImVec2(15.0f, 0), ImColor(255, 180, 0), 2.5f);
                            draw->AddLine(btnMax, btnMax - ImVec2(0, 15.0f), ImColor(255, 180, 0), 2.5f);

                            // Centrar texto del botón
                            ImGui::PushFont(Inter_S_1);
                            ImVec2 btnTextSize = ImGui::CalcTextSize("INICIAR SESION");
                            draw->AddText(btnMin + ImVec2((320.0f - btnTextSize.x) * 0.5f, (42.0f - btnTextSize.y) * 0.5f), ImColor(255, 255, 255), "INICIAR SESION");
                            ImGui::PopFont();

                            // Acción del botón (KeyAuth Integrado)
                            if (btnClicked)
                            {
                                if (KeyAuthApp.login(datos.Username, datos.Password))
                                {
                                    Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Sesion iniciada con exito!", 4.0f);
                                    loading = false;
                                    authenticed = false;
                                    show_login = false;
                                    choose_game = true;
                                }
                                else
                                {
                                    std::string safeError = KeyAuthApp.last_error;
                                    if (safeError.empty()) {
                                        safeError = "Credenciales incorrectas o error de conexion.";
                                    }
                                    Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", safeError, 5.0f);
                                }
                            }

                            // --- REGÍSTRESE AQUÍ LINK ---
                            ImGui::SetCursorPos(ImVec2(0, 205));
                            ImGui::PushFont(Inter_S);
                            std::string noAccountStr = "¿No tiene una cuenta? ";
                            std::string registerStr = "Registrese aqui";
                            ImVec2 sizeNoAccount = ImGui::CalcTextSize(noAccountStr.c_str());
                            ImVec2 sizeRegister = ImGui::CalcTextSize(registerStr.c_str());
                            float startX = (320.0f - (sizeNoAccount.x + sizeRegister.x)) * 0.5f;

                            ImVec2 textPos1 = ImGui::GetCursorScreenPos() + ImVec2(startX, 0);
                            ImVec2 textPos2 = textPos1 + ImVec2(sizeNoAccount.x, 0);

                            draw->AddText(textPos1, ImColor(160, 165, 175), noAccountStr.c_str());

                            ImGui::SetCursorScreenPos(textPos2);
                            bool regHovered = false;
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 1.0f, 1.0f));
                            if (ImGui::Selectable("Registrese aqui", &regHovered, ImGuiSelectableFlags_DontClosePopups, sizeRegister))
                            {
                                registrard = true;
                                login = false;
                            }
                            ImGui::PopStyleColor();

                            // Subrayado interactivo si se pasa el mouse
                            if (regHovered)
                            {
                                draw->AddLine(textPos2 + ImVec2(0, sizeRegister.y + 1.0f), textPos2 + ImVec2(sizeRegister.x, sizeRegister.y + 1.0f), ImColor(0, 200, 255), 1.0f);
                            }

                            ImGui::PopFont();
                        }
                        ImGui::EndChild();
                    }

                    if (registrard)
                    {
                        // 5. Título "REGISTRO"
                        ImGui::PushFont(Nev1);
                        std::string titleStr = "REGISTRO";
                        ImVec2 titleSize = ImGui::CalcTextSize(titleStr.c_str());
                        draw->AddText(ImVec2(pos.x + 400.0f - titleSize.x * 0.5f, pos.y + 90.0f), ImColor(255, 255, 255), titleStr.c_str());
                        ImGui::PopFont();

                        // Subtítulo
                        ImGui::PushFont(Inter_S);
                        std::string subtitleStr = "Ingrese sus datos para activar su licencia.";
                        ImVec2 subSize = ImGui::CalcTextSize(subtitleStr.c_str());
                        draw->AddText(ImVec2(pos.x + 400.0f - subSize.x * 0.5f, pos.y + 122.0f), ImColor(150, 155, 165), subtitleStr.c_str());
                        ImGui::PopFont();

                        // 6. Controles de Entrada (Inputs de Registro)
                        ImGui::SetCursorPos(ImVec2(240, 155));
                        ImGui::BeginChild("InputsAreaReg", ImVec2(320, 290), false, ImGuiChildFlags_None | ImGuiWindowFlags_NoBackground);
                        {
                            ImGuiStyle& style = ImGui::GetStyle();
                            float oldRounding = style.FrameRounding;
                            ImVec2 oldPadding = style.FramePadding;

                            style.FrameRounding = 6.0f;
                            style.FramePadding = ImVec2(35.0f, 8.0f);

                            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.08f, 0.09f, 0.11f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.12f, 0.13f, 0.16f, 1.0f));
                            ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.12f, 0.13f, 0.16f, 1.0f));
                            ImGui::PushItemWidth(320.0f);

                            // --- CORREO ELECTRONICO ---
                            ImGui::InputTextWithHint("##reg_usuario", "Correo electronico", datos.Username, sizeof(datos.Username));
                            ImVec2 uMin = ImGui::GetItemRectMin();
                            ImVec2 uMax = ImGui::GetItemRectMax();
                            float uMidY = (uMin.y + uMax.y) * 0.5f;

                            ImVec2 envMin(uMin.x + 12.0f, uMidY - 5.0f);
                            ImVec2 envMax(uMin.x + 26.0f, uMidY + 5.0f);
                            draw->AddRect(envMin, envMax, ImColor(0, 200, 220), 1.0f);
                            draw->AddLine(envMin, ImVec2(uMin.x + 19.0f, uMidY), ImColor(0, 200, 220));
                            draw->AddLine(ImVec2(uMin.x + 19.0f, uMidY), ImVec2(envMax.x, envMin.y), ImColor(0, 200, 220));

                            if (ImGui::IsItemActive()) draw->AddRect(uMin, uMax, ImColor(0, 220, 255), 6.0f, 0, 1.5f);

                            ImGui::Spacing();

                            // --- CONTRASEÑA ---
                            static bool show_reg_password = false;
                            ImGui::InputTextWithHint("##reg_password", "Contrasena", datos.Password, sizeof(datos.Password), show_reg_password ? 0 : ImGuiInputTextFlags_Password);
                            ImVec2 pMin = ImGui::GetItemRectMin();
                            ImVec2 pMax = ImGui::GetItemRectMax();
                            float pMidY = (pMin.y + pMax.y) * 0.5f;

                            ImVec2 lockMin(pMin.x + 13.0f, pMidY + 1.0f);
                            ImVec2 lockMax(pMin.x + 23.0f, pMidY + 7.0f);
                            draw->AddRectFilled(lockMin, lockMax, ImColor(0, 200, 220), 1.0f);
                            draw->PathClear();
                            draw->PathArcTo(ImVec2(pMin.x + 18.0f, pMidY + 1.0f), 3.0f, IM_PI, 0);
                            draw->PathStroke(ImColor(0, 200, 220), false, 1.2f);

                            // Toggle ojo
                            ImGui::SetCursorScreenPos(ImVec2(pMax.x - 32.0f, pMin.y));
                            if (ImGui::InvisibleButton("##toggle_reg_pass", ImVec2(32.0f, pMax.y - pMin.y))) show_reg_password = !show_reg_password;
                            ImVec2 eyeCenter(pMax.x - 16.0f, pMidY);
                            draw->PathClear();
                            draw->PathArcTo(ImVec2(eyeCenter.x, eyeCenter.y + 3.0f), 5.0f, -IM_PI * 0.75f, -IM_PI * 0.25f);
                            draw->PathStroke(ImColor(0, 200, 220), false, 1.2f);
                            draw->PathClear();
                            draw->PathArcTo(ImVec2(eyeCenter.x, eyeCenter.y - 3.0f), 5.0f, IM_PI * 0.25f, IM_PI * 0.75f);
                            draw->PathStroke(ImColor(0, 200, 220), false, 1.2f);
                            draw->AddCircleFilled(eyeCenter, 2.0f, ImColor(0, 220, 255));
                            if (show_reg_password) draw->AddLine(ImVec2(eyeCenter.x - 5.0f, eyeCenter.y - 3.0f), ImVec2(eyeCenter.x + 5.0f, eyeCenter.y + 3.0f), ImColor(255, 100, 100), 1.5f);

                            if (ImGui::IsItemActive()) draw->AddRect(pMin, pMax, ImColor(0, 220, 255), 6.0f, 0, 1.5f);

                            ImGui::Spacing();

                            // --- KEY / LICENCIA ---
                            ImGui::InputTextWithHint("##reg_key", "Codigo de Licencia", datos.Key, sizeof(datos.Key));
                            ImVec2 kMin = ImGui::GetItemRectMin();
                            ImVec2 kMax = ImGui::GetItemRectMax();
                            float kMidY = (kMin.y + kMax.y) * 0.5f;

                            // Icono de llave procedural
                            ImVec2 keyCenter(kMin.x + 17.0f, kMidY);
                            draw->AddCircle(ImVec2(keyCenter.x - 3.0f, keyCenter.y), 3.0f, ImColor(0, 200, 220), 0, 1.2f);
                            draw->AddLine(ImVec2(keyCenter.x, keyCenter.y), ImVec2(keyCenter.x + 8.0f, keyCenter.y), ImColor(0, 200, 220), 1.2f);
                            draw->AddLine(ImVec2(keyCenter.x + 4.0f, keyCenter.y), ImVec2(keyCenter.x + 4.0f, keyCenter.y + 3.0f), ImColor(0, 200, 220), 1.2f);
                            draw->AddLine(ImVec2(keyCenter.x + 7.0f, keyCenter.y), ImVec2(keyCenter.x + 7.0f, keyCenter.y + 3.0f), ImColor(0, 200, 220), 1.2f);

                            if (ImGui::IsItemActive()) draw->AddRect(kMin, kMax, ImColor(0, 220, 255), 6.0f, 0, 1.5f);

                            ImGui::PopStyleColor(3);
                            ImGui::PopItemWidth();

                            style.FrameRounding = oldRounding;
                            style.FramePadding = oldPadding;

                            // --- BOTÓN REGISTRARSE (Estilo sci-fi dorado/cyan) ---
                            ImGui::SetCursorPos(ImVec2(0, 160));
                            ImVec2 rBtnMin = ImGui::GetCursorScreenPos();
                            ImVec2 rBtnMax = rBtnMin + ImVec2(320.0f, 42.0f);

                            bool rBtnHovered = ImGui::IsMouseHoveringRect(rBtnMin, rBtnMax);
                            bool rBtnClicked = rBtnHovered && ImGui::IsMouseClicked(0);

                            ImColor rBtnBorder = rBtnHovered ? ImColor(255, 180, 0) : ImColor(0, 200, 255);
                            draw->AddRectFilled(rBtnMin, rBtnMax, rBtnHovered ? ImColor(12, 45, 60) : ImColor(6, 25, 35), 6.0f);
                            draw->AddRect(rBtnMin, rBtnMax, rBtnBorder, 6.0f, 0, 1.5f);

                            draw->AddLine(rBtnMin, rBtnMin + ImVec2(15.0f, 0), ImColor(255, 180, 0), 2.5f);
                            draw->AddLine(rBtnMin, rBtnMin + ImVec2(0, 15.0f), ImColor(255, 180, 0), 2.5f);
                            draw->AddLine(rBtnMax, rBtnMax - ImVec2(15.0f, 0), ImColor(255, 180, 0), 2.5f);
                            draw->AddLine(rBtnMax, rBtnMax - ImVec2(0, 15.0f), ImColor(255, 180, 0), 2.5f);

                            ImGui::PushFont(Inter_S_1);
                            ImVec2 rBtnTextSize = ImGui::CalcTextSize("REGISTRARSE");
                            draw->AddText(rBtnMin + ImVec2((320.0f - rBtnTextSize.x) * 0.5f, (42.0f - rBtnTextSize.y) * 0.5f), ImColor(255, 255, 255), "REGISTRARSE");
                            ImGui::PopFont();

                            if (rBtnClicked)
                            {
                                if (KeyAuthApp.regstr(datos.Username, datos.Password, datos.Key))
                                {
                                    registrard = false;
                                    login = true;
                                    Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Licencia activada con exito! Inicie sesion.", 5.0f);
                                }
                                else
                                {
                                    std::string safeError = KeyAuthApp.last_error;
                                    if (safeError.empty()) {
                                        safeError = "Error al activar la licencia o conexion fallida.";
                                    }
                                    Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", safeError, 5.0f);
                                }
                            }

                            // --- VOLVER AL LOGIN LINK ---
                            ImGui::SetCursorPos(ImVec2(0, 220));
                            ImGui::PushFont(Inter_S);
                            std::string backStr = "¿Ya tiene cuenta? ";
                            std::string loginLinkStr = "Inicie sesion aqui";
                            ImVec2 sizeBack = ImGui::CalcTextSize(backStr.c_str());
                            ImVec2 sizeLoginLink = ImGui::CalcTextSize(loginLinkStr.c_str());
                            float backStartX = (320.0f - (sizeBack.x + sizeLoginLink.x)) * 0.5f;

                            ImVec2 bPos1 = ImGui::GetCursorScreenPos() + ImVec2(backStartX, 0);
                            ImVec2 bPos2 = bPos1 + ImVec2(sizeBack.x, 0);

                            draw->AddText(bPos1, ImColor(160, 165, 175), backStr.c_str());

                            ImGui::SetCursorScreenPos(bPos2);
                            bool loginHovered = false;
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0f, 0.8f, 1.0f, 1.0f));
                            if (ImGui::Selectable("Inicie sesion aqui", &loginHovered, ImGuiSelectableFlags_DontClosePopups, sizeLoginLink))
                            {
                                registrard = false;
                                login = true;
                            }
                            ImGui::PopStyleColor();

                            if (loginHovered)
                            {
                                draw->AddLine(bPos2 + ImVec2(0, sizeLoginLink.y + 1.0f), bPos2 + ImVec2(sizeLoginLink.x, sizeLoginLink.y + 1.0f), ImColor(0, 200, 255), 1.0f);
                            }

                            ImGui::PopFont();
                        }
                        ImGui::EndChild();
                    }
                }
                ImGui::End();
                ImGui::PopStyleVar(3);
            }


            else if (loading && !authenticed)
            {
                // Renderizado ImGui
                ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
                if (ImGui::Begin("login", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
                {
                    auto draw = ImGui::GetWindowDrawList();
                    ImVec2 pos = ImGui::GetWindowPos();
                    ImVec2 size = ImGui::GetWindowSize();
                    
                    // Fondo Glassmorphism estilo Panel
                    draw->AddRectFilled(pos, pos + size, ImColor(10, 10, 14, 160), 8.0f);
                    ImColor themeBorder = ImColor(0, 200, 255, 150);
                    draw->AddRect(pos, pos + size, themeBorder, 6.0f, 0, 1.5f);

                    ImGui::SetCursorPos(ImVec2(340, 160));
                    ImVec4 textoColor = ImVec4(0.3f, 0.6f, 1.0f, 1.0f);

                    DrawCircularProgressBar(progress1, 6.0f, ImGui::GetColorU32(c::main_coCQlor1), "Cargando...", textoColor);

                    ImGui::Dummy(ImVec2(0, 0));

                    progress1 += load_speed * ImGui::GetIO().DeltaTime;
                    if (progress1 >= 1.0f)
                    {
                        loading = false;
                        authenticed = false;
                        show_login = false;
                        choose_game = true;
                    }


                }
                ImGui::End();

            }



            if (choose_game)
            {
                ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
                if (ImGui::Begin("Selecciona Version de Free Fire", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
                {
                    auto draw = ImGui::GetWindowDrawList();
                    ImVec2 pos = ImGui::GetWindowPos();
                    ImVec2 size = ImGui::GetWindowSize();

                    // Fondo Glassmorphism estilo Panel
                    ImColor themeBorder = ImColor(0, 200, 255, 150);
                    draw->AddRectFilled(pos, pos + size, ImColor(10, 10, 14, 160), 8.0f);
                    draw->AddRect(pos, pos + size, themeBorder, 6.0f, 0, 1.5f);

                    // Titulo superior
                    ImGui::PushFont(Nev1);
                    ImVec2 title_size = ImGui::CalcTextSize("ELIGE TU VERSION DE FREE FIRE");
                    ShadowText(draw, "ELIGE TU VERSION DE FREE FIRE", ImVec2(pos.x + (size.x - title_size.x) * 0.5f, pos.y + 40.0f), ImColor(0, 220, 255), ImColor(0, 200, 255, 80), 0.0f);
                    ImGui::PopFont();

                    // Opcion 1: Free Fire Normal
                    ImGui::SetCursorPos(ImVec2(42.5f, 150.0f));
                    if (ImGui::ImageButton("ff_normal_btn", (ImTextureID)image2::ffimagen, ImVec2(210.0f, 168.0f)))
                    {
                        selected_game_version = 0;
                        initOffsets(0);
                        choose_game = false;
                        loading2 = true;
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("Free Fire Normal (com.dts.freefireth)");
                        ImGui::EndTooltip();
                    }

                    // Opcion 2: Free Fire MAX
                    ImGui::SetCursorPos(ImVec2(295.0f, 150.0f));
                    if (ImGui::ImageButton("ff_max_btn", (ImTextureID)image2::ffimagenMAX, ImVec2(210.0f, 168.0f)))
                    {
                        selected_game_version = 1;
                        initOffsets(1);
                        choose_game = false;
                        loading2 = true;
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("Free Fire MAX (com.dts.freefiremax)");
                        ImGui::EndTooltip();
                    }

                    // Opcion 3: Free Fire MAX Beta
                    ImGui::SetCursorPos(ImVec2(547.5f, 150.0f));
                    if (ImGui::ImageButton("ff_max_beta_btn", (ImTextureID)image2::ffimagenTELA, ImVec2(210.0f, 168.0f)))
                    {
                        selected_game_version = 2;
                        initOffsets(2);
                        choose_game = false;
                        loading2 = true;
                    }
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("Free Fire MAX Beta (com.dts.freefiremax)");
                        ImGui::EndTooltip();
                    }

                    // Textos descriptivos debajo de cada imagen
                    ImGui::PushFont(Inter_S_1);
                    ImVec2 label1_size = ImGui::CalcTextSize("FREE FIRE NORMAL");
                    ImVec2 label2_size = ImGui::CalcTextSize("FREE FIRE MAX");
                    ImVec2 label3_size = ImGui::CalcTextSize("FREE FIRE MAX BETA");
                    draw->AddText(ImVec2(pos.x + 42.5f + (210.0f - label1_size.x) * 0.5f, pos.y + 330.0f), ImColor(255, 255, 255), "FREE FIRE NORMAL");
                    draw->AddText(ImVec2(pos.x + 295.0f + (210.0f - label2_size.x) * 0.5f, pos.y + 330.0f), ImColor(255, 255, 255), "FREE FIRE MAX");
                    draw->AddText(ImVec2(pos.x + 547.5f + (210.0f - label3_size.x) * 0.5f, pos.y + 330.0f), ImColor(255, 255, 255), "FREE FIRE MAX BETA");
                    ImGui::PopFont();
                }
                ImGui::End();
            }

            if (loading2 && !authenticed)
            {
                ImGui::SetNextWindowSize(ImVec2(800, 500), ImGuiCond_Always);
                if (ImGui::Begin("login_after_choose", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground))
                {
                    auto draw = ImGui::GetWindowDrawList();
                    ImVec2 pos = ImGui::GetWindowPos();
                    ImVec2 size = ImGui::GetWindowSize();
                    
                    // Fondo Glassmorphism estilo Panel
                    draw->AddRectFilled(pos, pos + size, ImColor(10, 10, 14, 160), 8.0f);
                    ImColor themeBorder = ImColor(0, 200, 255, 150);
                    draw->AddRect(pos, pos + size, themeBorder, 6.0f, 0, 1.5f);

                    ImGui::SetCursorPos(ImVec2(340, 160));
                    ImVec4 textoColor = ImVec4(0.3f, 0.6f, 1.0f, 1.0f);

                    DrawCircularProgressBar(progress2, 6.0f, ImGui::GetColorU32(c::main_coCQlor1), "Iniciando Panel...", textoColor);

                    ImGui::Dummy(ImVec2(0, 0));

                    // Dureza = 3 segundos
                    progress2 += (1.0f / 3.0f) * ImGui::GetIO().DeltaTime;
                    if (progress2 >= 1.0f)
                    {
                        loading2 = false;
                        authenticed = true;
                    }
                }
                ImGui::End();
            }

            if (authenticed)
            {

                


                ImGui::SetNextWindowSize(ImVec2(1000, 650), ImGuiCond_Always);
                if (ImGui::Begin("panel principal", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar))
                {

                    auto draw = ImGui::GetWindowDrawList();
                    auto background_list1 = ImGui::GetBackgroundDrawList();
                    ImVec2 pos = ImGui::GetWindowPos();
                    ImVec2 size = ImGui::GetWindowSize();
                    float splitX = size.x * 0.09f;
                    float WIDTH3 = 1000;
                    float HEIGHT3 = 650;
                    ImVec2 top_left1 = pos;
                    ImVec2 bottom_right1 = ImVec2(pos.x + WIDTH3, pos.y + HEIGHT3);
                    float rounding13313 = c::rounding;

                    ImColor themeBorder = ImColor(0, 200, 255, 150);
                    ImColor textCyan = ImColor(0, 220, 255, 255);

                    draw->AddRectFilled(pos, ImVec2(pos.x + splitX + 1.0f, pos.y + size.y), ImColor(10, 10, 14, 160), 8, ImDrawFlags_RoundCornersLeft);
                    draw->AddRectFilled(ImVec2(pos.x + splitX, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), ImColor(10, 10, 15, 140), 8, ImDrawFlags_RoundCornersRight);
                    ImGui::GetBackgroundDrawList()->AddRect(pos, pos + ImVec2(1000, 650), themeBorder, 6, 0, 1.5f);

                    // Decorative Cyber Circuits
                    ImColor circC = ImColor(0, 200, 255, 50);
                    draw->AddLine(ImVec2(pos.x + splitX + 20, pos.y + 40), ImVec2(pos.x + splitX + 80, pos.y + 40), circC, 1.5f);
                    draw->AddLine(ImVec2(pos.x + splitX + 80, pos.y + 40), ImVec2(pos.x + splitX + 100, pos.y + 60), circC, 1.5f);
                    draw->AddLine(ImVec2(pos.x + splitX + 100, pos.y + 60), ImVec2(pos.x + splitX + 150, pos.y + 60), circC, 1.5f);
                    draw->AddCircleFilled(ImVec2(pos.x + splitX + 150, pos.y + 60), 2.5f, circC);
                    
                    draw->AddLine(ImVec2(pos.x + size.x - 20, pos.y + 40), ImVec2(pos.x + size.x - 80, pos.y + 40), circC, 1.5f);
                    draw->AddLine(ImVec2(pos.x + size.x - 80, pos.y + 40), ImVec2(pos.x + size.x - 100, pos.y + 60), circC, 1.5f);
                    draw->AddLine(ImVec2(pos.x + size.x - 100, pos.y + 60), ImVec2(pos.x + size.x - 150, pos.y + 60), circC, 1.5f);
                    draw->AddCircleFilled(ImVec2(pos.x + size.x - 150, pos.y + 60), 2.5f, circC);

                    // Particles (Cyan themed)
                    static ImVec2 part_pos[80];
                    static ImVec2 part_vel[80];
                    static bool part_init = false;
                    if (!part_init) {
                        for (int i = 0; i < 80; i++) {
                            part_pos[i] = ImVec2(pos.x + (rand() % (int)size.x), pos.y + (rand() % (int)size.y));
                            part_vel[i] = ImVec2((rand() % 10 - 5) * 0.05f, (rand() % 10 - 5) * 0.05f);
                        }
                        part_init = true;
                    }
                    for (int i = 0; i < 80; i++) {
                        part_pos[i].x += part_vel[i].x;
                        part_pos[i].y += part_vel[i].y;
                        if (part_pos[i].x < pos.x) part_pos[i].x = pos.x + size.x;
                        if (part_pos[i].x > pos.x + size.x) part_pos[i].x = pos.x;
                        if (part_pos[i].y < pos.y) part_pos[i].y = pos.y + size.y;
                        if (part_pos[i].y > pos.y + size.y) part_pos[i].y = pos.y;
                        draw->AddCircleFilled(part_pos[i], 1.5f, ImColor(0, 200, 255, 120));
                        for (int j = i + 1; j < 80; j++) {
                            float dist = sqrtf(powf(part_pos[i].x - part_pos[j].x, 2) + powf(part_pos[i].y - part_pos[j].y, 2));
                            if (dist < 45.0f) {
                                float alpha = 1.0f - (dist / 45.0f);
                                draw->AddLine(part_pos[i], part_pos[j], ImColor(0, 200, 255, (int)(alpha * 120)), 1.0f);
                            }
                        }
                    }

                    // Removed old ASMODEUS logo and text to clean up top area


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

                    // Duplicate backgrounds removed for better transparency



                    /////////////////////////////////////////////////////////////////////////////////////////



                    // Left Sidebar UI (Matches REAL $ENSI aesthetic)
                    ImGui::SetCursorPos(ImVec2(20, 20));
                    ImGui::SetWindowFontScale(1.5f);
                    ImGui::Text("ASMO");
                    ImGui::SameLine(0, 0);
                    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "DEUS");
                    ImGui::SetWindowFontScale(1.0f);
                    
                    ImGui::SetWindowFontScale(1.0f);

                    ImGui::BeginGroup();
                    {
                        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 20));
                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.05f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1, 1, 1, 0.1f));
                        
                        ImGui::SetCursorPos(ImVec2(15, 80));
                        ImGui::BeginGroup();
                        {
                            auto SidebarButton = [&](const char* label, void* tex, int target_page) {
                                bool active = (page == target_page);
                                ImU32 col = active ? IM_COL32(255, 255, 255, 255) : IM_COL32(180, 180, 180, 255);
                                ImVec2 p = ImGui::GetCursorScreenPos();
                                
                                if (active) {
                                    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(p.x, p.y), ImVec2(p.x + 180, p.y + 35), IM_COL32(255, 255, 255, 20), 4.0f);
                                    ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x, p.y + 4), ImVec2(p.x, p.y + 31), IM_COL32(255, 255, 255, 255), 2.0f);
                                }
                                
                                if (ImGui::InvisibleButton(label, ImVec2(180, 35))) page = target_page;
                                
                                ImGui::GetWindowDrawList()->AddImage(tex, ImVec2(p.x + 10, p.y + 8), ImVec2(p.x + 30, p.y + 28), ImVec2(0,0), ImVec2(1,1), col);
                                ImGui::GetWindowDrawList()->AddText(ImVec2(p.x + 40, p.y + 8), col, label);
                            };

                            SidebarButton("Aimbot", (void*)image2::main, 0);
                            SidebarButton("Esp", (void*)image2::settings, 2);
                            SidebarButton("Chams", (void*)image2::panel, 3); // Dummy page
                            SidebarButton("KeyBind", (void*)image2::panel, 4); // Dummy page
                            SidebarButton("FixLag", (void*)image2::panel, 5); // Dummy page
                            SidebarButton("Enemies", (void*)image2::panel, 7); // Dummy page
                            SidebarButton("Config.", (void*)image2::settings, 6);
                        }
                        ImGui::EndGroup();
                        ImGui::PopStyleColor(3);
                        ImGui::PopStyleVar();
                        
                        // User Profile at bottom left
                        ImGui::SetCursorPos(ImVec2(15, 580));
                        ImVec2 upos = ImGui::GetCursorScreenPos();
                        ImGui::GetWindowDrawList()->AddImageRounded(logo, upos, ImVec2(upos.x + 40, upos.y + 40), ImVec2(0,0), ImVec2(1,1), IM_COL32(255,255,255,255), 20.0f);
                        std::string userStr = datos.Username;
                        if (userStr.empty()) userStr = "ᶻ̷ ᴴ𝒯𝐸𝒜𝑀";
                        std::string displayUser = "USUARIO\n" + userStr;
                        ImGui::GetWindowDrawList()->AddText(ImVec2(upos.x + 50, upos.y + 5), IM_COL32(200, 200, 200, 255), displayUser.c_str());
                        ImGui::Dummy(ImVec2(150, 40));
                    }
                    ImGui::EndGroup();

                    //////////////////////////
                    static bool toggleAim = true;
                    static bool toggleFunc = false;
                    static bool toggleChams = true;
                    static bool toggleConfig = false;
                    static bool toggleLag = true;
                    static bool toggleLagAdv = false;
                    static bool toggleSettings = true;
                    static bool toggleUI = true;

                    auto BeginGroupBox = [&](const char* title, const char* subtitle, const char* id, ImVec2 pos, ImVec2 size, bool* toggle)
                        {
                            ImDrawList* draw = ImGui::GetWindowDrawList();
                            ImVec2 windowPos = ImGui::GetWindowPos();

                            ImVec2 p1 = ImVec2(windowPos.x + pos.x, windowPos.y + pos.y);
                            ImVec2 p2 = ImVec2(p1.x + size.x, p1.y + size.y);

                            // Match new aesthetic: transparent glassmorphism matching the panel right side (alpha 140)
                            draw->AddRectFilled(p1, p2, IM_COL32(10, 10, 15, 140), 8.f);
                            draw->AddRect(p1, p2, IM_COL32(40, 40, 40, 140), 8.f, 0, 1.0f);
                            
                            // Banner section top (transparent alpha 140)
                            draw->AddRectFilledMultiColor(p1, ImVec2(p2.x, p1.y + 40), IM_COL32(40, 40, 45, 140), IM_COL32(15, 15, 18, 140), IM_COL32(15, 15, 18, 140), IM_COL32(40, 40, 45, 140));

                            draw->AddText(ImVec2(p1.x + 15.f, p1.y + 12.f), IM_COL32(255, 255, 255, 255), title);
                            
                            if (toggle != nullptr) {
                                ImGui::SetCursorPos(ImVec2(pos.x + size.x - 45.f, pos.y + 10.f));
                                ImGui::PushID(id);
                                ImGui::Checkbox2(" ", toggle);
                                ImGui::PopID();
                            }

                            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));
                            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
                            ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.f);
                            ImGui::SetCursorPos(ImVec2(pos.x + 10.f, pos.y + 50.f));
                            ImGui::BeginChild(id, ImVec2(size.x - 20.f, size.y - 60.f), false);
                        };

                    auto EndGroupBox = [&]()
                        {
                            ImGui::EndChild();
                            ImGui::PopStyleVar();
                            ImGui::PopStyleColor(2);
                        };


                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha* style->Alpha);
                    {

                        if (active_tab == 0)
                        {
                            BeginGroupBox("Aimbot - External", nullptr, "tab1", ImVec2(240, 80), ImVec2(720, 520), &toggleAim);
                            {
                                ImGui::SetCursorPosX(10);
                                if (ImGui::Checkbox2("INICIAR ADB", &adbboton)) {
                                    if (adbboton && !adbInitialized) { 
                                        Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Launching game and injecting...", 4.0f);
                                        initOffsets(selected_game_version);
                                        std::thread([]() {
                                            hdPlayerWindow = INJECTESPADB();
                                            while (!adbInitialized) std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                            EnabledEsp = true;
                                            Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Injected successfully!", 4.0f);
                                        }).detach();
                                    }
                                }

                                if (adbInitialized) {
                                    ImGui::Spacing();
                                    ImGui::Separator();
                                    ImGui::Spacing();
                                }

                                  ImGui::SetCursorPosX(10);
                                  ImGui::Checkbox2("Enable Aim Assist", &g_AimAssist);

                                  if (g_AimAssist) {
                                      ImGui::SetCursorPosX(10);
                                      ImGui::Checkbox2("Aim Teammates (Bots)", &g_AimTeammates);
                                      ImGui::SetCursorPosX(10);
                                      ImGui::PushItemWidth(120);
                                      const char* AimTypeNames[] = { "Memory Lock", "Memory Smooth", "Mouse Emulation", "Silent Aim" };
                                      ImGui::Combo("Aim Assist Type", &g_AimAssistType, AimTypeNames, IM_ARRAYSIZE(AimTypeNames));
                                      ImGui::PopItemWidth();

                                      if (g_AimAssistType == 1 || g_AimAssistType == 2) {
                                          ImGui::SetCursorPosX(10);
                                          ImGui::PushItemWidth(120);
                                          ImGui::SliderFloat("Smoothness", &g_AimSmoothness, 0.0f, 0.99f, "%.2f");
                                          ImGui::PopItemWidth();
                                      }

                                      ImGui::SetCursorPosX(10);
                                      ImGui::PushItemWidth(120);
                                      const char* AimKeyNames[] = { "Right Click", "Left Click", "Shift Key", "Caps Lock" };
                                      static int current_key_idx = 0; // Default Right Click (VK_RBUTTON)
                                      if (ImGui::Combo("Aim Assist Key", &current_key_idx, AimKeyNames, IM_ARRAYSIZE(AimKeyNames))) {
                                          if (current_key_idx == 0) g_AimKey = VK_RBUTTON;
                                          else if (current_key_idx == 1) g_AimKey = VK_LBUTTON;
                                          else if (current_key_idx == 2) g_AimKey = VK_SHIFT;
                                          else if (current_key_idx == 3) g_AimKey = VK_CAPITAL;
                                      }
                                      ImGui::PopItemWidth();

                                      ImGui::SetCursorPosX(10);
                                      ImGui::PushItemWidth(120);
                                      ImGui::Combo("Target Bone", &g_AimTargetBone, PullBoneOptions, IM_ARRAYSIZE(PullBoneOptions), 180);
                                      ImGui::PopItemWidth();

                                      ImGui::SetCursorPosX(10);
                                      ImGui::PushItemWidth(120);
                                      ImGui::SliderInt("Smooth Shots (Chest)", &g_ChestShotsCount, 0, 5, "%d shots");
                                      ImGui::PopItemWidth();
                                  }

                                  ImGui::SetCursorPosX(10);
                                  ImGui::Checkbox2("Show Aim FOV", &g_EspFov);

                                  if (g_EspFov) {
                                      ImGui::SetCursorPosX(10);
                                      ImGui::PushItemWidth(120);
                                      ImGui::SliderInt("Aim Fov Size", &g_EspFovRadius, 10, 800, "%d px");
                                      ImGui::PopItemWidth();
                                  }

                                  ImGui::SetCursorPosX(10);
                                  ImGui::Checkbox2("Silent Aim", &aimsilent);

                                  ImGui::SetCursorPosX(10);
                                  ImGui::Checkbox2("No Recoil", &NoRecoil);
                            }
                            EndGroupBox();
                        }

                        if (active_tab == 2)
                        {
                            // 1. Opciones y Sincronizacion de Interfaz de ESP
                            const char* performanceOpts[] = { "Simple", "Medium", "Complex" };
                            const char* linesOpts[] = { "None", "Top", "Center", "Bottom" };
                            const char* boxOpts[] = { "None", "Normal", "Filled", "Corner", "3D" };
                            const char* infoOpts[] = { "None", "Name", "Distance", "Name, Distance", "Distance, BR Rank", "All" };
                            const char* weaponOpts[] = { "None", "Name", "Icon", "Icon, Name" };
                            const char* extraOpts[] = { "None", "Skeleton", "Health", "Skeleton, Health" };

                            static int espPerformanceIdx = 0;
                            static int espLinesIdx = 0;
                            static int espBoxIdx = 0;
                            static int espInfoIdx = 0;
                            static int espWeaponIdx = 0;
                            static int espExtraIdx = 0;
                            static int espDistanceVal = 600;

                            static bool syncd = false;
                            if (!syncd) {
                                espLinesIdx = g_EspLine ? (g_EspLineStyle + 1) : 0;
                                espBoxIdx = g_EspBox ? g_EspBoxStyle : 0;
                                if (g_EspName && g_EspDistance) espInfoIdx = 3;
                                else if (g_EspName) espInfoIdx = 4;
                                else if (g_EspDistance) espInfoIdx = 2;
                                else espInfoIdx = 0;
                                espWeaponIdx = (g_EspWeaponIcon && g_EspWeaponText) ? 3 : (g_EspWeaponIcon ? 2 : (g_EspWeaponText ? 1 : 0));
                                espExtraIdx = (g_EspBones && g_EspHealth) ? 3 : (g_EspBones ? 1 : (g_EspHealth ? 2 : 0));
                                espDistanceVal = g_EspMaxDistance;
                                syncd = true;
                            }

                            // Lambda helper para filas elegantes de Dropdown
                            auto DrawComboRow = [](const char* label, const char** items, int count, int* current) {
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text(label);
                                ImGui::SameLine();
                                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 130.0f);
                                ImGui::PushItemWidth(120.0f);
                                std::string cid = "##" + std::string(label);
                                ImGui::Combo(cid.c_str(), current, items, count);
                                ImGui::PopItemWidth();
                            };

                            // --- COLUMNA IZQUIERDA: CONFIGURACION ---
                            BeginGroupBox("Visuals - ESP", nullptr, "tab1", ImVec2(240, 80), ImVec2(340, 520), &g_EspEnabled);
                            {
                                ImGui::SetCursorPosX(10);
                                DrawComboRow("ESP Performance", performanceOpts, IM_ARRAYSIZE(performanceOpts), &espPerformanceIdx);
                                ImGui::SetCursorPosX(10);
                                DrawComboRow("ESP Lines", linesOpts, IM_ARRAYSIZE(linesOpts), &espLinesIdx);
                                ImGui::SetCursorPosX(10);
                                DrawComboRow("ESP Box", boxOpts, IM_ARRAYSIZE(boxOpts), &espBoxIdx);
                                ImGui::SetCursorPosX(10);
                                DrawComboRow("ESP Info", infoOpts, IM_ARRAYSIZE(infoOpts), &espInfoIdx);
                                ImGui::SetCursorPosX(10);
                                DrawComboRow("ESP Weapon", weaponOpts, IM_ARRAYSIZE(weaponOpts), &espWeaponIdx);
                                ImGui::SetCursorPosX(10);
                                DrawComboRow("ESP Extra", extraOpts, IM_ARRAYSIZE(extraOpts), &espExtraIdx);

                                ImGui::Spacing();
                                ImGui::SetCursorPosX(10);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("Distance ESP");
                                ImGui::SameLine();
                                ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 40.0f);
                                ImGui::Text("%d", espDistanceVal);

                                ImGui::SetCursorPosX(10);
                                ImGui::PushItemWidth(ImGui::GetWindowWidth() - 20.0f);
                                ImGui::SliderInt("##distance_esp_val", &espDistanceVal, 10, 1500, "");
                                ImGui::PopItemWidth();

                                ImGui::Spacing();
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Fast Activation", &g_EspEnabled);
                                
                                static bool prev_g_EspEnabled = false;
                                if (g_EspEnabled && !prev_g_EspEnabled) {
                                    if (espLinesIdx == 0) espLinesIdx = 2; // Center
                                    if (espBoxIdx == 0) espBoxIdx = 1; // Normal
                                    if (espInfoIdx == 0) espInfoIdx = 3; // Name, Distance
                                    if (espWeaponIdx == 0) espWeaponIdx = 3; // Icon, Name
                                    if (espExtraIdx == 0) espExtraIdx = 3; // Skeleton, Health
                                    espDistanceVal = 1500;
                                } else if (!g_EspEnabled && prev_g_EspEnabled) {
                                    espLinesIdx = 0;
                                    espBoxIdx = 0;
                                    espInfoIdx = 0;
                                    espWeaponIdx = 0;
                                    espExtraIdx = 0;
                                }
                                prev_g_EspEnabled = g_EspEnabled;
                                
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Rainbow ESP (RGB)", &g_EspRainbowGlobal);
                            }
                            EndGroupBox();

                            // Aplicar cambios visuales a variables reales de ESP en tiempo real
                            g_EspLine = (espLinesIdx > 0);
                            if (g_EspLine) g_EspLineStyle = espLinesIdx - 1;

                            g_EspBox = (espBoxIdx > 0);
                            if (g_EspBox) g_EspBoxStyle = espBoxIdx;

                            if (espInfoIdx == 0) { g_EspName = false; g_EspDistance = false; }
                            else if (espInfoIdx == 1) { g_EspName = true; g_EspDistance = false; }
                            else if (espInfoIdx == 2) { g_EspName = false; g_EspDistance = true; }
                            else if (espInfoIdx == 3) { g_EspName = true; g_EspDistance = true; }
                            else if (espInfoIdx == 4) { g_EspName = true; g_EspDistance = false; } // BR Rank con nombre
                            else if (espInfoIdx == 5) { g_EspName = true; g_EspDistance = true; }

                            if (espWeaponIdx == 0) { g_EspWeaponText = false; g_EspWeaponIcon = false; }
                            else if (espWeaponIdx == 1) { g_EspWeaponText = true; g_EspWeaponIcon = false; }
                            else if (espWeaponIdx == 2) { g_EspWeaponText = false; g_EspWeaponIcon = true; }
                            else if (espWeaponIdx == 3) { g_EspWeaponText = true; g_EspWeaponIcon = true; }

                            if (espExtraIdx == 0) { g_EspBones = false; g_EspHealth = false; }
                            else if (espExtraIdx == 1) { g_EspBones = true; g_EspHealth = false; }
                            else if (espExtraIdx == 2) { g_EspBones = false; g_EspHealth = true; }
                            else if (espExtraIdx == 3) { g_EspBones = true; g_EspHealth = true; }

                            g_EspMaxDistance = espDistanceVal;

                            // --- COLUMNA DERECHA: ESP EDITOR PREVIEW ---
                            BeginGroupBox("ESP Editor", nullptr, "tab2", ImVec2(600, 80), ImVec2(360, 520), &g_EspEnabled);
                            {
                                ImVec2 previewPos = ImGui::GetCursorScreenPos();
                                float w = ImGui::GetContentRegionAvail().x;
                                float h = ImGui::GetContentRegionAvail().y;

                                // Dimensiones del recuadro del render
                                ImVec2 cardSize(250.0f, 290.0f);
                                ImVec2 cardPos = ImVec2(previewPos.x + (w - cardSize.x) * 0.5f, previewPos.y + (h - cardSize.y) * 0.5f - 10.0f);
                                ImVec2 cardMax = ImVec2(cardPos.x + cardSize.x, cardPos.y + cardSize.y);

                                ImDrawList* draw = ImGui::GetWindowDrawList();

                                // 1. Fondo del contenedor oscuro premium
                                draw->AddRectFilled(cardPos, cardMax, IM_COL32(8, 8, 10, 255), 10.0f);
                                draw->AddRect(cardPos, cardMax, IM_COL32(30, 30, 35, 255), 10.0f, 0, 1.5f);

                                // Centro y alineaciones internas
                                float cx = cardPos.x + cardSize.x * 0.5f;
                                float cy = cardPos.y + cardSize.y * 0.5f;
                                float sy = cy - 20.0f; // centro de gravedad del stick figure

                                // 2. Dibujar Skeleton (Stick Figure) interactivo
                                ImU32 boneCol = IM_COL32(255, 255, 255, 255);
                                if (g_EspBones) {
                                    // Cabeza
                                    ImVec2 head(cx, sy - 40.0f);
                                    draw->AddCircle(head, 14.0f, boneCol, 16, 1.8f);

                                    // Columna Vertebral
                                    ImVec2 neck(cx, sy - 26.0f);
                                    ImVec2 pelvis(cx, sy + 15.0f);
                                    draw->AddLine(neck, pelvis, boneCol, 1.8f);

                                    // Brazo Izquierdo
                                    ImVec2 lShoulder(cx - 10.0f, sy - 20.0f);
                                    ImVec2 lElbow(cx - 28.0f, sy - 10.0f);
                                    ImVec2 lHand(cx - 36.0f, sy + 12.0f);
                                    draw->AddLine(neck, lShoulder, boneCol, 1.8f);
                                    draw->AddLine(lShoulder, lElbow, boneCol, 1.8f);
                                    draw->AddLine(lElbow, lHand, boneCol, 1.8f);

                                    // Brazo Derecho
                                    ImVec2 rShoulder(cx + 10.0f, sy - 20.0f);
                                    ImVec2 rElbow(cx + 28.0f, sy - 10.0f);
                                    ImVec2 rHand(cx + 36.0f, sy + 12.0f);
                                    draw->AddLine(neck, rShoulder, boneCol, 1.8f);
                                    draw->AddLine(rShoulder, rElbow, boneCol, 1.8f);
                                    draw->AddLine(rElbow, rHand, boneCol, 1.8f);

                                    // Pierna Izquierda
                                    ImVec2 lKnee(cx - 16.0f, sy + 45.0f);
                                    ImVec2 lFoot(cx - 24.0f, sy + 80.0f);
                                    draw->AddLine(pelvis, lKnee, boneCol, 1.8f);
                                    draw->AddLine(lKnee, lFoot, boneCol, 1.8f);

                                    // Pierna Derecha
                                    ImVec2 rKnee(cx + 16.0f, sy + 45.0f);
                                    ImVec2 rFoot(cx + 24.0f, sy + 80.0f);
                                    draw->AddLine(pelvis, rKnee, boneCol, 1.8f);
                                    draw->AddLine(rKnee, rFoot, boneCol, 1.8f);
                                }

                                // Dimensiones de la caja del ESP
                                float boxW = 86.0f;
                                float boxH = 150.0f;
                                float bx = cx - boxW * 0.5f;
                                float by = sy - 58.0f;

                                // 3. Dibujar Caja ESP interactiva
                                if (g_EspBox && g_EspBoxStyle == 3) {
                                    float l = boxW / 4.0f;
                                    float t = boxH / 4.0f;
                                    ImU32 wCol = IM_COL32(255, 255, 255, 255);
                                    draw->AddLine(ImVec2(bx, by), ImVec2(bx + l, by), wCol, 2.5f);
                                    draw->AddLine(ImVec2(bx, by), ImVec2(bx, by + t), wCol, 2.5f);
                                    draw->AddLine(ImVec2(bx + boxW - l, by), ImVec2(bx + boxW, by), wCol, 2.5f);
                                    draw->AddLine(ImVec2(bx + boxW, by), ImVec2(bx + boxW, by + t), wCol, 2.5f);
                                    draw->AddLine(ImVec2(bx, by + boxH - t), ImVec2(bx, by + boxH), wCol, 2.5f);
                                    draw->AddLine(ImVec2(bx, by + boxH), ImVec2(bx + l, by + boxH), wCol, 2.5f);
                                    draw->AddLine(ImVec2(bx + boxW - l, by + boxH), ImVec2(bx + boxW, by + boxH), wCol, 2.5f);
                                    draw->AddLine(ImVec2(bx + boxW, by + boxH - t), ImVec2(bx + boxW, by + boxH), wCol, 2.5f);
                                }
                                else if (g_EspBox && g_EspBoxStyle == 1) {
                                    draw->AddRect(ImVec2(bx, by), ImVec2(bx + boxW, by + boxH), IM_COL32(255, 255, 255, 255), 0, 0, 1.5f);
                                }

                                // 4. Dibujar barra de vida verde grueso sólida
                                if (g_EspHealth) {
                                    float barW = 5.0f;
                                    float barX = bx + boxW + 6.0f;
                                    float barY = by;
                                    draw->AddRectFilled(ImVec2(barX - 1.0f, barY - 1.0f), ImVec2(barX + barW + 1.0f, barY + boxH + 1.0f), IM_COL32(0, 0, 0, 180), 1.0f);
                                    draw->AddRectFilled(ImVec2(barX, barY), ImVec2(barX + barW, barY + boxH), IM_COL32(0, 255, 120, 255), 1.0f);
                                }

                                // 5. Dibujar Info Superior (Corona + Nombre + Distancia)
                                if (g_EspName || g_EspDistance) {
                                    float crownY = by - 24.0f;
                                    ImU32 goldCol = IM_COL32(255, 215, 0, 255);
                                    ImU32 goldLight = IM_COL32(255, 245, 150, 255);
                                    ImU32 purpleCol = IM_COL32(180, 0, 255, 255);

                                    ImVec2 p0(cx - 7, crownY + 3);
                                    ImVec2 p1(cx - 7, crownY - 3);
                                    ImVec2 p2(cx - 3, crownY + 1);
                                    ImVec2 p3(cx, crownY - 7);
                                    ImVec2 p4(cx + 3, crownY + 1);
                                    ImVec2 p5(cx + 7, crownY - 3);
                                    ImVec2 p6(cx + 7, crownY + 3);

                                    draw->PathClear();
                                    draw->PathLineTo(p0); draw->PathLineTo(p1); draw->PathLineTo(p2);
                                    draw->PathLineTo(p3); draw->PathLineTo(p4); draw->PathLineTo(p5);
                                    draw->PathLineTo(p6);
                                    draw->PathFillConvex(goldCol);

                                    draw->AddCircleFilled(p1, 1.0f, goldLight);
                                    draw->AddCircleFilled(p3, 1.5f, goldLight);
                                    draw->AddCircleFilled(p5, 1.0f, goldLight);
                                    draw->AddCircleFilled(ImVec2(cx, crownY + 1), 1.8f, purpleCol);

                                    float textY = by - 14.0f;
                                    if (g_EspName) {
                                        ImVec2 nameSz = ImGui::CalcTextSize("Name");
                                        draw->AddText(ImVec2(cx - nameSz.x * 0.5f, textY), IM_COL32(255, 255, 255, 255), "Name");
                                        textY += nameSz.y + 0.5f;
                                    }
                                    if (g_EspDistance) {
                                        ImVec2 distSz = ImGui::CalcTextSize("10M");
                                        draw->AddText(ImVec2(cx - distSz.x * 0.5f, textY), IM_COL32(255, 255, 255, 255), "10M");
                                    }
                                }

                                // 6. Dibujar Info Inferior (Nombre de arma + Silueta)
                                float weaponY = by + boxH + 6.0f;
                                if (g_EspWeaponText) {
                                    ImVec2 nameSz = ImGui::CalcTextSize("AK47");
                                    draw->AddText(ImVec2(cx - nameSz.x * 0.5f, weaponY), IM_COL32(255, 255, 255, 255), "AK47");
                                    weaponY += nameSz.y + 1.0f;
                                }
                                if (g_EspWeaponIcon && font2::WeaponsIco) {
                                    const char* akIcon = zGetWIcon(2);
                                    ImGui::PushFont(font2::WeaponsIco);
                                    ImVec2 iconSz = ImGui::CalcTextSize(akIcon);
                                    ImVec2 iconPos(cx - iconSz.x * 0.5f, weaponY);
                                    draw->AddText(font2::WeaponsIco, 20.0f, iconPos, IM_COL32(255, 255, 255, 255), akIcon);
                                    ImGui::PopFont();
                                }
                            }
                            EndGroupBox();
                        }



                        if (active_tab == 6)
                        {
                            BeginGroupBox("Settings", nullptr, "tab1", ImVec2(240, 80), ImVec2(340, 520), &toggleSettings);
                            {
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Stream Mode", &EspWeaponsTex);

                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Show Keybinds", &g_ShowKeybinds);
                                
                                ImGui::SetCursorPosX(10);
                                ImGui::Checkbox2("Show FPS Counter", &g_ShowFps);
                            }
                            EndGroupBox();

                            BeginGroupBox("Performance & Optimizer", nullptr, "tab2", ImVec2(600, 80), ImVec2(360, 520), &toggleUI);
                            {
                                ImGui::SetCursorPosX(10);
                                ImGui::ColorEdit4("Accent Color", (float*)&color_edit4, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar);
                                
                                ImGui::SetCursorPosX(10);
                                static bool g_NotificationSounds = true;
                                ImGui::Checkbox2("Notification Sounds", &g_NotificationSounds);
                                
                                ImGui::Spacing();
                                ImGui::Separator();
                                ImGui::Spacing();
                                
                                ImGui::Spacing();
                                ImGui::SetCursorPosX(10);
                                ImVec2 btnSize(ImGui::GetContentRegionAvail().x - 20.0f, 45.0f);
                                
                                // Color animado pulsante estilo Cyber
                                float time = (float)ImGui::GetTime();
                                float pulse = (sinf(time * 4.0f) + 1.0f) * 0.5f; // Rango 0.0 a 1.0
                                
                                ImVec4 colorNormal = ImVec4(0.0f + pulse * 0.2f, 0.4f + pulse * 0.3f, 0.9f, 0.8f);
                                ImVec4 colorHover = ImVec4(0.1f, 0.8f, 1.0f, 1.0f);
                                ImVec4 colorActive = ImVec4(0.0f, 0.3f, 0.8f, 1.0f);
                                
                                ImGui::PushStyleColor(ImGuiCol_Button, colorNormal);
                                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, colorHover);
                                ImGui::PushStyleColor(ImGuiCol_ButtonActive, colorActive);
                                ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
                                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.5f);
                                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 1.0f, 1.0f, 0.5f));
                                
                                // Texto personalizado y animado
                                if (ImGui::Button(">> ACCELERATE & OPTIMIZE SYSTEM <<", btnSize)) {
                                    OptimizeGameAndBoostFPS();
                                    Notifications::Add("SYSTEM OPTIMIZER", "System boosted: RAM cleared, CPU prioritized, Android GC forced!", 5.0f);
                                }
                                
                                ImGui::PopStyleVar(2);
                                ImGui::PopStyleColor(4);
                            }
                            EndGroupBox();
                        }

                    }ImGui::PopStyleVar();




                }ImGui::End();

        }

        }

        Notifications::Draw();

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
    // Stop all active background threads to avoid executing freed memory
    //SilentAimLoop::StopLoop();
    //Fakelag::ForceOff();

    // Disable hooks and clean up MinHook first to prevent emulator crash when the DLL is unloaded
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    if (g_hinstDLL)
    {
        FreeLibraryAndExitThread(g_hinstDLL, 0);
    }

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
        g_hinstDLL = hinstDLL;


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
            CloseHandle(hCurrentUIThread);
            hCurrentUIThread = nullptr;
        }

        if (hThread1) {
            CloseHandle(hThread1);
            hThread1 = nullptr;
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
