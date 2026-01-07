
public OnCefInitialize(player_id, success){
	if(!success){
		SendClientMessage(player_id, COLOR_LIGHT_RED, "[CEF]: Плагин CEF не загружен.");
	}
	else{
		SendClientMessage(player_id, COLOR_GREEN, "[CEF]: Плагин CEF загружен.");

		ShowLogoTimer(player_id);
	}
	return 1;
}

public OnCefBrowserCreated(player_id, browser_id, status_code){
	if(browser_id == CEF_BROWSER_LOGO && status_code == 0){
		if(!player_logged[player_id]){
			RegistrationTimer(player_id);
		}
	}
	return 1;
}