## 2024-11-20 - Added hover state to ImGui SidebarButton
**Learning:** In immediate mode GUIs like ImGui, checking for hover states on a custom button requires submitting the invisible hit-box (`InvisibleButton`) *before* performing the conditional drawing.
**Action:** When implementing custom interactive elements in ImGui, ensure interaction logic (like `IsItemHovered()`) is evaluated right after declaring the hit-box, and use saved cursor positions for subsequent rendering.
