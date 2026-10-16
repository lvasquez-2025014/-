## 2024-10-16 - Custom ImGui Hover Effects
**Learning:** When using InvisibleButton to create custom drawn UI components in ImGui, the built-in style colors for hover/active states are ignored since no background is drawn. Users miss out on visual feedback.
**Action:** Manually check `ImGui::IsItemHovered()` immediately after `ImGui::InvisibleButton()` and implement custom drawing logic to provide hover state feedback, ensuring interactive elements feel responsive.
