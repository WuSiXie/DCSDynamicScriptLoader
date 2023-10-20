#pragma once
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <sstream>
#include <vector>
#include <thread>
#include<iostream>
//网络模块
#include "networkmoduel.h"

SOCKET ConnectionToServer;
char Env[10] = "";
int IsInputValidIPAddress(ImGuiInputTextCallbackData* data)
{
    if ('9' >= data->EventChar && data->EventChar >= '0' || data->EventChar == '.')
    {
        return 0;
    }
    else
        return 1;
}

int IsInputValidPort(ImGuiInputTextCallbackData* data)
{
    if ('9' >= data->EventChar && data->EventChar >= '0')
    {
        return 0;
    }
    else
        return 1;
}

void CheckPasswd(SOCKET* Connection, bool* PasswdStatus, bool* PasswdErrorPopUpComeOut)
{
    char PasswdCheckBuffer[20] = "";
    ReceiveData(*Connection,PasswdCheckBuffer,sizeof(PasswdCheckBuffer));
    //std::cout << PasswdCheckBuffer << std::endl;
    if (!strcmp(PasswdCheckBuffer, u8"Connected"))
    {
        //std::cout << u8"已连接" << std::endl;
        *PasswdStatus = true;
    }
    else
    {
        //std::cout << u8"无法连接" << std::endl;
        *PasswdStatus = false;
        *PasswdErrorPopUpComeOut = true;
    }
}
bool ConnectionLostPopup = false;
void ConnectinLostManager()
{
    if (ConnectionLostPopup)
    {
        ImGui::OpenPopup(u8"连接丢失");
        if (ImGui::BeginPopup(u8"连接丢失"))
            ImGui::Text(u8"与服务器的连接丢失");
        if (ImGui::Button("确定##ConnetionLost"))
        {
            ImGui::CloseCurrentPopup();
            ConnectionLostPopup = false;
        }
        ImGui::EndPopup();
    }
}

void envreciever(SOCKET* Connection, char* EnvBuffer, int EnvBufferSize)
{
    ReceiveData(*Connection, EnvBuffer, EnvBufferSize);
}

void AskForEnv(SOCKET* Connection, char* EnvData, int EnvDataSize)
{
    int SendingStatus = SendData(*Connection, "0 ENV");
    std::thread envdatareciever(envreciever, Connection, EnvData, EnvDataSize);
    envdatareciever.detach();
}

bool ConnectionWindowStatus = true;

void ShowConnectionPage(bool* networkConnectionStatus)
{
    static bool PasswdStatus = false;
    static bool ConnectionFailedPopUP = false;
    static bool PasswdFalsePopUP = false;
    static char ipInputBuffer[16] = "127.0.0.1";
    static int PortBuffer = 49527;
    static char Passwd[99] = "995277";
    ImGuiInputTextFlags inputFlags = ImGuiInputTextFlags_CallbackCharFilter;
    ImGui::Begin(u8"连接到服务器", &ConnectionWindowStatus);
    ImGui::Text(u8"服务器IP:"); ImGui::SameLine();
    ImGui::InputText("##IPC", ipInputBuffer, sizeof(ipInputBuffer), inputFlags,IsInputValidIPAddress);

    ImGui::Text(u8"端口:"); ImGui::SameLine(); ImGui::InputInt(u8"##端口", &PortBuffer);
    ImGui::Text(u8"密码:"); ImGui::SameLine(); ImGui::InputText(u8"##密码", Passwd,sizeof(Passwd));

    if (ImGui::Button("连接"))
    {
        ConnectionToServer = InitializeConnection(ipInputBuffer, PortBuffer);
        if (ConnectionToServer == INVALID_SOCKET)
        {
            ConnectionFailedPopUP = true;
        }
        else
        {
            int PasswdSendingStatus = SendData(ConnectionToServer, Passwd);
            if (PasswdSendingStatus == -1)
            {
                Disconnect(ConnectionToServer);
                ConnectionFailedPopUP = true;
            }
            else
            {
                std::thread PasswdChecker(CheckPasswd, &ConnectionToServer, &PasswdStatus,&PasswdFalsePopUP);
                PasswdChecker.detach();
            }
        }
    }
    if (PasswdStatus)
    {
        *networkConnectionStatus = true;
        ConnectionWindowStatus = false;
        PasswdStatus = false;
        AskForEnv(&ConnectionToServer, Env, sizeof(Env));
    }
    if (ConnectionFailedPopUP)
    {
        ImGui::OpenPopup(u8"连接错误");
        if (ImGui::BeginPopup(u8"连接错误"))
            ImGui::Text(u8"无法连接到服务器");
            if (ImGui::Button("确定##ConnetionFailed"))
            {
                ImGui::CloseCurrentPopup();
                ConnectionFailedPopUP = false;
            }
            ImGui::EndPopup();
    }
    if (PasswdFalsePopUP)
    {
        ImGui::OpenPopup(u8"密码错误");
        if (ImGui::BeginPopup(u8"密码错误"))
            ImGui::Text(u8"密码错误");
        if (ImGui::Button("确定##PasswdFalse"))
        {
            ImGui::CloseCurrentPopup();
            PasswdFalsePopUP = false;
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}
