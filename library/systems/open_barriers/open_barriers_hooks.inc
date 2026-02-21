
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	for(new i; i < BARRIERS_COUNT; i++){
		open_barrier_info[i][ob_object_id] = CreateDynamicObject(open_barrier_info[i][ob_model_id], 
			open_barrier_info[i][ob_pos][0], open_barrier_info[i][ob_pos][1], open_barrier_info[i][ob_pos][2],
			open_barrier_info[i][ob_pos][3], open_barrier_info[i][ob_pos][4], open_barrier_info[i][ob_pos][5],
			open_barrier_info[i][ob_vw], open_barrier_info[i][ob_int]);

		open_barrier_info[i][ob_3dtext] = CreateDynamic3DTextLabel(open_barrier_info[i][ob_text], -1, 
			open_barrier_info[i][ob_pos][0], open_barrier_info[i][ob_pos][1], open_barrier_info[i][ob_pos][2], 
			5.0, _, _, _, 
			open_barrier_info[i][ob_vw], open_barrier_info[i][ob_int]);
	
		for(new g; g < BARRIERS_MATERIALS_COUNT; g++){
			if(open_barrier_material_info[g][obm_index] != i) continue;

			SetDynamicObjectMaterial(open_barrier_info[i][ob_object_id], 
				open_barrier_material_info[g][obm_material_index], 
				open_barrier_material_info[g][obm_model_id], 
				open_barrier_material_info[g][obm_lib_name], 
				open_barrier_material_info[g][obm_texture_name], 
				open_barrier_material_info[g][obm_color]);
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK){
		for(new i; i < BARRIERS_COUNT; i++){
			if(IsPlayerInRangeOfPoint(playerid, 3.0, open_barrier_info[i][ob_pos][0], open_barrier_info[i][ob_pos][1], open_barrier_info[i][ob_pos][2])){

				if(GetPVarType(playerid, "OpenBarriers_Cooldown")){
					SendErrorMessage(playerid, "Подождите...");
					return Y_HOOKS_CONTINUE_RETURN_1;
				}

				switch(i){
					case 0:{ // - Дверь в мэрии
						new fraction_index = GetPlayerFractionIndex(playerid);
						if(fraction_index == -1){
							SendErrorMessage(playerid, "У Вас нет доступа к данной двери.");
							return Y_HOOKS_CONTINUE_RETURN_1;
						}
						if(GetFractionData(fraction_index, f_type) != FRAC_TYPE_GOVERMENT
							&& GetFractionData(fraction_index, f_type) != FRAC_TYPE_POLICE
							&& GetFractionData(fraction_index, f_type) != FRAC_TYPE_MEDIC){

							SendErrorMessage(playerid, "У Вас нет доступа к данной двери.");
							return Y_HOOKS_CONTINUE_RETURN_1;
						}
					}
				}

				if(open_barrier_info[i][ob_status] == OB_STATUS_CLOSE){
					MoveDynamicObject(open_barrier_info[i][ob_object_id], 
						open_barrier_info[i][ob_open_pos][0], open_barrier_info[i][ob_open_pos][1], open_barrier_info[i][ob_open_pos][2], 
						0.12, 
						open_barrier_info[i][ob_open_pos][3], open_barrier_info[i][ob_open_pos][4], open_barrier_info[i][ob_open_pos][5]);

					open_barrier_info[i][ob_status] = OB_STATUS_OPEN;
				}
				else{
					MoveDynamicObject(open_barrier_info[i][ob_object_id], 
						open_barrier_info[i][ob_pos][0], open_barrier_info[i][ob_pos][1], open_barrier_info[i][ob_pos][2], 
						0.12, 
						open_barrier_info[i][ob_pos][3], open_barrier_info[i][ob_pos][4], open_barrier_info[i][ob_pos][5]);

					open_barrier_info[i][ob_status] = OB_STATUS_CLOSE;
				}
				SetPVarInt(playerid, "OpenBarriers_Cooldown", 2);

				#warning DEBUG
				SendClientMessage(playerid, COLOR_GREEN, "Дверь открыта.");
				break;

			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnPlayerSecondUpdate(playerid){
	if(GetPVarType(playerid, "OpenBarriers_Cooldown") && GetPVarInt(playerid, "OpenBarriers_Cooldown") > 0){
		SetPVarInt(playerid, "OpenBarriers_Cooldown", GetPVarInt(playerid, "OpenBarriers_Cooldown") - 1);
		if(GetPVarInt(playerid, "OpenBarriers_Cooldown") <= 0){
			DeletePVar(playerid, "OpenBarriers_Cooldown");
		}
	}
	return continue(playerid);
}