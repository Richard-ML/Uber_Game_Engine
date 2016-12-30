--dofile("BaseController.lua") 
--MoveTo ( waypoint( x, y, z ), maxVelocity( x, y, z ) or timeInSeconds )
-- This spider's initial position is offsetX="21859.0f" offsetY="0.0f" offsetZ="51979.0f"

local waypoint = { x, y, z }
local maxVelocity = { x, y, z }
local acceleration = { x, y, z }
local velocity = { x, y, z }
local position = { x, y, z }
--if once == nil then
-- create() -- Create queue
-- addLocationTrigger(21060, 0.0, 52000, 1)
-- --Command group 1 here.. 
-- pushCommand("moveTo", 21060, 0.0, 52000, 3, 3, 3) -- Move to the right.. 
-- pushCommand("orientTo", 21060, 0, 52000, 0.01, 0.01, 0.01)
-- next()
-- --Command group 2 here.. 
-- pushCommand("moveTo", 21860, 0.0, 51980, 3, 3, 3)
-- pushCommand("orientTo", 21860, 0, 51980, 0.01, 0.01, 0.01)
--   next()
-- --Command group 3 here.. 
--
--  pushCommand("moveTo", 21860, 0.0, 52500, 3, 3, 3)
--  pushCommand("orientTo", 21860, 0, 52500, 0.01, 0.01, 0.01)
-- once = true
--end
-- update() -- Runs commands in active commandGroup
--end