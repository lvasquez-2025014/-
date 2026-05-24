
#include "main.h"


//using namespace KeyAuth;

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

// Función helper para obtener nombre de tecla
const char* GetKeyName(int vkCode)
{
    if (vkCode == 0) return "None";
    if (vkCode == VK_LBUTTON) return "Mouse Left";
    if (vkCode == VK_RBUTTON) return "Mouse Right";
    if (vkCode == VK_MBUTTON) return "Mouse Middle";
    if (vkCode == VK_XBUTTON1) return "Mouse X1";
    if (vkCode == VK_XBUTTON2) return "Mouse X2";
    if (vkCode == VK_SHIFT) return "Shift";
    if (vkCode == VK_CONTROL) return "Ctrl";
    if (vkCode == VK_MENU) return "Alt";
    if (vkCode >= 0x30 && vkCode <= 0x39) {
        static char buf[8];
        sprintf(buf, "%d", vkCode - 0x30);
        return buf;
    }
    if (vkCode >= 0x41 && vkCode <= 0x5A) {
        static char buf[8];
        sprintf(buf, "%c", vkCode);
        return buf;
    }
    if (vkCode >= VK_F1 && vkCode <= VK_F24) {
        static char buf[8];
        sprintf(buf, "F%d", vkCode - VK_F1 + 1);
        return buf;
    }
    if (vkCode == VK_SPACE) return "Space";
    if (vkCode == VK_RETURN) return "Enter";
    if (vkCode == VK_ESCAPE) return "ESC";
    if (vkCode == VK_TAB) return "Tab";
    if (vkCode == VK_BACK) return "Backspace";
    if (vkCode == VK_INSERT) return "Insert";
    if (vkCode == VK_DELETE) return "Delete";
    if (vkCode == VK_HOME) return "Home";
    if (vkCode == VK_END) return "End";
    if (vkCode == VK_PRIOR) return "Page Up";
    if (vkCode == VK_NEXT) return "Page Down";
    static char buf[16];
    sprintf(buf, "Key %d", vkCode);
    return buf;
}

// Función para botón de captura de hotkey
bool HotkeyButton(const char* label, int* hotkey, ImVec2 size = ImVec2(120, 0))
{
    ImGui::PushID(label);
    static bool isCapturing = false;
    static const char* capturingLabel = nullptr;
    static int* capturingHotkey = nullptr;
    
    bool clicked = false;
    
    // Verificar si estamos capturando para este botón
    bool isThisCapturing = (isCapturing && capturingLabel == label && capturingHotkey == hotkey);
    
    ImGui::PushStyleColor(ImGuiCol_Button, isThisCapturing ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.1f, 0.1f, 0.15f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.15f, 0.15f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.25f, 1.0f));
    
    const char* buttonText = isThisCapturing ? "Press Key..." : GetKeyName(*hotkey);
    if (ImGui::Button(buttonText, size)) {
        clicked = true;
        if (!isCapturing) {
            isCapturing = true;
            capturingLabel = label;
            capturingHotkey = hotkey;
        }
    }
    
    ImGui::PopStyleColor(3);
    
    // Capturar tecla si estamos en modo captura
    if (isCapturing && capturingLabel == label && capturingHotkey == hotkey) {
        for (int key = 1; key < 255; key++) {
            if (GetAsyncKeyState(key) & 0x8000) {
                if (key == VK_ESCAPE) {
                    *hotkey = 0; // None
                } else {
                    *hotkey = key;
                }
                isCapturing = false;
                capturingLabel = nullptr;
                capturingHotkey = nullptr;
                ImGui::ClearActiveID();
                break;
            }
        }
    }
    
    ImGui::PopID();
    return clicked;
}

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
    
    // 5. Texto centrado con efecto de sombra
    ImVec2 text_size = ImGui::CalcTextSize(label);
    ImVec2 text_pos = ImVec2(center.x - text_size.x * 0.5f, center.y - text_size.y * 0.5f);
    
    // Sombra del texto
    draw_list->AddText(ImVec2(text_pos.x + 2, text_pos.y + 2), ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.5f)), label);
    // Texto principal
    draw_list->AddText(text_pos, ImGui::GetColorU32(textColor), label);
}



void SetupImGuiStyle()
{
    // Dark Modern Style - Based on reference image
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 0.95f;
    style.DisabledAlpha = 0.5f;
    style.WindowPadding = ImVec2(12.0f, 12.0f);
    style.WindowRounding = 8.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Right;
    style.ChildRounding = 6.0f;
    style.ChildBorderSize = 0.0f;
    style.PopupRounding = 6.0f;
    style.PopupBorderSize = 0.0f;
    style.FramePadding = ImVec2(12.0f, 6.0f);
    style.FrameRounding = 4.0f;
    style.FrameBorderSize = 0.0f;
    style.ItemSpacing = ImVec2(12.0f, 8.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 8.0f);
    style.CellPadding = ImVec2(8.0f, 4.0f);
    style.IndentSpacing = 25.0f;
    style.ColumnsMinSpacing = 8.0f;
    style.ScrollbarSize = 12.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabMinSize = 12.0f;
    style.GrabRounding = 4.0f;
    style.TabRounding = 6.0f;
    style.TabBorderSize = 0.0f;
   
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.5f);

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.10f, 0.95f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.06f, 0.06f, 0.08f, 0.90f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.10f, 0.10f, 0.12f, 0.98f);
    colors[ImGuiCol_Border] = ImVec4(0.20f, 0.20f, 0.25f, 0.30f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.10f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.10f, 0.80f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.07f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.35f, 0.35f, 0.40f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.40f, 0.40f, 0.45f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.30f, 0.30f, 0.35f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.20f, 0.20f, 0.25f, 0.40f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.30f, 0.35f, 0.50f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.40f, 0.45f, 0.70f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.20f, 0.20f, 0.25f, 0.30f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.30f, 0.30f, 0.35f, 0.50f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.40f, 0.45f, 0.70f);
    colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.15f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.22f, 0.28f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.10f, 0.10f, 0.12f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.40f, 0.80f, 1.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.15f, 0.15f, 0.18f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.25f, 0.25f, 0.30f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.20f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.06f, 0.06f, 0.08f, 1.00f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.30f, 0.70f, 0.90f, 0.60f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.30f, 0.70f, 0.90f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.30f, 0.70f, 0.90f, 0.50f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.60f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.70f);
}

// Main code
int maindll()
{

   // KeyAuthApp.init();
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

        static int keyExit = VK_DELETE;

        if (GetAsyncKeyState(keyExit) & 1) {
            // Cerrar panel y emulador limpiamente
            open = false;
            Main_Window = false;
            
            // Cerrar emulador HD-Player
            DWORD procId = GetProcZ("HD-Player.exe");
            if (procId != 0) {
                KillProcZ(procId);
            }
            
            // Cerrar procesos ADB
            DWORD ProcIdAdb = GetProcZ("adb.exe");
            if (ProcIdAdb != 0) {
                KillProcZ(ProcIdAdb);
            }
            DWORD ProcIdAdbHD = GetProcZ("HD-Adb.exe");
            if (ProcIdAdbHD != 0) {
                KillProcZ(ProcIdAdbHD);
            }
            
            // Cerrar ventana de ImGui
            PostQuitMessage(0);
        }

        static int esp_performance_selected = 0;
        static const char* esp_performance_options[] = { "Simple", "Full", "None" };
        static int esp_lines_selected = 0;
        static const char* esp_lines_options[] = { "Bottom", "Top", "Center", "None" };
        static int esp_box_selected = 0;
        static const char* esp_box_options[] = { "Normal", "Filled", "Corners", "None" };
        static int esp_info_selected = 0;
        static const char* esp_info_options[] = { "Name", "Distance", "Health", "Weapon", "None" };
        static int esp_weapon_selected = 0;
        static const char* esp_weapon_options[] = { "Icon", "Text", "Both", "None" };
        static int esp_extra_selected = 0;
        static const char* esp_extra_options[] = { "Skeleton", "Snaplines", "None" };
        static int distance_esp = 0;
        static bool fast_activation = false;
        static bool rainbow_esp_rgb = false;
        static const char* aim_key_options[] = { "Left Click", "Right Click", "Middle Click", "X1", "X2" };

        static bool stream_mode = false;
        static bool show_keybinds = false;
        static bool show_fps_counter = false;
        static ImVec4 accent_color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // Yellow from screenshot
        static bool notification_sounds = false;


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
                
                ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 400) * 0.5f, (ImGui::GetIO().DisplaySize.y - 300) * 0.5f), ImGuiCond_FirstUseEver);
                if (ImGui::Begin("Login", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
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
                ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 400) * 0.5f, (ImGui::GetIO().DisplaySize.y - 300) * 0.5f), ImGuiCond_FirstUseEver);
                if (ImGui::Begin("Login", &open, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize))
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
                    }


                }
                ImGui::End();

            }



            if (authenticed)
            {

                


                ImGui::SetNextWindowPos(ImVec2((ImGui::GetIO().DisplaySize.x - 1100) * 0.5f, (ImGui::GetIO().DisplaySize.y - 700) * 0.5f), ImGuiCond_FirstUseEver);
                ImGui::SetNextWindowSize(ImVec2(1100, 700), ImGuiCond_FirstUseEver);
                ImGui::SetNextWindowBgAlpha(0.85f);
                if (ImGui::Begin("panel principal", &open, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
                {

                    auto draw = ImGui::GetWindowDrawList();
                    auto background_list1 = ImGui::GetBackgroundDrawList();
                    ImVec2 pos = ImGui::GetWindowPos();
                    ImVec2 size = ImGui::GetWindowSize();
                    float splitX = size.x * 0.09f;
                    float WIDTH3 = 1100;
                    float HEIGHT3 = 700;
                    ImVec2 top_left1 = pos;
                    ImVec2 bottom_right1 = ImVec2(pos.x + WIDTH3, pos.y + HEIGHT3);
                    float rounding13313 = c::rounding;

                    ImColor themeBorder = ImColor(0, 200, 255, 150);
                    ImColor textCyan = ImColor(0, 220, 255, 255);

                    draw->AddRectFilled(pos, ImVec2(pos.x + splitX + 1.0f, pos.y + size.y), ImColor(10, 10, 14, 160), 8, ImDrawFlags_RoundCornersLeft);
                    draw->AddRectFilled(ImVec2(pos.x + splitX, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), ImColor(10, 10, 15, 140), 8, ImDrawFlags_RoundCornersRight);
                    ImGui::GetBackgroundDrawList()->AddRect(pos, pos + ImVec2(1100, 700), themeBorder, 6, 0, 1.5f);

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

                    tab_alpha = ImLerp(tab_alpha, (page == active_tab) ? 1.f : 0.f, 15.f * ImGui::GetIO().DeltaTime);
                    if (tab_alpha < 0.01f && tab_add < 0.01f)
                        active_tab = page;
                    draw->AddRectFilled(ImVec2(pos.x + splitX, pos.y), ImVec2(pos.x + size.x, pos.y + size.y), ImColor(30, 30, 35, 180), 4, ImDrawFlags_RoundCornersRight);
                    ImGui::GetBackgroundDrawList()->AddRect(pos, pos + ImVec2(800, 500), ImColor(color_edit4), 4, 0, 1.0f);


                    ImVec2 size44 = ImVec2(66, 66);  // ? tama�o cuadrado para mejor calidad
                    ImGui::SetCursorPos(ImVec2(
                        (splitX - size44.x) * 1.5f,  // ? centrado horizontalmente en el panel izquierdo
                        25.0f                          // ? margen superior
                    ));
                    ImVec2 pos44 = ImGui::GetCursorScreenPos();
                    ImVec2 center2 = ImVec2(
                        pos44.x + size44.x * 0.5f,
                        pos44.y + size44.y * 0.5f
                    );
                    float radius = size44.x * 0.5f;


                    AddShadowCircle(draw, center2, radius, ImColor(color_edit4), 1.0f, ImVec2(0, 0));

                    // Imagen con maxima calidad (UV completo 0,0 -> 1,1)
                    draw->AddImageRounded(
                        logo,
                        pos44,
                        ImVec2(pos44.x + size44.x, pos44.y + size44.y),
                        ImVec2(0, 0), ImVec2(1, 1),
                        IM_COL32(255, 255, 255, 255),
                        radius  // ? borde circular para mejor presentacion
                    );
                    ImGui::Dummy(size44);


                    ImGui::PushFont(Nev1);
                    ImVec2 text_size = ImGui::CalcTextSize("Asmodeus");
                    ImVec2 text_pos = ImVec2(
                        pos.x + (size.x - text_size.x) * 0.5f,
                        pos.y + (65.0f - text_size.y) * 0.5f + 10.0f  // ? alineado con el logo
                    );
                    ShadowText(draw, "Asmodeus", text_pos, ImColor(color_edit4), ImColor(color_edit4), 0.0f);
                    ImGui::PopFont();

                    ImVec2 glow_center = ImVec2(pos.x + size.x - 400, pos.y + size.y + 170);
                    AddShadowRect(ImGui::GetWindowDrawList(), glow_center, glow_center, ImColor(color_edit4), 280.f, ImVec2(0, 0));
                    // Dummy para el texto
                    ImGui::SetCursorPos(ImVec2((size.x - text_size.x) * 0.5f, 10));
                    ImGui::Dummy(text_size);


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

                    // Left Sidebar UI (Matches version vieja aesthetic)
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
                            SidebarButton("Esp", (void*)image2::gless, 1);
                            SidebarButton("Chams", (void*)image2::help, 2);
                            SidebarButton("KeyBind", (void*)image2::panel, 3);
                            SidebarButton("FixLag", (void*)image2::other, 4);
                            SidebarButton("Enemies", (void*)image2::save, 5);
                            SidebarButton("Extras", (void*)image2::settings, 6);
                            SidebarButton("Config.", (void*)image2::settings, 7);
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

                            // Transparencia igual al panel principal del login (alpha 160 fondo, 140 borde)
                            draw->AddRectFilled(p1, p2, IM_COL32(10, 10, 15, 160), 8.f);
                            draw->AddRect(p1, p2, IM_COL32(40, 40, 40, 140), 8.f, 0, 1.0f);

                            // Banner section top (transparent alpha 160)
                            draw->AddRectFilledMultiColor(p1, ImVec2(p2.x, p1.y + 40), IM_COL32(40, 40, 45, 160), IM_COL32(15, 15, 18, 160), IM_COL32(15, 15, 18, 160), IM_COL32(40, 40, 45, 160));

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
                                static bool prev_adbboton = false;
                                if (ImGui::Checkbox2("INICIAR ADB", &adbboton)) {
                                    if (adbboton && !prev_adbboton) {
                                        Notifications::Add("ASMODEUS", "Iniciando ADB...", 3.0f);
                                    }
                                    if (adbboton && !adbInitialized) { 
                                        initOffsets(selected_game_version);
                                        std::thread([]() {
                                            hdPlayerWindow = INJECTESPADB();
                                            while (!adbInitialized) std::this_thread::sleep_for(std::chrono::milliseconds(100));
                                            EnabledEsp = true;
                                            Notifications::Add("ASMODEUS", "ADB Inyectado exitosamente!", 3.0f);
                                        }).detach();
                                    }
                                }
                                prev_adbboton = adbboton;

                                if (adbInitialized) {
                                    ImGui::Spacing();
                                    ImGui::Separator();
                                    ImGui::Spacing();
                                }

                                  ImGui::SetCursorPosX(10);
                                  // Mostrar estado actual (solo lectura, activación por hotkey)
                                  const char* statusText = g_AimAssist ? "Status: Active" : "Status: Inactive";
                                  ImVec4 statusColor = g_AimAssist ? ImVec4(0.2f, 0.8f, 0.2f, 1.0f) : ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
                                  ImGui::TextColored(statusColor, statusText);
                                  ImGui::SameLine();
                                  ImGui::Text("(Use hotkey to toggle)");

                                  // Hotkey siempre visible (fuera del bloque if)
                                  ImGui::SetCursorPosX(10);
                                  ImGui::AlignTextToFramePadding();
                                  ImGui::Text("Aim Assist Key");
                                  ImGui::SameLine();
                                  HotkeyButton("##aim_hotkey", &AimHotkey, ImVec2(120, 0));

                                  if (g_AimAssist) {
                                      ImGui::SetCursorPosX(10);
                                      static bool prev_g_AimTeammates = false;
                                      if (ImGui::Checkbox2("Aim Teammates (Bots)", &g_AimTeammates)) {
                                          if (g_AimTeammates && !prev_g_AimTeammates) {
                                              Notifications::Add("ASMODEUS", "Aim Teammates Activado", 3.0f);
                                          } else if (!g_AimTeammates && prev_g_AimTeammates) {
                                              Notifications::Add("ASMODEUS", "Aim Teammates Desactivado", 3.0f);
                                          }
                                      }
                                      prev_g_AimTeammates = g_AimTeammates;
                                      ImGui::SetCursorPosX(10);
                                      ImGui::PushItemWidth(120);
                                      const char* AimTypeNames[] = { "None", "Memory Lock", "Memory Smooth", "Mouse Emulation", "Silent Aim" };
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
                                      ImGui::Combo("Target Bone", &SelectedAimBone, AimBoneOptions, IM_ARRAYSIZE(AimBoneOptions), 180);
                                      ImGui::PopItemWidth();

                                      ImGui::SetCursorPosX(10);
                                      ImGui::PushItemWidth(120);
                                      ImGui::SliderInt("Smooth Shots (Chest)", &g_ChestShotsCount, 0, 5, "%d shots");
                                      ImGui::PopItemWidth();
                                  }

                                  ImGui::SetCursorPosX(10);
                                  static bool prev_ShowFov = false;
                                  if (ImGui::Checkbox2("Show Aim FOV", &ShowFov)) {
                                      if (ShowFov && !prev_ShowFov) {
                                          Notifications::Add("ASMODEUS", "Aim FOV Activado", 3.0f);
                                      } else if (!ShowFov && prev_ShowFov) {
                                          Notifications::Add("ASMODEUS", "Aim FOV Desactivado", 3.0f);
                                      }
                                  }
                                  prev_ShowFov = ShowFov;

                                  if (ShowFov) {
                                      ImGui::SetCursorPosX(10);
                                      ImGui::PushItemWidth(120);
                                      ImGui::SliderInt("Aim Fov Size", &FovAll, 10, 800, "%d px");
                                      ImGui::PopItemWidth();
                                  }

                                  ImGui::SetCursorPosX(10);
                                  static bool prev_NoRecoil = false;
                                  if (ImGui::Checkbox2("No Recoil", &NoRecoil)) {
                                      if (NoRecoil && !prev_NoRecoil) {
                                          Notifications::Add("ASMODEUS", "No Recoil Activado", 3.0f);
                                      } else if (!NoRecoil && prev_NoRecoil) {
                                          Notifications::Add("ASMODEUS", "No Recoil Desactivado", 3.0f);
                                      }
                                  }
                                  prev_NoRecoil = NoRecoil;

                                  // Nuevas funciones desactivadas temporalmente (offsets no inicializados)
                                  /*ImGui::SetCursorPosX(10);
                                  static bool prev_InfiniteAmmo = false;
                                  if (ImGui::Checkbox2("Infinite Ammo", &InfiniteAmmo)) {
                                      if (InfiniteAmmo && !prev_InfiniteAmmo) {
                                          Notifications::Add("ASMODEUS", "Infinite Ammo Activado", 3.0f);
                                      } else if (!InfiniteAmmo && prev_InfiniteAmmo) {
                                          Notifications::Add("ASMODEUS", "Infinite Ammo Desactivado", 3.0f);
                                      }
                                  }
                                  prev_InfiniteAmmo = InfiniteAmmo;

                                  ImGui::SetCursorPosX(10);
                                  static bool prev_FastFire = false;
                                  if (ImGui::Checkbox2("Fast Fire", &FastFire)) {
                                      if (FastFire && !prev_FastFire) {
                                          Notifications::Add("ASMODEUS", "Fast Fire Activado", 3.0f);
                                      } else if (!FastFire && prev_FastFire) {
                                          Notifications::Add("ASMODEUS", "Fast Fire Desactivado", 3.0f);
                                      }
                                  }
                                  prev_FastFire = FastFire;

                                  ImGui::SetCursorPosX(10);
                                  static bool prev_QuickSwitchHack = false;
                                  if (ImGui::Checkbox2("Quick Switch", &QuickSwitchHack)) {
                                      if (QuickSwitchHack && !prev_QuickSwitchHack) {
                                          Notifications::Add("ASMODEUS", "Quick Switch Activado", 3.0f);
                                      } else if (!QuickSwitchHack && prev_QuickSwitchHack) {
                                          Notifications::Add("ASMODEUS", "Quick Switch Desactivado", 3.0f);
                                      }
                                  }
                                  prev_QuickSwitchHack = QuickSwitchHack;

                                  ImGui::SetCursorPosX(10);
                                  static bool prev_MedikitHackNew = false;
                                  if (ImGui::Checkbox2("Medikit Hack", &MedikitHackNew)) {
                                      if (MedikitHackNew && !prev_MedikitHackNew) {
                                          Notifications::Add("ASMODEUS", "Medikit Hack Activado", 3.0f);
                                      } else if (!MedikitHackNew && prev_MedikitHackNew) {
                                          Notifications::Add("ASMODEUS", "Medikit Hack Desactivado", 3.0f);
                                      }
                                  }
                                  prev_MedikitHackNew = MedikitHackNew;*/
                            }
                            EndGroupBox();
                        }

                        if (active_tab == 1)
                        {
                            // Opciones y Sincronizacion de Interfaz de ESP
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
                                espLinesIdx = EspLineZ ? (SelectedESPBoxStyle + 1) : 0;
                                espBoxIdx = ESPBox ? SelectedESPBoxStyle : 0;
                                if (ESPNameZ && ESPHealth) espInfoIdx = 3;
                                else if (ESPNameZ) espInfoIdx = 1;
                                else if (ESPHealth) espInfoIdx = 2;
                                else espInfoIdx = 0;
                                espWeaponIdx = (EspWeaponsICON && EspWeaponsTex) ? 3 : (EspWeaponsICON ? 2 : (EspWeaponsTex ? 1 : 0));
                                espExtraIdx = (ESPBones && ESPHealth) ? 3 : (ESPBones ? 1 : (ESPHealth ? 2 : 0));
                                espDistanceVal = maxDistance;
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
                            BeginGroupBox("Visuals - ESP", nullptr, "tab1", ImVec2(240, 80), ImVec2(340, 520), &EspEnabled);
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
                                static bool prev_EspEnabled = false;
                                if (ImGui::Checkbox2("Fast Activation", &EspEnabled)) {
                                    if (EspEnabled && !prev_EspEnabled) {
                                        Notifications::Add("ASMODEUS", "ESP Activado", 3.0f);
                                        if (espLinesIdx == 0) espLinesIdx = 2; // Center
                                        if (espBoxIdx == 0) espBoxIdx = 1; // Normal
                                        if (espInfoIdx == 0) espInfoIdx = 3; // Name, Distance
                                        if (espWeaponIdx == 0) espWeaponIdx = 3; // Icon, Name
                                        if (espExtraIdx == 0) espExtraIdx = 3; // Skeleton, Health
                                        espDistanceVal = 1500;
                                    } else if (!EspEnabled && prev_EspEnabled) {
                                        Notifications::Add("ASMODEUS", "ESP Desactivado", 3.0f);
                                        espLinesIdx = 0;
                                        espBoxIdx = 0;
                                        espInfoIdx = 0;
                                        espWeaponIdx = 0;
                                        espExtraIdx = 0;
                                    }
                                }
                                prev_EspEnabled = EspEnabled;

                                ImGui::SetCursorPosX(10);
                                static bool prev_rainbowMode = false;
                                if (ImGui::Checkbox2("Rainbow ESP (RGB)", &rainbowMode)) {
                                    if (rainbowMode && !prev_rainbowMode) {
                                        Notifications::Add("ASMODEUS", "Rainbow ESP Activado", 3.0f);
                                    } else if (!rainbowMode && prev_rainbowMode) {
                                        Notifications::Add("ASMODEUS", "Rainbow ESP Desactivado", 3.0f);
                                    }
                                }
                                prev_rainbowMode = rainbowMode;

                                ImGui::Spacing();
                                ImGui::SetCursorPosX(10);
                                static bool prev_ESPShield = false;
                                if (ImGui::Checkbox2("ESP Shield", &ESPShield)) {
                                    if (ESPShield && !prev_ESPShield) {
                                        Notifications::Add("ASMODEUS", "ESP Shield Activado", 3.0f);
                                    } else if (!ESPShield && prev_ESPShield) {
                                        Notifications::Add("ASMODEUS", "ESP Shield Desactivado", 3.0f);
                                    }
                                }
                                prev_ESPShield = ESPShield;

                                ImGui::SetCursorPosX(10);
                                static bool prev_ESPDistance = false;
                                if (ImGui::Checkbox2("ESP Distance", &ESPDistance)) {
                                    if (ESPDistance && !prev_ESPDistance) {
                                        Notifications::Add("ASMODEUS", "ESP Distance Activado", 3.0f);
                                    } else if (!ESPDistance && prev_ESPDistance) {
                                        Notifications::Add("ASMODEUS", "ESP Distance Desactivado", 3.0f);
                                    }
                                }
                                prev_ESPDistance = ESPDistance;

                                ImGui::Spacing();
                                ImGui::SetCursorPosX(10);
                                static bool prev_SpeedHackEnabled = false;
                                if (ImGui::Checkbox2("Speed Hack", &SpeedHackEnabled)) {
                                    if (SpeedHackEnabled && !prev_SpeedHackEnabled) {
                                        Notifications::Add("ASMODEUS", "Speed Hack Activado", 3.0f);
                                    } else if (!SpeedHackEnabled && prev_SpeedHackEnabled) {
                                        Notifications::Add("ASMODEUS", "Speed Hack Desactivado", 3.0f);
                                    }
                                }
                                prev_SpeedHackEnabled = SpeedHackEnabled;

                                ImGui::SetCursorPosX(10);
                                ImGui::AlignTextToFramePadding();
                                ImGui::Text("Speed Toggle Key");
                                ImGui::SameLine();
                                HotkeyButton("##speed_hotkey", &SpeedHotkey, ImVec2(120, 0));

                                if (SpeedHackEnabled) {
                                    ImGui::SetCursorPosX(10);
                                    ImGui::AlignTextToFramePadding();
                                    ImGui::Text("Speed Multiplier");
                                    ImGui::SameLine();
                                    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 60.0f);
                                    char speedText[32];
                                    sprintf(speedText, "x%.1f", SpeedHackMultiplier);
                                    ImGui::Text(speedText);

                                    ImGui::SetCursorPosX(10);
                                    ImGui::PushItemWidth(ImGui::GetWindowWidth() - 20.0f);
                                    ImGui::SliderFloat("##speed_multiplier", &SpeedHackMultiplier, 1.0f, 1.7f, "");
                                    ImGui::PopItemWidth();
                                }
                            }
                            EndGroupBox();

                            // Aplicar cambios visuales a variables reales de ESP en tiempo real
                            EspLineZ = (espLinesIdx > 0);
                            if (EspLineZ) SelectedLineStyle = espLinesIdx;

                            ESPBox = (espBoxIdx > 0);
                            if (ESPBox) SelectedESPBoxStyle = espBoxIdx;

                            if (espInfoIdx == 0) { ESPNameZ = false; ESPHealth = false; }
                            else if (espInfoIdx == 1) { ESPNameZ = true; ESPHealth = false; }
                            else if (espInfoIdx == 2) { ESPNameZ = false; ESPHealth = true; }
                            else if (espInfoIdx == 3) { ESPNameZ = true; ESPHealth = true; }
                            else if (espInfoIdx == 4) { ESPNameZ = true; ESPHealth = false; }
                            else if (espInfoIdx == 5) { ESPNameZ = true; ESPHealth = true; }

                            if (espWeaponIdx == 0) { EspWeaponsTex = false; EspWeaponsICON = false; }
                            else if (espWeaponIdx == 1) { EspWeaponsTex = true; EspWeaponsICON = false; }
                            else if (espWeaponIdx == 2) { EspWeaponsTex = false; EspWeaponsICON = true; }
                            else if (espWeaponIdx == 3) { EspWeaponsTex = true; EspWeaponsICON = true; }

                            if (espExtraIdx == 0) { ESPBones = false; ESPHealth = false; }
                            else if (espExtraIdx == 1) { ESPBones = true; ESPHealth = false; }
                            else if (espExtraIdx == 2) { ESPBones = false; ESPHealth = true; }
                            else if (espExtraIdx == 3) { ESPBones = true; ESPHealth = true; }

                            maxDistance = espDistanceVal;

                            // --- COLUMNA DERECHA: ESP EDITOR PREVIEW ---
                            BeginGroupBox("ESP Editor", nullptr, "tab2", ImVec2(600, 80), ImVec2(360, 520), &EspEnabled);
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
                                if (ESPBones) {
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
                                if (ESPBox && SelectedESPBoxStyle == 3) {
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
                                else if (ESPBox && SelectedESPBoxStyle == 1) {
                                    draw->AddRect(ImVec2(bx, by), ImVec2(bx + boxW, by + boxH), IM_COL32(255, 255, 255, 255), 0, 0, 1.5f);
                                }

                                // 4. Dibujar barra de vida verde grueso sólida
                                if (ESPHealth) {
                                    float barW = 5.0f;
                                    float barX = bx + boxW + 6.0f;
                                    float barY = by;
                                    draw->AddRectFilled(ImVec2(barX - 1.0f, barY - 1.0f), ImVec2(barX + barW + 1.0f, barY + boxH + 1.0f), IM_COL32(0, 0, 0, 180), 1.0f);
                                    draw->AddRectFilled(ImVec2(barX, barY), ImVec2(barX + barW, barY + boxH), IM_COL32(0, 255, 120, 255), 1.0f);
                                }

                                // 5. Dibujar Info Superior (Corona + Nombre + Distancia)
                                if (ESPNameZ || ESPHealth) {
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
                                    if (ESPNameZ) {
                                        ImVec2 nameSz = ImGui::CalcTextSize("Name");
                                        draw->AddText(ImVec2(cx - nameSz.x * 0.5f, textY), IM_COL32(255, 255, 255, 255), "Name");
                                        textY += nameSz.y + 0.5f;
                                    }
                                    if (ESPHealth) {
                                        ImVec2 distSz = ImGui::CalcTextSize("10M");
                                        draw->AddText(ImVec2(cx - distSz.x * 0.5f, textY), IM_COL32(255, 255, 255, 255), "10M");
                                    }
                                }

                                // 6. Dibujar Info Inferior (Nombre de arma + Silueta)
                                float weaponY = by + boxH + 6.0f;
                                if (EspWeaponsTex) {
                                    ImVec2 nameSz = ImGui::CalcTextSize("AK47");
                                    draw->AddText(ImVec2(cx - nameSz.x * 0.5f, weaponY), IM_COL32(255, 255, 255, 255), "AK47");
                                    weaponY += nameSz.y + 1.0f;
                                }
                                if (EspWeaponsICON && font2::WeaponsIco) {
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
                        if (active_tab == 2)
                        {
                            BeginGroupBox("Chams Settings", nullptr, "tab1", ImVec2(240, 80), ImVec2(720, 520), &toggleChams);
                            {
                                ImGui::SetCursorPosX(10);
                                ImGui::Text("Chams features coming soon...");
                            }
                            EndGroupBox();
                        }
                        if (active_tab == 3)
                        {
                            BeginGroupBox("KeyBind Settings", nullptr, "tab1", ImVec2(240, 80), ImVec2(720, 520), &toggleFunc);
                            {
                                ImGui::SetCursorPosX(10);
                                static bool prev_UpCheck = false;
                                if (ImGui::Checkbox2("Up - Player", &UpCheck)) {
                                    if (UpCheck && !prev_UpCheck) {
                                        Notifications::Add("ASMODEUS", "Up - Player Activado", 3.0f);
                                    } else if (!UpCheck && prev_UpCheck) {
                                        Notifications::Add("ASMODEUS", "Up - Player Desactivado", 3.0f);
                                    }
                                }
                                prev_UpCheck = UpCheck;
                                ImGui::SetCursorPosX(10);
                                ImGui::Text("Pull Bone");
                                ImGui::SameLine(200);
                                ImGui::SetNextItemWidth(150);
                                ImGui::Combo("##pull_bone", &SelectedPullBone, PullBoneOptions, IM_ARRAYSIZE(PullBoneOptions));
                            }
                            EndGroupBox();
                        }
                        if (active_tab == 4)
                        {
                            BeginGroupBox("FixLag Settings", nullptr, "tab1", ImVec2(240, 80), ImVec2(720, 520), &toggleLag);
                            {
                                ImGui::SetCursorPosX(10);
                                ImGui::Text("FixLag features coming soon...");
                            }
                            EndGroupBox();
                        }
                        if (active_tab == 5)
                        {
                            BeginGroupBox("Enemies Settings", nullptr, "tab1", ImVec2(240, 80), ImVec2(720, 520), &toggleLagAdv);
                            {
                                ImGui::SetCursorPosX(10);
                                ImGui::Text("Enemies features coming soon...");
                            }
                            EndGroupBox();
                        }
                        if (active_tab == 7)
                        {
                            BeginGroupBox("Settings", nullptr, "tab1", ImVec2(240, 80), ImVec2(340, 520), &toggleSettings);
                            {
                                ImGui::SetCursorPosX(10);
                                static bool prev_EspWeaponsTex = false;
                                if (ImGui::Checkbox2("Stream Mode", &EspWeaponsTex)) {
                                    if (EspWeaponsTex && !prev_EspWeaponsTex) {
                                        Notifications::Add("ASMODEUS", "Stream Mode Activado", 3.0f);
                                    } else if (!EspWeaponsTex && prev_EspWeaponsTex) {
                                        Notifications::Add("ASMODEUS", "Stream Mode Desactivado", 3.0f);
                                    }
                                }
                                prev_EspWeaponsTex = EspWeaponsTex;

                                ImGui::SetCursorPosX(10);
                                static bool prev_g_ShowKeybinds = false;
                                if (ImGui::Checkbox2("Show Keybinds", &g_ShowKeybinds)) {
                                    if (g_ShowKeybinds && !prev_g_ShowKeybinds) {
                                        Notifications::Add("ASMODEUS", "Show Keybinds Activado", 3.0f);
                                    } else if (!g_ShowKeybinds && prev_g_ShowKeybinds) {
                                        Notifications::Add("ASMODEUS", "Show Keybinds Desactivado", 3.0f);
                                    }
                                }
                                prev_g_ShowKeybinds = g_ShowKeybinds;

                                ImGui::SetCursorPosX(10);
                                static bool prev_g_ShowFps = false;
                                if (ImGui::Checkbox2("Show FPS Counter", &g_ShowFps)) {
                                    if (g_ShowFps && !prev_g_ShowFps) {
                                        Notifications::Add("ASMODEUS", "Show FPS Counter Activado", 3.0f);
                                    } else if (!g_ShowFps && prev_g_ShowFps) {
                                        Notifications::Add("ASMODEUS", "Show FPS Counter Desactivado", 3.0f);
                                    }
                                }
                                prev_g_ShowFps = g_ShowFps;
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
                            }
                            EndGroupBox();
                        }

                    }
                    ImGui::PopStyleVar();




                }ImGui::End();

            }

           
        }


        

        // Rendering
        Notifications::Draw();
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

            // ? Cancela la carga de la DLL
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










