## 2024-10-18 - ImGui Custom Interactive Element Hover States
**Learning:** Custom interactive elements in ImGui like `ImGui::InvisibleButton` and non-standard `ImGui::Selectable` links don't inherit default pointer cursors when hovered, making them feel less discoverable and "clickable" to the user, similar to web buttons lacking `cursor: pointer`.
**Action:** Always append `if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);` immediately after such custom elements to restore standard UX interaction affordances in C++ ImGui applications.
