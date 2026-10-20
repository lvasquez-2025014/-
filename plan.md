1. **Explore & Analyze**: Checked `main.cpp` for interactive elements such as `InvisibleButton` and `Selectable` used as text links. Confirmed they lack explicit cursor feedback upon hovering, reducing discoverability.
2. **Implement Hand Cursor Hover States**: Add `if (ImGui::IsItemHovered()) ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);` right after the declaration of:
   - Password toggles (`##toggle_pass`, `##toggle_reg_pass`).
   - Registration and Login text links (`¿Olvido su contrasena?`, `Registrese aqui`, `Inicie sesion aqui`).
   - Sidebar buttons, including adding a subtle visual background for `SidebarButton` when hovered but inactive.
3. **Journal Entry**: Create `.Jules/palette.md` and log this critical UX finding about missing native hover states for custom ImGui elements and the required action.
4. **Pre-commit**: Complete pre-commit steps to ensure proper testing, verifications, reviews, and reflections are done.
5. **Submit**: Create PR for the micro-UX enhancement.
