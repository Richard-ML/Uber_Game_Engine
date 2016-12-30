--dofile("BaseController.lua") 
curTargetX, curTargetY, curTargetZ = wander(componentID, curTargetX, curTargetY, curTargetZ)

inSight, tarX, tarY, tarZ =  hunt(componentID, -2) -- -2 is the player 
if(inSight) then
curTargetX = tarX
curTargetY = tarY
curTargetZ = tarZ
end
moveTo(componentID, curTargetX, curTargetY, curTargetZ, 30.0, 30.0, 30.0)
--orientTo(componentID, curTargetX, curTargetY, curTargetZ, 0.01, 0.01, 0.01) -- TODO: FIX THIS
if once == nil then
 --create() -- Create queue
 --pushCommand("MoveTo", 21060, 0, 52000, 3, 3, 3)
 --once = true
end
