#include "vector.h"
#include "adb.h"

#include <imgui_internal.h>
#include <random>


double getCurrentTime() {
    auto now = std::chrono::high_resolution_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count() / 1000.0;
}

namespace font {
    inline ImFont* fuentezada = nullptr;
    inline ImFont* good = nullptr;

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
ImVec4 boxColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 healthColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 WeaponsColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 Weaponstext = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 bonesColor = ImVec4(255, 182, 193, 255);
ImVec4 bonesColorCIRCULOS = ImVec4(0, 128, 255, 255);
ImVec4 fovColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
ImVec4 crosshairColor = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
ImVec4 userColorAmarillo = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
inline ImVec4 ColorFov = ImColor(255, 255, 255);
ImVec4 lineColorTeam(0.0f, 1.0f, 0.0f, 1.0f);

const char* cheat_name = " ";
std::string EspADB = "/ostorga";
int g_windowWidth = GetSystemMetrics(SM_CXSCREEN);
int g_windowHeight = GetSystemMetrics(SM_CYSCREEN);
HWND hdPlayerWindow;
static auto clickStartTime = std::chrono::steady_clock::time_point();
bool PullPlayer = false;

inline namespace Main {
    bool EspMode;
    bool EnabledEsp = false;
}

int RadioMagnet = 200.0f;
std::chrono::steady_clock::time_point lastFireTime;
const int fireCooldownMs360 = 100; // Ajusta este valor seg?n necesites
float PullDis = 300.0f; // Distancia m?xima para pull
float PullFov = 300.0f; // FOV para el pull
float pullTime = 0.0f;
float suavidad360 = 2.0f; // Factor de suavidad

int VidaType = 0;
const char* items5[] = { "Right", "Left", "Below" };

inline namespace Combox {
    static int bypass_combo = 1;
    const char* bypass_list[] = { "Normal", "Max", "x86" };
}

int TpSelection = 0;
const char* listTp[] = { "Neck", "Head", "Chest" };

int SelectedPullBone = 0;
const char* PullBoneOptions[] = { "Head", "Neck" ,"Root" };

bool EnemyPullEnabledHead = false;
bool PuntoESP = false;
bool caidarapida = false;
bool enemyTeleport = false;
float enemydistancechupa = 0.0f;
float enemyplayerdistance = 5.0f;
float enemysidechupa = 0.0f;
bool ESPinfoZada = false;
bool ESPinfoZada2 = false;
bool ScopeOffset = false;
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
bool ESPBones = false, EspLineZ = false, ESPNameZ = false;
bool ShowFov = false, Shadow = false, rainbowMode = false, rgbMode = false;
inline static bool ShowTeam = false, ShowInvis = false, ShowWukongOrion = true;

bool AimbotRotation, AimbotRage, AimbotRage2, AimbotPlayerX, AimPrediction;
bool SilentHook, SilentHookG, aimbotHEX, alreadyReplaced = false;
bool NoRecoil = false, InstantFire = false, quickswitch = false, bugmira = false;
bool noreloadpro = false, MedkitHack = false, DamageFake = false, DamageFake2 = false, BugCamara = false, QuickSniper = false;
bool InfiniteBullents = false, MedikitFast = false, VisionHack = false;

// Nuevas funciones
bool InfiniteAmmo = false;
bool FastFire = false;
bool QuickSwitchHack = false;
bool MedikitHackNew = false;

// Variables de Asistencia de Apuntado (Aim Assist) - versión vieja
bool g_AimAssist = false;
bool g_AimTeammates = false;
int g_AimAssistType = 0; // 0: Memory Lock, 1: Memory Smooth, 2: Mouse Emulation, 3: Silent Aim
float g_AimSmoothness = 0.0f; // 0.0f - 1.0f (0 = instantáneo, 1 = súper suave/lento)
int g_ChestShotsCount = 0; // 0: Desactivado, 1-5: Tiros iniciales al pecho, luego cabeza
static int currentShotIndex = 0; // Contador de tiros actuales

// Variables de UI - versión vieja
bool g_ShowKeybinds = false;
bool g_ShowFps = false;

// Conectar g_AimAssist con AimbotRotation para compatibilidad
inline void UpdateAimbotCompatibility() {
    AimbotRotation = g_AimAssist;
    AimbotRage = g_AimTeammates;
}

bool PlayerZ = false, TpBaseCheck = false, UnderPlayerrr = false, FastFall = false;
bool TeleportBase = false, bTeleportEnabled = false, TpEnemyCheck = false;
bool NightMode = false, WallHackOffset = false;
bool TeleKCheck = false, TeleportMarkk = false, IgnoreAlture = false;
bool FlyWall = false, Rapidfall = false, isFrozen = false;
bool ESPinfo = false, ESPBox = false, ESPCercania = false, glowMode = false, SpringHack = false;
bool Bugmira = false, IsTeam = false, ESPID = false, ESPDisctanc = false;
bool mirabonita = false;
bool EspWeaponsICON = false, EspWeaponsTex = false, EspText = false;
bool UpCheck = false;
bool ESPHealth = false;
bool ESPHealthMAX = false;
bool ESPShield = false;
bool ESPDistance = false;
bool SpeedHackEnabled = false;
float SpeedHackMultiplier = 1.0f; // 1.0 a 1.7 (1.7 = x3)
bool isVisible = false, isDead = false, isKnocked = false, isCurrentlyFiring = false;
bool isFiring = false, QuickSwitchActive = false, BugCamaraActive = false;

// Hotkey system
int AimHotkey = 0; // 0 = none por defecto
int SpeedHotkey = 0; // 0 = none

static int aimbot_combo;
static constexpr const char* aimbot_list[] = { "Head", "Neck", "Leggit" };

static int rotation_combo;
static constexpr const char* rotation_list[] = { "Head", "Neck", "Chest" };

static int silent_combo;
static constexpr const char* silent_list[] = { "Normal", "360", "Fov Ultra" };

int SelectedESPBoxStyle = 1;
int SelectedAimBone = 0; // 0=Head, 1=Neck, 2=Chest
int SelectedLineStyle = 1; // 0=None, 1=Top, 2=Center, 3=Bottom
float aimSmooth = 0.0f;
static constexpr const char* ESPBoxOptions[5] = { "None", "Normal", "Filled", "Corners", "3D" };
static constexpr const char* AimBoneOptions[3] = { "Head", "Neck", "Chest" };

int SelectedTeleport = 0;


Quaternion QuaternionFromEuler(float pitch, float yaw, float roll) {
    float cy = cosf(roll * 0.5f);
    float sy = sinf(roll * 0.5f);
    float cp = cosf(pitch * 0.5f);
    float sp = sinf(pitch * 0.5f);
    float cyaw = cosf(yaw * 0.5f);
    float syaw = sinf(yaw * 0.5f);
    Quaternion q;
    q.w = cyaw * cp * cy + syaw * sp * sy;
    q.x = syaw * cp * cy - cyaw * sp * sy;
    q.y = cyaw * sp * cy + syaw * cp * sy;
    q.z = cyaw * cp * sy - syaw * sp * cy;
    return q;
}

Quaternion Slerp(Quaternion q1, Quaternion q2, float t) {
    float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;
    if (dot < 0.0f) {
        q2.x = -q2.x; q2.y = -q2.y; q2.z = -q2.z; q2.w = -q2.w;
        dot = -dot;
    }
    if (dot > 0.9995f) {
        Quaternion result;
        result.x = q1.x + t * (q2.x - q1.x);
        result.y = q1.y + t * (q2.y - q1.y);
        result.z = q1.z + t * (q2.z - q1.z);
        result.w = q1.w + t * (q2.w - q1.w);
        return result;
    }
    float theta0 = acosf(dot);
    float theta = theta0 * t;
    float sintheta = sinf(theta);
    float sintheta0 = sinf(theta0);
    float s0 = cosf(theta) - dot * sintheta / sintheta0;
    float s1 = sintheta / sintheta0;
    Quaternion result;
    result.x = s0 * q1.x + s1 * q2.x;
    result.y = s0 * q1.y + s1 * q2.y;
    result.z = s0 * q1.z + s1 * q2.z;
    result.w = s0 * q1.w + s1 * q2.w;
    return result;
}

// Estructura de una caja 3D
struct Box3D {
    Vector3 center;
    float height;
    float width;
    float depth;
};

// Obtener la caja 3D de un enemigo (centrada en pies, tamaño amplio)
Box3D GetEnemyBoxFromBones(const Vector3& headPos, const Vector3& rootPos) {
    Box3D box;
    box.center = rootPos;
    box.width = 0.8f; // Ancho ajustado para mejor ajuste al personaje
    box.depth = 0.8f; // Fondo ajustado para mejor ajuste al personaje

    if (headPos.y != 0 && rootPos.y != 0) {
        float headY = headPos.y;
        if (headY > rootPos.y) {
            box.height = (headY - rootPos.y) + 0.2f; // Hasta la cabeza + margen pequeño
        } else {
            box.height = 1.8f;
        }
    } else {
        box.height = 1.8f;
    }
    return box;
}

// Dibujar la caja 3D wireframe
void Draw3DBox(const Box3D& box, const Matrix4x4& viewMatrix, int screenWidth, int screenHeight, ImU32 color, int clientX, int clientY, ImDrawList* draw) {
    Vector3 vertices[8];
    float hw = box.width * 0.5f;
    float hd = box.depth * 0.5f;
    float yBottom = box.center.y;
    float yTop = box.center.y + box.height;

    vertices[0] = Vector3(box.center.x - hw, yBottom, box.center.z - hd);
    vertices[1] = Vector3(box.center.x + hw, yBottom, box.center.z - hd);
    vertices[2] = Vector3(box.center.x + hw, yBottom, box.center.z + hd);
    vertices[3] = Vector3(box.center.x - hw, yBottom, box.center.z + hd);
    vertices[4] = Vector3(box.center.x - hw, yTop, box.center.z - hd);
    vertices[5] = Vector3(box.center.x + hw, yTop, box.center.z - hd);
    vertices[6] = Vector3(box.center.x + hw, yTop, box.center.z + hd);
    vertices[7] = Vector3(box.center.x - hw, yTop, box.center.z + hd);

    ImVec2 screenVerts[8];
    int validCount = 0;
    for (int i = 0; i < 8; i++) {
        ImVec2 screen = WorldToScreenImVec2(viewMatrix, vertices[i], screenWidth, screenHeight);
        if (screen.x > 0 && screen.x < screenWidth && screen.y > 0 && screen.y < screenHeight) {
            validCount++;
        }
        screenVerts[i] = ImVec2(clientX + screen.x, clientY + screen.y);
    }
    // Dibujar si al menos 2 vértices son válidos (caja parcialmente visible)
    if (validCount < 2) return;

    float thickness = 2.0f; // Grosor aumentado para mejor visibilidad

    draw->AddLine(screenVerts[0], screenVerts[1], color, thickness);
    draw->AddLine(screenVerts[1], screenVerts[2], color, thickness);
    draw->AddLine(screenVerts[2], screenVerts[3], color, thickness);
    draw->AddLine(screenVerts[3], screenVerts[0], color, thickness);

    draw->AddLine(screenVerts[4], screenVerts[5], color, thickness);
    draw->AddLine(screenVerts[5], screenVerts[6], color, thickness);
    draw->AddLine(screenVerts[6], screenVerts[7], color, thickness);
    draw->AddLine(screenVerts[7], screenVerts[4], color, thickness);

    draw->AddLine(screenVerts[0], screenVerts[4], color, thickness);
    draw->AddLine(screenVerts[1], screenVerts[5], color, thickness);
    draw->AddLine(screenVerts[2], screenVerts[6], color, thickness);
    draw->AddLine(screenVerts[3], screenVerts[7], color, thickness);
}



float lineThicknessBase = 0.8f;
float NightIntensity = 0.7f;
inline static float velocidadcaida = 0.2f;
inline static float valueFire = 0.75f;
inline static float valuexde = 10.0f;
inline static float velocidadmedikit = 0.75f;

int maxDistance = 500;
int FovAll = 150;
int DistanceAll = 500;
int VisionSlider = 30;
int SecondsLag = 1;
int InfinitySliders = 555;
int TpDistance = 180;


int FovRage = 180;
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

// Dibuja un pol?gono con efecto de resplandor (glow) alrededor
static inline void PolyGlow(ImDrawList* dl, const ImVec2* pts, int count, ImU32 color, float strength = 6.0f, int layers = 6) {
    if (!dl || !pts || count <= 1 || layers <= 0 || strength <= 0.0f) return;
    for (int i = 1; i <= layers; ++i) {
        float t = (float)i / (float)layers;
        int a = (int)(255.0f * (0.10f * (1.0f - t)));
        ImU32 c = (color & 0x00FFFFFFu) | ((ImU32)a << 24);
        dl->AddPolyline(pts, count, c, ImDrawFlags_Closed, 1.0f + t * (strength * 0.15f));
    }
}

// Dibuja un rect?ngulo con efecto de resplandor (glow) alrededor
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
}




// Leer cadena wide (UTF-16) desde memoria del juego
std::wstring ReadWideString(uintptr_t nameAddr, size_t maxLength = 64) {
    std::wstring result;

    for (size_t i = 0; i < maxLength; i++) {
        uint16_t wchar_value = 0;
        if (!ReadZ(nameAddr + 0xC + (i * 2), wchar_value)) {
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

inline static bool s_weapons[7] = { false, false, false, false, false, false ,false, };
inline static const char* i_weapons[7] = { "AWM", "KAR98K", "M828", "AWM-Y", "M24", "KORD", "VSK94", };

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
    case 48:      return "WggoodPECKER";
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
std::unordered_map<uint32_t, Vector3> originalPositions; // Correg? el nombre "originalPositons"

namespace Fakelag
{
    static std::atomic<bool> activo{ false };
    static std::atomic<bool> threadRunning{ false };
    static std::atomic<bool> forceOff{ false };

    static std::condition_variable cv;
    static std::mutex cv_m;

    static std::atomic<uint64_t> lastStopMs{ 0 };

    static std::thread worker;

    static HANDLE hFreezeThread = nullptr;

    static uint64_t nowMs() {
        using namespace std::chrono;
        return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
    }

    static void StartFreezeThread() {
        if (hFreezeThread == nullptr) {
            
            if (!hFreezeThread) {
               
                throw std::runtime_error("CreateThread(FreezeThreadKyro) failed");
            }
            SetThreadPriority(hFreezeThread, THREAD_PRIORITY_BELOW_NORMAL);
        }
    }

    
    static void StopFreezeThread() {
        if (hFreezeThread) {
           
            DWORD wait = WaitForSingleObject(hFreezeThread, 200);
            CloseHandle(hFreezeThread);
            hFreezeThread = nullptr;
        }
        else {
          
        }
    }

    
    inline void ForceOff() {
        forceOff.store(true, std::memory_order_relaxed);
        cv.notify_all();
        StopFreezeThread();
    }
}

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

float GetDistance(ImVec2 a, ImVec2 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return sqrtf(dx * dx + dy * dy);
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


struct SilentTarget {
    uint32_t entity = 0;
    Vector3 headPosition;
    float screenDistance = FLT_MAX;
};

SilentTarget FindBestSilentTarget(uint32_t localPlayer, Vector3 cameraPos, Matrix4x4 viewMatrix, uint32_t entities, uint32_t count, int screenWidth, int screenHeight) {
    SilentTarget bestTarget;
    bestTarget.screenDistance = FLT_MAX;

    ImVec2 screenCenter = ImVec2(screenWidth / 2.0f, screenHeight / 2.0f);

    uint32_t maxCheck = (count > 50) ? 50 : count;

    for (uint32_t i = 0; i < maxCheck; ++i) {
        uint32_t entityAddr = 0;
        if (!ReadZ(entities + i * 4, entityAddr) || entityAddr == 0)
            continue;

        if (entityAddr == localPlayer)
            continue;

        // VERIFICACI?N COMPLETA DEL ESTADO DEL ENEMIGO
        bool isVisible = false, isTeam = false, isDead = false, isKnocked = false;
        uint32_t avatarManager, avatar, avatarData;

        // Leer informaci?n de avatar y visibilidad
        if (ReadZ(entityAddr + AvatarManager, avatarManager) &&
            ReadZ(avatarManager + Avatar, avatar) &&
            ReadZ(avatar + Avatar_Data, avatarData)) {
            ReadZ(avatar + Avatar_IsVisible, isVisible);
            ReadZ(avatarData + Avatar_Data_IsTeam, isTeam);
        }

        // Leer si est? Muerto
        ReadZ(entityAddr + Player_IsDead, isDead);

        // Verificar si est? noqueado mediante shadowBase
        uint32_t shadowBase = 0;
        if (ReadZ(entityAddr + Player_ShadowBase, shadowBase) && shadowBase != 0) {
            int pose;
            if (ReadZ(shadowBase + XPose, pose) && pose == 8)
                isKnocked = true;
        }

        // Filtrar enemigos no v?lidos
        if (isKnocked || isDead || !isVisible || isTeam)
            continue;

        uint32_t playerName = 0;
        if (!ReadZ(entityAddr + Player_Name, playerName))
            continue;

        uint32_t headBone = 0;
        if (!ReadZ(entityAddr + Head, headBone) || headBone == 0)
            continue;

        Vector3 headWorld = {};
        if (!GetNodePosition(headBone, headWorld))
            continue;

        float distance3D = Vector3::Distance(cameraPos, headWorld);

        if (distance3D > DistanceAll || distance3D < 1.0f)
            continue;

        ImVec2 head2D = WorldToScreenImVec2(viewMatrix, headWorld, screenWidth, screenHeight);

        if (head2D.x == -1 && head2D.y == -1)
            continue;

        if (silent_combo != 1) {
            if (head2D.x < 0 || head2D.x > screenWidth ||
                head2D.y < 0 || head2D.y > screenHeight)
                continue;
        }

        float distToCrosshair = GetDistance(head2D, screenCenter);

        if (SilentHook) {
            if (silent_combo == 0) {
                // Solo si está MUY cerca del crosshair (requiere apuntado manual preciso)
                if (distToCrosshair <= 25.0f) {
                    if (distToCrosshair < bestTarget.screenDistance) {
                        bestTarget.entity = entityAddr;
                        bestTarget.headPosition = headWorld;
                        bestTarget.screenDistance = distToCrosshair;
                    }
                }
            }
            else if (silent_combo == 1) {
                // Solo si está MUY cerca del crosshair (requiere apuntado manual preciso)
                if (distToCrosshair <= 25.0f) {
                    if (distToCrosshair < bestTarget.screenDistance) {
                        bestTarget.entity = entityAddr;
                        bestTarget.headPosition = headWorld;
                        bestTarget.screenDistance = distToCrosshair;
                    }
                }
            }
            else if (silent_combo == 2) {
                // Solo si está MUY cerca del crosshair (requiere apuntado manual preciso)
                if (distToCrosshair <= 25.0f) {
                    if (distToCrosshair < bestTarget.screenDistance) {
                        bestTarget.entity = entityAddr;
                        bestTarget.headPosition = headWorld;
                        bestTarget.screenDistance = distToCrosshair;
                    }
                }
            }
        }
    }

    return bestTarget;
}

void DoSilentAim(uint32_t localPlayer, const Vector3& targetHead)
{
    // Verificar si está disparando (AimSilent1)
    bool isShooting = false;
    if (!ReadZ<bool>(localPlayer + AimSilent1, isShooting) || !isShooting)
        return;

    // Obtener weapon data (AimSilent2)
    uint32_t weaponInstance = 0;
    if (!ReadZ(localPlayer + AimSilent2, weaponInstance) || weaponInstance == 0)
        return;

    // Leer posición de inicio (AimSilent3)
    Vector3 fireStart;
    if (!ReadZ(weaponInstance + AimSilent3, fireStart))
        return;

    // Anti-detección: Ajuste de posición de cabeza (basado en KrishuSIlentMax.cs)
    Vector3 adjustedHead = targetHead + Vector3(0, 0.1f, 0);
    Vector3 aimDir = adjustedHead - fireStart;

    if (std::isnan(aimDir.x) || std::isnan(aimDir.y) || std::isnan(aimDir.z) ||
        std::isinf(aimDir.x) || std::isinf(aimDir.y) || std::isinf(aimDir.z))
        return;

    float magnitude = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y + aimDir.z * aimDir.z);
    if (magnitude < 0.1f || magnitude > 1000.0f)
        return;

    // Escribir dirección de aim en el offset correcto (AimSilent4)
    WriteZ<Vector3>(weaponInstance + AimSilent4, aimDir);
}

namespace SilentAimLoop {
    static std::atomic<bool> running{ false };
    static std::atomic<bool> hasTarget{ false };
    static Vector3 targetHeadPos = Vector3::Zero();
    static uint32_t cachedLocalPlayer = 0;
    static std::thread workerThread;

    static void StartLoop() {
        if (running.load()) return;

        running.store(true);
        workerThread = std::thread([] {
            while (running.load()) {
                if (!hasTarget.load()) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    continue;
                }

                uint32_t lp = cachedLocalPlayer;
                if (lp == 0) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));
                    continue;
                }

                bool canShoot = false;
                if (!ReadZ(lp + isFiringOffset, canShoot)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                    continue;
                }

                if (!canShoot) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(2));
                    continue;
                }

                Vector3 currentTarget = targetHeadPos;
                DoSilentAim(lp, currentTarget);

                std::this_thread::yield();
            }
            });
    }

    static void StopLoop() {
        running.store(false);
        if (workerThread.joinable()) {
            workerThread.join();
        }
    }

    static void UpdateTarget(uint32_t localPlayer, const Vector3& headPos, bool active) {
        cachedLocalPlayer = localPlayer;
        targetHeadPos = headPos;
        hasTarget.store(active);
    }
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


HWND hwndEMul = zGetHwndBsEx();

// Funci?n auxiliar para obtener la posici?n de un hueso espec?fico
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

    // Sincronizar variables de compatibilidad
    UpdateAimbotCompatibility();

    // Verificar hotkeys para toggle
    static bool aimKeyPressed = false;
    static bool speedKeyPressed = false;

    if (AimHotkey != 0) {
        bool currentlyPressed = (GetAsyncKeyState(AimHotkey) & 0x8000) != 0;
        if (currentlyPressed && !aimKeyPressed) {
            g_AimAssist = !g_AimAssist;
            UpdateAimbotCompatibility();
        }
        aimKeyPressed = currentlyPressed;
    }

    if (SpeedHotkey != 0) {
        bool currentlyPressed = (GetAsyncKeyState(SpeedHotkey) & 0x8000) != 0;
        if (currentlyPressed && !speedKeyPressed) {
            SpeedHackEnabled = !SpeedHackEnabled;
        }
        speedKeyPressed = currentlyPressed;
    }

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

    // Verificar si viewMatrix es válido
    bool viewMatrixValid = (viewMatrix.m33 != 0.0f);
    if (!viewMatrixValid) {
        // Intentar obtener viewMatrix de otra forma si falla
        return;
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


        // LECTURA DE HUESOS
        std::vector<std::pair<uint32_t, Vector3*>> bones = {
            { (uint32_t)Head, &bonePositions.Cabeza },
            { (uint32_t)Spine, &bonePositions.Columna },
            { (uint32_t)Root, &bonePositions.Raiz },
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
                        if (ReadZ(pool + 0x10, Health)) ESPHealthInt = Health;
                    }
                }
            }
        }

        if (ESPShield) {
            uint32_t dataPool;
            if (ReadZ(entity + Player_Data, dataPool)) {
                uint32_t poolObj;
                if (ReadZ(dataPool + 0x8, poolObj)) {
                    uint32_t pool;
                    if (ReadZ(poolObj + 0x10, pool)) {
                        int Shield;
                        if (ReadZ(pool + 0x14, Shield)) ESPShieldInt = Shield;
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

      
        DistanceA = Vector3::Distance(mainPos, bonePositions.Cabeza);
        if (DistanceA > maxDistance) continue;

        auto headScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Cabeza, width, height);
        auto bottomScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Raiz, width, height);

        if (headScreenPos.x < 1 || headScreenPos.y < 1) continue;

        auto CuelloScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Columna, width, height);
        auto CaderaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.Cadera, width, height);
        auto HombroDerechoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.HombroDerecho, width, height);
        auto HombroIzquierdoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.HombroIzquierdo, width, height);
        auto CodoDerechoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.CodoDerecho, width, height);
        auto CodoIzquierdoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.CodoIzquierdo, width, height);
        auto ManoDerechaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.ManoDerecha, width, height);
        auto ManoIzquierdaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.ManoIzquierda, width, height);
        auto PieDerechoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.PieDerecho, width, height);
        auto PieIzquierdoScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.PieIzquierdo, width, height);
        auto PantorrillaDerechaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.PantorrillaDerecha, width, height);
        auto PantorrillaIzquierdaScreenPos = WorldToScreenImVec2(viewMatrix, bonePositions.PantorrillaIzquierda, width, height);

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

        if (EspLineZ && IsTeamVar != Bool3::True)
        {
            static HWND hwndEMulator = nullptr;
            if (!hwndEMulator || !IsWindow(hwndEMulator)) { hwndEMulator = FindRenderWindow(); }
            if (!hwndEMulator) return;
            RECT clientRect;
            POINT screenPos = { 0, 0 };
            GetClientRect(hwndEMulator, &clientRect);
            ClientToScreen(hwndEMulator, &screenPos);
            int eMulatorWidth = clientRect.right;
            int eMulatorHeight = clientRect.bottom;
            int clientX = screenPos.x;
            int clientY = screenPos.y;
            auto* drawList = ImGui::GetBackgroundDrawList();
            drawList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + eMulatorWidth, clientY + eMulatorHeight), true);

            // Calcular startPoint según el estilo de línea seleccionado
            ImVec2 startPoint;
            switch (SelectedLineStyle) {
            case 1: // Top
                startPoint = ImVec2(clientX + eMulatorWidth / 2, clientY);
                break;
            case 2: // Center
                startPoint = ImVec2(clientX + eMulatorWidth / 2, clientY + eMulatorHeight / 2);
                break;
            case 3: // Bottom
                startPoint = ImVec2(clientX + eMulatorWidth / 2, clientY + eMulatorHeight);
                break;
            default:
                startPoint = ImVec2(clientX + eMulatorWidth / 2, clientY);
                break;
            }

            ImVec2 endPoint = ImVec2(x + headScreenPos.x, y + headScreenPos.y - 5);
            if (startPoint.x >= clientX && startPoint.x <= clientX + eMulatorWidth && startPoint.y >= clientY && startPoint.y <= clientY + eMulatorHeight && endPoint.x >= clientX && endPoint.x <= clientX + eMulatorWidth && endPoint.y >= clientY && endPoint.y <= clientY + eMulatorHeight)
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
        if (ESPBones && IsTeamVar != Bool3::True)
        {
            static HWND hwndEMulator = nullptr;
            if (!hwndEMulator || !IsWindow(hwndEMulator)) {
                hwndEMulator = FindRenderWindow();
            }
            if (!hwndEMulator) continue;
            RECT clientRect;
            POINT screenPos = { 0, 0 };
            GetClientRect(hwndEMulator, &clientRect);
            ClientToScreen(hwndEMulator, &screenPos);
            int eMulatorWidth = clientRect.right;
            int eMulatorHeight = clientRect.bottom;
            int clientX = screenPos.x;
            int clientY = screenPos.y;
            auto* drawList = ImGui::GetForegroundDrawList();
            drawList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + eMulatorWidth, clientY + eMulatorHeight), true);
            auto isValidPosition = [&](const ImVec2& pos) {
                return pos.x > 0 && pos.y > 0 && pos.x < eMulatorWidth && pos.y < eMulatorHeight;
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

        if (ESPBox && IsTeamVar != Bool3::True)
        {
            HWND hwnd = FindWindowA(NULL, "MSI App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "BlueStacks App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "HD-Player");

            RECT clientRect, windowRect;
            int eMulatorWidth = 0, eMulatorHeight = 0;
            int clientX = 0, clientY = 0;

            if (hwnd)
            {
                GetClientRect(hwnd, &clientRect);
                GetWindowRect(hwnd, &windowRect);
                eMulatorWidth = clientRect.right - clientRect.left;
                eMulatorHeight = clientRect.bottom - clientRect.top;
                clientX = windowRect.left;
                clientY = windowRect.top;
            }

            auto vList = ImGui::GetForegroundDrawList();
            if (hwnd)
            {
                vList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + eMulatorWidth, clientY + eMulatorHeight), true);
            }
            float WBox = CornerWidth;
            float HBox = CornerHeight * 1.2f;
            float XBox = x + headScreenPos.x - (WBox / 2);
            float YBox = y + headScreenPos.y - (HBox * 0.15f);
            float thickness = 1.5f;
            ImVec4 currentColor = isVisible ? ImVec4(boxColor) : ImVec4(lineColorTeam);
            ImU32 borderColU32 = IM_COL32(currentColor.x * 255, currentColor.y * 255, currentColor.z * 255, 255);
            if (ESPFillBox)
            {
                ImU32 colBottom = IM_COL32((int)(fillBoxColor.x * 255), (int)(fillBoxColor.y * 255), (int)(fillBoxColor.z * 255), 140);
                ImU32 colTop = IM_COL32(255, 255, 255, 60);
                vList->AddRectFilledMultiColor(ImVec2(XBox, YBox), ImVec2(XBox + WBox, YBox + HBox), colTop, colTop, colBottom, colBottom);
            }
            switch (SelectedESPBoxStyle)
            {
            case 0: // None - no dibuja nada
                break;
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
            case 4: // 3D Box
            {
                Box3D box = GetEnemyBoxFromBones(bonePositions.Cabeza, bonePositions.Raiz);
                Draw3DBox(box, viewMatrix, eMulatorWidth, eMulatorHeight, borderColU32, clientX, clientY, vList);
                break;
            }
            default:
                vList->AddRect(ImVec2(XBox, YBox), ImVec2(XBox + WBox, YBox + HBox), borderColU32, 1.0f, 0, thickness);
                break;
            }
            if (hwnd)
            {
                vList->PopClipRect();
            }
        }
        if (EspWeaponsICON && IsTeamVar != Bool3::True)
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

            // Fondo negro para mejor visibilidad del icono
            ImVec2 iconBgMin = ImVec2(iconPos.x - 2, iconPos.y - 2);
            ImVec2 iconBgMax = ImVec2(iconPos.x + 18, iconPos.y + 18);
            auto vListIcon = ImGui::GetForegroundDrawList();
            vListIcon->AddRectFilled(iconBgMin, iconBgMax, IM_COL32(0, 0, 0, 180), 3.0f);

            zRenderWeapon(font::WeaponsIco, iconPos, weaponColorFinal, false, 16, weaponIcon);
        }

        if (EspWeaponsTex && IsTeamVar != Bool3::True) {
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

            // Fondo negro para mejor legibilidad del nombre del arma
            ImVec2 bgMin = ImVec2(textPos.x - 3, textPos.y - 1);
            ImVec2 bgMax = ImVec2(textPos.x + textSize.x + 3, textPos.y + textSize.y + 1);
            vListText->AddRectFilled(bgMin, bgMax, IM_COL32(0, 0, 0, 180), 2.0f);

            vListText->AddText(fontToUse, dynamicFontSize, textPos, weaponTextColor, weaponName);
        }
        if (ESPHealth && IsTeamVar != Bool3::True)
        {
            HWND hwnd = FindWindowA(NULL, "MSI App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "BlueStacks App Player");
            if (!hwnd) return;

            RECT clientRect, windowRect;
            GetClientRect(hwnd, &clientRect);
            GetWindowRect(hwnd, &windowRect);

            int eMulatorWidth = clientRect.right - clientRect.left;
            int eMulatorHeight = clientRect.bottom - clientRect.top;
            int clientX = windowRect.left;
            int clientY = windowRect.top + (windowRect.bottom - windowRect.top - eMulatorHeight);

            ImDrawList* vList = ImGui::GetForegroundDrawList();
            vList->PushClipRect(ImVec2(clientX, clientY),
                ImVec2(clientX + eMulatorWidth, clientY + eMulatorHeight), true);

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

        if (ESPHealthMAX && IsTeamVar != Bool3::True)
        {
            HWND hwnd = FindWindowA(NULL, "MSI App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "BlueStacks App Player");
            if (!hwnd) return;

            RECT clientRect, windowRect;
            GetClientRect(hwnd, &clientRect);
            GetWindowRect(hwnd, &windowRect);

            int eMulatorWidth = clientRect.right - clientRect.left;
            int eMulatorHeight = clientRect.bottom - clientRect.top;
            int clientX = windowRect.left;
            int clientY = windowRect.top + (windowRect.bottom - windowRect.top - eMulatorHeight);

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
                    ImVec2(clientX + eMulatorWidth, clientY + eMulatorHeight), true);

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
        if (ESPCercania && IsTeamVar != Bool3::True)
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
                float distanceToCrosshair = sqrtf(powf(headScreenPos.x - screenCenterX, 2) +
                    powf(headScreenPos.y - screenCenterY, 2));
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
        if (ESPNameZ && IsTeamVar != Bool3::True) {
            HWND hwnd = FindWindowA(NULL, "MSI App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "BlueStacks App Player");
            if (!hwnd) return;
            RECT clientRect, windowRect;
            GetClientRect(hwnd, &clientRect);
            GetWindowRect(hwnd, &windowRect);
            int eMulatorWidth = clientRect.right - clientRect.left;
            int eMulatorHeight = clientRect.bottom - clientRect.top;
            int clientX = windowRect.left;
            int clientY = windowRect.top + (windowRect.bottom - windowRect.top - eMulatorHeight);
            auto vList = ImGui::GetForegroundDrawList();
            vList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + eMulatorWidth, clientY + eMulatorHeight), true);

            // Leer nombre real del jugador desde memoria
            std::string playerName = "Unknown";
            uint32_t nameAddr;
            if (ReadZ(entity + Player_Name, nameAddr) && nameAddr != 0) {
                std::string scannedName = ReadPlayerName(nameAddr);
                if (!scannedName.empty() && scannedName != "Unknown") {
                    playerName = scannedName;
                }
            }

            // Si no se pudo leer el nombre, usar ID del jugador
            if (playerName == "Unknown") {
                playerName = "Player_" + std::to_string(entity % 1000);
            }

            ImGui::PushFont(font::fuentezada);
            ImVec2 nameSize = ImGui::CalcTextSize(playerName.c_str());
            float paddingX = 4.0f;
            float paddingY = 2.0f;
            float rounding = 3.0f;

            // Color según visibilidad
            ImU32 nameColor = isVisible ? IM_COL32(255, 255, 255, 255) : IM_COL32(160, 32, 240, 255);
            ImU32 bgColor = IM_COL32(0, 0, 0, 180);

            float boxWidth = nameSize.x + (paddingX * 2);
            float boxHeight = nameSize.y + (paddingY * 2);
            ImVec2 startPos(x + headScreenPos.x - boxWidth / 2, y + headScreenPos.y - 30.0f);
            ImVec2 rectMin = startPos;
            ImVec2 rectMax = ImVec2(startPos.x + boxWidth, startPos.y + boxHeight);

            // Fondo negro para mejor legibilidad
            vList->AddRectFilled(rectMin, rectMax, bgColor, rounding);
            vList->AddRect(rectMin, rectMax, nameColor, rounding, 0, 1.0f);

            // Texto del nombre centrado
            ImVec2 textPos(rectMin.x + paddingX, rectMin.y + paddingY);
            vList->AddText(font::fuentezada, ImGui::GetFontSize(), textPos, nameColor, playerName.c_str());
            ImGui::PopFont();
            vList->PopClipRect();
        }

        if (ESPDistance && IsTeamVar != Bool3::True) {
            HWND hwnd = FindWindowA(NULL, "MSI App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "BlueStacks App Player");
            if (!hwnd) return;
            RECT clientRect, windowRect;
            GetClientRect(hwnd, &clientRect);
            GetWindowRect(hwnd, &windowRect);
            int eMulatorWidth = clientRect.right - clientRect.left;
            int eMulatorHeight = clientRect.bottom - clientRect.top;
            int clientX = windowRect.left;
            int clientY = windowRect.top + (windowRect.bottom - windowRect.top - eMulatorHeight);
            auto vList = ImGui::GetForegroundDrawList();
            vList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + eMulatorWidth, clientY + eMulatorHeight), true);

            // Calcular distancia real en metros
            float realDistance = DistanceA;
            std::string distText = std::to_string((int)realDistance) + "m";

            // Color según distancia: rojo (cerca), amarillo (medio), verde (lejos)
            ImU32 distColor;
            if (realDistance < 50.0f) {
                distColor = IM_COL32(255, 50, 50, 255); // Rojo - muy cerca
            } else if (realDistance < 100.0f) {
                distColor = IM_COL32(255, 165, 0, 255); // Naranja - cerca
            } else if (realDistance < 200.0f) {
                distColor = IM_COL32(255, 255, 0, 255); // Amarillo - medio
            } else {
                distColor = IM_COL32(0, 255, 0, 255); // Verde - lejos
            }

            ImGui::PushFont(font::fuentezada);
            ImVec2 textSize = ImGui::CalcTextSize(distText.c_str());

            // Posición: debajo de la caja ESP
            ImVec2 textPos(x + headScreenPos.x - textSize.x / 2, y + headScreenPos.y + CornerHeight * 1.2f + 5.0f);

            // Fondo negro para mejor legibilidad
            ImVec2 bgMin = ImVec2(textPos.x - 3, textPos.y - 1);
            ImVec2 bgMax = ImVec2(textPos.x + textSize.x + 3, textPos.y + textSize.y + 1);
            vList->AddRectFilled(bgMin, bgMax, IM_COL32(0, 0, 0, 180), 2.0f);

            vList->AddText(font::fuentezada, ImGui::GetFontSize(), textPos, distColor, distText.c_str());
            ImGui::PopFont();
            vList->PopClipRect();
        }

        if (ESPShield && IsTeamVar != Bool3::True) {
            HWND hwnd = FindWindowA(NULL, "MSI App Player");
            if (!hwnd) hwnd = FindWindowA(NULL, "BlueStacks App Player");
            if (!hwnd) return;
            RECT clientRect, windowRect;
            GetClientRect(hwnd, &clientRect);
            GetWindowRect(hwnd, &windowRect);
            int eMulatorWidth = clientRect.right - clientRect.left;
            int eMulatorHeight = clientRect.bottom - clientRect.top;
            int clientX = windowRect.left;
            int clientY = windowRect.top + (windowRect.bottom - windowRect.top - eMulatorHeight);

            ImDrawList* vList = ImGui::GetForegroundDrawList();
            vList->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + eMulatorWidth, clientY + eMulatorHeight), true);

            float WBox = CornerWidth;
            float HBox = CornerHeight * 1.2f;
            float XBox = PlayerPosHead.x - (WBox / 2.0f);
            float YBox = PlayerPosHead.y - (HBox * 0.15f);
            float shieldPorcentaje = std::clamp(ESPShieldInt / 100.0f, 0.0f, 1.0f);

            ImU32 colorU32;
            if (shieldPorcentaje > 0.66f) colorU32 = IM_COL32(0, 150, 255, 255);
            else if (shieldPorcentaje > 0.33f) colorU32 = IM_COL32(255, 200, 0, 255);
            else colorU32 = IM_COL32(255, 100, 100, 255);

            float barWidth = 2.0f;
            float gap = 4.0f;
            float barX = XBox + WBox + gap;
            float barY = YBox;
            float filledHeight = HBox * shieldPorcentaje;

            vList->AddRectFilled(
                ImVec2(barX, barY),
                ImVec2(barX + barWidth, barY + HBox),
                IM_COL32(0, 0, 0, 150));

            vList->AddRectFilled(
                ImVec2(barX, barY + HBox - filledHeight),
                ImVec2(barX + barWidth, barY + HBox),
                colorU32);

            if (glowMode) {
                ImU32 glowColor1 = (colorU32 & 0x00FFFFFF) | (0x60 << 24);
                ImU32 glowColor2 = (colorU32 & 0x00FFFFFF) | (0x30 << 24);
                ImU32 glowColor3 = (colorU32 & 0x00FFFFFF) | (0x15 << 24);

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




        if (AimbotRotation && !IsTeam)
        {
            static HWND hwndEMulator = nullptr;
            if (!hwndEMulator || !IsWindow(hwndEMulator)) {
                hwndEMulator = FindRenderWindow();
            }

            if (!hwndEMulator) continue;

            RECT clientRect;
            POINT screenPos = { 0, 0 };
            GetClientRect(hwndEMulator, &clientRect);
            ClientToScreen(hwndEMulator, &screenPos);

            int eMulatorWidth = clientRect.right;
            int eMulatorHeight = clientRect.bottom;
            int clientX = screenPos.x;
            int clientY = screenPos.y;

            auto drawList = ImGui::GetForegroundDrawList();
            ImVec2 screenCenter(eMulatorWidth / 2.0f, eMulatorHeight / 2.0f);
            ImVec2 adjustedScreenCenter = ImVec2(clientX + screenCenter.x, clientY + screenCenter.y);

            // VARIABLES PARA ENCONTRAR EL MEJOR OBJETIVO
            float closestDistance = FLT_MAX;
            ImVec2 bestTargetPos = ImVec2(0, 0);
            Vector3 bestBoneWorld;
            bool targetInFov = false;

            // BUSCAR EL OBJETIVO M?S CERCANO DENTRO DEL FOV
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

                // Filtrar enemigos no v?lidos
                if (isKnocked || isDead || !isVisible || isTeam) continue;

                // OBTENER EL BONE SELECCIONADO (con soporte para Smooth Shots)
                uint32_t selectedBone;
                Vector3 bonePos;

                int activeBone = SelectedAimBone;
                // Si Smooth Shots está activo y estamos en los primeros tiros, usar pecho
                if (g_ChestShotsCount > 0 && currentShotIndex <= g_ChestShotsCount) {
                    activeBone = 1; // Columna/Cuello (pecho)
                }

                switch (activeBone)
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

                // CALCULAR DISTANCIA Y FILTRAR POR DISTANCIA M?XIMA
                float distanceToPlayer = Vector3::Distance(mainPos, bonePos);
                if (distanceToPlayer > DistanceRotation) continue;

                // Convertir a posici?n en pantalla
                ImVec2 targetPos = WorldToScreenImVec2(viewMatrix, bonePos, eMulatorWidth, eMulatorHeight);
                bool isOnScreen = targetPos.x > 0 && targetPos.y > 0 && targetPos.x < eMulatorWidth && targetPos.y < eMulatorHeight;

                if (!isOnScreen) continue;

                // Calcular distancia al centro del FOV
                float dx = targetPos.x - screenCenter.x;
                float dy = targetPos.y - screenCenter.y;
                float distanceToCrosshair = sqrtf(dx * dx + dy * dy);

                // Verificar si está dentro del FOV (sistema FOV-based aimbot)
                bool isInsideFov = (distanceToCrosshair <= FovAll);

                // Prioridad: objetivo más cercano al centro de la mira
                if (isInsideFov && distanceToCrosshair < closestDistance) {

                    targetInFov = true;
                    closestDistance = distanceToCrosshair;
                    bestTargetPos = ImVec2(clientX + targetPos.x, clientY + targetPos.y);
                    bestBoneWorld = bonePos;
                }
            }

            

            // DIBUJAR L?NEA DE PREDICCI?N SOLO AL MEJOR OBJETIVO
            if (targetInFov && closestDistance < FLT_MAX)
            {
                drawList->AddLine(adjustedScreenCenter, bestTargetPos, ImColor(lineaverdexdddd), 1.5f);
            }

            // LEER SI ESTÁ DISPARANDO
            bool isFiring = false;
            ReadZ(localPlayer + isFiringOffset, isFiring);

            // DETECTAR DISPARO REAL Y CONTAR TIROS
            static std::chrono::steady_clock::time_point aimingStartTime;
            static bool wasAiming = false;
            static bool isActuallyFiring = false;
            static bool wasFiringLastFrame = false;

            if (isFiring && !wasAiming) {
                aimingStartTime = std::chrono::steady_clock::now();
                wasAiming = true;
                isActuallyFiring = false;
            }
            else if (!isFiring) {
                wasAiming = false;
                isActuallyFiring = false;
                // Resetear contador de tiros cuando se deja de disparar
                if (wasFiringLastFrame && g_ChestShotsCount > 0) {
                    currentShotIndex = 0;
                }
            }
            else if (wasAiming && isFiring) {
                auto now = std::chrono::steady_clock::now();
                auto aimingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(now - aimingStartTime).count();
                if (aimingDuration > 150) {
                    isActuallyFiring = true;
                    // Incrementar contador de tiros cuando se está disparando realmente
                    if (!wasFiringLastFrame && g_ChestShotsCount > 0) {
                        currentShotIndex++;
                    }
                }
            }

            wasFiringLastFrame = isFiring;

            // APLICAR AIMBOT SOLO AL MEJOR OBJETIVO CUANDO ES DISPARO REAL (versión vieja con 4 tipos)
            if (g_AimAssist && isActuallyFiring && targetInFov && closestDistance < FLT_MAX)
            {
                if (g_AimAssistType == 0) { // Memory Lock (Directo)
                    auto aimRotation = AimBZv2::GetRotationToLocation(bestBoneWorld, 0.0f, mainPos);
                    WriteZ(localPlayer + AimRotation, aimRotation);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                else if (g_AimAssistType == 1) { // Memory Smooth (Rotación Suave con Slerp)
                    auto aimRotation = AimBZv2::GetRotationToLocation(bestBoneWorld, 0.0f, mainPos);
                    Quaternion currentRotation = {0, 0, 0, 1};
                    if (ReadZ(localPlayer + AimRotation, currentRotation)) {
                        float t = 1.0f - g_AimSmoothness;
                        // Si smoothing es 0, usar aimbot instantáneo directo
                        if (g_AimSmoothness <= 0.0f) {
                            WriteZ(localPlayer + AimRotation, aimRotation);
                        } else {
                            if (t < 0.01f) t = 0.01f;
                            if (t > 1.0f) t = 1.0f;
                            Quaternion smoothRotation = Quaternion::Slerp(currentRotation, aimRotation, t);
                            WriteZ(localPlayer + AimRotation, smoothRotation);
                        }
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                else if (g_AimAssistType == 2) { // Mouse Emulation (Emulación de Mouse)
                    // Implementación de mouse emulation (requiere código adicional)
                    auto aimRotation = AimBZv2::GetRotationToLocation(bestBoneWorld, 0.0f, mainPos);
                    WriteZ(localPlayer + AimRotation, aimRotation);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
                else if (g_AimAssistType == 3) { // Silent Aim (Disparo Silencioso)
                    static Quaternion originalRotation = {0, 0, 0, 1};
                    static bool rotationSaved = false;

                    // Guardar rotación original al inicio del disparo
                    if (!rotationSaved) {
                        if (ReadZ(localPlayer + AimRotation, originalRotation)) {
                            rotationSaved = true;
                        }
                    }

                    // Aplicar rotación al objetivo durante el disparo
                    auto aimRotation = AimBZv2::GetRotationToLocation(bestBoneWorld, 0.0f, mainPos);
                    WriteZ(localPlayer + AimRotation, aimRotation);
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));

                    // Restaurar rotación original cuando se deja de disparar
                    if (!isFiring && rotationSaved) {
                        WriteZ(localPlayer + AimRotation, originalRotation);
                        rotationSaved = false;
                    }
                }
            }
        }
        if (ScopeOffset && !IsTeam)
        {
            static HWND hwndEMulator = nullptr;
            if (!hwndEMulator || !IsWindow(hwndEMulator)) { hwndEMulator = FindRenderWindow(); }
            if (!hwndEMulator) continue; RECT clientRect; POINT screenPos = { 0, 0 };
            GetClientRect(hwndEMulator, &clientRect); ClientToScreen(hwndEMulator, &screenPos);
            int eMulatorWidth = clientRect.right; int eMulatorHeight = clientRect.bottom;
            int clientX = screenPos.x; int clientY = screenPos.y;
            auto drawList = ImGui::GetForegroundDrawList();
            ImVec2 screenCenter(eMulatorWidth / 2.0f, eMulatorHeight / 2.0f);
            ImVec2 adjustedScreenCenter = ImVec2(clientX + screenCenter.x, clientY + screenCenter.y);
            float closestDistance = FLT_MAX; ImVec2 bestTargetPos = ImVec2(0, 0);
            Vector3 bestBoneWorld; bool targetInFov = false;
            for (int i = 0; i < entitiesCount; ++i) {
                uint32_t entity;
                if (!ReadZ((uintptr_t)(i * 0x4 + entities), entity) || entity == 0 || entity == localPlayer) continue;
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
                    int pose; if (ReadZ(shadowBase + XPose, pose) && pose == 8)isKnocked = true;
                }
                if (isKnocked || isDead || !isVisible || isTeam) continue;
                uint32_t selectedBone; Vector3 bonePos;
                if (!ReadZ(entity + Hip, selectedBone)) continue;
                if (!GetNodePosition(selectedBone, bonePos)) continue;
                float distanceToPlayer = Vector3::Distance(mainPos, bonePos);
                if (distanceToPlayer > DistanceRotation) continue;
                ImVec2 targetPos = WorldToScreenImVec2(viewMatrix, bonePos, eMulatorWidth, eMulatorHeight);
                bool isOnScreen = targetPos.x > 0 && targetPos.y > 0 && targetPos.x < eMulatorWidth && targetPos.y < eMulatorHeight;
                if (!isOnScreen) continue; float dx = targetPos.x - screenCenter.x; float dy = targetPos.y - screenCenter.y;
                float distanceToCrosshair = sqrtf(dx * dx + dy * dy);
                bool isInsideFov = (distanceToCrosshair <= FovAll);
                if (isInsideFov) {
                    targetInFov = true;
                    if (distanceToCrosshair < closestDistance) {
                        closestDistance = distanceToCrosshair;
                        bestTargetPos = ImVec2(clientX + targetPos.x, clientY + targetPos.y);
                        bestBoneWorld = bonePos;
                    }
                }
            }
            if (ShowFov)
            {
                RECT rect;
                GetWindowRect(hwndEMul, &rect);
                int x = rect.left;
                int y = rect.top;
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;
                ImVec2 fovCenter = ImVec2(x + width / 2, y + height / 2);
                ImDrawList* draw = ImGui::GetBackgroundDrawList();
                draw->PushClipRect(ImVec2(x, y), ImVec2(x + width, y + height), true);
                if (rgbMode) { DrawAnimatedRainbowFOVWithGlow(fovCenter, FovAll, 1.f, 0.f, 100, 0.0f); }
                else { ImU32 color = ImGui::GetColorU32(ColorFov); draw->AddCircle(fovCenter, FovAll, color, 100, 1.f); }
                const char* cheat_name2 = " "; const char* titleText = cheat_name2;
                ImVec2 textSize = ImGui::CalcTextSize(titleText);
                ImVec2 textPos = ImVec2(fovCenter.x - textSize.x / 2.0f, fovCenter.y - FovAll - textSize.y - 8.0f);
                ImU32 outlineColor = IM_COL32(0, 0, 0, 255); ImU32 textColor = IM_COL32(255, 255, 255, 255);
                for (int ox = -1; ox <= 1; ++ox) for (int oy = -1; oy <= 1; ++oy) if (ox != 0 || oy != 0) draw->AddText(ImVec2(textPos.x + ox, textPos.y + oy), outlineColor, titleText);
                draw->AddText(textPos, textColor, titleText); draw->PopClipRect();
            }
            if (targetInFov && closestDistance < FLT_MAX)
            {
                drawList->AddLine(adjustedScreenCenter, bestTargetPos, ImColor(lineaverdexdddd), 1.5f);
            }
            bool isFiring = false;
            ReadZ(localPlayer + isFiringOffset, isFiring);
            static std::chrono::steady_clock::time_point aimingStartTime;
            static bool wasAiming = false; static bool isActuallyFiring = false;
            if (isFiring && !wasAiming) { aimingStartTime = std::chrono::steady_clock::now(); wasAiming = true; isActuallyFiring = false; }
            else if (!isFiring) { wasAiming = false; isActuallyFiring = false; }
            else if (wasAiming && isFiring) {
                auto now = std::chrono::steady_clock::now(); auto aimingDuration = std::chrono::duration_cast<std::chrono::milliseconds>(now - aimingStartTime).count();
                if (aimingDuration > 150) { isActuallyFiring = true; }
            }
            if (isActuallyFiring && targetInFov && closestDistance < FLT_MAX)
            {
                // Aplicar el aimbot directo al bone seleccionado del objetivo (versión vieja funcional)
                auto aimRotation = AimBZv2::GetRotationToLocation(bestBoneWorld, 0.0f, mainPos);
                WriteZ(localPlayer + AimRotation, aimRotation);
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
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

            if (AimPrediction == true && !IsTeam)
            {
                static HWND hwndEMulator = nullptr;
                if (!hwndEMulator || !IsWindow(hwndEMulator))
                    hwndEMulator = FindRenderWindow();

                if (!hwndEMulator) continue;

                RECT clientRect;
                POINT screenPos = { 0, 0 };
                GetClientRect(hwndEMulator, &clientRect);
                ClientToScreen(hwndEMulator, &screenPos);

                int eMulatorWidth = clientRect.right;
                int eMulatorHeight = clientRect.bottom;
                int clientX = screenPos.x;
                int clientY = screenPos.y;

                float closestScreenDistance = FLT_MAX;
                float closestPlayerDistance = FLT_MAX;
                ImVec2 closestHeadScreenPos = ImVec2(0, 0);
                Vector3 localPlayerHeadV3 = bonePositions.Cabeza;

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

                    if (isKnocked) continue;
                    if (isDead) continue;

                    uint32_t headBone;
                    if (!ReadZ(entity + Head, headBone)) continue;

                    Vector3 headV3;
                    if (!GetNodePosition(headBone, headV3)) continue;

                    Vector3 predictedHead = headV3;
                    if (auto it = enemiesSunk.find(entity); it != enemiesSunk.end())
                    {
                        const Vector3& velocity = it->second;
                        predictedHead = headV3 + velocity * 0.25f;
                    }

                    float distanceToPlayer = Vector3::Distance(localPlayerHeadV3, headV3);
                    ImVec2 headScreenPos = WorldToScreenImVec2(viewMatrix, predictedHead, width, height);
                    if (headScreenPos.x <= 0 || headScreenPos.y <= 0)
                        continue;

                    float screenCenterX = width / 2.0f;
                    float screenCenterY = height / 2.0f;
                    float distanceToCrosshair = sqrtf(pow(headScreenPos.x - screenCenterX, 2) + pow(headScreenPos.y - screenCenterY, 2));

                    if (distanceToCrosshair > RadioMagnet)
                        continue;

                    if (distanceToCrosshair < closestScreenDistance ||
                        (distanceToCrosshair == closestScreenDistance && distanceToPlayer < closestPlayerDistance))
                    {
                        closestScreenDistance = distanceToCrosshair;
                        closestPlayerDistance = distanceToPlayer;
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

                    RightLeftPlayerPositionAuto = -deltaX * scaleFactor;
                    UpPlayerAltureAuto = -deltaY * scaleFactor;
                }
            }
        }
        
        if (TeleportMarkk && !IsTeam)
        {
            uint32_t localRootBonePtr;
            if (!ReadZ(localPlayer + Root, localRootBonePtr))
                continue;
            uint32_t baseGameFacade;
            if (!ReadZ(Il2Cpp + InitBase, baseGameFacade) || baseGameFacade == 0)
                continue;
            uint32_t staticGameFacade;
            if (!ReadZ(baseGameFacade + StaticClass, staticGameFacade) || staticGameFacade == 0)
                continue;
            uint32_t _1;
            ReadZ(staticGameFacade, _1);
            uint32_t _2;
            ReadZ(_1 + 0x8, _2);
            uint32_t _3;
            ReadZ(_2 + 0x1E4, _3);
            uint32_t _4;
            ReadZ(_3 + 0x50, _4);
            uint32_t _5;
            ReadZ(_4 + 0x88, _5);
            Vector3 markPos;
            if (!ReadZ(_5 + 0x54, markPos))
                continue;
            if (markPos.x == 0 && markPos.y == 0 && markPos.z == 0)
                continue;
            uint32_t localTransformValue;
            if (!ReadZ(localRootBonePtr + 0x8, localTransformValue))
                continue;
            uint32_t localTransformObjPtr;
            if (!ReadZ(localTransformValue + 0x8, localTransformObjPtr))
                continue;
            uint32_t localMatrixValue;
            if (!ReadZ(localTransformObjPtr + 0x20, localMatrixValue))
                continue;
            Vector3 currentPos;
            if (!GetNodePosition(localRootBonePtr, currentPos))
                continue;
            if (IgnoreAlture)
            {
                markPos.y = currentPos.y;
            }
            WriteZ<Vector3>(localMatrixValue + 0x80, markPos);
        }

        if (TpBaseCheck && !IsTeam)
        {
            float closestDistance = FLT_MAX;
            uint32_t bestEnemy = 0;
            Vector3 bestEnemyPos;
            uint32_t localRootBonePtr;
            if (ReadZ(localPlayer + Root, localRootBonePtr))
            {
                Vector3 localRootPos;
                if (GetNodePosition(localRootBonePtr, localRootPos))
                {
                    uint32_t entity = 0;
                    if (ReadZ((uintptr_t)(i * 0x4 + entities), entity))
                    {
                        bool isDead = false;
                        if (ReadZ(entity + Player_IsDead, isDead) && !isDead)
                        {
                            uint32_t targetBoneOffset;
                            if (TpSelection == 0) targetBoneOffset = Head;
                            else if (TpSelection == 1) targetBoneOffset = Spine;
                            else if (TpSelection == 2) targetBoneOffset = Root;
                            uint32_t enemyTargetBonePtr;
                            if (ReadZ(entity + targetBoneOffset, enemyTargetBonePtr))
                            {
                                Vector3 enemyPos;
                                if (GetNodePosition(enemyTargetBonePtr, enemyPos))
                                {
                                    float playerDistance = Vector3::Distance(localRootPos, enemyPos);

                                    if (playerDistance <= TpDistance && playerDistance < closestDistance)
                                    {
                                        closestDistance = playerDistance;
                                        bestEnemy = entity;
                                        bestEnemyPos = enemyPos;
                                    }
                                }
                            }
                        }
                    }
                    if (bestEnemy != 0)
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
                                    WriteZ<Vector3>(localMatrixValue + 0x80, bestEnemyPos);
                                }
                            }
                        }
                    }
                }
            }
        }

        if (TeleportBase && !IsTeam) {
            float closestDistance = FLT_MAX;
            uint32_t bestEnemy = 0;
            Vector3 bestEnemyPos;
            uint32_t localRootBonePtr;
            detectedPlayers.clear();
            if (ReadZ(localPlayer + Root, localRootBonePtr)) {
                Vector3 localRootPos;
                if (GetNodePosition(localRootBonePtr, localRootPos)) {
                    for (int i = 0; i < 100; i++) {
                        uint32_t entity = 0;
                        if (!ReadZ((uintptr_t)(i * 0x4 + entities), entity) || entity == 0) continue;
                        if (entity == localPlayer) continue;
                        bool isDead = false;
                        if (!ReadZ(entity + Player_IsDead, isDead) || isDead) continue;
                        uint32_t enemyRootBonePtr;
                        if (ReadZ(entity + Root, enemyRootBonePtr)) {
                            Vector3 enemyPos;
                            if (GetNodePosition(enemyRootBonePtr, enemyPos)) {
                                float playerDistance = Vector3::Distance(localRootPos, enemyPos);
                                std::string currentName = "Unknown";
                                uint32_t nameAddr;
                                if (ReadZ(entity + Player_Name, nameAddr)) {
                                    currentName = ReadPlayerName(nameAddr);
                                    if (currentName.empty()) currentName = "Unknown";
                                }
                                if (detectedPlayers.size() < 10) {
                                    detectedPlayers.push_back({ entity, currentName, playerDistance });
                                }
                                if (selectedEntity != 0) {
                                    if (entity == selectedEntity) {
                                        bestEnemy = entity;
                                        bestEnemyPos = enemyPos;
                                        selectedDist = playerDistance;
                                    }
                                }
                                else {
                                    if (playerDistance < closestDistance && playerDistance <= TpDistance) {
                                        closestDistance = playerDistance;
                                        bestEnemy = entity;
                                        bestEnemyPos = enemyPos;
                                    }
                                }
                            }
                        }
                    }
                    if (bestEnemy != 0) {
                        uint32_t localTransformValue;
                        if (ReadZ(localRootBonePtr + 0x8, localTransformValue)) {
                            uint32_t localTransformObjPtr;
                            if (ReadZ(localTransformValue + 0x8, localTransformObjPtr)) {
                                uint32_t localMatrixValue;
                                if (ReadZ(localTransformObjPtr + 0x20, localMatrixValue)) {
                                    WriteZ<Vector3>(localMatrixValue + 0x80, bestEnemyPos);
                                }
                            }
                        }
                    }
                }
            }
        }

        if (bTeleportEnabled && SelectedTeleport != -1 && localPlayer != 0)
        {
            Vector3 teleportPos;
            switch (SelectedTeleport)
            {
            case 0:  teleportPos = { 663.7f, 75.2f, 76.3f }; break;     // Peak
            case 1:  teleportPos = { 217.8f, 11.9f, -102.2f }; break;   // Clock Tower
            case 2:  teleportPos = { 893.9f, 54.7f, 501.8f }; break;    // Mill
            case 3:  teleportPos = { -16.1f, -0.3f, 64.2f }; break;     // Hangar
            case 4:  teleportPos = { 1181.9f, 5.1f, -404.2f }; break;   // Sentosa
            case 5:  teleportPos = { -147.9f, 32.2f, 398.6f }; break;   // Observatory
            case 6:  teleportPos = { 652.4f, 24.9f, -272.3f }; break;   // Pochinok
            case 7:  teleportPos = { 417.1f, 10.8f, 506.6f }; break;    // Plantation
            case 8:  teleportPos = { 433.3f, 15.8f, -214.0f }; break;   // Factory
            case 9:  teleportPos = { 729.8f, 17.1f, 543.3f }; break;    // Riverside
            case 10: teleportPos = { 65.0f, 9.8f, 656.5f }; break;      // Bullseye
            case 11: teleportPos = { -18.584f, 8.401f, 521.036f }; break;// Graveyard
            case 12: teleportPos = { 434.738f, 18.997f, 198.089f }; break;// Bimasakti Strip
            case 13: teleportPos = { 174.9f, 1.9f, 370.1f }; break;     // Katulistiwa
            case 14: teleportPos = { 949.6f, 19.9f, -42.2f }; break;    // Kota Tua
            case 15: teleportPos = { 516.8f, 24.7f, -535.1f }; break;   // Mars Electric
            case 16: teleportPos = { 1297.6f, -0.0f, 84.8f }; break;    // Cape Town
            case 17: teleportPos = { 293.496f, 9.229f, 738.235f }; break;// Shipyard
            case 18: teleportPos = { 1119.7f, 17.7f, 292.0f }; break;   // Keraton
            case 19: teleportPos = { -150.547f, -13.588f, -289.385f }; break; // Rim Nam
            default: bTeleportEnabled = false; break; // Seguridad
            }

            if (bTeleportEnabled)
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
                                WriteZ<Vector3>(localMatrixValue + 0x80, teleportPos);
                                bTeleportEnabled = false;
                            }
                        }
                    }
                }
            }
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
                                if (ReadZ<Vector3>(enemyMatrixValue + 0x80, currentPos))
                                {
                                    Vector3 upPos = currentPos + Vector3(0, UpX, 0);
                                    float targetHeight = mainPos.y + UpX;
                                    float deltaY = currentPos.y - mainPos.y;

                                    if (deltaY < (UpX * 0.2f))
                                    {
                                        Vector3 newPos = currentPos;
                                        newPos.y = mainPos.y + UpX;
                                        WriteZ<Vector3>(enemyMatrixValue + 0x80, newPos);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (enemyTeleport && !IsTeam)
        {
            float closestDistance = FLT_MAX;
            uint32_t bestEnemy = 4;
            Vector3 localRootPos;
            uint32_t localRootBonePtr;
            if (ReadZ(localPlayer + Root, localRootBonePtr) && GetNodePosition(localRootBonePtr, localRootPos))
            {
                uint32_t localHeadBonePtr;
                Vector3 localHeadPos;

                if (ReadZ(localPlayer + Head, localHeadBonePtr) && GetNodePosition(localHeadBonePtr, localHeadPos))
                {
                    Vector3 forwardDir = Vector3::Normalized(Vector3(localHeadPos.x, 0, localHeadPos.z) - Vector3(localRootPos.x, 0, localRootPos.z));
                    Vector3 rightDir = Vector3::Normalized(Vector3(forwardDir.z, 0, -forwardDir.x));
                    Vector3 offsetPos = localRootPos + forwardDir * enemydistancechupa + rightDir * enemysidechupa;
                    uint32_t entity = 0;
                    if (!ReadZ((uintptr_t)(i * 0x4 + entities), entity)) continue;
                    bool isDead = false;
                    if (!ReadZ(entity + Player_IsDead, isDead) || isDead) continue;
                    uint32_t enemyRootBonePtr;
                    if (ReadZ(entity + Root, enemyRootBonePtr))
                    {
                        Vector3 enemyPos;
                        if (GetNodePosition(enemyRootBonePtr, enemyPos))
                        {
                            float playerDistance = Vector3::Distance(localRootPos, enemyPos);
                            if (playerDistance > enemyplayerdistance) continue;

                            if (playerDistance < closestDistance)
                            {
                                closestDistance = playerDistance;
                                bestEnemy = entity;
                            }
                        }
                    }
                    static Vector3 lastOffsetPos = Vector3::Zero();
                    if (bestEnemy != 0)
                    {
                        uint32_t enemyRootBonePtr = 0;
                        if (ReadZ(bestEnemy + Root, enemyRootBonePtr) && enemyRootBonePtr != 0)
                        {
                            uint32_t enemyTransformValue = 0;
                            if (ReadZ(enemyRootBonePtr + 0x8, enemyTransformValue) && enemyTransformValue != 0)
                            {
                                uint32_t enemyTransformObjPtr = 0;
                                if (ReadZ(enemyTransformValue + 0x8, enemyTransformObjPtr) && enemyTransformObjPtr != 0)
                                {
                                    uint32_t enemyMatrixValue = 0;
                                    if (ReadZ(enemyTransformObjPtr + 0x20, enemyMatrixValue) && enemyMatrixValue != 0)
                                    {
                                        Vector3 currentPos;
                                        if (ReadZ<Vector3>(enemyMatrixValue + 0x80, currentPos))
                                        {
                                            if (Vector3::Distance(currentPos, offsetPos) > 0.01f)
                                            {
                                                WriteZ<Vector3>(enemyMatrixValue + 0x80, offsetPos);
                                                lastOffsetPos = offsetPos;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (SilentHook == true)
        {
            if (ShowFov)
            {
                RECT rect;
                GetWindowRect(hwndEMul, &rect);
                int x = rect.left;
                int y = rect.top;
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;
                ImVec2 fovCenter = ImVec2(x + width / 2, y + height / 2);
                ImDrawList* draw = ImGui::GetBackgroundDrawList();
                draw->PushClipRect(ImVec2(x, y), ImVec2(x + width, y + height), true);
                if (rgbMode) {
                    DrawAnimatedRainbowFOVWithGlow(fovCenter, FovAll, 1.f, 0.f, 100, 0.0f);
                }
                else {
                    ImU32 color = ImGui::GetColorU32(ColorFov);
                    draw->AddCircle(fovCenter, FovAll, color, 100, 1.f);
                }

                const char* titleText = cheat_name;
                ImVec2 textSize = ImGui::CalcTextSize(titleText);
                ImVec2 textPos = ImVec2(fovCenter.x - textSize.x / 2.0f, fovCenter.y - FovAll - textSize.y - 8.0f);
                ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
                ImU32 textColor = IM_COL32(255, 255, 255, 255);
                for (int ox = -1; ox <= 1; ++ox) for (int oy = -1; oy <= 1; ++oy) if (ox != 0 || oy != 0) draw->AddText(ImVec2(textPos.x + ox, textPos.y + oy), outlineColor, titleText);
                draw->AddText(textPos, textColor, titleText);
                draw->PopClipRect();

            }

            SilentAimLoop::StartLoop();

            static int frameSkip = 0;
            if (frameSkip % 3 == 0) {
                SilentTarget best = FindBestSilentTarget(localPlayer, mainPos, viewMatrix, entities, entitiesCount, g_windowWidth, g_windowHeight);
                if (best.entity != 0)
                {
                    SilentAimLoop::UpdateTarget(localPlayer, best.headPosition, true);
                }
                else
                {
                    SilentAimLoop::UpdateTarget(localPlayer, Vector3::Zero(), false);
                }
            }
            frameSkip++;
        }
        else
        {
            SilentAimLoop::StopLoop();
        }

        static int lastMatchStatus1 = -1;
        if (SilentHookG) {
            if (matchStatus != lastMatchStatus1) {
                lastMatchStatus1 = matchStatus;

                if (matchStatus == 1) {
                    SilentHook = true;
                }
                else {
                    SilentHook = false;
                }
            }
        }
        else {
            SilentHook = false;
            lastMatchStatus1 = -1;
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

                       
                        for (int i = 0; i < 50; i++)
                        {
                            WriteZ<Vector3>(matrixPtr + 0x80, newPos);
                            std::this_thread::sleep_for(std::chrono::milliseconds(1));
                        }

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
                                std::this_thread::sleep_for(std::chrono::milliseconds(1));
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
                            rot.x = 0;              // No inclinaci?n lateral
                            rot.y = sinf(rad / 2);  // Giro sobre el eje Y
                            rot.z = 0;              // No inclinaci?n frontal
                            rot.w = cosf(rad / 2);  // Componente escalar

                           
                            WriteZ<Vector4>(localMatrixValue + 0x70, rot);
                        }
                    }
                }
            }
        }

        // Speed Hack con Anti-Detección (basado en SpeedTimer.cs)
        static int speedApplyCounter = 0;
        static std::mt19937 speedRandom(std::chrono::steady_clock::now().time_since_epoch().count());
        const int ApplyEveryNFrames = 3;
        const float SpeedVariation = 0.005f;
        const float NormalSpeed = 0.033000f;

        if (SpeedHackEnabled) {
            speedApplyCounter++;
            if (speedApplyCounter >= ApplyEveryNFrames) {
                // Obtener GameTimer desde currentGame usando el offset correcto
                uint32_t gameTimer = 0;
                if (ReadZ(currentGame + GameTimer, gameTimer) && gameTimer != 0) {
                    // Calcular valor de velocidad con multiplicador (1.7 = x5)
                    float currentSpeedValue = NormalSpeed * SpeedHackMultiplier * (5.0f / 1.7f);

                    // Anti-detección: Agregar variación aleatoria
                    float variation = ((float)speedRandom() / speedRandom.max() - 0.5f) * SpeedVariation;
                    float safeSpeedValue = currentSpeedValue + variation;

                    // Escribir valor de velocidad en FixedDeltaTime del timer
                    WriteZ<float>(gameTimer + FixedDeltaTime, safeSpeedValue);
                }
                speedApplyCounter = 0;
            }
        }

        // --- Vision Hack ---
        if (VisionHack)
        {
            uint32_t followCamera = 0;
            if (ReadZ(localPlayer + FollowCamera, followCamera) && followCamera != 0)
            {
                WriteZ<float>(followCamera + 0x44, VisionSlider);
            }
        }

        if (InfiniteBullents) {
            uint32_t weapon = 0;
            if (ReadZ(localPlayer + Weapon, weapon) && weapon != 0)
            {
                WriteZ(weapon + AmmoOffs, InfinitySliders);
            }
        }

        bool activar = true;
        if (noreloadpro) {
            uint32_t reloadPtr = 0;
            if (ReadZ(localPlayer + LocalPlayerAttributes, reloadPtr) && reloadPtr != 0) {
                WriteZ<bool>(reloadPtr + NoReloadOffset, activar);
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

        // Infinite Ammo - Nueva función (DESACTIVADO - offsets no inicializados)
        /*if (InfiniteAmmo) {
            uint32_t weapon = 0;
            if (ReadZ(localPlayer + Weapon, weapon) && weapon != 0) {
                WriteZ<int>(weapon + AmmoOffs, 9999);
            }
        }*/

        // Fast Fire - Nueva función (DESACTIVADO - offsets no inicializados)
        /*if (FastFire) {
            uint32_t weapon = 0;
            if (ReadZ(localPlayer + Weapon, weapon) && weapon != 0) {
                WriteZ<float>(weapon + FasFireOffs, 0.01f);
            }
        }*/

        // Quick Switch - Nueva función (DESACTIVADO - offsets no inicializados)
        /*if (QuickSwitchHack) {
            uint32_t weapon = 0;
            if (ReadZ(localPlayer + Weapon, weapon) && weapon != 0) {
                WriteZ<bool>(weapon + OffsQuickSwitch, true);
            }
        }*/

        // Medikit Hack - Nueva función (DESACTIVADO - offsets no inicializados)
        /*if (MedikitHackNew) {
            uint32_t medikit = 0;
            if (ReadZ(localPlayer + MedikitOffset, medikit) && medikit != 0) {
                WriteZ<float>(medikit + 0x60, 0.01f);
            }
        }*/


        if (NoRecoil) {
            uint32_t weapon = 0;
            if (ReadZ(localPlayer + Weapon, weapon) && weapon != 0) {
                uint32_t weaponData = 0;
                if (ReadZ(weapon + WeaponData, weaponData) && weaponData != 0) {
                    WriteZ<float>(weaponData + WeaponRecoil, 0.0f);
                }
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

                                    for (int i = 0; i < 50; i++)
                                    {
                                        WriteZ<Vector3>(matrixPtr + 0x80, newPos);
                                        std::this_thread::sleep_for(std::chrono::milliseconds(flywallTickDelay2));
                                    }
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
                                            std::this_thread::sleep_for(std::chrono::milliseconds(flywallTickDelay2));
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
                    float crosshairDist = sqrtf(powf(bone2D.x - screenCenter.x, 2) + powf(bone2D.y - screenCenter.y, 2));
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
    }

    // Draw FOV circle outside entity loop - always active
    if (ShowFov)
    {
        static HWND hwndEMul = nullptr;
        if (!hwndEMul || !IsWindow(hwndEMul)) {
            hwndEMul = FindRenderWindow();
        }
        if (hwndEMul)
        {
            RECT rect;
            GetWindowRect(hwndEMul, &rect);
            int x = rect.left;
            int y = rect.top;
            int width = rect.right - rect.left;
            int height = rect.bottom - rect.top;
            ImVec2 fovCenter = ImVec2(x + width / 2, y + height / 2);
            ImDrawList* draw = ImGui::GetBackgroundDrawList();
            draw->PushClipRect(ImVec2(x, y), ImVec2(x + width, y + height), true);
            if (rgbMode) {
                DrawAnimatedRainbowFOVWithGlow(fovCenter, FovAll, 1.f, 0.f, 100, 0.0f);
            }
            else {
                ImU32 color = ImGui::GetColorU32(ColorFov);
                draw->AddCircle(fovCenter, FovAll, color, 100, 1.f);
            }

            const char* titleText = cheat_name;
            ImVec2 textSize = ImGui::CalcTextSize(titleText);
            ImVec2 textPos = ImVec2(fovCenter.x - textSize.x / 2.0f, fovCenter.y - FovAll - textSize.y - 8.0f);
            ImU32 outlineColor = IM_COL32(0, 0, 0, 255);
            ImU32 textColor = IM_COL32(255, 255, 255, 255);
            for (int ox = -1; ox <= 1; ++ox) for (int oy = -1; oy <= 1; ++oy) if (ox != 0 || oy != 0) draw->AddText(ImVec2(textPos.x + ox, textPos.y + oy), outlineColor, titleText);
            draw->AddText(textPos, textColor, titleText);
            draw->PopClipRect();
        }
    }
}





















