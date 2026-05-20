#pragma once
#include <windows.h>
#include <tlhelp32.h>
#include "vector.h"
#include "lib.h"
#include "C:\Users\Asmodeus\Downloads\protecto parte 5\protecto parte 5\imgui-master\MinHook\include\MinHook.h"
#include "offset.h"
#include "notificaciones.h"
DWORD GetProcZ(const char* processName)
{
    DWORD processID = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32W pe32;
        pe32.dwSize = sizeof(PROCESSENTRY32W);
        if (Process32FirstW(hSnapshot, &pe32))
        {
            wchar_t wProcessName[MAX_PATH];
            MultiByteToWideChar(CP_ACP, 0, processName, -1, wProcessName, MAX_PATH);

            do
            {
                if (wcscmp(pe32.szExeFile, wProcessName) == 0)
                {
                    processID = pe32.th32ProcessID;
                    break;
                }
            } while (Process32NextW(hSnapshot, &pe32));
        }
        CloseHandle(hSnapshot);
    }
    return processID;
}

bool CheckProcessInstancesZ(const char* processName, int& count) {
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32)) {
        wchar_t wProcessName[MAX_PATH];
        MultiByteToWideChar(CP_ACP, 0, processName, -1, wProcessName, MAX_PATH);

        do {
            if (wcscmp(pe32.szExeFile, wProcessName) == 0) {
                count++;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
    return true;
}

bool KillProcZ(DWORD processID)
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
    if (hProcess == NULL)
    {
        return false;
    }
    bool result = TerminateProcess(hProcess, 0);
    CloseHandle(hProcess);
    return result;
}

bool IsProcRunZ(const char* processName, DWORD& processID)
{
    bool isRunning = false;
    processID = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return false;
    }
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &pe32)) {
        wchar_t wProcessName[MAX_PATH];
        MultiByteToWideChar(CP_ACP, 0, processName, -1, wProcessName, MAX_PATH);

        do {
            if (wcscmp(pe32.szExeFile, wProcessName) == 0) {
                isRunning = true;
                processID = pe32.th32ProcessID;
                break;
            }
        } while (Process32NextW(hSnapshot, &pe32));
    }
    CloseHandle(hSnapshot);

    return isRunning;
}

bool ForgeKillProcZ(DWORD processID)
{
    HWND hwnd = NULL;
    DWORD dwPID = 0;
    do {
        hwnd = FindWindowEx(NULL, hwnd, NULL, NULL);
        GetWindowThreadProcessId(hwnd, &dwPID);
    } while (dwPID != processID && hwnd != NULL);

    if (hwnd != NULL)
    {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        return true;
    }
    return false;
}

inline static auto lastCleanTime = std::chrono::steady_clock::now();
inline std::mutex cacheMutex;  // Protección para acceso concurrente al caché
inline std::atomic<bool> runningxd2(true);

// Estructura de caché optimizada
struct zCacheT {
    uintptr_t addr;
    std::chrono::steady_clock::time_point zTimeTemp;
};

inline std::unordered_map<uintptr_t, zCacheT> CacheZ;
inline std::shared_mutex zCacheSharMtx;
inline std::unordered_map<uint32_t, Vector3> enemiesSunk;
inline std::unordered_set<uint32_t> enemiesAlreadySunk;
inline const auto zTimeMs = std::chrono::milliseconds(1500);

enum class AimKey {
    RightMouseButton,
    LeftMouseButton,
    MiddleMouseButton,
    ShiftKey,
    ControlKey,
    AltKey
};

inline AimKey selectedKey = AimKey::LeftMouseButton;
inline MatchStatusEnum currentMatchStatus = MATCH_NOT_STARTED;

struct BonePositions
{
    // Parte superior
    Vector3 Cabeza, Columna, Raiz, Cadera;

    // Brazos
    Vector3 HombroIzquierdo, HombroDerecho;
    Vector3 CodoIzquierdo, CodoDerecho;
    Vector3 MuñecaIzquierda, MuñecaDerecha;
    Vector3 ManoIzquierda, ManoDerecha;

    // Piernas
    Vector3 PantorrillaIzquierda, PantorrillaDerecha;
    Vector3 PieIzquierdo, PieDerecho;
};

inline BonePositions bonePositions;

// Variables globales optimizadas
inline void* vmPtr = nullptr;
inline void* pVMAddr = nullptr;
inline void* cpuAddr = nullptr;
inline int zCacheSize = 5000;  // Reducido para gama baja

// Tipos de funciones optimizadas
typedef int(__cdecl* PGMPhysReadFunc)(void*, uintptr_t, void*, size_t);
typedef int(__cdecl* PGMPhysSimpleWriteGCPhysFunc)(void*, uintptr_t, void*, size_t);
typedef int(__cdecl* PGMPhysGCPtr2GCPhysFunc)(void*, uintptr_t, uintptr_t*);
typedef void* (__cdecl* VMMGetCpuByIdFunc)(void*, int);

// Funciones originales
inline PGMPhysReadFunc ogPhysRead = nullptr;
inline VMMGetCpuByIdFunc ogCPU = nullptr;
inline PGMPhysGCPtr2GCPhysFunc ogCast = nullptr;
inline PGMPhysSimpleWriteGCPhysFunc ogWrite = nullptr;

// Función de escritura optimizada
inline size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* buffer) {
    size_t total = size * nmemb;
    buffer->append((char*)contents, total);
    return total;
}

// Hooks optimizados
int __cdecl HookedPGMPhysRead(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
    if (!vmPtr) vmPtr = pVM;
    return ogPhysRead(pVM, GCPhys, pvBuf, cbRead);
}

int zHookWrite(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
    return ogWrite(pVM, GCPhys, pvBuf, cbRead);
}

int zHookRead(void* pVM, uintptr_t GCPhys, void* pvBuf, size_t cbRead) {
    return ogPhysRead(pVM, GCPhys, pvBuf, cbRead);
}

void* CPU(void* pVM, int cpuId) {
    return ogCPU(pVM, cpuId);
}

int Cast(void* pVCpu, uintptr_t address, uintptr_t* physAddress) {
    return ogCast(pVCpu, address, physAddress);
}

void InitializeZ(void* pVM)
{
    pVMAddr = pVM;
    cpuAddr = CPU(pVM, 0);
}

//Reset ESP
bool ConvertZ(uintptr_t address, uintptr_t& phys) {
    auto now = std::chrono::steady_clock::now();
    auto it = CacheZ.find(address);

    // Si está en caché, usar el valor viejo primero
    if (it != CacheZ.end()) {
        phys = it->second.addr;
    }

    // Intentar actualizarlo sin eliminar el valor previo (soportando emuladores con hasta 8 núcleos)
    for (int i = 0; i < 8; ++i) {
        void* cpu = ogCPU(pVMAddr, i);
        if (!cpu) continue;

        uintptr_t newPhys;
        if (ogCast(cpu, address, &newPhys) == 0) {
            // Actualizar la caché con nueva info
            CacheZ[address] = { newPhys, now };
            phys = newPhys; // usar el actualizado
            return true;
        }
    }

    // Si no se pudo actualizar pero existía antes, devolvemos el viejo
    return (it != CacheZ.end());
}

// Lectura optimizada
template<typename T>
bool ReadZ(uintptr_t address, T& data) {
    uintptr_t physAddress;
    return ConvertZ(address, physAddress) && (zHookRead(pVMAddr, physAddress, &data, sizeof(T)) == 0);
}

// Lectura de punteros dinámicos (32-bit y 64-bit)
inline bool ReadPointer(uintptr_t address, uintptr_t& outPtr) {
    if (is64Bit) {
        uint64_t temp = 0;
        if (ReadZ(address, temp)) {
            outPtr = (uintptr_t)temp;
            return true;
        }
        return false;
    } else {
        uint32_t temp = 0;
        if (ReadZ(address, temp)) {
            outPtr = (uintptr_t)temp;
            return true;
        }
        return false;
    }
}

// Lectura de arrays optimizada
template<typename T>
bool ReadArrayZ(uintptr_t address, std::vector<T>& array) {
    uintptr_t physAddress;
    if (!ConvertZ(address, physAddress)) return false;

    return zHookRead(pVMAddr, physAddress, array.data(), sizeof(T) * array.size()) == 0;
}

template<typename T>
bool ReadArrayZ2(uintptr_t address, std::vector<T>& array)
{
    uintptr_t convertedAddress;
    bool result = ConvertZ(address, convertedAddress);

    if (!result)
        return false;

    size_t size = sizeof(T) * array.size();
    DWORD status = zHookRead(pVMAddr, convertedAddress, array.data(), size);

    return status == 0;
}

// Conversión de wstring a string optimizada
std::string WideStringToString(const std::wstring& wideString) {
    if (wideString.empty()) {
        return std::string();
    }

    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), (int)wideString.length(), NULL, 0, NULL, NULL);

    std::string result(sizeNeeded, 0);


    WideCharToMultiByte(CP_UTF8, 0, wideString.c_str(), (int)wideString.length(), &result[0], sizeNeeded, NULL, NULL);

    return result;
}

// Lectura de strings optimizada
std::string ReadStringZ2(uintptr_t address, int size, bool unicode = true)
{
    std::vector<uint8_t> stringBytes(size);

    bool read = ReadArrayZ2(address, stringBytes);

    if (!read) return "";

    std::string readString;
    if (unicode) {

        std::wstring wideString(reinterpret_cast<wchar_t*>(stringBytes.data()), size / 2);
        readString = WideStringToString(wideString);
    }
    else {

        readString = std::string(stringBytes.begin(), stringBytes.end());
    }

    auto nullTerminator = readString.find('\0');
    if (nullTerminator != std::string::npos)
        readString = readString.substr(0, nullTerminator);

    return readString;
}

// Escritura optimizada
template<typename T>
void WriteZ(uintptr_t address, const T& value) {
    uintptr_t physAddress;
    if (ConvertZ(address, physAddress)) {
        zHookWrite(pVMAddr, physAddress, (void*)&value, sizeof(T));
    }
}

// Estructuras de datos optimizadas
struct TMatrix {
    Vector4 position;
    Quaternion rotation;
    Vector4 scale;
};

struct ShieldData {
    float escudoPorcentaje;
    std::string escudoStr;
};

struct HealthData {
    float vidaPorcentaje;
    std::string vidaStr;
};

struct BoxData {
    ImVec2 topLeft;
    ImVec2 bottomRight;
};
// Función GetPosition optimizada
// Función GetPosition optimizada para 32-bit y 64-bit
bool GetPosition(uintptr_t transform, Vector3& pos)
{
    pos = Vector3::Zero();
    uintptr_t transformObjValue = 0;
    
    // Internal transform pointer offset: 32-bit = 0x8, 64-bit = 0x10
    uintptr_t internalPtrOffset = is64Bit ? 0x10 : 0x8;
    if (is64Bit) {
        uint64_t temp = 0;
        if (!ReadZ(transform + internalPtrOffset, temp)) return false;
        transformObjValue = (uintptr_t)temp;
    } else {
        uint32_t temp = 0;
        if (!ReadZ(transform + internalPtrOffset, temp)) return false;
        transformObjValue = (uintptr_t)temp;
    }
    if (transformObjValue == 0) return false;

    // indexValue (int32_t): 32-bit = 0x24, 64-bit = 0x38
    uint32_t indexValue = 0;
    uintptr_t indexOffset = is64Bit ? 0x38 : 0x24;
    if (!ReadZ(transformObjValue + indexOffset, indexValue)) return false;

    // matrixValue: 32-bit = 0x20, 64-bit = 0x30
    uintptr_t matrixValue = 0;
    uintptr_t matrixOffset = is64Bit ? 0x30 : 0x20;
    if (is64Bit) {
        uint64_t temp = 0;
        if (!ReadZ(transformObjValue + matrixOffset, temp)) return false;
        matrixValue = (uintptr_t)temp;
    } else {
        uint32_t temp = 0;
        if (!ReadZ(transformObjValue + matrixOffset, temp)) return false;
        matrixValue = (uintptr_t)temp;
    }
    if (matrixValue == 0) return false;

    // matrixListValue: 32-bit = 0x18, 64-bit = 0x18
    uintptr_t matrixListValue = 0;
    uintptr_t matrixListOffset = 0x18;
    if (is64Bit) {
        uint64_t temp = 0;
        if (!ReadZ(matrixValue + matrixListOffset, temp)) return false;
        matrixListValue = (uintptr_t)temp;
    } else {
        uint32_t temp = 0;
        if (!ReadZ(matrixValue + matrixListOffset, temp)) return false;
        matrixListValue = (uintptr_t)temp;
    }
    if (matrixListValue == 0) return false;

    // matrixIndicesValue: 32-bit = 0x1C, 64-bit = 0x20
    uintptr_t matrixIndicesValue = 0;
    uintptr_t matrixIndicesOffset = is64Bit ? 0x20 : 0x1C;
    if (is64Bit) {
        uint64_t temp = 0;
        if (!ReadZ(matrixValue + matrixIndicesOffset, temp)) return false;
        matrixIndicesValue = (uintptr_t)temp;
    } else {
        uint32_t temp = 0;
        if (!ReadZ(matrixValue + matrixIndicesOffset, temp)) return false;
        matrixIndicesValue = (uintptr_t)temp;
    }
    if (matrixIndicesValue == 0) return false;

    Vector3 resultValue;
    if (!ReadZ(indexValue * 0x30 + matrixListValue, resultValue)) return false;

    int maxTries = 50;
    int tries = 0;
    int transformIndexValue = -1;
    
    // Read first index from indices list
    if (!ReadZ((uintptr_t)(matrixIndicesValue + (indexValue * 0x4)), transformIndexValue)) return false;

    while (transformIndexValue >= 0)
    {
        tries++;
        if (tries == maxTries) break;
        TMatrix tMatrixValue;
        if (!ReadZ((uintptr_t)(matrixListValue + (0x30 * transformIndexValue)), tMatrixValue)) return false;

        float rotX = tMatrixValue.rotation.x;
        float rotY = tMatrixValue.rotation.y;
        float rotZ = tMatrixValue.rotation.z;
        float rotW = tMatrixValue.rotation.w;
        float scaleX = resultValue.x * tMatrixValue.scale.x;
        float scaleY = resultValue.y * tMatrixValue.scale.y;
        float scaleZ = resultValue.z * tMatrixValue.scale.z;

        resultValue.x = (float)(tMatrixValue.position.x + scaleX +
            (scaleX * ((rotY * rotY * -2.0) - (rotZ * rotZ * 2.0))) +
            (scaleY * ((rotW * rotZ * -2.0) - (rotY * rotX * -2.0))) +
            (scaleZ * ((rotZ * rotX * 2.0) - (rotW * rotY * -2.0))));
        resultValue.y = (float)(tMatrixValue.position.y + scaleY +
            (scaleX * ((rotX * rotY * 2.0) - (rotW * rotZ * -2.0))) +
            (scaleY * ((rotZ * rotZ * -2.0) - (rotX * rotX * 2.0))) +
            (scaleZ * ((rotW * rotX * -2.0) - (rotZ * rotY * -2.0))));
        resultValue.z = (float)(tMatrixValue.position.z + scaleZ +
            (scaleX * ((rotW * rotY * -2.0) - (rotX * rotZ * -2.0))) +
            (scaleY * ((rotY * rotZ * 2.0) - (rotW * rotX * -2.0))) +
            (scaleZ * ((rotX * rotX * -2.0) - (rotY * rotY * 2.0))));

        if (!ReadZ((uintptr_t)(matrixIndicesValue + (transformIndexValue * 0x4)), transformIndexValue)) return false;
    }
    pos = resultValue;
    return tries != maxTries;
}

bool GetNodePosition(uintptr_t nodeTransform, Vector3& result) {
    uintptr_t transformValue = 0;
    if (is64Bit) {
        uint64_t temp = 0;
        if (!ReadZ(nodeTransform + 0x8, temp)) return false;
        transformValue = (uintptr_t)temp;
    } else {
        uint32_t temp = 0;
        if (!ReadZ(nodeTransform + 0x8, temp)) return false;
        transformValue = (uintptr_t)temp;
    }
    return GetPosition(transformValue, result);
}

// Función para encontrar ventana HD-Player optimizada
BOOL CALLBACK GetPosEmlBS(HWND hWnd, LPARAM lParam) {
    char title[256];
    GetWindowTextA(hWnd, title, sizeof(title));
    std::string windowName(title);

    if (windowName == "HD-Player" || windowName == "_ctl.Window") {
        *reinterpret_cast<HWND*>(lParam) = hWnd;
        return FALSE;
    }
    return TRUE;
}

HWND FindHDPlayerWindow(HWND parent) {
    HWND renderWindow = nullptr;
    EnumChildWindows(parent, GetPosEmlBS, reinterpret_cast<LPARAM>(&renderWindow));
    return renderWindow;
}

inline std::string ErrorLoadAdb = "NUll";

std::string GetExeDirectoryZ() {
    extern HINSTANCE g_hinstDLL;
    char path[MAX_PATH];
    GetModuleFileNameA(g_hinstDLL, path, MAX_PATH);

    std::string fullPath(path);
    size_t lastSlashIndex = fullPath.find_last_of("\\/");
    if (lastSlashIndex != std::string::npos) {
        return fullPath.substr(0, lastSlashIndex);
    }
    return "";
}

std::string GetExeDirectoryPIDZ(DWORD processID) {
    char path[MAX_PATH];
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);

    if (hProcess) {
        if (GetModuleFileNameExA(hProcess, NULL, path, MAX_PATH)) {
            std::string fullPath(path);
            size_t lastSlashIndex = fullPath.find_last_of("\\/");
            CloseHandle(hProcess);
            if (lastSlashIndex != std::string::npos) {
                return fullPath.substr(0, lastSlashIndex);
            }
        }
        CloseHandle(hProcess);
    }

    return "";
}

bool CambiarDirectoryZ(const std::string& path) {
    return SetCurrentDirectoryA(path.c_str());
}

void KillAdbZ() {
    DWORD ProcIdAdb = GetProcZ("adb.exe");
    DWORD ProcIdAdb2 = 0;
    KillProcZ(ProcIdAdb);
    if (IsProcRunZ("adb.exe", ProcIdAdb2)) {
        ForgeKillProcZ(ProcIdAdb2);
    }
    DWORD ProcIdAdbHD = GetProcZ("HD-Adb.exe");
    DWORD ProcIdAdbHD2 = 0;
    KillProcZ(ProcIdAdbHD);
    if (IsProcRunZ("HD-Adb.exe", ProcIdAdbHD2)) {
        ForgeKillProcZ(ProcIdAdbHD2);
    }

    // Esperar reducida para no causar delays innecesarios
    Sleep(50);
}

std::string ExtraerLibAddressZ(const std::string& input) {
    std::istringstream iss(input);
    std::string line;
    while (std::getline(iss, line)) {
        std::size_t pos = line.find('-');
        if (pos != std::string::npos && pos >= 8) {
            bool isHex = true;
            for (std::size_t i = 0; i < pos; i++) {
                if (!isxdigit(static_cast<unsigned char>(line[i]))) {
                    isHex = false;
                    break;
                }
            }
            if (isHex) {
                return line.substr(0, pos);
            }
        }
    }
    return "";
}


inline uintptr_t uIntExtrZ(std::string c, int base = 16) {
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
        || sizeof(uintptr_t) == sizeof(unsigned long long),
        "Please add string to handle conversion for this architecture.");
    if (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return strtoul(c.c_str(), nullptr, base);
    }
    return strtoull(c.c_str(), nullptr, base);
}

// Función duplicada para libunity.so
inline uintptr_t uIntExtrZ2(std::string c, int base = 16) {
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long)
        || sizeof(uintptr_t) == sizeof(unsigned long long),
        "Please add string to handle conversion for this architecture.");
    if (sizeof(uintptr_t) == sizeof(unsigned long)) {
        return strtoul(c.c_str(), nullptr, base);
    }
    return strtoull(c.c_str(), nullptr, base);
}

inline static std::string GetAdbPathZ()
{
    // 1) Intentar local en el directorio del ejecutable o DLL
    std::string localPath = ".\\HD-Adb.exe";
    DWORD attrib = GetFileAttributesA(localPath.c_str());
    if (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        return localPath;
    }

    // 2) Intentar la ruta oficial de instalación de BlueStacks 5 (donde reside HD-Adb.exe)
    std::string officialPath = "C:\\Program Files\\BlueStacks_nxt\\HD-Adb.exe";
    attrib = GetFileAttributesA(officialPath.c_str());
    if (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        return "\"" + officialPath + "\"";
    }

    // 3) Fallback por defecto
    return ".\\HD-Adb";
}

inline static std::string AdbRunCapture_NoWindow(const std::string& args)
{
    std::string adbPath = GetAdbPathZ();
    std::string cmdLine = adbPath + " " + args;

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    HANDLE hStdOutRead = NULL, hStdOutWrite = NULL;
    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) return "";
    SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0);

    HANDLE hStdInRead = NULL, hStdInWrite = NULL;
    if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) {
        CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
        return "";
    }
    SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOA si{};
    si.cb = sizeof(si);
    si.hStdOutput = hStdOutWrite;
    si.hStdError = hStdOutWrite;
    si.hStdInput = hStdInRead;
    si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi{};
    if (!CreateProcessA(
        NULL,
        const_cast<LPSTR>(cmdLine.c_str()),
        NULL, NULL,
        TRUE,                 // heredar handles
        CREATE_NO_WINDOW,     // sin consola
        NULL, NULL,
        &si, &pi))
    {
        CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
        CloseHandle(hStdInRead);  CloseHandle(hStdInWrite);
        return "";
    }

    // Cierro extremos que no uso
    CloseHandle(hStdInRead);
    CloseHandle(hStdInWrite);
    CloseHandle(hStdOutWrite);

    // Leo salida
    std::string output;
    CHAR buffer[4096];
    DWORD read = 0;
    while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
        buffer[read] = '\0';
        output += buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    CloseHandle(hStdOutRead);

    return output;
}

inline static std::vector<std::string> ParseAdbDevices_List(const std::string& out)
{
    std::vector<std::string> devices;
    std::istringstream iss(out);
    std::string line;
    while (std::getline(iss, line)) {
        if (line.find("List of devices") != std::string::npos) continue;
        // Formato: "<deviceId>\tdevice"
        auto tab = line.find('\t');
        if (tab != std::string::npos) {
            auto id = line.substr(0, tab);
            if (!id.empty() && line.find("device") != std::string::npos)
                devices.push_back(id);
        }
    }
    return devices;
}

inline static std::string GetFirstAdbDeviceId()
{
    // 1) Intento directo
    {
        const auto out = AdbRunCapture_NoWindow("devices");
        auto list = ParseAdbDevices_List(out);
        if (!list.empty())
            return list[0];
    }

    // 2) Conectar a puertos comunes sin mostrar CMD
    static const int ports[] = { 5556, 5555, 5557, 5558, 5559, 5510 };
    for (int port : ports) {
        AdbRunCapture_NoWindow(std::string("connect 127.0.0.1:") + std::to_string(port));
        Sleep(50);  // Restaurado al valor original

        // Verificar si la conexión fue exitosa
        const auto checkOut = AdbRunCapture_NoWindow("devices");
        auto checkList = ParseAdbDevices_List(checkOut);
        if (!checkList.empty()) {
            break;  // Conexión exitosa, no intentar más puertos
        }
    }

    // 3) Reintento
    {
        const auto out2 = AdbRunCapture_NoWindow("devices");
        auto list2 = ParseAdbDevices_List(out2);
        return list2.empty() ? std::string() : list2[0];
    }
}

std::string ShellGetAddressZ(const std::string& firstCommand, const std::string& secondCommand) {
    HANDLE hStdOutRead, hStdOutWrite;
    HANDLE hStdInRead, hStdInWrite;

    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) return "";
    if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) return "";
    if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) return "";
    if (!SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0)) return "";

    STARTUPINFOA si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = hStdOutWrite;
    si.hStdOutput = hStdOutWrite;
    si.hStdInput = hStdInRead;
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    std::string deviceId = GetFirstAdbDeviceId();
    std::string fullCommand = deviceId.empty()
        ? ".\\HD-Adb shell"
        : ".\\HD-Adb -s " + deviceId + " shell";

    if (!CreateProcessA(NULL, (LPSTR)fullCommand.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        return "";
    }

    CloseHandle(hStdOutWrite);
    CloseHandle(hStdInRead);

    DWORD written;
    std::string commands = firstCommand + "\n" + secondCommand + "\n";
    if (!WriteFile(hStdInWrite, commands.c_str(), commands.length(), &written, NULL)) return "";
    CloseHandle(hStdInWrite);

    CHAR buffer[128];
    DWORD read;
    std::string output;
    while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
        buffer[read] = '\0';
        output += buffer;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(hStdOutRead);

    return ExtraerLibAddressZ(output);
}

std::string ShellGetAddressNoSuZ(const std::string& comand) {
    std::string deviceId = GetFirstAdbDeviceId();
    std::string adbPath = GetAdbPathZ();

    // Intentaremos varias estrategias de ejecución para maximizar compatibilidad de Root
    std::vector<std::string> shellCommands = {
        // 1. Ruta específica de BlueStacks (bstk/su)
        deviceId.empty() ? adbPath + " shell \"/boot/android/android/system/xbin/bstk/su\"" 
                         : adbPath + " -s " + deviceId + " shell \"/boot/android/android/system/xbin/bstk/su\"",
        // 2. Comando su estándar directo
        deviceId.empty() ? adbPath + " shell \"su\"" 
                         : adbPath + " -s " + deviceId + " shell \"su\"",
        // 3. Ejecución directa sin su (en emuladores pre-rooteados/desarrollo)
        deviceId.empty() ? adbPath + " shell" 
                         : adbPath + " -s " + deviceId + " shell"
    };

    for (const auto& fullCommand : shellCommands) {
        HANDLE hStdOutRead, hStdOutWrite;
        HANDLE hStdInRead, hStdInWrite;
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = NULL;

        if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) continue;
        if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) {
            CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
            continue;
        }
        if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) {
            CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
            continue;
        }
        if (!SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0)) {
            CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
            CloseHandle(hStdInRead); CloseHandle(hStdInWrite);
            continue;
        }

        STARTUPINFOA si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.hStdError = hStdOutWrite;
        si.hStdOutput = hStdOutWrite;
        si.hStdInput = hStdInRead;
        si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (CreateProcessA(NULL, (LPSTR)fullCommand.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            CloseHandle(hStdOutWrite);
            CloseHandle(hStdInRead);

            DWORD written;
            std::string commands = comand + "\n";
            WriteFile(hStdInWrite, commands.c_str(), commands.length(), &written, NULL);
            CloseHandle(hStdInWrite);

            CHAR buffer[128];
            DWORD read;
            std::string output;
            while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
                buffer[read] = '\0';
                output += buffer;
            }

            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            CloseHandle(hStdOutRead);

            std::string parsedAddr = ExtraerLibAddressZ(output);
            if (!parsedAddr.empty()) {
                return parsedAddr; // Dirección encontrada con éxito
            }
        } else {
            CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
            CloseHandle(hStdInRead); CloseHandle(hStdInWrite);
        }
    }
    return "";
}

// Función duplicada optimizada para reutilizar la implementación principal de forma limpia
std::string ShellGetAddressNoSuZ2(const std::string& comand) {
    return ShellGetAddressNoSuZ(comand);
}

inline static std::string ShellRunCommandZ(const std::string& comand) {
    std::string deviceId = GetFirstAdbDeviceId();
    std::string adbPath = GetAdbPathZ();

    std::vector<std::string> shellCommands = {
        deviceId.empty() ? adbPath + " shell \"/boot/android/android/system/xbin/bstk/su\"" 
                         : adbPath + " -s " + deviceId + " shell \"/boot/android/android/system/xbin/bstk/su\"",
        deviceId.empty() ? adbPath + " shell \"su\"" 
                         : adbPath + " -s " + deviceId + " shell \"su\"",
        deviceId.empty() ? adbPath + " shell" 
                         : adbPath + " -s " + deviceId + " shell"
    };

    for (const auto& fullCommand : shellCommands) {
        HANDLE hStdOutRead, hStdOutWrite;
        HANDLE hStdInRead, hStdInWrite;
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = TRUE;
        sa.lpSecurityDescriptor = NULL;

        if (!CreatePipe(&hStdOutRead, &hStdOutWrite, &sa, 0)) continue;
        if (!SetHandleInformation(hStdOutRead, HANDLE_FLAG_INHERIT, 0)) {
            CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
            continue;
        }
        if (!CreatePipe(&hStdInRead, &hStdInWrite, &sa, 0)) {
            CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
            continue;
        }
        if (!SetHandleInformation(hStdInWrite, HANDLE_FLAG_INHERIT, 0)) {
            CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
            CloseHandle(hStdInRead); CloseHandle(hStdInWrite);
            continue;
        }

        STARTUPINFOA si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        si.hStdError = hStdOutWrite;
        si.hStdOutput = hStdOutWrite;
        si.hStdInput = hStdInRead;
        si.dwFlags |= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
        si.wShowWindow = SW_HIDE;

        PROCESS_INFORMATION pi;
        ZeroMemory(&pi, sizeof(pi));

        if (CreateProcessA(NULL, (LPSTR)fullCommand.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
            CloseHandle(hStdOutWrite);
            CloseHandle(hStdInRead);

            DWORD written;
            std::string commands = comand + "\n";
            WriteFile(hStdInWrite, commands.c_str(), commands.length(), &written, NULL);
            CloseHandle(hStdInWrite);

            CHAR buffer[128];
            DWORD read;
            std::string output;
            while (ReadFile(hStdOutRead, buffer, sizeof(buffer) - 1, &read, NULL) && read > 0) {
                buffer[read] = '\0';
                output += buffer;
            }

            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            CloseHandle(hStdOutRead);

            if (!output.empty()) {
                return output;
            }
        } else {
            CloseHandle(hStdOutRead); CloseHandle(hStdOutWrite);
            CloseHandle(hStdInRead); CloseHandle(hStdInWrite);
        }
    }
    return "";
}

void ComdADBZ(const std::string& command) {
    std::string deviceId = GetFirstAdbDeviceId();
    std::string adbPath = GetAdbPathZ();
    std::string fullCommand = deviceId.empty()
        ? adbPath + " shell " + command
        : adbPath + " -s " + deviceId + " shell " + command;

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.wShowWindow = SW_HIDE;
    ZeroMemory(&pi, sizeof(pi));
    if (!CreateProcessA(NULL, const_cast<LPSTR>(fullCommand.c_str()), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        std::cerr << "Failed to start process. Error: " << GetLastError() << std::endl;
        return;
    }
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

/////////////////////////////////////////////


void KillProcessByFenix(const wchar_t* processNameW) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return;

    PROCESSENTRY32W pe32{};
    pe32.dwSize = sizeof(pe32);

    if (Process32FirstW(hSnap, &pe32)) {
        do {
            if (_wcsicmp(pe32.szExeFile, processNameW) == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pe32.th32ProcessID);
                if (hProcess) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);
                    wprintf(L"Proceso %s cerrado exitosamente.\n", processNameW);
                }
            }
        } while (Process32NextW(hSnap, &pe32));
    }
    CloseHandle(hSnap);
}




inline void BuscarInitBaseManual(uintptr_t startOffset, uintptr_t endOffset);

// Thread para inicializar ADB en background sin afectar FPS
inline static std::atomic<bool> adbThreadRunning = false;
inline static std::atomic<bool> adbInitialized = false;

/////////////////////////////////////////////////////

inline void CleanEmulatorHosts() {
    Notifications::Add("ZH X TEAM", "Limpiando bloqueos Anti-Ban (Hosts/IP)...", 4.0f);
    
    // 1. Remount system partition as RW
    ComdADBZ("su -c \"mount -o remount,rw /system\"");
    Sleep(100);
    
    // 2. Overwrite /system/etc/hosts with default localhost only
    ComdADBZ("su -c \"echo '127.0.0.1       localhost' > /system/etc/hosts\"");
    Sleep(50);
    ComdADBZ("su -c \"echo '::1             ip6-localhost' >> /system/etc/hosts\"");
    Sleep(50);
    
    // 3. Restore permissions
    ComdADBZ("su -c \"chmod 644 /system/etc/hosts\"");
    Sleep(100);
    
    // 4. Limpiar reglas de firewall (iptables) que bloquean la subida de datos (telemetria)
    ComdADBZ("su -c \"iptables -P INPUT ACCEPT\"");
    ComdADBZ("su -c \"iptables -P OUTPUT ACCEPT\"");
    ComdADBZ("su -c \"iptables -P FORWARD ACCEPT\"");
    ComdADBZ("su -c \"iptables -F\"");
    ComdADBZ("su -c \"iptables -X\"");
    Sleep(50);
    
    Notifications::Add("ZH X TEAM", "Red limpia. Telemetria restaurada.", 4.0f);
}

inline void ApplySuperSecurity() {
    Notifications::Add("ZH X TEAM", "Aplicando Super Seguridad...", 4.0f);
    
    // 1. Limpiar Logcat para que el anti-cheat no lea logs del sistema ni inyecciones
    ComdADBZ("su -c \"logcat -c\"");
    
    // 2. Eliminar directorios de Crash Report y Telemetría del juego actual
    std::string cleanCmd1 = "su -c \"rm -rf /sdcard/Android/data/" + GamePackage + "/files/report/*\"";
    ComdADBZ(cleanCmd1.c_str());
    
    std::string cleanCmd2 = "su -c \"rm -rf /sdcard/Android/data/" + GamePackage + "/files/CrashReport/*\"";
    ComdADBZ(cleanCmd2.c_str());
    
    std::string cleanCmd3 = "su -c \"rm -rf /sdcard/Android/data/" + GamePackage + "/cache/*\"";
    ComdADBZ(cleanCmd3.c_str());
    
    std::string cleanCmd4 = "su -c \"rm -rf /data/data/" + GamePackage + "/cache/*\"";
    ComdADBZ(cleanCmd4.c_str());
    
    // 3. Limpiar carpeta de GCloud (Módulo de telemetría de Tencent/Garena)
    std::string cleanCmd5 = "su -c \"rm -rf /sdcard/Android/data/" + GamePackage + "/files/GCloud/*\"";
    ComdADBZ(cleanCmd5.c_str());

    // 4. Limpiar rastros de memoria en /data/tombstones (Crash dumps nativos de Android)
    ComdADBZ("su -c \"rm -rf /data/tombstones/*\"");
    
    Sleep(200);
    Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Seguridad Activa. Rastros limpios.", 4.0f);
}

// Helper to robustly get PID of target process on Android emulator
inline static std::string AdbGetPidZ(const std::string& packageName) {
    // 1) Intentar con pidof normal
    std::string out = ShellRunCommandZ("pidof " + packageName);
    if (!out.empty()) {
        // Remover saltos de línea y limpiar espacios
        out.erase(std::remove_if(out.begin(), out.end(), [](unsigned char c) { return std::isspace(c); }), out.end());
        if (!out.empty() && std::all_of(out.begin(), out.end(), ::isdigit)) {
            return out;
        }
    }

    // 2) Fallback: Consultar ps -A y extraer el PID
    std::string psOut = ShellRunCommandZ("ps -A | grep " + packageName);
    if (psOut.empty()) {
        psOut = ShellRunCommandZ("ps | grep " + packageName);
    }
    
    if (!psOut.empty()) {
        std::istringstream iss(psOut);
        std::string line;
        while (std::getline(iss, line)) {
            if (line.find(packageName) != std::string::npos) {
                std::istringstream lineStream(line);
                std::vector<std::string> tokens;
                std::string tok;
                while (lineStream >> tok) {
                    tokens.push_back(tok);
                }
                // En ps -A el formato usual es: USER PID PPID VSZ RSS WCHAN ADDR S NAME
                // Donde el segundo token (índice 1) es el PID
                if (tokens.size() > 1 && std::all_of(tokens[1].begin(), tokens[1].end(), ::isdigit)) {
                    return tokens[1];
                }
            }
        }
    }
    return "";
}

DWORD WINAPI ADBInitThread(LPVOID lpParam) {
    // Proceso ADB en thread separado para no causar tirones
    Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Waiting for secure connection...", 4.0f);
    KillAdbZ();
    Sleep(50);

    ComdADBZ("kill-server");
    Sleep(100);

    ComdADBZ("start-server");
    Sleep(150);

    ComdADBZ("devices");
    Sleep(50);

    // Automáticamente limpiar el hosts del emulador para evitar ban de telemetría
    CleanEmulatorHosts();
    
    // Automáticamente limpiar logs y rastros para Super Seguridad Anti-Ban
    ApplySuperSecurity();

    // Detección automática inteligente del juego activo en ejecución en el emulador
    std::string activePid = AdbGetPidZ(GamePackage);
    if (activePid.empty()) {
        // Intercambiar de forma inteligente si la otra versión está ejecutándose
        std::string alternatePkg = (GamePackage == "com.dts.freefireth") ? "com.dts.freefiremax" : "com.dts.freefireth";
        std::string altPid = AdbGetPidZ(alternatePkg);
        if (!altPid.empty()) {
            GamePackage = alternatePkg;
            activePid = altPid;
            // Actualizar la estructura global de offsets para la nueva versión de juego elegida
            initOffsets(alternatePkg == "com.dts.freefiremax" ? 1 : 0);
            Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Auto-cambio: Detectada versión alternativa activa", 5.0f);
        }
    }

    // Si aún así no se encuentra, reintentar durante un periodo corto por si el juego está abriéndose
    if (activePid.empty()) {
        for (int i = 0; i < 10; i++) {
            activePid = AdbGetPidZ(GamePackage);
            if (!activePid.empty()) break;
            Sleep(400);
        }
    }

    // Buscar libil2cpp.so con reintentos para dar tiempo a que cargue el juego
    std::string adbCmd = "cat /proc/" + (activePid.empty() ? "$(pidof " + GamePackage + ")" : activePid) + "/maps | grep libil2cpp.so";
    std::string il2cppStr = "";
    for (int i = 0; i < 15; i++) {
        il2cppStr = ShellGetAddressNoSuZ(adbCmd.c_str());
        if (!il2cppStr.empty()) break;
        Sleep(400);
    }

    if (!il2cppStr.empty()) {
        Il2Cpp = uIntExtrZ(il2cppStr);
        is64Bit = (Il2Cpp > 0xFFFFFFFF); // Si es mayor a 32 bits, entonces el guest es 64 bits
        initOffsets(selected_game_version); // Refrescar offsets dinámicamente con la arquitectura detectada
    } else {
        Il2Cpp = 0;
    }

    // Buscar libunity.so con reintentos
    std::string adbCmd2 = "cat /proc/" + (activePid.empty() ? "$(pidof " + GamePackage + ")" : activePid) + "/maps | grep libunity.so";
    std::string libunityStr = "";
    for (int i = 0; i < 15; i++) {
        libunityStr = ShellGetAddressNoSuZ2(adbCmd2.c_str());
        if (!libunityStr.empty()) break;
        Sleep(400);
    }

    if (!libunityStr.empty()) {
        Libunity = uIntExtrZ2(libunityStr);
    } else {
        Libunity = 0;
    }

    KillProcessByFenix(L"HD-Adb.exe");
    // KillProcessByFenix(L"BstkSVC.exe"); // Comentado para evitar que el emulador se cierre inesperadamente

    // Buscar y verificar la Init Base de forma robusta e hilada de fondo (no bloqueante)
    if (Il2Cpp == 0) {
        std::string pkgMsg = "Error: No se detectó el juego " + (GamePackage.find("max") != std::string::npos ? std::string("FF MAX") : std::string("FF Normal"));
        Notifications::Add("ᶻH ᴴ𝒯𝐸𝒜𝑀", pkgMsg.c_str(), 8.0f);
        Notifications::Add("ᶻH ᴴ𝒯𝐸𝒜𝑀", "¿Iniciaste el juego antes de activar el ADB?", 8.0f);
        adbInitialized = false;
        adbThreadRunning = false;
        return 0;
    }

    // Inicializamos el ADB inmediatamente para que el hilo de dibujo ESP empiece a correr a 60 FPS.
    // Esto es CLAVE, ya que el bucle de dibujo llamará a ConvertZ a una frecuencia altísima y capturará
    // instantáneamente la ventana de planificación de la CPU del emulador, rellenando la caché CacheZ de VMM.
    adbInitialized = true;
    adbThreadRunning = false;

    Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", is64Bit ? "ADB Conectado (Guest 64-Bit). Sincronizando..." : "ADB Conectado (Guest 32-Bit). Sincronizando...", 4.0f);

    // Bucle de sincronización asíncrono no bloqueante en hilo secundario
    std::thread([]() {
        bool found = false;
        uint32_t gameFacade = 0;
        
        // Hacemos hasta 60 intentos (12 segundos) esperando que el bucle principal de dibujo sincronice y almacene en cache el mapeo físico
        for (int i = 0; i < 60; i++) {
            if (ReadZ(Il2Cpp + InitBase, gameFacade) && gameFacade != 0) {
                found = true;
                break;
            }
            Sleep(200);
        }

        if (found) {
            char msg[128];
            snprintf(msg, sizeof(msg), "¡Init Base Sincronizada! Facade: 0x%X", gameFacade);
            Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", msg, 5.0f);
        } else {
            Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Sync fallido. Iniciando escaneo heurístico de InitBase...", 5.0f);
            BuscarInitBaseManual(0x8000000, 0xD000000);
        }
    }).detach();

    return 0;
}

//////////////////////////////////////////////////////////

// Buscador manual de InitBase heurístico y ultra-rápido (escaneo por páginas)
inline void BuscarInitBaseManual(uintptr_t startOffset, uintptr_t endOffset) {
    Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", is64Bit ? "Iniciando escaneo de InitBase (64-Bit)..." : "Iniciando escaneo de InitBase (32-Bit)...", 4.0f);
    
    if (Il2Cpp == 0) {
        Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Error: libil2cpp.so no está mapeado. Activa el ADB primero.", 5.0f);
        return;
    }
    
    // Alinear offsets a 8 bytes si es 64-bit, o 4 bytes si es 32-bit
    size_t alignment = is64Bit ? 8 : 4;
    startOffset = (startOffset & ~(alignment - 1));
    endOffset = (endOffset & ~(alignment - 1));
    
    uintptr_t correctOffset = 0;
    uint64_t foundFacade = 0;
    
    const size_t PAGE_SIZE = 4096;
    std::vector<uint8_t> buffer(PAGE_SIZE);
    
    bool found = false;
    
    for (uintptr_t offset = startOffset; offset < endOffset; offset += PAGE_SIZE) {
        uintptr_t virtualPageStart = Il2Cpp + offset;
        uintptr_t physicalPageStart = 0;
        
        // Traducir dirección virtual de la página a física
        if (!ConvertZ(virtualPageStart, physicalPageStart)) {
            continue; 
        }
        
        // Leer la página completa (4KB) de un solo golpe
        if (zHookRead(pVMAddr, physicalPageStart, buffer.data(), PAGE_SIZE) != 0) {
            continue; 
        }
        
        // Buscar con alineación correspondiente
        for (size_t localOffset = 0; localOffset < PAGE_SIZE; localOffset += alignment) {
            uintptr_t candidateOffset = offset + localOffset;
            uint64_t gameFacadeVal = 0;
            
            if (is64Bit) {
                memcpy(&gameFacadeVal, buffer.data() + localOffset, 8);
            } else {
                uint32_t val32 = 0;
                memcpy(&val32, buffer.data() + localOffset, 4);
                gameFacadeVal = val32;
            }
            
            if (gameFacadeVal == 0) continue;
            
            // Validar que gameFacadeVal sea un puntero virtual mapeado en el emulador
            uintptr_t physDummy = 0;
            if (!ConvertZ((uintptr_t)gameFacadeVal, physDummy)) continue;
            
            // Heurística de Validación Dinámica de la Cadena de Clases del Juego:
            // 1. Intentar leer staticGameFacade (gameFacadeVal + StaticClass)
            uintptr_t staticGameFacade = 0;
            if (is64Bit) {
                uint64_t temp = 0;
                if (!ReadZ((uintptr_t)(gameFacadeVal + StaticClass), temp) || temp == 0) continue;
                staticGameFacade = (uintptr_t)temp;
            } else {
                uint32_t temp = 0;
                if (!ReadZ((uintptr_t)(gameFacadeVal + StaticClass), temp) || temp == 0) continue;
                staticGameFacade = (uintptr_t)temp;
            }
            if (!ConvertZ((uintptr_t)staticGameFacade, physDummy)) continue;
            
            // 2. Intentar leer currentGame (staticGameFacade)
            uintptr_t currentGame = 0;
            if (is64Bit) {
                uint64_t temp = 0;
                if (!ReadZ((uintptr_t)staticGameFacade, temp) || temp == 0) continue;
                currentGame = (uintptr_t)temp;
            } else {
                uint32_t temp = 0;
                if (!ReadZ((uintptr_t)staticGameFacade, temp) || temp == 0) continue;
                currentGame = (uintptr_t)temp;
            }
            if (!ConvertZ((uintptr_t)currentGame, physDummy)) continue;
            
            // 3. Intentar leer currentMatch (currentGame + CurrentMatch)
            uintptr_t currentMatch = 0;
            if (is64Bit) {
                uint64_t temp = 0;
                if (!ReadZ((uintptr_t)(currentGame + CurrentMatch), temp) || temp == 0) continue;
                currentMatch = (uintptr_t)temp;
            } else {
                uint32_t temp = 0;
                if (!ReadZ((uintptr_t)(currentGame + CurrentMatch), temp) || temp == 0) continue;
                currentMatch = (uintptr_t)temp;
            }
            if (!ConvertZ((uintptr_t)currentMatch, physDummy)) continue;
            
            // 4. Intentar leer localPlayer (currentMatch + LocalPlayer)
            uintptr_t localPlayer = 0;
            if (is64Bit) {
                uint64_t temp = 0;
                if (!ReadZ((uintptr_t)(currentMatch + LocalPlayer), temp) || temp == 0) continue;
                localPlayer = (uintptr_t)temp;
            } else {
                uint32_t temp = 0;
                if (!ReadZ((uintptr_t)(currentMatch + LocalPlayer), temp) || temp == 0) continue;
                localPlayer = (uintptr_t)temp;
            }
            if (!ConvertZ((uintptr_t)localPlayer, physDummy)) continue;
            
            // ¡SI TODO PASÓ CON ÉXITO, HEMOS ENCONTRADO LA INITBASE CORRECTA!
            correctOffset = candidateOffset;
            foundFacade = gameFacadeVal;
            found = true;
            break;
        }
        
        if (found) break;
    }
    
    if (found) {
        InitBase = correctOffset;
        char msg[256];
        snprintf(msg, sizeof(msg), "¡InitBase encontrada! Offset: 0x%llX (Facade: 0x%llX)", (unsigned long long)InitBase, (unsigned long long)foundFacade);
        Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", msg, 8.0f);
    } else {
        Notifications::Add("ᶻ̷ ᴴ𝒯𝐸𝒜𝑀", "Escaneo finalizado sin resultados. Prueba otro rango.", 6.0f);
    }
}

//////////////////////////////////////////////////////////


// Iniciar ADB y obtener ventana del emulador - Sin Tirones, Sin Lag
HWND INJECTESPADB() {
    static bool initialized = false;
    static HWND cachedHwnd = NULL;

    // Return instantáneo si ya está listo
    if (initialized && cachedHwnd != NULL) {
        return cachedHwnd;
    }

    bool adbinject = true;
    if (!adbinject) {
        return NULL;
    }

    auto vmm = GetModuleHandleA("BstkVMM.dll");
    if (!vmm) {
        return NULL;
    }

    auto readFunc = (PGMPhysReadFunc)GetProcAddress(vmm, "PGMPhysRead");
    if (!readFunc) {
        return NULL;
    }

    // Hooks solo una vez
    static bool hooksInitialized = false;
    if (!hooksInitialized) {
        MH_Initialize();
        if (MH_CreateHook((LPVOID)readFunc, HookedPGMPhysRead, (LPVOID*)&ogPhysRead) != MH_OK) {
            return NULL;
        }
        if (MH_EnableHook((LPVOID)readFunc) != MH_OK) {
            return NULL;
        }
        hooksInitialized = true;
    }

    // Espera no bloqueante para vmPtr
    int attempts = 0;
    while (vmPtr == nullptr && attempts < 100) {
        Sleep(10);  // Sleep corto para no causar tirones
        attempts++;
    }

    if (vmPtr == nullptr) {
        return NULL;
    }

    ogCPU = (VMMGetCpuByIdFunc)GetProcAddress(vmm, "VMMGetCpuById");
    ogCast = (PGMPhysGCPtr2GCPhysFunc)GetProcAddress(vmm, "PGMPhysGCPtr2GCPhys");
    ogWrite = (PGMPhysSimpleWriteGCPhysFunc)GetProcAddress(vmm, "PGMPhysSimpleWriteGCPhys");

    if (!ogCPU || !ogCast || !ogWrite) {
        return NULL;
    }

    InitializeZ(vmPtr);
    CambiarDirectoryZ(GetExeDirectoryZ());

    // Iniciar thread ADB en background (no bloquea el ESP)
    if (!adbThreadRunning && !adbInitialized) {
        adbThreadRunning = true;
        HANDLE hThread = CreateThread(NULL, 0, ADBInitThread, NULL, 0, NULL);
        if (hThread) {
            CloseHandle(hThread);  // Dejar que corra en background
        }
    }


    cachedHwnd = FindRenderWindow();

    // Marcar como inicializado aunque ADB siga en background
    initialized = true;

    return cachedHwnd;
}


//////////////////////////////////////////////////////



bool GetBluestacksMiddlePos(POINT& middlePos) {

    HWND HwndEmul = NULL;
    HwndEmul = FindWindowA(NULL, "BlueStacks App Player");
    if (HwndEmul == NULL)
    {
        HwndEmul = FindWindowA(NULL, "MSI App Player");
        if (HwndEmul == NULL)
        {
            HwndEmul = FindWindowA(NULL, "BlueStacks");
            if (HwndEmul == NULL)
            {
                HwndEmul = FindWindowA(NULL, "App Player");
                if (HwndEmul == NULL)
                {
                    HwndEmul = FindWindowA(NULL, "Ultra God");
                    if (HwndEmul == NULL)
                    {
                        HwndEmul = FindWindowA(NULL, "e4vX Bs4");
                        if (HwndEmul == NULL)
                        {
                            HwndEmul = FindWindowA(NULL, "@cosmo_byte");
                            if (HwndEmul == NULL)
                            {

                            }
                            else
                            {

                            }
                        }
                        else
                        {

                        }
                    }
                    else
                    {

                    }
                }
                else
                {

                }
            }
            else
            {

            }
        }
        else
        {

        }
    }

    RECT clientRect;
    if (!GetClientRect(HwndEmul, &clientRect)) {
        return false;
    }

    middlePos.x = (clientRect.right - clientRect.left) / 2;
    middlePos.y = (clientRect.bottom - clientRect.top) / 2;

    ClientToScreen(HwndEmul, &middlePos);
    return true;
}
///////////////////////////////

