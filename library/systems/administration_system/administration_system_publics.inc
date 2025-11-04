
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