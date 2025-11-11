
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	for(new i; i < CAR_SHOWROOMS_COUNT; i++){
		if(!CarShowroom:GetOnSaleCount(i)){
			for(new g, j = CarShowroom:GetMaxSlots(i); g < j; g++){
				CarShowroom:AddRandomVehicle(i);
			}
		}

		big_string[0] = EOS;
		format(big_string, sizeof(big_string),
			""cl_lime"%s\n\
			"cl_grey"Всего машин на продаже: "cl_white"%d шт",
			car_showroom_info[i][cs_name], CarShowroom:GetOnSaleCount(i));

		car_showroom_info[i][cs_text] = CreateDynamic3DTextLabel(big_string, -1, 
			car_showroom_info[i][cs_pos][0], 
			car_showroom_info[i][cs_pos][1], 
			car_showroom_info[i][cs_pos][2], 
			25.0, _, _, 1, 0, 0);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate){
	if(newstate == PLAYER_STATE_DRIVER && GetVehicleData(GetPlayerVehicleID(playerid), v_status) == VEHICLE_STATUS_CAR_SHOWROOM){
		CarShowroom:ShowVehicleInfo(playerid, CarShowroom:GetVehicleShowroom(GetPlayerVehicleID(playerid)));
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}