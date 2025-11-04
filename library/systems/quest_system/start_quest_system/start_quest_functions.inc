
stock StartQuest:LoadPlayerStartQuests(playerid){
	static const fmt_query[] = "SELECT * FROM start_player_quests WHERE account_id = %d";
	new query[sizeof(fmt_query) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerData(playerid, p_id));
	mysql_tquery(mysql, query, "LoadPlayerStartQuests", "ii", playerid, 1);
	return true;
}

stock StartQuest:GetIndexByID(quest_id){
	for(new i = 1; i <= START_QUESTS_COUNT; i++){
		if(start_quest_info[i][sq_id] == quest_id) return i;
	}
	return -1;
}

//==============================================================================
stock StartQuest:StartQuest(playerid, quest_index){
	if(quest_index <= 0 || quest_index > START_QUESTS_COUNT) return false;
	if(GetPlayerStartQuestData(playerid, quest_index, sqp_status) != SQ_STATUS_NONE) return false;
	if(!GetPlayerStartQuestData(playerid, quest_index, sqp_id)) return false;

	switch(quest_index){
		case SQ_WELCOME_TO_STATE:{
			new slot_index = Inventory:GetFreeSlotIndex(playerid);
			if(slot_index == -1) return false;

			Inventory:GiveItem(playerid, slot_index, ITEM_LETTER_ID, _, _, quest_index, "Добро пожаловать в штат United!\nУ меня тут есть старый друг, ищи его на автовокзале,\nты сразу его узнаешь!\nС уважением, Билл", 0);

			SendClientMessage(playerid, COLOR_YELLOW, "Вам передали письмо! Просмотрите его через инвентарь.");
			SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Откройте чат клавишей F6 и введите команду /inv");
		}
		case SQ_FIRST_MONEY:{
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"Отправляйтесь на любую из работ и заработайте %d$.",
				GetStartQuestData(quest_index, sq_points));
			SendClientMessage(playerid, COLOR_YELLOW, small_string);
			small_string[0] = EOS;

			SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Посмотреть доступные работы можно в '/gps'.");
		}
		case SQ_THIRD_LEVEL:{
			if(GetPlayerData(playerid, p_level) >= 3){
				SetPlayerStartQuestData(playerid, quest_index, sqp_status, SQ_STATUS_IN_PROGRESS);
				StartQuest:FinishQuest(playerid, quest_index);
				return true;
			}
			else{
				SendClientMessage(playerid, COLOR_YELLOW, "Получите 3-й игровой уровень.");
				SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Поднять уровень можно накопив опыт (exp). Он копиться на работах и каждый PayDay.");
			}
		}
		case SQ_DRIVING_LICENSE:{
			SendClientMessage(playerid, COLOR_YELLOW, "Джонни хочет поговорить с Вами. Он отмечен на Вашей мини-карте.");
			GPSEnable(playerid, SQ_JONNY_POS);
		}
	}

	SetPlayerStartQuestData(playerid, quest_index, sqp_status, SQ_STATUS_IN_PROGRESS);
	
	static const fmt_query[] = "UPDATE start_player_quests SET status = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, SQ_STATUS_IN_PROGRESS, GetPlayerStartQuestData(playerid, quest_index, sqp_id));
	mysql_tquery(mysql, query);
	return true;
}

stock StartQuest:FinishQuest(playerid, quest_index){
	if(quest_index <= 0 || quest_index > START_QUESTS_COUNT) return false;
	if(GetPlayerStartQuestData(playerid, quest_index, sqp_status) != SQ_STATUS_IN_PROGRESS) return false;
	if(!GetPlayerStartQuestData(playerid, quest_index, sqp_id)) return false;

	SetPlayerStartQuestData(playerid, quest_index, sqp_status, SQ_STATUS_COMPLETED);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		"Квест \"%s\" завершён! Ваша награда %d$.",
		GetStartQuestData(quest_index, sq_name), GetStartQuestData(quest_index, sq_reward_money));
	SendClientMessage(playerid, COLOR_YELLOW, small_string);
	small_string[0] = EOS;

	GiveMoney(playerid, GetStartQuestData(quest_index, sq_reward_money));

	static const fmt_query[] = "UPDATE start_player_quests SET status = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, SQ_STATUS_COMPLETED, GetPlayerStartQuestData(playerid, quest_index, sqp_id));
	mysql_tquery(mysql, query);

	OnPlayerFinishStartQuest(playerid, quest_index);
	return true;
}

stock StartQuest:SetStage(playerid, quest_index, stage){
	if(quest_index <= 0 || quest_index > START_QUESTS_COUNT) return false;
	if(GetPlayerStartQuestData(playerid, quest_index, sqp_status) != SQ_STATUS_IN_PROGRESS) return false;
	if(!GetPlayerStartQuestData(playerid, quest_index, sqp_id)) return false;

	SetPlayerStartQuestData(playerid, quest_index, sqp_stage, stage);
	StartQuest:UpdateQuest(playerid, quest_index);

	static const fmt_query[] = "UPDATE start_player_quests SET stage = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, stage, GetPlayerStartQuestData(playerid, quest_index, sqp_id));
	mysql_tquery(mysql, query);
	return true;
}

stock StartQuest:GivePoints(playerid, quest_index, points){
	if(quest_index <= 0 || quest_index > START_QUESTS_COUNT) return false;
	if(GetPlayerStartQuestData(playerid, quest_index, sqp_status) != SQ_STATUS_IN_PROGRESS) return false;
	if(!GetPlayerStartQuestData(playerid, quest_index, sqp_id)) return false;

	start_quest_player_info[playerid][quest_index][sqp_points] += points;
	if(GetPlayerStartQuestData(playerid, quest_index, sqp_points) >= GetStartQuestData(quest_index, sq_points)){
		StartQuest:FinishQuest(playerid, quest_index);
	}
	else{
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Прогресс квеста \"%s\": "cl_white"%d/%d.",
			GetStartQuestData(quest_index, sq_name),
			GetPlayerStartQuestData(playerid, quest_index, sqp_points),
			GetStartQuestData(quest_index, sq_points));
		SendClientMessage(playerid, COLOR_YELLOW, small_string);
		small_string[0] = EOS;
	}

	static const fmt_query[] = "UPDATE start_player_quests SET points = %d WHERE id = %d";
	new query[sizeof(fmt_query) + (-2 + 11) + (-2 + 11)];
	mysql_format(mysql, query, sizeof(query), fmt_query, GetPlayerStartQuestData(playerid, quest_index, sqp_points), GetPlayerStartQuestData(playerid, quest_index, sqp_id));
	mysql_tquery(mysql, query);
	return true;
}

stock StartQuest:UpdateQuest(playerid, quest_index){
	if(quest_index <= 0 || quest_index > START_QUESTS_COUNT) return false;
	if(GetPlayerStartQuestData(playerid, quest_index, sqp_status) != SQ_STATUS_IN_PROGRESS) return false;
	if(!GetPlayerStartQuestData(playerid, quest_index, sqp_id)) return false;

	switch(quest_index){
		case SQ_WELCOME_TO_STATE:{

			switch(GetPlayerStartQuestData(playerid, quest_index, sqp_stage)){
				case SQ_STAGE_WAIT_LETTER:{
					new letter_find;
					for(new i; i < MAX_INVENTORY_SLOTS; i++){
						if(GetPlayerInventoryItemData(playerid, i, inv_player_id)
							&& GetPlayerInventoryItemData(playerid, i, inv_player_item_id) == ITEM_LETTER_ID
							&& GetPlayerInventoryItemData(playerid, i, inv_player_value_two) == SQ_WELCOME_TO_STATE){
							letter_find = 1;
							break;
						}
					}

					if(!letter_find){
						new slot_index = Inventory:GetFreeSlotIndex(playerid);
						if(slot_index == -1){
							SendErrorMessage(playerid, "У Вас нет места в инвнтаре для продолжения квеста. Очистите место в инвнтаре и обновите квест через '/startquest'");
							return false;
						}

						Inventory:GiveItem(playerid, slot_index, ITEM_LETTER_ID, _, _, quest_index, "Добро пожаловать в штат United!\nУ меня тут есть старый друг, ищи его на автовокзале,\nты сразу его узнаешь!\nС уважением, Билл", 0);
					}

					SendClientMessage(playerid, COLOR_YELLOW, "Вам передали письмо! Просмотрите его через инвентарь.");
					SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Откройте чат клавишей F6 и введите команду /inv");
				}
				case SQ_STAGE_FIND_FRIEND:{
					SendClientMessage(playerid, COLOR_YELLOW, "Знакмоый Джонни отмечен на Вашем GPS!");
					GPSEnable(playerid, SQ_JONNY_POS);
				}
			}
		}

		case SQ_FIRST_MONEY:{
			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"Отправляйтесь на любую из работ и заработайте %d$. Заработано: %d$ из %d$.",
				GetStartQuestData(quest_index, sq_points),
				GetPlayerStartQuestData(playerid, quest_index, sqp_points),
				GetStartQuestData(quest_index, sq_points));
			SendClientMessage(playerid, COLOR_YELLOW, small_string);
			small_string[0] = EOS;

			SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Посмотреть доступные работы можно в '/gps'.");
		}

		case SQ_THIRD_LEVEL:{
			if(GetPlayerData(playerid, p_level) >= 3){
				StartQuest:FinishQuest(playerid, SQ_THIRD_LEVEL);
			}
			else{
				SendClientMessage(playerid, COLOR_YELLOW, "Получите 3-й игровой уровень.");
				SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Поднять уровень можно накопив опыт (exp). Он копиться на работах и каждый PayDay.");
			}
		}

		case SQ_DRIVING_LICENSE:{

			switch(GetPlayerStartQuestData(playerid, quest_index, sqp_stage)){
				case SQ_STAGE_DRIVING_TO_JONNY:{
					SendClientMessage(playerid, COLOR_YELLOW, "Джонни хочет поговорить с Вами. Он отмечен на Вашей мини-карте.");
					GPSEnable(playerid, SQ_JONNY_POS);
				}
				case SQ_STAGE_DRIVING_TO_LC:{
					SendClientMessage(playerid, COLOR_YELLOW, "Отправляйтесь к центру лицензирования. Он отмечен на Вашей мини-карте");
					GPSEnable(playerid, SQ_LICENSING_CENTER_POS);
				}
				case SQ_STAGE_DRIVING_TO_RUBY:{
					SendClientMessage(playerid, COLOR_YELLOW, "Найдите Руби в центре лицензирования. Обычно он за стойкой рецепшена.");
				}
				case SQ_STAGE_DRIVING_DRIVE:{
					SendClientMessage(playerid, COLOR_YELLOW, "Успешно пройдите практический экзамен.");
				}
			}
		}
	}
	return true;
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

stock StartQuest:ShowJonnyDialog(playerid){
	if(GetPlayerStartQuestData(playerid, SQ_WELCOME_TO_STATE, sqp_status) == SQ_STATUS_IN_PROGRESS
		&& GetPlayerStartQuestData(playerid, SQ_WELCOME_TO_STATE, sqp_stage) == SQ_STAGE_FIND_FRIEND){

		Dialog_Open(playerid, Dialog:D_START_QUESTS_JONNY_1, DIALOG_STYLE_MSGBOX,
			"Джонни", 
			""cl_blue"- "cl_white"Привет, ты должно быть от Билла?",
			"Да", "Нет");
	}
	else if(GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_status) == SQ_STATUS_IN_PROGRESS
		&& GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_stage) == SQ_STAGE_DRIVING_TO_JONNY){

		Dialog_Open(playerid, Dialog:D_START_QUESTS_JONNY_7, DIALOG_STYLE_MSGBOX,
			"Джонни", 
			""cl_blue"- "cl_white"Вижу ты уже освоился в нашем штате, не так ли?",
			"Да", "Нет");
	}
	return true;
}

stock StartQuest:ShowJonnySelectCountry(playerid){
	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Откуда ты пожаловал к нам?\n\
		"cl_grey"["cl_white"Я из прекрасной страны под названием %s"cl_grey"]\n\
		"cl_grey"["cl_white"Какая тебе разница?"cl_grey"]",
		character_nationality[GetPlayerData(playerid, p_nationality)]);

	Dialog_Open(playerid, Dialog:D_START_QUESTS_JONNY_2, DIALOG_STYLE_LIST,
		"Джонни", 
		big_string,
		"Ответить", "");
	big_string[0] = EOS;
	return true;
}

stock StartQuest:ShowJonnyExit(playerid){
	Dialog_Open(playerid, Dialog:D_NULL, DIALOG_STYLE_MSGBOX,
		"Джонни", 
		""cl_blue"- "cl_white"Тогда оставиь меня в покое и проваливай.",
		"Ок", "");
	return true;
}

stock StartQuest:ShowJonnyIlligalJob(playerid){
	Dialog_Open(playerid, Dialog:D_START_QUESTS_JONNY_6, DIALOG_STYLE_MSGBOX,
		"Джонни", 
		""cl_blue"- "cl_white"Но ты можешь попробовать не совсем легальную работёнку::\n\
		\\c"cl_grey"- Доставщик сахарка\n\
		\\c"cl_grey"- Собиратель оружия\n\n\
		"cl_blue"- "cl_white"Найти ты эти работы можешь в любое время на своём GPS "cl_grey"(/gps)",
		"Хорошо", "");
	return true;
}

stock OnPlayerFinishStartQuest(playerid, quest_index){
	switch(quest_index){
		case SQ_FIRST_MONEY: StartQuest:StartQuest(playerid, SQ_THIRD_LEVEL);
	}
	return 1;
}