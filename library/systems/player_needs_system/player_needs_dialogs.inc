
DialogResponse:D_NEEDS_MENU(playerid, response, listitem, inputtext[]){
	if(response){
		small_string[0] = EOS;
		format(small_string, sizeof(small_string),
			"[Подсказка]: "cl_white"%s.",
			GetNeedData(listitem, n_advice));
		SendClientMessage(playerid, COLOR_LIME, small_string);
		small_string[0] = EOS;
		PlayerNeeds:ShowNeedMenu(playerid);
	}
}