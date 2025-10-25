
stock StartQuest:LoadPlayerStartQuests(playerid){
	static const fmt_query[] = "SELECT * FROM start_player_quests WHERE account_id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query, "LoadPlayerStartQuests", "ii", playerid, 1);
}

stock StartQuest:GetIndexByID(quest_id){
	for(new i = 1; i <= START_QUESTS_COUNT; i++){
		if(start_quest_info[i][sq_id] == quest_id) return i;
	}
	return -1;
}

//==============================================================================
stock StartQuest:ShowQuestMenu(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string), ""cl_grey"Название квеста\t"cl_grey"Статус");
	
	new count;
	for(new i, quest_status[32]; i <= START_QUESTS_COUNT; i++){
		if(!GetPlayerStartQuestData(playerid, i, sqp_id)) continue;

		switch(GetPlayerStartQuestData(playerid, i, sqp_status)){
			case SQ_STATUS_NONE: format(quest_status, sizeof(quest_status), ""cl_dark_red"Не начат");
			case SQ_STATUS_IN_PROGRESS: format(quest_status, sizeof(quest_status), ""cl_gold"В процессе");
			case SQ_STATUS_COMPLETED: format(quest_status, sizeof(quest_status), ""cl_green"Выполнен");
		}

		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_blue"%d. "cl_white"%s\t"cl_grey"[%s"cl_grey"]",
			big_string, i,
			GetStartQuestData(i, sq_name), quest_status);

		player_listitems[playerid][count] = i;
		count++;
	}

	if(!count){
		big_string[0] = EOS;
		SendErrorMessage(playerid, "В данный момент нет доступных стартовых квестов.");
		return false;
	}

	Dialog_Open(playerid, Dialog:D_START_QUESTS_MAIN_MENU, DIALOG_STYLE_TABLIST_HEADERS,
		"Стартовые квесты", big_string,
		"Далее", "Закрыть");
	big_string[0] = EOS;
	return true;
}