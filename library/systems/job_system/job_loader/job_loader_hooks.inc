
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnPlayerDisconnect(playerid, reason){
	if(GetPlayerData(playerid, p_job) == JOB_LOADER) Job:LoaderUninvite(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterCheckpoint(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_LOADER){
		switch(GetPVarInt(playerid, "JobLoader_Stage")){
			case JOB_LOADER_STAGE_TO_BOX:{
				SetPlayerAttachedObject(playerid, JOB_ATTACH_INDEX, 2969, 1, 0.1119, 0.3770, 0.0000, 0.0000, -94.0999, 0.0000, 1.0000, 1.0000, 1.0000, 0, 0);
				SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
				Job:GenerateLoaderUnload(playerid);
			}
			case JOB_LOADER_STAGE_TO_WAREHOUSE:{
				SendClientMessage(playerid, COLOR_WHITE, "Вы отнесли ящик и заработали "#JOB_LOADER_STATIC_PAY"$"cl_white".");

				SetPVarInt(playerid, "JobLoader_BoxesCount", GetPVarInt(playerid, "JobLoader_BoxesCount") + 1);
				GiveMoney(playerid, JOB_LOADER_STATIC_PAY);
				
				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					"Всего отнесено ящиков за смену %d шт и заработано %d$.",
					GetPVarInt(playerid, "JobLoader_BoxesCount"), GetPVarInt(playerid, "JobLoader_BoxesCount") * JOB_LOADER_STATIC_PAY);
				SendClientMessage(playerid, COLOR_YELLOW, small_string);
				small_string[0] = EOS;

				SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
				RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);
				Job:GenerateLoaderLoad(playerid);
			}
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook stock OnPlayerSecondUpdate(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_LOADER){
		if(!IsPlayerInRangeOfPoint(playerid, 100.0, JOB_LOADER_CENTRAL_POINT)) Job:LoaderUninvite(playerid);
	}
	return continue(playerid);
}