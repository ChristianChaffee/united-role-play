
forward ShowLogoTimer(playerid);
public ShowLogoTimer(playerid){
	if(cef_player_has_plugin(playerid)){
		cef_create_browser(playerid, CEF_BROWSER_LOGO, LOGO_URL, false, false);
	}
	return true;
}