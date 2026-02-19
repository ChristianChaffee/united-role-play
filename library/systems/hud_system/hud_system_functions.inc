
forward HudTimerUpdate();
public HudTimerUpdate(){
	for(new playerid, j = GetPlayerPoolSize(); playerid <= j; playerid++){
		if(!IsPlayerConnected(playerid)) continue;

		Hud:UpdatePlayer(playerid);
	}
}

//==============================================================================
stock Hud:UpdatePlayer(playerid){
	GetPlayerHealth(playerid, player_hud_info[playerid][hud_health]);
	GetPlayerArmour(playerid, player_hud_info[playerid][hud_armour]);
	player_hud_info[playerid][hud_money] = GetPlayerData(playerid, p_money);
	player_hud_info[playerid][hud_weaponid] = GetPlayerWeapon(playerid);
	player_hud_info[playerid][hud_ammo] = GetPlayerAmmo(playerid);
	player_hud_info[playerid][hud_wanted] = GetPlayerData(playerid, p_wanted);

	cef_emit_event(playerid, "hud:setHealth", CEFINT(floatround(player_hud_info[playerid][hud_health])));
	cef_emit_event(playerid, "hud:setArmour", CEFINT(floatround(player_hud_info[playerid][hud_armour])));
	cef_emit_event(playerid, "hud:setMoney", CEFINT(player_hud_info[playerid][hud_money]));
	cef_emit_event(playerid, "hud:setWeapon", CEFINT(player_hud_info[playerid][hud_weaponid]), CEFINT(player_hud_info[playerid][hud_ammo]));
	cef_emit_event(playerid, "hud:setWanted", CEFINT(player_hud_info[playerid][hud_wanted]));
	return true;
}