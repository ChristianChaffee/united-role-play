
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

//==============================================================================
hook OnGameModeInit(){
	cef_subscribe("Chat_InputClosed", "Chat_InputClosed");
	cef_subscribe("Chat_MessageSent", "Chat_MessageSent");
	cef_subscribe("Chat_RequestLayout", "Chat_RequestLayout");
	cef_subscribe("Chat_InputOpened", "Chat_InputOpened");
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnUserChangeLayout(playerid, layout[]){
	if(GetPVarType(playerid, "Chat_IsOpened")){
		cef_emit_event(playerid, "Chat_SetLayout",
			CEFSTR(layout));
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook function SendClientMessage(playerid, color, const message[]){
	cef_emit_event(playerid, "Chat_SendMessage", CEFINT(color), CEFSTR(message));
	return continue(playerid, color, message);
}

hook function SendClientMessageToAll(color, const message[]){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		cef_emit_event(i, "Chat_SendMessage", CEFINT(color), CEFSTR(message));
	}
	return continue(color, message);
}