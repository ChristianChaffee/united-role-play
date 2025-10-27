
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	for(new i; i < sizeof(job_lumberjack_trees_info); i++){
		Job:CreateLumberjackTree(i);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	if(GetPlayerData(playerid, p_job) == JOB_LUMBERJACK) Job:LumberjackUninvite(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK && GetPlayerData(playerid, p_job) == JOB_LUMBERJACK && GetPVarInt(playerid, "JobLumberjack_Stage") == JOB_LUMBERJACK_STAGE_TO_TREES){
		for(new i, is_find; i < sizeof(job_lumberjack_trees_info); i++){
			if(job_lumberjack_trees_info[i][jlt_timer] > 0) continue;
			if(!IsPlayerInRangeOfPoint(playerid, 2.0, job_lumberjack_trees_info[i][jlt_pos][0], job_lumberjack_trees_info[i][jlt_pos][1], job_lumberjack_trees_info[i][jlt_pos][2])) continue;

			is_find = 0;
			for(new g, j = GetPlayerPoolSize(); g <= j; g++){
				if(GetPVarInt(g, "JobLumberjack_TreeIndex") == i){
					is_find = 1;
					break;
				}
			}
			if(is_find) continue;

			ApplyAnimation(playerid, "SWORD", "sword_4", 4.0, 1, 0, 0, 0, 0);
			SetPVarInt(playerid, "JobLumberjack_TreeTimer", JOB_LUMBERJACK_TIMER);
			SetPVarInt(playerid, "JobLumberjack_TreeIndex", i);
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterCheckpoint(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_LUMBERJACK && GetPVarInt(playerid, "JobLumberjack_Stage") == JOB_LUMBERJACK_STAGE_TO_TREES){
		DisablePlayerCheckpoint(playerid);
	}
	if(GetPlayerData(playerid, p_job) == JOB_LUMBERJACK && GetPVarInt(playerid, "JobLumberjack_Stage") == JOB_LUMBERJACK_STAGE_TO_WAREH){
		SendClientMessage(playerid, COLOR_WHITE, "Вы отнесли дерево и заработали "#JOB_LUMBERJACK_STATIC_PAY"$"cl_white".");

		SetPVarInt(playerid, "JobLumberjack_WoodCount", GetPVarInt(playerid, "JobLumberjack_WoodCount") + 1);
		GiveMoney(playerid, JOB_LUMBERJACK_STATIC_PAY);
		
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"Всего отнесено дерева за смену %d шт и заработано %d$.",
			GetPVarInt(playerid, "JobLumberjack_WoodCount"), GetPVarInt(playerid, "JobLumberjack_WoodCount") * JOB_LUMBERJACK_STATIC_PAY);
		SendClientMessage(playerid, COLOR_YELLOW, small_string);
		small_string[0] = EOS;

		SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
		RemovePlayerAttachedObject(playerid, JOB_ATTACH_INDEX);
		Job:SetLumberjackTreesCP(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook stock OnPlayerSecondUpdate(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_LUMBERJACK && GetPVarInt(playerid, "JobLumberjack_TreeTimer") > 0){
		SetPVarInt(playerid, "JobLumberjack_TreeTimer", GetPVarInt(playerid, "JobLumberjack_TreeTimer") - 1);
		
		if(GetPVarInt(playerid, "JobLumberjack_TreeTimer") <= 0){
			DeletePVar(playerid, "JobLumberjack_TreeTimer");
			ClearAnimations(playerid);
			Job:LumberjackDestroyTree(GetPVarInt(playerid, "JobLumberjack_TreeIndex"));
			DeletePVar(playerid, "JobLumberjack_TreeIndex");
			Job:SetLumberjackWarehouseCP(playerid);
		}
	}

	if(GetPlayerData(playerid, p_job) == JOB_LUMBERJACK){
		if(!IsPlayerInRangeOfPoint(playerid, 150.0, JOB_LUMBERJACK_CENTRAL_POINT)) Job:LumberjackUninvite(playerid);
	}
	return continue(playerid);
}

hook stock SecondTimerStock(){
	for(new i; i < sizeof(job_lumberjack_trees_info); i++){
		if(job_lumberjack_trees_info[i][jlt_timer] > 0){
			job_lumberjack_trees_info[i][jlt_timer]--;
			if(job_lumberjack_trees_info[i][jlt_timer] <= 0){
				Job:CreateLumberjackTree(i);
			}
		}
	}
	return continue();
}