
forward spawnSelectorSelect(playerid, const arguments[]);
public spawnSelectorSelect(playerid, const arguments[]){
	new selected_spawn_index;
	sscanf(arguments, "i", selected_spawn_index);

	switch(selected_spawn_index){
		case 0: SpawnSelect:SetPlayerSpawn(playerid, SPAWN_TYPE_STANDART);
		case 1: SpawnSelect:SetPlayerSpawn(playerid, SPAWN_TYPE_HOUSE);
		case 2: SpawnSelect:SetPlayerSpawn(playerid, SPAWN_TYPE_EXIT);
		case 3: SpawnSelect:SetPlayerSpawn(playerid, SPAWN_TYPE_FRACTION);
	}

	cef_emit_event(playerid, "hideSpawnSelector");
	CancelSelectTextDraw(playerid);
	return true;
}