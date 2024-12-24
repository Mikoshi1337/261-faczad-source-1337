//#define IMGUI_DEFINE_MATH_OPERATORS
//#include <vector>
//#include <algorithm>
//#include <windows.h>
//#include <urlmon.h>
//#pragma comment(lib, "urlmon.lib")
//#include <vector>
//#include <string>
//bool applied = false;
//static bool sh_save_cfg = false;
//static bool sh_load_cfg = false;
//#include <chrono>
//#include <xstring>
//
//long getMils() {
//    auto duration = std::chrono::system_clock::now().time_since_epoch();
//
//    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
//}
//
//int y = 0;
//#include <map>
//#include "../Visuals/menu/imgui/imgui.h"
//#include "../Visuals/menu/imgui/imgui_internal.h"
//#include "c.h"
//using namespace ImGui;
//void CricleProgress(const char* name, float progress, float max, float radius, const ImVec2& size)
//{
//    const float tickness = 3.f;
//    static float position = 0.f;
//
//    position = progress / max * 6.28f;
//
//    ImGui::GetForegroundDrawList()->PathClear();
//    ImGui::GetForegroundDrawList()->PathArcTo(GetCursorScreenPos() + size, radius, 0.f, 2.f * IM_PI, 120.f);
//    ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(c::elements::background_widget), 0, tickness);
//
//    ImGui::GetForegroundDrawList()->PathClear();
//    ImGui::GetForegroundDrawList()->PathArcTo(GetCursorScreenPos() + size, radius, IM_PI * 1.5f, IM_PI * 1.5f + position, 120.f);
//    ImGui::GetForegroundDrawList()->PathStroke(ImGui::GetColorU32(c::accent), 0, tickness);
//
//    int procent = progress / (int)max * 100;
//
//    std::string procent_str = std::to_string(procent) + "%";
//
//}
//
//struct Notification {
//    int id;
//    std::string message;
//    std::chrono::steady_clock::time_point startTime;
//    std::chrono::steady_clock::time_point endTime;
//};
//
//class NotificationSystem {
//public:
//    NotificationSystem() : notificationIdCounter(0) {}
//
//    void AddNotification(const std::string& message, int durationMs) {
//        auto now = std::chrono::steady_clock::now();
//        auto endTime = now + std::chrono::milliseconds(durationMs);
//        notifications.push_back({ notificationIdCounter++, message, now, endTime });
//    }
//
//    void DrawNotifications() {
//        auto now = std::chrono::steady_clock::now();
//
//        std::sort(notifications.begin(), notifications.end(),
//            [now](const Notification& a, const Notification& b) -> bool {
//                float durationA = std::chrono::duration_cast<std::chrono::milliseconds>(a.endTime - a.startTime).count();
//                float elapsedA = std::chrono::duration_cast<std::chrono::milliseconds>(now - a.startTime).count();
//                float percentageA = (durationA - elapsedA) / durationA;
//
//                float durationB = std::chrono::duration_cast<std::chrono::milliseconds>(b.endTime - b.startTime).count();
//                float elapsedB = std::chrono::duration_cast<std::chrono::milliseconds>(now - b.startTime).count();
//                float percentageB = (durationB - elapsedB) / durationB;
//
//                return percentageA < percentageB;
//            }
//        );
//
//        int currentNotificationPosition = 0;
//
//        for (auto& notification : notifications) {
//            if (now < notification.endTime) {
//                float duration = std::chrono::duration_cast<std::chrono::milliseconds>(notification.endTime - notification.startTime).count();
//                float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - notification.startTime).count();
//                float percentage = (duration - elapsed) / duration * 100.0f;
//
//                ShowNotification(currentNotificationPosition, notification.message, percentage);
//                currentNotificationPosition++;
//            }
//        }
//
//        notifications.erase(std::remove_if(notifications.begin(), notifications.end(),
//            [now](const Notification& notification) { return now >= notification.endTime; }),
//            notifications.end());
//    }
//
//private:
//    std::vector<Notification> notifications;
//    int notificationIdCounter;
//
//    void ShowNotification(int position, const std::string& message, float percentage) {
//
//        float duePercentage = 100.0f - percentage;
//        float alpha = percentage > 10.0f ? 1.0f : percentage / 10.0f;
//        GetStyle().WindowPadding = ImVec2(15, 10);
//
//        ImGui::SetNextWindowPos(ImVec2(duePercentage < 15.f ? duePercentage : 15.f, 15 + position * 90));
//
//        ImGui::Begin(("##NOTIFY" + std::to_string(position)).c_str(), nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus);
//
//        ImVec2 pos = ImGui::GetWindowPos(), spacing = ImGui::GetStyle().ItemSpacing, region = ImGui::GetContentRegionMax();
//
//        GetBackgroundDrawList()->AddRectFilledMultiColor(pos, pos + region, ImGui::GetColorU32s(c::background::filling, alpha), ImGui::GetColorU32s(c::accent, 0.01f), ImGui::GetColorU32s(c::accent, 0.01f), ImGui::GetColorU32s(c::background::filling, alpha));
//        GetBackgroundDrawList()->AddRectFilled(pos, pos + region, ImGui::GetColorU32s(c::background::filling, 0.4f), c::elements::rounding);
//        GetBackgroundDrawList()->AddRect(pos, pos + region, ImGui::GetColorU32s(c::background::stroke, alpha), c::elements::rounding);
//
//
//        GetBackgroundDrawList()->AddRectFilled(pos + ImVec2(0, region.y - 3), pos + ImVec2(region.x * (duePercentage / 100.0f), region.y), ImGui::GetColorU32s(c::accent, alpha), c::elements::rounding);
//
//        PushFont(font::lexend_bold);
//        ImGui::TextColored(ImColor(ImGui::GetColorU32s(c::accent, alpha)), "%s", "[Notification]");
//        ImGui::TextColored(ImColor(ImGui::GetColorU32s(c::elements::text_active, alpha)), "%s", message.c_str());
//        ImGui::Dummy(ImVec2(CalcTextSize(message.c_str()).x + 15, 5));
//        PopFont();
//
//        ImGui::End();
//    }
//};
//
//NotificationSystem notificationSystem;
