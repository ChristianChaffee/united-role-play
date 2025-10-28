
stock Job:ShowGunFactoryInvite(playerid){
	if(GetPlayerData(playerid, p_job) != JOB_NONE && GetPlayerData(playerid, p_job) != JOB_GUN_FACTORY){
		SendErrorMessage(playerid, "Для начала увольтесь с другой работы.");
		return false;
	}

	if(GetPlayerData(playerid, p_job) == JOB_GUN_FACTORY){
		Dialog_Open(playerid, Dialog:D_JOB_GF_UNINVITE, DIALOG_STYLE_MSGBOX,
			"Начальник цеха", 
			""cl_blue"- "cl_white"Ты действительно хочешь уволиться с завода?",
			"Да", "Нет");
		return false;
	}

	Dialog_Open(playerid, Dialog:D_JOB_GF_INVITE, DIALOG_STYLE_MSGBOX,
		"Начальник цеха", 
		""cl_blue"- "cl_white"Мы тут занимаемся одним дельцем, если хочешь подзаработать, то добро пожаловать.\n\
		Плачу "cl_green""#JOB_GUN_FACTORY_PAY"$ "cl_white"за \"единицу\". Ты в деле?",
		"Да", "Нет");
	return true;
}

stock Job:GenerateMaterials(playerid){
	DisablePlayerCheckpoint(playerid);
	RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);

	SetPlayerCheckpoint(playerid, JOB_GUN_FACTORY_MATERIALS_POS, 1.0);
	SetPVarInt(playerid, "JobGunFactory_Stage", JOB_GF_STAGE_TO_MATERIALS);

	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отправляйтесь за материалами.");
	return true;
}

stock Job:GunFactoryUninvite(playerid){
	SetPlayerSkin(playerid, GetPlayerData(playerid, p_skin));

	RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);
	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

	DeletePVar(playerid, "JobGunFactory_Stage");
	DeletePVar(playerid, "JobGunFactory_MachineIndex");
	DeletePVar(playerid, "JobGunFactory_Timer");
	DeletePVar(playerid, "JobGunFactory_Count");

	SetPlayerData(playerid, p_job, JOB_NONE);
	DisablePlayerCheckpoint(playerid);

	SendClientMessage(playerid, COLOR_GREY, "Вы уволились с работы на заводе оружия.");
	return true;
}