
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM actors", "LoadActors");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK){
		for(new i; i < MAX_ACTORS; i++){
			if(!GetActorData(i, a_id) || !GetActorData(i, a_useble)) continue;
			if(!IsPlayerInRangeOfPoint(playerid, 2.0, GetActorData(i, a_pos_x), GetActorData(i, a_pos_y), GetActorData(i, a_pos_z))) continue;

			switch(GetActorData(i, a_id)){
				case SQ_JONNY_ACTOR_ID:{
					StartQuest:ShowJonnyDialog(playerid);
				}
				case JOB_LOADER_ACTOR_ID:{
					Job:ShowLoaderDialog(playerid);
				}
			}
			return Y_HOOKS_CONTINUE_RETURN_1;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}