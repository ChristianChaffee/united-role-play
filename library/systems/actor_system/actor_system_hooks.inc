
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	mysql_tquery(mysql, "SELECT * FROM actors", "LoadActors");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys){
	if(newkeys == KEY_WALK){
		for(new i; i < MAX_ACTORS; i++){
			if(!GetActorData(i, a_id) || !GetActorData(i, a_useble)) continue;

			switch(GetActorData(i, a_id)){
				case SQ_JONNY_ACTOR_ID:{
					StartQuest:ShowJonnyDialog(playerid);
				}
			}
			return Y_HOOKS_CONTINUE_RETURN_1;
		}
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}