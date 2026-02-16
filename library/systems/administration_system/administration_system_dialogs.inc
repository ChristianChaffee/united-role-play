
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

	DeletePVar(playerid, "Admin_BusinessCreatePrice");
	DeletePVar(playerid, "Admin_BusinessCreateType");
	DeletePVar(playerid, "Admin_BusinessCreateName");
	return true;
}

DialogResponse:D_ADMIN_HOUSE_CREATE_TYPE(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	SetPVarInt(playerid, "Admin_HouseCreateType", listitem);
	Admin:ShowCreateHousePrice(playerid);
	return true;
}

DialogResponse:D_ADMIN_HOUSE_CREATE_PRICE(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Admin_HouseCreateType");
		callcmd::addhouse(playerid);
		return false;
	}

	new price = strval(inputtext);
	if(price < 0 || price > 500000000){
		SendErrorMessage(playerid, "Цена дома должна быть от 0 до 500.000.000$");
		Admin:ShowCreateHousePrice(playerid);
		return false;
	}

	SetPVarInt(playerid, "Admin_HouseCreatePrice", price);
	Admin:ShowCreateHouseAccept(playerid);
	return true;
}

DialogResponse:D_ADMIN_HOUSE_CREATE_ACCEPT(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Admin_HouseCreatePrice");
		Admin:ShowCreateHousePrice(playerid);
		return false;
	}

	new Float:pos[3], interior_index;
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	switch(GetPVarInt(playerid, "Admin_HouseCreateType")){
		case 0: interior_index = RandomEx(0, 9);
		case 1: interior_index = RandomEx(10, 21);
		case 2, 3: interior_index = RandomEx(22, 27);
	}

	static const fmt_query[] = "INSERT INTO houses (price, class, pos_x, pos_y, pos_z, interior, vw, interior_index) VALUES (%d, %d, %f, %f, %f, %d, %d, %d)";
	new query[sizeof(fmt_query) + ((-2 + 11) * 8)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPVarInt(playerid, "Admin_HouseCreatePrice"), GetPVarInt(playerid, "Admin_HouseCreateType"),
		pos[0], pos[1], pos[2],
		GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid),
		interior_index);
	mysql_tquery(mysql, query, "NewHouseCreated");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] Администратор %s[%d] создал(-а) дом класса %s с гос. стоимостью %d$.",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid,
		house_class_names[GetPVarInt(playerid, "Admin_HouseCreateType")], GetPVarInt(playerid, "Admin_HouseCreatePrice"));
	Admin:SendAdminMessage(COLOR_GREY, small_string);
	small_string[0] = EOS;

	DeletePVar(playerid, "Admin_HouseCreateType");
	DeletePVar(playerid, "Admin_HouseCreatePrice");
	return true;
}

DialogResponse:D_ADMIN_FRACMENU(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	switch(listitem){
		case 0: Admin:ShowCreateFraction(playerid);
		default: Admin:ShowFractionInfo(playerid, player_listitems[playerid][listitem - 1]);
	}
	return true;
}

DialogResponse:D_ADMIN_FRAC_CREATE(playerid, response, listitem, inputtext[]){
	if(!response){
		Admin:DeleteFractionPVars(playerid);
		callcmd::fracmenu(playerid);
		return false;
	}

	switch(listitem){
		case 0: Admin:ShowCreateFractionName(playerid);
		case 1: Admin:ShowCreateFractionType(playerid);
		case 2: Admin:ShowCreateFractionColor(playerid);
		case 3:{

			new fraction_name[64];
			GetPVarString(playerid, "Admin_FracCreateName", fraction_name, sizeof(fraction_name));

			static const fmt_query[] = "INSERT INTO fractions (name, type, color_str, color_value) VALUES ('%s', %d, '%s', %d)";
			new query[sizeof(fmt_query) + (-2 + 64) + (-2 + 32) + ((-2 + 11) * 2)];
			mysql_format(mysql, query, sizeof(query), fmt_query,
				fraction_name, GetPVarInt(playerid, "Admin_FracCreateType"),
				color_info[GetPVarInt(playerid, "Admin_FracCreateColor")][color_str],
				color_info[GetPVarInt(playerid, "Admin_FracCreateColor")][color_code]);
			mysql_tquery(mysql, query, "OnAdminFractionCreate", "i", playerid);
		}
	}
	return true;
}

DialogResponse:D_ADMIN_FRAC_CREATE_NAME(playerid, response, listitem, inputtext[]){
	if(!response){
		Admin:ShowCreateFraction(playerid);
		return false;
	}

	if(strlen(inputtext) < 5 || strlen(inputtext) >= 64){
		Admin:ShowCreateFractionName(playerid, ""cl_grey"Ошибка: "cl_white"название фракции должно быть от 5 до 64 символов.");
		return false;
	}

	SetPVarString(playerid, "Admin_FracCreateName", inputtext);
	Admin:ShowCreateFraction(playerid);
	return true;
}

DialogResponse:D_ADMIN_FRAC_CREATE_TYPE(playerid, response, listitem, inputtext[]){
	if(!response){
		Admin:ShowCreateFraction(playerid);
		return false;
	}

	SetPVarInt(playerid, "Admin_FracCreateType", listitem);
	Admin:ShowCreateFraction(playerid);
	return true;
}

DialogResponse:D_ADMIN_FRAC_CREATE_COLOR(playerid, response, listitem, inputtext[]){
	if(!response){
		Admin:ShowCreateFraction(playerid);
		return false;
	}

	SetPVarInt(playerid, "Admin_FracCreateColor", listitem);
	Admin:ShowCreateFraction(playerid);
	return true;
}

//==============================================================================
DialogResponse:D_ADMIN_FRAC_INFO(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Admin_FractionIndex");
		callcmd::fracmenu(playerid);
		return false;
	}

	switch(listitem){
		case 0: Admin:ShowChangeFracName(playerid);
		case 1: Admin:ShowChangeFracTag(playerid);
		case 2: Admin:ShowChangeFracType(playerid);
		case 3:{
			if(!GetFractionData(GetPVarInt(playerid, "Admin_FractionIndex"), f_leader_account_id)){
				SendClientMessage(playerid, COLOR_GREY, "- Чтобы назначить лидера используйте команду "cl_white"'/makeleader [ID]'");
				Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
				return false;
			}
			Admin:ShowAcceptUninviteLeader(playerid);
		}
		case 4: Admin:ShowChangeFracColor(playerid);
		case 5: Admin:ShowRanksMenu(playerid);
		case 6:{
			new fraction_index = GetPVarInt(playerid, "Admin_FractionIndex");
			DeletePVar(playerid, "Admin_FractionIndex");

			GetPlayerPos(playerid, 
				GetFractionData(fraction_index, f_spawn_x),
				GetFractionData(fraction_index, f_spawn_y),
				GetFractionData(fraction_index, f_spawn_z));
			GetPlayerFacingAngle(playerid, GetFractionData(fraction_index, f_spawn_angle));
			SetFractionData(fraction_index, f_spawn_vw, GetPlayerVirtualWorld(playerid));
			SetFractionData(fraction_index, f_spawn_int, GetPlayerInterior(playerid));

			static const fmt_query[] = "UPDATE fractions SET spawn_x = '%f', spawn_y = '%f', spawn_z = '%f', spawn_angle = '%f', spawn_vw = %d, spawn_int = %d WHERE id = %d";
			new query[sizeof(fmt_query) + ((-2 + 11) * 7)];
			mysql_format(mysql, query, sizeof(query), fmt_query,
				GetFractionData(fraction_index, f_spawn_x), GetFractionData(fraction_index, f_spawn_y), 
				GetFractionData(fraction_index, f_spawn_z), GetFractionData(fraction_index, f_spawn_angle),
				GetFractionData(fraction_index, f_spawn_vw), GetFractionData(fraction_index, f_spawn_int),
				GetFractionData(fraction_index, f_id));
			mysql_tquery(mysql, query);

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"[A][%d] %s[%d] изменил(-а) место спавна фракции \"%s\"",
				GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name),
				playerid, GetFractionData(fraction_index, f_name));
			Admin:SendAdminMessage(COLOR_GREY, small_string);
			small_string[0] = EOS;
		}
		case 7:{
			new fraction_index = GetPVarInt(playerid, "Admin_FractionIndex");
			DeletePVar(playerid, "Admin_FractionIndex");

			GetPlayerPos(playerid, 
				GetFractionData(fraction_index, f_dressroom_x),
				GetFractionData(fraction_index, f_dressroom_y),
				GetFractionData(fraction_index, f_dressroom_z));
			SetFractionData(fraction_index, f_dressroom_vw, GetPlayerVirtualWorld(playerid));
			SetFractionData(fraction_index, f_dressroom_int, GetPlayerInterior(playerid));

			static const fmt_query[] = "UPDATE fractions SET dressroom_x = '%f', dressroom_y = '%f', dressroom_z = '%f', dressroom_vw = %d, dressroom_int = %d WHERE id = %d";
			new query[sizeof(fmt_query) + ((-2 + 11) * 6)];
			mysql_format(mysql, query, sizeof(query), fmt_query,
				GetFractionData(fraction_index, f_dressroom_x), GetFractionData(fraction_index, f_dressroom_y), 
				GetFractionData(fraction_index, f_dressroom_z), GetFractionData(fraction_index, f_dressroom_vw), 
				GetFractionData(fraction_index, f_dressroom_int), GetFractionData(fraction_index, f_id));
			mysql_tquery(mysql, query);

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"[A][%d] %s[%d] изменил(-а) место раздевалки фракции \"%s\"",
				GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name),
				playerid, GetFractionData(fraction_index, f_name));
			Admin:SendAdminMessage(COLOR_GREY, small_string);
			small_string[0] = EOS;

			Fraction:UpdateDressroom(fraction_index);
		}
	}
	return true;
}

DialogResponse:D_ADMIN_FRAC_CHANGE_NAME(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	if(!response){
		Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
		return false;
	}

	if(strlen(inputtext) < 5 || strlen(inputtext) >= 64){
		Admin:ShowChangeFracName(playerid,""cl_grey"Ошибка: "cl_white"Название должно быть от 5 до 64 символов.");
		return false;
	}

	new fraction_index = GetPVarInt(playerid, "Admin_FractionIndex");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d] поменял(-а) название фракции \"%s\" на \"%s\".",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name),
		playerid, GetFractionData(fraction_index, f_name), inputtext);
	Admin:SendAdminMessage(COLOR_GREY, small_string);
	small_string[0] = EOS;

	format(GetFractionData(fraction_index, f_name), 64, "%s", inputtext);
	Fraction:UpdateStr(fraction_index, "name", GetFractionData(fraction_index, f_name));

	Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
	return true;
}

DialogResponse:D_ADMIN_FRAC_CHANGE_TAG(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	if(!response){
		Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
		return false;
	}

	if(strlen(inputtext) < 2 || strlen(inputtext) > 6){
		Admin:ShowChangeFracTag(playerid,""cl_grey"Ошибка: "cl_white"Тэг должен быть от 2 до 6 символов.");
		return false;
	}

	new fraction_index = GetPVarInt(playerid, "Admin_FractionIndex");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d] установил(-а) тэг фракции \"%s\" на \"%s\".",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name),
		playerid, GetFractionData(fraction_index, f_name), inputtext);
	Admin:SendAdminMessage(COLOR_GREY, small_string);
	small_string[0] = EOS;

	format(GetFractionData(fraction_index, f_tag), 64, "%s", inputtext);
	Fraction:UpdateStr(fraction_index, "tag", GetFractionData(fraction_index, f_tag));

	Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
	return true;
}

DialogResponse:D_ADMIN_FRAC_CHANGE_TYPE(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	if(!response){
		Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
		return false;
	}

	new fraction_index = GetPVarInt(playerid, "Admin_FractionIndex");
	if(listitem == GetFractionData(fraction_index, f_type)){
		SendErrorMessage(playerid, "Выберите тип фракции, отличающийся от нынешнего.");
		Admin:ShowChangeFracType(playerid);
		return false;
	}

	SetFractionData(fraction_index, f_type, listitem);
	Fraction:UpdateInt(fraction_index, "type", listitem);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d] изменил(-а) тип фракции \"%s\" на \"%s\".",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name),
		playerid, GetFractionData(fraction_index, f_name), fraction_type_names[listitem]);
	Admin:SendAdminMessage(COLOR_GREY, small_string);
	small_string[0] = EOS;

	Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
	return true;
}

DialogResponse:D_ADMIN_FRAC_ACCEPT_LEADER(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	if(!response){
		Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
		return false;
	}

	new fraction_index = GetPVarInt(playerid, "Admin_FractionIndex");
	if(!GetFractionData(fraction_index, f_leader_account_id)){
		SendErrorMessage(playerid, "Произошла ошибка.");
		Admin:ShowFractionInfo(playerid, fraction_index);
		return false;
	}

	Fraction:UninvitePlayer(GetFractionData(fraction_index, f_leader_account_id));

	SetFractionData(fraction_index, f_leader_account_id, 0);
	Fraction:UpdateInt(fraction_index, "leader_account_id", 0);

	SendClientMessage(playerid, COLOR_GREEN, "Вы сняли лидера фракции.");
	Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
	return true;
}

DialogResponse:D_ADMIN_FRAC_CHANGE_COLOR(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	if(!response){
		Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
		return false;
	}

	new fraction_index = GetPVarInt(playerid, "Admin_FractionIndex");

	SetFractionData(fraction_index, f_color_value, color_info[listitem][color_code]);
	format(GetFractionData(fraction_index, f_color_str), 16, "%s", color_info[listitem][color_str]);

	Fraction:UpdateInt(fraction_index, "color_value", GetFractionData(fraction_index, f_color_value));
	Fraction:UpdateStr(fraction_index, "color_str", GetFractionData(fraction_index, f_color_str));

	SendClientMessage(playerid, COLOR_GREEN, "Вы изменили цвет фракции.");
	Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));

	#warning MAKE_LOGIC
	return true;
}

DialogResponse:D_ADMIN_FRAC_RANKS_MENU(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	if(!response){
		Admin:ShowFractionInfo(playerid, GetPVarInt(playerid, "Admin_FractionIndex"));
		return false;
	}

	switch(listitem){
		case 0: Admin:ShowFracRankList(playerid);
		case 1: Admin:ShowFracChangeMaxRank(playerid);
	}
	return true;
}

DialogResponse:D_ADMIN_FRAC_MAX_RANK(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	if(!response){
		Admin:ShowRanksMenu(playerid);
		return true;
	}

	new rank;
	if(sscanf(inputtext, "i", rank)){
		Admin:ShowFracChangeMaxRank(playerid, ""cl_grey"Ошибка: "cl_white"максимальный ранг должен быть числовым значением.");
		return false;
	}
	if(rank < 5 || rank > MAX_FRACTION_RANKS){
		Admin:ShowFracChangeMaxRank(playerid, ""cl_grey"Ошибка: "cl_white"максимальный ранг должен быть от 5 до "#MAX_FRACTION_RANKS".");
		return false;
	}

	new fraction_index = GetPVarInt(playerid, "Admin_FractionIndex");

	SetFractionData(fraction_index, f_ranks_count, rank);
	Fraction:UpdateInt(fraction_index, "ranks_count", rank);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d] изменил(-а) максимальный ранг фракции \"%s\" на %d.",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name),
		playerid, GetFractionData(fraction_index, f_name), rank);
	Admin:SendAdminMessage(COLOR_GREY, small_string);
	small_string[0] = EOS;

	static const fmt_query[] = "SELECT id FROM accounts WHERE fraction = %d AND fraction_rank >= %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetFractionData(fraction_index, f_id), rank);
	mysql_tquery(mysql, query, "CheckFractionRankChange", "i", fraction_index);

	Admin:ShowRanksMenu(playerid);
	return true;
}

DialogResponse:D_ADMIN_FRAC_RANKS_LIST(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;

	if(!response){
		Admin:ShowRanksMenu(playerid);
		return true;
	}

	SetPVarInt(playerid, "Admin_FractionRank", listitem);
	Admin:ShowChangeFracRank(playerid);
	return true;
}

DialogResponse:D_ADMIN_FRAC_CHANGE_RANK(playerid, response, listitem, inputtext[]){
	if(!GetPVarType(playerid, "Admin_FractionIndex")) return false;
	if(!GetPVarType(playerid, "Admin_FractionRank")) return false;

	if(!response){
		DeletePVar(playerid, "Admin_FractionRank");
		Admin:ShowFracRankList(playerid);
		return true;
	}

	if(strlen(inputtext) < 2 || strlen(inputtext) >= 32){
		Admin:ShowChangeFracRank(playerid, ""cl_grey"Ошибка: "cl_white"название ранга должно быть от 2 до 32 символов.");
		return false;
	}

	new fraction_index = GetPVarInt(playerid, "Admin_FractionIndex");

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d] изменил(-а) название ранга %d фракции \"%s\" на %s.",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name),
		playerid, GetPVarInt(playerid, "Admin_FractionRank") + 1, 
		GetFractionData(fraction_index, f_name), inputtext);
	Admin:SendAdminMessage(COLOR_GREY, small_string);
	small_string[0] = EOS;

	format(fraction_ranks[fraction_index][GetPVarInt(playerid, "Admin_FractionRank")], 32, "%s", inputtext);

	static const fmt_query[] = "UPDATE fractions SET rank_%d = '%s' WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2) + (-2 + 32)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPVarInt(playerid, "Admin_FractionRank") + 1,
		inputtext, GetFractionData(fraction_index, f_id));
	mysql_tquery(mysql, query);

	DeletePVar(playerid, "Admin_FractionRank");
	Admin:ShowFracRankList(playerid);
	return true;
}

//==============================================================================
DialogResponse:D_MAKELEADER(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Admin_MakeleaderID");
		return false;
	}

	new scdplayerid = GetPVarInt(playerid, "Admin_MakeleaderID"),
		fraction_index = player_listitems[playerid][listitem];

	DeletePVar(playerid, "Admin_MakeleaderID");

	if(!IsPlayerConnected(scdplayerid) && !player_logged[scdplayerid]) return SendErrorMessage(playerid, "Игрок не в сети.");
	if(GetPlayerData(scdplayerid, p_fraction) > 0) return SendErrorMessage(playerid, "Игрок уже состоит во фракции.");


	SetPlayerData(scdplayerid, p_fraction, GetFractionData(fraction_index, f_id));
	SetPlayerData(scdplayerid, p_fraction_rank, GetFractionData(fraction_index, f_ranks_count));

	new fraction_skin_index = Fraction:UpdatePlayerSkinIndex(scdplayerid);
	if(fraction_skin_index != -1){
		SetPlayerData(scdplayerid, p_fraction_skin, GetFractionSkinData(fraction_skin_index, fs_id));	
	}

	SetFractionData(fraction_index, f_leader_account_id, GetPlayerData(scdplayerid, p_id));
	Fraction:UpdateInt(fraction_index, "leader_account_id", GetFractionData(fraction_index, f_leader_account_id));

	static const fmt_query[] = "UPDATE accounts SET fraction = %d, fraction_rank = %d, fraction_skin = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 4)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPlayerData(scdplayerid, p_fraction), GetPlayerData(scdplayerid, p_fraction_rank),
		GetPlayerData(scdplayerid, p_fraction_skin),
		GetPlayerData(scdplayerid, p_id));
	mysql_tquery(mysql, query);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] %s[%d] назначил(-а) %s[%d] лидером фракции \"%s\".",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name),
		playerid, 
		GetPlayerData(scdplayerid, p_name), scdplayerid,
		GetFractionData(fraction_index, f_name));
	Admin:SendAdminMessage(COLOR_GREY, small_string);

	format(small_string, sizeof(small_string),
		"Администратор %s назначил(-а) Вас лидером фракции \"%s\".",
		GetPlayerData(playerid, p_name), GetFractionData(fraction_index, f_name));
	SendClientMessage(scdplayerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	return true;
}