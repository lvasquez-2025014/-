

#include "vector.h"
#include "adb.h"

#include <imgui_internal.h>


double getCurrentTime() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count() / 1000.0;
}

inline ImFont* medium_font[30];
char defaultName[255] = "";
inline ImColor lineaverdexdddd(0, 255, 0);

ImVec4 hongoColor(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 fillBoxColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 namecolor(1.0f, 0.823f, 0.0f, 1.0f);
ImVec4 lineColorEnemy(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 lineColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImU32 invisibleColor = IM_COL32(160, 32, 240, 200);
ImVec4 BoxColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 healthColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 WeaponsColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 Weaponstext = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 bonesColor = ImVec4(255, 182, 193, 255);
ImVec4 bonesColorCIRCULOS = ImVec4(0, 128, 255, 255);
ImVec4 fovColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 crosshairColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 userColorAmarillo = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
inline ImVec4 ColorFov = ImColor(255, 255, 255);
ImVec4 boxColor(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 lineColorTeam(0.0f, 1.0f, 0.0f, 1.0f);

const char* cheat_name = " ";
std::string EspADB = "/cheat";
int g_windowWidth = GetSystemMetrics(SM_CXSCREEN);
int g_windowHeight = GetSystemMetrics(SM_CYSCREEN);
HWND hdPlayerWindow;
static auto clickStartTime = std::chrono::steady_clock::time_point();
bool PullPlayer = false;
bool PullPlayerV2 = false;

// FIX: Reemplazar macro peligrosa con constexpr
constexpr float SMALL_FLOAT = 1e-10f;

inline namespace Main {
    bool EspMode;
    bool EnabledEsp = false;
}
struct PullTarget {
    uint32_t entity;
    Vector3 targetBonePos;
    Vector3 rootPos;
    float crosshairDist;
};
int RadioMagnet = 200.0f;
std::chrono::steady_clock::time_point lastFireTime;
const int fireCooldownMs360 = 100; // Ajusta este valor según necesites
float PullDis = 300.0f; // Distancia máxima para pull
float PullFov = 300.0f; // FOV para el pull
float pullTime = 0.0f;
float suavidad360 = 2.0f; // Factor de suavidad

int VidaType = 0;
const char* items5[] = { "Right", "Left", "Below" };

inline namespace Combox {
    static int bypass_combo = 1;
    const char* bypass_list[] = { "Normal", "Max", "x86" };
}


int SelectedPullBone = 0;
const char* PullBoneOptions[] = { "Head", "Neck" ,"Root" };

bool EnemyPullEnabledHead = false;
bool PuntoESP = false;
bool caidarapida = false;
bool ESPinfoZada = false;
bool ESPinfoZada2 = false;
bool ESPFillBox = false;
bool ESPHongo = false;

static float screenPullRangesilent = 140.0f;
static float PullHeightOffset = -0.05f;
static float screenPullRange = 220.0f;
static float minDistanceToSkip = 0.5f;
static float smoothFactorFull = 0.15f;
static float fire_interval_value = 0.3f;

struct PlayerData {
    uint32_t entity;
    std::string name;
    float distance;
};
std::vector<PlayerData> detectedPlayers;


bool EspEnabled = false;
bool EspBox = false, ESPBones = false, EspLineZ = false, ESPNameZ = false;
bool ShowFov = false, Shadow = false, rainbowMode = false, rgbMode = false;
static bool ShowTeam = false, ShowInvis = false, ShowWukongOrion = true;

// Nuevas opciones de ESP
bool ESPLineTop = false;    // Línea desde la cabeza
bool ESPLineCenter = false; // Línea desde el centro del cuerpo
bool ESPLineBottom = false; // Línea desde los pies
bool ESPCrosshair = false;  // Mira personalizada
bool ESPInfoBox = false;    // Cuadro de información en la parte inferior
int ESPCrosshairStyle = 0;  // Estilo de la mira (0: Cross, 1: Circle, 2: Dot)
float ESPCrosshairSize = 10.0f;  // Tamaño de la mira
ImVec4 ESPCrosshairColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);  // Color de la mira

bool AimbotRotation = false, AimbotRage = false, AimbotRage2 = false, AimbotPlayerX = false, AimPrediction = false;
bool SilentHook, SilentHookG, aimbotHEX, alreadyReplaced = false;
bool NoRecoil = false, InstantFire = false, quickswitch = false, bugmira = false;
bool MedkitHack = false, DamageFake = false, DamageFake2 = false, BugCamara = false, QuickSniper = false;
bool InfiniteBullents = false, MedikitFast = false;

bool PlayerZ = false, UnderPlayerrr = false, FastFall = false;
bool NightMode = false, WallHackOffset = false;
bool IgnoreAlture = true;
bool FlyWall = false, Rapidfall = false, isFrozen = false;
bool ESPinfo = true, ESPBox = false, ESPCercania = false, glowMode = true;
bool Bugmira = false, IsTeam = false, ESPID = false, ESPDisctanc = false;
bool mirabonita = false;
bool EspWeaponsICON = false, EspWeaponsTex = false, EspText = false;
bool UpCheck = false;
bool ESPHealth = false;
bool ESPHealthMAX = false;

// Opciones de ESP y Radar 2D
bool EspBox3D = false;
bool EspRadar2D = false;
float EspRadarRange = 150.0f;
float EspRadarSize = 130.0f;
ImVec2 EspRadarPos = ImVec2(980, 20); // Posición superior derecha por defecto

// Extern keybind variables declared in main.h
extern int KeyFlyWall;
extern int KeyQuickSwitch;
extern bool ShowKeybindsHUD;
extern int neckShotsCount;
extern bool aimSilentEnabled; // Aimbot silent activado
extern bool Main_Window; // Variable del panel principal
extern bool Verified; // Variable de verificación
extern bool isClickable; // Variable de clickabilidad

bool isVisible = false, isDead = false, isKnocked = false, isCurrentlyFiring = false;
bool isFiring = false, QuickSwitchActive = false, BugCamaraActive = false;

static int aimbot_combo = 0; // FIX: Inicializar variable
const char* aimbot_list[] = { "Head", "Neck", "Leggit" };

static int aimbot_bone_combo = 0; // Selector de bone para aimbot
const char* aimbot_bone_list[] = { "Head", "Neck", "Chest", "Hip" };

static int rotation_combo = 0; // FIX: Inicializar variable
const char* rotation_list[] = { "Head", "Neck", "Chest" };

static int silent_combo = 0; // FIX: Inicializar variable
const char* silent_list[] = { "Normal", "360", "Fov Ultra" };


int SelectedESPBoxStyle = 1;
const char* ESPBoxOptions[4] = { "None", "Normal", "Filled", "Corners" };

bool SpringHack = false;


float lineThicknessBase = 0.8f;
float NightIntensity = 0.7f;
static float velocidadcaida = 0.2f;
static float valueFire = 0.75f;
static float valuexde = 10.0f;
static float velocidadmedikit = 0.75f;

int maxDistance = 500;
int FovAll = 150;
int DistanceAll = 500;
int SecondsLag = 1;
int InfinitySliders = 555;


int FovRage = 180; // FIX: Inicializar FovRage correctamente
int DistanceRage = 150;
int delayToHeadMs = 200;
int DelayHead = 300;


int FovRotation = 360;
int DistanceRotation = 150;
int FovHook = 360;
int DistanceHook = 150;


int EnemyDistance = 0;
int EnemyDistanceX = 8;
int EnemySide = 0;
int UpX = 5;


#ifndef BOOL3_H
#define BOOL3_H

enum class Bool3 {
    True,
    False,
    Unknown
};

#endif

// ClipGuard para manejar el recorte en ImGui
struct ClipGuard {
    ImDrawList* a; ImDrawList* b;
    ClipGuard(ImDrawList* a_, ImDrawList* b_, ImVec2 mn, ImVec2 mx) : a(a_), b(b_) {
        if (a) a->PushClipRect(mn, mx, true);
        if (b) b->PushClipRect(mn, mx, true);
    }
    ~ClipGuard() {
        if (b) b->PopClipRect();
        if (a) a->PopClipRect();
    }
};

// Dibuja un polígono con efecto de resplandor (glow) alrededor
static inline void PolyGlow(ImDrawList* dl, const ImVec2* pts, int count, ImU32 color, float strength = 6.0f, int layers = 6) {
    if (!dl || !pts || count <= 1 || layers <= 0 || strength <= 0.0f) return;
    for (int i = 1; i <= layers; ++i) {
        float t = (float)i / (float)layers;
        int a = (int)(255.0f * (0.10f * (1.0f - t)));
        ImU32 c = (color & 0x00FFFFFFu) | ((ImU32)a << 24);
        dl->AddPolyline(pts, count, c, ImDrawFlags_Closed, 1.0f + t * (strength * 0.15f));
    }
}

// Dibuja un rectángulo con efecto de resplandor (glow) alrededor
static inline void DrawGlowRect(ImDrawList* dl, ImVec2 a, ImVec2 b, ImU32 color, float strength = 12.0f, int layers = 8, float rounding = 0.0f)
{
    if (!dl) return;
    if (!(std::isfinite(a.x) && std::isfinite(a.y) && std::isfinite(b.x) && std::isfinite(b.y))) return;
    if (a.x >= b.x || a.y >= b.y || strength <= 0.0f || layers <= 0) return;

    // preserva el alpha de 'color'
    const int base_a = (int)((color >> IM_COL32_A_SHIFT) & 0xFF);

    for (int i = 1; i <= layers; ++i) {
        const float t = (float)i / (float)layers;
        const float inflate = t * strength;
        // desvanecido y respetar alpha base
        int alpha = (int)(base_a * (0.10f * (1.0f - t)));
        ImU32 c = (color & 0x00FFFFFFu) | ((ImU32)alpha << IM_COL32_A_SHIFT);

        // grosor 1.0f, flags modernos (no usar corner-flags legacy)
        dl->AddRect(ImVec2(a.x - inflate, a.y - inflate),
            ImVec2(b.x + inflate, b.y + inflate),
            c,
            rounding,                 // puedes hacer rounding + inflate*0.3f si quieres que crezca
            ImDrawFlags_None,
            1.0f);
    }
}

// Font ESPWeapon
namespace font
{
    inline ImFont* WeaponsIco = nullptr;
    inline ImFont* fuentezada = nullptr;
}




// Leer cadena wide (UTF-16) desde memoria del juego
std::wstring ReadWideString(uintptr_t nameAddr, size_t maxLength = 64) {
    std::wstring result;

    for (size_t i = 0; i < maxLength; i++) {
        uint16_t wchar_value = 0;
        if (!ReadZ(nameAddr + (i * 2), wchar_value)) {
            break;
        }
        if (wchar_value == 0x0000) {
            break;
        }

        result += static_cast<wchar_t>(wchar_value);
    }

    return result;
}

// Convertir UTF-16 (std::wstring) a UTF-8 (std::string)
std::string ConvertUTF16ToUTF8(const std::wstring& wstr) {
    if (wstr.empty()) return "";

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (size_needed <= 0) return "[InvalidName]";

    std::string utf8Str(size_needed - 1, 0);
    if (!WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8Str[0], size_needed, nullptr, nullptr)) {
        return "[InvalidName]";
    }

    return utf8Str;
}

// Convertir cadena ANSI a UTF-8
std::string ANSItoUTF8(const std::string& ansiStr) {
    int wideLen = MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, nullptr, 0);
    std::wstring wideStr(wideLen, 0);
    MultiByteToWideChar(CP_ACP, 0, ansiStr.c_str(), -1, &wideStr[0], wideLen);

    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string utf8Str(utf8Len, 0);
    WideCharToMultiByte(CP_UTF8, 0, wideStr.c_str(), -1, &utf8Str[0], utf8Len, nullptr, nullptr);

    return utf8Str;
}

static Vector3 Normalize(const Vector3& v)
{
    float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (length == 0) return Vector3{ 0,0,0 };
    return Vector3{ v.x / length, v.y / length, v.z / length };
}

// Leer nombre del jugador desde memoria y convertirlo a UTF-8
std::string ReadPlayerName(uintptr_t nameAddr) {
    return ConvertUTF16ToUTF8(ReadWideString(nameAddr));
}

// Obtener ID del arma equipada
uint32_t zGetInfoW(uint32_t player, bool zWOnScan = true)
{
    uint32_t zWdata, zWdata1, zWdata2, zWID;
    if (zWOnScan)
    {
        if (!ReadZ(player + 0x44, zWdata)) return 0;
        if (!ReadZ(zWdata + 0x8, zWdata1)) return 0;
        if (!ReadZ(zWdata1 + 0x20, zWdata2)) return 0;
        if (ReadZ(zWdata2 + 0x10, zWID)) return zWID;
    }
    return 0;
}

static bool s_weapons[7] = { false, false, false, false, false, false ,false, };
static const char* i_weapons[7] = { "AWM", "KAR98K", "M828", "AWM-Y", "M24", "KORD", "VSK94", };

const char* AimKeyNames[] = {
    "Left Mouse",
    "Right Mouse",
    "Middle Mouse",
    "Shift",
    "Control",
    "Alt"
};

// =========== Esp Armas Nombres ===========
const char* zGetWName(uint32_t ID)
{
    switch (ID)
    {
    case 0:       return "AK47";
    case 1:       return "Punos";
    case 2:       return "M4A1";
    case 3:       return "USP";
    case 4:       return "AWM";
    case 5:       return "M1014";
    case 6:       return "AK47";
    case 7:       return "UMP";
    case 8:       return "MP5";
    case 9:       return "DESERT EAGLE";
    case 10:      return "G18";
    case 11:      return "M14";
    case 12:      return "SCAR";
    case 13:      return "VSS";
    case 14:      return "GROZA";
    case 15:      return "MP40";
    case 16:      return "SARTEN";
    case 17:      return "MACHETE";
    case 18:      return "SKS";
    case 19:      return "M249";
    case 20:      return "M1873";
    case 21:      return "KAR98K";
    case 24:      return "FAMAS";
    case 25:      return "M500";
    case 26:      return "SVD";
    case 27:      return "BATE";
    case 28:      return "XM8";
    case 29:      return "SPAS12";
    case 30:      return "M60";
    case 32:      return "P90";
    case 33:      return "AN94";
    case 34:      return "KATANA";
    case 35:      return "CG15";
    case 39:      return "PLASMA";
    case 41:      return "M1887";
    case 43:      return "THOMPSON";
    case 45:      return "M828";
    case 46:      return "AUG";
    case 47:      return "PARAFAL";
    case 48:      return "WOODPECKER";
    case 49:      return "VECTOR";
    case 50:      return "MAG-7";
    case 51:      return "HOZ";
    case 54:      return "KORD";
    case 55:      return "M1917";
    case 56:      return "USP-2";
    case 57:      return "KINGFISHER";
    case 58:      return "MINI UZI";
    case 60:      return "MP5 1";
    case 62:      return "VSS 1";
    case 65:      return "AWM-Y";
    case 67:      return "FAMAS 1";
    case 70:      return "GROZA-X";
    case 71:      return "M249-X";
    case 72:      return "SVD-Y";
    case 73:      return "G36";
    case 75:      return "M24";
    case 78:      return "RIFLE CURATIVO";
    case 80:      return "M4A1 1";
    case 81:      return "M4A1 2";
    case 82:      return "M4A1 3";
    case 86:      return "CHARGE BUSTER";
    case 88:      return "MAC10";
    case 89:      return "AC80";
    case 93:      return "PISTOLA CURATIVA";
    case 99:      return "ARMA ESCUDO";
    case 100:     return "LANZALLAMAS";
    case 119:     return "M1887-X";
    case 120:     return "MP5 2";
    case 121:     return "MP5 3";
    case 124:     return "VSS 2";
    case 125:     return "VSS 3";
    case 130:     return "FAMAS 2";
    case 131:     return "FAMAS 3";
    case 150:     return "BIZON";
    case 178:     return "SCAR 1";
    case 179:     return "SCAR 2";
    case 180:     return "SCAR 3";
    case 181:     return "TROGON";
    case 182:     return "TROGON - GRANADA";
    case 184:     return "M1014 1";
    case 185:     return "M1014 2";
    case 186:     return "M1014 3";
    case 193:     return "AUG 1";
    case 194:     return "AUG 2";
    case 195:     return "AUG 3";
    case 197:     return "VSK94";
    case 228:     return "MAC10 1";
    case 229:     return "MAC10 2";
    case 230:     return "MAC10 3";
    case 602:     return "GRANADA CEGADORA";
    case 608:     return "GRANADA HIELO";
    case 1204:    return "PARED GLOO";
    case 10449:   return "TRIPLE SHURIKEN";
    case 21001:   return "PISTOLA CURATIVA-Y";
    case 21002:   return "M590";
    case 21003:   return "HEAVY SHURIKEN";
    case 21004:   return "LIGHT SHURIKEN";
    default:      return "Desconocido";
    }
}
const char* zGetWIcon(uint32_t ID) {
    switch (ID) {
    case 0:   return "K";
    case 1: return "h";
    case 2: return "b";
    case 3: return "+";
    case 4: return "L";
    case 5: return "j";
    case 6: return "K";
    case 7: return "3";
    case 8:   return "f";
    case 9:   return "9";
    case 10:   return "A";
    case 11:   return "&";
    case 12:   return "-";
    case 13:   return "5";
    case 14:   return "P";
    case 15:   return "W";
    case 16:   return ".";
    case 17:   return ":";
    case 18:   return "U";
    case 19:   return "M";
    case 20:   return "=";
    case 21:   return "i";
    case 24:   return "E";
    case 25:   return "B";
    case 26:   return "[";
    case 27:   return "F";
    case 28:   return "`";
    case 29:   return ")";
    case 30:   return "'";
    case 32:   return "4";
    case 33:   return "/";
    case 34:   return ">";
    case 35:   return "]";
    case 39:   return "R";
    case 41:   return "S";
    case 43:   return "<";
    case 45:   return "#";
    case 46:   return "e";
    case 47:   return "I";
    case 48:   return "C";
    case 49:   return "N";
    case 50:   return "Q";
    case 51:   return "V";
    case 54:   return ";";
    case 55:   return "c";
    case 56:   return "+";
    case 57:   return ",";
    case 58:   return "O";
    case 60: return "f";
    case 62: return "5";
    case 65: return "L";
    case 67:   return "E";
    case 70:   return "P";
    case 72:   return "[";
    case 71:   return "M";
    case 73:   return "D";
    case 75:   return "8";
    case 78:   return "(";
    case 80:   return "b";
    case 81:   return "b";
    case 82:   return "b";
    case 86:   return "T";
    case 88:   return "%";
    case 89:   return "G";
    case 93:   return "_";
    case 99:   return "$";
    case 100:   return "Y";
    case 119:   return "S";
    case 120: return "f";
    case 121: return "f";
    case 124: return "5";
    case 125: return "5";
    case 130:   return "E";
    case 131:   return "E";
    case 150:   return "a";
    case 178:   return "-";
    case 179:   return "-";
    case 180:   return "-";
    case 181:   return "*";
    case 182:   return "*";
    case 184:   return "j";
    case 185:   return "j";
    case 186:   return "j";
    case 193:   return "e";
    case 194:   return "e";
    case 195:   return "e";
    case 197:   return "Z";
    case 228:   return "%";
    case 229:   return "%";
    case 230:   return "%";
    case 602:   return "1";
    case 608:   return "1";
    case 1204:   return "g";
    case 10449:   return "m";
    case 21001:   return "_";
    case 21002:   return "?";
    case 21003:   return "k";
    case 21004:   return "l";
    default: return "n";
    }
}

void zRenderWeapon(const ImFont* zFont, ImVec2 Pos, ImColor Color, bool borde, float size, const char* zIDW)
{
    auto DrawList = ImGui::GetForegroundDrawList();
    if (borde)
    {
        ImU32 black = IM_COL32(0, 0, 0, 255);
        DrawList->AddText((ImFont*)zFont, size, ImVec2(Pos.x - 1, Pos.y - 1), black, zIDW, nullptr);
        DrawList->AddText((ImFont*)zFont, size, ImVec2(Pos.x + 1, Pos.y - 1), black, zIDW, nullptr);
        DrawList->AddText((ImFont*)zFont, size, ImVec2(Pos.x - 1, Pos.y + 1), black, zIDW, nullptr);
        DrawList->AddText((ImFont*)zFont, size, ImVec2(Pos.x + 1, Pos.y + 1), black, zIDW, nullptr);
    }
    DrawList->AddText((ImFont*)zFont, size, Pos, (ImU32)Color, zIDW, nullptr);
}

bool IsWeaponSelected(uint32_t weaponId)
{
    if (weaponId == 0) return false;

    switch (weaponId)
    {
    case 4:   return s_weapons[0]; // AWM
    case 21:  return s_weapons[1]; // KAR98K
    case 45:  return s_weapons[2]; // M828
    case 65:  return s_weapons[3]; // AWM-Y
    case 75:  return s_weapons[4]; // M24
    case 54:  return s_weapons[5]; // KORD
    case 197: return s_weapons[6]; // VSK94
    default:  return false;
    }
}

// Esp Team
Bool3 IsTeamVar = Bool3::Unknown;
uint32_t currentPullTarget = 0;
std::unordered_map<uint32_t, Vector3> originalPositions; // Corregí el nombre "originalPositons"

// Fakelag namespace removed to prevent bans

float VelocidadMagnet = 0.016f;
float FuerzaMagnet = 0.05f;
int HuesoMagnetIndex = 0;
uint32_t objetivoMagnetActual = 0;
std::unordered_map<uint32_t, Vector3> posicionesOriginalesMagnet;
float tiempoAtraccionMagnet = 0.0f;
std::chrono::steady_clock::time_point ultimoDisparoMagnet;
int cooldownDisparoMagnet = 500.0f;

uint32_t offsetsHuesosMagnet[] = { Head, Spine, 0x1A8, Hip };


Vector3 GetEntityRootPosition(uint32_t entity) {
    Vector3 pos = Vector3::Zero();
    uint32_t rootTransform;
    if (ReadZ(entity + Root, rootTransform)) {
        GetNodePosition(rootTransform, pos);
    }
    return pos;
}

struct ColorRGBA {
    float r, g, b, a;
    ColorRGBA(float r = 0, float g = 0, float b = 0, float a = 1) : r(r), g(g), b(b), a(a) {}
};

ColorRGBA CustomSkyColor(0.0f, 0.0f, 0.0f, 1.0f);  // Negro
ColorRGBA OriginalSkyColor(0.0f, 0.0f, 0.0f, 1.0f);
bool firstTimeNightMode = true;

Vector3 GetLocalPlayerPosition()
{
    Vector3 pos{};
    uintptr_t localPlayer;
    if (ReadZ(Il2Cpp + InitBase + LocalPlayer, localPlayer))
        ReadZ(localPlayer + Player_Data + XPose, pos);
    return pos;
}

// OPTIMIZACIÓN: Usar distancia al cuadrado para comparaciones, evitar sqrtf cuando sea posible
inline float GetDistanceSquared(ImVec2 a, ImVec2 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return dx * dx + dy * dy;
}

float GetDistance(ImVec2 a, ImVec2 b)
{
    return sqrtf(GetDistanceSquared(a, b));
}






bool FovShow = false;
int Fovcosmos = 200.f;



// SILENT HOOK



bool GetNodePosition2(uint32_t nodeTransform, Vector3& result)
{
    uint32_t transformValue;
    if (!ReadZ(nodeTransform + 0x8, transformValue))
    {
        result = Vector3::Zero();
        return false;
    }

    return GetPosition(transformValue, result);
}




Matrix4x4 g_cameraMatrix;

void DrawAnimatedRainbowFOVWithGlow(ImVec2 center, float radius, float lineThickness, float glowThickness, int segments = 100, float rainbowSpeed = 0.2f) {
    auto* drawList = ImGui::GetBackgroundDrawList();
    double time = ImGui::GetTime();
    float hueOffset = fmod(time * rainbowSpeed, 1.0f);
    float glowOpacity = 0.10f;
    for (float thickness = glowThickness; thickness > 1.0f; thickness -= 1.5f) {
        for (int i = 0; i < segments; ++i) {
            float a0 = (2.0f * IM_PI * i) / segments;
            float a1 = (2.0f * IM_PI * (i + 1)) / segments;

            ImVec2 p0 = ImVec2(center.x + cosf(a0) * radius, center.y + sinf(a0) * radius);
            ImVec2 p1 = ImVec2(center.x + cosf(a1) * radius, center.y + sinf(a1) * radius);

            ImU32 glowColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, glowOpacity));
            drawList->AddLine(p0, p1, glowColor, thickness);
        }
        glowOpacity *= 0.6f;
    }
    for (int i = 0; i < segments; ++i) {
        float a0 = (2.0f * IM_PI * i) / segments;
        float a1 = (2.0f * IM_PI * (i + 1)) / segments;
        ImVec2 p0 = ImVec2(center.x + cosf(a0) * radius, center.y + sinf(a0) * radius);
        ImVec2 p1 = ImVec2(center.x + cosf(a1) * radius, center.y + sinf(a1) * radius);
        float hue = fmod(hueOffset + (float)i / segments, 1.0f);
        ImU32 color = ImColor::HSV(hue, 1.0f, 1.0f);
        drawList->AddLine(p0, p1, color, lineThickness);
    }
}

struct SearchData {
    DWORD pid;
    HWND resultHwnd = nullptr;
};

BOOL CALLBACK EnumChildProc(HWND hWnd, LPARAM lParam) {
    char title[256];
    GetWindowTextA(hWnd, title, sizeof(title));
    std::string windowTitle(title);
    if (windowTitle == "HD-Player" || windowTitle == "_ctl.Window") {
        SearchData* data = reinterpret_cast<SearchData*>(lParam);
        data->resultHwnd = hWnd;
        return FALSE;
    }
    return TRUE;
}

BOOL CALLBACK EnumTopLevelProc(HWND hWnd, LPARAM lParam) {
    SearchData* data = reinterpret_cast<SearchData*>(lParam);
    DWORD windowPid = 0;
    GetWindowThreadProcessId(hWnd, &windowPid);
    if (windowPid == data->pid) {
        EnumChildWindows(hWnd, EnumChildProc, lParam);
        if (data->resultHwnd != nullptr)
            return FALSE;
    }
    return TRUE;
}

HWND zGetHwndBsEx()
{
    DWORD pid = GetProcZ("Bluestacks.exe");
    if (pid == 0)
    {
        pid = GetCurrentProcessId();
    }

    SearchData data;
    data.pid = pid;
    EnumWindows(EnumTopLevelProc, reinterpret_cast<LPARAM>(&data));
    return data.resultHwnd;
}


HWND hwndEmul = zGetHwndBsEx();

// Función auxiliar para obtener la posición de un hueso específico
Vector3 GetBonePos(uintptr_t player, uint32_t offset) {
    uint32_t boneTransform = 0;
    Vector3 pos = Vector3::Zero();
    if (ReadZ(player + offset, boneTransform)) {
        GetNodePosition(boneTransform, pos);
    }
    return pos;
}












///////////////////////////////////////////////////////////////////////////////////////
// aqui creamos las esp line
void ESP_line()
{
    // --- Keybinds eliminados - todo se activa con checkbox ---

    static bool timerStarted = false;
    static bool timerRunning = false;
    static bool showPlayMessage = false;
    static float timeRemaining = 190.0f;
    static uint32_t prevMatchStatus = 0;
    static std::chrono::steady_clock::time_point startTime;

    RECT clientRect;
    POINT screenPos = { 0, 0 };
    GetClientRect(RenderWindow, &clientRect);
    ClientToScreen(RenderWindow, &screenPos);

    int x = screenPos.x;
    int y = screenPos.y;
    int width = clientRect.right;
    int height = clientRect.bottom;
    Vector2 screenCenter = Vector2(x + width / 2, y + height / 2);

    uint32_t gameFacade = 0;
    if (!ReadZ(Il2Cpp + InitBase, gameFacade) || gameFacade == 0) return;

    uint32_t staticGameFacade = 0;
    if (!ReadZ(gameFacade + StaticClass, staticGameFacade) || staticGameFacade == 0) return;

    uint32_t currentGame = 0;
    if (!ReadZ(staticGameFacade, currentGame) || currentGame == 0) return;

    uint32_t currentMatch = 0;
    if (!ReadZ(currentGame + CurrentMatch, currentMatch) || currentMatch == 0) return;

    uint32_t matchStatus = 0;
    if (!ReadZ(currentMatch + MatchStatus, matchStatus) || matchStatus == 0) return;

    // --- Desactivar todas las funciones si no está en partida ---
    // matchStatus == 1 indica que está en partida (MATCH_RUNNING)
    static bool wasInMatch = false;
    bool isInMatch = (matchStatus == 1);

    if (!isInMatch && wasInMatch)
    {
        // Acaba de salir de la partida, desactivar todas las funciones
        // NOTA: El panel (Main_Window) NO se desactiva aquí - debe seguir funcionando
        AimbotRotation = false;
        NoRecoil = false;
        PullPlayer = false;
        UpCheck = false;
        EspBox3D = false;
        // EspRadar2D no se desactiva - debe estar siempre visible
        ESPCrosshair = false;
    }
    wasInMatch = isInMatch;

    static uint32_t savedLocalPlayer = 0;
    uint32_t localPlayer = 0;

    if (!ReadZ(currentMatch + LocalPlayer, localPlayer) || localPlayer == 0) {
        if (savedLocalPlayer == 0) return;
        localPlayer = savedLocalPlayer;
    }
    else {
        savedLocalPlayer = localPlayer;
    }

    uint32_t TimeGames = 0;

    ReadZ(currentMatch + 0x8, TimeGames);

    Vector3 mainPos = { 0.0f, 0.0f, 0.0f };
    uint32_t mainTransform = 0;
    if (ReadZ(localPlayer + MainCameraTransform, mainTransform) && mainTransform != 0) {
        GetPosition(mainTransform, mainPos);
    }

    Matrix4x4 viewMatrix = {};
    uint32_t followCamera = 0;
    uint32_t camera = 0;
    uint32_t cameraBase = 0;

    if (ReadZ(localPlayer + FollowCamera, followCamera) && followCamera != 0 &&
        ReadZ(followCamera + Camera, camera) && camera != 0 &&
        ReadZ(camera + 0x8, cameraBase) && cameraBase != 0) {
        ReadZ(cameraBase + ViewMatrix, viewMatrix);
    }

    uint32_t entityDictionary = 0;
    if (!ReadZ(currentGame + DictionaryEntities, entityDictionary) || entityDictionary == 0) return;

    int entitiesCount = 0;
    ReadZ(entityDictionary + 0x10, entitiesCount);

    uint32_t entities = 0;
    ReadZ(entityDictionary + 0x0C, entities);

    entities = entities + 0x10;




    static std::unordered_map<std::string, ImVec4> enemyColors;
    static std::vector<std::pair<std::string, int>> enemyList;
    std::unordered_map<uint32_t, Vector3> enemiesSunk;
    int Enemigos = 0;

    for (uint32_t i = 0; i < entitiesCount; i++)
    {

        bool isDead = false;
        bool IsKnown = false;
        bool IsTeam = false;
        bool isVisible = false;
        bool isTeam;
        int ESPHealthInt = 0;
        int ESPShieldInt = 0;
        int ESPMushromInt = 0;
        int enemyId = i;
        float DistanceA = 0.0f;
        float rectWidth = 150.0f;
        float rectHeight = 30.0f;
        static int NextMatch = -1;
        static int NextMatchRage = -1;

        uint32_t entry = entities + (i * 0x10);

        int hash = 0;
        ReadZ(entry + 0x0, hash);
        if (hash < 0) continue;

        uint32_t entity = 0;
        ReadZ(entry + 0x0C, entity);
        if (entity == 0) continue;
        if (entity == localPlayer) continue;

        if (!ReadZ(entity + WukongOrion, NextMatch)) continue;

        uint32_t avatarManager;
        if (!ReadZ(entity + AvatarManager, avatarManager) || avatarManager == 0) continue;

        uint32_t avatar;
        if (!ReadZ(avatarManager + Avatar, avatar) || avatar == 0) continue;

        if (!ReadZ(avatar + Avatar_IsVisible, isVisible));

        uint32_t avatarData;
        if (!ReadZ(avatar + Avatar_Data, avatarData) || avatarData == 0) continue;

        if (ReadZ(avatarData + Avatar_Data_IsTeam, isTeam))
        {
            if (isTeam)
            {
                IsTeamVar = Bool3::True;
            }
            else
            {
                IsTeamVar = Bool3::False;
            }
        }

        bool isDeadLocal = 0;
        if (ReadZ(entity + Player_IsDead, isDeadLocal)) {
            isDead = isDeadLocal;
        }

        
        if (ShowTeam == false && IsTeamVar == Bool3::True) continue;

        bool isWukongOrion = (NextMatch == 1 || NextMatch == 2);
        if (!ShowInvis && !isVisible && !isWukongOrion) continue;
        if (!ShowWukongOrion && isWukongOrion) continue;
        if (isDead) continue;


        // OPTIMIZACIÓN: Solo leer huesos si son necesarios para ESP activo
        bool needBones = ESPBones || ESPBox || ESPFillBox || (SelectedESPBoxStyle == 3) || ESPHealth || ESPHealthMAX || EspWeaponsICON || EspWeaponsTex;
        
        // Leer huesos básicos (Head y Root) siempre necesarios para cálculos
        uint32_t headBone = 0;
        if (ReadZ(entity + Head, headBone) && headBone != 0) {
            GetNodePosition(headBone, bonePositions.Cabeza);
        }
        
        uint32_t rootBone = 0;
        if (ReadZ(entity + Root, rootBone) && rootBone != 0) {
            GetNodePosition(rootBone, bonePositions.Raiz);
        }

        // Solo leer huesos completos si se necesitan para ESP
        if (needBones) {
            std::vector<std::pair<uint32_t, Vector3*>> bones = {
                { (uint32_t)Spine, &bonePositions.Columna },
                { (uint32_t)Hip, &bonePositions.Cadera },
                { (uint32_t)LeftSholder, &bonePositions.HombroIzquierdo },
                { (uint32_t)LeftElbow, &bonePositions.CodoIzquierdo },
                { (uint32_t)LeftWrist, &bonePositions.MuñecaIzquierda },
                { (uint32_t)LeftHand, &bonePositions.ManoIzquierda },
                { (uint32_t)RightSholder, &bonePositions.HombroDerecho },
                { (uint32_t)RightElbow, &bonePositions.CodoDerecho },
                { (uint32_t)RightWrist, &bonePositions.MuñecaDerecha },
                { (uint32_t)RightHand, &bonePositions.ManoDerecha },
                { (uint32_t)LeftCalf, &bonePositions.PantorrillaIzquierda },
                { (uint32_t)LeftFoot, &bonePositions.PieIzquierdo },
                { (uint32_t)RightCalf, &bonePositions.PantorrillaDerecha },
                { (uint32_t)RightFoot, &bonePositions.PieDerecho }
            };

            for (const auto& [boneID, bonePos] : bones) {
                uint32_t bone = 0;
                if (ReadZ(entity + boneID, bone) && bone != 0) {
                    Vector3 temp;
                    if (GetNodePosition(bone, temp)) {
                        if (temp.x == 0.f && temp.y == 0.f && temp.z == 0.f) continue;
                        *bonePos = temp;
                    }
                }
            }
        }

        bool isKnocked = false;
        uint32_t shadowBase = 0;
        if (ReadZ(entity + Player_ShadowBase, shadowBase)) {
            if (shadowBase != 0) {
                int xpose;
                if (ReadZ(shadowBase + XPose, xpose)) {
                    if (xpose == 8) {
                        isKnocked = true;
                    }
                }
            }
        }

        if (ESPHealth) {
            uint32_t dataPool;
            if (ReadZ(entity + Player_Data, dataPool)) {
                uint32_t poolObj;
                if (ReadZ(dataPool + 0x8, poolObj)) {
                    uint32_t pool;
                    if (ReadZ(poolObj + 0x10, pool)) {
                        int Health;
                        if (ReadZ(pool + 0xC, Health)) ESPHealthInt = Health;
                    }
                }
            }
        }

        uint32_t nameAddr;
        std::string strEspNameScan = defaultName;
        if (ReadZ(entity + Player_Name, nameAddr)) {
            strEspNameScan = ReadPlayerName(nameAddr);
            if (strEspNameScan.empty()) strEspNameScan = defaultName;
        }

        // OPTIMIZACIÓN: Calcular distancia primero antes de hacer conversiones costosas
        DistanceA = Vector3::Distance(mainPos, bonePositions.Cabeza);
        if (DistanceA > maxDistance) continue;

        // Obtener salud para el HUD de info del enemigo más cercano
        int HUDHealthVal = 200;
        uint32_t dataPoolVal;
        if (ReadZ(entity + Player_Data, dataPoolVal) && dataPoolVal != 0) {
            uint32_t poolObjVal;
            if (ReadZ(dataPoolVal + 0x8, poolObjVal) && poolObjVal != 0) {
                uint32_t poolVal;
                if (ReadZ(poolObjVal + 0x10, poolVal) && poolVal != 0) {
                    ReadZ(poolVal + 0xC, HUDHealthVal);
                }
            }
        }


        // OPTIMIZACIÓN: Solo convertir a pantalla después de verificar distancia
        auto headScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Cabeza, width, height);
        auto bottomScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Raiz, width, height);

        if (headScreenPos.x < 1 || headScreenPos.y < 1) continue;

        // OPTIMIZACIÓN: Declarar variables fuera del condicional pero solo calcular si se necesitan
        ImVec2 CuelloScreenPos, CaderaScreenPos, HombroDerechoScreenPos, HombroIzquierdoScreenPos;
        ImVec2 CodoDerechoScreenPos, CodoIzquierdoScreenPos, ManoDerechaScreenPos, ManoIzquierdaScreenPos;
        ImVec2 PieDerechoScreenPos, PieIzquierdoScreenPos, PantorrillaDerechaScreenPos, PantorrillaIzquierdaScreenPos;

        // OPTIMIZACIÓN: Solo convertir huesos adicionales si se necesitan para ESP activo
        if (needBones) {
            CuelloScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Columna, width, height);
            CaderaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Cadera, width, height);
            HombroDerechoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.HombroDerecho, width, height);
            HombroIzquierdoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.HombroIzquierdo, width, height);
            CodoDerechoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.CodoDerecho, width, height);
            CodoIzquierdoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.CodoIzquierdo, width, height);
            ManoDerechaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.ManoDerecha, width, height);
            ManoIzquierdaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.ManoIzquierda, width, height);
            PieDerechoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.PieDerecho, width, height);
            PieIzquierdoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.PieIzquierdo, width, height);
            PantorrillaDerechaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.PantorrillaDerecha, width, height);
            PantorrillaIzquierdaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.PantorrillaIzquierda, width, height);
        }

        Vector2 PlayerPosHead = Vector2(x + headScreenPos.x, y + headScreenPos.y);
        Vector2 PlayerPosBott = Vector2(x + bottomScreenPos.x, y + bottomScreenPos.y);
        float CornerHeight = std::abs(PlayerPosHead.y - PlayerPosBott.y);
        float CornerWidth = CornerHeight * 0.65f;

        auto screenHead = WorldToScreenImVec2(viewMatrix, bonePositions.Cabeza, g_windowWidth, g_windowHeight);
        if (screenHead.x == -1 && screenHead.y == -1) continue;

        float dx = screenHead.x - (g_windowWidth / 2.0f);
        float dy = screenHead.y - (g_windowHeight / 2.0f);

        float DistancePlayers = Vector3::Distance(mainPos, bonePositions.Cabeza);

        EspMode = true;

        if (EspLineZ && !IsTeam)
        {
            static HWND hwndEmulator = nullptr;
            if (!hwndEmulator || !IsWindow(hwndEmulator)) { hwndEmulator = FindRenderWindow(); }
            if (!hwndEmulator) return;
            RECT clientRect;
            POINT screenPos = { 0, 0 };
            GetClientRect(hwndEmulator, &clientRect);
            ClientToScreen(hwndEmulator, &screenPos);
            int emulatorWidth = clientRect.right;
            int emulatorHeight = clientRect.bottom;
            int clientX = screenPos.x;
            int clientY = screenPos.y;
            auto* drawList = ImGui::GetBackgroundDrawList();
            drawList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emulatorWidth, clientY + emulatorHeight), true);
            ImVec2 startPoint = ImVec2(clientX + emulatorWidth / 2, clientY); // Centro de arriba de la pantalla
            ImVec2 endPoint = ImVec2(x + headScreenPos.x, y + headScreenPos.y - 5);
            if (startPoint.x >= clientX && startPoint.x <= clientX + emulatorWidth && startPoint.y >= clientY && startPoint.y <= clientY + emulatorHeight && endPoint.x >= clientX && endPoint.x <= clientX + emulatorWidth && endPoint.y >= clientY && endPoint.y <= clientY + emulatorHeight)
            {
                const int segments = 80;
                float time = ImGui::GetTime();
                float pulse = 3.0f + 2.5f * sin(time * 2.0f);
                if (Shadow) {
                    ImU32 shadowColor = ImGui::ColorConvertFloat4ToU32(ImVec4(lineColorEnemy.x, lineColorEnemy.y, lineColorEnemy.z, 0.15f));
                    for (float thickness = 10.0f; thickness > 1.0f; thickness -= 1.5f) {
                        for (int i = 0; i < segments; ++i) {
                            float t1 = i / (float)segments;
                            float t2 = (i + 1) / (float)segments;
                            ImVec2 point1 = ImVec2((1 - t1) * startPoint.x + t1 * endPoint.x, (1 - t1) * startPoint.y + t1 * endPoint.y);
                            ImVec2 point2 = ImVec2((1 - t2) * startPoint.x + t2 * endPoint.x, (1 - t2) * startPoint.y + t2 * endPoint.y);
                            drawList->AddLine(point1, point2, shadowColor, thickness);
                            ImU32 segColor;
                            if (rainbowMode) { float hue = fmod(time * 0.3f + t1 * 0.5f, 1.0f); segColor = ImColor::HSV(hue, 0.8f, 1.0f); }
                            else { segColor = ImGui::GetColorU32(lineColorEnemy); }
                            drawList->AddLine(point1, point2, segColor, lineThicknessBase + pulse * 0.08f);
                        }
                    }
                }
                for (int i = 0; i < segments; ++i) {
                    float t1 = i / (float)segments;
                    float t2 = (i + 1) / (float)segments;
                    ImVec2 point1 = ImVec2((1 - t1) * startPoint.x + t1 * endPoint.x, (1 - t1) * startPoint.y + t1 * endPoint.y);
                    ImVec2 point2 = ImVec2((1 - t2) * startPoint.x + t2 * endPoint.x, (1 - t2) * startPoint.y + t2 * endPoint.y);
                    ImU32 segColor;
                    if (rainbowMode) { float hue = fmod(time * 0.3f + t1 * 0.5f, 1.0f); segColor = ImColor::HSV(hue, 0.8f, 1.0f); }
                    else { segColor = ImGui::GetColorU32(lineColorEnemy); }
                    drawList->AddLine(point1, point2, segColor, lineThicknessBase + pulse * 0.05f);
                }
            }
            drawList->PopClipRect();
        }

        // ESP Líneas CENTER, BOTTOM
        if ((ESPLineCenter || ESPLineBottom) && !IsTeam)
        {
            static HWND hwndEmulator = nullptr;
            if (!hwndEmulator || !IsWindow(hwndEmulator)) { hwndEmulator = FindRenderWindow(); }
            if (!hwndEmulator) continue;
            RECT clientRect;
            POINT screenPos = { 0, 0 };
            GetClientRect(hwndEmulator, &clientRect);
            ClientToScreen(hwndEmulator, &screenPos);
            int emulatorWidth = clientRect.right;
            int emulatorHeight = clientRect.bottom;
            int clientX = screenPos.x;
            int clientY = screenPos.y;
            auto* drawList = ImGui::GetBackgroundDrawList();
            drawList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emulatorWidth, clientY + emulatorHeight), true);

            // Calcular posiciones de los huesos
            ImVec2 headScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Cabeza, emulatorWidth, emulatorHeight);
            ImVec2 rootScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Raiz, emulatorWidth, emulatorHeight);

            // Línea CENTER (desde el centro de la pantalla hacia la cabeza)
            if (ESPLineCenter && headScreenPos.x != -1 && headScreenPos.y != -1)
            {
                ImVec2 startPoint = ImVec2(clientX + emulatorWidth / 2, clientY + emulatorHeight / 2); // Centro de la pantalla
                ImVec2 endPoint = ImVec2(clientX + headScreenPos.x, clientY + headScreenPos.y);
                ImU32 lineColor = ImGui::GetColorU32(lineColorEnemy);
                drawList->AddLine(startPoint, endPoint, lineColor, 1.5f);
            }

            // Línea BOTTOM (desde el centro de abajo de la pantalla hacia los pies/root)
            if (ESPLineBottom && rootScreenPos.x != -1 && rootScreenPos.y != -1)
            {
                ImVec2 startPoint = ImVec2(clientX + emulatorWidth / 2, clientY + emulatorHeight); // Centro de abajo de la pantalla
                ImVec2 endPoint = ImVec2(clientX + rootScreenPos.x, clientY + rootScreenPos.y);
                ImU32 lineColor = ImGui::GetColorU32(lineColorEnemy);
                drawList->AddLine(startPoint, endPoint, lineColor, 1.5f);
            }

            drawList->PopClipRect();
        }

        // ESP Distance - Dibujar en el root del enemigo
        if (ESPDisctanc && !IsTeam)
        {
            static HWND hwndEmulator = nullptr;
            if (!hwndEmulator || !IsWindow(hwndEmulator)) { hwndEmulator = FindRenderWindow(); }
            if (!hwndEmulator) continue;
            RECT clientRect;
            POINT screenPos = { 0, 0 };
            GetClientRect(hwndEmulator, &clientRect);
            ClientToScreen(hwndEmulator, &screenPos);
            int emulatorWidth = clientRect.right;
            int emulatorHeight = clientRect.bottom;
            int clientX = screenPos.x;
            int clientY = screenPos.y;
            auto* drawList = ImGui::GetBackgroundDrawList();
            drawList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emulatorWidth, clientY + emulatorHeight), true);

            // Calcular distancia
            float distance = Vector3::Distance(mainPos, bonePositions.Cabeza);
            std::string distanceStr = std::to_string((int)distance) + "m";

            // Calcular posición de los pies (root)
            ImVec2 rootScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Raiz, emulatorWidth, emulatorHeight);

            ImGui::PushFont(font::fuentezada);
            ImVec2 textSize = ImGui::CalcTextSize(distanceStr.c_str());
            ImVec2 textPos = ImVec2(clientX + rootScreenPos.x - textSize.x / 2, clientY + rootScreenPos.y + 15.0f);
            
            // Dibujar fondo negro para mejor legibilidad
            ImVec2 bgMin = ImVec2(textPos.x - 2, textPos.y - 2);
            ImVec2 bgMax = ImVec2(textPos.x + textSize.x + 2, textPos.y + textSize.y + 2);
            drawList->AddRectFilled(bgMin, bgMax, IM_COL32(0, 0, 0, 150), 2.0f);
            
            // Dibujar texto de distancia
            ImU32 textColor = IM_COL32(255, 255, 255, 255);
            drawList->AddText(textPos, textColor, distanceStr.c_str());
            ImGui::PopFont();
            drawList->PopClipRect();
        }

        if (ESPBones && !IsTeam)
        {
            static HWND hwndEmulator = nullptr;
            if (!hwndEmulator || !IsWindow(hwndEmulator)) {
                hwndEmulator = FindRenderWindow();
            }
            if (!hwndEmulator) continue;
            RECT clientRect;
            POINT screenPos = { 0, 0 };
            GetClientRect(hwndEmulator, &clientRect);
            ClientToScreen(hwndEmulator, &screenPos);
            int emulatorWidth = clientRect.right;
            int emulatorHeight = clientRect.bottom;
            int clientX = screenPos.x;
            int clientY = screenPos.y;
            auto* drawList = ImGui::GetForegroundDrawList();
            drawList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emulatorWidth, clientY + emulatorHeight), true);
            auto isValidPosition = [&](const ImVec2& pos) {
                return pos.x > 0 && pos.y > 0 && pos.x < emulatorWidth && pos.y < emulatorHeight;
                };
            float maxBoneDistance = 500.0f;
            auto isReasonableDistance = [&](const ImVec2& pos1, const ImVec2& pos2) {
                float dx = pos1.x - pos2.x;
                float dy = pos1.y - pos2.y;
                return (dx * dx + dy * dy) < (maxBoneDistance * maxBoneDistance);
                };
            float r = ((invisibleColor >> IM_COL32_R_SHIFT) & 0xFF) / 255.0f;
            float g = ((invisibleColor >> IM_COL32_G_SHIFT) & 0xFF) / 255.0f;
            float b = ((invisibleColor >> IM_COL32_B_SHIFT) & 0xFF) / 255.0f;
            float a = ((invisibleColor >> IM_COL32_A_SHIFT) & 0xFF) / 255.0f;
            ImVec4 boneColor = !isVisible ? ImVec4(r, g, b, a) :
                (isKnocked ? ImVec4(bonesColor) : rgbMode ? ImVec4(
                    0.5f * (1.0f + sin(2.0 * M_PI * 0.5 * getCurrentTime())),
                    0.5f * (1.0f + sin(2.0 * M_PI * 0.5 * getCurrentTime() + 2.0 * M_PI / 3.0)),
                    0.5f * (1.0f + sin(2.0 * M_PI * 0.5 * getCurrentTime() + 4.0 * M_PI / 3.0)), 1.0f) : bonesColor);
            std::vector<std::pair<ImVec2, ImVec2>> boneSegments = {
                {headScreenPos, CuelloScreenPos},
                {CuelloScreenPos, CaderaScreenPos},
                {CaderaScreenPos, PieDerechoScreenPos},
                {PieDerechoScreenPos, PantorrillaDerechaScreenPos},
                {CaderaScreenPos, PieIzquierdoScreenPos},
                {PieIzquierdoScreenPos, PantorrillaIzquierdaScreenPos},
                {CuelloScreenPos, HombroDerechoScreenPos},
                {HombroDerechoScreenPos, CodoDerechoScreenPos},
                {CodoDerechoScreenPos, ManoDerechaScreenPos},
                {CuelloScreenPos, HombroIzquierdoScreenPos},
                {HombroIzquierdoScreenPos, CodoIzquierdoScreenPos},
                {CodoIzquierdoScreenPos, ManoIzquierdaScreenPos}
            };
            float scaleFactor = std::clamp(100.0f / (DistanceA + 100.0f), 0.2f, 0.7f);
            for (const auto& segment : boneSegments) {
                ImVec2 start = { x + segment.first.x, y + segment.first.y };
                ImVec2 end = { x + segment.second.x, y + segment.second.y };
                if (isValidPosition(start) && isValidPosition(end) && isReasonableDistance(start, end)) {
                    drawList->AddLine(start, end, ImColor(boneColor), 0.2);
                }
            }
            drawList->PopClipRect();
        }

        if (ESPBox && !IsTeam)
        {
            auto vList = ImGui::GetForegroundDrawList();
            vList->PushClipRect(ImVec2(x, y), ImVec2(x + width, y + height), true);
            
            ImVec4 currentColor = isVisible ? ImVec4(boxColor) : ImVec4(lineColorTeam);
            ImU32 borderColU32 = IM_COL32(currentColor.x * 255, currentColor.y * 255, currentColor.z * 255, 255);
            float thickness = 0.8f;

            if (EspBox3D)
            {
                // Calcular caja 3D alrededor del enemigo de forma dinámica según sus huesos reales
                Vector3 bottomPos = bonePositions.Raiz;
                Vector3 topPos = bonePositions.Cabeza;
                
                // Si la cabeza y los pies tienen posiciones válidas
                if (bottomPos.y != 0 || topPos.y != 0)
                {
                    // Altura del jugador real en coordenadas del mundo
                    float heightOffset = topPos.y - bottomPos.y;
                    
                    // Añadimos un pequeño margen por encima de la cabeza
                    float extraHeight = heightOffset * 0.1f;
                    topPos.y += extraHeight;
                    heightOffset += extraHeight;
                    
                    // Ancho de la caja en proporción a su altura real
                    float boxWidth = heightOffset * 0.35f;

                    // Definir los 8 puntos del cubo en 3D
                    Vector3 points[8] = {
                        bottomPos + Vector3(-boxWidth, 0, -boxWidth),
                        bottomPos + Vector3(boxWidth, 0, -boxWidth),
                        bottomPos + Vector3(boxWidth, 0, boxWidth),
                        bottomPos + Vector3(-boxWidth, 0, boxWidth),
                        topPos + Vector3(-boxWidth, 0, -boxWidth),
                        topPos + Vector3(boxWidth, 0, -boxWidth),
                        topPos + Vector3(boxWidth, 0, boxWidth),
                        topPos + Vector3(-boxWidth, 0, boxWidth)
                    };

                    // Proyectar puntos 3D a la pantalla 2D utilizando el canvas del overlay
                    ImVec2 screenPoints[8];
                    bool pointsValid = true;
                    for (int pIndex = 0; pIndex < 8; pIndex++)
                    {
                        screenPoints[pIndex] = WorldToScreenImVec2(viewMatrix, points[pIndex], width, height);
                        if (screenPoints[pIndex].x == -1 || screenPoints[pIndex].y == -1) {
                            pointsValid = false;
                            break;
                        }
                        screenPoints[pIndex].x += x;
                        screenPoints[pIndex].y += y;
                    }

                    if (pointsValid)
                    {
                        // Dibujar aristas de la base inferior (0-3)
                        for (int side = 0; side < 4; side++)
                            vList->AddLine(screenPoints[side], screenPoints[(side + 1) % 4], borderColU32, thickness);

                        // Dibujar aristas de la tapa superior (4-7)
                        for (int side = 0; side < 4; side++)
                            vList->AddLine(screenPoints[side + 4], screenPoints[((side + 1) % 4) + 4], borderColU32, thickness);

                        // Dibujar aristas verticales de conexión
                        for (int side = 0; side < 4; side++)
                            vList->AddLine(screenPoints[side], screenPoints[side + 4], borderColU32, thickness);
                    }
                }
            }
            else
            {
                float WBox = CornerWidth;
                float HBox = CornerHeight * 1.2f;
                float XBox = PlayerPosHead.x - (WBox / 2.0f);
                float YBox = PlayerPosHead.y - (HBox * 0.15f);
                if (ESPFillBox)
                {
                    ImU32 colBottom = IM_COL32((int)(fillBoxColor.x * 255), (int)(fillBoxColor.y * 255), (int)(fillBoxColor.z * 255), 140);
                    ImU32 colTop = IM_COL32(255, 255, 255, 60);
                    vList->AddRectFilledMultiColor(ImVec2(XBox, YBox), ImVec2(XBox + WBox, YBox + HBox), colTop, colTop, colBottom, colBottom);
                }
                switch (SelectedESPBoxStyle)
                {
                case 1: 
                {
                    vList->AddRect(ImVec2(XBox, YBox), ImVec2(XBox + WBox, YBox + HBox), borderColU32, 1.0f, 0, thickness);
                    break;
                }
                case 2: 
                {
                    vList->AddRect(ImVec2(XBox, YBox), ImVec2(XBox + WBox, YBox + HBox), borderColU32, 0.0f, 0, thickness);
                    break;
                }
                case 3:
                {
                    float lW = WBox / 4.0f;
                    float lH = HBox / 4.0f;
                    vList->AddLine(ImVec2(XBox, YBox), ImVec2(XBox + lW, YBox), borderColU32, thickness);
                    vList->AddLine(ImVec2(XBox, YBox), ImVec2(XBox, YBox + lH), borderColU32, thickness);
                    vList->AddLine(ImVec2(XBox + WBox - lW, YBox), ImVec2(XBox + WBox, YBox), borderColU32, thickness);
                    vList->AddLine(ImVec2(XBox + WBox, YBox), ImVec2(XBox + WBox, YBox + lH), borderColU32, thickness);
                    vList->AddLine(ImVec2(XBox, YBox + HBox - lH), ImVec2(XBox, YBox + HBox), borderColU32, thickness);
                    vList->AddLine(ImVec2(XBox, YBox + HBox), ImVec2(XBox + lW, YBox + HBox), borderColU32, thickness);
                    vList->AddLine(ImVec2(XBox + WBox - lW, YBox + HBox), ImVec2(XBox + WBox, YBox + HBox), borderColU32, thickness);
                    vList->AddLine(ImVec2(XBox + WBox, YBox + HBox - lH), ImVec2(XBox + WBox, YBox + HBox), borderColU32, thickness);
                    break;
                }
                }
            }
            vList->PopClipRect();
        }

        if (EspWeaponsICON && !IsTeam)
        {
            uint32_t weaponIDValue = zGetInfoW(entity);
            const char* weaponIcon = zGetWIcon(weaponIDValue);
            float scaleFactor = std::clamp(100.0f / (DistanceA + 100.0f), 0.2f, 0.7f);
            ImVec2 headPosScreen(headScreenPos.x, headScreenPos.y);
            ImVec2 feetPosScreen(headScreenPos.x, headScreenPos.y + (CornerHeight * scaleFactor * 1.4f));
            float XBox = x + headPosScreen.x - (CornerWidth * scaleFactor / 2.0f);
            float YBox = y + headPosScreen.y;
            float WBox = CornerWidth * scaleFactor;
            float HBox = feetPosScreen.y - headPosScreen.y;
            float weaponTextX = XBox + (WBox / 2.0f);
            float weaponTextY = YBox + HBox + 17.0f; 
            ImVec2 textPos(weaponTextX, weaponTextY);
            const float OFFSET_LEFT = 6.0f; 
            ImVec2 iconPos(textPos.x - OFFSET_LEFT, textPos.y);
            ImU32 weaponColorFinal = ImGui::ColorConvertFloat4ToU32(WeaponsColor);
            if (!isVisible)
                weaponColorFinal = invisibleColor;
            zRenderWeapon(font::WeaponsIco, iconPos, weaponColorFinal, false, 16, weaponIcon);
        }

        if (EspWeaponsTex && !IsTeam) {
            uint32_t weaponIDValue = zGetInfoW(entity);
            auto weaponName = zGetWName(weaponIDValue);
            float scaleFactor = std::clamp(100.0f / (DistanceA + 100.0f), 0.2f, 0.7f);
            ImVec2 headPosScreen = ImVec2(headScreenPos.x, headScreenPos.y);
            ImVec2 feetPosScreen = ImVec2(headScreenPos.x, headScreenPos.y + (CornerHeight * scaleFactor * 1.4f));
            float XBox = x + headPosScreen.x - (CornerWidth * scaleFactor / 2);
            float YBox = y + headPosScreen.y;
            float WBox = CornerWidth * scaleFactor;
            float HBox = feetPosScreen.y - headPosScreen.y;
            float weaponTextX = XBox + (WBox / 2);
            float weaponTextY = YBox + HBox + 9;
            auto vListText = ImGui::GetForegroundDrawList();
            ImU32 weaponTextColor = isVisible ? ImColor(WeaponsColor) : invisibleColor;
            float dynamicFontSize = 14.0f * scaleFactor;
            ImFont* fontToUse = ImGui::GetFont();
            ImVec2 textSize = fontToUse->CalcTextSizeA(dynamicFontSize, FLT_MAX, 0.0f, weaponName);
            ImVec2 textPos = ImVec2(weaponTextX - (textSize.x / 2), weaponTextY);
            vListText->AddText(fontToUse, dynamicFontSize, textPos, weaponTextColor, weaponName);
        }
        if (ESPHealth && !IsTeam)
        {
            HWND hwnd = FindWindowA(NULL, "MSI App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "BlueStacks App Player");
            if (!hwnd) return;

            RECT clientRect, windowRect;
            GetClientRect(hwnd, &clientRect);
            GetWindowRect(hwnd, &windowRect);

            int emulatorWidth = clientRect.right - clientRect.left;
            int emulatorHeight = clientRect.bottom - clientRect.top;
            int clientX = windowRect.left;
            int clientY = windowRect.top + (windowRect.bottom - windowRect.top - emulatorHeight);

            ImDrawList* vList = ImGui::GetForegroundDrawList();
            vList->PushClipRect(ImVec2(clientX, clientY),
                ImVec2(clientX + emulatorWidth, clientY + emulatorHeight), true);

            float WBox = CornerWidth;
            float HBox = CornerHeight * 1.2f;
            float XBox = PlayerPosHead.x - (WBox / 2.0f);
            float YBox = PlayerPosHead.y - (HBox * 0.15f);
            float vidaPorcentaje = std::clamp(ESPHealthInt / 200.0f, 0.0f, 1.0f);

            ImU32 colorU32;
            if (vidaPorcentaje > 0.66f) colorU32 = IM_COL32(0, 255, 0, 255);
            else if (vidaPorcentaje > 0.33f) colorU32 = IM_COL32(255, 165, 0, 255);
            else                             colorU32 = IM_COL32(255, 0, 0, 255);

            float barWidth = 2.0f;
            float gap = 4.0f;
            float barX = XBox - gap;
            float barY = YBox;
            float filledHeight = HBox * vidaPorcentaje;

            
            vList->AddRectFilled(
                ImVec2(barX, barY),
                ImVec2(barX + barWidth, barY + HBox),
                IM_COL32(0, 0, 0, 150));

           
            vList->AddRectFilled(
                ImVec2(barX, barY + HBox - filledHeight),
                ImVec2(barX + barWidth, barY + HBox),
                colorU32);

            
            if (glowMode) {
                ImU32 glowColor1 = (colorU32 & 0x00FFFFFF) | (0x60 << 24); // ~38% alpha
                ImU32 glowColor2 = (colorU32 & 0x00FFFFFF) | (0x30 << 24); // ~19% alpha
                ImU32 glowColor3 = (colorU32 & 0x00FFFFFF) | (0x15 << 24); // ~8%  alpha

                float gY = barY + HBox - filledHeight;

                vList->AddRectFilled(
                    ImVec2(barX - 1, gY - 1),
                    ImVec2(barX + barWidth + 1, barY + HBox + 1),
                    glowColor1, 2.0f);

                vList->AddRectFilled(
                    ImVec2(barX - 2, gY - 2),
                    ImVec2(barX + barWidth + 2, barY + HBox + 2),
                    glowColor2, 3.0f);

                vList->AddRectFilled(
                    ImVec2(barX - 3, gY - 3),
                    ImVec2(barX + barWidth + 3, barY + HBox + 3),
                    glowColor3, 4.0f);
            }

            vList->PopClipRect();
        }

        if (ESPHealthMAX && !IsTeam)
        {
            HWND hwnd = FindWindowA(NULL, "MSI App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "BlueStacks App Player");
            if (!hwnd) return;

            RECT clientRect, windowRect;
            GetClientRect(hwnd, &clientRect);
            GetWindowRect(hwnd, &windowRect);

            int emulatorWidth = clientRect.right - clientRect.left;
            int emulatorHeight = clientRect.bottom - clientRect.top;
            int clientX = windowRect.left;
            int clientY = windowRect.top + (windowRect.bottom - windowRect.top - emulatorHeight);

            int EspHealthInt = -1;
            uint32_t dataPool, poolObj, pool;

            if (ReadZ(entity + Player_Data, dataPool) &&
                ReadZ(dataPool + 0x8, poolObj) &&
                ReadZ(poolObj + 0x10, pool) &&
                ReadZ(pool + 0x10, EspHealthInt))
            {
                EspHealthInt = std::clamp(EspHealthInt, 0, 200);
            }
            else {
                EspHealthInt = -1;
            }

            if (EspHealthInt >= 0)
            {
                ImDrawList* vList = ImGui::GetForegroundDrawList();
                vList->PushClipRect(ImVec2(clientX, clientY),
                    ImVec2(clientX + emulatorWidth, clientY + emulatorHeight), true);

                float scaleFactor = std::clamp(100.0f / (DistanceA + 100.0f), 0.2f, 0.7f);
                float WBox = CornerWidth * scaleFactor;
                float HBox = CornerHeight * scaleFactor * 1.4f;
                float XBox = PlayerPosHead.x - (WBox / 2.0f);
                float YBox = PlayerPosHead.y;
                float vidaPorcentaje = std::clamp(static_cast<float>(EspHealthInt) / 200.0f, 0.0f, 1.0f);

                ImU32 colorU32;
                if (vidaPorcentaje > 0.66f) colorU32 = IM_COL32(0, 255, 0, 255);
                else if (vidaPorcentaje > 0.33f) colorU32 = IM_COL32(255, 165, 0, 255);
                else                             colorU32 = IM_COL32(255, 0, 0, 255);

                ImU32 backgroundColor = IM_COL32(0, 0, 0, 255);

                float barHeight = 2.0f;
                float barX = XBox;
                float barY = YBox + HBox + 3.0f;
                float barWidth = WBox;
                float filledWidth = barWidth * vidaPorcentaje;
                float shadowPad = 2.0f;
                float shadowThickness = 12.0f;

               
                vList->AddRectFilled({ barX, barY },
                    { barX + barWidth, barY + barHeight },
                    backgroundColor, 0.5f);

               
                if (glowMode) {
                    ImU32 glowColor1 = (colorU32 & 0x00FFFFFF) | (0x60 << 24); // ~38% alpha
                    ImU32 glowColor2 = (colorU32 & 0x00FFFFFF) | (0x30 << 24); // ~19% alpha
                    ImU32 glowColor3 = (colorU32 & 0x00FFFFFF) | (0x15 << 24); // ~8%  alpha

                    vList->AddRectFilled(
                        { barX - shadowPad, barY - shadowPad },
                        { barX + filledWidth + shadowPad, barY + barHeight + shadowPad },
                        glowColor1, 3.0f);

                    vList->AddRectFilled(
                        { barX - shadowPad * 2, barY - shadowPad * 2 },
                        { barX + filledWidth + shadowPad * 2, barY + barHeight + shadowPad * 2 },
                        glowColor2, 4.0f);

                    vList->AddRectFilled(
                        { barX - shadowPad * 3, barY - shadowPad * 3 },
                        { barX + filledWidth + shadowPad * 3, barY + barHeight + shadowPad * 3 },
                        glowColor3, 5.0f);
                }

                // Barra de vida rellena
                vList->AddRectFilled({ barX, barY },
                    { barX + filledWidth, barY + barHeight },
                    colorU32, 0.5f);

                vList->PopClipRect();
            }
        }

        static BonePositions savedBones;
        static bool selfMarkInitialized = false;

        if (PuntoESP) {
            if (!selfMarkInitialized) {
                savedBones.Cabeza = GetBonePos(localPlayer, (uint32_t)Head);
                savedBones.Columna = GetBonePos(localPlayer, (uint32_t)Spine);
                savedBones.Cadera = GetBonePos(localPlayer, (uint32_t)Hip);
                savedBones.HombroDerecho = GetBonePos(localPlayer, (uint32_t)RightSholder);
                savedBones.HombroIzquierdo = GetBonePos(localPlayer, (uint32_t)LeftSholder);
                savedBones.CodoDerecho = GetBonePos(localPlayer, (uint32_t)RightElbow);
                savedBones.CodoIzquierdo = GetBonePos(localPlayer, (uint32_t)LeftElbow);
                savedBones.ManoDerecha = GetBonePos(localPlayer, (uint32_t)RightHand);
                savedBones.ManoIzquierda = GetBonePos(localPlayer, (uint32_t)LeftHand);
                savedBones.PieDerecho = GetBonePos(localPlayer, (uint32_t)RightFoot);
                savedBones.PieIzquierdo = GetBonePos(localPlayer, (uint32_t)LeftFoot);
                selfMarkInitialized = true;
            }
            auto W2S = [&](Vector3 world) {
                ImVec2 s = WorldToScreenImVec2(viewMatrix, world, width, height);
                return ImVec2(x + s.x, y + s.y);
                };
            ImVec2 sHead = W2S(savedBones.Cabeza), sCuello = W2S(savedBones.Columna), sCadera = W2S(savedBones.Cadera);
            ImVec2 sPieD = W2S(savedBones.PieDerecho), sPieI = W2S(savedBones.PieIzquierdo);
            ImVec2 sHomD = W2S(savedBones.HombroDerecho), sCodD = W2S(savedBones.CodoDerecho), sManD = W2S(savedBones.ManoDerecha);
            ImVec2 sHomI = W2S(savedBones.HombroIzquierdo), sCodI = W2S(savedBones.CodoIzquierdo), sManI = W2S(savedBones.ManoIzquierda);
            auto* drawList = ImGui::GetForegroundDrawList();
            std::vector<std::pair<ImVec2, ImVec2>> skeleton = {
                {sHead, sCuello}, {sCuello, sCadera},
                {sCadera, sPieD}, {sCadera, sPieI},
                {sCuello, sHomD}, {sHomD, sCodD}, {sCodD, sManD},
                {sCuello, sHomI}, {sHomI, sCodI}, {sCodI, sManI}
            };
            for (const auto& line : skeleton) {
                drawList->AddLine(line.first, line.second, IM_COL32(0, 0, 255, 200), 0.2f);
            }
        }
        else { selfMarkInitialized = false; }
        if (ESPCercania && !IsTeam)
        {
            RECT rect;
            GetWindowRect(hdPlayerWindow, &rect);
            int x = rect.left;
            int y = rect.top;
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            auto* drawList = ImGui::GetBackgroundDrawList();
            float closestScreenDistance = FLT_MAX;
            float closestPlayerDistance = FLT_MAX;
            ImVec2 closestHeadScreenPos = ImVec2(0, 0);
            for (int i = 0; i < entitiesCount; i++)
            {
                uint32_t entity;
                if (!ReadZ((uintptr_t)(i * 0x4 + entities), entity) || !entity || entity == localPlayer)
                    continue;
                bool isDead = false, IsTeamEntity = false, isVisible = false;
                ReadZ(entity + Player_IsDead, isDead);
                if (isDead) continue;
                uint32_t avatarManager, avatar, avatarData;
                if (!ReadZ(entity + AvatarManager, avatarManager) || !avatarManager) continue;
                if (!ReadZ(avatarManager + Avatar, avatar) || !avatar) continue;
                if (!ReadZ(avatar + Avatar_Data, avatarData) || !avatarData) continue;
                ReadZ(avatarData + Avatar_Data_IsTeam, IsTeamEntity);
                if (IsTeamEntity) continue;
                if (!ReadZ(avatar + Avatar_IsVisible, isVisible) || !isVisible) continue;
                BonePositions bonePositions;
                uint32_t headBone;
                if (!ReadZ(entity + Head, headBone) || !headBone) continue;
                GetNodePosition(headBone, bonePositions.Cabeza);
                ImVec2 headScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Cabeza, width, height);
                float screenCenterX = width / 2.0f;
                float screenCenterY = height / 2.0f;
                // OPTIMIZACIÓN: Usar distancia al cuadrado para comparaciones
                float distanceToCrosshairSquared = GetDistanceSquared(headScreenPos, ImVec2(screenCenterX, screenCenterY));
                float distanceToCrosshair = sqrtf(distanceToCrosshairSquared);
                Vector3 localHeadPos;
                uint32_t localHeadBone;
                ReadZ(localPlayer + Head, localHeadBone);
                GetNodePosition(localHeadBone, localHeadPos);
                float distanceToPlayer = Vector3::Distance(localHeadPos, bonePositions.Cabeza);
                if (distanceToCrosshair < closestScreenDistance ||
                    (distanceToCrosshair == closestScreenDistance && distanceToPlayer < closestPlayerDistance))
                {
                    closestScreenDistance = distanceToCrosshair;
                    closestPlayerDistance = distanceToPlayer;
                    closestHeadScreenPos = ImVec2(x + headScreenPos.x, y + headScreenPos.y);
                }
            }
            if (closestScreenDistance < FLT_MAX)
            {
                ImVec2 startPoint = ImVec2(x + width / 2, y + height / 2);
                ImVec2 endPoint = closestHeadScreenPos;
                ImU32 colorLine = ImColor(lineaverdexdddd);
                drawList->AddLine(startPoint, endPoint, colorLine, 0.8f);
            }
        }
        if (ESPNameZ && !IsTeam) {
            HWND hwnd = FindWindowA(NULL, "MSI App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "BlueStacks App Player");
            if (!hwnd) return;
            RECT clientRect, windowRect;
            GetClientRect(hwnd, &clientRect);
            GetWindowRect(hwnd, &windowRect);
            int emulatorWidth = clientRect.right - clientRect.left;
            int emulatorHeight = clientRect.bottom - clientRect.top;
            int clientX = windowRect.left;
            int clientY = windowRect.top + (windowRect.bottom - windowRect.top - emulatorHeight);
            auto vList = ImGui::GetForegroundDrawList();
            vList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emulatorWidth, clientY + emulatorHeight), true);
            std::string styledName = "BOT-Training";
            uint32_t nameAddr;
            if (ReadZ(entity + Player_Name, nameAddr) && nameAddr != 0) {
                std::string scannedName = ReadPlayerName(nameAddr);
                if (!scannedName.empty()) styledName = scannedName;
            }
            int randomValue = 35 + (entity % (65 - 0 + 1));
            std::string idNumberStr = std::to_string(randomValue);
            ImGui::PushFont(font::fuentezada);
            ImVec2 nameSize = ImGui::CalcTextSize(styledName.c_str());
            ImVec2 numSize = ImGui::CalcTextSize(idNumberStr.c_str());
            float paddingX = 2.5f;
            float paddingY = 1.5f;
            float gap = 2.0f;
            float rounding = 2.2f;

            auto GetColorFromNumber = [](int val) -> ImU32 {
                int r = (val * 37 + 23) % 256;
                int g = (val * 73 + 11) % 256;
                int b = (val * 91 + 5) % 256;
                return IM_COL32(r, g, b, 255);
                };

            ImU32 levelboxColor = GetColorFromNumber(randomValue);

            ImU32 colBlack = IM_COL32(0, 0, 0, 0);
            ImU32 colWhite = IM_COL32(255, 255, 255, 255);
            float dynamicBoxWidth = numSize.x + (paddingX * 2);
            float blackBoxWidth = nameSize.x + (paddingX * 2);
            float boxHeight = nameSize.y + (paddingY * 2);
            float totalWidth = dynamicBoxWidth + gap + blackBoxWidth;
            ImVec2 startPos(x + headScreenPos.x - totalWidth / 2, y + headScreenPos.y - 25.0f);
            ImVec2 dynRectMin = startPos;
            ImVec2 dynRectMax = ImVec2(startPos.x + dynamicBoxWidth, startPos.y + boxHeight);
            vList->AddRectFilled(dynRectMin, dynRectMax, levelboxColor, rounding);
            vList->AddText(font::fuentezada, ImGui::GetFontSize(), ImVec2(dynRectMin.x + paddingX, dynRectMin.y + paddingY), colWhite, idNumberStr.c_str());
            ImVec2 blackRectMin = ImVec2(dynRectMax.x + gap, startPos.y);
            ImVec2 blackRectMax = ImVec2(blackRectMin.x + blackBoxWidth, startPos.y + boxHeight);
            vList->AddRectFilled(blackRectMin, blackRectMax, colBlack, rounding);
            vList->AddText(font::fuentezada, ImGui::GetFontSize(), ImVec2(blackRectMin.x + paddingX, blackRectMin.y + paddingY), colWhite, styledName.c_str());
            ImGui::PopFont();
            vList->PopClipRect();
        }

        // === BÚSQUEDA DE ENEMIGO MÁS CERCANO AL CENTRO PARA FOV ===
        static HWND hwndEmulatorFov = nullptr;
        if (!hwndEmulatorFov || !IsWindow(hwndEmulatorFov)) {
            hwndEmulatorFov = FindRenderWindow();
        }

        static float closestDistanceFov = FLT_MAX;
        static Vector3 bestBoneWorldFov;
        static uint32_t bestEntityFov = 0;

        if (ShowFov && hwndEmulatorFov)
        {
            RECT clientRect;
            POINT screenPos = { 0, 0 };
            GetClientRect(hwndEmulatorFov, &clientRect);
            ClientToScreen(hwndEmulatorFov, &screenPos);

            int emulatorWidth = clientRect.right;
            int emulatorHeight = clientRect.bottom;
            int clientX = screenPos.x;
            int clientY = screenPos.y;

            ImVec2 screenCenter(emulatorWidth / 2.0f, emulatorHeight / 2.0f);

            closestDistanceFov = FLT_MAX;
            bestEntityFov = 0;

            // Buscar enemigo más cercano al centro
            for (int i = 0; i < entitiesCount; ++i)
            {
                uint32_t entry = entities + (i * 0x10);

                int hash = 0;
                ReadZ(entry + 0x0, hash);
                if (hash < 0) continue;

                uint32_t entity = 0;
                ReadZ(entry + 0x0C, entity);
                if (entity == 0) continue;
                if (entity == localPlayer) continue;

                bool isVisible = false, isTeam = false, isDead = false, isKnocked = false;
                uint32_t avatarManager, avatar, avatarData;

                if (ReadZ(entity + AvatarManager, avatarManager) &&
                    ReadZ(avatarManager + Avatar, avatar) &&
                    ReadZ(avatar + Avatar_Data, avatarData)) {
                    ReadZ(avatar + Avatar_IsVisible, isVisible);
                    ReadZ(avatarData + Avatar_Data_IsTeam, isTeam);
                }

                ReadZ(entity + Player_IsDead, isDead);

                uint32_t shadowBase = 0;
                if (ReadZ(entity + Player_ShadowBase, shadowBase) && shadowBase != 0) {
                    int pose;
                    if (ReadZ(shadowBase + XPose, pose) && pose == 8)
                        isKnocked = true;
                }

                if (isKnocked || isDead || !isVisible || isTeam) continue;

                // Obtener bone según selección
                uint32_t selectedBone;
                Vector3 bonePos;

                switch (rotation_combo)
                {
                case 0: // Cabeza
                    if (!ReadZ(entity + Head, selectedBone)) continue;
                    break;
                case 1: // Columna/Cuello
                    if (!ReadZ(entity + Spine, selectedBone)) continue;
                    break;
                case 2: // Cadera
                    if (!ReadZ(entity + Hip, selectedBone)) continue;
                    break;
                }

                if (!GetNodePosition(selectedBone, bonePos)) continue;

                // Convertir a pantalla
                ImVec2 targetPos = WorldToScreenImVec2(viewMatrix, bonePos, emulatorWidth, emulatorHeight);
                bool isOnScreen = targetPos.x > 0 && targetPos.y > 0 && targetPos.x < emulatorWidth && targetPos.y < emulatorHeight;

                if (!isOnScreen) continue;

                // Calcular distancia al centro
                float dx = targetPos.x - screenCenter.x;
                float dy = targetPos.y - screenCenter.y;
                float distanceToCrosshair = sqrtf(dx * dx + dy * dy);

                // Seleccionar el más cercano al centro
                if (distanceToCrosshair < closestDistanceFov)
                {
                    closestDistanceFov = distanceToCrosshair;
                    bestBoneWorldFov = bonePos;
                    bestEntityFov = entity;
                }
            }

            // DIBUJAR LÍNEA VERDE AL ENEMIGO MÁS CERCANO
            if (bestEntityFov != 0)
            {
                ImVec2 targetPos = WorldToScreenImVec2(viewMatrix, bestBoneWorldFov, emulatorWidth, emulatorHeight);
                ImVec2 adjustedTargetPos = ImVec2(clientX + targetPos.x, clientY + targetPos.y);
                ImVec2 screenCenter(clientX + emulatorWidth / 2.0f, clientY + emulatorHeight / 2.0f);

                ImDrawList* draw = ImGui::GetBackgroundDrawList();
                draw->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emulatorWidth, clientY + emulatorHeight), true);
                ImU32 predictionColor = IM_COL32(0, 255, 0, 255);
                draw->AddLine(screenCenter, adjustedTargetPos, predictionColor, 2.0f);
                draw->PopClipRect();

                // DIBUJAR CUADRO DE INFORMACIÓN EN LA PARTE INFERIOR CENTRAL
                if (ESPInfoBox)
                {
                    uint32_t nameAddr;
                    std::string playerName = "Unknown";
                    if (ReadZ(bestEntityFov + Player_Name, nameAddr) && nameAddr != 0) {
                        playerName = ReadPlayerName(nameAddr);
                        if (playerName.empty()) playerName = "Unknown";
                    }

                    // Obtener HP del enemigo
                    int enemyHP = 0;
                    uint32_t dataPool, poolObj, pool;
                    if (ReadZ(bestEntityFov + Player_Data, dataPool) && dataPool != 0) {
                        if (ReadZ(dataPool + 0x8, poolObj) && poolObj != 0) {
                            if (ReadZ(poolObj + 0x10, pool) && pool != 0) {
                                ReadZ(pool + 0xC, enemyHP);
                            }
                        }
                    }

                    // Calcular distancia
                    float distanceToEnemy = Vector3::Distance(mainPos, bestBoneWorldFov);

                    // Crear texto del cuadro
                    char infoText[256];
                    sprintf_s(infoText, sizeof(infoText), "Asmodeus Test | HP : %d | Distance : %.1fM", enemyHP, distanceToEnemy);

                    // Dibujar cuadro en la parte inferior central
                    ImGui::PushFont(font::fuentezada);
                    ImVec2 textSize = ImGui::CalcTextSize(infoText);
                    float boxWidth = textSize.x + 20.0f;
                    float boxHeight = textSize.y + 10.0f;
                    float boxX = clientX + (emulatorWidth - boxWidth) / 2.0f;
                    float boxY = clientY + emulatorHeight - boxHeight - 20.0f;

                    ImDrawList* infoDraw = ImGui::GetBackgroundDrawList();
                    infoDraw->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emulatorWidth, clientY + emulatorHeight), true);

                    // Fondo del cuadro (Blanco semitransparente como en la foto)
                    ImU32 bgColor = IM_COL32(255, 255, 255, 180);
                    infoDraw->AddRectFilled(ImVec2(boxX, boxY), ImVec2(boxX + boxWidth, boxY + boxHeight), bgColor, 0.0f);
                    
                    // Borde del cuadro (Sin borde prominente, o blanco/gris suave)
                    ImU32 borderColor = IM_COL32(200, 200, 200, 200);
                    infoDraw->AddRect(ImVec2(boxX, boxY), ImVec2(boxX + boxWidth, boxY + boxHeight), borderColor, 0.0f, 0, 1.0f);

                    // Texto (Negro)
                    ImU32 textColor = IM_COL32(0, 0, 0, 255);
                    ImVec2 textPos = ImVec2(boxX + 10.0f, boxY + 5.0f);
                    infoDraw->AddText(textPos, textColor, infoText);

                    infoDraw->PopClipRect();
                    ImGui::PopFont();
                }
            }
        }

        if (AimbotRotation && !IsTeam)
        {
            static HWND hwndEmulator = nullptr;
            if (!hwndEmulator || !IsWindow(hwndEmulator)) {
                hwndEmulator = FindRenderWindow();
            }

            if (!hwndEmulator) continue;

            RECT clientRect;
            POINT screenPos = { 0, 0 };
            GetClientRect(hwndEmulator, &clientRect);
            ClientToScreen(hwndEmulator, &screenPos);

            int emulatorWidth = clientRect.right;
            int emulatorHeight = clientRect.bottom;
            int clientX = screenPos.x;
            int clientY = screenPos.y;

            auto drawList = ImGui::GetForegroundDrawList();
            ImVec2 screenCenter(emulatorWidth / 2.0f, emulatorHeight / 2.0f);
            ImVec2 adjustedScreenCenter = ImVec2(clientX + screenCenter.x, clientY + screenCenter.y);

            // VARIABLES PARA ENCONTRAR EL MEJOR OBJETIVO
            float closestDistance = FLT_MAX;
            ImVec2 bestTargetPos = ImVec2(0, 0);
            Vector3 bestBoneWorld;
            bool targetInFov = false;
            uint32_t bestEntity = 0; // FIX: Guardar la entidad objetivo

            // Sistema de predicción de movimiento mejorado
            static std::unordered_map<uint32_t, Vector3> lastEnemyPositions;
            static std::unordered_map<uint32_t, Vector3> enemyVelocities;
            static std::unordered_map<uint32_t, Vector3> enemyAccelerations;
            static std::unordered_map<uint32_t, Vector3> lastEnemyVelocities;
            static auto lastPredictionUpdate = std::chrono::steady_clock::now();

            // Actualizar velocidades y aceleraciones cada 50ms
            auto now = std::chrono::steady_clock::now();
            auto timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastPredictionUpdate).count();
            if (timeSinceLastUpdate > 50) {
                lastPredictionUpdate = now;
                // Las velocidades y aceleraciones se calcularán en el loop
            }

            // BUSCAR EL OBJETIVO MÁS CERCANO DENTRO DEL FOV
            for (int i = 0; i < entitiesCount; ++i) {


                uint32_t entry = entities + (i * 0x10);

                int hash = 0;
                ReadZ(entry + 0x0, hash);
                if (hash < 0) continue;

                uint32_t entity = 0;
                ReadZ(entry + 0x0C, entity);
                if (entity == 0) continue;
                if (entity == localPlayer) continue;

                bool isVisible = false, isTeam = false, isDead = false, isKnocked = false;
                uint32_t avatarManager, avatar, avatarData;

                if (ReadZ(entity + AvatarManager, avatarManager) &&
                    ReadZ(avatarManager + Avatar, avatar) &&
                    ReadZ(avatar + Avatar_Data, avatarData)) {
                    ReadZ(avatar + Avatar_IsVisible, isVisible);
                    ReadZ(avatarData + Avatar_Data_IsTeam, isTeam);
                }

                // LEER ESTADO DEL ENEMIGO
                ReadZ(entity + Player_IsDead, isDead);

                uint32_t shadowBase = 0;
                if (ReadZ(entity + Player_ShadowBase, shadowBase) && shadowBase != 0) {
                    int pose;
                    if (ReadZ(shadowBase + XPose, pose) && pose == 8)
                        isKnocked = true;
                }

                // Filtrar enemigos no válidos
                if (isKnocked || isDead || !isVisible || isTeam) continue;

                // OBTENER EL BONE SELECCIONADO
                uint32_t selectedBone;
                Vector3 bonePos;

                switch (rotation_combo)
                {
                case 0: // Cabeza
                    if (!ReadZ(entity + Head, selectedBone)) continue;
                    break;
                case 1: // Columna/Cuello
                    if (!ReadZ(entity + Spine, selectedBone)) continue;
                    break;
                case 2: // Cadera
                    if (!ReadZ(entity + Hip, selectedBone)) continue;
                    break;
                }

                if (!GetNodePosition(selectedBone, bonePos)) continue;

                // Calcular velocidad y aceleración del enemigo para predicción mejorada
                if (timeSinceLastUpdate > 50) {
                    if (lastEnemyPositions.find(entity) != lastEnemyPositions.end()) {
                        Vector3 lastPos = lastEnemyPositions[entity];
                        Vector3 velocity = bonePos - lastPos;
                        
                        // Calcular aceleración basada en el cambio de velocidad
                        if (lastEnemyVelocities.find(entity) != lastEnemyVelocities.end()) {
                            Vector3 lastVel = lastEnemyVelocities[entity];
                            Vector3 acceleration = velocity - lastVel;
                            enemyAccelerations[entity] = acceleration;
                        }
                        
                        // Suavizar velocidad con promedio móvil
                        if (enemyVelocities.find(entity) != enemyVelocities.end()) {
                            Vector3 oldVel = enemyVelocities[entity];
                            Vector3 smoothedVel = (oldVel * 0.7f) + (velocity * 0.3f);
                            enemyVelocities[entity] = smoothedVel;
                        } else {
                            enemyVelocities[entity] = velocity;
                        }
                        
                        lastEnemyVelocities[entity] = velocity;
                    }
                    lastEnemyPositions[entity] = bonePos;
                }

                // Aplicar predicción de movimiento mejorada
                Vector3 predictedBonePos = bonePos;
                if (enemyVelocities.find(entity) != enemyVelocities.end()) {
                    Vector3 velocity = enemyVelocities[entity];
                    
                    // Calcular tiempo de predicción dinámico basado en la distancia
                    float distanceToPlayer = Vector3::Distance(mainPos, bonePos);
                    float predictionTime = 0.05f + (distanceToPlayer * 0.001f); // 50ms base + 1ms por metro
                    if (predictionTime > 0.3f) predictionTime = 0.3f; // Máximo 300ms
                    if (predictionTime < 0.05f) predictionTime = 0.05f; // Mínimo 50ms
                    
                    // Predicción con aceleración: posición + velocidad*t + 0.5*aceleración*t²
                    Vector3 acceleration = Vector3::Zero();
                    if (enemyAccelerations.find(entity) != enemyAccelerations.end()) {
                        acceleration = enemyAccelerations[entity];
                    }
                    
                    predictedBonePos = bonePos + (velocity * predictionTime) + (acceleration * 0.5f * predictionTime * predictionTime);
                }

                // CALCULAR DISTANCIA Y FILTRAR POR DISTANCIA MÁXIMA
                float distanceToPlayer = Vector3::Distance(mainPos, predictedBonePos);
                if (distanceToPlayer > DistanceRotation) continue;

                // Convertir a posición en pantalla
                ImVec2 targetPos = WorldToScreenImVec2(viewMatrix, predictedBonePos, emulatorWidth, emulatorHeight);
                bool isOnScreen = targetPos.x > 0 && targetPos.y > 0 && targetPos.x < emulatorWidth && targetPos.y < emulatorHeight;

                if (!isOnScreen) continue;

                // Calcular distancia al centro del FOV
                float dx = targetPos.x - screenCenter.x;
                float dy = targetPos.y - screenCenter.y;
                float distanceToCrosshair = sqrtf(dx * dx + dy * dy);

                // Verificar si está dentro del FOV
                bool isInsideFov = (distanceToCrosshair <= FovAll);

                if (isInsideFov) {
                    targetInFov = true;

                    // Seleccionar el objetivo más cercano al centro del FOV
                    if (distanceToCrosshair < closestDistance) {
                        closestDistance = distanceToCrosshair;
                        bestTargetPos = ImVec2(clientX + targetPos.x, clientY + targetPos.y);
                        bestBoneWorld = bonePos;
                        bestEntity = entity; // FIX: Guardar la entidad objetivo
                    }
                }
            }

            

            // DIBUJAR LÍNEA DE PREDICCIÓN SOLO AL MEJOR OBJETIVO
            if (closestDistance < FLT_MAX && bestEntity != 0)
            {
                // Leer el bone seleccionado para dibujar la línea
                uint32_t selectedBone;
                switch (aimbot_bone_combo) {
                    case 0: // Head
                        if (!ReadZ<uint32_t>(bestEntity + Head, selectedBone)) selectedBone = 0;
                        break;
                    case 1: // Neck/Spine
                        if (!ReadZ<uint32_t>(bestEntity + Spine, selectedBone)) selectedBone = 0;
                        break;
                    case 2: // Chest (usamos Hip como aproximación)
                        if (!ReadZ<uint32_t>(bestEntity + Hip, selectedBone)) selectedBone = 0;
                        break;
                    case 3: // Hip
                        if (!ReadZ<uint32_t>(bestEntity + Hip, selectedBone)) selectedBone = 0;
                        break;
                    default:
                        if (!ReadZ<uint32_t>(bestEntity + Head, selectedBone)) selectedBone = 0;
                        break;
                }

                if (selectedBone != 0) {
                    Vector3 bonePos;
                    if (GetNodePosition(selectedBone, bonePos)) {
                        ImVec2 boneScreenPos = WorldToScreenImVec2(viewMatrix, bonePos, emulatorWidth, emulatorHeight);
                        ImVec2 adjustedBonePos = ImVec2(clientX + boneScreenPos.x, clientY + boneScreenPos.y);
                        drawList->AddLine(adjustedScreenCenter, adjustedBonePos, ImColor(lineaverdexdddd), 1.5f);
                    }
                }
            }

            // LEER SI ESTÁ DISPARANDO
            bool isFiring = false;
            ReadZ(localPlayer + isFiringOffset, isFiring);

            // DETECTAR DISPARO REAL Y VERIFICAR SI ES SNIPER
            static std::chrono::steady_clock::time_point aimingStartTime;
            static bool wasAiming = false;
            static bool isActuallyFiring = false;

            uint32_t currentWeaponID = zGetInfoW(entity, true);
            // IDs de Snipers comunes: AWM(4), KAR98K(21), M82B(45), AWM-Y(65), M24(75), VSK94(197)
            bool isSniper = (currentWeaponID == 4 || currentWeaponID == 21 || currentWeaponID == 45 || 
                             currentWeaponID == 65 || currentWeaponID == 75 || currentWeaponID == 197);

            if (isFiring && !wasAiming) {
                aimingStartTime = std::chrono::steady_clock::now();
                wasAiming = true;
                isActuallyFiring = false;
            }
            else if (!isFiring) {
                wasAiming = false;
                isActuallyFiring = false;
            }
            else if (wasAiming && isFiring) {
                auto now = std::chrono::steady_clock::now();
                auto aimingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(now - aimingStartTime).count();
                // Si es sniper, reaccionar instantáneamente (0ms de retraso). Si no, usar los 150ms habituales.
                if (isSniper || aimingDuration > 150) {
                    isActuallyFiring = true;
                }
            }

            
            if (isActuallyFiring && targetInFov && closestDistance < FLT_MAX && bestEntity != 0)
            {
                // Contador estático para rastrear tiros al cuello y el objetivo actual
                static int neckShotCounter = 0;
                static uint32_t lastTargetEntity = 0;

                // Resetear contador si cambió el objetivo
                if (lastTargetEntity != bestEntity) {
                    neckShotCounter = 0;
                    lastTargetEntity = bestEntity;
                }

                // Leer el bone seleccionado de la entidad objetivo guardada
                uint32_t selectedBone;
                
                // Si el bone seleccionado es Head y neckShotsCount > 0, aplicar lógica de tiros al cuello
                if (aimbot_bone_combo == 0 && neckShotsCount > 0) {
                    if (neckShotCounter < neckShotsCount) {
                        // Usar cuello para los primeros X tiros
                        if (!ReadZ<uint32_t>(bestEntity + Spine, selectedBone)) continue;
                        neckShotCounter++;
                    } else {
                        // Después de X tiros al cuello, usar cabeza
                        if (!ReadZ<uint32_t>(bestEntity + Head, selectedBone)) continue;
                    }
                } else {
                    // Selección normal del bone
                    switch (aimbot_bone_combo) {
                        case 0: // Head
                            if (!ReadZ<uint32_t>(bestEntity + Head, selectedBone)) continue;
                            break;
                        case 1: // Neck/Spine
                            if (!ReadZ<uint32_t>(bestEntity + Spine, selectedBone)) continue;
                            break;
                        case 2: // Chest (usamos Hip como aproximación)
                            if (!ReadZ<uint32_t>(bestEntity + Hip, selectedBone)) continue;
                            break;
                        case 3: // Hip
                            if (!ReadZ<uint32_t>(bestEntity + Hip, selectedBone)) continue;
                            break;
                        default:
                            if (!ReadZ<uint32_t>(bestEntity + Head, selectedBone)) continue;
                            break;
                    }
                }

                Vector3 targetBonePos;
                if (!GetNodePosition(selectedBone, targetBonePos)) continue;

                // Aplicar predicción de movimiento al aimbot
                Vector3 predictedTargetPos = targetBonePos;
                if (enemyVelocities.find(bestEntity) != enemyVelocities.end()) {
                    Vector3 velocity = enemyVelocities[bestEntity];
                    float predictionTime = 0.1f;
                    predictedTargetPos = targetBonePos + (velocity * predictionTime);
                }

                // Rotación objetivo hacia el bone seleccionado
                Quaternion targetRot = AimBZv2::GetRotationToLocation(predictedTargetPos, 0.0f, mainPos);

                // AIMBOT SILENT - Modificar dirección de las balas directamente
                if (aimSilentEnabled)
                {
                    // Intentar modificar la dirección de las balas usando offsets sAim
                    uint32_t sAim1 = 0, sAim2 = 0, sAim3 = 0, sAim4 = 0;
                    
                    // Intentar leer la cadena de offsets sAim
                    if (ReadZ(localPlayer + sAim1, sAim1) && sAim1 != 0)
                    {
                        if (ReadZ(sAim1 + sAim2, sAim2) && sAim2 != 0)
                        {
                            if (ReadZ(sAim2 + sAim3, sAim3) && sAim3 != 0)
                            {
                                if (ReadZ(sAim3 + sAim4, sAim4) && sAim4 != 0)
                                {
                                    // Escribir la rotación objetivo para modificar la dirección de las balas
                                    WriteZ(sAim4, targetRot);
                                }
                            }
                        }
                    }
                    
                    // Fallback: También escribir en AimRotation para asegurar que funcione
                    WriteZ(localPlayer + AimRotation, targetRot);
                }
                else
                {
                    // AIMBOT NORMAL - Modificar rotación de cámara
                    WriteZ(localPlayer + AimRotation, targetRot);
                }
            }
        }
        if (AimbotPlayerX && !IsTeam)
        {
            ImVec2 screenCenter(g_windowWidth / 2, g_windowHeight / 2);
            POINT middlePos;

            static std::unordered_map<uint32_t, float> originalYStored;
            static std::unordered_map<uint32_t, float> originalXStored;
            static auto lastFireTime = std::chrono::steady_clock::now();

            if (localPlayer == 0 || entities == 0 || entitiesCount <= 0)
                continue;

            float UpPlayerAltureAuto = 0.0f;
            float RightLeftPlayerPositionAuto = 0.0f;
        }
        



       

        if (UpCheck && !IsTeam)
        {
            uintptr_t bestTarget = 0;
            float closestDistance = FLT_MAX;
            {
                uint32_t entry = entities + (i * 0x10);

                int hash = 0;
                ReadZ(entry + 0x0, hash);
                if (hash < 0) continue;

                uint32_t entity = 0;
                ReadZ(entry + 0x0C, entity);
                if (entity == 0) continue;
                if (entity == localPlayer) continue;

                bool isDead = false;
                if (ReadZ(entity + Player_IsDead, isDead) && isDead) continue;
                bool isKnocked = false;
                uint32_t shadowBase;
                if (ReadZ(entity + Player_ShadowBase, shadowBase) && shadowBase)
                {
                    int xpose;
                    if (ReadZ(shadowBase + XPose, xpose) && xpose == 8) isKnocked = true;
                }
                if (isKnocked) continue;
                uint32_t enemyRootBonePtr;
                if (!ReadZ(entity + Root, enemyRootBonePtr)) continue;
                Vector3 enemyRootPos;
                if (!GetNodePosition(enemyRootBonePtr, enemyRootPos)) continue;
                float playerDistance = Vector3::Distance(mainPos, enemyRootPos);
                if (playerDistance > 100.0f) continue;
                if (playerDistance < closestDistance)
                {
                    closestDistance = playerDistance;
                    bestTarget = entity;
                }
            }
            if (bestTarget != 0)
            {
                uint32_t enemyRootBonePtr;
                if (ReadZ(bestTarget + Root, enemyRootBonePtr))
                {
                    uint32_t enemyTransformValue;
                    if (ReadZ(enemyRootBonePtr + 0x8, enemyTransformValue))
                    {
                        uint32_t enemyTransformObjPtr;
                        if (ReadZ(enemyTransformValue + 0x8, enemyTransformObjPtr))
                        {
                            uint32_t enemyMatrixValue;
                            if (ReadZ(enemyTransformObjPtr + 0x20, enemyMatrixValue))
                            {
                                Vector3 currentPos;
                                if (ReadZ<Vector3>(enemyMatrixValue + 0x60, currentPos))
                                {
                                    Vector3 upPos = currentPos + Vector3(0, UpX, 0);
                                    float targetHeight = mainPos.y + UpX;
                                    float deltaY = currentPos.y - mainPos.y;

                                    if (deltaY < (UpX * 0.2f))
                                    {
                                        Vector3 newPos = currentPos;
                                        newPos.y = mainPos.y + UpX;
                                        WriteZ<Vector3>(enemyMatrixValue + 0x60, newPos);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        // Aim Prediction independiente del ESP (activado con ShowFov)
        if (ShowFov == true)
        {
            static HWND hwndEmulator = nullptr;
            if (!hwndEmulator || !IsWindow(hwndEmulator))
                hwndEmulator = FindRenderWindow();

            if (hwndEmulator)
            {
                RECT clientRect;
                POINT screenPos = { 0, 0 };
                GetClientRect(hwndEmulator, &clientRect);
                ClientToScreen(hwndEmulator, &screenPos);

                int emulatorWidth = clientRect.right;
                int emulatorHeight = clientRect.bottom;
                int clientX = screenPos.x;
                int clientY = screenPos.y;

                float closestScreenDistance = FLT_MAX;
                float closestPlayerDistance = FLT_MAX;
                ImVec2 closestHeadScreenPos = ImVec2(0, 0);

                for (int i = 0; i < entitiesCount; i++)
                {
                    uint32_t entity;
                    if (!ReadZ((uintptr_t)(i * 0x4 + entities), entity) || entity == 0 || entity == localPlayer)
                        continue;

                    bool isVisible = false, isTeam = false, isDead = false;
                    uint32_t avatarManager, avatar, avatarData;

                    if (ReadZ(entity + AvatarManager, avatarManager) &&
                        ReadZ(avatarManager + Avatar, avatar) &&
                        ReadZ(avatar + Avatar_Data, avatarData))
                    {
                        ReadZ(avatar + Avatar_IsVisible, isVisible);
                        ReadZ(avatarData + Avatar_Data_IsTeam, isTeam);
                    }

                    if (isTeam) continue;
                    if (isDead) continue;

                    uint32_t headBone;
                    if (!ReadZ(entity + Head, headBone)) continue;

                    Vector3 headV3;
                    if (!GetNodePosition(headBone, headV3)) continue;

                    ImVec2 headScreenPos = WorldToScreenImVec2(viewMatrix, headV3, width, height);
                    if (headScreenPos.x <= 0 || headScreenPos.y <= 0)
                        continue;

                    float screenCenterX = width / 2.0f;
                    float screenCenterY = height / 2.0f;
                    float distanceToCrosshair = sqrtf(pow(headScreenPos.x - screenCenterX, 2) + pow(headScreenPos.y - screenCenterY, 2));

                    if (distanceToCrosshair > RadioMagnet)
                        continue;

                    if (distanceToCrosshair < closestScreenDistance)
                    {
                        closestScreenDistance = distanceToCrosshair;
                        closestHeadScreenPos = ImVec2(headScreenPos.x, headScreenPos.y);
                    }
                }

                if (closestScreenDistance < FLT_MAX)
                {
                    float screenCenterX = width / 2.0f;
                    float screenCenterY = height / 2.0f;

                    float deltaX = closestHeadScreenPos.x - screenCenterX;
                    float deltaY = closestHeadScreenPos.y - screenCenterY;

                    constexpr float scaleFactor = 0.05f;
                }
            }
        }

        if (UnderPlayerrr)
        {
            uint32_t localRootBonePtr;
            Vector3 localRootPos;

            
            if (ReadZ(localPlayer + Root, localRootBonePtr) && GetNodePosition(localRootBonePtr, localRootPos))
            {
                uint32_t localHeadBonePtr;
                Vector3 localHeadPos;

                if (ReadZ(localPlayer + Head, localHeadBonePtr) && GetNodePosition(localHeadBonePtr, localHeadPos))
                {
                   
                    Vector3 forwardDir = Vector3::Normalized(Vector3(localHeadPos.x, 0, localHeadPos.z) - Vector3(localRootPos.x, 0, localRootPos.z));
                    Vector3 rightDir = Vector3::Normalized(Vector3(forwardDir.x, 0, -forwardDir.x));

                   
                    float teleportDownDistance = 0.7f;
                    static bool isFrozen = false;
                    static Vector3 frozenPos = Vector3::Zero();

                    float t1s;
                    float t2s;
                  
                    uint32_t t1 = 0, t2 = 0, matrixPtr = 0;
                    if (!ReadZ(localRootBonePtr + 0x8, t1) || t1s == 0) return;
                    if (!ReadZ(t1 + 0x8, t2) || t2s == 0) return;
                    if (!ReadZ(t2 + 0x20, matrixPtr) || matrixPtr == 0) return;

                
                    Vector3 currentPos;
                    if (!ReadZ<Vector3>(matrixPtr + 0x80, currentPos)) return;

                    if (!isFrozen)
                    {
                        
                        Vector3 newPos = currentPos;
                        newPos.y -= teleportDownDistance;

                       
                        // REMOVIDO: sleep_for causaba caídas de FPS en el loop de renderizado
                        // Escribir una sola vez es suficiente
                        WriteZ<Vector3>(matrixPtr + 0x80, newPos);

                        frozenPos = newPos;
                        isFrozen = true;
                    }
                    else
                    {
                      
                        Vector3 posNow;
                        if (ReadZ<Vector3>(matrixPtr + 0x80, posNow))
                        {
                            float delta = Vector3::Distance(posNow, frozenPos);
                            if (delta > 0.5f)
                            {
                                isFrozen = false;
                                // REMOVIDO: sleep_for causaba caídas de FPS en el loop de renderizado
                                return;
                            }
                        }

                       
                        WriteZ<Vector3>(matrixPtr + 0x80, frozenPos);
                    }
                }
            }
        }

        
        static float visualRotation = 0.0f;

        if (SpringHack && localPlayer != 0)
        {
            uint32_t localRootBonePtr;
            if (ReadZ(localPlayer + Root, localRootBonePtr))
            {
                uint32_t localTransformValue;
                if (ReadZ(localRootBonePtr + 0x8, localTransformValue))
                {
                    uint32_t localTransformObjPtr;
                    if (ReadZ(localTransformValue + 0x8, localTransformObjPtr))
                    {
                        uint32_t localMatrixValue;
                        if (ReadZ(localTransformObjPtr + 0x20, localMatrixValue))
                        {
                            
                            visualRotation += 40.0f;
                            if (visualRotation > 360.0f) visualRotation = 0.0f;

                         
                            float rad = visualRotation * (3.14159265f / 180.0f);

                           
                            Vector4 rot;
                            rot.x = 0;              // No inclinación lateral
                            rot.y = sinf(rad / 2);  // Giro sobre el eje Y
                            rot.z = 0;              // No inclinación frontal
                            rot.w = cosf(rad / 2);  // Componente escalar

                           
                            WriteZ<Vector4>(localMatrixValue + 0x70, rot);
                        }
                    }
                }
            }
        }

        float newspeedcaida = 25.0f;
        if (caidarapida) {
            uint32_t playerAttributes = 0;
            if (ReadZ(localPlayer + 0x404, playerAttributes) && playerAttributes != 0)
            {
                WriteZ<float>(playerAttributes + 0x15C, newspeedcaida);
                WriteZ<float>(playerAttributes + 0x160, newspeedcaida);
            }
        }


        if (InfiniteBullents) {
            uint32_t weapon = 0;
            if (ReadZ(localPlayer + Weapon, weapon) && weapon != 0)
            {
                WriteZ(weapon + AmmoOffs, InfinitySliders);
            }
        }


        if (MedikitFast)
        {
            uint32_t XDE = 0;
            if (ReadZ(localPlayer + MedikitOffset, XDE) && XDE)
            {
                WriteZ<float>(XDE + 0x80, velocidadmedikit < 0.75f ? 0.75f : velocidadmedikit);
            }
        }
        

        static uint32_t lastWeapon = 0;
        static bool recoilPatched = false;

        if (NoRecoil) {
            uint32_t weapon = 0;
            if (ReadZ(localPlayer + Weapon, weapon) && weapon != 0) {
                // Solo actuar si el arma cambió o aún no hemos parcheado esta arma
                if (weapon != lastWeapon || !recoilPatched) {
                    uint32_t weaponData = 0;
                    if (ReadZ(weapon + WeaponData, weaponData) && weaponData != 0) {
                        // Valor aleatorio entre 0.000 y 0.015 para simular recoil mínimo
                        float fakeRecoil = ((float)(rand() % 16) / 1000.0f);
                        WriteZ<float>(weaponData + WeaponRecoil, fakeRecoil);

                        // Opcional: también eliminar spread si tienes el offset
                        // WriteZ<float>(weaponData + WeaponSpread, fakeRecoil);

                        lastWeapon = weapon;
                        recoilPatched = true;
                    }
                }
            }
            else {
                recoilPatched = false; // Reseteamos si no hay arma
            }
        }

        if (InstantFire)
        {
            uint32_t XDE = 0;
            if (ReadZ(localPlayer + LocalPlayerAttributes, XDE) && XDE)
            {
                WriteZ<float>(XDE + 0x17C, 0.2f);
            }
        }

        static bool isFlywallActive2 = false;
        static Vector3 flywallPosition2 = Vector3(0, 0, 0);
        static float flywallOffset2 = 1.9f;
        static int flywallTickDelay2 = 1;
        if (FlyWall) {
            uint32_t rootPtr = 0;
            if (localPlayer == 0) {}
            else if (!ReadZ<uint32_t>(localPlayer + (uint32_t)Root, rootPtr) || rootPtr == 0) {}
            else {
                uint32_t t1 = 0;
                if (!ReadZ<uint32_t>(rootPtr + 0x8, t1) || t1 == 0) {}
                else {
                    uint32_t t2 = 0;
                    if (!ReadZ<uint32_t>(t1 + 0x8, t2) || t2 == 0) {}
                    else {
                        uint32_t matrixPtr = 0;
                        if (!ReadZ<uint32_t>(t2 + 0x20, matrixPtr) || matrixPtr == 0) {}
                        else {
                            Vector3 currentPos;
                            if (!ReadZ<Vector3>(matrixPtr + 0x80, currentPos)) {}
                            else {
                                if (!isFlywallActive2) {
                                    Vector3 newPos = currentPos;
                                    newPos.y -= flywallOffset2;

                                    // REMOVIDO: sleep_for causaba caídas de FPS en el loop de renderizado
                                    // Escribir una sola vez es suficiente
                                    WriteZ<Vector3>(matrixPtr + 0x80, newPos);
                                    flywallPosition2 = newPos;
                                    isFlywallActive2 = true;
                                }
                                else {
                                    Vector3 posNow;
                                    if (ReadZ<Vector3>(matrixPtr + 0x80, posNow))
                                    {
                                        float delta = Vector3::Distance(posNow, flywallPosition2);
                                        if (delta > 0.3f)
                                        {
                                            isFlywallActive2 = false;
                                            // REMOVIDO: sleep_for causaba caídas de FPS en el loop de renderizado
                                        }
                                        else
                                        {
                                            WriteZ<Vector3>(matrixPtr + 0x80, flywallPosition2);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        else {
            isFlywallActive2 = false;
        }

        if (PullPlayer)
        {
            bool isCurrentlyFiring = false;
            if (ReadZ(localPlayer + isFiringOffset, isCurrentlyFiring) && isCurrentlyFiring)
                lastFireTime = std::chrono::steady_clock::now();
            bool isFiring = (std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now() - lastFireTime).count() < fireCooldownMs360);
            if (isFiring)
            {
                uint32_t bestEntity = 0;
                float closestCrosshairDist = FLT_MAX;
                ImVec2 screenCenter(width / 2.0f, height / 2.0f);
                uint32_t targetBoneOffsett;
                if (SelectedPullBone == 0) targetBoneOffsett = Head;
                else if (SelectedPullBone == 1) targetBoneOffsett = Spine;
                else targetBoneOffsett = Root;
                for (uint32_t i = 0; i < entitiesCount; ++i)
                {

                    uint32_t entityAddr = entities + (i * 0x10);

                    int hash = 0;
                    ReadZ(entityAddr + 0x0, hash);
                    if (hash < 0) continue;

                    
                    ReadZ(entityAddr + 0x0C, entityAddr);
                    if (entityAddr == 0) continue;
                    if (entityAddr == localPlayer) continue;

                 
                    bool isDead = false, isTeam = false;
                    if (!ReadZ(entityAddr + Player_IsDead, isDead) || isDead) continue;
                    uint32_t avatarManager, avatar, avatarData;
                    if (!ReadZ(entityAddr + AvatarManager, avatarManager) ||
                        !ReadZ(avatarManager + Avatar, avatar) ||
                        !ReadZ(avatar + Avatar_Data, avatarData)) continue;
                    if (!ReadZ(avatarData + Avatar_Data_IsTeam, isTeam) || isTeam) continue;
                    bool isKnocked = false;
                    uint32_t shadowBase = 0;
                    if (ReadZ(entityAddr + Player_ShadowBase, shadowBase) && shadowBase != 0) {
                        int pose;
                        if (ReadZ(shadowBase + XPose, pose) && pose == 8) isKnocked = true;
                    }
                    if (isKnocked) continue;
                    uint32_t boneAddr = 0;
                    if (!ReadZ(entityAddr + targetBoneOffsett, boneAddr)) continue;
                    Vector3 targetPos = {};
                    if (!GetNodePosition(boneAddr, targetPos)) continue;
                    float dist3D = Vector3::Distance(mainPos, targetPos);
                    if (dist3D > PullDis || dist3D < minDistanceToSkip) continue;
                    ImVec2 bone2D = WorldToScreenImVec2(viewMatrix, targetPos, width, height);
                    // OPTIMIZACIÓN: Usar distancia al cuadrado para comparaciones
                    float crosshairDistSquared = GetDistanceSquared(bone2D, screenCenter);
                    float crosshairDist = sqrtf(crosshairDistSquared);
                    if (crosshairDist < screenPullRange && crosshairDist < closestCrosshairDist)
                    {
                        closestCrosshairDist = crosshairDist;
                        bestEntity = entityAddr;
                    }
                }

                if (bestEntity)
                {
                    currentPullTarget = bestEntity;
                    uint32_t boneToPull = 0, rootBone = 0;
                    if (ReadZ(bestEntity + targetBoneOffsett, boneToPull) && ReadZ(bestEntity + Root, rootBone))
                    {
                        Vector3 targetBonePos = {}, rootPos = {};
                        if (GetNodePosition(boneToPull, targetBonePos) && GetNodePosition(rootBone, rootPos))
                        {
                            uint32_t trans = 0, obj = 0, matrix = 0;
                            if (ReadZ(rootBone + 0x8, trans) && ReadZ(trans + 0x8, obj) && ReadZ(obj + 0x20, matrix))
                            {
                                if (originalPositions.find(bestEntity) == originalPositions.end())
                                    originalPositions[bestEntity] = rootPos;
                                Vector3 viewDir(viewMatrix.m02, viewMatrix.m12, viewMatrix.m22);
                                viewDir.Normalize();
                                float depth = Vector3::Distance(mainPos, targetBonePos);
                                Vector3 projectedPos = mainPos + (viewDir * depth);
                                Vector3 offset = projectedPos - targetBonePos;
                                pullTime += 0.025f;
                                float pullProgress = fminf(pullTime * suavidad360, 1.0f);
                                Vector3 pulledPos = rootPos + (offset * pullProgress);
                                WriteZ<Vector3>(matrix + 0x60, pulledPos);
                            }
                        }
                    }
                }
            }
            else if (!originalPositions.empty())
            {
                for (auto const& [entityID, pos] : originalPositions)
                {
                    uint32_t rootBone = 0;
                    if (ReadZ(entityID + Root, rootBone))
                    {
                        uint32_t trans = 0, obj = 0, matrix = 0;
                        if (ReadZ(rootBone + 0x8, trans) && ReadZ(trans + 0x8, obj) && ReadZ(obj + 0x20, matrix))
                            WriteZ<Vector3>(matrix + 0x60, pos);
                    }
                }
                originalPositions.clear();
                pullTime = 0.0f;
                currentPullTarget = 0;
            }
        }




    // --- Renderizado de Radar 2D sobre el minimapa ---
    if (EspRadar2D)
    {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        
        RECT rect;
        GetWindowRect(hwndEmul, &rect);
        int x = rect.left, y = rect.top, width = rect.right - rect.left, height = rect.bottom - rect.top;
        
        // Posición del minimapa (esquina superior derecha - posición aproximada)
        ImVec2 minimapPos = ImVec2(x + width - 170, y + 30);
        float minimapSize = 140.0f;
        
        // Dibujar contenedor circular del radar sobre el minimapa
        ImVec2 radarCenter = ImVec2(minimapPos.x + minimapSize / 2, minimapPos.y + minimapSize / 2);
        drawList->AddCircleFilled(radarCenter, minimapSize / 2, IM_COL32(20, 20, 20, 160));
        drawList->AddCircle(radarCenter, minimapSize / 2, IM_COL32(60, 60, 80, 220), 0, 1.5f);
        
        // Cruz central (Mira)
        drawList->AddLine(ImVec2(radarCenter.x - minimapSize / 2, radarCenter.y), ImVec2(radarCenter.x + minimapSize / 2, radarCenter.y), IM_COL32(80, 80, 80, 100), 1.0f);
        drawList->AddLine(ImVec2(radarCenter.x, radarCenter.y - minimapSize / 2), ImVec2(radarCenter.x, radarCenter.y + minimapSize / 2), IM_COL32(80, 80, 80, 100), 1.0f);

        // Buscar y pintar enemigos en el Radar 2D (siempre activo)
        Vector3 localPos = GetLocalPlayerPosition();
        for (int k = 0; k < entitiesCount; k++)
            {
                uint32_t entityAddr;
                if (!ReadZ((uintptr_t)(k * 0x4 + entities), entityAddr) || entityAddr == 0 || entityAddr == localPlayer)
                    continue;

                bool isDead = false;
                ReadZ(entityAddr + Player_IsDead, isDead);
                if (isDead) continue;

                uint32_t rootBone = 0;
                if (ReadZ(entityAddr + Root, rootBone) && rootBone != 0)
                {
                    Vector3 enemyPos;
                    if (GetNodePosition(rootBone, enemyPos))
                    {
                        float deltaX = enemyPos.x - localPos.x;
                        float deltaZ = enemyPos.z - localPos.z;

                        // Rotar posiciones en el radar según la dirección de la cámara (AimRotation)
                        Quaternion rotation;
                        if (ReadZ(localPlayer + AimRotation, rotation))
                        {
                            // Aproximación de yaw de cámara
                            float angle = atan2f(2.0f * (rotation.y * rotation.w - rotation.x * rotation.z), 1.0f - 2.0f * (rotation.y * rotation.y + rotation.z * rotation.z));
                            float cosYaw = cosf(-angle);
                            float sinYaw = sinf(-angle);

                            float rotatedX = deltaX * cosYaw - deltaZ * sinYaw;
                            float rotatedZ = deltaX * sinYaw + deltaZ * cosYaw;

                            // Escalar rango al radar (minimapa)
                            float scale = (minimapSize / 2) / EspRadarRange;
                            float dotX = radarCenter.x + (rotatedX * scale);
                            float dotY = radarCenter.y + (rotatedZ * scale);

                            // Verificar límites del radar circular
                            float distanceSquared = (dotX - radarCenter.x) * (dotX - radarCenter.x) + (dotY - radarCenter.y) * (dotY - radarCenter.y);
                            if (distanceSquared <= (minimapSize / 2) * (minimapSize / 2))
                            {
                                // Pintar punto del enemigo
                                drawList->AddCircleFilled(ImVec2(dotX, dotY), 3.0f, IM_COL32(255, 0, 0, 255));
                            }
                        }
                    }
                }
            }
        }
    }

    // ESP Crosshair - Mira personalizada
    if (ESPCrosshair)
    {
        RECT rect;
        GetWindowRect(hwndEmul, &rect);
        int x = rect.left;
        int y = rect.top;
        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        ImVec2 center = ImVec2(x + width / 2, y + height / 2);
        ImDrawList* draw = ImGui::GetBackgroundDrawList();
        draw->PushClipRect(ImVec2(x, y), ImVec2(x + width, y + height), true);

        ImU32 crosshairColor = ImGui::GetColorU32(ESPCrosshairColor);
        float size = ESPCrosshairSize;

        switch (ESPCrosshairStyle)
        {
        case 0: // Cross
            draw->AddLine(ImVec2(center.x - size, center.y), ImVec2(center.x + size, center.y), crosshairColor, 1.5f);
            draw->AddLine(ImVec2(center.x, center.y - size), ImVec2(center.x, center.y + size), crosshairColor, 1.5f);
            break;
        case 1: // Circle
            draw->AddCircle(center, size, crosshairColor, 32, 1.5f);
            draw->AddLine(ImVec2(center.x - size, center.y), ImVec2(center.x + size, center.y), crosshairColor, 1.5f);
            draw->AddLine(ImVec2(center.x, center.y - size), ImVec2(center.x, center.y + size), crosshairColor, 1.5f);
            break;
        case 2: // Dot
            draw->AddCircleFilled(center, 2.0f, crosshairColor);
            break;
        }

        draw->PopClipRect();
    }

}

// --- Función de validación de memoria segura ---
bool IsMemoryReadable(uintptr_t address)
{
    try
    {
        if (address == 0 || address < 0x1000) return false;
        
        MEMORY_BASIC_INFORMATION mbi;
        if (VirtualQuery((LPCVOID)address, &mbi, sizeof(mbi)) == 0)
            return false;
        
        if (mbi.State != MEM_COMMIT) return false;
        if (mbi.Protect == PAGE_NOACCESS || mbi.Protect == PAGE_GUARD) return false;
        
        return true;
    }
    catch (...)
    {
        return false;
    }
}

// --- Función separada para dibujar el FOV (solo cuando ShowFov está activado) ---
void DrawFOV()
{
    // Solo dibujar FOV si ShowFov está activado
    if (!ShowFov) return;
    
    try
    {
        static HWND hwndEmul = zGetHwndBsEx();
        if (!hwndEmul || !IsWindow(hwndEmul))
        {
            hwndEmul = zGetHwndBsEx();
        }
        
        if (hwndEmul)
        {
            RECT rect;
            GetWindowRect(hwndEmul, &rect);
            int x = rect.left;
            int y = rect.top;
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            ImVec2 fovCenter = ImVec2(x + width / 2, y + height / 2);
            ImDrawList* draw = ImGui::GetBackgroundDrawList();
            
            // Validar puntero antes de usarlo
            if (!draw) return;
            
            draw->PushClipRect(ImVec2(x, y), ImVec2(x + width, y + height), true);
            
            if (rgbMode)
            {
                DrawAnimatedRainbowFOVWithGlow(fovCenter, FovAll, 1.f, 0.f, 100, 0.0f);
            }
            else
            {
                ImU32 color = ImGui::GetColorU32(ColorFov);
                draw->AddCircle(fovCenter, FovAll, color, 100, 1.f);
            }
            
            draw->PopClipRect();
        }
    }
    catch (...)
    {
        // Silenciar errores para prevenir crash del panel
    }
}

// --- Función separada para dibujar el HUD de Keybinds (siempre activo) ---
void DrawKeybindsHUD()
{
    if (ShowKeybindsHUD)
    {
        try
        {
            // Contar funciones activas en tiempo real
            int activeCount = 0;
            if (AimbotRotation) activeCount++;
            if (NoRecoil) activeCount++;
            if (PullPlayer) activeCount++;
            if (UpCheck) activeCount++;

            // Dibujar el HUD siempre (incluso si no hay funciones activas)
            // Calcular altura dinámica de la ventana (mínimo para mostrar "No functions active")
            float rowHeight = 32.0f;
            float baseHeight = 55.0f;
            float windowHeight = baseHeight + ((activeCount > 0 ? activeCount : 1) * rowHeight);

            // Configuraciones de estilo temporales para que se parezca exactamente al de la foto
            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.08f, 0.85f)); // Fondo oscuro translúcido
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));       // Borde verde
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);                      // Bordes redondeados
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 1.0f);

            // Establecer tamaño dinámico flotante
            ImGui::SetNextWindowSize(ImVec2(240, windowHeight), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiCond_FirstUseEver);

            // Ventana sin título estándar, sin colapsar ni redimensionar de forma ordinaria para verse como HUD
            ImGui::Begin("KeybindsHUD", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
            {
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                ImVec2 windowPos = ImGui::GetWindowPos();
                ImVec2 windowSize = ImGui::GetWindowSize();

                // Dibujar la línea superior verde y el encabezado
                drawList->AddLine(
                    ImVec2(windowPos.x + 10, windowPos.y + 35),
                    ImVec2(windowPos.x + windowSize.x - 10, windowPos.y + 35),
                    IM_COL32(0, 204, 0, 255), 2.0f
                );

                // Título central
                ImGui::PushFont(font::fuentezada);
                ImVec2 titleSize = ImGui::CalcTextSize("asmodeus - Keybinds");
                ImGui::SetCursorPos(ImVec2((windowSize.x - titleSize.x) * 0.5f, 12.0f));
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "asmodeus - Keybinds");
                ImGui::PopFont();

                // Espaciado inicial del cuerpo
                ImGui::SetCursorPosY(48.0f);

                // Macro interna para pintar cada fila con su divisor inferior verde
                auto DrawKeybindRow = [&](const char* featureName, const char* stateStr, ImVec4 stateColor)
                    {
                        ImGui::SetCursorPosX(15.0f);

                        // Obtener posición del cursor para dibujar resplandor
                        ImVec2 cursorScreen = ImGui::GetCursorScreenPos();

                        // Dibujar resplandor verde detrás del texto
                        ImVec2 glowMin = ImVec2(windowPos.x + 8, cursorScreen.y - 2);
                        ImVec2 glowMax = ImVec2(windowPos.x + windowSize.x - 8, cursorScreen.y + 18);
                        drawList->AddRectFilled(glowMin, glowMax, IM_COL32(0, 255, 0, 30), 4.0f);

                        ImGui::Text(featureName);

                        ImGui::SameLine(windowSize.x - 65.0f);
                        ImGui::TextColored(stateColor, stateStr);

                        // Separador verde muy sutil
                        cursorScreen = ImGui::GetCursorScreenPos();
                        drawList->AddLine(
                            ImVec2(windowPos.x + 10, cursorScreen.y + 5),
                            ImVec2(windowPos.x + windowSize.x - 10, cursorScreen.y + 5),
                            IM_COL32(0, 204, 0, 40), 1.0f // Línea divisoria semi-transparente
                        );
                        ImGui::Dummy(ImVec2(0, 10)); // Espacio vertical
                    };

                ImVec4 activeColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Verde para activo
                ImVec4 inactiveColor = ImVec4(0.6f, 0.6f, 0.6f, 1.0f); // Gris para inactivo

                // Dibujar filas de funciones activas
                if (AimbotRotation)      DrawKeybindRow("Aimbot", "[ ON ]", activeColor);
                if (NoRecoil)           DrawKeybindRow("No Recoil", "[ ON ]", activeColor);
                if (PullPlayer)         DrawKeybindRow("Pull Player", "[ ON ]", activeColor);
                if (UpCheck)            DrawKeybindRow("Up - Player", "[ ON ]", activeColor);

                // Si no hay funciones activas, mostrar mensaje
                if (activeCount == 0)
                {
                    ImGui::SetCursorPosX(15.0f);
                    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.6f, 1.0f), "No functions active");
                }
            }
            ImGui::End();

            ImGui::PopStyleVar(2);
            ImGui::PopStyleColor(2);
        }
        catch (...)
        {
            // Silenciar errores para prevenir crash del panel
        }
    }
}