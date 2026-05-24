#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <algorithm>
#include "imgui.h"

struct Notification {
    std::string title;
    std::string message;
    float maxDuration;     // seconds
    float remainingTime;   // seconds
    float alpha;           // current alpha for fading
    float slideY;          // animation offset
};

namespace Notifications {
    inline std::vector<Notification> list;
    inline std::mutex mtx;

    inline void Add(const std::string& title, const std::string& message, float duration = 4.0f) {
        std::lock_guard<std::mutex> lock(mtx);
        Notification n;
        n.title = title;
        n.message = message;
        n.maxDuration = duration;
        n.remainingTime = duration;
        n.alpha = 0.0f;
        n.slideY = -50.0f;
        list.push_back(n);
    }

    inline void Draw() {
        std::lock_guard<std::mutex> lock(mtx);
        if (list.empty()) return;

        ImDrawList* draw = ImGui::GetForegroundDrawList();
        ImGuiIO& io = ImGui::GetIO();
        float deltaTime = io.DeltaTime;

        // Position coordinates (Top-Left of the screen)
        ImVec2 displaySize = io.DisplaySize;
        float startX = 20.0f;
        float startY = 20.0f;

        float gap = 12.0f;
        float currentY = startY;

        for (auto it = list.begin(); it != list.end(); ) {
            // Update timers
            it->remainingTime -= deltaTime;

            if (it->remainingTime <= 0.0f) {
                // Fade out
                it->alpha = std::max(0.0f, it->alpha - deltaTime * 4.0f);
                if (it->alpha <= 0.0f) {
                    it = list.erase(it);
                    continue;
                }
            }
            else {
                // Fade in and slide down
                it->alpha = std::min(1.0f, it->alpha + deltaTime * 5.0f);
                it->slideY = it->slideY + (0.0f - it->slideY) * deltaTime * 10.0f;
            }

            // Notification Dimensions
            float width = 320.0f;
            float height = 65.0f;

            ImVec2 pMin(startX, currentY + it->slideY);
            ImVec2 pMax(startX + width, currentY + height + it->slideY);

            // Draw shadow
            ImU32 shadowCol = IM_COL32(0, 0, 0, (int)(110 * it->alpha));
            draw->AddRectFilled(ImVec2(pMin.x + 3, pMin.y + 3), ImVec2(pMax.x + 3, pMax.y + 3), shadowCol, 6.0f);

            // Draw Background (Semi-transparent very dark premium style)
            ImU32 bgCol = IM_COL32(15, 15, 15, (int)(235 * it->alpha));
            draw->AddRectFilled(pMin, pMax, bgCol, 6.0f);

            // Draw left border accent (Gold)
            ImU32 accentCol = IM_COL32(255, 180, 0, (int)(255 * it->alpha));
            draw->AddRectFilled(pMin, ImVec2(pMin.x + 4.0f, pMax.y), accentCol, 6.0f, ImDrawFlags_RoundCornersLeft);

            // Draw border line
            ImU32 borderCol = IM_COL32(45, 45, 45, (int)(200 * it->alpha));
            draw->AddRect(pMin, pMax, borderCol, 6.0f, 0, 1.0f);

            // Draw Logo / Icon (Golden Emblem style)
            ImVec2 iconCenter(pMin.x + 25.0f, pMin.y + height * 0.5f);

            // Draw a beautiful hexagon (like a glowing badge/shield)
            ImVec2 points[6];
            float r = 10.0f;
            for (int i = 0; i < 6; i++) {
                float angle = i * 3.14159265f / 3.0f;
                points[i] = ImVec2(iconCenter.x + cosf(angle) * r, iconCenter.y + sinf(angle) * r);
            }
            draw->AddConvexPolyFilled(points, 6, IM_COL32(255, 180, 0, (int)(60 * it->alpha)));
            draw->AddPolyline(points, 6, accentCol, ImDrawFlags_Closed, 1.5f);

            // Innermost glowing dot
            draw->AddCircleFilled(iconCenter, 3.0f, accentCol);

            // Draw Title ("ZERO CHEATS")
            ImU32 titleCol = IM_COL32(255, 180, 0, (int)(255 * it->alpha));
            ImVec2 titlePos(pMin.x + 50.0f, pMin.y + 12.0f);
            draw->AddText(titlePos, titleCol, it->title.c_str());

            // Draw Subtitle / Message
            ImU32 msgCol = IM_COL32(230, 230, 230, (int)(255 * it->alpha));
            ImVec2 msgPos(pMin.x + 50.0f, pMin.y + 32.0f);
            draw->AddText(msgPos, msgCol, it->message.c_str());

            currentY += height + gap;
            it++;
        }
    }
}
