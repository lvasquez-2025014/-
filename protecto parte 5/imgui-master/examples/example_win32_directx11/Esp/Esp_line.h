#pragma once
#include "vector.h"
#include "adb.h"
#include "../../../imgui_internal.h"
#include <chrono>
#include <unordered_map>
#include <string>

// Referencia externa a variables declaradas en main.h
extern bool aimsilent;
extern bool Main_Window;
extern bool EnabledEsp;
extern bool NoRecoil;
extern bool FlyWall;
extern bool caidarapida;
extern bool noreloadpro;

inline bool IsMouseCursorVisible() {
    CURSORINFO ci = { sizeof(CURSORINFO) };
    if (GetCursorInfo(&ci)) {
        return (ci.flags & CURSOR_SHOWING) != 0;
    }
    return false;
}

namespace font2 {
    inline ImFont* WeaponsIco = nullptr;
}

// ======================== VARIABLES GLOBALES (configurables desde UI) ========================

// ESP general
inline bool g_EspEnabled;
inline bool g_EspBox;
inline int g_EspBoxStyle;        // 0-11
inline bool g_EspName;
inline bool g_EspHealth;
inline bool g_EspShield;
inline bool g_EspDistance;
inline bool g_EspID;
inline bool g_EspBones;
inline bool g_EspWeaponIcon;
inline bool g_EspWeaponText;
inline bool g_EspKnockedIcon;
inline bool g_EspLine;
inline int g_EspLineStyle;       // 0-4
inline bool g_EspLineRainbow;
inline bool g_EspRainbowGlobal;
inline bool g_EspLineGlow;
inline float g_EspLineThickness;
inline float g_EspMaxDistance;
inline bool g_EspNearestOnly;
inline bool g_EspShowTeam;
inline bool g_EspShowInvisible;
inline bool g_EspWukongColor;
inline bool g_EspFov;
inline int g_EspFovRadius;
inline bool g_ShowFps;
inline bool g_ShowKeybinds;

// Variables de Asistencia de Apuntado (Aim Assist)
inline bool g_AimAssist;
inline bool g_AimTeammates;
inline int g_AimAssistType; // 0: Memory Lock, 1: Memory Smooth, 2: Mouse Emulation, 3: Silent Aim
inline float g_AimFovRadius;
inline bool g_AimShowFov;
inline int g_AimTargetBone; // 0: Cabeza, 1: Cuello, 2: Torso, 3: Cadera
inline float g_AimSmoothness; // 0.0f - 1.0f (0 = instantáneo, 1 = súper suave/lento)
inline int g_AimKey; // Código de tecla (ej. VK_RBUTTON, VK_LBUTTON)
inline int g_ChestShotsCount; // 0: Desactivado, 1-5: Tiros iniciales al pecho, luego cabeza

// Colores
inline ImVec4 g_BoxColor;
inline ImVec4 g_NameColor;
inline ImVec4 g_HealthColor;
inline ImVec4 g_ShieldColor;
inline ImVec4 g_BonesColor;
inline ImVec4 g_WeaponColor;
inline ImVec4 g_FovColor;
inline ImVec4 g_InvisibleColor;

// ======================== IMPLEMENTACIÓN ========================

// Valores por defecto se inicializan directamente en las variables inline declaradas arriba
struct DefaultInitializer {
    DefaultInitializer() {
        g_EspEnabled = false;
        g_EspBox = false;
        g_EspBoxStyle = 0;
        g_EspName = false;
        g_EspHealth = false;
        g_EspShield = false;
        g_EspDistance = false;
        g_EspID = false;
        g_EspBones = false;
        g_EspWeaponIcon = false;
        g_EspWeaponText = false;
        g_EspKnockedIcon = false;
        g_EspLine = false;
        g_EspLineStyle = 0;
        g_EspLineRainbow = false;
        g_EspRainbowGlobal = false;
        g_EspLineGlow = false;
        g_EspLineThickness = 1.0f;
        g_EspMaxDistance = 0.0f;
        g_EspNearestOnly = false;
        g_EspShowTeam = false;
        g_EspShowInvisible = true;
        g_EspWukongColor = false;
        g_EspFov = false;
        g_EspFovRadius = 150;
        g_ShowFps = true;
        g_ShowKeybinds = false;

        g_AimAssist = false;
        g_AimTeammates = false;
        g_AimAssistType = 0;
        g_AimFovRadius = 150.0f;
        g_AimShowFov = false;
        g_AimTargetBone = 0;
        g_AimSmoothness = 0.4f;
        g_AimKey = VK_RBUTTON;
        g_ChestShotsCount = 0;

        g_BoxColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        g_NameColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        g_HealthColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
        g_ShieldColor = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
        g_BonesColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        g_WeaponColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        g_FovColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        g_InvisibleColor = ImVec4(0.6f, 0.2f, 0.9f, 1.0f);
    }
};
inline DefaultInitializer g_defaultInitializer;

// Nombres de estilos
const char* g_BoxStyleNames[] = {
    "None", "Normal", "Filled", "Corners", "3D", "Glow", "Fade",
    "Corners Filled", "Rainbow", "Rainbow Glow", "Cyber Double", "Neon Gradient"
};
const char* g_LineStyleNames[] = {
    "Top Center", "Screen Center", "Player Position", "Crosshair", "Nearest Only"
};



// Funciones auxiliares

Vector2 ComputeAimAngle(const Vector3& source, const Vector3& target) {
    Vector3 delta;
    delta.x = target.x - source.x;
    delta.y = target.y - source.y;
    delta.z = target.z - source.z;
    float dist = sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
    if (dist < 0.001f) return {0, 0};

    // Yaw: horizontal. atan2 en C++ devuelve rango [-pi, pi], convertido a grados es [-180, 180]
    float yaw = atan2(delta.x, delta.z) * (180.0f / 3.1415926535f);

    // Pitch: vertical. Limitamos delta.y / dist para evitar NAN en asin
    float ratio = delta.y / dist;
    if (ratio > 1.0f) ratio = 1.0f;
    if (ratio < -1.0f) ratio = -1.0f;
    float pitch = -asin(ratio) * (180.0f / 3.1415926535f);
    
    // Normalizar Yaw a rango [-180, 180]
    if (yaw > 180.0f) yaw -= 360.0f;
    if (yaw < -180.0f) yaw += 360.0f;

    // Retornamos {pitch, yaw} para que coincida con el orden de memoria de Unity/Free Fire (X=Pitch, Y=Yaw)
    return {pitch, yaw};
}

Vector3 GetLocalHeadPosition(uintptr_t localPlayer) {
    Vector3 headPos = {0,0,0};
    uintptr_t headBone = 0;
    if (is64Bit) {
        uint64_t temp = 0;
        if (ReadZ(localPlayer + Head, temp) && temp) {
            headBone = (uintptr_t)temp;
        }
    } else {
        uint32_t temp = 0;
        if (ReadZ(localPlayer + Head, temp) && temp) {
            headBone = (uintptr_t)temp;
        }
    }
    if (headBone) GetNodePosition(headBone, headPos);
    return headPos;
}

static HWND GetEmulatorWindow()
{
    static HWND cached = nullptr;
    if (cached && IsWindow(cached)) return cached;
    cached = FindWindowA(NULL, "MSI App Player");
    if (!cached) cached = FindWindowA(NULL, "BlueStacks App Player");
    if (!cached) cached = FindWindowA(NULL, "HD-Player");
    return cached;
}

static HWND GetGameRenderWindow()
{
    HWND parent = GetEmulatorWindow();
    if (!parent) return nullptr;

    // Search child windows for the actual rendering window of the emulator
    RenderWindow = nullptr;
    EnumChildWindows(parent, EnumChildWindowsProc, 0);
    if (RenderWindow && IsWindow(RenderWindow))
        return RenderWindow;

    return parent; // Fallback
}

static ImU32 GetRainbowColor(float offset = 0.0f)
{
    float time = ImGui::GetTime() + offset;
    float hue = fmod(time * 0.3f, 1.0f);
    return ImColor::HSV(hue, 1.0f, 1.0f);
}

static void DrawLineWithGlow(ImDrawList* draw, const ImVec2& p1, const ImVec2& p2, ImU32 color, float thickness, bool glow)
{
    if (glow)
        for (int i = 3; i >= 1; --i)
            draw->AddLine(p1, p2, (color & 0x00FFFFFF) | ((int)(((color >> 24) & 0xFF) * 0.2f / i) << 24), thickness + i);
    draw->AddLine(p1, p2, color, thickness);
}

std::string ReadPlayerName(uintptr_t nameAddr) {
    if (!nameAddr) return "";
    
    // En Unity / Il2Cpp las cadenas de caracteres (System.String) tienen el puntero a caracteres Unicode UTF-16
    // en nameAddr + 0x14, y su longitud en nameAddr + 0x10.
    int length = 0;
    if (ReadZ(nameAddr + 0x10, length) && length > 0 && length < 64) {
        return ReadStringZ2(nameAddr + 0x14, length * 2, true);
    }
    
    // Fallback por si acaso lee el buffer directo
    return ReadStringZ2(nameAddr, 32, true);
}

uint32_t zGetInfoW(uintptr_t player, bool zWOnScan = true)
{
    uintptr_t zWdata = 0, zWdata1 = 0, zWdata2 = 0;
    uint32_t zWID = 0;
    if (zWOnScan)
    {
        // Try Free Fire MAX new offsets first
        bool read1 = false;
        if (is64Bit) {
            uint64_t temp = 0;
            if (ReadZ(player + Weapon, temp) && temp) {
                zWdata = (uintptr_t)temp;
                read1 = true;
            }
        } else {
            uint32_t temp = 0;
            if (ReadZ(player + Weapon, temp) && temp) {
                zWdata = (uintptr_t)temp;
                read1 = true;
            }
        }
        
        if (read1) {
            bool read2 = false;
            if (is64Bit) {
                uint64_t temp = 0;
                if (ReadZ(zWdata + WeaponInfo, temp) && temp) {
                    zWdata1 = (uintptr_t)temp;
                    read2 = true;
                }
            } else {
                uint32_t temp = 0;
                if (ReadZ(zWdata + WeaponInfo, temp) && temp) {
                    zWdata1 = (uintptr_t)temp;
                    read2 = true;
                }
            }
            
            if (read2) {
                if (ReadZ(zWdata1 + WeaponID, zWID)) return zWID;
            }
        }
        
        // Fallback for older versions or Normal FF
        bool fb1 = false;
        if (is64Bit) {
            uint64_t temp = 0;
            if (ReadZ(player + 0x44, temp) && temp) {
                zWdata = (uintptr_t)temp;
                fb1 = true;
            }
        } else {
            uint32_t temp = 0;
            if (ReadZ(player + 0x44, temp) && temp) {
                zWdata = (uintptr_t)temp;
                fb1 = true;
            }
        }
        
        if (!fb1) return 0;
        
        bool fb2 = false;
        if (is64Bit) {
            uint64_t temp = 0;
            if (ReadZ(zWdata + 0x8, temp) && temp) {
                zWdata1 = (uintptr_t)temp;
                fb2 = true;
            }
        } else {
            uint32_t temp = 0;
            if (ReadZ(zWdata + 0x8, temp) && temp) {
                zWdata1 = (uintptr_t)temp;
                fb2 = true;
            }
        }
        
        if (!fb2) return 0;
        
        bool fb3 = false;
        if (is64Bit) {
            uint64_t temp = 0;
            if (ReadZ(zWdata1 + 0x20, temp) && temp) {
                zWdata2 = (uintptr_t)temp;
                fb3 = true;
            }
        } else {
            uint32_t temp = 0;
            if (ReadZ(zWdata1 + 0x20, temp) && temp) {
                zWdata2 = (uintptr_t)temp;
                fb3 = true;
            }
        }
        
        if (!fb3) return 0;
        if (ReadZ(zWdata2 + 0x10, zWID)) return zWID;
    }
    return 0;
}

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
    default:      return "Arma";
    }
}

const char* zGetWIcon(uint32_t ID)
{
    switch (ID)
    {
    case 0:   return "W";
    case 1:   return "J";
    case 2:   return "W";
    case 3:   return "G";
    case 4:   return "A";
    case 5:   return "B";
    case 6:   return "W";
    case 7:   return "U";
    case 8:   return "I";
    case 9:   return "H";
    case 10:  return "a";
    case 11:  return "l";
    case 12:  return "R";
    case 13:  return "T";
    case 14:  return "p";
    case 15:  return "Q";
    case 16:  return "V";
    case 17:  return "V";
    case 18:  return "C";
    case 19:  return "D";
    case 20:  return "L";
    case 21:  return "M";
    case 24:  return "N";
    case 25:  return "O";
    case 26:  return "P";
    case 27:  return "V";
    case 28:  return "S";
    case 29:  return "K";
    case 30:  return "D";
    case 32:  return "P";
    case 33:  return "A";
    case 34:  return "K";
    case 35:  return "C";
    case 39:  return "P";
    case 41:  return "M";
    case 43:  return "<";
    case 45:  return "#";
    case 46:  return "e";
    case 47:  return "I";
    case 48:  return "C";
    case 49:  return "N";
    case 50:  return "Q";
    case 51:  return "V";
    case 54:  return ";";
    case 55:  return "c";
    case 56:  return "+";
    case 57:  return ",";
    case 58:  return "O";
    case 60: return "f";
    case 62: return "5";
    case 65: return "L";
    case 67:  return "E";
    case 70:  return "P";
    case 72:  return "[";
    case 71:  return "M";
    case 73:  return "D";
    case 75:  return "8";
    case 78:  return "(";
    default:  return "n";
    }
}

struct TargetInfo {
    uint32_t entity;
    ImVec2 headScreen;
    ImVec2 footScreen;
    Vector3 headPos;
    Vector3 rootPos;
    std::string name;
    int health;
    int shield;
    int id;
    float distance3D;
    float distanceToCrosshair;
    bool isTeam;
    bool isVisible;
    bool isKnocked;
    bool isWukongOrion;
    uint32_t weaponId;
    std::vector<Vector3> bones;
    bool isDead;
};

// Obtener la matriz de vista y el jugador local (se reutiliza cada frame)
static bool GetGameData(uint32_t& localPlayer, Matrix4x4& viewMatrix, Vector3& localPos, uint32_t& entities, int& entitiesCount, int& emuWidth, int& emuHeight, int& clientX, int& clientY)
{
    HWND hwnd = GetGameRenderWindow();
    if (!hwnd) return false;
    RECT rc;
    POINT pt = { 0, 0 };
    GetClientRect(hwnd, &rc);
    ClientToScreen(hwnd, &pt);
    emuWidth = rc.right;
    emuHeight = rc.bottom;
    clientX = pt.x;
    clientY = pt.y;

    uint32_t gameFacade = 0;
    if (!ReadZ(Il2Cpp + InitBase, gameFacade) || !gameFacade) return false;
    uint32_t staticGameFacade = 0;
    if (!ReadZ(gameFacade + StaticClass, staticGameFacade) || !staticGameFacade) return false;
    uint32_t currentGame = 0;
    if (!ReadZ(staticGameFacade, currentGame) || !currentGame) return false;
    uint32_t currentMatch = 0;
    if (!ReadZ(currentGame + CurrentMatch, currentMatch) || !currentMatch) return false;
    if (!ReadZ(currentMatch + LocalPlayer, localPlayer) || !localPlayer) return false;

    // Detectar si estamos en modo espectador (observando a alguien más)
    uint32_t activeLocalPlayer = localPlayer;
    uint32_t currentObserverPtr = 0;
    if (ReadZ(localPlayer + CurrentObserver, currentObserverPtr) && currentObserverPtr) {
        uint32_t observedPlayer = 0;
        if (ReadZ(currentObserverPtr + ObserverPlayer, observedPlayer) && observedPlayer) {
            activeLocalPlayer = observedPlayer;
        }
    }

    uint32_t followCam = 0, cam = 0, camBase = 0;
    if (!ReadZ(activeLocalPlayer + FollowCamera, followCam) || !followCam) {
        // Si falla con el jugador observado, intentamos de todos modos con el localPlayer original
        if (activeLocalPlayer != localPlayer) {
            activeLocalPlayer = localPlayer;
            if (!ReadZ(activeLocalPlayer + FollowCamera, followCam) || !followCam) return false;
        } else {
            return false;
        }
    }
    if (!ReadZ(followCam + Camera, cam) || !cam) return false;
    if (!ReadZ(cam + 0x8, camBase) || !camBase) return false;
    if (!ReadZ(camBase + ViewMatrix, viewMatrix)) return false;

    uint32_t localRoot = 0;
    if (ReadZ(activeLocalPlayer + Root, localRoot) && localRoot)
        GetNodePosition(localRoot, localPos);
    else localPos = Vector3(0, 0, 0);

    // Asignar al parámetro de salida para que las distancias y filtros usen el jugador activo
    localPlayer = activeLocalPlayer;

    uint32_t entityDict = 0;
    if (!ReadZ(currentGame + DictionaryEntities, entityDict) || !entityDict) return false;
    ReadZ(entityDict + 0x10, entitiesCount);
    if (entitiesCount <= 0) return false;
    ReadZ(entityDict + 0x0C, entities);
    entities += 0x10;
    return true;
}

// Leer datos de un enemigo
static TargetInfo ReadEnemyData(uint32_t entity, const Vector3& localPos, const Matrix4x4& viewMatrix, int emuWidth, int emuHeight)
{
    TargetInfo info = {};
    info.entity = entity;

    // Estado (Hacemos las lecturas opcionales para no ignorar a los bots de entrenamiento que no tienen Avatar)
    info.isVisible = true;
    info.isTeam = false;
    info.isDead = false;

    uint32_t avatarMgr = 0, avatar = 0, avatarData = 0;
    if (ReadZ(entity + AvatarManager, avatarMgr) && avatarMgr) {
        if (ReadZ(avatarMgr + Avatar, avatar) && avatar) {
            ReadZ(avatar + Avatar_IsVisible, info.isVisible);
            if (ReadZ(avatar + Avatar_Data, avatarData) && avatarData) {
                ReadZ(avatarData + Avatar_Data_IsTeam, info.isTeam);
            }
        }
    }
    
    uint8_t isDeadFlag = 0;
    if (ReadZ(entity + Player_IsDead, isDeadFlag)) {
        info.isDead = (isDeadFlag != 0);
    }

    // Noqueado
    uint32_t shadowBase = 0;
    if (ReadZ(entity + Player_ShadowBase, shadowBase) && shadowBase) {
        int xpose = 0;
        if (ReadZ(shadowBase + XPose, xpose) && xpose == 8)
            info.isKnocked = true;
    }

    // Wukong/Orion
    int wukong = 0;
    ReadZ(entity + WukongOrion, wukong);
    info.isWukongOrion = (wukong == 1 || wukong == 2);

    // Nombre
    uint32_t nameAddr = 0;
    if (ReadZ(entity + Player_Name, nameAddr) && nameAddr)
        info.name = ReadPlayerName(nameAddr);
    if (info.name.empty()) info.name = "Player";

    // Salud y escudo
    uint32_t dataPool = 0, poolObj = 0, pool = 0;
    int rawHealth = 0;
    if (ReadZ(entity + Player_Data, dataPool) && dataPool &&
        ReadZ(dataPool + 0x8, poolObj) && poolObj &&
        ReadZ(poolObj + 0x10, pool) && pool) {
        ReadZ(pool + 0xC, rawHealth);
        ReadZ(pool + HealdShieldEP, info.shield);
    }
    
    // FILTRO DE ENTIDADES FANTASMA MEJORADO: 
    // Evita ocultar jugadores reales si el puntero de salud falla
    if (rawHealth < 0 || rawHealth > 1000) {
        info.health = 200; // Asumir 200 hp si la memoria es corrupta para evitar parpadeos
    }
    else if (rawHealth == 0 && info.name == "Player") {
        info.isDead = true; // Si es 0 y sin nombre, es un vehículo/objeto seguro
    }

    info.health = std::clamp(rawHealth, 0, 250);
    info.shield = std::clamp(info.shield, 0, 250);

    // ID (si se quiere mostrar un número único basado en la dirección)
    info.id = (entity & 0xFFFF) % 9999;

    // Posiciones
    uint32_t headBone = 0, rootBone = 0;
    if (!ReadZ(entity + Head, headBone) || !headBone) return info;
    if (!ReadZ(entity + Root, rootBone) || !rootBone) return info;
    if (!GetNodePosition(headBone, info.headPos)) return info;
    if (!GetNodePosition(rootBone, info.rootPos)) return info;
    info.distance3D = Vector3::Distance(localPos, info.headPos);
    if (g_EspMaxDistance > 0 && info.distance3D > g_EspMaxDistance) return info;

    // Proyección a pantalla
    info.headScreen = WorldToScreenImVec2(viewMatrix, info.headPos, emuWidth, emuHeight);
    info.footScreen = WorldToScreenImVec2(viewMatrix, info.rootPos, emuWidth, emuHeight);
    if (info.headScreen.x < 1 || info.headScreen.y < 1) return info;

    float dx = info.headScreen.x - emuWidth / 2.0f;
    float dy = info.headScreen.y - emuHeight / 2.0f;
    info.distanceToCrosshair = sqrtf(dx * dx + dy * dy);

    // Arma
    uint32_t weapon = 0;
    if (ReadZ(entity + Weapon, weapon) && weapon)
        info.weaponId = zGetInfoW(entity);

    // Huesos
    if (g_EspBones || g_AimAssist) {
        uint32_t bones[] = { Head, Neck, Spine, Hip, LeftSholder, LeftElbow, LeftHand, RightSholder, RightElbow, RightHand, LeftCalf, LeftFoot, RightCalf, RightFoot };
        for (uint32_t off : bones) {
            uint32_t bone = 0;
            Vector3 pos;
            if (ReadZ(entity + off, bone) && bone && GetNodePosition(bone, pos))
                info.bones.push_back(pos);
            else
                info.bones.push_back(Vector3(0, 0, 0));
        }
    }

    return info;
}

// Dibujar línea ESP
static void DrawLineESP(const TargetInfo& target, const ImVec2& startPoint, ImDrawList* draw, int clientX, int clientY, int emuWidth, int emuHeight)
{
    ImVec2 end = ImVec2(clientX + target.headScreen.x, clientY + target.headScreen.y - 5);
    if (end.x < clientX || end.x > clientX + emuWidth || end.y < clientY || end.y > clientY + emuHeight) return;
    ImU32 color;
    if (g_EspLineRainbow) color = GetRainbowColor(target.distanceToCrosshair / emuWidth);
    else color = ImGui::GetColorU32(g_BoxColor);
    DrawLineWithGlow(draw, startPoint, end, color, g_EspLineThickness, g_EspLineGlow);
}

// Estructura de una caja 3D
struct Box3D {
    Vector3 center;
    float height;
    float width;
    float depth;
};

// Obtener la caja 3D de un enemigo (centrada en pies, tamaño amplio)
Box3D GetEnemyBoxFromBones(const std::vector<Vector3>& bones, const Vector3& rootPos) {
    Box3D box;
    box.center = rootPos;
    box.width = 1.0f; // Ancho fijo amplio (sobresale bastante)
    box.depth = 1.0f; // Fondo fijo amplio

    if (bones.size() > 0 && bones[0].y != 0 && rootPos.y != 0) {
        float headY = bones[0].y;
        if (headY > rootPos.y) {
            box.height = (headY - rootPos.y) + 0.3f; // Hasta la cabeza + margen arriba
        } else {
            box.height = 1.9f;
        }
    } else {
        box.height = 1.9f;
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
    bool anyValid = false;
    for (int i = 0; i < 8; i++) {
        ImVec2 screen = WorldToScreenImVec2(viewMatrix, vertices[i], screenWidth, screenHeight);
        if (screen.x > 0 && screen.x < screenWidth && screen.y > 0 && screen.y < screenHeight) {
            anyValid = true;
        }
        screenVerts[i] = ImVec2(clientX + screen.x, clientY + screen.y);
    }
    if (!anyValid) return;

    draw->AddLine(screenVerts[0], screenVerts[1], color, 1.5f);
    draw->AddLine(screenVerts[1], screenVerts[2], color, 1.5f);
    draw->AddLine(screenVerts[2], screenVerts[3], color, 1.5f);
    draw->AddLine(screenVerts[3], screenVerts[0], color, 1.5f);
    
    draw->AddLine(screenVerts[4], screenVerts[5], color, 1.5f);
    draw->AddLine(screenVerts[5], screenVerts[6], color, 1.5f);
    draw->AddLine(screenVerts[6], screenVerts[7], color, 1.5f);
    draw->AddLine(screenVerts[7], screenVerts[4], color, 1.5f);
    
    draw->AddLine(screenVerts[0], screenVerts[4], color, 1.5f);
    draw->AddLine(screenVerts[1], screenVerts[5], color, 1.5f);
    draw->AddLine(screenVerts[2], screenVerts[6], color, 1.5f);
    draw->AddLine(screenVerts[3], screenVerts[7], color, 1.5f);
}

// Dibujar caja ESP
static void DrawBoxESP(const TargetInfo& target, ImDrawList* draw, int clientX, int clientY, float cornerHeight, float cornerWidth, const Matrix4x4& viewMatrix, int emuWidth, int emuHeight)
{
    float X = clientX + target.headScreen.x - cornerWidth / 2;
    float Y = clientY + target.headScreen.y - cornerHeight * 0.15f;
    float W = cornerWidth;
    float H = cornerHeight * 1.2f;

    // Color de la caja (respetando Rainbow global o color estático)
    ImU32 color = target.isVisible ? ImGui::GetColorU32(g_BoxColor) : ImGui::GetColorU32(g_InvisibleColor);
    if (g_EspRainbowGlobal) {
        color = GetRainbowColor(target.distanceToCrosshair / emuWidth);
    }
    if (g_EspWukongColor && target.isWukongOrion) color = IM_COL32(255, 100, 0, 255);
    if (target.isKnocked) color = IM_COL32(255, 80, 80, 255);

    switch (g_EspBoxStyle) {
    case 0: break; // None
    case 1: // Normal
        draw->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), color, 2.0f, 0, 1.0f);
        break;
    case 2: // Filled
        draw->AddRectFilled(ImVec2(X, Y), ImVec2(X + W, Y + H), (color & 0x00FFFFFF) | (0x45 << 24), 2.0f);
        draw->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), color, 2.0f, 0, 1.0f);
        break;
    case 3: // Corners
    {
        float l = W / 4, t = H / 4;
        ImU32 wCol = color;
        draw->AddLine(ImVec2(X, Y), ImVec2(X + l, Y), wCol, 2.5f);
        draw->AddLine(ImVec2(X, Y), ImVec2(X, Y + t), wCol, 2.5f);
        draw->AddLine(ImVec2(X + W - l, Y), ImVec2(X + W, Y), wCol, 2.5f);
        draw->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + t), wCol, 2.5f);
        draw->AddLine(ImVec2(X, Y + H - t), ImVec2(X, Y + H), wCol, 2.5f);
        draw->AddLine(ImVec2(X, Y + H), ImVec2(X + l, Y + H), wCol, 2.5f);
        draw->AddLine(ImVec2(X + W - l, Y + H), ImVec2(X + W, Y + H), wCol, 2.5f);
        draw->AddLine(ImVec2(X + W, Y + H - t), ImVec2(X + W, Y + H), wCol, 2.5f);
    }
    break;
    case 4: // 3D
    {
        Box3D box = GetEnemyBoxFromBones(target.bones, target.rootPos);
        Draw3DBox(box, viewMatrix, emuWidth, emuHeight, color, clientX, clientY, draw);
    }
    break;
    case 5: // Glow
        for (int i = 1; i <= 5; i++) {
            float off = i * 2;
            int a = (int)(100 * (1.0f - i / 6.0f));
            draw->AddRect(ImVec2(X - off, Y - off), ImVec2(X + W + off, Y + H + off), (color & 0x00FFFFFF) | (a << 24), 2.0f, 0, 1.0f);
        }
        draw->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), color, 2.0f);
        break;
    case 6: // Fade
        draw->AddRectFilledMultiColor(ImVec2(X, Y), ImVec2(X + W, Y + H), color, color, (color & 0x00FFFFFF), (color & 0x00FFFFFF));
        draw->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), color, 2.0f);
        break;
    case 7: // Corners Filled
        draw->AddRectFilled(ImVec2(X, Y), ImVec2(X + W, Y + H), (color & 0x00FFFFFF) | (0x35 << 24), 2.0f);
        {
            float l = W / 4, t = H / 4;
            draw->AddLine(ImVec2(X, Y), ImVec2(X + l, Y), color, 1.5f);
            draw->AddLine(ImVec2(X, Y), ImVec2(X, Y + t), color, 1.5f);
            draw->AddLine(ImVec2(X + W - l, Y), ImVec2(X + W, Y), color, 1.5f);
            draw->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + t), color, 1.5f);
            draw->AddLine(ImVec2(X, Y + H - t), ImVec2(X, Y + H), color, 1.5f);
            draw->AddLine(ImVec2(X, Y + H), ImVec2(X + l, Y + H), color, 1.5f);
            draw->AddLine(ImVec2(X + W - l, Y + H), ImVec2(X + W, Y + H), color, 1.5f);
            draw->AddLine(ImVec2(X + W, Y + H - t), ImVec2(X + W, Y + H), color, 1.5f);
        }
        break;
    case 8: // Rainbow
    {
        float t = ImGui::GetTime();
        ImU32 rc = IM_COL32((int)((sin(t) + 1) * 127), (int)((sin(t + 2) + 1) * 127), (int)((sin(t + 4) + 1) * 127), 255);
        draw->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), rc, 2.0f);
    }
    break;
    case 9: // Rainbow Glow
    {
        float t = ImGui::GetTime();
        ImU32 rc = IM_COL32((int)((sin(t) + 1) * 127), (int)((sin(t + 2) + 1) * 127), (int)((sin(t + 4) + 1) * 127), 255);
        for (int i = 1; i <= 5; i++) {
            float off = i * 2;
            draw->AddRect(ImVec2(X - off, Y - off), ImVec2(X + W + off, Y + H + off), (rc & 0x00FFFFFF) | ((int)(70 * (1 - i / 6.0f)) << 24), 2.0f);
        }
        draw->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), rc, 2.0f);
    }
    break;
    case 10: // Cyber Double
        draw->AddRect(ImVec2(X - 2, Y - 2), ImVec2(X + W + 2, Y + H + 2), color, 1.0f);
        draw->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), (color & 0x00FFFFFF) | 0x70 << 24, 1.0f);
        break;
    case 11: // Neon Gradient
        draw->AddRectFilledMultiColor(ImVec2(X, Y), ImVec2(X + W, Y + H), IM_COL32(255, 0, 220, 180), IM_COL32(255, 0, 220, 180), IM_COL32(0, 255, 240, 180), IM_COL32(0, 255, 240, 180));
        draw->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), IM_COL32(255, 255, 255, 100), 1.0f);
        break;
    }
}

// Dibujar nombre y distancia con estilo premium (stacked text con corona vectorizada por encima de la cabeza y caja)
static void DrawTextESP(const TargetInfo& target, ImDrawList* draw, int clientX, int clientY, float cornerHeight, float cornerWidth)
{
    if (!g_EspName && !g_EspDistance) return;

    float X = clientX + target.headScreen.x;
    // Borde superior de la caja ESP (cornerHeight * 0.15f arriba del centro de la cabeza)
    float boxTopY = clientY + target.headScreen.y - cornerHeight * 0.15f;

    std::string nameText = g_EspName ? target.name : "";
    std::string distText = g_EspDistance ? std::to_string((int)target.distance3D) + "M" : "";

    ImVec2 nameSize = ImGui::CalcTextSize(nameText.c_str());
    ImVec2 distSize = ImGui::CalcTextSize(distText.c_str());

    // Calculamos el espacio vertical necesario para la corona y textos de forma que queden apilados arriba
    float currentY = boxTopY - 4.0f; // Pequeño margen sobre el borde superior

    // Sombra del texto
    ImU32 textCol = target.isVisible ? ImGui::GetColorU32(g_NameColor) : ImGui::GetColorU32(g_InvisibleColor);
    if (g_EspRainbowGlobal) {
        textCol = GetRainbowColor(target.distanceToCrosshair / 500.0f);
    }
    ImU32 shadowCol = IM_COL32(0, 0, 0, 220);

    // 1. Apilar de abajo hacia arriba (Distancia, luego Nombre, luego Corona)
    ImVec2 distPos(0, 0);
    if (!distText.empty()) {
        currentY -= distSize.y;
        distPos = ImVec2(X - distSize.x / 2.0f, currentY);
    }

    ImVec2 namePos(0, 0);
    if (!nameText.empty()) {
        currentY -= nameSize.y + 1.0f;
        namePos = ImVec2(X - nameSize.x / 2.0f, currentY);
    }

    // 2. Dibujar corona arriba de todo el stack
    float centerY = currentY - 10.0f;
    ImU32 goldCol = IM_COL32(255, 215, 0, 255);
    ImU32 goldLight = IM_COL32(255, 245, 150, 255);
    ImU32 purpleCol = IM_COL32(180, 0, 255, 255);

    // Corona vectorizada premium
    ImVec2 p0(X - 8, centerY + 3);  // Bottom-left
    ImVec2 p1(X - 8, centerY - 3);  // Left peak
    ImVec2 p2(X - 4, centerY + 1);  // Left valley
    ImVec2 p3(X, centerY - 7);      // Tall center peak
    ImVec2 p4(X + 4, centerY + 1);  // Right valley
    ImVec2 p5(X + 8, centerY - 3);  // Right peak
    ImVec2 p6(X + 8, centerY + 3);  // Bottom-right

    // Rellenar cuerpo de la corona
    draw->PathClear();
    draw->PathLineTo(p0); draw->PathLineTo(p1); draw->PathLineTo(p2);
    draw->PathLineTo(p3); draw->PathLineTo(p4); draw->PathLineTo(p5);
    draw->PathLineTo(p6);
    draw->PathFillConvex(goldCol);

    // Dibujar borde sutil
    draw->PathClear();
    draw->PathLineTo(p0); draw->PathLineTo(p1); draw->PathLineTo(p2);
    draw->PathLineTo(p3); draw->PathLineTo(p4); draw->PathLineTo(p5);
    draw->PathLineTo(p6);
    draw->PathStroke(IM_COL32(180, 140, 0, 255), true, 1.0f);

    // Joyas en los picos de la corona
    draw->AddCircleFilled(p1, 1.2f, goldLight);
    draw->AddCircleFilled(p3, 1.6f, goldLight);
    draw->AddCircleFilled(p5, 1.2f, goldLight);

    // Gema púrpura en el centro
    draw->AddCircleFilled(ImVec2(X, centerY + 1), 2.0f, purpleCol);

    // 3. Renderizar los textos ya calculados en sus posiciones
    if (!nameText.empty()) {
        draw->AddText(ImVec2(namePos.x + 1, namePos.y + 1), shadowCol, nameText.c_str());
        draw->AddText(namePos, textCol, nameText.c_str());
    }
    if (!distText.empty()) {
        draw->AddText(ImVec2(distPos.x + 1, distPos.y + 1), shadowCol, distText.c_str());
        draw->AddText(distPos, textCol, distText.c_str());
    }

    // Dibujar ID opcional
    if (g_EspID) {
        char buf[16]; sprintf(buf, "#%d", target.id);
        draw->AddText(ImVec2(X + cornerWidth / 2.0f + 6.0f, boxTopY), IM_COL32(255, 255, 255, 180), buf);
    }
}

// Dibujar salud (Barra estilizada premium de vida en el lado derecho de la caja)
static void DrawHealthESP(const TargetInfo& target, ImDrawList* draw, int clientX, int clientY, float cornerHeight, float cornerWidth, float X, float Y, float H)
{
    if (!g_EspHealth && !g_EspShield) return;

    // Posición en el lado derecho de la caja ESP
    float barWidth = 5.0f;
    float barX = X + cornerWidth + 6.0f;
    float barY = Y;
    float barHeight = H;

    float healthPercent = target.health / 200.0f;
    if (healthPercent > 1.0f) healthPercent = 1.0f;
    if (healthPercent < 0.0f) healthPercent = 0.0f;

    // Verde sólido brillante
    ImU32 healthCol = IM_COL32(0, 255, 120, 255);

    // 1. Fondo negro de la barra de vida
    draw->AddRectFilled(ImVec2(barX - 1.0f, barY - 1.0f), ImVec2(barX + barWidth + 1.0f, barY + barHeight + 1.0f), IM_COL32(0, 0, 0, 180), 1.0f);

    // 2. Barra de vida llena verde sólido
    float currentBarHeight = barHeight * healthPercent;
    if (currentBarHeight > 0.0f) {
        draw->AddRectFilled(ImVec2(barX, barY + barHeight - currentBarHeight), ImVec2(barX + barWidth, barY + barHeight), healthCol, 1.0f);
    }
}

// Dibujar arma (Nombre en blanco abajo e icono de silueta debajo del nombre)
static void DrawWeaponESP(const TargetInfo& target, ImDrawList* draw, int clientX, int clientY, float cornerHeight, float cornerWidth, float X, float Y, float H)
{
    if (!g_EspWeaponIcon && !g_EspWeaponText) return;

    const char* name = zGetWName(target.weaponId);
    const char* icon = zGetWIcon(target.weaponId);
    ImU32 col = target.isVisible ? ImGui::GetColorU32(g_WeaponColor) : ImGui::GetColorU32(g_InvisibleColor);
    if (g_EspRainbowGlobal) {
        col = GetRainbowColor(target.distanceToCrosshair / 500.0f);
    }
    ImU32 shadowCol = IM_COL32(0, 0, 0, 200);

    float centerX = X + cornerWidth / 2.0f;
    float currentY = Y + H + 6.0f;

    // 1. Nombre del arma
    if (g_EspWeaponText && name && strlen(name) > 0) {
        ImVec2 sz = ImGui::CalcTextSize(name);
        ImVec2 textPos(centerX - sz.x / 2.0f, currentY);
        // Sombra de texto
        draw->AddText(ImVec2(textPos.x + 1, textPos.y + 1), shadowCol, name);
        draw->AddText(textPos, col, name);
        currentY += sz.y + 2.0f;
    }

    // 2. Icono de silueta del arma (Directamente abajo de la palabra)
    if (g_EspWeaponIcon && icon && strlen(icon) > 0 && font2::WeaponsIco) {
        ImGui::PushFont(font2::WeaponsIco);
        ImVec2 sz = ImGui::CalcTextSize(icon);
        ImVec2 iconPos(centerX - sz.x / 2.0f, currentY);
        
        // Efecto de brillo blanco/sombra
        draw->AddText(font2::WeaponsIco, 20.0f, ImVec2(iconPos.x + 1, iconPos.y + 1), shadowCol, icon);
        draw->AddText(font2::WeaponsIco, 20.0f, iconPos, col, icon);
        ImGui::PopFont();
    }
}

// Dibujar huesos (Esqueleto)
static void DrawBonesESP(const TargetInfo& target, ImDrawList* draw, int clientX, int clientY, const Matrix4x4& viewMatrix, int emuWidth, int emuHeight)
{
    if (target.bones.size() < 14) return;

    auto toScreen = [&](const Vector3& world) -> ImVec2 {
        if (world.x == 0.0f && world.y == 0.0f && world.z == 0.0f) return ImVec2(0, 0);
        return WorldToScreenImVec2(viewMatrix, world, emuWidth, emuHeight);
    };

    ImU32 col = target.isVisible ? ImGui::GetColorU32(g_BonesColor) : ImGui::GetColorU32(g_InvisibleColor);
    if (g_EspRainbowGlobal) {
        col = GetRainbowColor(target.distanceToCrosshair / emuWidth);
    }

    // Conexiones del esqueleto corregidas segun los indices de bones[]
    // 0: Head, 1: Neck, 2: Spine, 3: Hip, 4: LeftSholder, 5: LeftElbow, 6: LeftHand,
    // 7: RightSholder, 8: RightElbow, 9: RightHand, 10: LeftCalf, 11: LeftFoot, 12: RightCalf, 13: RightFoot
    std::vector<std::pair<int, int>> links = {
        {0, 1},   // Cabeza -> Cuello
        {1, 2},   // Cuello -> Columna
        {2, 3},   // Columna -> Cadera

        // Brazo Izquierdo
        {2, 4},   // Columna -> Hombro Izquierdo
        {4, 5},   // Hombro Izquierdo -> Codo Izquierdo
        {5, 6},   // Codo Izquierdo -> Mano Izquierda

        // Brazo Derecho
        {2, 7},   // Columna -> Hombro Derecho
        {7, 8},   // Hombro Derecho -> Codo Derecho
        {8, 9},   // Codo Derecho -> Mano Derecha

        // Pierna Izquierda
        {3, 10},  // Cadera -> Pantorrilla Izquierda
        {10, 11}, // Pantorrilla Izquierda -> Pie Izquierdo

        // Pierna Derecha
        {3, 12},  // Cadera -> Pantorrilla Derecha
        {12, 13}  // Pantorrilla Derecha -> Pie Derecho
    };

    for (auto& link : links) {
        if (link.first >= target.bones.size() || link.second >= target.bones.size()) continue;
        
        Vector3 w1 = target.bones[link.first];
        Vector3 w2 = target.bones[link.second];
        
        // Si algun hueso falló en leerse (es 0,0,0), evitamos dibujar esa linea rota
        if ((w1.x == 0.0f && w1.y == 0.0f && w1.z == 0.0f) || 
            (w2.x == 0.0f && w2.y == 0.0f && w2.z == 0.0f)) continue;

        ImVec2 p1 = toScreen(w1);
        ImVec2 p2 = toScreen(w2);

        // Asegurar coordenadas válidas de pantalla antes de dibujar
        if (p1.x > 0 && p2.x > 0 && p1.x < emuWidth && p2.x < emuWidth && p1.y > 0 && p2.y > 0) {
            draw->AddLine(ImVec2(clientX + p1.x, clientY + p1.y), ImVec2(clientX + p2.x, clientY + p2.y), col, 1.5f);
        }
    }

    // Dibujar círculo estético en la cabeza para completar el esqueleto
    if (target.bones[0].x != 0.0f || target.bones[0].y != 0.0f || target.bones[0].z != 0.0f) {
        ImVec2 headPos2D = toScreen(target.bones[0]);
        if (headPos2D.x > 0 && headPos2D.x < emuWidth && headPos2D.y > 0) {
            float cornerHeight = std::abs(target.headScreen.y - target.footScreen.y);
            float radius = cornerHeight * 0.08f; // Escala dinámica basada en la distancia
            if (radius < 3.0f) radius = 3.0f;
            if (radius > 10.0f) radius = 10.0f;
            draw->AddCircle(ImVec2(clientX + headPos2D.x, clientY + headPos2D.y), radius, col, 16, 1.5f);
        }
    }
}

// Optimizar juego y rendimiento (Boost de FPS)
inline void OptimizeGameAndBoostFPS() {
    // 1. Establecer prioridad ALTA para los procesos del emulador
    const char* emulators[] = { "HD-Player.exe", "MEmuHeadless.exe", "LdVBoxHeadless.exe", "AndroidProcess.exe", "dnplayer.exe" };
    for (const char* emuName : emulators) {
        DWORD pid = GetProcZ(emuName);
        if (pid != 0) {
            HANDLE hProc = OpenProcess(PROCESS_SET_INFORMATION | PROCESS_QUERY_INFORMATION, FALSE, pid);
            if (hProc != NULL) {
                SetPriorityClass(hProc, HIGH_PRIORITY_CLASS);
                CloseHandle(hProc);
            }
        }
    }
    
    // 2. Establecer prioridad de nuestro overlay a HIGH para refresco ultra-suave
    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
    
    // 3. Optimizacion de Android (Requiere ADB Activo)
    if (adbInitialized) {
        // Limpiar caché de RAM de Linux/Android (PageCache, dentries, inodes)
        ComdADBZ("su -c \"echo 3 > /proc/sys/vm/drop_caches\"");
        
        // Forzar Garbage Collection (GC) en la aplicación del juego para liberar memoria trabada
        if (!GamePackage.empty()) {
            std::string gcCmd = "su -c \"am send-trim-memory " + GamePackage + " HIDDEN\"";
            ComdADBZ(gcCmd.c_str());
        }
    }
    
    // 4. Limpiar caché del Hypervisor / DMA (Lecturas atrasadas)
    CacheZ.clear();
}

// Función principal ESP_line
void ESP_line()
{
    // 1. Permitir que la ESP funcione de forma global o de manera individual independiente (Incluir Keybinds y FPS)
    bool anyEspActive = g_EspEnabled || g_EspBox || g_EspBones || g_EspName || g_EspDistance || g_EspLine || g_EspHealth || g_EspWeaponIcon || g_EspWeaponText || g_AimAssist || aimsilent || g_ShowKeybinds || g_ShowFps;
    if (!anyEspActive) return;

    static std::vector<TargetInfo> cachedTargets;
    static Matrix4x4 cachedViewMatrix;
    static Vector3 cachedLocalPos;
    static int cachedEmuWidth = 0, cachedEmuHeight = 0, cachedClientX = 0, cachedClientY = 0;
    static auto lastUpdateTime = std::chrono::steady_clock::now();

    // Contadores estáticos para ráfagas de disparos para el multi-hitbox (Pecho -> Cabeza)
    static int currentShotIndex = 0;
    static bool wasFiring = false;
    static auto lastShotTime = std::chrono::steady_clock::now();

    int emuWidth, emuHeight, clientX, clientY;
    uint32_t localPlayer = 0;
    Matrix4x4 viewMatrix;
    Vector3 localPos;
    uint32_t entities = 0;
    int entitiesCount = 0;
    
    bool dataValid = GetGameData(localPlayer, viewMatrix, localPos, entities, entitiesCount, emuWidth, emuHeight, clientX, clientY);

    // Leer el estado de disparo del jugador local
    bool isCurrentlyFiring = false;
    if (dataValid && localPlayer != 0) {
        ReadZ(localPlayer + AimSilent1, isCurrentlyFiring);

        // ======================== NO RECOIL ========================
        if (NoRecoil) {
            uint32_t weaponPtr = 0;
            if (ReadZ(localPlayer + Weapon, weaponPtr) && weaponPtr) {
                uint32_t weaponData = 0;
                if (ReadZ(weaponPtr + WeaponData, weaponData) && weaponData) {
                    // Anular el retroceso del arma (escribir 0.0f en el multiplicador de recoil)
                    WriteZ<float>(weaponData + WeaponRecoil, 0.0f);
                }
            }
        }
    }

    // Actualizar ráfagas y contador de disparos (Promedio de 100ms entre balas automáticas)
    bool isFiring = isCurrentlyFiring || (GetAsyncKeyState(g_AimKey) & 0x8000);
    if (isFiring) {
        auto nowTime = std::chrono::steady_clock::now();
        if (!wasFiring) {
            currentShotIndex = 1;
            lastShotTime = nowTime;
            wasFiring = true;
        } else {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(nowTime - lastShotTime).count();
            if (elapsed >= 100) { // 100ms de promedio por disparo
                currentShotIndex++;
                lastShotTime = nowTime;
            }
        }
    } else {
        wasFiring = false;
        currentShotIndex = 0;
    }

    auto now = std::chrono::steady_clock::now();

    struct CachedEnemy {
        TargetInfo info;
        std::chrono::steady_clock::time_point lastSeen;
    };
    static std::vector<CachedEnemy> enemyCache;

    // Limpiar enemigos de la caché que no se han visto en más de 2000 ms para evitar acumulación
    for (auto it = enemyCache.begin(); it != enemyCache.end(); ) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - it->lastSeen).count() > 2000) {
            it = enemyCache.erase(it);
        } else {
            ++it;
        }
    }

    std::vector<TargetInfo> validTargets;
    if (dataValid) {
        for (uint32_t i = 0; i < entitiesCount; i++) {
            uint32_t entry = entities + i * 0x10;
            int hash = 0;
            ReadZ(entry, hash);
            if (hash < 0) continue;
            uint32_t entity = 0;
            ReadZ(entry + 0x0C, entity);
            if (entity == 0 || entity == localPlayer) continue;

            TargetInfo info = ReadEnemyData(entity, localPos, viewMatrix, emuWidth, emuHeight);

            // Buscar si ya tenemos a este enemigo en nuestra caché
            CachedEnemy* found = nullptr;
            for (auto& e : enemyCache) {
                if (e.info.entity == entity) {
                    found = &e;
                    break;
                }
            }

            if (info.entity != 0) {
                // Lectura exitosa: actualizamos la caché persistente
                if (found != nullptr) {
                    found->info = info;
                    found->lastSeen = now;
                } else {
                    enemyCache.push_back({ info, now });
                }
            } else {
                // Lectura fallida temporalmente: intentamos recuperar de la caché
                if (found != nullptr) {
                    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - found->lastSeen).count();
                    // Si se vio hace menos de 800 ms y no estaba muerto, lo rescatamos de la caché para evitar parpadeos
                    if (elapsed < 800 && !found->info.isDead) {
                        info = found->info;
                        
                        // Recalcular las proyecciones en pantalla 2D con la matriz de vista actual
                        info.headScreen = WorldToScreenImVec2(viewMatrix, info.headPos, emuWidth, emuHeight);
                        info.footScreen = WorldToScreenImVec2(viewMatrix, info.rootPos, emuWidth, emuHeight);
                        
                        float dx = info.headScreen.x - emuWidth / 2.0f;
                        float dy = info.headScreen.y - emuHeight / 2.0f;
                        info.distanceToCrosshair = sqrtf(dx * dx + dy * dy);
                    }
                }
            }

            if (info.entity == 0) continue;
            if (!info.isVisible && !g_EspShowInvisible) continue;
            if (info.isTeam && !g_EspShowTeam && !g_AimTeammates) continue;
            if (info.isDead) continue;
            if (g_EspMaxDistance > 0 && info.distance3D > g_EspMaxDistance) continue;
            validTargets.push_back(info);
        }
    }

    if (dataValid && !validTargets.empty()) {
        cachedTargets = validTargets;
        cachedViewMatrix = viewMatrix;
        cachedLocalPos = localPos;
        cachedEmuWidth = emuWidth;
        cachedEmuHeight = emuHeight;
        cachedClientX = clientX;
        cachedClientY = clientY;
        lastUpdateTime = now;
    } else {
        // Anti-Flicker: usar caché si falló la lectura pero fue hace menos de 1500ms
        if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count() < 1500) {
            validTargets = cachedTargets;
            viewMatrix = cachedViewMatrix;
            localPos = cachedLocalPos;
            emuWidth = cachedEmuWidth;
            emuHeight = cachedEmuHeight;
            clientX = cachedClientX;
            clientY = cachedClientY;
        } else {
            return;
        }
    }

    if (validTargets.empty()) return;

    // Ordenar por distancia al crosshair si solo se dibuja el más cercano
    if (g_EspNearestOnly && g_EspLine && g_EspLineStyle == 4) {
        std::sort(validTargets.begin(), validTargets.end(),
            [](const TargetInfo& a, const TargetInfo& b) { return a.distanceToCrosshair < b.distanceToCrosshair; });
        validTargets.resize(1);
    }

    ImDrawList* draw = ImGui::GetBackgroundDrawList();
    draw->PushClipRect(ImVec2(clientX, clientY), ImVec2(clientX + emuWidth, clientY + emuHeight), true);

    // Calcular punto de inicio de líneas (si está activada)
    ImVec2 lineStartPoint;
    bool drawLine = g_EspLine; // Activado si g_EspLine está activo (Nearest Only también usa lineStartPoint)
    if (drawLine) {
        switch (g_EspLineStyle) {
        case 0: lineStartPoint = ImVec2(clientX + emuWidth / 2, clientY + 80.0f); break;
        case 1: lineStartPoint = ImVec2(clientX + emuWidth / 2, clientY + emuHeight / 2); break;
        case 2: {
            ImVec2 lpScr = WorldToScreenImVec2(viewMatrix, localPos, emuWidth, emuHeight);
            if (lpScr.x > 0) lineStartPoint = ImVec2(clientX + lpScr.x, clientY + lpScr.y);
            else drawLine = false;
            break;
        }
        case 3: lineStartPoint = ImVec2(clientX + emuWidth / 2, clientY + emuHeight / 2); break;
        default: lineStartPoint = ImVec2(clientX + emuWidth / 2, clientY + 80.0f); break;
        }
    }

    for (const auto& target : validTargets) {
        if (target.isTeam && !g_EspShowTeam) continue;

        float cornerHeight = std::abs(target.headScreen.y - target.footScreen.y);
        float cornerWidth = cornerHeight * 0.65f;
        float X = clientX + target.headScreen.x - cornerWidth / 2;
        float Y = clientY + target.headScreen.y - cornerHeight * 0.15f;
        float H = cornerHeight * 1.2f;

        if (g_EspLine) {
            if (g_EspNearestOnly) {
                // Si es Nearest Only, solo dibujamos la línea para el objetivo más cercano (que ya es el primero de la lista)
                if (&target == &validTargets[0])
                    DrawLineESP(target, lineStartPoint, draw, clientX, clientY, emuWidth, emuHeight);
            }
            else if (drawLine) {
                DrawLineESP(target, lineStartPoint, draw, clientX, clientY, emuWidth, emuHeight);
            }
        }

        if (g_EspBox && g_EspBoxStyle != 0)
            DrawBoxESP(target, draw, clientX, clientY, cornerHeight, cornerWidth, viewMatrix, emuWidth, emuHeight);

        if (g_EspName || g_EspDistance || g_EspID)
            DrawTextESP(target, draw, clientX, clientY, cornerHeight, cornerWidth);

        if (g_EspHealth || g_EspShield)
            DrawHealthESP(target, draw, clientX, clientY, cornerHeight, cornerWidth, X, Y, H);

        if (g_EspWeaponIcon || g_EspWeaponText)
            DrawWeaponESP(target, draw, clientX, clientY, cornerHeight, cornerWidth, X, Y, H);

        if (g_EspBones)
            DrawBonesESP(target, draw, clientX, clientY, viewMatrix, emuWidth, emuHeight);

        if (g_EspKnockedIcon && target.isKnocked) {
            const char* txt = "KNOCKED!";
            ImVec2 sz = ImGui::CalcTextSize(txt);
            draw->AddText(ImVec2(clientX + target.headScreen.x - sz.x / 2, clientY + target.headScreen.y - 55), IM_COL32(255, 80, 80, 255), txt);
        }
    }

    // Círculo FOV para ESP (ahora se dibuja de manera unificada al final si está activado)

    // ======================== ASISTENCIA DE APUNTADO (AIM ASSIST) ========================
    if (g_AimAssist && !Main_Window && !IsMouseCursorVisible()) {
        float bestDistanceToCrosshair = FLT_MAX;
        TargetInfo bestTarget = {};
        bool targetFound = false;

        ImVec2 screenCenter(clientX + emuWidth / 2.0f, clientY + emuHeight / 2.0f);

        for (const auto& target : validTargets) {
            // Solo apuntar a enemigos vivos, no abatidos, visibles y que no sean compañeros (a menos que se permita apuntar a bots/amigos)
            if (target.isDead || target.isKnocked || !target.isVisible || (target.isTeam && !g_AimTeammates)) continue;

            // Conseguir la posición del hueso seleccionado para apuntar con soporte de Multi-Hitbox (Pecho -> Cabeza)
            int activeBone = g_AimTargetBone;
            if (g_ChestShotsCount > 0) {
                if (currentShotIndex <= g_ChestShotsCount) {
                    activeBone = 2; // Pecho (Torso, index 2)
                } else {
                    activeBone = 0; // Cabeza
                }
            }

            Vector3 targetBoneWorld;
            if (activeBone == 0 && target.bones.size() > 0 && target.bones[0].y != 0) {
                targetBoneWorld = target.bones[0]; // Cabeza
            } else if (activeBone == 1 && target.bones.size() > 2 && target.bones[2].y != 0) {
                targetBoneWorld = target.bones[2]; // Cuello (ahora apunta a la Columna físicamente)
            } else if (activeBone == 2 && target.bones.size() > 1 && target.bones[1].y != 0) {
                targetBoneWorld = target.bones[1]; // Torso / Columna (ahora apunta al Cuello físicamente)
            } else if (activeBone == 3 && target.bones.size() > 3 && target.bones[3].y != 0) {
                targetBoneWorld = target.bones[3]; // Hip / Cadera
            } else {
                targetBoneWorld = target.headPos; // Fallback
            }

            if (targetBoneWorld.x == 0 && targetBoneWorld.y == 0 && targetBoneWorld.z == 0) continue;

            ImVec2 boneScreen = WorldToScreenImVec2(viewMatrix, targetBoneWorld, emuWidth, emuHeight);
            ImVec2 absoluteScreenPos(clientX + boneScreen.x, clientY + boneScreen.y);

            // Calcular distancia del enemigo en pantalla al centro de la retícula
            float dx = absoluteScreenPos.x - screenCenter.x;
            float dy = absoluteScreenPos.y - screenCenter.y;
            float dist = sqrtf(dx * dx + dy * dy);

            // Solo apuntar si está dentro del radio FOV (si se activó el FOV manualmente)
            if (!g_EspFov || dist <= g_EspFovRadius) {
                // Priorizar al más cercano al centro de la cruz en lugar de la distancia 3D
                if (dist < bestDistanceToCrosshair) {
                    bestDistanceToCrosshair = dist;
                    bestTarget = target;
                    targetFound = true;
                }
            }
        }

        // Si encontramos un objetivo válido dentro del FOV, dibujamos la línea siempre (sin requerir tecla física)
        if (targetFound) {
            int activeBone = g_AimTargetBone;
            if (g_ChestShotsCount > 0) {
                if (currentShotIndex <= g_ChestShotsCount) {
                    activeBone = 2; // Pecho (Torso)
                } else {
                    activeBone = 0; // Cabeza
                }
            }

            Vector3 targetBoneWorld;
            if (activeBone == 0 && bestTarget.bones.size() > 0 && bestTarget.bones[0].y != 0) {
                targetBoneWorld = bestTarget.bones[0];
            } else if (activeBone == 1 && bestTarget.bones.size() > 2 && bestTarget.bones[2].y != 0) {
                targetBoneWorld = bestTarget.bones[2]; // Cuello (ahora apunta a la Columna físicamente)
            } else if (activeBone == 2 && bestTarget.bones.size() > 1 && bestTarget.bones[1].y != 0) {
                targetBoneWorld = bestTarget.bones[1]; // Torso (ahora apunta al Cuello físicamente)
            } else if (activeBone == 3 && bestTarget.bones.size() > 3 && bestTarget.bones[3].y != 0) {
                targetBoneWorld = bestTarget.bones[3]; // Hip
            } else {
                targetBoneWorld = bestTarget.headPos;
            }

            if (targetBoneWorld.x != 0 || targetBoneWorld.y != 0 || targetBoneWorld.z != 0) {
                // DIBUJAR LÍNEA DE PREDICCIÓN SOLO AL MEJOR OBJETIVO (Siempre visible si g_AimAssist está activo)
                ImVec2 boneScreen = WorldToScreenImVec2(viewMatrix, targetBoneWorld, emuWidth, emuHeight);
                if (boneScreen.x > 0 && boneScreen.y > 0) {
                    ImVec2 absoluteScreenPos(clientX + boneScreen.x, clientY + boneScreen.y);
                    draw->AddLine(screenCenter, absoluteScreenPos, ImColor(0, 255, 0), 1.5f);
                }

                // APLICAR ASISTENCIA (Escribir en memoria o mover mouse) ÚNICAMENTE SI SE MANTIENE PRESIONADA LA TECLA DE APUNTAR
                if (GetAsyncKeyState(g_AimKey) & 0x8000) {
                    if (g_AimAssistType == 0) { // Memory Lock (Directo)
                        Vector3 cameraPosition = {0, 0, 0};
                        uint32_t mainCameraTransform = 0;
                        bool cameraReadSuccess = false;
                        
                        if (ReadZ(localPlayer + MainCameraTransform, mainCameraTransform) && mainCameraTransform != 0) {
                            if (GetPosition(mainCameraTransform, cameraPosition)) {
                                cameraReadSuccess = true;
                            }
                        }
                        
                        if (!cameraReadSuccess) {
                            cameraPosition = GetLocalHeadPosition(localPlayer);
                            if (cameraPosition.x == 0 && cameraPosition.y == 0 && cameraPosition.z == 0) {
                                cameraPosition = localPos; // Fallback
                            }
                        }

                        // Calcular la rotación (Quaternion) hacia el hueso objetivo
                        Quaternion aimRotation = AimBZv2::GetRotationToLocation(targetBoneWorld, 0.0f, cameraPosition);

                        // Escribir la rotación en la memoria
                        WriteZ(localPlayer + AimRotation, aimRotation);
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                    else if (g_AimAssistType == 1) { // Memory Smooth (Rotación Suave)
                        Vector3 cameraPosition = {0, 0, 0};
                        uint32_t mainCameraTransform = 0;
                        bool cameraReadSuccess = false;
                        
                        if (ReadZ(localPlayer + MainCameraTransform, mainCameraTransform) && mainCameraTransform != 0) {
                            if (GetPosition(mainCameraTransform, cameraPosition)) {
                                cameraReadSuccess = true;
                            }
                        }
                        
                        if (!cameraReadSuccess) {
                            cameraPosition = GetLocalHeadPosition(localPlayer);
                            if (cameraPosition.x == 0 && cameraPosition.y == 0 && cameraPosition.z == 0) {
                                cameraPosition = localPos; // Fallback
                            }
                        }

                        Quaternion aimRotation = AimBZv2::GetRotationToLocation(targetBoneWorld, 0.0f, cameraPosition);
                        
                        // Leer la rotación actual del juego
                        Quaternion currentRotation = {0, 0, 0, 1};
                        if (ReadZ(localPlayer + AimRotation, currentRotation)) {
                            // Interpolación esférica (Slerp) basada en el valor de Smoothness
                            float t = 1.0f - g_AimSmoothness;
                            if (t < 0.01f) t = 0.01f;
                            if (t > 1.0f) t = 1.0f;
                            
                            Quaternion smoothRotation = Quaternion::Slerp(currentRotation, aimRotation, t);
                            WriteZ(localPlayer + AimRotation, smoothRotation);
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                    else if (g_AimAssistType == 2) { // Mouse Emulation (Emulación de Mouse)
                        // Si el cursor de la pantalla del emulador está desbloqueado o visible, no forzar la mira
                        if (!IsMouseCursorVisible()) {
                            ImVec2 boneScreen = WorldToScreenImVec2(viewMatrix, targetBoneWorld, emuWidth, emuHeight);
                            if (boneScreen.x > 0 && boneScreen.y > 0) {
                                ImVec2 absoluteScreenPos(clientX + boneScreen.x, clientY + boneScreen.y);
                                float dx = absoluteScreenPos.x - screenCenter.x;
                                float dy = absoluteScreenPos.y - screenCenter.y;
                                
                                // Suavizado dinámico escalado correctamente para mouse_event
                                float speedFactor = (1.0f - g_AimSmoothness) * 0.05f;
                                if (speedFactor < 0.001f) speedFactor = 0.001f;
                                if (speedFactor > 1.0f) speedFactor = 1.0f;
                                
                                float moveX_f = dx * speedFactor;
                                float moveY_f = dy * speedFactor;
                                
                                // Solución al problema "ni siquiera se centra":
                                // Si la distancia es pequeña, moveX/moveY se truncaba a 0, dejándolo descentrado.
                                // Ahora forzamos un mínimo de 1 pixel de movimiento hasta que esté perfectamente en el centro.
                                int moveX = 0;
                                if (abs(dx) > 0.5f) {
                                    if (abs(moveX_f) < 1.0f) moveX = (dx > 0) ? 1 : -1;
                                    else moveX = (int)moveX_f;
                                }
                                
                                int moveY = 0;
                                if (abs(dy) > 0.5f) {
                                    if (abs(moveY_f) < 1.0f) moveY = (dy > 0) ? 1 : -1;
                                    else moveY = (int)moveY_f;
                                }
                                
                                if (moveX != 0 || moveY != 0) {
                                    mouse_event(MOUSEEVENTF_MOVE, moveX, moveY, 0, 0);
                                }
                            }
                        }
                    }
                    else if (g_AimAssistType == 3) { // Silent Aim (Disparo Silencioso integrado)
                        uint32_t weaponInstance = 0;
                        if (ReadZ(localPlayer + AimSilent2, weaponInstance) && weaponInstance != 0) {
                            Vector3 fireStart = {0,0,0};
                            if (ReadZ(weaponInstance + AimSilent3, fireStart)) {
                                Vector3 aimDir = targetBoneWorld - fireStart;
                                float magnitude = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y + aimDir.z * aimDir.z);
                                if (magnitude > 0.001f && !std::isnan(aimDir.x) && !std::isnan(aimDir.y) && !std::isnan(aimDir.z) &&
                                    !std::isinf(aimDir.x) && !std::isinf(aimDir.y) && !std::isinf(aimDir.z)) {
                                    aimDir.x /= magnitude;
                                    aimDir.y /= magnitude;
                                    aimDir.z /= magnitude;
                                    WriteZ<Vector3>(weaponInstance + AimSilent4, aimDir);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // ======================== SILENT AIM (Disparo Silencioso) ========================
    if (aimsilent && !Main_Window && !IsMouseCursorVisible()) {
        float bestDistanceToCrosshair = FLT_MAX;
        TargetInfo bestTarget = {};
        bool targetFound = false;

        ImVec2 screenCenter(clientX + emuWidth / 2.0f, clientY + emuHeight / 2.0f);

        for (const auto& target : validTargets) {
            if (target.isDead || target.isKnocked || !target.isVisible || (target.isTeam && !g_AimTeammates)) continue;

            // Conseguir la posición del hueso con soporte de Multi-Hitbox (Pecho -> Cabeza)
            int activeBone = g_AimTargetBone;
            if (g_ChestShotsCount > 0) {
                if (currentShotIndex <= g_ChestShotsCount) {
                    activeBone = 2; // Pecho (Torso)
                } else {
                    activeBone = 0; // Cabeza
                }
            }

            Vector3 targetBoneWorld;
            if (activeBone == 0 && target.bones.size() > 0 && target.bones[0].y != 0) {
                targetBoneWorld = target.bones[0]; // Cabeza
            } else if (activeBone == 1 && target.bones.size() > 2 && target.bones[2].y != 0) {
                targetBoneWorld = target.bones[2]; // Cuello (ahora apunta a la Columna físicamente)
            } else if (activeBone == 2 && target.bones.size() > 1 && target.bones[1].y != 0) {
                targetBoneWorld = target.bones[1]; // Torso / Columna (ahora apunta al Cuello físicamente)
            } else if (activeBone == 3 && target.bones.size() > 3 && target.bones[3].y != 0) {
                targetBoneWorld = target.bones[3]; // Hip / Cadera
            } else {
                targetBoneWorld = target.headPos; // Fallback
            }

            if (targetBoneWorld.x == 0 && targetBoneWorld.y == 0 && targetBoneWorld.z == 0) continue;

            ImVec2 boneScreen = WorldToScreenImVec2(viewMatrix, targetBoneWorld, emuWidth, emuHeight);
            ImVec2 absoluteScreenPos(clientX + boneScreen.x, clientY + boneScreen.y);

            float dx = absoluteScreenPos.x - screenCenter.x;
            float dy = absoluteScreenPos.y - screenCenter.y;
            float dist = sqrtf(dx * dx + dy * dy);

            // Verificar si el objetivo está dentro del rango del FOV (si se activó el FOV manualmente)
            if (!g_EspFov || dist <= g_EspFovRadius) {
                // Priorizar al más cercano al centro de la cruz en lugar de la distancia 3D
                if (dist < bestDistanceToCrosshair) {
                    bestDistanceToCrosshair = dist;
                    bestTarget = target;
                    targetFound = true;
                }
            }
        }

        if (targetFound) {
            // Conseguir la posición del hueso con soporte de Multi-Hitbox (Pecho -> Cabeza)
            int activeBone = g_AimTargetBone;
            if (g_ChestShotsCount > 0) {
                if (currentShotIndex <= g_ChestShotsCount) {
                    activeBone = 2; // Pecho (Torso)
                } else {
                    activeBone = 0; // Cabeza
                }
            }

            Vector3 targetBoneWorld;
            if (activeBone == 0 && bestTarget.bones.size() > 0 && bestTarget.bones[0].y != 0) {
                targetBoneWorld = bestTarget.bones[0];
            } else if (activeBone == 1 && bestTarget.bones.size() > 2 && bestTarget.bones[2].y != 0) {
                targetBoneWorld = bestTarget.bones[2]; // Cuello (ahora apunta a la Columna físicamente)
            } else if (activeBone == 2 && bestTarget.bones.size() > 1 && bestTarget.bones[1].y != 0) {
                targetBoneWorld = bestTarget.bones[1]; // Torso (ahora apunta al Cuello físicamente)
            } else if (activeBone == 3 && bestTarget.bones.size() > 3 && bestTarget.bones[3].y != 0) {
                targetBoneWorld = bestTarget.bones[3]; // Hip
            } else {
                targetBoneWorld = bestTarget.headPos;
            }
            
            if (targetBoneWorld.x != 0 || targetBoneWorld.y != 0 || targetBoneWorld.z != 0) {
                // Solo aplicar si el jugador está disparando en el juego o manteniendo presionada la tecla del apuntado
                if (isCurrentlyFiring || (GetAsyncKeyState(g_AimKey) & 0x8000)) {
                    uint32_t weaponInstance = 0;
                    // AimSilent2 (0x948) es el puntero de disparo del arma local (Correcto para Silent)
                    if (ReadZ(localPlayer + AimSilent2, weaponInstance) && weaponInstance != 0) {
                        Vector3 fireStart = {0,0,0};
                        // AimSilent3 (0x38) es la posición de inicio del proyectil (Correcto)
                        if (ReadZ(weaponInstance + AimSilent3, fireStart)) {
                            Vector3 aimDir = targetBoneWorld - fireStart;

                            // Validar y normalizar el vector dirección para máxima estabilidad y precisión
                            float magnitude = std::sqrt(aimDir.x * aimDir.x + aimDir.y * aimDir.y + aimDir.z * aimDir.z);
                            if (magnitude > 0.001f && !std::isnan(aimDir.x) && !std::isnan(aimDir.y) && !std::isnan(aimDir.z) &&
                                !std::isinf(aimDir.x) && !std::isinf(aimDir.y) && !std::isinf(aimDir.z)) {
                                
                                // Normalización del vector (escala unitaria = 1.0f)
                                aimDir.x /= magnitude;
                                aimDir.y /= magnitude;
                                aimDir.z /= magnitude;

                                // Escribir en AimSilent4 (0x2C) que es el vector dirección de bala esperado (Correcto)
                                WriteZ<Vector3>(weaponInstance + AimSilent4, aimDir);
                            }
                        }
                    }
                }
            }
        }
    }

    // Dibujar Círculo FOV si está activado manualmente
    if (g_EspFov) {
        ImVec2 fovCenter = ImVec2(clientX + emuWidth / 2, clientY + emuHeight / 2);
        ImU32 circleColor;
        
        if (g_EspRainbowGlobal) {
            // Generar color RGB dinámico (Arcoíris)
            float r, g_c, b_c;
            ImGui::ColorConvertHSVtoRGB(fmodf((float)ImGui::GetTime() * 0.5f, 1.0f), 1.0f, 1.0f, r, g_c, b_c);
            circleColor = ImGui::GetColorU32(ImVec4(r, g_c, b_c, 1.0f));
        } else {
            circleColor = ImGui::GetColorU32(g_FovColor);
        }
        
        draw->AddCircle(fovCenter, g_EspFovRadius, circleColor, 64, 1.5f);
    }

    // Dibujar Contador de FPS si está activado
    if (g_ShowFps) {
        char fpsText[32];
        sprintf(fpsText, "FPS: %.0f", ImGui::GetIO().Framerate);
        ImVec2 sz = ImGui::CalcTextSize(fpsText);
        
        // Posición: Esquina superior derecha con margen de 20px
        ImVec2 pos(clientX + emuWidth - sz.x - 20.0f, clientY + 20.0f);
        
        // Fondo negro semi-transparente para máxima visibilidad (diseño premium con esquinas redondeadas)
        draw->AddRectFilled(ImVec2(pos.x - 8.0f, pos.y - 6.0f), ImVec2(pos.x + sz.x + 8.0f, pos.y + sz.y + 6.0f), IM_COL32(0, 0, 0, 180), 6.0f);
        
        // Efecto neon glow o color verde brillante para estilo de juego
        draw->AddText(pos, IM_COL32(0, 255, 120, 255), fpsText);
    }

    // Dibujar Keybinds si está activado
    if (g_ShowKeybinds) {
        float boxWidth = 190.0f;
        float headerHeight = 35.0f;
        float itemHeight = 25.0f;
        
        struct KeybindItem {
            const char* name;
            bool active;
        };
        
        KeybindItem items[] = {
            { "Aim Assist", g_AimAssist },
            { "ESP Player", EnabledEsp },
            { "No Recoil", NoRecoil },
            { "Fly Hack", FlyWall },
            { "Speed Hack", caidarapida },
            { "No Reload", noreloadpro }
        };
        int numItems = sizeof(items) / sizeof(items[0]);
        
        float boxHeight = headerHeight + (numItems * itemHeight) + 10.0f;
        ImVec2 boxPos(clientX + emuWidth - boxWidth - 20.0f, clientY + 70.0f);
        
        // 1. Fondo negro semi-transparente con bordes redondeados y sombra
        for (int i = 1; i <= 6; i++) {
            float grow = (float)i * 1.5f;
            draw->AddRectFilled(
                ImVec2(boxPos.x - grow, boxPos.y - grow + 2.0f),
                ImVec2(boxPos.x + boxWidth + grow, boxPos.y + boxHeight + grow + 2.0f),
                IM_COL32(0, 0, 0, (int)(15.0f * (1.0f - (float)i / 6.0f))),
                8.0f + grow
            );
        }
        draw->AddRectFilled(boxPos, ImVec2(boxPos.x + boxWidth, boxPos.y + boxHeight), IM_COL32(10, 10, 10, 220), 8.0f);
        draw->AddRect(boxPos, ImVec2(boxPos.x + boxWidth, boxPos.y + boxHeight), IM_COL32(255, 255, 255, 15), 8.0f); // Borde sutil
        
        // 2. Círculo indicador rojo al lado del título
        ImVec2 dotCenter(boxPos.x + 18.0f, boxPos.y + 18.0f);
        draw->AddCircleFilled(dotCenter, 4.5f, IM_COL32(255, 0, 80, 255)); // Dot rojo puro
        draw->AddCircleFilled(dotCenter, 7.5f, IM_COL32(255, 0, 80, 50));  // Glow suave
        
        // 3. Título "Keybinds"
        draw->AddText(ImVec2(boxPos.x + 32.0f, boxPos.y + 10.0f), IM_COL32(255, 255, 255, 255), "Keybinds");
        
        // 4. Separador horizontal rojo brillante
        draw->AddLine(
            ImVec2(boxPos.x + 10.0f, boxPos.y + headerHeight - 2.0f),
            ImVec2(boxPos.x + boxWidth - 10.0f, boxPos.y + headerHeight - 2.0f),
            IM_COL32(255, 0, 80, 255),
            1.5f
        );
        
        // 5. Lista de funciones
        float currentY = boxPos.y + headerHeight + 5.0f;
        for (int i = 0; i < numItems; i++) {
            // Nombre de la función
            draw->AddText(ImVec2(boxPos.x + 15.0f, currentY), IM_COL32(220, 220, 220, 255), items[i].name);
            
            // Estado (ON / OFF)
            const char* statusStr = items[i].active ? "ON" : "OFF";
            ImVec2 szStatus = ImGui::CalcTextSize(statusStr);
            ImVec2 statusPos(boxPos.x + boxWidth - 15.0f - szStatus.x, currentY);
            
            ImU32 statusColor = items[i].active ? IM_COL32(0, 255, 120, 255) : IM_COL32(140, 140, 140, 255);
            draw->AddText(statusPos, statusColor, statusStr);
            
            currentY += itemHeight;
        }
    }

    draw->PopClipRect();
}
