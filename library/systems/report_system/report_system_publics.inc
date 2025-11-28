
forward ShowAdminReportsList(playerid);
public ShowAdminReportsList(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows){
		SendErrorMessage(playerid, "В данный момент нет обращений от игроков для рассмотрения.");
		return false;
	}

	new temp_year, 
		temp_month, 
		temp_day, 
		temp_hour, 
		temp_minute;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Ник-нейм\t"cl_grey"Тип\t"cl_grey"Текст\t"cl_grey"Дата и время");

	for(new i, temp_id, temp_name[MAX_PLAYER_NAME], temp_type, temp_text[256], temp_timestamp; i < rows; i++){

		cache_get_value_name_int(i, "id", temp_id);
		cache_get_value_name(i, "name", temp_name);
		cache_get_value_name_int(i, "type", temp_type);
		cache_get_value_name(i, "text", temp_text);
		cache_get_value_name_int(i, "timestamp", temp_timestamp);

		ConvertTime(temp_timestamp, temp_year, temp_month, temp_day, temp_hour, temp_minute);

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t%s\t%s\t%02d.%02d.%d в %02d:%02d",
			big_string, i + 1,
			temp_name,
			(temp_type == REPORT_TYPE_QUESTION) ? ("Вопрос") : ("Жалоба"),
			temp_text,
			temp_day, temp_month, temp_year, temp_hour, temp_minute);

		player_listitems[playerid][i] = temp_id;
	}

	Dialog_Open(playerid, Dialog:D_REPORT_ADMIN_LIST, DIALOG_STYLE_TABLIST_HEADERS,
		"Обращения от игроков", big_string,
		"Выбрать", "Назад");
	big_string[0] = EOS;
	return true;
}

forward CheckValidReport(playerid, callback_type, temp_value[]);
public CheckValidReport(playerid, callback_type, temp_value[]){
	new rows;
	cache_get_row_count(rows);

	switch(callback_type){
		case REPORT_CALLBACK_ACTION_LIST:{
			if(!rows){
				SendErrorMessage(playerid, "Данного репорта не существует.");
				DeletePVar(playerid, "Report_ID");
				callcmd::rep(playerid);
				return false;
			}

			Dialog_Open(playerid, Dialog:D_REPORT_ADMIN_ACTION, DIALOG_STYLE_LIST,
				"Выберите действие",
				""cl_blue"- "cl_white"Ответить на обращение\n\
				"cl_blue"- "cl_white"Удалить обращение",
				"Выбрать", "Назад");
		}

		case REPORT_CALLBACK_ENTER_ANSWER:{
			if(!rows){
				SendErrorMessage(playerid, "Данного репорта не существует.");
				DeletePVar(playerid, "Report_ID");
				callcmd::rep(playerid);
				return false;
			}

			new temp_name[MAX_PLAYER_NAME], 
				temp_type, 
				temp_text[256], 
				temp_timestamp,
				temp_year, 
				temp_month, 
				temp_day, 
				temp_hour, 
				temp_minute,
				temp_second,
				player_status[32];

			cache_get_value_name(0, "name", temp_name);
			cache_get_value_name_int(0, "type", temp_type);
			cache_get_value_name(0, "text", temp_text);
			cache_get_value_name_int(0, "timestamp", temp_timestamp);

			ConvertTime(temp_timestamp, temp_year, temp_month, temp_day, temp_hour, temp_minute, temp_second);

			if(GetPlayerId(temp_name) != INVALID_PLAYER_ID){
				format(player_status, sizeof(player_status), "ID: %d", GetPlayerId(temp_name));
			}
			else format(player_status, sizeof(player_status), "Offline");

			big_string[0] = EOS;
			format(big_string, sizeof(big_string),
				""cl_blue"- "cl_white"%s от игрока %s "cl_grey"(%s)"cl_white":\n\
				\\c"cl_grey"["cl_light_red"%s"cl_grey"]\n\n\
				"cl_grey"Дата обращения: "cl_white"%02d.%02d.%d\n\
				"cl_grey"Время обращения: "cl_white"%02d:%02d:%02d",
				(temp_type == REPORT_TYPE_QUESTION) ? ("Вопрос") : ("Жалоба"),
				temp_name, player_status, temp_text,
				temp_day, temp_month, temp_year, temp_hour, temp_minute, temp_second);

			Dialog_Open(playerid, Dialog:D_REPORT_ADMIN_ANSWER, DIALOG_STYLE_INPUT,
				"Введите ответ", big_string,
				"Отправить", "Назад");
			big_string[0] = EOS;
		}

		case REPORT_CALLBACK_DELETE:{
			if(!rows){
				SendErrorMessage(playerid, "Данного репорта не существует.");
				DeletePVar(playerid, "Report_ID");
				callcmd::rep(playerid);
				return false;
			}

			new temp_name[MAX_PLAYER_NAME], 
				temp_type, 
				temp_text[256];

			cache_get_value_name(0, "name", temp_name);
			cache_get_value_name_int(0, "type", temp_type);
			cache_get_value_name(0, "text", temp_text);

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"[A] %s[%d] удалил(-а) %s от игрока %s:",
				GetPlayerData(playerid, p_name), playerid, 
				(temp_type == REPORT_TYPE_QUESTION) ? ("вопрос") : ("жалобу"),
				temp_name);
			Admin:SendAdminMessage(COLOR_LIME, small_string);
			Admin:SendAdminMessage(COLOR_WHITE, temp_text);
			small_string[0] = EOS;

			static const fmt_query[] = "DELETE FROM reports WHERE id = %d";
			new query[sizeof(fmt_query) + (-2 + 11)];
			mysql_format(mysql, query, sizeof(query), fmt_query, GetPVarInt(playerid, "Report_ID"));
			mysql_tquery(mysql, query);

			DeletePVar(playerid, "Report_ID");
			callcmd::rep(playerid);
		}

		case REPORT_CALLBACK_SEND_ANSWER:{
			if(!rows){
				SendErrorMessage(playerid, "Данного репорта не существует.");
				DeletePVar(playerid, "Report_ID");
				callcmd::rep(playerid);
				return false;
			}

			new temp_name[MAX_PLAYER_NAME], 
				temp_type;

			static const fmt_query[] = "UPDATE reports SET answer = '%s', admin_account_id = %d, status = %d, answer_timestamp = %d WHERE id = %d";
			new query[sizeof(fmt_query) + ((-2 + 11) * 4) + (-2 + 256)];
			mysql_format(mysql, query, sizeof(query), fmt_query,
				temp_value, GetPlayerData(playerid, p_id), REPORT_STATUS_ANSWERED, gettime(), GetPVarInt(playerid, "Report_ID"));
			mysql_tquery(mysql, query);

			cache_get_value_name(0, "name", temp_name);
			cache_get_value_name_int(0, "type", temp_type);

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"[A] %s[%d] ответил(-а) на %s %s:",
				GetPlayerData(playerid, p_name), playerid, 
				(temp_type == REPORT_TYPE_QUESTION) ? ("вопрос") : ("жалобу"),
				temp_name);
			Admin:SendAdminMessage(COLOR_LIME, small_string);
			Admin:SendAdminMessage(COLOR_WHITE, temp_value);
			small_string[0] = EOS;

			if(GetPlayerId(temp_name) != INVALID_PLAYER_ID){
				Report:CheckPlayerAnswers(GetPlayerId(temp_name));
			}
		}
	}

	return true;
}

forward CheckPlayerAnswers(playerid);
public CheckPlayerAnswers(playerid){
	new rows;
	cache_get_row_count(rows);
	if(!rows) return false;

	new temp_answered_count;
	cache_get_value_name_int(0, "answered_count", temp_answered_count);
	if(!temp_answered_count) return false;

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"У Вас есть "cl_white"%d "cl_lime"непрочитанных ответов на обращения от администрации. Используйте "cl_white"'/rep'"cl_lime".",
		temp_answered_count);
	SendClientMessage(playerid, COLOR_LIME, small_string);
	small_string[0] = EOS;

	PlayerPlaySound(playerid, 30800, 0.0, 0.0, 0.0);
	return true;
}

forward ShowSelectType(playerid);
public ShowSelectType(playerid){
	new temp_answered_count;
	cache_get_value_name_int(0, "answered_count", temp_answered_count);

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Вопрос по игре\n\
		"cl_blue"- "cl_white"Жалоба\n\
		"cl_blue"- "cl_white"Просмотреть ответы на обращения "cl_grey"[Новых: %d]",
		temp_answered_count);

	Dialog_Open(playerid, Dialog:D_REPORT_SELECT_TYPE, DIALOG_STYLE_LIST,
		"Выберите тип обращения", big_string,
		"Далее", "Закрыть");
	
	big_string[0] = EOS;
	return true;
}

forward ShowPlayerReportList(playerid);
public ShowPlayerReportList(playerid){
	new rows;
	cache_get_row_count(rows);

	if(!rows){
		SendErrorMessage(playerid, "У Вас нет репортов.");
		Report:ShowSelectType(playerid);
		return false;
	}

	new temp_year, 
		temp_month, 
		temp_day, 
		temp_hour, 
		temp_minute;

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_grey"Текст\t"cl_grey"Тип\t"cl_grey"Статус\t"cl_grey"Дата и время подачи");

	for(new i, temp_text[256], temp_type, temp_status, temp_timestamp, status_str[64]; i < rows; i++){
		cache_get_value_name_int(i, "id", player_listitems[playerid][i]);
		cache_get_value_name(i, "text", temp_text);
		cache_get_value_name_int(i, "type", temp_type);
		cache_get_value_name_int(i, "status", temp_status);
		cache_get_value_name_int(i, "timestamp", temp_timestamp);

		switch(temp_status){
			case REPORT_STATUS_WAIT: format(status_str, sizeof(status_str), ""cl_light_red"Не отвечен");
			case REPORT_STATUS_ANSWERED: format(status_str, sizeof(status_str), ""cl_yellow"Отвечен (не просмотрен)");
			case REPORT_STATUS_CHECKED: format(status_str, sizeof(status_str), ""cl_green"Отвечен (просмотрен)");
		}

		ConvertTime(temp_timestamp, temp_year, temp_month, temp_day, temp_hour, temp_minute);

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t"cl_white"%s\t%s\t%02d.%02d.%d в %02d:%02d",
			big_string, i + 1, temp_text,
			(temp_status == REPORT_TYPE_QUESTION) ? ("Вопрос") : ("Жалоба"),
			status_str,
			temp_day, temp_month, temp_year, temp_hour, temp_minute);
	}

	Dialog_Open(playerid, Dialog:D_REPORTS_LIST, DIALOG_STYLE_TABLIST_HEADERS,
		"Список обращений", big_string,
		"Выбрать", "Назад");
	big_string[0] = EOS;
	return true;
}

forward ShowReportInfo(playerid, type);
public ShowReportInfo(playerid, type){
	new rows;
	cache_get_row_count(rows);

	if(!rows){
		SendErrorMessage(playerid, "Прозошла ошибка.");
		Report:ShowPlayerReportList(playerid);
		return false;
	}

	switch(type){
		case 0:{
			new temp_status;
			cache_get_value_name_int(0, "status", temp_status);

			if(temp_status > REPORT_STATUS_WAIT){
				static const fmt_query[] = "SELECT a.name, r.text, r.status, r.type, r.timestamp, r.answer, r.answer_timestamp FROM reports r INNER JOIN accounts a ON a.id = r.admin_account_id WHERE r.id = %d";
				new query[sizeof(fmt_query) + (-2 + 11)];
				mysql_format(mysql, query, sizeof(query), fmt_query, GetPVarInt(playerid, "Report_ID"));
				mysql_tquery(mysql, query, "ShowReportInfo", "ii", playerid, 1);
				return false;
			}

			new temp_text[256],
				temp_type,
				temp_timestamp,
				temp_year, 
				temp_month, 
				temp_day, 
				temp_hour, 
				temp_minute;

			cache_get_value_name(0, "text", temp_text);
			cache_get_value_name_int(0, "type", temp_type);
			cache_get_value_name_int(0, "timestamp", temp_timestamp);

			ConvertTime(temp_timestamp, temp_year, temp_month, temp_day, temp_hour, temp_minute);
 
			big_string[0] = EOS;
			format(big_string, sizeof(big_string),
				""cl_blue"Текст %s: "cl_white"%s\n\
				"cl_blue"Статус: "cl_light_red"Не отвечен\n\
				"cl_blue"Дата и время подачи: "cl_white"%02d.%02d.%d в %02d:%02d\n\n\
				\\c"cl_grey"[Ожидайте ответа от администрации]",
				(temp_type == REPORT_TYPE_QUESTION) ? ("вопроса") : ("жалобы"),
				temp_text,
				temp_day, temp_month, temp_year, temp_hour, temp_minute);

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"Обращение №%d", GetPVarInt(playerid, "Report_ID"));

			Dialog_Open(playerid, Dialog:D_REPORT_INFO, DIALOG_STYLE_MSGBOX,
				small_string, big_string,
				"Ок", "");

			big_string[0] = EOS;
			small_string[0] = EOS;
		}
		case 1:{
			new temp_status;
			cache_get_value_name_int(0, "status", temp_status);

			if(temp_status == REPORT_STATUS_ANSWERED){
				temp_status = REPORT_STATUS_CHECKED;

				static const fmt_query[] = "UPDATE reports SET status = %d WHERE id = %d";
				new query[sizeof(fmt_query) + ((-2 + 11) * 2)];
				mysql_format(mysql, query, sizeof(query), fmt_query, REPORT_STATUS_CHECKED, GetPVarInt(playerid, "Report_ID"));
				mysql_tquery(mysql, query);
			}

			new temp_text[256],
				temp_type,
				temp_timestamp,
				temp_answer[256],
				temp_answer_timestamp,
				temp_name[MAX_PLAYER_NAME],
				temp_year, 
				temp_month, 
				temp_day, 
				temp_hour, 
				temp_minute,
				temp_answer_year, 
				temp_answer_month, 
				temp_answer_day, 
				temp_answer_hour, 
				temp_answer_minute;

			cache_get_value_name(0, "text", temp_text);
			cache_get_value_name_int(0, "type", temp_type);
			cache_get_value_name_int(0, "timestamp", temp_timestamp);
			cache_get_value_name(0, "name", temp_name);
			cache_get_value_name(0, "answer", temp_answer);
			cache_get_value_name_int(0, "answer_timestamp", temp_answer_timestamp);

			ConvertTime(temp_timestamp, temp_year, temp_month, temp_day, temp_hour, temp_minute);
			ConvertTime(temp_answer_timestamp, temp_answer_year, temp_answer_month, temp_answer_day, temp_answer_hour, temp_answer_minute);
 
			big_string[0] = EOS;
			format(big_string, sizeof(big_string),
				""cl_blue"Текст %s: "cl_white"%s\n\
				"cl_blue"Статус: "cl_green"Отвечен (просмотрен)\n\
				"cl_blue"Дата и время подачи: "cl_white"%02d.%02d.%d в %02d:%02d\n\n\
				"cl_blue"Ответ от администратора %s: "cl_white"%s\n\
				"cl_blue"Дата и время ответа: "cl_white"%02d.%02d.%d в %02d:%02d",
				(temp_type == REPORT_TYPE_QUESTION) ? ("вопроса") : ("жалобы"),
				temp_text,
				temp_day, temp_month, temp_year, temp_hour, temp_minute,
				temp_name, temp_answer,
				temp_answer_day, temp_answer_month, temp_answer_year, temp_answer_hour, temp_answer_minute);

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"Обращение №%d", GetPVarInt(playerid, "Report_ID"));

			Dialog_Open(playerid, Dialog:D_REPORT_INFO, DIALOG_STYLE_MSGBOX,
				small_string, big_string,
				"Ок", "");

			big_string[0] = EOS;
			small_string[0] = EOS;
		}
	}
	return true;
}