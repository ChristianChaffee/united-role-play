
forward LoadStartQuests();
public LoadStartQuests(){
	new rows, tick_time = GetTickCount();
	cache_get_row_count(rows);

	if(rows > START_QUESTS_COUNT){
		printf("[LoadStartQuests - ERROR]: Количество записей в базе данных превышает размер массива (%d > %d)", rows, START_QUESTS_COUNT);
		return false;
	}

	for(new i; i < rows; i++){
		cache_get_value_name_int(i, "id", GetStartQuestData(i + 1, sq_id));
		cache_get_value_name(i, "name", GetStartQuestData(i + 1, sq_name));
		cache_get_value_name(i, "description", GetStartQuestData(i + 1, sq_description));
		cache_get_value_name_int(i, "reward_money", GetStartQuestData(i + 1, sq_reward_money));
	}

	printf("[LoadStartQuests]: Стартовые квесты загружены <%d шт> <%d мс>", rows, GetTickCount() - tick_time);
	return true;
}

forward LoadPlayerStartQuests(playerid, quest_index);
public LoadPlayerStartQuests(playerid, quest_index){
	new rows;
	cache_get_row_count(rows);

	if(!rows || rows != START_QUESTS_COUNT){
		static const fmt_query[] = "SELECT id FROM start_player_quests WHERE account_id = %d AND quest_id = %d";
		new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];

		for(new i = quest_index; i <= START_QUESTS_COUNT; i++){
			mysql_format(mysql, query, sizeof(query), fmt_query,
				GetPlayerData(playerid, p_id), GetStartQuestData(i, sq_id));
			mysql_tquery(mysql, query, "IsPlayerHaveQuestInDB", "ii", playerid, i);
		}

		return false;
	}

	for(new i, temp_quest_id, temp_quest_index, in_progress_find; i < rows; i++){
		cache_get_value_name_int(i, "quest_id", temp_quest_id);
		temp_quest_index = StartQuest:GetIndexByID(temp_quest_id);
		if(temp_quest_index == -1) continue;

		SetPlayerStartQuestData(playerid, temp_quest_index, sqp_quest_id, temp_quest_id);

		cache_get_value_name_int(i, "id", GetPlayerStartQuestData(playerid, temp_quest_index, sqp_id));
		cache_get_value_name_int(i, "account_id", GetPlayerStartQuestData(playerid, temp_quest_index, sqp_account_id));
		cache_get_value_name_int(i, "status", GetPlayerStartQuestData(playerid, temp_quest_index, sqp_status));
		cache_get_value_name_int(i, "stage", GetPlayerStartQuestData(playerid, temp_quest_index, sqp_stage));

		if(temp_quest_index == SQ_WELCOME_TO_STATE){
			StartQuest:StartQuest(playerid, SQ_WELCOME_TO_STATE);
		}

		if(!in_progress_find && GetPlayerStartQuestData(playerid, temp_quest_index, sqp_status) == SQ_STATUS_IN_PROGRESS){
			StartQuest:UpdateQuest(playerid, temp_quest_index);
			in_progress_find = 1;
		}
	}
	return true;
}

forward IsPlayerHaveQuestInDB(playerid, quest_index);
public IsPlayerHaveQuestInDB(playerid, quest_index){
	new rows;
	cache_get_row_count(rows);

	if(!rows){
		static const fmt_query[] = "INSERT INTO start_player_quests (account_id, status, quest_id, timestamp) VALUES (%d, DEFAULT, %d, %d)";
		new query[sizeof(fmt_query) + (-2 + 11)];

		mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id), GetStartQuestData(quest_index, sq_id), gettime());
		mysql_tquery(mysql, query, 
			(quest_index == START_QUESTS_COUNT) ? ("LastPlayerQuestInsert") : (""), 
			(quest_index == START_QUESTS_COUNT) ? ("i") : (""),
			(quest_index == START_QUESTS_COUNT) ? (playerid) : (-1));
	}	
	else{
		static const fmt_query[] = "SELECT * FROM start_player_quests WHERE account_id = %d";
		new query[sizeof(fmt_query) + (-2 + 11)];
		mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
		mysql_tquery(mysql, query, "LoadPlayerStartQuests", "ii", playerid, quest_index);
	}
	return true;
}

forward LastPlayerQuestInsert(playerid);
public LastPlayerQuestInsert(playerid) { StartQuest:LoadPlayerStartQuests(playerid); }