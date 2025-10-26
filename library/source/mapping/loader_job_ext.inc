
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	new tmpobjid, object_world = -1, object_int = -1;
	#pragma unused tmpobjid
	tmpobjid = CreateDynamicObject(970, 2202.437011, -2286.605468, 14.264667, 0.000000, 0.000000, 44.900020, object_world, object_int, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(970, 2205.355712, -2283.697753, 14.264667, 0.000000, 0.000000, 44.900020, object_world, object_int, -1, 300.00, 300.00); 
	tmpobjid = CreateDynamicObject(970, 2205.365966, -2280.777343, 14.264667, 0.000000, 0.000000, 134.900024, object_world, object_int, -1, 300.00, 300.00); 
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid){
	RemoveBuildingForPlayer(playerid, 3574, 2193.260, -2286.290, 14.812, 0.250);
	RemoveBuildingForPlayer(playerid, 3744, 2193.260, -2286.290, 14.812, 0.250);
	RemoveBuildingForPlayer(playerid, 3569, 2204.629, -2274.409, 14.937, 0.250);
	RemoveBuildingForPlayer(playerid, 3747, 2204.629, -2274.409, 14.937, 0.250);
	RemoveBuildingForPlayer(playerid, 3569, 2212.090, -2267.070, 14.937, 0.250);
	RemoveBuildingForPlayer(playerid, 3747, 2212.090, -2267.070, 14.937, 0.250);
	RemoveBuildingForPlayer(playerid, 3627, 2220.780, -2261.050, 15.906, 0.250);
	RemoveBuildingForPlayer(playerid, 3686, 2220.780, -2261.050, 15.906, 0.250);
	RemoveBuildingForPlayer(playerid, 3569, 2219.419, -2259.520, 14.882, 0.250);
	RemoveBuildingForPlayer(playerid, 3747, 2219.419, -2259.520, 14.882, 0.250);
	RemoveBuildingForPlayer(playerid, 3569, 2226.969, -2252.139, 14.937, 0.250);
	RemoveBuildingForPlayer(playerid, 3747, 2226.969, -2252.139, 14.937, 0.250);
	RemoveBuildingForPlayer(playerid, 3569, 2234.389, -2244.830, 14.937, 0.250);
	RemoveBuildingForPlayer(playerid, 3747, 2234.389, -2244.830, 14.937, 0.250);
	return Y_HOOKS_CONTINUE_RETURN_1;
}