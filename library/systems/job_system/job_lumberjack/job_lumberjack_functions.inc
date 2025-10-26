
stock Job:CreateLumberjackTree(tree_index){
	if(tree_index < 0 || tree_index >= sizeof(job_lumberjack_trees_info)) return false;

	if(IsValidDynamicObject(job_lumberjack_trees_info[tree_index][jlt_object_id])){
		DestroyDynamicObject(job_lumberjack_trees_info[tree_index][jlt_object_id]);
	}
	if(IsValidDynamic3DTextLabel(job_lumberjack_trees_info[tree_index][jlt_text])){
		DestroyDynamic3DTextLabel(job_lumberjack_trees_info[tree_index][jlt_text]);
	}

	job_lumberjack_trees_info[tree_index][jlt_object_id] = CreateDynamicObject(685, 
		job_lumberjack_trees_info[tree_index][jlt_pos][0], 
		job_lumberjack_trees_info[tree_index][jlt_pos][1], 
		job_lumberjack_trees_info[tree_index][jlt_pos][2], 
		job_lumberjack_trees_info[tree_index][jlt_pos][3], 
		job_lumberjack_trees_info[tree_index][jlt_pos][4], 
		job_lumberjack_trees_info[tree_index][jlt_pos][5]);

	job_lumberjack_trees_info[tree_index][jlt_text] = CreateDynamic3DTextLabel(""cl_blue"Дерево\n"cl_grey"'Left ALT'", -1, 
		job_lumberjack_trees_info[tree_index][jlt_pos][0], 
		job_lumberjack_trees_info[tree_index][jlt_pos][1], 
		job_lumberjack_trees_info[tree_index][jlt_pos][2] + 1.5, 
		25.0, _, _, _, 0, 0);

	job_lumberjack_trees_info[tree_index][jlt_timer] = 0;
	return true;
}

stock Job:LumberjackDestroyTree(tree_index){
	if(tree_index < 0 || tree_index >= sizeof(job_lumberjack_trees_info)) return false;

	if(IsValidDynamicObject(job_lumberjack_trees_info[tree_index][jlt_object_id])){
		DestroyDynamicObject(job_lumberjack_trees_info[tree_index][jlt_object_id]);
	}
	if(IsValidDynamic3DTextLabel(job_lumberjack_trees_info[tree_index][jlt_text])){
		DestroyDynamic3DTextLabel(job_lumberjack_trees_info[tree_index][jlt_text]);
	}

	job_lumberjack_trees_info[tree_index][jlt_timer] = JOB_LUMBERJACK_TREE_TIMER;
	return true;
}

stock Job:ShowLumberjackDialog(playerid){
	if(GetPlayerData(playerid, p_nationality) != 1 && GetPlayerData(playerid, p_level) < NATIONALITY_LEVEL){
		SendErrorMessage(playerid, "У Вас нет гражданства США для устройства на работу лесоруьа.");
		return false;
	}

	new players_count;
	for(new i, g = GetPlayerPoolSize(); i <= g; i++){
		if(GetPlayerData(i, p_job) == JOB_LUMBERJACK && player_logged[i]) players_count++;
	}

	if(players_count >= JOB_LUMBERJACK_MAX_PLAYERS){
		SendErrorMessage(playerid, "В данный момент на лесопилке нет свободных вакансий. Приходите позже.");
		return false;
	}

	if(GetPlayerData(playerid, p_job) == JOB_LUMBERJACK){
		Dialog_Open(playerid, Dialog:D_JOB_LUMBERJACK_UNINVITE, DIALOG_STYLE_MSGBOX,
			"Начальник лесопилки", 
			""cl_blue"- "cl_white"Вы действительно хотите уволиться с работы лесоруба?",
			"Да", "Нет");
		return false;
	}

	Dialog_Open(playerid, Dialog:D_JOB_LUMBERJACK_INVITE, DIALOG_STYLE_MSGBOX,
		"Начальник лесопилки", 
		""cl_blue"- "cl_white"Вы хотите устроиться на работу на нашу лесопилку? Работа заключается в том,\n\
		чтобы срубать деревья и относить их на склад. Оплата статичная и составляет "cl_green""#JOB_LUMBERJACK_STATIC_PAY"$"cl_white".",
		"Да", "Нет");
	return true;
}

stock Job:LumberjackGiveChainsaw(playerid){
	if(GetPlayerData(playerid, p_job) != JOB_LUMBERJACK) return false;

	RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);
	SetPlayerAttachedObject(playerid, JOB_ATTACH_INDEX, 341, 14, 0.2019, 0.0170, 0.0000, 104.4000, 0.0000, 0.0000, 1.0000, 1.0000, 1.0000, 0, 0);
	return true;
}

stock Job:SetLumberjackTreesCP(playerid){
	if(GetPlayerData(playerid, p_job) != JOB_LUMBERJACK) return false;

	SetPVarInt(playerid, "JobLumberjack_Stage", JOB_LUMBERJACK_STAGE_TO_TREES);

	Job:LumberjackGiveChainsaw(playerid);
	SetPlayerCheckpoint(playerid, JOB_LUMBERJACK_TREES_CP, 5.0);
	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отправляйтесь рубить деревья. Место сруба отмечено на Вашей мини-карте.");
	return true;
}

stock Job:SetLumberjackWarehouseCP(playerid){
	if(GetPlayerData(playerid, p_job) != JOB_LUMBERJACK) return false;

	SetPVarInt(playerid, "JobLumberjack_Stage", JOB_LUMBERJACK_STAGE_TO_WAREH);
	RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
	SetPlayerAttachedObject(playerid, JOB_ATTACH_INDEX, 19793, 1, 0.1119, 0.3770, 0.0000, 0.0000, -94.0999, 0.0000, 1.0000, 1.0000, 1.0000, 0, 0);

	SetPlayerCheckpoint(playerid, JOB_LUMBERJACK_WAREH_CP, 2.5);
	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отнесите срубленое дерево на склад. Склад отмечен на Вашей мини-карте.");
	return true;
}

stock Job:LumberjackUninvite(playerid){
	SetPlayerSkin(playerid, GetPlayerData(playerid, p_skin));

	RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	DeletePVar(playerid, "JobLumberjack_Stage");
	DeletePVar(playerid, "JobLumberjack_TreeTimer");
	DeletePVar(playerid, "JobLumberjack_TreeIndex");
	DeletePVar(playerid, "JobLumberjack_WoodCount");

	SetPlayerData(playerid, p_job, JOB_NONE);
	DisablePlayerCheckpoint(playerid);

	SendClientMessage(playerid, COLOR_GREY, "Вы уволились с работы лесоруба.");
	return true;
}