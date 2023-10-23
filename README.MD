基于Dear Imgui的图形化动态脚本加载器  
将ScriptLoaderInMission.lua加载至任务环境后可以实时通过该程序添加新脚本  
在加载到任务中前需要手动注释DCS根目录下\script\MissionScripting.lua部分的do end  
如  
--[[  
do  
	sanitizeModule('os')  
	sanitizeModule('io')  
	sanitizeModule('lfs')  
	_G['require'] = nil  
	_G['loadlib'] = nil  
	_G['package'] = nil  
end  
--]]  
如果想要加载到服务器环境中则不需要注释MissionScripting.lua也不需要将ScriptLoaderInMission.lua加载至任务环境  
只需将ScriptLoaderInServerENV.lua放到保存的游戏\DCS\scripts\Hooks中  
[Dear ImGui]:https://github.com/ocornut/imgui
