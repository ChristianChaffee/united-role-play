
DialogResponse:D_START_QUESTS_JONNY_1(playerid, response, listitem, inputtext[]){
	if(!response) StartQuest:ShowJonnyExit(playerid);
	else StartQuest:ShowJonnySelectCountry(playerid);
}

DialogResponse:D_START_QUESTS_JONNY_2(playerid, response, listitem, inputtext[]){
	switch(listitem){
		case 0: StartQuest:ShowJonnySelectCountry(playerid);
		case 1:{
			if(GetPlayerData(playerid, p_nationality) != 1){
				Dialog_Open(playerid, Dialog:D_START_QUESTS_JONNY_4, DIALOG_STYLE_MSGBOX,
					"Джонни", 
					""cl_blue"- "cl_white"Беда, на легальную работу без паспорта гражданина США нынче не устроишься.",
					"Да", "Нет");
			}
			else{
				Dialog_Open(playerid, Dialog:D_START_QUESTS_JONNY_3, DIALOG_STYLE_MSGBOX,
					"Джонни", 
					""cl_blue"- "cl_white"Отлично, тогда с бумажками проблем не будет.",
					"Далее", "");
			}
		}
		case 2: StartQuest:ShowJonnyExit(playerid);
	}
}

DialogResponse:D_START_QUESTS_JONNY_3(playerid, response, listitem, inputtext[]){
	Dialog_Open(playerid, Dialog:D_START_QUESTS_JONNY_4, DIALOG_STYLE_MSGBOX,
		"Джонни", 
		""cl_blue"- "cl_white"Для начала советую тебе эти легальные работы:\n\
		\\c"cl_grey"- Грузчик на складе\n\
		\\c"cl_grey"- Лесоруб\n\
		\\c"cl_grey"- Сортировщик мусора\n\n\
		"cl_blue"- "cl_white"Найти ты эти работы можешь в любое время на своём GPS "cl_grey"(/gps)",
		"Далее", "");
}

DialogResponse:D_START_QUESTS_JONNY_4(playerid, response, listitem, inputtext[]){
	StartQuest:ShowJonnyIlligalJob(playerid);
}

DialogResponse:D_START_QUESTS_JONNY_5(playerid, response, listitem, inputtext[]){
	StartQuest:ShowJonnyIlligalJob(playerid);
}

DialogResponse:D_START_QUESTS_JONNY_6(playerid, response, listitem, inputtext[]){
	StartQuest:FinishQuest(playerid, SQ_WELCOME_TO_STATE);
	StartQuest:StartQuest(playerid, SQ_FIRST_MONEY);
}

DialogResponse:D_START_QUESTS_JONNY_7(playerid, response, listitem, inputtext[]){
	if(response){
		Dialog_Open(playerid, Dialog:D_START_QUESTS_JONNY_8, DIALOG_STYLE_MSGBOX,
			"Джонни", 
			""cl_blue"- "cl_white"Но есть одно но, ты пока что не можешь легально управлять транспортом, исправим это?",
			"Да", "");
	}
}

DialogResponse:D_START_QUESTS_JONNY_8(playerid, response, listitem, inputtext[]){
	Dialog_Open(playerid, Dialog:D_START_QUESTS_JONNY_9, DIALOG_STYLE_MSGBOX,
		"Джонни", 
		""cl_blue"- "cl_white"Супер, тогда отправляйся в центр лицензирования, он находиться в Сан-Фиерро и поговори там с Руби.",
		"Хорошо", "");
}

DialogResponse:D_START_QUESTS_JONNY_9(playerid, response, listitem, inputtext[]){
	StartQuest:SetStage(playerid, SQ_DRIVING_LICENSE, SQ_STAGE_DRIVING_TO_LC);
}

DialogResponse:D_START_QUESTS_RUBY_1(playerid, response, listitem, inputtext[]){
	if(player_info[playerid][p_licenses][0] && player_info[playerid][p_licenses_times][0] > 0){
		Dialog_Open(playerid, Dialog:D_START_QUESTS_RUBY_2, DIALOG_STYLE_MSGBOX,
			"Джонни", 
			""cl_blue"- "cl_white"Я смотрю у тебя уже есть водительская лицензия. В таком случае\n\
			зря ты ко мне приехал. Ну ничего, бывает.",
			"Хорошо", "");
	}
	else{
		Dialog_Open(playerid, Dialog:D_START_QUESTS_RUBY_3, DIALOG_STYLE_MSGBOX,
			"Джонни", 
			""cl_blue"- "cl_white"Я сделаю для тебя бесплатный практический экзамен. Готов начинать?",
			"Да", "Нет");
	}
}

DialogResponse:D_START_QUESTS_RUBY_2(playerid, response, listitem, inputtext[]){
	StartQuest:FinishQuest(playerid, SQ_DRIVING_LICENSE);
}

DialogResponse:D_START_QUESTS_RUBY_3(playerid, response, listitem, inputtext[]){
	if(!response){
		Dialog_Open(playerid, Dialog:D_NULL, DIALOG_STYLE_MSGBOX,
			"Джонни", 
			""cl_blue"- "cl_white"Хорошо. Возвращайся как будешь готов!",
			"Спасибо", "");
	}
	else{
		StartQuest:SetStage(playerid, SQ_DRIVING_LICENSE, SQ_STAGE_DRIVING_DRIVE);
		SetPVarInt(playerid, "LicensingCenter_Index", 0);
		LicensingCenter:StartExam(playerid);
	}
}