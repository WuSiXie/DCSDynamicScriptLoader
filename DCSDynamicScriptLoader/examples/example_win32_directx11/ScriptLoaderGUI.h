#pragma once
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <sstream>
#include <vector>
#include "networkmoduel.h"
#include "NetworkGUI.h"

void DCSScriptLoader()
{

    static ImGuiInputTextFlags LuaScriptInputflags = ImGuiInputTextFlags_AllowTabInput;
    static bool ScriptLoaderON = false;
    static char* ScriptBuffer = new char[1024*1024-5];
    static bool DATASENDFAILEDPOPUP = false;
    static bool DATASENDSUCCEEDPOPUP = false;
    static bool LoadSCinMission = false;
    ImGui::Checkbox(u8"脚本加载器##ScriptLoaderONCheck", &ScriptLoaderON);
    if (ScriptLoaderON)
    {
        ImGui::Begin(u8"DCS脚本加载器", &ScriptLoaderON);
        if (!strcmp(Env, "Server"))
        {
            ImGui::Checkbox("在任务环境加载脚本", &LoadSCinMission);
            ImGui::Text("由于ED的BUG，该选项在专用服务器和以服务器模式运行的DCS中可能无效");
        }
        else 
        {
            LoadSCinMission = false;
        }
        ImGui::InputTextMultiline("##LuaScriptsInput", ScriptBuffer, 1024 * 1024 - 5, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 20), LuaScriptInputflags);
        if (ImGui::Button(u8"发送##SendLuaScript"))
        {
            static char* LuaDataToSend = new char[1024 * 1024]();
            if (LoadSCinMission)
            {
                strcpy(LuaDataToSend, "2 ");
            }
            else
            {
                strcpy(LuaDataToSend, "1 ");
            }
            strcat(LuaDataToSend, ScriptBuffer);
            int LuaScriptSendingStatus = SendData(ConnectionToServer, LuaDataToSend);
            if (LuaScriptSendingStatus == -1)
            {
                DATASENDFAILEDPOPUP = true;
                Disconnect(ConnectionToServer);
                ConnectionLostPopup = true;
            }
            else
            {
                DATASENDSUCCEEDPOPUP = true;
                ScriptBuffer[0] = '\0';
                LuaDataToSend[0] = '\0';
            }

        }
        if (DATASENDFAILEDPOPUP)
        {
            ImGui::OpenPopup(u8"LUA数据发送出错");
            if (ImGui::BeginPopup(u8"LUA数据发送出错"))
                ImGui::Text(u8"LUA数据发送出错");
            if (ImGui::Button("确定##LUADATASENDINGFAILD"))
            {
                ImGui::CloseCurrentPopup();
                DATASENDFAILEDPOPUP = false;
            }
            ImGui::EndPopup();
        }
        else if (DATASENDSUCCEEDPOPUP)
        {
            ImGui::OpenPopup(u8"LUA数据发送成功");
            if (ImGui::BeginPopup(u8"LUA数据发送成功"))
                ImGui::Text(u8"LUA数据发送成功");
            if (ImGui::Button("确定##LUADATASENDINGSUCCESS"))
            {
                ImGui::CloseCurrentPopup();
                DATASENDSUCCEEDPOPUP = false;
            }
            ImGui::EndPopup();
        }
        ImGui::End();
    }
}
