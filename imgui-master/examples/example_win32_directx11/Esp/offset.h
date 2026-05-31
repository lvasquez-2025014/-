///////////
#pragma once
#include <cstdint>
#include <string>
// Offsets Principales

inline std::string GamePackage;
inline uintptr_t Il2Cpp;
inline uintptr_t Libunity;
inline uintptr_t WallHackLocation;
inline uintptr_t NightLocation;
inline uintptr_t InitBase;
inline uintptr_t StaticClass;
inline uintptr_t DictionaryEntities;
inline uintptr_t WukongOrion;
inline uintptr_t ViewMatrix;


inline uintptr_t MatchStatus;
inline uintptr_t CurrentMatch;


inline uintptr_t LocalPlayer;
inline uintptr_t Player_IsDead;
inline uintptr_t Player_Name;
inline uintptr_t Player_Data;
inline uintptr_t Player_ShadowBase;
inline uintptr_t AvatarManager;
inline uintptr_t Avatar;
inline uintptr_t Avatar_IsVisible;
inline uintptr_t Avatar_Data;
inline uintptr_t Avatar_Data_IsTeam;
inline uintptr_t HealdShieldEP;
inline uintptr_t XPose;


inline uintptr_t FollowCamera;
inline uintptr_t Camera;
inline uintptr_t AimRotation;
inline uintptr_t MainCameraTransform;


inline uintptr_t Weapon;
inline uintptr_t WeaponData;
inline uintptr_t WeaponRecoil;

inline uintptr_t LocalPlayerAttributes;
inline uintptr_t NoReloadOffset;
inline uintptr_t MedikitOffset;
inline uintptr_t Speed;
inline uintptr_t RageCollider;
inline uintptr_t isFiringOffset;
inline uintptr_t BaseProfileInfo;
inline uintptr_t PlayerIDOffset;
inline uintptr_t AmmoOffs;
inline uintptr_t FasFireOffs;
inline uintptr_t OffsQuickSwitch;
inline uintptr_t AimSilent1;
inline uintptr_t AimSilent2;
inline uintptr_t AimSilent3;
inline uintptr_t AimSilent4;

// New offset variables
inline uintptr_t CurrentObserver;
inline uintptr_t ObserverPlayer;
inline uintptr_t WeaponInfo;
inline uintptr_t WeaponID;
inline uintptr_t Player_Level;
inline uintptr_t WeaponOnHand;
inline uintptr_t NoReload;
inline uintptr_t bullet_hit;
inline uintptr_t IS_FIRING;
inline uintptr_t sAim1;
inline uintptr_t sAim2;
inline uintptr_t sAim3;
inline uintptr_t sAim4;
inline uintptr_t GameTimer;
inline uintptr_t FixedDeltaTime;
inline uintptr_t LeftShoulder;
inline uintptr_t LeftWristJoint;
inline uintptr_t RightShoulder;
inline uintptr_t RightWristJoint;

inline uintptr_t Head;

inline uintptr_t Spine;
inline uintptr_t Root;
inline uintptr_t Hip;

inline uintptr_t LeftSholder;
inline uintptr_t LeftElbow;
inline uintptr_t LeftWrist;
inline uintptr_t LeftHand;


inline uintptr_t RightSholder;
inline uintptr_t RightElbow;
inline uintptr_t RightWrist;
inline uintptr_t RightHand;


inline uintptr_t LeftCalf;
inline uintptr_t LeftFoot;


inline uintptr_t RightCalf;
inline uintptr_t RightFoot;

// Inicializar offsets segun el juego
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
        //speed internal
        GameTimer = 0x10;
        FixedDeltaTime = 0x24;
        //

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
