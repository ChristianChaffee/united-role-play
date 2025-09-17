
stock SetPlayerSpawn(playerid, spawn_type){
	
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
	}

	player_logged[playerid] = true;
	SpawnPlayer(playerid);

	SetPlayerVirtualWorld(playerid, spawn_vw);
	SetPlayerInterior(playerid, spawn_int);
	SetCameraBehindPlayer(playerid);
	TogglePlayerControllable(playerid, true);
	return true;
}