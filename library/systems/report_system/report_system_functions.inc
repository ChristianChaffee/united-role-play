
stock Report:ShowSelectType(playerid){
	static const fmt_query[] = "SELECT COUNT(*) AS answered_count FROM reports WHERE account_id = %d AND status = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id), REPORT_STATUS_ANSWERED);
	mysql_tquery(mysql, query, "ShowSelectType", "i", playerid);
	return true;
}

stock Report:ShowEnterText(playerid){
	if(!GetPVarType(playerid, "Report_SelectedType")) return false;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Введите текст "cl_grey"%s",
		(GetPVarInt(playerid, "Report_SelectedType") == REPORT_TYPE_QUESTION) ? ("вопроса") : ("жалобы"));

	Dialog_Open(playerid, Dialog:D_REPORT_ENTER_TEXT, DIALOG_STYLE_INPUT,
		(GetPVarInt(playerid, "Report_SelectedType") == REPORT_TYPE_QUESTION) ? ("Вопрос по игре") : ("Жалоба"),
		big_string,
		"Отправить", "Назад");
	big_string[0] = EOS;
	return true;
}

stock Report:ShowAdminList(playerid){
	if(GetPlayerAdminData(playerid, pa_id) <= 0 || !GetPlayerAdminData(playerid, pa_logged)) return false;

	static const fmt_query[] = "SELECT a.name, r.id, r.type, r.text, r.timestamp FROM reports r INNER JOIN accounts a ON a.id = r.account_id WHERE r.status = %d ORDER BY r.type DESC, r.timestamp ASC LIMIT 50";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, REPORT_STATUS_WAIT);
	mysql_tquery(mysql, query, "ShowAdminReportsList", "i", playerid);
	return true;
}

//==============================================================================
stock Report:CheckValidReport(playerid, report_id, callback_type, temp_value[] = ""){
	static const fmt_query[] = "SELECT a.name, r.* FROM reports r INNER JOIN accounts a ON a.id = r.account_id WHERE r.status = %d AND r.id = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query, REPORT_STATUS_WAIT, report_id);
	mysql_tquery(mysql, query, "CheckValidReport", "iis", playerid, callback_type, temp_value);
	return true;
}

stock Report:CheckPlayerAnswers(playerid){
	static const fmt_query[] = "SELECT COUNT(*) AS answered_count FROM reports WHERE account_id = %d AND status = %d";
	new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id), REPORT_STATUS_ANSWERED);
	mysql_tquery(mysql, query, "CheckPlayerAnswers", "i", playerid);
	return true;
}

stock Report:ShowPlayerReportList(playerid){
	if(GetPVarType(playerid, "Report_ID")) DeletePVar(playerid, "Report_ID");

	static const fmt_query[] = "SELECT id, text, type, status, timestamp FROM reports WHERE account_id = %d ORDER BY timestamp DESC LIMIT 50";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query, "ShowPlayerReportList", "i", playerid);
	return true;
}

stock Report:ShowReportInfo(playerid, report_id){
	static const fmt_query[] = "SELECT text, status, type, timestamp FROM reports WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, report_id);
	mysql_tquery(mysql, query, "ShowReportInfo", "ii", playerid, 0);
	return true;
}