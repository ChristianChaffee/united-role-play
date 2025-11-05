
DialogResponse:D_ADMIN_CREATE_PASSWORD(playerid, response, listitem, inputtext[]){
	if(!GetPlayerAdminData(playerid, pa_id)) return false;
	if(!response) return false;

	new Regex:password_regex = Regex_New("^[-A-Za-z0-9!@#$^&*()_+[\\];\\\\<>,.\\/?~]{8,32}$");
	if(!Regex_Check(inputtext, password_regex)){
		SendErrorMessage(playerid, "Пароль должен состоять от 8 до 32 латинских букв и символов.");
		callcmd::alogin(playerid);
		Regex_Delete(password_regex);
		return false;
	}

	static const fmt_query[] = "UPDATE administration SET password = MD5('%s') WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 32) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, inputtext, GetPlayerAdminData(playerid, pa_id));
	mysql_tquery(mysql, query, "AdminAccountPasswordCreated", "is", playerid, inputtext);
	return true;
}

DialogResponse:D_ADMIN_LOGIN(playerid, response, listitem, inputtext[]){
	if(!GetPlayerAdminData(playerid, pa_id)) return false;
	if(!response) return false;

	static const fmt_query[] = "SELECT id FROM administration WHERE password = MD5('%s') AND id = %d";
	new query[sizeof(fmt_query) + (-2 + 32) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, inputtext, GetPlayerAdminData(playerid, pa_id));
	mysql_tquery(mysql, query, "CheckPlayerAdminPassword", "i", playerid);
	return true;
}

DialogResponse:D_ADMIN_GIVE_INV(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Admin_GiveItemID");
		return false;
	}

	new 
		scdplayerid = GetPVarInt(playerid, "Admin_GiveItemID"),
		slot_index = Inventory:GetFreeSlotIndex(scdplayerid);

	if(slot_index == -1){
		SendErrorMessage(playerid, "У данного игрока нет места в инвентаре.");
		DeletePVar(playerid, "Admin_GiveItemID");
		return false;
	}

	Inventory:GiveItem(scdplayerid, slot_index, listitem + 1);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d] выдал(-а) предмет \"%s\" игроку %s[%d].",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid,
		GetInventoryItemData(listitem + 1, item_name),
		GetPlayerData(scdplayerid, p_name), scdplayerid);
	Admin:SendAdminMessage(COLOR_GREY, small_string);

	format(small_string, sizeof(small_string),
		"Администратор %s[%d] выдал(-а) Вам предмет \"%s\".",
		GetPlayerData(playerid, p_name), playerid, GetInventoryItemData(listitem + 1, item_name));
	SendClientMessage(scdplayerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	DeletePVar(playerid, "Admin_GiveItemID");
	return true;
}