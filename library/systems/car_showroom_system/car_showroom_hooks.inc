
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	for(new i; i < CAR_SHOWROOMS_COUNT; i++){
		if(!CarShowroom:GetOnSaleCount(i)){
			for(new g, j = CarShowroom:GetMaxSlots(i); g < j; g++){
				CarShowroom:AddRandomVehicle(i);
			}
		}

		CarShowroom:UpdateText(i);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerStateChange(playerid, newstate, oldstate){
	if(newstate == PLAYER_STATE_DRIVER && GetVehicleData(GetPlayerVehicleID(playerid), v_status) == VEHICLE_STATUS_CAR_SHOWROOM){
		CarShowroom:ShowVehicleInfo(playerid, CarShowroom:GetVehicleShowroom(GetPlayerVehicleID(playerid)));
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}