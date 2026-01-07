
public OnCefInitialize(player_id, success){
	if(!success){
		SendClientMessage(player_id, COLOR_LIGHT_RED, "[CEF]: Плагин CEF не загружен.");
	}
	else{
		SendClientMessage(player_id, COLOR_GREEN, "[CEF]: Плагин CEF загружен.");
	}
	return 1;
}