
stock SpawnSelect:ShowUI(playerid){
	SelectTextDraw(playerid, -1);
	cef_emit_event(playerid,
		"showSpawnSelector");

	new is_house_blocked = true,
	 	is_last_pos_blocked = true,
	 	is_fraction_blocked = true;

	for(new i; i < MAX_HOUSES; i++){
		if(!GetHouseData(i, h_id)) continue;
		if(GetHouseData(i, h_owner_id) != GetPlayerData(playerid, p_id)) continue;

		is_house_blocked = false;
		break;
	}

	if(GetPlayerData(playerid, p_tenant_house_id) != 0){
		is_house_blocked = false;
	}

	if(GetPlayerData(playerid, p_spawn_x) != 0.0 && GetPlayerData(playerid, p_spawn_y) != 0.0 && GetPlayerData(playerid, p_spawn_z) != 0.0){
		is_last_pos_blocked = false;
	}

	if(GetPlayerData(playerid, p_fraction) != 0 && GetPlayerFractionIndex(playerid) != -1){
		new fraction_index = GetPlayerFractionIndex(playerid);
		if(GetFractionData(fraction_index, f_spawn_x) != 0.0 || GetFractionData(fraction_index, f_spawn_y) != 0.0
			|| GetFractionData(fraction_index, f_spawn_z) != 0.0){

			is_fraction_blocked = false;
		}
	}

	cef_emit_event(playerid, "setSpawnSelectorData",
		CEFINT(0),
		CEFINT(is_house_blocked),
		CEFINT(is_last_pos_blocked),
		CEFINT(is_fraction_blocked));
	return true;
}

stock SpawnSelect:SetPlayerSpawn(playerid, spawn_type){
	
	new spawn_vw, spawn_int;
	switch(spawn_type){
		case SPAWN_TYPE_STANDART:{
			new spawn_pos_index = random(sizeof(standart_spawn_pos));

			SetSpawnInfo(playerid, 0, GetPlayerData(playerid, p_skin), 
				standart_spawn_pos[spawn_pos_index][0], 
				standart_spawn_pos[spawn_pos_index][1], 
				standart_spawn_pos[spawn_pos_index][2], 
				standart_spawn_pos[spawn_pos_index][3], 
				0, 0, 0, 0, 0, 0);
			
			spawn_vw = GAME_VW;
			spawn_int = GAME_INT;
		}

		case SPAWN_TYPE_HOUSE:{
 			new house_index = -1;
 			if(GetPlayerData(playerid, p_tenant_house_id) != 0){
 				house_index = House:GetHouseIndexByID(GetPlayerData(playerid, p_tenant_house_id));
 			}
 			else{
 				for(new i; i < MAX_HOUSES; i++){
 					if(!GetHouseData(i, h_id)) continue;
 					if(GetHouseData(i, h_owner_id) != GetPlayerData(playerid, p_id)) continue;

 					house_index = i;
 					break;
 				}
 			}

 			if(house_index == -1){
 				SpawnSelect:SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
 				return false;
 			}

 			SetSpawnInfo(playerid, 0, GetPlayerData(playerid, p_skin),
 				house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][0],
 				house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][1],
 				house_interior_info[GetHouseData(house_index, h_interior_index)][h_enter_pos][2],
 				0.0,
 				0, 0, 0, 0, 0, 0);

 			spawn_vw = GetHouseData(house_index, h_id);
 			spawn_int = GetHouseInteriorData(GetHouseData(house_index, h_interior_index), h_int_int);
		}

		case SPAWN_TYPE_EXIT:{
			if(GetPlayerData(playerid, p_spawn_x) == 0.0 && GetPlayerData(playerid, p_spawn_y) == 0.0 && GetPlayerData(playerid, p_spawn_z) == 0.0){
				SpawnSelect:SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
				return false;
			}

			SetSpawnInfo(playerid, 0, GetPlayerData(playerid, p_skin),
 				GetPlayerData(playerid, p_spawn_x),
 				GetPlayerData(playerid, p_spawn_y),
 				GetPlayerData(playerid, p_spawn_z),
 				GetPlayerData(playerid, p_spawn_angle),
 				0, 0, 0, 0, 0, 0);

 			spawn_vw = GetPlayerData(playerid, p_spawn_vw);
 			spawn_int = GetPlayerData(playerid, p_spawn_int);
		}

		case SPAWN_TYPE_FRACTION:{
			new fraction_index = GetPlayerFractionIndex(playerid);
			if(fraction_index == -1){
				SpawnSelect:SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
				return false;
			}
			if(GetFractionData(fraction_index, f_spawn_x) == 0.0 && GetFractionData(fraction_index, f_spawn_y) == 0.0 && GetFractionData(fraction_index, f_spawn_z) == 0.0){
				SpawnSelect:SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
				return false;
			}

			new skin_id = GetPlayerData(playerid, p_skin);
			if(Fraction:GetPlayerSkinIndex(playerid) != -1){
				skin_id = GetFractionSkinData(Fraction:GetPlayerSkinIndex(playerid), fs_skin_id);
			}

			SetSpawnInfo(playerid, 0, skin_id,
 				GetFractionData(fraction_index, f_spawn_x),
 				GetFractionData(fraction_index, f_spawn_y),
 				GetFractionData(fraction_index, f_spawn_z),
 				GetFractionData(fraction_index, f_spawn_angle),
 				0, 0, 0, 0, 0, 0);

 			spawn_vw = GetFractionData(fraction_index, f_spawn_vw);
 			spawn_int = GetFractionData(fraction_index, f_spawn_int);
		
 			SetPlayerData(playerid, p_fraction_duty, true);
		}
	}

	ResetPlayerMoney(playerid);
	GivePlayerMoney(playerid, GetPlayerData(playerid, p_money));

	SetPlayerScore(playerid, GetPlayerData(playerid, p_level));
	SetPlayerHealth(playerid, GetPlayerData(playerid, p_health));
	SetPlayerDrunkLevel(playerid, GetPlayerData(playerid, p_drunked_level));
	SetPlayerWantedLevel(playerid, GetPlayerData(playerid, p_wanted));

	CheckPlayerLicenses(playerid);

	player_logged[playerid] = true;
	SpawnPlayer(playerid);

	SetPlayerVirtualWorld(playerid, spawn_vw);
	SetPlayerInterior(playerid, spawn_int);
	SetCameraBehindPlayer(playerid);
	TogglePlayerControllable(playerid, true);

	GivePlayerAllWeapons(playerid);
	SetPlayerColor(playerid, GetPlayerColorEx(playerid));

	CheckPlayerNotifications(playerid);
	return true;
}