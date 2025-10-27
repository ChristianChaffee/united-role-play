
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnPlayerDisconnect(playerid, reason){
	if(GetPlayerData(playerid, p_job) == JOB_GARBAGE_SORTER) Job:GSUninvite(playerid);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnGameModeInit(){
	for(new i; i < JOB_GS_GARBAGE_COUNT; i++){
		Job:GSCreateGarbage(i);
	}

	CreateDynamic3DTextLabel(""cl_blue"Сброс мусора\n"cl_grey"'Left ALT'", -1, 
		JOB_GS_WAREH_POS, 25.0, _, _, 1, 0, 0);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK && GetPlayerData(playerid, p_job) == JOB_GARBAGE_SORTER && !GetPVarType(playerid, "JobGarbageSorter_Index")){
		if(IsPlayerInRangeOfPoint(playerid, 1.5, JOB_GS_WAREH_POS)){
			if(GetPVarFloat(playerid, "JobGarbageSorter_Weight") <= 0){
				SendErrorMessage(playerid, "У Вас нет мусора в ведре.");
				return Y_HOOKS_CONTINUE_RETURN_1;
			}

			Job:GSDestroyPlayerGarbage(playerid);
			return Y_HOOKS_CONTINUE_RETURN_1;
		}

		for(new i, is_find; i < JOB_GS_GARBAGE_COUNT; i++){
			is_find = 0;
			if(gs_garbage_info[i][gsg_timer] > 0) continue;
			if(!IsPlayerInRangeOfPoint(playerid, 1.5, job_gs_garbage_pos[i][0], job_gs_garbage_pos[i][1], job_gs_garbage_pos[i][2])) continue;
			if(GetPVarType(playerid, "JobGarbageSorter_Type") && gs_garbage_info[i][gsg_type] != GetPVarInt(playerid, "JobGarbageSorter_Type")){
				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					"Вы не можете собрать данный мусор, так как у Вас в ведре \"%s\".",
					gs_garbage_name_type[GetPVarInt(playerid, "JobGarbageSorter_Type")][0]);
				SendErrorMessage(playerid, small_string);
				small_string[0] = EOS;
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
			if(GetPVarType(playerid, "JobGarbageSorter_Weight") && GetPVarFloat(playerid, "JobGarbageSorter_Weight") >= JOB_GS_MAX_GARBAGE){
				SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отправляйтесь в сортировочную чтобы освободить ведро.");
				Job:GSShowWarehouseCP(playerid);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
			for(new g, j = GetPlayerPoolSize(); g <= j; g++){
				if(GetPVarType(g, "JobGarbageSorter_Index") == i){
					is_find = 1;
					break;
				}
			}
			if(is_find) continue;

			SetPVarInt(playerid, "JobGarbageSorter_Index", i);
			SetPVarInt(playerid, "JobGarbageSorter_Timer", JOB_GS_PLAYER_TIMER);
			ApplyAnimation(playerid, "BOMBER", "BOM_PLANT", 4.0, 1, 1, 1, 0, 0);
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook stock OnPlayerSecondUpdate(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_GARBAGE_SORTER && GetPVarType(playerid, "JobGarbageSorter_Timer")){
		SetPVarInt(playerid, "JobGarbageSorter_Timer", GetPVarInt(playerid, "JobGarbageSorter_Timer") - 1);
		if(GetPVarInt(playerid, "JobGarbageSorter_Timer") <= 0){
			ClearAnimations(playerid);

			SetPVarInt(playerid, "JobGarbageSorter_Type", gs_garbage_info[GetPVarInt(playerid, "JobGarbageSorter_Index")][gsg_type]);

			new Float:give_weight;
			if(GetPVarFloat(playerid, "JobGarbageSorter_Weight") + gs_garbage_info[GetPVarInt(playerid, "JobGarbageSorter_Index")][gsg_weight] <= JOB_GS_MAX_GARBAGE){
				give_weight = gs_garbage_info[GetPVarInt(playerid, "JobGarbageSorter_Index")][gsg_weight];
			}
			else give_weight = JOB_GS_MAX_GARBAGE - GetPVarFloat(playerid, "JobGarbageSorter_Weight");

			SetPVarFloat(playerid, "JobGarbageSorter_Weight", GetPVarFloat(playerid, "JobGarbageSorter_Weight") + give_weight);
			gs_garbage_info[GetPVarInt(playerid, "JobGarbageSorter_Index")][gsg_weight] -= give_weight;

			if(gs_garbage_info[GetPVarInt(playerid, "JobGarbageSorter_Index")][gsg_weight] <= 0){
				Job:GSDestroyGarbage(GetPVarInt(playerid, "JobGarbageSorter_Index"));
			}
			else{
				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					""cl_blue"%s\n\
					"cl_white"%.02f кг\n\
					"cl_grey"'Left ALT'",
					gs_garbage_name_type[gs_garbage_info[GetPVarInt(playerid, "JobGarbageSorter_Index")][gsg_type]][0],
					gs_garbage_info[GetPVarInt(playerid, "JobGarbageSorter_Index")][gsg_weight]);
				UpdateDynamic3DTextLabelText(gs_garbage_info[GetPVarInt(playerid, "JobGarbageSorter_Index")][gsg_text], -1, small_string);
				small_string[0] = EOS;
			}

			if(GetPVarFloat(playerid, "JobGarbageSorter_Weight") >= JOB_GS_MAX_GARBAGE){
				SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: "cl_white"Отнесите собранный мусор на переработку.");
				Job:GSShowWarehouseCP(playerid);
			}
			else{
				small_string[0] = EOS;
				format(small_string, sizeof(small_string),
					"[Подсказка]: "cl_white"Отправляйтесь к следующему мусору или сдайте мусор на переработку. У Вас %.02f кг (%s).",
					GetPVarFloat(playerid, "JobGarbageSorter_Weight"), gs_garbage_name_type[GetPVarInt(playerid, "JobGarbageSorter_Type")][0]);
				SendClientMessage(playerid, COLOR_GREY, small_string);
				small_string[0] = EOS;
			}

			DeletePVar(playerid, "JobGarbageSorter_Timer");
			DeletePVar(playerid, "JobGarbageSorter_Index");
		}
	}

	if(GetPlayerData(playerid, p_job) == JOB_GARBAGE_SORTER && !IsPlayerInRangeOfPoint(playerid, 100.0, JOB_GS_CENTRAL_POINT)){
		Job:GSUninvite(playerid);
	}
	return continue(playerid);
}

hook stock SecondTimerStock(){
	for(new i; i < JOB_GS_GARBAGE_COUNT; i++){
		if(gs_garbage_info[i][gsg_timer] > 0){
			gs_garbage_info[i][gsg_timer]--;
			if(gs_garbage_info[i][gsg_timer] <= 0){
				Job:GSCreateGarbage(i);
			}
		}
	}
	return continue();
}

hook OnPlayerEnterCheckpoint(playerid){
	if(GetPlayerData(playerid, p_job) == JOB_GARBAGE_SORTER){
		Job:GSDestroyPlayerGarbage(playerid);
		DisablePlayerCheckpoint(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}