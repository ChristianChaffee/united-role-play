
CMD:savegarage(playerid){
	if(!player_logged[playerid]) return false;
	if(!GetPVarType(playerid, "House_IsCreateGarage")) return SendErrorMessage(playerid, "Вы не создаёте гараж.");

	new house_index = GetPVarInt(playerid, "House_IsCreateGarage");
	DeletePVar(playerid, "House_IsCreateGarage");

	if(GetHouseData(house_index, h_owner_id) != GetPlayerData(playerid, p_id)){
		SendErrorMessage(playerid, "Дом не принадлежит Вам.");
		return false;
	}
	if(!IsPlayerInRangeOfPoint(playerid, 100.0, GetHouseData(house_index, h_pos_x), GetHouseData(house_index, h_pos_y), GetHouseData(house_index, h_pos_z))){
		SendErrorMessage(playerid, "Вы слишком далеко отошли от дома.");
		return false;
	}

	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	static const fmt_query[] = "INSERT INTO garages (house_id, pos_x, pos_y, pos_z) VALUES (%d, %f, %f, %f)";
	new query[sizeof(fmt_query) + ((-2 + 11) * 4)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetHouseData(house_index, h_id), pos[0], pos[1], pos[2]);
	mysql_tquery(mysql, query, "LoadCreateGarage", "i", playerid);
	return true;
}