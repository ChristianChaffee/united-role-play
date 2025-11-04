
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