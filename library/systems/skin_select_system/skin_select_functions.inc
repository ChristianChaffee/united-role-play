
stock SkinSelect:ShowInterface(playerid){
	for(new i; i < sizeof(samp_rp_TD); i++){
		TextDrawShowForPlayer(playerid, samp_rp_TD[i]);
	}
	SelectTextDraw(playerid, COLOR_BLUE);
	return true;
}

stock SkinSelect:HideInterface(playerid){
	for(new i; i < sizeof(samp_rp_TD); i++){
		TextDrawHideForPlayer(playerid, samp_rp_TD[i]);
	}
	CancelSelectTextDraw(playerid);
	return true;
}