player = {
	name = "PesBarbos",
	age = 47,
	alltime = 0.0
}

function player:OnInit( data )
	Console:PrintLine("Player OnInit");
	Console:PrintLine("Username: " .. self.name)

	--Console:Show(true)
	Game:gotoMenu(true)

	local formats = System:EnumDisplayFormats()
	--for i=1, #formats do
	--	Console:PrintLine("["..i.."]".. formats[i].width .. " x " .. formats[i].height .. " x " .. formats[i].bpp)
	--end

	Game:CreateVariable("game_inited", 1)
end

function player:PostInit( )
	Game:LoadLevel("test")
	setvar("gravity", 200)
	setvar("floor", 20)
	System:ShowConsole(false)
	Game:gotoMenu()
end


function player:TestChanges(string)
	Console:PrintLine("TestChanges"):PrintLine("call chain"..string)
	Console:PrintLine("Username: " .. self.name)
	

	return Console;
end

function player:Update( dt )
	self.alltime = self.alltime + dt;
end



function test(  )
	player:OnInit(123)
end

test()


