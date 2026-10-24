## 2024-10-24 - Added Missing Hand Cursors to Interactive Elements
**Learning:** In this C++ ImGui application, interactive elements created using `ImGui::InvisibleButton` or `ImGui::Selectable` do not automatically show a hand cursor when hovered, leaving users without clear visual feedback that they are clickable.
**Action:** Applied `ImGui::IsItemHovered()` followed by `ImGui::SetMouseCursor(ImGuiMouseCursor_Hand)` to all such elements to provide the missing UX affordance.
