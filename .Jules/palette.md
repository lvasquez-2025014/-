## 2024-05-15 - Missing hover affordances on ImGui custom interactive elements
**Learning:** In this project, `ImGui::InvisibleButton` for icons and `ImGui::Selectable` configured for text links do not have native visible hover states or cursor changes, which creates poor usability.
**Action:** When working with these elements, remember to always use `ImGui::IsItemHovered()` to manually change styles (like link underlines and eye stroke colors) and explicitely set `ImGui::SetMouseCursor(ImGuiMouseCursor_Hand)`.
