waypoint = { x, y, z }
maxVelocity = { x, y, z }
acceleration = { x, y, z }
velocity = { x, y, z }
position = { x, y, z }


 function create()
  --TODO parse any existing queue and remove all of its values
  -- Initialize first commandGroup
  print("Initialized the Queue")
  Queue = newAutotable(3)
  currentCommandGroupCount = 0
  currentCommandCount = 0
  activeCommandGroup = 0

end


function next( )
	-- Next command group! :)
	--activeCommandGroup = activeCommandGroup+1
	currentCommandGroupCount = currentCommandGroupCount +1 -- Stack
	currentCommandCount = 0

end


function pushCommand(...)
 local arg={...}
 printResult = "ComponentID: " .. componentID .. " pushed a command "
 local argNum = 1
  Queue[currentCommandGroupCount][currentCommandCount][0]= true --Active state
 for i,v in ipairs(arg) do
  Queue[currentCommandGroupCount][currentCommandCount][argNum] = v
  argNum = argNum + 1
  printResult = printResult .. " " .. tostring(v)
 end -- End of for i,v in ipairs(arg) do

 print(printResult)
 --for argIndex = 0, #arg, 1 do
 --Queue[currentCommandGroupCount][currentCommandCount] = ...
 --end
 currentCommandCount =  currentCommandCount + 1
end -- End of function Queue.pushCommand(commandName, ...)



function update()
 validID,
 position.x, position.y, position.z, 
 velocity.x, velocity.y, velocity.z,
 acceleration.x, acceleration.y, acceleration.z,
 maxVelocity.x, maxVelocity.y, maxVelocity.z = getComponentState(componentID)
 --Print all commands in current commandGroup
 if currentCommandGroupCount ~= nil then
  --print ("Number of commands in command group " .. #Queue[0] + 1) -- Index starts at 0 so add one
 -- The # operator gets the length of a table
 if(Queue[activeCommandGroup] ~= nil) then
   local hasActive = false
   for command = 0, #Queue[activeCommandGroup], 1 do

	if Queue[activeCommandGroup][command][0] ==  true then -- a command is Active
	hasActive = true
	
	printResult = "Command ".. command .. ": "
	for argu = 1, #Queue[activeCommandGroup][command], 1 do
	 printResult = printResult .. " " .. Queue[activeCommandGroup][command][argu]
	end --end of for arg = 1, #Queue[activeCommandGroup][command], 1 do
	local done = false
	
	
	if Queue[activeCommandGroup][command][1] == "moveTo" then
	 done = moveTo(componentID, Queue[activeCommandGroup][command][2], Queue[activeCommandGroup][command][3], Queue[activeCommandGroup][command][4],
		           Queue[activeCommandGroup][command][5], Queue[activeCommandGroup][command][6], Queue[activeCommandGroup][command][7])
	 if done then
	   --Arrived at destination 
	   Queue[activeCommandGroup][command][0] = false
	   print("Component ", componentID, " arrived at destination ",Queue[activeCommandGroup][command][2], "x ", Queue[activeCommandGroup][command][3], "y ", Queue[activeCommandGroup][command][4], "z " )  
	 end -- end of  if done then
     else -- end of  if Queue[activeCommandGroup][command][1] == "moveTo" then

	if Queue[activeCommandGroup][command][1] == "orientTo" then
	 done = orientTo(componentID, Queue[activeCommandGroup][command][2], Queue[activeCommandGroup][command][3], Queue[activeCommandGroup][command][4],
		           Queue[activeCommandGroup][command][5], Queue[activeCommandGroup][command][6], Queue[activeCommandGroup][command][7])
	 if done then
	   --Oriented to destination 
	   Queue[activeCommandGroup][command][0] = false
	   print("Component ", componentID, " is now oriented to destination ",Queue[activeCommandGroup][command][2], "x ", Queue[activeCommandGroup][command][3], "y ", Queue[activeCommandGroup][command][4], "z " )  
	 end -- end of  if done then
	end -- end of  if Queue[activeCommandGroup][command][1] == "orientTo" then
	end -- end of  if Queue[activeCommandGroup][command][1] == "moveTo" then
	print(printResult.." - Command Active ")
   end
	if not hasActive then
		if(activeCommandGroup <= currentCommandGroupCount) then
	      print("Component ", componentID, " switched to next phase since all commands in commandGroup have completed..")
	      activeCommandGroup = activeCommandGroup+1
	    end
	end
  end -- End of if currentCommandGroupCount ~= nil then
 end
 end

 end

 function newAutotable(dim)
	local MT = {};
	for i=1, dim do
		MT[i] = {__index = function(t, k)
			if i < dim then
				t[k] = setmetatable({}, MT[i+1])
				return t[k];
			end
		end}
	end
	return setmetatable({}, MT[1]);
end