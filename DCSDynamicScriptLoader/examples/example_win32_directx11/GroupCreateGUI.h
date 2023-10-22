#pragma once
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include <sstream>
#include <vector>
#include "networkmoduel.h"
class pointTasks
{
    char* otherPointTasks = new char[1024]();
};
class points
{
    pointTasks pointTasksForPoints;
};
class singlePylon
{
    singlePylon* PrevPylon = nullptr;
    char* WeaponType = new char[20]();
    singlePylon* NextPylon = nullptr;
};
class unitPayload
{
    singlePylon unitPayloads;
    int fuel = 5000;
    int flare = 10;
    int chaff = 10;
    int gun = 100;
};
class unit
{
    unit* Prevunit = nullptr;
    char* alt = new char[10] {"2000"};
    char* alt_type = new char[10] {"BARO"};
    char* livery_id = new char[30]();
    char* skill = new char[10] {"client"};
    float speed = 300.0f;
    char* type = new char[10]();
    char* onboard_num = new char[10]();
    bool useLL = false;
    int y = 0;
    int x = 0;
    char* name = new char[30]();
    int heading = 0;
    char* callsign = new char[30]();
    unitPayload unitPayloadsForunit;
    unit* Nextunit = nullptr;
};
class tasksForGroup
{
    tasksForGroup* PrevTask = nullptr;
    char* taskContent = new char[1024]();
    tasksForGroup* NextTask = nullptr;
};
class AirPlaneGroupInfo
{
public:
    char* country = new char[10]();
    char* frequency = new char[10] {"305"};
    char* modulation = new char[2] {"0"};
    char* OtherParams = new char[1024]();
    const int unitNumbLimit = 4;
    tasksForGroup tasksForAirGroup;
    points pointsForAirGroup;
    unit unitsForAirGroup;
};

void CreateGroup()
{
    static bool CreateGroupON = false;
    ImGui::Checkbox(u8"�̶���Ⱥ�鴴����", &CreateGroupON);
    ImGui::Begin(u8"�̶���Ⱥ�鴴����##Window", &CreateGroupON);
    static AirPlaneGroupInfo GroupToCreate;
    ImGui::Text(u8"Ⱥ�����������ţ�"); ImGui::SameLine; ImGui::InputText("##GroupCountry:", GroupToCreate.country,10);
    ImGui::Text(u8"Ⱥ��Ƶ�ʣ�"); ImGui::SameLine; ImGui::InputText("##GroupCountry:", GroupToCreate.frequency,10);
    ImGui::Text(u8"modulation(��֪������0)��"); ImGui::SameLine; ImGui::InputText("##GroupCountry:", GroupToCreate.modulation,2);
    static bool OtherParams = false;
    ImGui::Checkbox(u8"Ⱥ����������", &OtherParams);
    if (OtherParams)
    {
        ImGui::Text(u8"����Ⱥ�����##otherparams");
        ImGui::InputTextMultiline("##OtherParams", GroupToCreate.OtherParams, 1024);
    }
    ImGui::End();
}
