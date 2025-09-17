
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook OnGameModeInit(){
	samp_rp_TD[0] = TextDrawCreate(610.0000, 44.0000, "_"); // пусто
	TextDrawLetterSize(samp_rp_TD[0], 0.3000, 1.0000);
	TextDrawTextSize(samp_rp_TD[0], 1280.0000, 1280.0000);
	TextDrawAlignment(samp_rp_TD[0], 0);
	TextDrawColor(samp_rp_TD[0], -572662273);
	TextDrawSetOutline(samp_rp_TD[0], 1);
	TextDrawBackgroundColor(samp_rp_TD[0], 255);
	TextDrawFont(samp_rp_TD[0], 3);
	TextDrawSetProportional(samp_rp_TD[0], 0);
	TextDrawSetShadow(samp_rp_TD[0], 2);

	samp_rp_TD[1] = TextDrawCreate(320.0000, 408.4375, "SELECT"); // пусто
	TextDrawLetterSize(samp_rp_TD[1], 0.2205, 1.0575);
	TextDrawTextSize(samp_rp_TD[1], 75.0000, 336.5000);
	TextDrawAlignment(samp_rp_TD[1], 2);
	TextDrawColor(samp_rp_TD[1], -1);
	TextDrawBackgroundColor(samp_rp_TD[1], 255);
	TextDrawFont(samp_rp_TD[1], 2);
	TextDrawSetProportional(samp_rp_TD[1], 1);
	TextDrawSetShadow(samp_rp_TD[1], 0);
	TextDrawSetSelectable(samp_rp_TD[1], true);

	samp_rp_TD[2] = TextDrawCreate(252.0000, 406.5625, "LD_SPAC:WHITE"); // пусто
	TextDrawLetterSize(samp_rp_TD[2], 0.6000, 2.0000);
	TextDrawTextSize(samp_rp_TD[2], 28.0000, 15.0000);
	TextDrawAlignment(samp_rp_TD[2], 1);
	TextDrawColor(samp_rp_TD[2], 656880639);
	TextDrawUseBox(samp_rp_TD[2], 1);
	TextDrawBoxColor(samp_rp_TD[2], 838860800);
	TextDrawSetOutline(samp_rp_TD[2], 1);
	TextDrawBackgroundColor(samp_rp_TD[2], 255);
	TextDrawFont(samp_rp_TD[2], 4);
	TextDrawSetProportional(samp_rp_TD[2], 1);
	TextDrawSetShadow(samp_rp_TD[2], 0);

	samp_rp_TD[3] = TextDrawCreate(239.0000, 403.0625, "LD_BEAT:CHIT"); // пусто
	TextDrawLetterSize(samp_rp_TD[3], 0.6000, 2.0000);
	TextDrawTextSize(samp_rp_TD[3], 24.0000, 22.1000);
	TextDrawAlignment(samp_rp_TD[3], 1);
	TextDrawColor(samp_rp_TD[3], 656880639);
	TextDrawUseBox(samp_rp_TD[3], 1);
	TextDrawBoxColor(samp_rp_TD[3], 838860800);
	TextDrawSetOutline(samp_rp_TD[3], 1);
	TextDrawBackgroundColor(samp_rp_TD[3], 255);
	TextDrawFont(samp_rp_TD[3], 4);
	TextDrawSetProportional(samp_rp_TD[3], 1);
	TextDrawSetShadow(samp_rp_TD[3], 0);

	samp_rp_TD[4] = TextDrawCreate(360.0000, 406.5625, "LD_SPAC:WHITE"); // пусто
	TextDrawLetterSize(samp_rp_TD[4], 0.6000, 2.0000);
	TextDrawTextSize(samp_rp_TD[4], 28.0000, 15.0000);
	TextDrawAlignment(samp_rp_TD[4], 1);
	TextDrawColor(samp_rp_TD[4], 656880639);
	TextDrawUseBox(samp_rp_TD[4], 1);
	TextDrawBoxColor(samp_rp_TD[4], 838860800);
	TextDrawSetOutline(samp_rp_TD[4], 1);
	TextDrawBackgroundColor(samp_rp_TD[4], 255);
	TextDrawFont(samp_rp_TD[4], 4);
	TextDrawSetProportional(samp_rp_TD[4], 1);
	TextDrawSetShadow(samp_rp_TD[4], 0);

	samp_rp_TD[5] = TextDrawCreate(377.0000, 403.0625, "LD_BEAT:CHIT"); // пусто
	TextDrawLetterSize(samp_rp_TD[5], 0.6000, 2.0000);
	TextDrawTextSize(samp_rp_TD[5], 24.0000, 22.1000);
	TextDrawAlignment(samp_rp_TD[5], 1);
	TextDrawColor(samp_rp_TD[5], 656880639);
	TextDrawUseBox(samp_rp_TD[5], 1);
	TextDrawBoxColor(samp_rp_TD[5], 838860800);
	TextDrawSetOutline(samp_rp_TD[5], 1);
	TextDrawBackgroundColor(samp_rp_TD[5], 255);
	TextDrawFont(samp_rp_TD[5], 4);
	TextDrawSetProportional(samp_rp_TD[5], 1);
	TextDrawSetShadow(samp_rp_TD[5], 0);

	samp_rp_TD[6] = TextDrawCreate(279.5000, 406.5625, "LD_SPAC:WHITE"); // пусто
	TextDrawLetterSize(samp_rp_TD[6], 0.6000, 2.0000);
	TextDrawTextSize(samp_rp_TD[6], 81.0000, 15.0000);
	TextDrawAlignment(samp_rp_TD[6], 1);
	TextDrawColor(samp_rp_TD[6], 488450047);
	TextDrawUseBox(samp_rp_TD[6], 1);
	TextDrawBoxColor(samp_rp_TD[6], 838860800);
	TextDrawSetOutline(samp_rp_TD[6], 1);
	TextDrawBackgroundColor(samp_rp_TD[6], 255);
	TextDrawFont(samp_rp_TD[6], 4);
	TextDrawSetProportional(samp_rp_TD[6], 1);
	TextDrawSetShadow(samp_rp_TD[6], 0);

	samp_rp_TD[7] = TextDrawCreate(254.0000, 407.4375, "<<<"); // пусто
	TextDrawLetterSize(samp_rp_TD[7], 0.2675, 1.3768);
	TextDrawTextSize(samp_rp_TD[7], 270.0000, 11.0000);
	TextDrawAlignment(samp_rp_TD[7], 1);
	TextDrawColor(samp_rp_TD[7], -1);
	TextDrawBackgroundColor(samp_rp_TD[7], 255);
	TextDrawFont(samp_rp_TD[7], 1);
	TextDrawSetProportional(samp_rp_TD[7], 1);
	TextDrawSetShadow(samp_rp_TD[7], 0);
	TextDrawSetSelectable(samp_rp_TD[7], true);

	samp_rp_TD[8] = TextDrawCreate(370.5000, 407.4375, ">>>"); // пусто
	TextDrawLetterSize(samp_rp_TD[8], 0.2675, 1.3768);
	TextDrawTextSize(samp_rp_TD[8], 385.0000, 11.0000);
	TextDrawAlignment(samp_rp_TD[8], 1);
	TextDrawColor(samp_rp_TD[8], -1);
	TextDrawBackgroundColor(samp_rp_TD[8], 255);
	TextDrawFont(samp_rp_TD[8], 1);
	TextDrawSetProportional(samp_rp_TD[8], 1);
	TextDrawSetShadow(samp_rp_TD[8], 0);
	TextDrawSetSelectable(samp_rp_TD[8], true);
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid){
	if(clickedid == samp_rp_TD[8]){ // Next
		if(GetPVarType(playerid, "Registration_SkinIndex")){
			SetPVarInt(playerid, "Registration_SkinIndex", GetPVarInt(playerid, "Registration_SkinIndex") + 1);
			if(GetPVarInt(playerid, "Registration_SkinIndex") >= REGISTRATION_SKINS_COUNT){
				SetPVarInt(playerid, "Registration_SkinIndex", 0);
			}
			SetPlayerSkin(playerid, registration_skins[GetPlayerData(playerid, p_sex)][GetPVarInt(playerid, "Registration_SkinIndex")]);
		}
	}
	else if(clickedid == samp_rp_TD[7]){ // Previous
		if(GetPVarType(playerid, "Registration_SkinIndex")){
			SetPVarInt(playerid, "Registration_SkinIndex", GetPVarInt(playerid, "Registration_SkinIndex") - 1);
			if(GetPVarInt(playerid, "Registration_SkinIndex") < 0){
				SetPVarInt(playerid, "Registration_SkinIndex", REGISTRATION_SKINS_COUNT - 1);
			}
			SetPlayerSkin(playerid, registration_skins[GetPlayerData(playerid, p_sex)][GetPVarInt(playerid, "Registration_SkinIndex")]);
		}
	}
	else if(clickedid == samp_rp_TD[1]){ // Select
		if(GetPVarType(playerid, "Registration_SkinIndex")){
			SetPlayerData(playerid, p_skin, registration_skins[GetPlayerData(playerid, p_sex)][GetPVarInt(playerid, "Registration_SkinIndex")]);
			DeletePVar(playerid, "Registration_SkinIndex");
			Registration:SetCamera(playerid);
			Registration:ShowRegCharacter(playerid);
			SkinSelect:HideInterface(playerid);
		}
	}
	else if(_:clickedid == INVALID_TEXT_DRAW){
		if(GetPVarType(playerid, "Registration_SkinIndex")){
			SetPlayerData(playerid, p_skin, registration_skins[GetPlayerData(playerid, p_sex)][GetPVarInt(playerid, "Registration_SkinIndex")]);
			DeletePVar(playerid, "Registration_SkinIndex");
			Registration:SetCamera(playerid);
			Registration:ShowRegCharacter(playerid);
			SkinSelect:HideInterface(playerid);
		}
	}
}