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
scriptLoader.RefreshRate = 1.0
scriptLoader.PasswdTimeOut = 5
--scriptLoader.Passwd = "995277" --不建议将密码写在任务脚本里
gpasswd = ""
dofile(lfs.currentdir().."ScriptLoaderPasswd\\passwd.lua") --请把ScriptLoaderPasswd文件夹放到游戏根目录里
scriptLoader.Passwd = gpasswd
gpasswd = nil
scriptLoader.clients = {}
scriptLoader.clientsWithoutPsswd = {}
scriptLoader.clientsWithoutPsswdInitTime = {}
scriptLoader.Enviorment = "Mission"

-- 主循循环，等待连接请求

local receiver = coroutine.create(function()
	while true do
		--trigger.action.outText("Try RecieveDATAO",5,false)
		for i, client in ipairs(scriptLoader.clients) do
			--trigger.action.outText("Try RecieveDATA",5,false)
			local data, err ,anotherDATA = client:receive(1024*4)
			if (data or anotherDATA) then
				local moduleId = ""
				local message = ""
				if (data) then
					--trigger.action.outText(data,5,false)
					moduleId, message = string.match(data, "^(.-) (.+)$")
					while true do
						local data1, err1, anotherDATA1 = client:receive(1024*4)
						if (anotherDATA1) then
							message = message..anotherDATA1
							break
						elseif data1 then
							message = message..data1
						else
							break
						end
					end
				elseif(anotherDATA) then
					if (anotherDATA ~= "") then
						--trigger.action.outText(anotherDATA,5,false)
						--net.log("anotherDATA:"..anotherDATA)
						moduleId, message = string.match(anotherDATA, "^(.-) (.+)$")
					end
				end
				if (moduleId and message) or anotherDATA~="" then
					--net.log("moduleId"..moduleId)
					--net.log("message"..message)
					if (moduleId == "1") then
						local MesFunc = loadstring(message)
						if MesFunc then
							pcall(MesFunc)
						end
					elseif (moduleId == "0") then
						client:send(scriptLoader.Enviorment)
					end
				end
			end
			if err == "closed" then
				-- 客户端关闭连接
				table.remove(scriptLoader.clients, i)
			end
		end
		for i, client in ipairs(scriptLoader.clientsWithoutPsswd) do
			local data, err ,anotherDATA= client:receive(100)
			if anotherDATA == scriptLoader.Passwd then
				client:send("Connected")
				--trigger.action.outText("Connected",5,false)
				table.insert(scriptLoader.clients, client)
				--trigger.action.outText("Connected1",5,false)
				table.remove(scriptLoader.clientsWithoutPsswd, i)
				--trigger.action.outText("Connected2",5,false)
				table.remove(scriptLoader.clientsWithoutPsswdInitTime, i)
				--trigger.action.outText("Connected3",5,false)
			elseif anotherDATA ~= nil then
				--trigger.action.outText("disconnect1",5,false)
				client:send("PasswdError")
				client:close()
				table.remove(scriptLoader.clientsWithoutPsswd, i)
				table.remove(scriptLoader.clientsWithoutPsswdInitTime, i)
			elseif data then
				--trigger.action.outText("disconnect2",5,false)
				client:send("PasswdError")
				client:close()
				table.remove(scriptLoader.clientsWithoutPsswd, i)
				table.remove(scriptLoader.clientsWithoutPsswdInitTime, i)
			elseif timer.getTime() - scriptLoader.clientsWithoutPsswdInitTime[i] > scriptLoader.PasswdTimeOut then
				--trigger.action.outText("disconnect3",5,false)
				client:close()
				table.remove(scriptLoader.clientsWithoutPsswd, i)
				table.remove(scriptLoader.clientsWithoutPsswdInitTime, i)
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
    timer.scheduleFunction(mainLoop, nil, timer.getTime() + scriptLoader.RefreshRate)  -- 1秒钟后再次调用主循环
end

-- 启动主循环
mainLoop()
