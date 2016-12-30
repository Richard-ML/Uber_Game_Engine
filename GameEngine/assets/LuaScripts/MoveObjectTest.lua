r, x, y, z, vx, vy, vz = getObjectState(ObjectID)
if ( r == true ) then
    print( "xyz=", x, ",", y, ", ", z )
end 
 x = x + 0.01
setObjectState(ObjectID, x, y, z, Vx, Vy, Vz )
