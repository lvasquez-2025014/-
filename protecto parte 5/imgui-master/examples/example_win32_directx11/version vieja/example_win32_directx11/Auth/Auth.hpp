#pragma once
#include <windows.h>
#include <wininet.h>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include "skCrypt.h"

#pragma comment(lib, "wininet.lib")

namespace KeyAuth {

    // Codificación URL robusta para evitar errores de caracteres especiales y espacios
    inline std::string url_encode(const std::string& value) {
        std::ostringstream escaped;
        escaped << std::hex;
        for (char c : value) {
            if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
                escaped << c;
            } else {
                escaped << '%' << std::uppercase << std::setw(2) << std::setfill('0') << (int)(unsigned char)c;
            }
        }
        return escaped.str();
    }

    inline std::string get_hwid() {
        DWORD serialNum = 0;
        GetVolumeInformationA("C:\\", NULL, 0, &serialNum, NULL, NULL, NULL, 0);
        
        char compName[MAX_COMPUTERNAME_LENGTH + 1] = "COMP";
        DWORD compNameLen = sizeof(compName);
        GetComputerNameA(compName, &compNameLen);
        
        char userName[256] = "USER";
        DWORD userNameLen = sizeof(userName);
        GetUserNameA(userName, &userNameLen);

        std::stringstream ss;
        ss << "ᶻ̷ ᴴ𝒯𝐸𝒜𝑀-" << std::hex << serialNum << "-" << compName << "-" << userName;
        std::string hwid = ss.str();
        
        // Relleno de seguridad si la longitud es inferior a 24 caracteres
        while (hwid.length() < 24) {
            hwid += "-SYSTEM";
        }
        return hwid;
    }

    inline std::string req(const std::string& data, const std::string& api_url) {
        HINTERNET hInternet = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
        if (!hInternet) return "";

        // Parse host and path from api_url
        std::string host = "keyauth.win";
        std::string path = "/api/1.3/";

        HINTERNET hConnect = InternetConnectA(hInternet, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
        if (!hConnect) {
            InternetCloseHandle(hInternet);
            return "";
        }

        const char* acceptTypes[] = { "application/json", NULL };
        HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", path.c_str(), NULL, NULL, acceptTypes, INTERNET_FLAG_SECURE | INTERNET_FLAG_RELOAD, 0);
        if (!hRequest) {
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            return "";
        }

        std::string headers = "Content-Type: application/x-www-form-urlencoded\r\n";
        BOOL sent = HttpSendRequestA(hRequest, headers.c_str(), headers.length(), (LPVOID)data.c_str(), data.length());

        std::string response = "";
        if (sent) {
            char buffer[1024];
            DWORD bytesRead = 0;
            while (InternetReadFile(hRequest, buffer, sizeof(buffer) - 1, &bytesRead) && bytesRead > 0) {
                buffer[bytesRead] = '\0';
                response += buffer;
            }
        }

        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return response;
    }

    class api {
    public:
        std::string name, ownerid, version, url;
        std::string sessionid;
        std::string last_error;

        api(std::string name, std::string ownerid, std::string version, std::string url)
            : name(name), ownerid(ownerid), version(version), url(url) {}

        void init() {
            try {
                std::string data = "type=init&ver=" + url_encode(version) + "&name=" + url_encode(name) + "&ownerid=" + url_encode(ownerid);
                std::string response = req(data, url);

                if (response.empty()) {
                    last_error = "El servidor KeyAuth no respondio. Verifica tu conexion a internet.";
                    return;
                }

                if (response.find("\"success\":true") != std::string::npos) {
                    size_t sessPos = response.find("\"sessionid\":\"");
                    if (sessPos != std::string::npos) {
                        sessPos += 13;
                        size_t endPos = response.find("\"", sessPos);
                        if (endPos != std::string::npos) {
                            sessionid = response.substr(sessPos, endPos - sessPos);
                        }
                    }
                } else {
                    size_t msgPos = response.find("\"message\":\"");
                    if (msgPos != std::string::npos) {
                        msgPos += 11;
                        if (msgPos < response.length()) {
                            size_t endPos = response.find("\"", msgPos);
                            if (endPos != std::string::npos && endPos > msgPos) {
                                last_error = response.substr(msgPos, endPos - msgPos);
                            } else {
                                last_error = "Error en formato de respuesta del servidor.";
                            }
                        } else {
                            last_error = "Respuesta del servidor incompleta.";
                        }
                    } else {
                        last_error = "Error al conectar con el panel de KeyAuth.";
                    }
                }
            } catch (...) {
                last_error = "Error critico de red durante la inicializacion.";
            }
        }

        bool login(std::string username, std::string password) {
            try {
                if (sessionid.empty()) {
                    init();
                    if (sessionid.empty()) {
                        return false;
                    }
                }
                std::string hwid = get_hwid();
                std::string data = "type=login&username=" + url_encode(username) + "&pass=" + url_encode(password) + "&hwid=" + url_encode(hwid) + "&sessionid=" + url_encode(sessionid) + "&name=" + url_encode(name) + "&ownerid=" + url_encode(ownerid);
                std::string response = req(data, url);

                if (response.empty()) {
                    last_error = "El servidor no respondio al iniciar sesion.";
                    return false;
                }

                if (response.find("\"success\":true") != std::string::npos) {
                    return true;
                } else {
                    size_t msgPos = response.find("\"message\":\"");
                    if (msgPos != std::string::npos) {
                        msgPos += 11;
                        if (msgPos < response.length()) {
                            size_t endPos = response.find("\"", msgPos);
                            if (endPos != std::string::npos && endPos > msgPos) {
                                last_error = response.substr(msgPos, endPos - msgPos);
                            } else {
                                last_error = "Credenciales incorrectas (Error de formato).";
                            }
                        } else {
                            last_error = "Respuesta de inicio de sesion incompleta.";
                        }
                    } else {
                        last_error = "Error al iniciar sesion. Intente de nuevo.";
                    }
                    return false;
                }
            } catch (...) {
                last_error = "Error critico de memoria durante el inicio de sesion.";
                return false;
            }
        }

        bool regstr(std::string username, std::string password, std::string key) {
            try {
                if (sessionid.empty()) {
                    init();
                    if (sessionid.empty()) {
                        return false;
                    }
                }
                std::string hwid = get_hwid();
                std::string data = "type=register&username=" + url_encode(username) + "&pass=" + url_encode(password) + "&key=" + url_encode(key) + "&hwid=" + url_encode(hwid) + "&sessionid=" + url_encode(sessionid) + "&name=" + url_encode(name) + "&ownerid=" + url_encode(ownerid);
                std::string response = req(data, url);

                if (response.empty()) {
                    last_error = "El servidor no respondio al registrarse.";
                    return false;
                }

                if (response.find("\"success\":true") != std::string::npos) {
                    return true;
                } else {
                    size_t msgPos = response.find("\"message\":\"");
                    if (msgPos != std::string::npos) {
                        msgPos += 11;
                        if (msgPos < response.length()) {
                            size_t endPos = response.find("\"", msgPos);
                            if (endPos != std::string::npos && endPos > msgPos) {
                                last_error = response.substr(msgPos, endPos - msgPos);
                            } else {
                                last_error = "Licencia invalida o ya usada.";
                            }
                        } else {
                            last_error = "Respuesta de registro de licencia incompleta.";
                        }
                    } else {
                        last_error = "Error al registrar la licencia.";
                    }
                    return false;
                }
            } catch (...) {
                last_error = "Error critico de memoria durante el registro.";
                return false;
            }
        }
    };
}
