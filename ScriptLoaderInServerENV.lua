package.path  = package.path..";.\\LuaSocket\\?.lua;"
package.cpath = package.cpath..";.\\LuaSocket\\?.dll;"
net.log("**************************************************************************")
net.log(lfs.currentdir())
--LoopTimes = 1
local socket = require("socket")
local coroutine = require("coroutine")
local DynamicLoaderCallbacks = {}
-- 创建服务器套接字
local server = socket.bind("0.0.0.0", 59527) 
server:settimeout(0) 

-- 存储已连接的客户端
local scriptLoader = {}
scriptLoader.RefreshRate = 1.0
scriptLoader.PasswdTimeOut = 5
scriptLoader.Passwd = "995277" --服务器脚本可以内置密码
scriptLoader.clients = {}
scriptLoader.clientsWithoutPsswd = {}
scriptLoader.clientsWithoutPsswdInitTime = {}
scriptLoader.Enviorment = "Server"
--scriptLoader.Enviorment = "mission"

-- 主循循环，等待连接请求

local receiver = coroutine.create(function()
	while true do
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
			net.log(scriptLoader.Passwd)--debug
			if anotherDATA == scriptLoader.Passwd then
				client:send("Connected")
				table.insert(scriptLoader.clients, client)
				table.remove(scriptLoader.clientsWithoutPsswd, i)
				table.remove(scriptLoader.clientsWithoutPsswdInitTime, i)
				net.log("Connected")
			elseif anotherDATA ~= nil then
				net.log("Disconnect1")--debug
				client:send("PasswdError")
				client:close()
				table.remove(scriptLoader.clientsWithoutPsswd, i)
				table.remove(scriptLoader.clientsWithoutPsswdInitTime, i)
			elseif data then
				client:send("PasswdError")
				net.log("Disconnect2")--debug
				client:close()
				table.remove(scriptLoader.clientsWithoutPsswd, i)
				table.remove(scriptLoader.clientsWithoutPsswdInitTime, i)
			elseif DCS.getModelTime() - scriptLoader.clientsWithoutPsswdInitTime[i] > scriptLoader.PasswdTimeOut then
				net.log("TimeOut:"..scriptLoader.clientsWithoutPsswdInitTime[i]+scriptLoader.PasswdTimeOut)
				net.log("CurrentTime:"..DCS.getModelTime())
				net.log("Disconnect3")--debug
				client:close()
				table.remove(scriptLoader.clientsWithoutPsswd, i)
				table.remove(scriptLoader.clientsWithoutPsswdInitTime, i)
			end
		end
		coroutine.yield()
	end
end)

local LastTime = DCS.getModelTime()

function DynamicLoaderCallbacks.onSimulationFrame()
    -- 检查是否有新的客户端连接
	if LastTime + scriptLoader.RefreshRate < DCS.getModelTime() then
		--net.log(LoopTimes)
		--LoopTimes = LoopTimes + 1
        LastTime = DCS.getModelTime()
		local client = server:accept()
		if client then
			client:settimeout(0)
			table.insert(scriptLoader.clientsWithoutPsswd, client)
			table.insert(scriptLoader.clientsWithoutPsswdInitTime, DCS.getModelTime())--ServerENV
		end
		
		coroutine.resume(receiver)
	end

end

DCS.setUserCallbacks(DynamicLoaderCallbacks)