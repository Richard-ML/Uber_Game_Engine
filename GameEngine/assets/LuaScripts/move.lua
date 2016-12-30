r, x, y, z, vx, vy, vz = getComponentState(ObjectID)
if ( r == true ) then
    print( "xyz=", x, ",", y, ", ", z )
end 
 x = x - 10.0
 print("WHAT THE FUCkkkkk")
setComponentState(ObjectID, x, y, z, Vx, Vy, Vz )
