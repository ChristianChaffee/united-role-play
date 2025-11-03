
stock Job:ShowGSDialog(playerid){
	if(GetPlayerData(playerid, p_nationality) != 1 && GetPlayerData(playerid, p_level) < NATIONALITY_LEVEL){
		SendErrorMessage(playerid, "У Вас нет гражданства США для устройства на работу сортировщика мусора.");
		return false;
	}

	new players_count;
	for(new i, g = GetPlayerPoolSize(); i <= g; i++){
		if(GetPlayerData(i, p_job) == JOB_GARBAGE_SORTER && player_logged[i]) players_count++;
	}

	if(players_count >= JOB_GS_MAX_PLAYERS){
		SendErrorMessage(playerid, "В данный момент на свалке нет свободных вакансий. Приходите позже.");
		return false;
	}

	if(GetPlayerData(playerid, p_job) == JOB_GARBAGE_SORTER){
		Dialog_Open(playerid, Dialog:D_JOB_GS_UNINVITE, DIALOG_STYLE_MSGBOX,
			"Начальник свалки", 
			""cl_blue"- "cl_white"Вы действительно хотите уволиться с работы сортировщика мусора?",
			"Да", "Нет");
		return false;
	}

	big_string[0] = EOS;
	format(big_string, sizeof(big_string),
		""cl_blue"- "cl_white"Вы хотите устроиться на работу на свалку? Работа заключается в том,\n\
		чтобы cобирать мусор определенного типа, не перемешивая его, а затем сдавать на переработку.\n");

	for(new i; i < GS_GARBAGE_MAX_TYPE; i++){
		format(big_string, sizeof(big_string),
			"%s\n\
			"cl_grey"%d. Цена за 1 кг %s: "cl_green"%d$",
			big_string, i + 1,
			gs_garbage_name_type[i][1], gs_garbage_type_prices[i]);
	}

	Dialog_Open(playerid, Dialog:D_JOB_GS_INVITE, DIALOG_STYLE_MSGBOX,
		"Начальник свалки", big_string,
		"Да", "Нет");
	return true;
}

stock Job:GSCreateGarbage(garbage_index){
	if(garbage_index < 0 || garbage_index >= JOB_GS_GARBAGE_COUNT) return false;

	if(IsValidDynamicObject(gs_garbage_info[garbage_index][gsg_object])){
		DestroyDynamicObject(gs_garbage_info[garbage_index][gsg_object]);
	}
	if(IsValidDynamic3DTextLabel(gs_garbage_info[garbage_index][gsg_text])){
		DestroyDynamic3DTextLabel(gs_garbage_info[garbage_index][gsg_text]);
	}

	gs_garbage_info[garbage_index][gsg_type] = random(GS_GARBAGE_MAX_TYPE);
	gs_garbage_info[garbage_index][gsg_weight] = frandom(JOB_GS_MAX_WEIGHT);

	gs_garbage_info[garbage_index][gsg_object] = CreateDynamicObject(JOB_GS_GARBAGE_MODEL_ID, 
		job_gs_garbage_pos[garbage_index][0], 
		job_gs_garbage_pos[garbage_index][1], 
		job_gs_garbage_pos[garbage_index][2], 
		job_gs_garbage_pos[garbage_index][3], 
		job_gs_garbage_pos[garbage_index][4], 
		job_gs_garbage_pos[garbage_index][5], 
		0, 0, _, _, 50.0);

	small_string[0] = EOS;
	format(small_string, sizeof(small_string),
		""cl_blue"%s\n\
		"cl_white"%.02f кг\n\
		"cl_grey"'Left ALT'",
		gs_garbage_name_type[gs_garbage_info[garbage_index][gsg_type]][0],
		gs_garbage_info[garbage_index][gsg_weight]);

	gs_garbage_info[garbage_index][gsg_text] = CreateDynamic3DTextLabel(small_string, -1, 
		job_gs_garbage_pos[garbage_index][0], 
		job_gs_garbage_pos[garbage_index][1], 
		job_gs_garbage_pos[garbage_index][2], 
		20.0, _, _, _, 0, 0);
	small_string[0] = EOS;
	return true;
}

stock Job:GSDestroyGarbage(garbage_index){
	if(garbage_index < 0 || garbage_index >= JOB_GS_GARBAGE_COUNT) return false;

	if(IsValidDynamicObject(gs_garbage_info[garbage_index][gsg_object])){
		DestroyDynamicObject(gs_garbage_info[garbage_index][gsg_object]);
	}
	if(IsValidDynamic3DTextLabel(gs_garbage_info[garbage_index][gsg_text])){
		DestroyDynamic3DTextLabel(gs_garbage_info[garbage_index][gsg_text]);
	}

	gs_garbage_info[garbage_index][gsg_timer] = JOB_GS_TIMER;
	return true; 
}

stock Job:GSShowWarehouseCP(playerid){
	SetPlayerCheckpoint(playerid, JOB_GS_WAREH_POS, 1.0);
	return true;
}

stock Job:GSDestroyPlayerGarbage(playerid){
	new Float:pay = GetPVarFloat(playerid, "JobGarbageSorter_Weight") * float(gs_garbage_type_prices[GetPVarInt(playerid, "JobGarbageSorter_Type")]);

	small_string[0] = EOS;
	if(GetPVarFloat(playerid, "JobGarbageSorter_Weight") < 8.0){
		format(small_string, sizeof(small_string),
		"Вы отнесли %.02f кг (%s), заработав %d$.",
			GetPVarFloat(playerid, "JobGarbageSorter_Weight"),
			gs_garbage_name_type[GetPVarInt(playerid, "JobGarbageSorter_Type")][0],
			floatround(pay));
	}
	else{
		format(small_string, sizeof(small_string),
		"Вы отнесли %.02f кг (%s), заработав %d$ и "#JOB_GS_EXP" exp.",
			GetPVarFloat(playerid, "JobGarbageSorter_Weight"),
			gs_garbage_name_type[GetPVarInt(playerid, "JobGarbageSorter_Type")][0],
			floatround(pay));

		GivePlayerExp(playerid, JOB_GS_EXP);
	}

	SendClientMessage(playerid, COLOR_YELLOW, small_string);
	small_string[0] = EOS;

	GiveMoney(playerid, floatround(pay));
	StartQuest:GivePoints(playerid, SQ_FIRST_MONEY, floatround(pay));

	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отправляйтесь собирать мусор.");
	DeletePVar(playerid, "JobGarbageSorter_Weight");
	DeletePVar(playerid, "JobGarbageSorter_Type");
	return true;
}

stock Job:GSUninvite(playerid){
	SetPlayerSkin(playerid, GetPlayerData(playerid, p_skin));

	RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	DeletePVar(playerid, "JobGarbageSorter_Type");
	DeletePVar(playerid, "JobGarbageSorter_Weight");
	DeletePVar(playerid, "JobGarbageSorter_Index");
	DeletePVar(playerid, "JobGarbageSorter_Timer");

	SetPlayerData(playerid, p_job, JOB_NONE);
	DisablePlayerCheckpoint(playerid);

	SendClientMessage(playerid, COLOR_GREY, "Вы уволились с работы сортировщика мусора.");
	return true;
}