## 2024-10-14 - Added tooltips for ImGui icon buttons
**Learning:** ImGui InvisibleButtons used as icons lack context. In ImGui, hovering interactions are usually detected with `ImGui::IsItemHovered()` immediately after the UI element, combined with `ImGui::SetTooltip()`.
**Action:** Always check `ImGui::InvisibleButton` usages for missing tooltips.
