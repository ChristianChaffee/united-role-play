
DialogResponse:D_REPORT_SELECT_TYPE(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	if(listitem == 2){
		Report:ShowPlayerReportList(playerid);
		return true;
	}

	SetPVarInt(playerid, "Report_SelectedType", listitem + 1);
	Report:ShowEnterText(playerid);
	return true;
}

DialogResponse:D_REPORT_ENTER_TEXT(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Report_SelectedType");
		Report:ShowSelectType(playerid);
		return false;
	}

	if(strlen(inputtext) <= 0 || strlen(inputtext) >= 256){
		SendErrorMessage(playerid, "Текст обращения к администрации должен быть от 1 до 256 символов.");
		Report:ShowEnterText(playerid);
		return false;
	}

	static const fmt_query[] = "INSERT INTO reports (type, text, account_id, status, timestamp) VALUES (%d, '%s', %d, %d, %d)";
	new query[sizeof(fmt_query) + ((-2 + 11) * 4) + (-2 + 256)];
	mysql_format(mysql, query, sizeof(query), fmt_query,
		GetPVarInt(playerid, "Report_SelectedType"), inputtext,
		GetPlayerData(playerid, p_id), REPORT_STATUS_WAIT, gettime());
	mysql_tquery(mysql, query);

	if(GetPVarInt(playerid, "Report_SelectedType") == REPORT_TYPE_QUESTION){
		SendClientMessage(playerid, COLOR_LIME, "Ваш вопрос отправлен администрации:");
	}
	else{
		SendClientMessage(playerid, COLOR_LIME, "Ваша жалоба отправлена администрации:");
	}

	SendClientMessage(playerid, COLOR_WHITE, inputtext);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"%s от игрока %s[%d]:",
		(GetPVarInt(playerid, "Report_SelectedType") == REPORT_TYPE_QUESTION) ? ("Вопрос") : ("Жалоба"),
		GetPlayerData(playerid, p_name), playerid);

	Admin:SendAdminMessage(COLOR_LIME, small_string);
	Admin:SendAdminMessage(COLOR_WHITE, inputtext);
	small_string[0] = EOS;

	DeletePVar(playerid, "Report_SelectedType");
	return true;
}

DialogResponse:D_REPORT_ADMIN_LIST(playerid, response, listitem, inputtext[]){
	if(!response) return false;

	SetPVarInt(playerid, "Report_ID", player_listitems[playerid][listitem]);
	Report:CheckValidReport(playerid, GetPVarInt(playerid, "Report_ID"), REPORT_CALLBACK_ACTION_LIST);
	return true;
}

DialogResponse:D_REPORT_ADMIN_ACTION(playerid, response, listitem, inputtext[]){
	if(!response){
		DeletePVar(playerid, "Report_ID");
		callcmd::rep(playerid);
		return false;
	}

	switch(listitem){
		case 0:{ // - Ответить на репорт
			Report:CheckValidReport(playerid, GetPVarInt(playerid, "Report_ID"), REPORT_CALLBACK_ENTER_ANSWER);
		}
		case 1:{ // - Удалить репорт
			Report:CheckValidReport(playerid, GetPVarInt(playerid, "Report_ID"), REPORT_CALLBACK_DELETE);
		}
	}
	return true;
}

DialogResponse:D_REPORT_ADMIN_ANSWER(playerid, response, listitem, inputtext[]){
	if(!response){
		Report:CheckValidReport(playerid, GetPVarInt(playerid, "Report_ID"), REPORT_CALLBACK_ACTION_LIST);
		return false;
	}

	if(strlen(inputtext) <= 0 || strlen(inputtext) >= 256){
		SendErrorMessage(playerid, "Ответ должен быть от 1 до 256 символов.");
		Report:CheckValidReport(playerid, GetPVarInt(playerid, "Report_ID"), REPORT_CALLBACK_ENTER_ANSWER);
		return false;
	}

	Report:CheckValidReport(playerid, GetPVarInt(playerid, "Report_ID"), REPORT_CALLBACK_SEND_ANSWER, inputtext);
	return true;
}

DialogResponse:D_REPORTS_LIST(playerid, response, listitem, inputtext[]){
	if(!response) return Report:ShowSelectType(playerid);

	SetPVarInt(playerid, "Report_ID", player_listitems[playerid][listitem]);
	Report:ShowReportInfo(playerid, GetPVarInt(playerid, "Report_ID"));
	return true;
}

DialogResponse:D_REPORT_INFO(playerid, response, listitem, inputtext[]){
	Report:ShowPlayerReportList(playerid);
	return true;
}