
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM start_quests", "LoadStartQuests");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerEnterRCP(playerid){
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerDisconnect(playerid, reason){
	if(GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_status) == SQ_STATUS_IN_PROGRESS
		&& GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_stage) > SQ_STAGE_DRIVING_TO_LC){

		StartQuest:SetStage(playerid, SQ_DRIVING_LICENSE, SQ_STAGE_DRIVING_TO_LC);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook stock OnPlayerSecondUpdate(playerid){
	if(GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_status) == SQ_STATUS_IN_PROGRESS
		&& GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_stage) == SQ_STAGE_DRIVING_TO_RUBY
		&& !IsPlayerInRangeOfPoint(playerid, 50.0, SQ_RUBY_POS)){

		StartQuest:SetStage(playerid, SQ_DRIVING_LICENSE, SQ_STAGE_DRIVING_TO_LC);
	}
	return continue(playerid);
}

hook stock OnPlayerAccountLoaded(playerid){
	StartQuest:LoadPlayerStartQuests(playerid);
	return continue (playerid);
}

hook stock EnterExits_EnterPlayer(playerid, enter_exit_index){
	new result = continue(playerid, enter_exit_index);

	if(!result) return result;

	if(GetEnterExitData(enter_exit_index, ee_id) == SQ_LICENSING_CENTER_ENTER_ID
		&& GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_status) == SQ_STATUS_IN_PROGRESS
		&& GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_stage) == SQ_STAGE_DRIVING_TO_LC){

		StartQuest:SetStage(playerid, SQ_DRIVING_LICENSE, SQ_STAGE_DRIVING_TO_RUBY);
	}

	return result;
}

hook stock LicensingCenter_ShowMenu(playerid){
	if(GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_status) == SQ_STATUS_IN_PROGRESS
		&& GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_stage) == SQ_STAGE_DRIVING_TO_RUBY){

		Dialog_Open(playerid, Dialog:D_START_QUESTS_RUBY_1, DIALOG_STYLE_MSGBOX,
			"Руби", 
			""cl_blue"- "cl_white"Привет, ты видимо от Джонни, он мне о тебе говорил.",
			"Да", "");
		return true;

	}
	else return continue(playerid);
}

hook stock LicensingCenter_StopExam(playerid){
	new result = continue(playerid);

	if(GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_status) == SQ_STATUS_IN_PROGRESS
		&& GetPlayerStartQuestData(playerid, SQ_DRIVING_LICENSE, sqp_stage) == SQ_STAGE_DRIVING_DRIVE){

		if(player_info[playerid][p_licenses][0] && player_info[playerid][p_licenses_times][0] > 0){
			StartQuest:FinishQuest(playerid, SQ_DRIVING_LICENSE);
		}
		else{
			StartQuest:SetStage(playerid, SQ_DRIVING_LICENSE, SQ_STAGE_DRIVING_TO_RUBY);
		}
	}

	return result;
}