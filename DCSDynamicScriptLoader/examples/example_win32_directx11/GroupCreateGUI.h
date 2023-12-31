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
public:
    pointTasks* PrevPointTasks = nullptr;
    char* PointTasks = new char[1024]();
    pointTasks* NextPointTasks = nullptr;
};
class points
{
public:
    pointTasks pointTasksForPoints;
};
class singlePylon
{
public:
    singlePylon* PrevPylon = nullptr;
    char* WeaponType = new char[20]();
    singlePylon* NextPylon = nullptr;
};
class unitPayload
{
public:
    singlePylon unitPayloads;
    int fuel = 5000;
    int flare = 10;
    int chaff = 10;
    int gun = 100;
};
class unit
{
public:
    unit* Prevunit = nullptr;
    char* alt = new char[10] {"2000"};
    char* alt_type = new char[10] {"BARO"};
    char* livery_id = new char[30]();
    char* skill = new char[10] {"client"};
    float speed = 300.0f;
    char* type = new char[10]();
    char* onboard_num = new char[10]();
    bool useLL = false;
    double y = 0;
    double x = 0;
    char* name = new char[30]();
    int heading = 0;
    char* callsign = new char[30]();
    unitPayload unitPayloadsForunit;
    unit* Nextunit = nullptr;
};
class tasksForGroup
{
public:
    tasksForGroup* PrevTask = nullptr;
    char* taskContent = new char[1024]();
    tasksForGroup* NextTask = nullptr;
};
class AirPlaneGroupInfo
{
public:
    char* country = new char[10]();
    char* frequency = new char[10] {"305"};
    char* modulation = new char[10] {"0"};
    char* OtherParams = new char[1024]();
    const int unitNumbLimit = 4;
    tasksForGroup tasksForAirGroup;
    points pointsForAirGroup;
    unit unitsForAirGroup;
};

void CreateGroup()
{
    static bool CreateGroupON = false;
    ImGui::Checkbox(u8"固定翼群组创建器", &CreateGroupON);
    ImGui::Begin(u8"固定翼群组创建器##Window", &CreateGroupON);
    static AirPlaneGroupInfo GroupToCreate;
    ImGui::Text(u8"群组所属国代号:"); ImGui::SameLine(); ImGui::InputText("##GroupCountry:", GroupToCreate.country, 10);
    ImGui::Text(u8"群组频率:"); ImGui::SameLine(); ImGui::InputText("##GroupCountry:", GroupToCreate.frequency, 10);
    ImGui::Text(u8"modulation(不知道请填0):"); ImGui::SameLine(); ImGui::InputText("##GroupCountry:", GroupToCreate.modulation, 10);
    static bool OtherParams = false;
    ImGui::Checkbox(u8"输入群组其他参数", &OtherParams);
    if (OtherParams)
    {
        ImGui::Text(u8"其他群组参数##otherparams");
        ImGui::InputTextMultiline("##OtherParams", GroupToCreate.OtherParams, 1024);
    }
    ImGui::Text(u8"群组任务类型:"); ImGui::SameLine(); ImGui::InputText("##GroupTaskk:", GroupToCreate.task, 10);
    ImGui::Checkbox("仅限管理员##AirGroupAdminOnly", &GroupToCreate.unctrolled);
    ImGui::End();
}
