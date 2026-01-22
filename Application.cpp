#include "Application.h"
#include "Logger.h"
#include "imgui/imgui.h"

/*
    Created the Logger class
    No other classes have been changed
*/
namespace ClassGame {
    //
    // our global variables
    //

    //
    // game starting point
    // this is called by the main render loop in main.cpp
    //
    void GameStartUp() 
    {
        // Initialize logging system
        // Logger& logger = Logger::GetInstance();
        // logger.LogInfo("Game started successfully");
        // logger.LogGameEvent("Application initialized");
    }

    //
    // game render loop
    // this is called by the main render loop in main.cpp
    //
    void RenderGame() 
    {
        ImGui::DockSpaceOverViewport();
        ImGui::ShowDemoWindow();
        Logger::GetInstance().Render();
    }

    //
    // end turn is called by the game code at the end of each turn
    // this is where we check for a winner
    //
    void EndOfTurn() 
    {
    }
}
