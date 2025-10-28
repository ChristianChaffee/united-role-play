
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	for(new i; i < sizeof(job_gun_factory_machine_pos); i++){
		CreateDynamic3DTextLabel(""cl_blue"Станок\n"cl_grey"'Left ALT'", -1, 
			job_gun_factory_machine_pos[i][0],
			job_gun_factory_machine_pos[i][1],
			job_gun_factory_machine_pos[i][2], 
			15.0, _, _, 1, 
			JOB_GUN_FACTORY_VW, JOB_GUN_FACTORY_INT);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	if(GetPlayerData(playerid, p_job) == JOB_GUN_FACTORY) Job:GunFactoryUninvite(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterCheckpoint(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_GUN_FACTORY){
		if(GetPVarInt(playerid, "JobGunFactory_Stage") == JOB_GF_STAGE_TO_MATERIALS){
			DisablePlayerCheckpoint(playerid);

			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
			SetPlayerAttachedObject(playerid, JOB_ATTACH_INDEX, 19843, 1, -0.0509, 0.4069, 0.0000, 0.0000, 88.7999, 1.2999, 0.5679, 0.4759, 1.0000, 0, 0);

			SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отправляйтесь к свободному станку.");
			SetPVarInt(playerid, "JobGunFactory_Stage", JOB_GF_STAGE_TO_MACHINE);
		}
		else if(GetPVarInt(playerid, "JobGunFactory_Stage") == JOB_GF_STAGE_TO_WAREHOUSE){
			DisablePlayerCheckpoint(playerid);
			SetPVarInt(playerid, "JobGunFactory_Count", GetPVarInt(playerid, "JobGunFactory_Count") + 1);

			SendClientMessage(playerid, COLOR_YELLOW, "Вы отнесли оружие и заработали "#JOB_GUN_FACTORY_PAY"$"cl_white".");
			GiveMoney(playerid, JOB_GUN_FACTORY_PAY);
			StartQuest:GivePoints(playerid, SQ_FIRST_MONEY, JOB_GUN_FACTORY_PAY);

			small_string[0] = EOS;
			format(small_string, sizeof(small_string),
				"Всего сделано оружия за смену %d шт и заработано %d$.",
				GetPVarInt(playerid, "JobGunFactory_Count"), GetPVarInt(playerid, "JobGunFactory_Count") * JOB_GUN_FACTORY_PAY);
			SendClientMessage(playerid, COLOR_YELLOW, small_string);
			small_string[0] = EOS;

			RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);

			Job:GenerateMaterials(playerid);
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(GetPlayerData(playerid, p_job) == JOB_GUN_FACTORY && GetPVarInt(playerid, "JobGunFactory_Stage") == JOB_GF_STAGE_TO_MACHINE
		&& !GetPVarType(playerid, "JobGunFactory_MachineIndex")){
		
		for(new i, is_find; i < sizeof(job_gun_factory_machine_pos); i++){
			is_find = 0;
			if(!IsPlayerInRangeOfPoint(playerid, 1.0, job_gun_factory_machine_pos[i][0], job_gun_factory_machine_pos[i][1], job_gun_factory_machine_pos[i][2])) continue;
			
			for(new g, j = GetPlayerPoolSize(); g <= j; g++){
				if(GetPVarType(g, "JobGunFactory_MachineIndex") && GetPVarInt(g, "JobGunFactory_MachineIndex") == i){
					is_find = 1;
					break;
				}
			}

			if(is_find) continue;

			SetPVarInt(playerid, "JobGunFactory_MachineIndex", i);
			SetPVarInt(playerid, "JobGunFactory_Timer", JOB_GUN_FACTORY_TIMER);

			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
			RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);

			SetPlayerPos(playerid, job_gun_factory_machine_pos[i][0], job_gun_factory_machine_pos[i][1], job_gun_factory_machine_pos[i][2]);
			SetPlayerFacingAngle(playerid, job_gun_factory_machine_pos[i][3]);

			ApplyAnimation(playerid, "OTB", "BETSLP_LOOP", 4.1, 1, 0, 0, 0, 0);

			SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Ожидайте сборку оружия.");
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook stock OnPlayerSecondUpdate(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_GUN_FACTORY){
		if(!IsPlayerInRangeOfPoint(playerid, 100.0, JOB_GUN_FACTORY_CENTRAL_POINT)
			|| GetPlayerVirtualWorld(playerid) != JOB_GUN_FACTORY_VW
			|| GetPlayerInterior(playerid) != JOB_GUN_FACTORY_INT){ 

			Job:GunFactoryUninvite(playerid);
		}
	}

	if(GetPlayerData(playerid, p_job) == JOB_GUN_FACTORY && GetPVarType(playerid, "JobGunFactory_Timer")){
		SetPVarInt(playerid, "JobGunFactory_Timer", GetPVarInt(playerid, "JobGunFactory_Timer") - 1);
		if(GetPVarInt(playerid, "JobGunFactory_Timer") <= 0){

			ClearAnimations(playerid);

			DeletePVar(playerid, "JobGunFactory_Timer");
			DeletePVar(playerid, "JobGunFactory_MachineIndex");

			SetPlayerAttachedObject(playerid, JOB_ATTACH_INDEX, job_gun_factory_guns_model_id[random(sizeof(job_gun_factory_guns_model_id))], 1, -0.0109, 0.2750, -0.1949, -93.0999, 0.0000, -88.3000, 1.0000, 1.0000, 1.0000, 0, 0);
			SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);

			SetPlayerCheckpoint(playerid, JOB_GUN_FACTORY_WAREHOUSE_POS, 1.0);

			SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отнесите собранное оружие на склад.");
			SetPVarInt(playerid, "JobGunFactory_Stage", JOB_GF_STAGE_TO_WAREHOUSE);
		}
	}
	return continue(playerid);
}