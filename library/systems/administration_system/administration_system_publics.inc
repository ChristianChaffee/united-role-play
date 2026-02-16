
forward LoadPlayerAdminAccount(playerid);
public LoadPlayerAdminAccount(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows) return false;

	cache_get_value_name_int(0, "id", GetPlayerAdminData(playerid, pa_id));
	cache_get_value_name_int(0, "account_id", GetPlayerAdminData(playerid, pa_account_id));
	cache_get_value_name_int(0, "level", GetPlayerAdminData(playerid, pa_level));

	SendClientMessage(playerid, COLOR_GREEN, "Вы являетесь администратором. Чтобы авторизоваться в панели администратора введите '/alogin'");
	return true;
}

forward IsAdministrationHavePassword(playerid);
public IsAdministrationHavePassword(playerid){
	if(!GetPlayerAdminData(playerid, pa_id)) return false;

	new rows;
	cache_get_row_count(rows);

	if(rows) Admin:ShowCreatePassword(playerid);
	else Admin:ShowLogin(playerid);

	return true;
}

forward AdminAccountPasswordCreated(playerid, password[]);
public AdminAccountPasswordCreated(playerid, password[]){
	if(!GetPlayerAdminData(playerid, pa_id)) return false;

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Ваш новый пароль от панели администратора: "cl_white"%s "cl_dark_red"(ЗАПОМНИТЕ ЕГО!)",
		password);
	SendClientMessage(playerid, COLOR_DARK_RED, small_string);
	small_string[0] = EOS;

	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Чтобы авторизоваться в админ-панели повторно введите '/alogin'.");
	return true;
}

forward CheckPlayerAdminPassword(playerid);
public CheckPlayerAdminPassword(playerid){
	if(!GetPlayerAdminData(playerid, pa_id)) return false;

	new rows;
	cache_get_row_count(rows);

	if(!rows){
		new temp_ip[32];
		GetPlayerIp(playerid, temp_ip, sizeof(temp_ip));

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"[A] Администратор %d-го уровня %s[%d] неверно ввёл пароль от панели администратора (IP: %s).",
			GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid, temp_ip);
		Admin:SendAdminMessage(COLOR_GREY, small_string);
		small_string[0] = EOS;

		SendErrorMessage(playerid, "Вы неверно ввели пароль от панели администратора.");
		callcmd::alogin(playerid);
		return false;
	}
	else{
		SetPlayerAdminData(playerid, pa_logged, true);

		new temp_ip[32];
		GetPlayerIp(playerid, temp_ip, sizeof(temp_ip));

		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"[A] Администратор %d-го уровня %s[%d] успешно авторизовался в панели администратора (IP: %s).",
			GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid, temp_ip);
		Admin:SendAdminMessage(COLOR_GREY, small_string);
		small_string[0] = EOS;
		return true;
	}
}

forward OnAdminFractionCreate(playerid);
public OnAdminFractionCreate(playerid){
	new temp_frac_id = cache_insert_id(),
		free_frac_index = -1;

	for(new i; i < MAX_FRACTIONS; i++){
		if(GetFractionData(i, f_id) != 0) continue;
		free_frac_index = i;
		break;
	}

	if(!temp_frac_id || free_frac_index == -1){
		SendErrorMessage(playerid, "Произошла ошибка.");
		Admin:DeleteFractionPVars(playerid);
		return false;
	}

	new fraction_name[64];
	GetPVarString(playerid, "Admin_FracCreateName", fraction_name, sizeof(fraction_name));

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"[A][%d] Администратор %s[%d] создал(-а) фракцию \"%s\" (%s).",
		GetPlayerAdminData(playerid, pa_level), GetPlayerData(playerid, p_name), playerid,
		fraction_name, fraction_type_names[GetPVarInt(playerid, "Admin_FracCreateType")]);
	Admin:SendAdminMessage(COLOR_GREY, small_string);
	small_string[0] = EOS;

	static const fmt_query[] = "SELECT * FROM fractions WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, temp_frac_id);
	mysql_tquery(mysql, query, "LoadFractions", "i", free_frac_index);

	Admin:DeleteFractionPVars(playerid);
	return true;
}

forward ShowFractionInfo(playerid, fraction_index);
public ShowFractionInfo(playerid, fraction_index){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS) return false;

	new rows;
	cache_get_row_count(rows);
	if(!rows) return false;

	new temp_name[MAX_PLAYER_NAME];
	cache_get_value_name(0, "leader_name", temp_name);

	Fraction:ShowFractionInfoDialog(playerid, fraction_index, temp_name);
	return true;
}

forward CheckFractionRankChange(fraction_index);
public CheckFractionRankChange(fraction_index){
	if(fraction_index < 0 || fraction_index >= MAX_FRACTIONS) return false;

	new rows;
	cache_get_row_count(rows);

	if(!rows) return true;

	static const fmt_query[] = "UPDATE accounts SET fraction_rank = %d WHERE id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];

	for(new i, temp_id; i < rows; i++){

		cache_get_value_name_int(i, "id", temp_id);

		if(GetFractionData(fraction_index, f_leader_account_id) == temp_id){
			mysql_format(mysql, query, sizeof(query), fmt_query,
				GetFractionData(fraction_index, f_ranks_count),
				temp_id);
			mysql_tquery(mysql, query);
		}
		else{
			mysql_format(mysql, query, sizeof(query), fmt_query,
				GetFractionData(fraction_index, f_ranks_count) - 1,
				temp_id);
			mysql_tquery(mysql, query);
		}

		if(GetPlayerIdBySqlId(temp_id) != INVALID_PLAYER_ID){
			if(GetFractionData(fraction_index, f_leader_account_id) == temp_id){
				SetPlayerData(GetPlayerIdBySqlId(temp_id), p_fraction_rank, GetFractionData(fraction_index, f_ranks_count));
			}
			else{
				SetPlayerData(GetPlayerIdBySqlId(temp_id), p_fraction_rank, GetFractionData(fraction_index, f_ranks_count) - 1);
			}
		}
	}

	return true;
}