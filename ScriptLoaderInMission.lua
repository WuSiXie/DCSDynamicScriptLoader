package.path  = package.path..";.\\LuaSocket\\?.lua;"
package.cpath = package.cpath..";.\\LuaSocket\\?.dll;"


local socket = require("socket")
local coroutine = require("coroutine")
local lfs = require("lfs")

-- 创建服务器套接字
local server = socket.bind("0.0.0.0", 49527)  -- 在所有网络接口上监听端口 49527
server:settimeout(0)  -- 设置为非阻塞模式

-- 存储已连接的客户端
local scriptLoader = {}
scriptLoader.PasswdTimeOut = 5
--scriptLoader.Passwd = "995277" --不建议将密码写在任务脚本里
gpasswd = ""
dofile(lfs.currentdir().."ScriptLoaderPasswd\\passwd.lua") --请把ScriptLoaderPasswd文件夹放到游戏根目录里
scriptLoader.Passwd = gpasswd
gpasswd = nil
scriptLoader.clients = {}
scriptLoader.clientsWithoutPsswd = {}
scriptLoader.clientsWithoutPsswdInitTime = {}

-- 主循循环，等待连接请求

local receiver = coroutine.create(function()
	while true do
		for i, client in ipairs(scriptLoader.clients) do
			local data, err ,anotherDATA= client:receive(1024*4)
			if (data) then
				trigger.action.outText(data,5,false)
				local moduleId, message = string.match(data, "^(.-) (.+)$")
				if moduleId and message then
					--trigger.action.outText(moduleId,5,false)
					--trigger.action.outText(message,5,false)
					if(moduleId == "1") then
						local MesFunc = loadstring(message)
						if MesFunc then
							pcall(MesFunc)
						end
					end
				else
					trigger.action.outText("数据错误",5,false)
				end

			elseif(anotherDATA) then
				if (anotherDATA~="") then
					--trigger.action.outText(anotherDATA,5,false)
					
					local moduleId, message = string.match(anotherDATA, "^(.-) (.+)$")
					if moduleId and message then
						--trigger.action.outText(moduleId,5,false)
						--trigger.action.outText(message,5,false)
						if (moduleId == "1") then
							local MesFunc = loadstring(message)
							if MesFunc then
								pcall(MesFunc)
							end
						end
					else
						trigger.action.outText("数据错误",5,false)
					end
					
					
				end
			else
				trigger.action.outText(err,5,true)
			end
			if data then
				-- 处理接收到的数据
			elseif err == "closed" then
				-- 客户端关闭连接
				table.remove(scriptLoader.clients, i)
			end
		end
		for i, client in ipairs(scriptLoader.clientsWithoutPsswd) do
			local data, err ,anotherDATA= client:receive(100)
			if anotherDATA == scriptLoader.Passwd then
				client:send("Connected")
				table.insert(scriptLoader.clients, client)
				table.remove(scriptLoader.clientsWithoutPsswd, i)
			elseif anotherDATA ~= nil then
				client:send("PasswdError")
				client:close()
				table.remove(scriptLoader.clientsWithoutPsswd, i)
			elseif data then
				client:send("PasswdError")
				client:close()
				table.remove(scriptLoader.clientsWithoutPsswd, i)
			end
			if scriptLoader.clientsWithoutPsswdInitTime[i]+scriptLoader.PasswdTimeOut >timer.getTime() then
				client:close()
				table.remove(scriptLoader.clientsWithoutPsswd, i)
			end
		end
		coroutine.yield()
	end
end)

function mainLoop()
    -- 检查是否有新的客户端连接
    local client = server:accept()
    if client then
		client:settimeout(0)
        table.insert(scriptLoader.clientsWithoutPsswd, client)
        table.insert(scriptLoader.clientsWithoutPsswdInitTime, timer.getTime())--MissionENV
    end
	
	coroutine.resume(receiver)

    -- 调度下一次主循循环
    timer.scheduleFunction(mainLoop, nil, timer.getTime() + 1.0)  -- 1秒钟后再次调用主循环
end

-- 启动主循环
mainLoop()
