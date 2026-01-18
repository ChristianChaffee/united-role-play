
CMD:alogin(playerid){
	if(!player_logged[playerid]) return false;
	if(!GetPlayerAdminData(playerid, pa_id)) return false;

	if(GetPlayerAdminData(playerid, pa_logged)){
		new temp_ip[32];
		GetPlayerIp(playerid, temp_ip, sizeof(temp_ip));

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"[A] Администратор %d-го уровня %s[%d] вышел из админ-панели (IP: %s).",
			GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid, temp_ip);
		Admin:SendAdminMessage(COLOR_GREY, small_string);
		small_string[0] = EOS;

		SetPlayerAdminData(playerid, pa_logged, false);
	}
	else{
		static const fmt_query[] = "SELECT id FROM administration WHERE password IS NULL AND id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerAdminData(playerid, pa_id));
		mysql_tquery(mysql, query, "IsAdministrationHavePassword", "i", playerid);
	}
	return true;
}

CMD:a(playerid, params[]){
	if(sscanf(params, "s[144]", params[0])) return SendErrorMessage(playerid, "Используйте '/a [Текст]'.");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d]: %s",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid, params[0]);
	Admin:SendAdminMessage(COLOR_LIME, small_string);
	small_string[0] = EOS;
	return true;
}

CMD:hp(playerid){
	SendClientMessage(playerid, COLOR_LIME, "Вы восстановили себе здоровье и нужды.");

	PlayerNeeds:ClearValues(playerid);
	SetPlayerHealth(playerid, 100.0);
	PlayerNeeds:SaveNeeds(playerid);
	return true;
}

CMD:kick(playerid, params[]){
	if(sscanf(params, "is[32]", params[0], params[1])) return SendErrorMessage(playerid, "Используйте '/kick [ID] [Причина]'.");

	if(!IsPlayerConnected(params[0])) return SendErrorMessage(playerid, "Данный игрок не в сети.");
	if(playerid == params[0]) return SendErrorMessage(playerid, "Вы не можете отключить самого себя.");
	if(GetPlayerAdminData(playerid, pa_level) < GetPlayerAdminData(params[0], pa_level)) return SendErrorMessage(playerid, "Вы не можете отключить данного игрока.");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Администратор %s отключил(-а) игрока %s.",
		GetPlayerData(playerid, p_name), GetPlayerData(params[0], p_name));
	Admin:SendAdminActionMessage(COLOR_LIGHT_RED, small_string);

	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d] отключил(-а) игрока %s по причине: %s",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid,
		GetPlayerData(params[0], p_name), params[1]);
	Admin:SendAdminMessage(COLOR_GREY, small_string);

	format(small_string, sizeof(small_string),
		"Администратор %s отключил(-а) Вас от сервера по причине: %s",
		GetPlayerData(playerid, p_name), params[1]);
	SendClientMessage(params[0], COLOR_LIGHT_RED, small_string);
	small_string[0] = EOS;

	KickPlayer(params[0]);
	return true;
}

CMD:ooc(playerid, params[]){
	if(sscanf(params, "s[144]", params[0])) return SendErrorMessage(playerid, "Используйте '/ooc [Текст]'.");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"<< %s[%d]: %s >>",
		GetPlayerData(playerid, p_name), playerid, params[0]);
	SendClientMessageToAll(COLOR_WHITE, small_string);
	small_string[0] = EOS;
	return true;
}

CMD:tveh(playerid, params[]){
	if(sscanf(params, "i", params[0])) return SendErrorMessage(playerid, "Используйте '/tveh [ID модели]'");
	if(params[0] < 400 || params[0] > 610) return SendErrorMessage(playerid, "Данной модели транспорта не существует.");

	new Float:pos[4];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, pos[3]);
	new vehicleid = CreateVehicle(params[0], pos[0], pos[1], pos[2], pos[3], random(255), random(255), 1000);
	PutPlayerInVehicle(playerid, vehicleid, 0);
	SetPVarInt(playerid, "InTempVehicle", vehicleid);
	return true;
}

CMD:nrg(playerid){ return callcmd::tveh(playerid, "522"); }

CMD:delveh(playerid, params[]){
	if(sscanf(params, "i", params[0])) return SendErrorMessage(playerid, "Используйте '/delveh [ID транспорта]'");
	if(!IsValidVehicle(params[0])) return SendErrorMessage(playerid, "Данного транспорта не существует.");

	DestroyVehicle(params[0]);
	return true;
}

CMD:tpcoord(playerid, params[]){
	new Float:pos[3];
	if(sscanf(params, "fff", pos[0], pos[1], pos[2])) return SendErrorMessage(playerid, "Используйте '/tpcoord [X] [Y] [Z]'");

	SetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Вы телепортировались по координатам %f, %f, %f.",
		pos[0], pos[1], pos[2]);
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;
	return true;
}

CMD:setvw(playerid, params[]){
	if(sscanf(params, "ii", params[0], params[1])) return SendErrorMessage(playerid, "Используйте '/setvw [ID игрока] [Вирт. Мир]'");

	if(!IsPlayerConnected(params[0])) return SendErrorMessage(playerid, "Игрок не в сети.");
	if(params[1] < 0 || params[1] > 10000) return SendErrorMessage(playerid, "Виртуальный мир должен быть от 0 до 1000.");

	SetPlayerVirtualWorld(params[0], params[1]);

	small_string[0] = EOS;
	if(playerid == params[0]){
		format(small_string, sizeof(small_string),
			"Вы установили себе виртуальный мир %d.", params[1]);
		SendClientMessage(playerid, COLOR_LIME, small_string);
	}
	else{
		format(small_string, sizeof(small_string),
			"Вы установили игроку %s[%d] виртуальный мир %d.", 
			GetPlayerData(params[0], p_name), params[0], params[1]);
		SendClientMessage(playerid, COLOR_LIME, small_string);

		format(small_string, sizeof(small_string),
			"Администратор %s установил(-а) Вам виртуальный мир %d.", 
			GetPlayerData(playerid, p_name), params[1]);
		SendClientMessage(params[0], COLOR_LIME, small_string);
	}

	small_string[0] = EOS;
	return true;
}

CMD:setint(playerid, params[]){
	if(sscanf(params, "ii", params[0], params[1])) return SendErrorMessage(playerid, "Используйте '/setint [ID игрока] [Интерьер]'");

	if(!IsPlayerConnected(params[0])) return SendErrorMessage(playerid, "Игрок не в сети.");
	if(params[1] < 0 || params[1] > 10000) return SendErrorMessage(playerid, "Интерьер должен быть от 0 до 1000.");

	SetPlayerInterior(params[0], params[1]);

	small_string[0] = EOS;
	if(playerid == params[0]){
		format(small_string, sizeof(small_string),
			"Вы установили себе интерьер %d.", params[1]);
		SendClientMessage(playerid, COLOR_LIME, small_string);
	}
	else{
		format(small_string, sizeof(small_string),
			"Вы установили игроку %s[%d] интерьер %d.", 
			GetPlayerData(params[0], p_name), params[0], params[1]);
		SendClientMessage(playerid, COLOR_LIME, small_string);

		format(small_string, sizeof(small_string),
			"Администратор %s установил(-а) Вам интерьер %d.", 
			GetPlayerData(playerid, p_name), params[1]);
		SendClientMessage(params[0], COLOR_LIME, small_string);
	}
	
	small_string[0] = EOS;
	return true;
}

CMD:giveinv(playerid, params[]){
	if(sscanf(params, "i", params[0])) return SendErrorMessage(playerid, "Используйте '/giveinv [ID игрока]'");

	if(!IsPlayerConnected(params[0])) return SendErrorMessage(playerid, "Игрок не в сети.");

	SetPVarInt(playerid, "Admin_GiveItemID", params[0]);

	big_string[0] = EOS;
	for(new i = 1; i <= MAX_INVENTORY_ITEMS; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, i, GetInventoryItemData(i, item_name));
	}
	Dialog_Open(playerid, Dialog:D_ADMIN_GIVE_INV, DIALOG_STYLE_LIST,
		"Выдать предмет", big_string,
		"Выдать", "Закрыть");
	big_string[0] = EOS;
	return true;
}

CMD:agivemoney(playerid, params[]){
	if(sscanf(params, "ii", params[0], params[1])) return SendErrorMessage(playerid, "Используйте '/agivemoney [ID игрока] [Кол-во денег]'");

	if(!IsPlayerConnected(params[0])) return SendErrorMessage(playerid, "Игрок не в сети.");
	if(params[1] < 0 || params[1] > 10000000) return SendErrorMessage(playerid, "Количество денежных средств должно быть от 1$ до 10000000$.");

	GiveMoney(params[0], params[1]);

	small_string[0] = EOS;
	if(playerid == params[0]){
		format(small_string, sizeof(small_string),
			"[A][%d] Администратор %s[%d] выдал(-а) себе %d$.",
			GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid, params[1]);
		Admin:SendAdminMessage(COLOR_GREY, small_string);

		format(small_string, sizeof(small_string),
			"Вы выдали себе %d$.",
			params[1]);
		SendClientMessage(playerid, COLOR_LIME, small_string);
	}
	else{
		format(small_string, sizeof(small_string),
			"[A][%d] Администратор %s[%d] выдал(-а) игроку %s[%d] %d$.",
			GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid, 
			GetPlayerData(params[0], p_name), params[0], params[1]);
		Admin:SendAdminMessage(COLOR_GREY, small_string);

		format(small_string, sizeof(small_string),
			"Администратор %s выдал(-а) Вам %d$.",
			GetPlayerData(playerid, p_name), params[1]);
		SendClientMessage(params[0], COLOR_LIME, small_string);
	}

	small_string[0] = EOS;
	return true;
}

CMD:gmx(playerid){
	SendClientMessageToAll(COLOR_LIGHT_RED, "Рестарт сервера произойдёт через 5 секунд.");
	SetTimer("ServerRestartTimer", 5000, false);
	return true;
}

CMD:payday(playerid){
	gettime(global_hour);
	PayDay(global_hour);
	return true;
}

CMD:paydayres(playerid){
	PayDay(5);
	return true;
}

CMD:gotobiz(playerid, params[]){
	if(sscanf(params, "i", params[0]) || !params[0]) return SendErrorMessage(playerid, "Используйте '/gotobiz [ID бизнеса]'");
	
	new business_index = -1;
	for(new i; i < MAX_BUSINESS; i++){
		if(GetBusinessData(i, b_id) != params[0]) continue;
		business_index = i;
		break;
	}

	if(business_index == -1) return SendErrorMessage(playerid, "Данного бизнеса не существует.");

	if(IsPlayerInAnyVehicle(playerid)){
		SetVehiclePos(GetPlayerVehicleID(playerid), GetBusinessData(business_index, b_pos_x), GetBusinessData(business_index, b_pos_y), GetBusinessData(business_index, b_pos_z));
		SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), GetBusinessData(business_index, b_vw));
		LinkVehicleToInterior(GetPlayerVehicleID(playerid), GetBusinessData(business_index, b_int));
	}
	else{
		SetPlayerPos(playerid, GetBusinessData(business_index, b_pos_x), GetBusinessData(business_index, b_pos_y), GetBusinessData(business_index, b_pos_z));
	}

	SetPlayerVirtualWorld(playerid, GetBusinessData(business_index, b_vw));
	SetPlayerInterior(playerid, GetBusinessData(business_index, b_int));
	return true;
}

CMD:gotohouse(playerid, params[]){
	if(sscanf(params, "i", params[0]) || !params[0]) return SendErrorMessage(playerid, "Используйте '/gotohouse [ID дома]'");
	
	new house_index = -1;
	for(new i; i < MAX_HOUSES; i++){
		if(GetHouseData(i, h_id) != params[0]) continue;
		house_index = i;
		break;
	}

	if(house_index == -1) return SendErrorMessage(playerid, "Данного дома не существует.");

	if(IsPlayerInAnyVehicle(playerid)){
		SetVehiclePos(GetPlayerVehicleID(playerid), GetHouseData(house_index, h_pos_x), GetHouseData(house_index, h_pos_y), GetHouseData(house_index, h_pos_z));
		SetVehicleVirtualWorld(GetPlayerVehicleID(playerid), GetHouseData(house_index, h_vw));
		LinkVehicleToInterior(GetPlayerVehicleID(playerid), GetHouseData(house_index, h_int));
	}
	else{
		SetPlayerPos(playerid, GetHouseData(house_index, h_pos_x), GetHouseData(house_index, h_pos_y), GetHouseData(house_index, h_pos_z));
	}

	SetPlayerVirtualWorld(playerid, GetHouseData(house_index, h_vw));
	SetPlayerInterior(playerid, GetHouseData(house_index, h_int));
	return true;
}

CMD:addbiz(playerid){
	big_string[0] = EOS;
	for(new i = 1; i < BUSINESS_TYPES_COUNT; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s",
			big_string, i,
			business_type_names[i]);
	}

	Dialog_Open(playerid, Dialog:D_ADMIN_BIZ_CREATE_TYPE, DIALOG_STYLE_LIST,
		"Выберите тип бизнеса", big_string,
		"Далее", "Закрыть");
	big_string[0] = EOS;
	return true;
}