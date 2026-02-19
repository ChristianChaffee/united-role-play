
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

		cef_always_listen_keys(player_id, CEF_BROWSER_LOGO, true);

		SetChatStatus(player_id, false);
		cef_emit_event(player_id, "Chat_SetStatus", CEFINT(1));
	}
	return 1;
}