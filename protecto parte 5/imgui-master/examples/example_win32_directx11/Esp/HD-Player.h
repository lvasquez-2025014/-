
HWND RenderWindow;

std::vector<DWORD> GetProcessIdsByName(const std::wstring& ProcessName)
{
    std::vector<DWORD> processIds;

    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnapshot == INVALID_HANDLE_VALUE)
        return processIds;

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(PROCESSENTRY32W);

    if (Process32FirstW(hSnapshot, &processEntry))
    {
        do
        {
            std::wstring exeFileName = processEntry.szExeFile;
            if (_wcsicmp(ProcessName.c_str(), exeFileName.c_str()) == 0) 
            {
                processIds.push_back(processEntry.th32ProcessID);
            }
        } while (Process32NextW(hSnapshot, &processEntry));
    }

    CloseHandle(hSnapshot);
    return processIds;
}



inline BOOL CALLBACK EnumChildWindowsProc(HWND hWnd, LPARAM lParam)
{
    char windowName[256];
    GetWindowTextA(hWnd, windowName, sizeof(windowName));
    std::string name(windowName);

    char className[256];
    GetClassNameA(hWnd, className, sizeof(className));
    std::string class_str(className);

    if (name == "_ctl.Window" && class_str.find("BlueStacksApp") != std::wstring::npos)
    {
        RenderWindow = hWnd;
        return FALSE;
    }
    else if (name == "HD-Player" && class_str.find("Qt") != std::wstring::npos)
    {
        RenderWindow = hWnd;
        return FALSE;
    }

    return TRUE;
}


inline BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
    DWORD processId = *reinterpret_cast<DWORD*>(lParam);

    DWORD windowProcessId;
    GetWindowThreadProcessId(hWnd, &windowProcessId);

    if (windowProcessId == processId)
    {
        EnumChildWindows(hWnd, EnumChildWindowsProc, lParam);
    }

    return TRUE;
}


HWND FindRenderWindow()
{
    auto BlueStacksProcesses = GetProcessIdsByName(L"HD-Player.exe");

    if (BlueStacksProcesses.empty()) {
        std::cout << "[ERROR] Nenhum processo do BlueStacks encontrado!" << std::endl;
        return nullptr;
    }

    for (const auto& ProcessId : BlueStacksProcesses)
    {
        HANDLE ProcessHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
        if (ProcessHandle)
        {
            EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&ProcessId));

            CloseHandle(ProcessHandle);
        }
    }

    return RenderWindow;
}
