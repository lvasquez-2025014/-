#pragma once
#include <cstdint>
#include <string>
// Offsets Principales

std::string GamePackage;
uintptr_t Il2Cpp;
uintptr_t Libunity;
uintptr_t WallHackLocation;
uintptr_t NightLocation;
uintptr_t InitBase;
uintptr_t StaticClass;
uintptr_t DictionaryEntities;
uintptr_t WukongOrion;
uintptr_t ViewMatrix;


uintptr_t MatchStatus;
uintptr_t CurrentMatch;


uintptr_t LocalPlayer;
uintptr_t Player_IsDead;
uintptr_t Player_Name;
uintptr_t Player_Data;
uintptr_t Player_ShadowBase;
uintptr_t AvatarManager;
uintptr_t Avatar;
uintptr_t Avatar_IsVisible;
uintptr_t Avatar_Data;
uintptr_t Avatar_Data_IsTeam;
uintptr_t HealdShieldEP;
uintptr_t XPose;


uintptr_t FollowCamera;
uintptr_t Camera;
uintptr_t AimRotation;
uintptr_t MainCameraTransform;


uintptr_t Weapon;
uintptr_t WeaponData;
uintptr_t WeaponRecoil;

uintptr_t LocalPlayerAttributes;
uintptr_t NoReloadOffset;
uintptr_t MedikitOffset;
uintptr_t Speed;
uintptr_t RageCollider;
uintptr_t isFiringOffset;
uintptr_t BaseProfileInfo;
uintptr_t PlayerIDOffset;
uintptr_t AmmoOffs;
uintptr_t FasFireOffs;
uintptr_t OffsQuickSwitch;
uintptr_t AimSilent1;
uintptr_t AimSilent2;
uintptr_t AimSilent3;
uintptr_t AimSilent4;
uintptr_t FixedDeltaTime;
uintptr_t GameTimer;

uintptr_t Head;


uintptr_t Spine;
uintptr_t Root;
uintptr_t Hip;


uintptr_t LeftSholder;
uintptr_t LeftElbow;
uintptr_t LeftWrist;
uintptr_t LeftHand;


uintptr_t RightSholder;
uintptr_t RightElbow;
uintptr_t RightWrist;
uintptr_t RightHand;


uintptr_t LeftCalf;
uintptr_t LeftFoot;


uintptr_t RightCalf;
uintptr_t RightFoot;

// Inicializar offsets seg�n el juego
void initOffsets(int game)
{
    if (game == 0) {
        // FF Normal
        GamePackage = "com.dts.freefireth";

        Il2Cpp = 0x0;
        Libunity = 0x0;
        InitBase = 0xA115650;
        StaticClass = 0x5C;

        WukongOrion = 0xB50;
        WallHackLocation = 0x152570E;
        NightLocation = 0x2F89FF;

        CurrentMatch = 0x50;
        MatchStatus = 0x8c;

        LocalPlayer = 0x94;
        DictionaryEntities = 0x68;

        Player_IsDead = 0x50;
        Player_Name = 0x2e4;
        Player_Data = 0x48;
        RageCollider = 0x4a8;

        Player_ShadowBase = 0x16bc;
        XPose = 0x78;

        AvatarManager = 0x4c4;
        Avatar = 0xa0;
        Avatar_IsVisible = 0x95;
        Avatar_Data = 0x14;
        Avatar_Data_IsTeam = 0x59;

        FollowCamera = 0x454;
        Camera = 0x18;
        AimRotation = 0x404;
        MainCameraTransform = 0x254;

        Weapon = 0x3f8;
        WeaponData = 0x58;
        WeaponRecoil = 0xC;

        ViewMatrix = 0xe8;
        LocalPlayerAttributes = 0x4c0;
        isFiringOffset = 0x544;
        BaseProfileInfo = 0x16d0;
        HealdShieldEP = 0x10;
        NoReloadOffset = 0x91;
        FixedDeltaTime = 0x24;
        GameTimer = 0x10;
        AimSilent1 = 0x544;
        AimSilent2 = 0x948;
        AimSilent3 = 0x38;
        AimSilent4 = 0x2c;
        // Offsets de Huesos (�ltima actualizaci�n)
        Head = 0x45C;            // Cabeza

        // === Torso / Core ===
        Spine = 0x464;           // Columna (Usando valor de Neck/Cuello)
        Root = 0x470;            // Ra�z
        Hip = 0x46C;             // Cadera

        // === Brazo Izquierdo ===
        LeftSholder = 0x494;     // Hombro Izquierdo
        LeftElbow = 0x4A0;       // Codo Izquierdo
        LeftWrist = 0x498;       // Mu�eca Izquierda
        LeftHand = 0x498;        // Mano Izquierda

        // === Brazo Derecho ===
        RightSholder = 0x490;    // Hombro Derecho
        RightElbow = 0x4A4;      // Codo Derecho
        RightWrist = 0x488;      // Mu�eca Derecha
        RightHand = 0x49C;       // Mano Derecha

        // === Pierna Izquierda ===
        LeftCalf = 0x47C;        // Pantorrilla Izquierda
        LeftFoot = 0x484;        // Pie Izquierdo

        // === Pierna Derecha ===
        RightCalf = 0x478;       // Pantorrilla Derecha
        RightFoot = 0x480;       // Pie Derecho
    }

}
