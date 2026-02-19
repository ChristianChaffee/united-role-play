
forward Chat_InputClosed(playerid, const arguments[]);
public Chat_InputClosed(playerid, const arguments[]){
	DeletePVar(playerid, "Chat_IsOpened");
}

forward Chat_MessageSent(playerid, const arguments[]);
public Chat_MessageSent(playerid, const arguments[]){
	if(!player_logged[playerid]) return false;

	if(strfind(arguments, "/") != 0){
		if(GetPVarType(playerid, "Phone_CallID") && GetPlayerCallerId(playerid) != INVALID_PLAYER_ID){
			new temp_string[144];
			format(temp_string, sizeof(temp_string),
				"[Телефон] %s[%d]: %s",
				GetPlayerData(playerid, p_name), playerid, arguments);
			
			ProxDetector(playerid, 20.0, COLOR_GREY, temp_string);
			ProxDetector(GetPlayerCallerId(playerid), 20.0, COLOR_GREY, temp_string);
		}
		else{
			new temp_string[144];
			format(temp_string, sizeof(temp_string),
				"%s[%d]: %s",
				GetPlayerData(playerid, p_name), playerid, arguments);
			ProxDetector(playerid, 20.0, COLOR_WHITE, temp_string);
		}
	}
	else{
		PC_EmulateCommand(playerid, arguments);
	}
	return true;
}

forward Chat_RequestLayout(playerid, const arguments[]);
public Chat_RequestLayout(playerid, const arguments[]){
	new layout_string[8];
	GetUserLayout(playerid, layout_string);

	cef_emit_event(playerid, "Chat_SetLayout",
		CEFSTR(layout_string));
}

forward Chat_InputOpened(playerid, const arguments[]);
public Chat_InputOpened(playerid, const arguments[]){
	new layout_string[8];
	GetUserLayout(playerid, layout_string);

	cef_emit_event(playerid, "Chat_SetLayout",
		CEFSTR(layout_string));

	SetPVarInt(playerid, "Chat_IsOpened", 1);
}