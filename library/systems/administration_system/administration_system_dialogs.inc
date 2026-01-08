
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

DialogResponse:D_ADMIN_BIZ_CREATE_TYPE(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	SetPVarInt(playerid, "Admin_BusinessCreateType", listitem + 1);
	Admin:ShowCreateBusinessPrice(playerid);
	return true;
}

DialogResponse:D_ADMIN_BIZ_CREATE_PRICE(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Admin_BusinessCreateType");
		callcmd::addbiz(playerid);
		return false;
	}

	new price = strval(inputtext);
	if(price < 0 || price > 500000000){
		SendErrorMessage(playerid, "Цена бизнеса должна быть от 0 до 500.000.000$");
		Admin:ShowCreateBusinessPrice(playerid);
		return false;
	}

	SetPVarInt(playerid, "Admin_BusinessCreatePrice", price);
	Admin:ShowCreateBusinessName(playerid);
	return true;
}

DialogResponse:D_ADMIN_BIZ_CREATE_NAME(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Admin_BusinessCreatePrice");
		Admin:ShowCreateBusinessPrice(playerid);
		return false;
	}

	if(strlen(inputtext) <= 0 || strlen(inputtext) >= 64){
		SendErrorMessage(playerid, "Название бизнеса должно быть от 1 до 64 символов.");
		Admin:ShowCreateBusinessName(playerid);
		return false;
	}

	SetPVarString(playerid, "Admin_BusinessCreateName", inputtext);
	Admin:ShowBusinessCreateAccept(playerid);
	return true;
}

DialogResponse:D_ADMIN_BIZ_CREATE_ACCEPT(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Admin_BusinessCreateName");
		Admin:ShowCreateBusinessName(playerid);
		return false;
	}

	new business_name[64],
		Float:pos[3];

	GetPVarString(playerid, "Admin_BusinessCreateName", business_name, sizeof(business_name));
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	static const fmt_query[] = "INSERT INTO business (type, name, pos_x, pos_y, pos_z, vw, `int`, price) VALUES \
		(%d, '%s', %f, %f, %f, %d, %d, %d)";

	new query[sizeof(fmt_query) + ((-2 + 11) * 7) + (-2 + 64)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPVarInt(playerid, "Admin_BusinessCreateType"),
		business_name,
		pos[0], pos[1], pos[2], 
		GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid),
		GetPVarInt(playerid, "Admin_BusinessCreatePrice"));
	mysql_tquery(mysql, query, "NewBusinessCreated");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] Администратор %s[%d] создал(-а) бизнес \"%s\" (%s) с гос. стоимостью %d$.",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid,
		business_name, business_type_names[GetPVarInt(playerid, "Admin_BusinessCreateType")],
		GetPVarInt(playerid, "Admin_BusinessCreatePrice"));
	Admin:SendAdminMessage(COLOR_GREY, small_string);
	small_string[0] = EOS;
	return true;
}