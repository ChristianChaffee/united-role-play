
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