#include <algorithm>
#include <chrono> // modern, preferred over ctime (C-style)
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "imgui/imgui.h"

class Logger {
    #pragma region Meyers Singleton
    public:
        static Logger& GetInstance() {
            /*
                Creates a function-local static variable
                - initializes only the first time the function is called
                - statically allocated
                - is thread safe since C++11
                - lives until the end of the program and is disposed properly
            */
            static Logger instance;
            return instance;
        }

        Logger(const Logger&) = delete; // copy constructor
        Logger& operator=(const Logger&) = delete; // assignment operator

    private:
        Logger() = default;
        ~Logger() = default;
    #pragma endregion Meyers Singleton

    private:
        bool _isWindowOpen = true;

        enum class LogMessageType { Info, Warning, Error };
        const ImVec4 WHITE = ImVec4(1, 1, 1, 1);
        const ImVec4 YELLOW = ImVec4(1, 1, 0, 1);
        const ImVec4 RED = ImVec4(1, 0, 0, 1);
        struct LogMessage {
            std::string Time;
            LogMessageType Type;
            std::string Message;
        };
        std::vector<LogMessage> _logMessages;

        int logMessageMinLevelFilter = 0;

        std::string GetTime() {
            // written by ChatGPT because I don't know what a chrono is

            auto now = std::chrono::system_clock::now();

            std::time_t secondsSinceEpoch = std::chrono::system_clock::to_time_t(now);
            auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

            std::tm localTime;
            localtime_s(&localTime, &secondsSinceEpoch);

            std::stringstream stringStream;
            stringStream
                << std::put_time(&localTime, "%H:%M:%S") // HH:MM:SS
                << '.' << std::setfill('0') << std::setw(3) << milliseconds.count(); // .MMM
            return stringStream.str();
        }

        void LogToFile() {
            std::ofstream LogFile("custom_log.txt");
            for (const auto& logMessage : _logMessages) {
                std::string logMessageType_Uppercase = LogMessageTypeToString(logMessage.Type);
                std::transform(logMessageType_Uppercase.begin(), logMessageType_Uppercase.end(), logMessageType_Uppercase.begin(), ::toupper);
                std::string completeMessage = std::format("[{}] [{}] {}", logMessage.Time, logMessageType_Uppercase, logMessage.Message);
                LogFile << completeMessage << std::endl;
            }
            LogFile.close();
        }

        int LogMessageTypeToInt(LogMessageType type) {
            switch (type) {
                case LogMessageType::Info:
                    return 0;
                case LogMessageType::Warning:
                    return 1;
                case LogMessageType::Error:
                    return 2;
            }
            throw "Switch statement exited in LogMessageTypeToInt()";
        }

        ImVec4 LogMessageTypeToColor(LogMessageType type) {
            switch (type) {
                case LogMessageType::Info:
                    return WHITE;
                case LogMessageType::Warning:
                    return YELLOW;
                case LogMessageType::Error:
                    return RED;
            }
            throw "Switch statement exited in LogMessageTypeToColor()";
        }

        std::string LogMessageTypeToString(LogMessageType type) {
            switch (type) {
                case LogMessageType::Info:
                    return "Info";
                case LogMessageType::Warning:
                    return "Warn";
                case LogMessageType::Error:
                    return "Error";
            }
            throw "Switch statement exited in LogMessageTypeToString()";
        }

    public:
        bool IsWindowOpen() {
            return _isWindowOpen;
        }
        void ToggleWindow() {
            _isWindowOpen = !_isWindowOpen;
        }
        void Render() {
            if (!_isWindowOpen) {
                return;
            }

            if (!ImGui::Begin("Game Log", &_isWindowOpen)) { // pass a bool* to Begin() to enable 'X' button
                ImGui::End();
                return;
            }

            if (ImGui::Button("Test Info")) {
                _logMessages.push_back({ GetTime(), LogMessageType::Info, "This is a test info message" });
            }
            ImGui::SameLine();

            if (ImGui::Button("Test Warning")) {
                _logMessages.push_back({ GetTime(), LogMessageType::Warning, "This is a test warning message" });
            }
            ImGui::SameLine();

            if (ImGui::Button("Test Error")) {
                _logMessages.push_back({ GetTime(), LogMessageType::Error, "This is a test error message" });
            }
            ImGui::SameLine();

            if (ImGui::Button("Clear")) {
                _logMessages.clear();
            }

            if (ImGui::Button("Log To File (Custom)")) {
                LogToFile(); // overwrites
            }
            ImGui::SameLine();

            if (ImGui::Button("Log To File (ImGui)")) {
                ImGui::LogToFile(); // appends
            }
            ImGui::Separator();

            ImGui::Combo("Level Filter", &logMessageMinLevelFilter, "Info\0Warning\0Error\0\0");

            if (ImGui::BeginChild("Scrolling Region", ImVec2(0, 0), ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar)) {
                for (const auto& logMessage : _logMessages) {
                    if (LogMessageTypeToInt(logMessage.Type) < logMessageMinLevelFilter) {
                        continue;
                    }

                    ImGui::PushStyleColor(ImGuiCol_Text, LogMessageTypeToColor(logMessage.Type));

                    std::string logMessageType_Uppercase = LogMessageTypeToString(logMessage.Type);
                    std::transform(logMessageType_Uppercase.begin(), logMessageType_Uppercase.end(), logMessageType_Uppercase.begin(), ::toupper);
                    std::string completeMessage = std::format("[{}] [{}] {}", logMessage.Time, logMessageType_Uppercase, logMessage.Message);
                    ImGui::TextUnformatted(completeMessage.c_str());

                    ImGui::PopStyleColor();
                }
            }
            ImGui::EndChild();

            ImGui::End();
        }
};
