
CMD:phone(playerid){
	if(!player_logged[playerid]) return false;
	if(!GetPlayerData(playerid, p_phone)) return SendErrorMessage(playerid, "У Вас нет телефона.");
	
	Phone:ShowPhone(playerid);
	return true;
}

CMD:p(playerid){
	if(!player_logged[playerid]) return false;
	if(GetPVarType(playerid, "Phone_CallID")) return SendErrorMessage(playerid, "Вы уже разговариваете по телефону.");
	new scdplayerid = GetPlayerCallerId(playerid);
	if(scdplayerid == INVALID_PLAYER_ID) return SendErrorMessage(playerid, "Вам никто не звонит.");

	SetPVarInt(playerid, "Phone_CallID", scdplayerid);

	Phone:ShowPhone(playerid);
	cef_emit_event(playerid, "startCallInterface",
		CEFSTR(GetPlayerData(scdplayerid, p_name)));

	SetPlayerSpecialAction(playerid, SPECIAL_ACTION_USECELLPHONE);
	SendMeAction(playerid, "поднял(-а) трубку.");

	SendClientMessage(playerid, COLOR_GREY, "[Подсказка]: Чтобы сбросить трубку введите '/h' или откройте телефон с помощью '/phone'.");
	return true;
}

CMD:h(playerid){
	if(!player_logged[playerid]) return false;
	if(!GetPVarType(playerid, "Phone_CallID") && GetPlayerCallerId(playerid) == INVALID_PLAYER_ID) return SendErrorMessage(playerid, "У Вас нет активного вызова.");

	new scdplayerid = GetPlayerCallerId(playerid);
	if(scdplayerid != INVALID_PLAYER_ID){
		SendClientMessage(scdplayerid, COLOR_GREY, "Абонент сбросил(-а) трубку.");
		Phone:StopCall(scdplayerid);
	}

	Phone:StopCall(playerid);
	SendMeAction(playerid, "положил(-а) телефон.");
	return true;
}