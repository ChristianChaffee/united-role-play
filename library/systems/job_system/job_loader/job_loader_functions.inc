
stock Job:ShowLoaderDialog(playerid){
	if(GetPlayerData(playerid, p_nationality) != 1 && GetPlayerData(playerid, p_level) < NATIONALITY_LEVEL){
		SendErrorMessage(playerid, "У Вас нет гражданства США для устройства на работу грузчика.");
		return false;
	}

	new players_count;
	for(new i, g = GetPlayerPoolSize(); i <= g; i++){
		if(GetPlayerData(i, p_job) == JOB_LOADER && player_logged[i]) players_count++;
	}

	if(players_count >= JOB_LOADER_MAX_PLAYERS){
		SendErrorMessage(playerid, "В данный момент на складе нет свободных вакансий. Приходите позже.");
		return false;
	}

	if(GetPlayerData(playerid, p_job) == JOB_LOADER){
		Dialog_Open(playerid, Dialog:D_JOB_LOADER_UNINVITE, DIALOG_STYLE_MSGBOX,
			"Начальник склада", 
			""cl_blue"- "cl_white"Вы действительно хотите уволиться с работы грузчика?",
			"Да", "Нет");
		return false;
	}

	Dialog_Open(playerid, Dialog:D_JOB_LOADER_INVITE, DIALOG_STYLE_MSGBOX,
		"Начальник склада", 
		""cl_blue"- "cl_white"Вы хотите устроиться на работу на наш склад? Работа заключается в том,\n\
		чтобы переносить ящика с Ж/Д платформы на склад. Оплата статичная и составляет "cl_green""#JOB_LOADER_STATIC_PAY"$"cl_white".",
		"Да", "Нет");
	return true;
}

stock Job:GenerateLoaderLoad(playerid){
	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отправляйтесь на Ж/Д платформу за ящиком. Она помечена на Вашей мини-карте.");

	SetPVarInt(playerid, "JobLoader_Stage", JOB_LOADER_STAGE_TO_BOX);
	new random_box_pos = random(sizeof(job_loader_boxes_pos));

	SetPlayerCheckpoint(playerid, 
		job_loader_boxes_pos[random_box_pos][0],
		job_loader_boxes_pos[random_box_pos][1],
		job_loader_boxes_pos[random_box_pos][2],
		1.0);
	return true;
}

stock Job:GenerateLoaderUnload(playerid){
	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отправляйтесь на склад для разгрузки. Он помечен на Вашей мини-карте.");

	SetPVarInt(playerid, "JobLoader_Stage", JOB_LOADER_STAGE_TO_WAREHOUSE);
	new random_box_pos = random(sizeof(job_loader_warehouse_pos));

	SetPlayerCheckpoint(playerid, 
		job_loader_warehouse_pos[random_box_pos][0],
		job_loader_warehouse_pos[random_box_pos][1],
		job_loader_warehouse_pos[random_box_pos][2],
		1.0);
	return true;
}

stock Job:LoaderUninvite(playerid){
	SetPlayerSkin(playerid, GetPlayerData(playerid, p_skin));

	RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	DeletePVar(playerid, "JobLoader_Stage");
	DeletePVar(playerid, "JobLoader_BoxesCount");

	SetPlayerData(playerid, p_job, JOB_NONE);
	DisablePlayerCheckpoint(playerid);

	SendClientMessage(playerid, COLOR_GREY, "Вы уволились с работы грузчика.");
	return true;
}