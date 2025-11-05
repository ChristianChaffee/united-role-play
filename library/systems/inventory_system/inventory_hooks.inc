
#include <YSI-Includes-5.x\YSI_Coding\y_hooks>

hook stock OnPlayerSecondUpdate(playerid){
	if(GetPVarType(playerid, "Inventory_EatDrinkAnimationTimer") && GetPVarInt(playerid, "Inventory_EatDrinkAnimationTimer") > 0){
		SetPVarInt(playerid, "Inventory_EatDrinkAnimationTimer", GetPVarInt(playerid, "Inventory_EatDrinkAnimationTimer") - 1);
		if(GetPVarInt(playerid, "Inventory_EatDrinkAnimationTimer")){
			
			ClearAnimations(playerid);
			DeletePVar(playerid, "Inventory_EatDrinkAnimationTimer");
		}
	}
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid){
	if(_:clickedid == INVALID_TEXT_DRAW && GetPVarType(playerid, "Inventory_Page")){
		Inventory:Hide(playerid);
	}
	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerClickPTD(playerid, PlayerText:playertextid){
	if(InvParadox_PTD[playerid][0] <= playertextid <= InvParadox_PTD[playerid][20]){
		#warning DEBUG
		for(new i; i < 21; i++){
			if(playertextid == InvParadox_PTD[playerid][i]){
				printf("%d", i);
				break;
			}
		}
	}
	if(SlotParadox_PTD[playerid][0] <= playertextid <= SlotParadox_PTD[playerid][24]){

		PlayerPlaySound(playerid, 1083, 0.0, 0.0, 0.0);

		if(!GetPVarType(playerid, "Inventory_SelectedSlot")){
			PlayerTextDrawHide(playerid, playertextid);
			PlayerTextDrawColor(playerid, playertextid, COLOR_GREY);
			PlayerTextDrawShow(playerid, playertextid);

			for(new i; i < 25; i++){
				if(playertextid == SlotParadox_PTD[playerid][i]){
					SetPVarInt(playerid, "Inventory_SelectedSlot", i);
					break;
				}
			}
			return Y_HOOKS_CONTINUE_RETURN_1;
		}
		else{
			new 
				clicked_slot, 
				selected_slot = GetPVarInt(playerid, "Inventory_SelectedSlot");
			
			for(new i; i < 25; i++){
				if(playertextid == SlotParadox_PTD[playerid][i]){
					clicked_slot = i;
					break;
				}
			}

			new
				clicked_index = Inventory:FindItemWithSlot(playerid, (GetPVarInt(playerid, "Inventory_Page") - 1) * 25 + clicked_slot),
				selected_index = Inventory:FindItemWithSlot(playerid, (GetPVarInt(playerid, "Inventory_Page") - 1) * 25 + selected_slot);

			if(clicked_slot == selected_slot && clicked_index == -1){
				DeletePVar(playerid, "Inventory_SelectedSlot");

				PlayerTextDrawHide(playerid, playertextid);
				PlayerTextDrawColor(playerid, playertextid, -1);
				PlayerTextDrawShow(playerid, playertextid);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
			else if(clicked_slot == selected_slot && clicked_index != -1){
				Inventory:Hide(playerid);
				Inventory:ShowActionDialog(playerid, clicked_index);

				DeletePVar(playerid, "Inventory_SelectedSlot");
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
			else if(clicked_slot != selected_slot && selected_index == -1){
				SetPVarInt(playerid, "Inventory_SelectedSlot", clicked_slot);

				PlayerTextDrawHide(playerid, playertextid);
				PlayerTextDrawColor(playerid, playertextid, COLOR_GREY);
				PlayerTextDrawShow(playerid, playertextid);

				PlayerTextDrawHide(playerid, SlotParadox_PTD[playerid][selected_slot]);
				PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][selected_slot], -1);
				PlayerTextDrawShow(playerid, SlotParadox_PTD[playerid][selected_slot]);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
			else if(clicked_slot != selected_slot
				&& GetPlayerInventoryItemData(playerid, selected_index, inv_player_item_id) && clicked_index == -1){

				SetPlayerInventoryItemData(playerid, selected_index, inv_player_item_slot, (GetPVarInt(playerid, "Inventory_Page") - 1) * 25 + clicked_slot);

				DeletePVar(playerid, "Inventory_SelectedSlot");
				PlayerTextDrawHide(playerid, SlotParadox_PTD[playerid][selected_slot]);
				PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][selected_slot], -1);
				PlayerTextDrawShow(playerid, SlotParadox_PTD[playerid][selected_slot]);

				Inventory:UpdatePreviewModels(playerid);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
			else if(clicked_slot != selected_slot
				&& GetPlayerInventoryItemData(playerid, selected_index, inv_player_item_id)
				&& GetPlayerInventoryItemData(playerid, clicked_index, inv_player_item_id)){

				Inventory:ChangePlayerItemDTS(playerid, selected_index, clicked_index);

				DeletePVar(playerid, "Inventory_SelectedSlot");
				PlayerTextDrawHide(playerid, SlotParadox_PTD[playerid][selected_slot]);
				PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][selected_slot], -1);
				PlayerTextDrawShow(playerid, SlotParadox_PTD[playerid][selected_slot]);

				Inventory:UpdatePreviewModels(playerid);
				return Y_HOOKS_CONTINUE_RETURN_1;
			}
		}
	}
	else if(playertextid == InvParadox_PTD[playerid][18]){ // - Страница 1
		if(GetPVarInt(playerid, "Inventory_Page") == 1) return Y_HOOKS_CONTINUE_RETURN_1;
		
		PlayerTextDrawHide(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")]);
		PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")], -1);
		PlayerTextDrawShow(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")]);
		DeletePVar(playerid, "Inventory_SelectedSlot");

		SetPVarInt(playerid, "Inventory_Page", 1);
		Inventory:UpdatePages(playerid);
	}
	else if(playertextid == InvParadox_PTD[playerid][19]){ // - Страница 2
		if(GetPVarInt(playerid, "Inventory_Page") == 2) return Y_HOOKS_CONTINUE_RETURN_1;

		PlayerTextDrawHide(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")]);
		PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")], -1);
		PlayerTextDrawShow(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")]);
		DeletePVar(playerid, "Inventory_SelectedSlot");

		SetPVarInt(playerid, "Inventory_Page", 2);
		Inventory:UpdatePages(playerid);
	}
	else if(playertextid == InvParadox_PTD[playerid][20]){ // - Страница 3
		if(GetPVarInt(playerid, "Inventory_Page") == 3) return Y_HOOKS_CONTINUE_RETURN_1;

		PlayerTextDrawHide(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")]);
		PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")], -1);
		PlayerTextDrawShow(playerid, SlotParadox_PTD[playerid][GetPVarInt(playerid, "Inventory_SelectedSlot")]);
		DeletePVar(playerid, "Inventory_SelectedSlot");

		SetPVarInt(playerid, "Inventory_Page", 3);
		Inventory:UpdatePages(playerid);
	}

	return Y_HOOKS_CONTINUE_RETURN_1;
}

//==============================================================================
hook OnGameModeInit(){
	InvParadox_TD[0] = TextDrawCreate(289.5249, 108.4498, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[0], 0.0000, 24.6215);
	TextDrawTextSize(InvParadox_TD[0], 479.6984, 0.0000);
	TextDrawAlignment(InvParadox_TD[0], 1);
	TextDrawColor(InvParadox_TD[0], -1);
	TextDrawUseBox(InvParadox_TD[0], 1);
	TextDrawBoxColor(InvParadox_TD[0], 673529599);
	TextDrawBackgroundColor(InvParadox_TD[0], 255);
	TextDrawFont(InvParadox_TD[0], 1);
	TextDrawSetProportional(InvParadox_TD[0], 1);
	TextDrawSetShadow(InvParadox_TD[0], 0);

	InvParadox_TD[1] = TextDrawCreate(178.4853, 108.0166, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[1], 0.0000, 24.6646);
	TextDrawTextSize(InvParadox_TD[1], 279.6984, 0.0000);
	TextDrawAlignment(InvParadox_TD[1], 1);
	TextDrawColor(InvParadox_TD[1], -1);
	TextDrawUseBox(InvParadox_TD[1], 1);
	TextDrawBoxColor(InvParadox_TD[1], 673529599);
	TextDrawBackgroundColor(InvParadox_TD[1], 255);
	TextDrawFont(InvParadox_TD[1], 1);
	TextDrawSetProportional(InvParadox_TD[1], 1);
	TextDrawSetShadow(InvParadox_TD[1], 0);

	InvParadox_TD[2] = TextDrawCreate(178.4165, 108.4166, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[2], 0.0000, 0.8169);
	TextDrawTextSize(InvParadox_TD[2], 279.4088, 0.0000);
	TextDrawAlignment(InvParadox_TD[2], 1);
	TextDrawColor(InvParadox_TD[2], -1);
	TextDrawUseBox(InvParadox_TD[2], 1);
	TextDrawBoxColor(InvParadox_TD[2], -864228353);
	TextDrawBackgroundColor(InvParadox_TD[2], 255);
	TextDrawFont(InvParadox_TD[2], 1);
	TextDrawSetProportional(InvParadox_TD[2], 1);
	TextDrawSetShadow(InvParadox_TD[2], 0);

	InvParadox_TD[3] = TextDrawCreate(289.5274, 108.4166, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[3], 0.0000, 0.8385);
	TextDrawTextSize(InvParadox_TD[3], 479.4888, 0.0000);
	TextDrawAlignment(InvParadox_TD[3], 1);
	TextDrawColor(InvParadox_TD[3], -1);
	TextDrawUseBox(InvParadox_TD[3], 1);
	TextDrawBoxColor(InvParadox_TD[3], -864228353);
	TextDrawBackgroundColor(InvParadox_TD[3], 255);
	TextDrawFont(InvParadox_TD[3], 1);
	TextDrawSetProportional(InvParadox_TD[3], 1);
	TextDrawSetShadow(InvParadox_TD[3], 0);

	InvParadox_TD[4] = TextDrawCreate(211.7500, 107.8498, "ACCESSORIES"); // пусто
	TextDrawLetterSize(InvParadox_TD[4], 0.1755, 0.8999);
	TextDrawAlignment(InvParadox_TD[4], 1);
	TextDrawColor(InvParadox_TD[4], -1);
	TextDrawBackgroundColor(InvParadox_TD[4], 255);
	TextDrawFont(InvParadox_TD[4], 1);
	TextDrawSetProportional(InvParadox_TD[4], 1);
	TextDrawSetShadow(InvParadox_TD[4], 0);

	InvParadox_TD[5] = TextDrawCreate(372.3598, 107.8498, "INVENTORY"); // пусто
	TextDrawLetterSize(InvParadox_TD[5], 0.1755, 0.8999);
	TextDrawAlignment(InvParadox_TD[5], 1);
	TextDrawColor(InvParadox_TD[5], -1);
	TextDrawBackgroundColor(InvParadox_TD[5], 255);
	TextDrawFont(InvParadox_TD[5], 1);
	TextDrawSetProportional(InvParadox_TD[5], 1);
	TextDrawSetShadow(InvParadox_TD[5], 0);

	InvParadox_TD[6] = TextDrawCreate(210.5144, 222.2998, "LD_BEAT:CHIT"); // пусто
	TextDrawTextSize(InvParadox_TD[6], 40.0000, 13.0000);
	TextDrawAlignment(InvParadox_TD[6], 1);
	TextDrawColor(InvParadox_TD[6], 34);
	TextDrawBackgroundColor(InvParadox_TD[6], 255);
	TextDrawFont(InvParadox_TD[6], 4);
	TextDrawSetProportional(InvParadox_TD[6], 0);
	TextDrawSetShadow(InvParadox_TD[6], 0);

	InvParadox_TD[7] = TextDrawCreate(182.6826, 239.5832, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[7], 0.0000, 4.6588);
	TextDrawTextSize(InvParadox_TD[7], 275.0000, 0.0000);
	TextDrawAlignment(InvParadox_TD[7], 1);
	TextDrawColor(InvParadox_TD[7], -1);
	TextDrawUseBox(InvParadox_TD[7], 1);
	TextDrawBoxColor(InvParadox_TD[7], 589248136);
	TextDrawBackgroundColor(InvParadox_TD[7], 255);
	TextDrawFont(InvParadox_TD[7], 1);
	TextDrawSetProportional(InvParadox_TD[7], 1);
	TextDrawSetShadow(InvParadox_TD[7], 0);

	InvParadox_TD[8] = TextDrawCreate(182.6770, 239.3665, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[8], 0.0000, -0.2606);
	TextDrawTextSize(InvParadox_TD[8], 275.0000, 0.0000);
	TextDrawAlignment(InvParadox_TD[8], 1);
	TextDrawColor(InvParadox_TD[8], -1);
	TextDrawUseBox(InvParadox_TD[8], 1);
	TextDrawBoxColor(InvParadox_TD[8], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[8], 255);
	TextDrawFont(InvParadox_TD[8], 1);
	TextDrawSetProportional(InvParadox_TD[8], 1);
	TextDrawSetShadow(InvParadox_TD[8], 0);

	InvParadox_TD[9] = TextDrawCreate(182.6770, 283.6693, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[9], 0.0000, -0.2606);
	TextDrawTextSize(InvParadox_TD[9], 274.9898, 0.0000);
	TextDrawAlignment(InvParadox_TD[9], 1);
	TextDrawColor(InvParadox_TD[9], -1);
	TextDrawUseBox(InvParadox_TD[9], 1);
	TextDrawBoxColor(InvParadox_TD[9], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[9], 255);
	TextDrawFont(InvParadox_TD[9], 1);
	TextDrawSetProportional(InvParadox_TD[9], 1);
	TextDrawSetShadow(InvParadox_TD[9], 0);

	InvParadox_TD[10] = TextDrawCreate(182.7086, 312.4512, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[10], 0.0000, 0.7232);
	TextDrawTextSize(InvParadox_TD[10], 209.0000, 0.0000);
	TextDrawAlignment(InvParadox_TD[10], 1);
	TextDrawColor(InvParadox_TD[10], -1);
	TextDrawUseBox(InvParadox_TD[10], 1);
	TextDrawBoxColor(InvParadox_TD[10], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[10], 255);
	TextDrawFont(InvParadox_TD[10], 1);
	TextDrawSetProportional(InvParadox_TD[10], 1);
	TextDrawSetShadow(InvParadox_TD[10], 0);

	InvParadox_TD[11] = TextDrawCreate(215.1105, 312.4512, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[11], 0.0000, 0.7232);
	TextDrawTextSize(InvParadox_TD[11], 241.4019, 0.0000);
	TextDrawAlignment(InvParadox_TD[11], 1);
	TextDrawColor(InvParadox_TD[11], -1);
	TextDrawUseBox(InvParadox_TD[11], 1);
	TextDrawBoxColor(InvParadox_TD[11], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[11], 255);
	TextDrawFont(InvParadox_TD[11], 1);
	TextDrawSetProportional(InvParadox_TD[11], 1);
	TextDrawSetShadow(InvParadox_TD[11], 0);

	InvParadox_TD[12] = TextDrawCreate(247.8125, 312.4512, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[12], 0.0000, 0.7232);
	TextDrawTextSize(InvParadox_TD[12], 274.1039, 0.0000);
	TextDrawAlignment(InvParadox_TD[12], 1);
	TextDrawColor(InvParadox_TD[12], -1);
	TextDrawUseBox(InvParadox_TD[12], 1);
	TextDrawBoxColor(InvParadox_TD[12], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[12], 255);
	TextDrawFont(InvParadox_TD[12], 1);
	TextDrawSetProportional(InvParadox_TD[12], 1);
	TextDrawSetShadow(InvParadox_TD[12], 0);

	InvParadox_TD[13] = TextDrawCreate(182.7086, 296.1502, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[13], 0.0000, 0.7232);
	TextDrawTextSize(InvParadox_TD[13], 209.0000, 0.0000);
	TextDrawAlignment(InvParadox_TD[13], 1);
	TextDrawColor(InvParadox_TD[13], -1);
	TextDrawUseBox(InvParadox_TD[13], 1);
	TextDrawBoxColor(InvParadox_TD[13], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[13], 255);
	TextDrawFont(InvParadox_TD[13], 1);
	TextDrawSetProportional(InvParadox_TD[13], 1);
	TextDrawSetShadow(InvParadox_TD[13], 0);

	InvParadox_TD[14] = TextDrawCreate(215.1105, 296.1502, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[14], 0.0000, 0.7232);
	TextDrawTextSize(InvParadox_TD[14], 241.4019, 0.0000);
	TextDrawAlignment(InvParadox_TD[14], 1);
	TextDrawColor(InvParadox_TD[14], -1);
	TextDrawUseBox(InvParadox_TD[14], 1);
	TextDrawBoxColor(InvParadox_TD[14], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[14], 255);
	TextDrawFont(InvParadox_TD[14], 1);
	TextDrawSetProportional(InvParadox_TD[14], 1);
	TextDrawSetShadow(InvParadox_TD[14], 0);

	InvParadox_TD[15] = TextDrawCreate(247.8125, 296.1502, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[15], 0.0000, 0.7232);
	TextDrawTextSize(InvParadox_TD[15], 274.1039, 0.0000);
	TextDrawAlignment(InvParadox_TD[15], 1);
	TextDrawColor(InvParadox_TD[15], -1);
	TextDrawUseBox(InvParadox_TD[15], 1);
	TextDrawBoxColor(InvParadox_TD[15], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[15], 255);
	TextDrawFont(InvParadox_TD[15], 1);
	TextDrawSetProportional(InvParadox_TD[15], 1);
	TextDrawSetShadow(InvParadox_TD[15], 0);

	InvParadox_TD[16] = TextDrawCreate(294.5742, 319.9671, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[16], 0.0000, 0.7221);
	TextDrawTextSize(InvParadox_TD[16], 378.0000, 0.0000);
	TextDrawAlignment(InvParadox_TD[16], 1);
	TextDrawColor(InvParadox_TD[16], -1);
	TextDrawUseBox(InvParadox_TD[16], 1);
	TextDrawBoxColor(InvParadox_TD[16], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[16], 255);
	TextDrawFont(InvParadox_TD[16], 1);
	TextDrawSetProportional(InvParadox_TD[16], 1);
	TextDrawSetShadow(InvParadox_TD[16], 0);

	InvParadox_TD[17] = TextDrawCreate(390.9801, 319.9671, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[17], 0.0000, 0.7221);
	TextDrawTextSize(InvParadox_TD[17], 474.4058, 0.0000);
	TextDrawAlignment(InvParadox_TD[17], 1);
	TextDrawColor(InvParadox_TD[17], -1);
	TextDrawUseBox(InvParadox_TD[17], 1);
	TextDrawBoxColor(InvParadox_TD[17], 1111712255);
	TextDrawBackgroundColor(InvParadox_TD[17], 255);
	TextDrawFont(InvParadox_TD[17], 1);
	TextDrawSetProportional(InvParadox_TD[17], 1);
	TextDrawSetShadow(InvParadox_TD[17], 0);

	InvParadox_TD[18] = TextDrawCreate(188.4048, 249.4998, "EXP:"); // пусто
	TextDrawLetterSize(InvParadox_TD[18], 0.2158, 0.9582);
	TextDrawAlignment(InvParadox_TD[18], 1);
	TextDrawColor(InvParadox_TD[18], -1045732353);
	TextDrawBackgroundColor(InvParadox_TD[18], 255);
	TextDrawFont(InvParadox_TD[18], 1);
	TextDrawSetProportional(InvParadox_TD[18], 1);
	TextDrawSetShadow(InvParadox_TD[18], 0);

	InvParadox_TD[19] = TextDrawCreate(212.7062, 249.4998, "DONATE:"); // пусто
	TextDrawLetterSize(InvParadox_TD[19], 0.2158, 0.9582);
	TextDrawAlignment(InvParadox_TD[19], 1);
	TextDrawColor(InvParadox_TD[19], -1045732353);
	TextDrawBackgroundColor(InvParadox_TD[19], 255);
	TextDrawFont(InvParadox_TD[19], 1);
	TextDrawSetProportional(InvParadox_TD[19], 1);
	TextDrawSetShadow(InvParadox_TD[19], 0);

	InvParadox_TD[20] = TextDrawCreate(249.8085, 249.4998, "BANK:"); // пусто
	TextDrawLetterSize(InvParadox_TD[20], 0.2158, 0.9582);
	TextDrawAlignment(InvParadox_TD[20], 1);
	TextDrawColor(InvParadox_TD[20], -1045732353);
	TextDrawBackgroundColor(InvParadox_TD[20], 255);
	TextDrawFont(InvParadox_TD[20], 1);
	TextDrawSetProportional(InvParadox_TD[20], 1);
	TextDrawSetShadow(InvParadox_TD[20], 0);

	InvParadox_TD[21] = TextDrawCreate(174.7754, 327.6833, "LD_BEAT:CHIT"); // пусто
	TextDrawTextSize(InvParadox_TD[21], 10.0000, 11.0000);
	TextDrawAlignment(InvParadox_TD[21], 1);
	TextDrawColor(InvParadox_TD[21], 673529599);
	TextDrawBackgroundColor(InvParadox_TD[21], 255);
	TextDrawFont(InvParadox_TD[21], 4);
	TextDrawSetProportional(InvParadox_TD[21], 0);
	TextDrawSetShadow(InvParadox_TD[21], 0);

	InvParadox_TD[22] = TextDrawCreate(272.8813, 327.6833, "LD_BEAT:CHIT"); // пусто
	TextDrawTextSize(InvParadox_TD[22], 10.0000, 11.0000);
	TextDrawAlignment(InvParadox_TD[22], 1);
	TextDrawColor(InvParadox_TD[22], 673529599);
	TextDrawBackgroundColor(InvParadox_TD[22], 255);
	TextDrawFont(InvParadox_TD[22], 4);
	TextDrawSetProportional(InvParadox_TD[22], 0);
	TextDrawSetShadow(InvParadox_TD[22], 0);

	InvParadox_TD[23] = TextDrawCreate(285.7821, 327.6833, "LD_BEAT:CHIT"); // пусто
	TextDrawTextSize(InvParadox_TD[23], 10.0000, 11.0000);
	TextDrawAlignment(InvParadox_TD[23], 1);
	TextDrawColor(InvParadox_TD[23], 673529599);
	TextDrawBackgroundColor(InvParadox_TD[23], 255);
	TextDrawFont(InvParadox_TD[23], 4);
	TextDrawSetProportional(InvParadox_TD[23], 0);
	TextDrawSetShadow(InvParadox_TD[23], 0);

	InvParadox_TD[24] = TextDrawCreate(472.9935, 327.6833, "LD_BEAT:CHIT"); // пусто
	TextDrawTextSize(InvParadox_TD[24], 10.0000, 11.0000);
	TextDrawAlignment(InvParadox_TD[24], 1);
	TextDrawColor(InvParadox_TD[24], 673529599);
	TextDrawBackgroundColor(InvParadox_TD[24], 255);
	TextDrawFont(InvParadox_TD[24], 4);
	TextDrawSetProportional(InvParadox_TD[24], 0);
	TextDrawSetShadow(InvParadox_TD[24], 0);

	InvParadox_TD[25] = TextDrawCreate(182.9456, 332.0332, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[25], 0.0000, 0.2547);
	TextDrawTextSize(InvParadox_TD[25], 275.0000, 0.0000);
	TextDrawAlignment(InvParadox_TD[25], 1);
	TextDrawColor(InvParadox_TD[25], -1);
	TextDrawUseBox(InvParadox_TD[25], 1);
	TextDrawBoxColor(InvParadox_TD[25], 673529599);
	TextDrawBackgroundColor(InvParadox_TD[25], 255);
	TextDrawFont(InvParadox_TD[25], 1);
	TextDrawSetProportional(InvParadox_TD[25], 1);
	TextDrawSetShadow(InvParadox_TD[25], 0);

	InvParadox_TD[26] = TextDrawCreate(293.5167, 332.0332, "Box"); // пусто
	TextDrawLetterSize(InvParadox_TD[26], 0.0000, 0.3016);
	TextDrawTextSize(InvParadox_TD[26], 476.0000, 0.0000);
	TextDrawAlignment(InvParadox_TD[26], 1);
	TextDrawColor(InvParadox_TD[26], -1);
	TextDrawUseBox(InvParadox_TD[26], 1);
	TextDrawBoxColor(InvParadox_TD[26], 673529599);
	TextDrawBackgroundColor(InvParadox_TD[26], 255);
	TextDrawFont(InvParadox_TD[26], 1);
	TextDrawSetProportional(InvParadox_TD[26], 1);
	TextDrawSetShadow(InvParadox_TD[26], 0);

	return Y_HOOKS_CONTINUE_RETURN_1;
}

hook OnPlayerConnect(playerid){
	InvParadox_PTD[playerid][0] = CreatePlayerTextDraw(playerid, 180.4346, 123.5000, ""); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][0], 95.0000, 114.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][0], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][0], 0);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][0], 5);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][0], 0);
	PlayerTextDrawSetPreviewModel(playerid, InvParadox_PTD[playerid][0], 223);
	PlayerTextDrawSetPreviewRot(playerid, InvParadox_PTD[playerid][0], 0.0000, 0.0000, 15.0000, 1.0000);

	InvParadox_PTD[playerid][1] = CreatePlayerTextDraw(playerid, 194.9902, 264.5000, "[10/22]"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][1], 0.1577, 0.9408);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][1], 2);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][1], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][1], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][1], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][1], 0);

	InvParadox_PTD[playerid][2] = CreatePlayerTextDraw(playerid, 226.0921, 264.5000, "100000"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][2], 0.1577, 0.9408);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][2], 2);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][2], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][2], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][2], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][2], 0);

	InvParadox_PTD[playerid][3] = CreatePlayerTextDraw(playerid, 259.1940, 264.5000, "$100000200"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][3], 0.1577, 0.9408);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][3], 2);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][3], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][3], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][3], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][3], 0);

	InvParadox_PTD[playerid][4] = CreatePlayerTextDraw(playerid, 189.7364, 295.1166, "HELP"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][4], 0.1755, 0.8999);
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][4], 201.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][4], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][4], -1);
	PlayerTextDrawUseBox(playerid, InvParadox_PTD[playerid][4], 1);
	PlayerTextDrawBoxColor(playerid, InvParadox_PTD[playerid][4], 0);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][4], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][4], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][4], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][4], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][4], true);

	InvParadox_PTD[playerid][5] = CreatePlayerTextDraw(playerid, 220.7382, 295.1166, "MENU"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][5], 0.1755, 0.8999);
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][5], 235.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][5], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][5], -1);
	PlayerTextDrawUseBox(playerid, InvParadox_PTD[playerid][5], 1);
	PlayerTextDrawBoxColor(playerid, InvParadox_PTD[playerid][5], 0);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][5], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][5], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][5], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][5], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][5], true);

	InvParadox_PTD[playerid][6] = CreatePlayerTextDraw(playerid, 251.1401, 295.1166, "REPORT"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][6], 0.1755, 0.8999);
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][6], 271.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][6], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][6], -1);
	PlayerTextDrawUseBox(playerid, InvParadox_PTD[playerid][6], 1);
	PlayerTextDrawBoxColor(playerid, InvParadox_PTD[playerid][6], 0);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][6], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][6], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][6], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][6], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][6], true);

	InvParadox_PTD[playerid][7] = CreatePlayerTextDraw(playerid, 186.0157, 311.5501, "QUESTS"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][7], 0.1755, 0.8999);
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][7], 206.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][7], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][7], -1);
	PlayerTextDrawUseBox(playerid, InvParadox_PTD[playerid][7], 1);
	PlayerTextDrawBoxColor(playerid, InvParadox_PTD[playerid][7], 0);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][7], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][7], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][7], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][7], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][7], true);

	InvParadox_PTD[playerid][8] = CreatePlayerTextDraw(playerid, 217.5175, 311.5501, "MYCARS"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][8], 0.1755, 0.8999);
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][8], 239.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][8], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][8], -1);
	PlayerTextDrawUseBox(playerid, InvParadox_PTD[playerid][8], 1);
	PlayerTextDrawBoxColor(playerid, InvParadox_PTD[playerid][8], 0);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][8], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][8], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][8], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][8], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][8], true);

	InvParadox_PTD[playerid][9] = CreatePlayerTextDraw(playerid, 251.6197, 311.5501, "FAMILY"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][9], 0.1755, 0.8999);
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][9], 270.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][9], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][9], -1);
	PlayerTextDrawUseBox(playerid, InvParadox_PTD[playerid][9], 1);
	PlayerTextDrawBoxColor(playerid, InvParadox_PTD[playerid][9], 0);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][9], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][9], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][9], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][9], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][9], true);

	InvParadox_PTD[playerid][10] = CreatePlayerTextDraw(playerid, 315.8316, 318.7838, "OPEN_TRUNK"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][10], 0.2336, 0.9524);
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][10], 360.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][10], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][10], -1);
	PlayerTextDrawUseBox(playerid, InvParadox_PTD[playerid][10], 1);
	PlayerTextDrawBoxColor(playerid, InvParadox_PTD[playerid][10], 0);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][10], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][10], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][10], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][10], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][10], true);

	InvParadox_PTD[playerid][11] = CreatePlayerTextDraw(playerid, 408.6372, 318.7838, "OPEN_CABINET"); // пусто
	PlayerTextDrawLetterSize(playerid, InvParadox_PTD[playerid][11], 0.2336, 0.9524);
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][11], 457.0000, 8.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][11], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][11], -1);
	PlayerTextDrawUseBox(playerid, InvParadox_PTD[playerid][11], 1);
	PlayerTextDrawBoxColor(playerid, InvParadox_PTD[playerid][11], 0);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][11], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][11], 1);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][11], 1);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][11], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][11], true);

	InvParadox_PTD[playerid][12] = CreatePlayerTextDraw(playerid, 183.0086, 133.4501, ""); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][12], 26.0000, 29.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][12], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][12], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][12], 1111712255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][12], 5);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][12], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][12], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][12], true);
	PlayerTextDrawSetPreviewModel(playerid, InvParadox_PTD[playerid][12], 1649);
	PlayerTextDrawSetPreviewRot(playerid, InvParadox_PTD[playerid][12], 0.0000, 0.0000, 90.0000, 1.0000);

	InvParadox_PTD[playerid][13] = CreatePlayerTextDraw(playerid, 183.0086, 166.4342, ""); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][13], 26.0000, 29.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][13], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][13], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][13], 1111712255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][13], 5);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][13], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][13], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][13], true);
	PlayerTextDrawSetPreviewModel(playerid, InvParadox_PTD[playerid][13], 1649);
	PlayerTextDrawSetPreviewRot(playerid, InvParadox_PTD[playerid][13], 0.0000, 0.0000, 90.0000, 1.0000);

	InvParadox_PTD[playerid][14] = CreatePlayerTextDraw(playerid, 183.0086, 199.7675, ""); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][14], 26.0000, 29.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][14], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][14], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][14], 1111712255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][14], 5);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][14], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][14], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][14], true);
	PlayerTextDrawSetPreviewModel(playerid, InvParadox_PTD[playerid][14], 1649);
	PlayerTextDrawSetPreviewRot(playerid, InvParadox_PTD[playerid][14], 0.0000, 0.0000, 90.0000, 1.0000);

	InvParadox_PTD[playerid][15] = CreatePlayerTextDraw(playerid, 248.7113, 133.4833, ""); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][15], 26.0000, 29.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][15], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][15], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][15], 1111712255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][15], 5);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][15], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][15], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][15], true);
	PlayerTextDrawSetPreviewModel(playerid, InvParadox_PTD[playerid][15], 1649);
	PlayerTextDrawSetPreviewRot(playerid, InvParadox_PTD[playerid][15], 0.0000, 0.0000, 90.0000, 1.0000);

	InvParadox_PTD[playerid][16] = CreatePlayerTextDraw(playerid, 248.7113, 166.4674, ""); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][16], 26.0000, 29.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][16], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][16], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][16], 1111712255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][16], 5);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][16], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][16], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][16], true);
	PlayerTextDrawSetPreviewModel(playerid, InvParadox_PTD[playerid][16], 1649);
	PlayerTextDrawSetPreviewRot(playerid, InvParadox_PTD[playerid][16], 0.0000, 0.0000, 90.0000, 1.0000);

	InvParadox_PTD[playerid][17] = CreatePlayerTextDraw(playerid, 248.7113, 199.8007, ""); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][17], 26.0000, 29.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][17], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][17], -1);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][17], 1111712255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][17], 5);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][17], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][17], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][17], true);
	PlayerTextDrawSetPreviewModel(playerid, InvParadox_PTD[playerid][17], 1649);
	PlayerTextDrawSetPreviewRot(playerid, InvParadox_PTD[playerid][17], 0.0000, 0.0000, 90.0000, 1.0000);

	InvParadox_PTD[playerid][18] = CreatePlayerTextDraw(playerid, 329.1872, 331.2170, "LD_SPAC:white"); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][18], 35.0000, 3.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][18], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][18], -1503414785);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][18], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][18], 4);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][18], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][18], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][18], true);

	InvParadox_PTD[playerid][19] = CreatePlayerTextDraw(playerid, 368.2376, 330.9002, "LD_SPAC:white"); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][19], 35.0000, 3.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][19], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][19], 2134219775);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][19], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][19], 4);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][19], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][19], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][19], true);

	InvParadox_PTD[playerid][20] = CreatePlayerTextDraw(playerid, 406.6879, 331.0002, "LD_SPAC:white"); // пусто
	PlayerTextDrawTextSize(playerid, InvParadox_PTD[playerid][20], 35.0000, 3.0000);
	PlayerTextDrawAlignment(playerid, InvParadox_PTD[playerid][20], 1);
	PlayerTextDrawColor(playerid, InvParadox_PTD[playerid][20], 2134219775);
	PlayerTextDrawBackgroundColor(playerid, InvParadox_PTD[playerid][20], 255);
	PlayerTextDrawFont(playerid, InvParadox_PTD[playerid][20], 4);
	PlayerTextDrawSetProportional(playerid, InvParadox_PTD[playerid][20], 0);
	PlayerTextDrawSetShadow(playerid, InvParadox_PTD[playerid][20], 0);
	PlayerTextDrawSetSelectable(playerid, InvParadox_PTD[playerid][20], true);

	SlotParadox_PTD[playerid][0] = CreatePlayerTextDraw(playerid, 292.4155, 121.2673, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][0], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][0], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][0], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][0], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][0], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][0], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][0], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][0], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][0], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][0], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][1] = CreatePlayerTextDraw(playerid, 330.1344, 121.2673, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][1], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][1], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][1], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][1], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][1], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][1], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][1], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][1], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][1], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][1], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][2] = CreatePlayerTextDraw(playerid, 368.0844, 121.1673, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][2], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][2], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][2], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][2], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][2], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][2], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][2], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][2], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][2], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][2], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][3] = CreatePlayerTextDraw(playerid, 405.8348, 121.2840, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][3], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][3], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][3], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][3], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][3], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][3], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][3], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][3], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][3], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][3], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][4] = CreatePlayerTextDraw(playerid, 443.1851, 121.2840, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][4], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][4], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][4], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][4], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][4], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][4], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][4], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][4], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][4], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][4], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][5] = CreatePlayerTextDraw(playerid, 292.4155, 160.8006, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][5], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][5], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][5], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][5], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][5], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][5], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][5], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][5], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][5], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][5], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][6] = CreatePlayerTextDraw(playerid, 330.1344, 160.8006, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][6], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][6], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][6], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][6], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][6], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][6], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][6], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][6], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][6], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][6], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][7] = CreatePlayerTextDraw(playerid, 368.0844, 160.7006, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][7], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][7], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][7], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][7], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][7], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][7], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][7], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][7], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][7], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][7], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][8] = CreatePlayerTextDraw(playerid, 405.8348, 160.8172, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][8], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][8], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][8], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][8], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][8], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][8], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][8], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][8], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][8], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][8], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][9] = CreatePlayerTextDraw(playerid, 443.1851, 160.8172, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][9], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][9], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][9], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][9], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][9], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][9], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][9], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][9], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][9], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][9], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][10] = CreatePlayerTextDraw(playerid, 292.4155, 200.2338, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][10], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][10], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][10], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][10], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][10], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][10], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][10], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][10], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][10], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][10], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][11] = CreatePlayerTextDraw(playerid, 330.1344, 200.2338, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][11], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][11], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][11], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][11], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][11], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][11], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][11], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][11], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][11], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][11], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][12] = CreatePlayerTextDraw(playerid, 368.0844, 200.1339, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][12], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][12], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][12], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][12], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][12], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][12], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][12], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][12], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][12], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][12], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][13] = CreatePlayerTextDraw(playerid, 405.8348, 200.2505, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][13], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][13], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][13], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][13], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][13], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][13], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][13], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][13], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][13], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][13], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][14] = CreatePlayerTextDraw(playerid, 443.1851, 200.2505, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][14], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][14], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][14], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][14], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][14], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][14], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][14], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][14], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][14], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][14], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][15] = CreatePlayerTextDraw(playerid, 292.4525, 239.6506, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][15], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][15], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][15], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][15], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][15], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][15], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][15], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][15], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][15], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][15], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][16] = CreatePlayerTextDraw(playerid, 330.1714, 239.6506, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][16], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][16], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][16], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][16], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][16], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][16], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][16], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][16], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][16], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][16], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][17] = CreatePlayerTextDraw(playerid, 368.1214, 239.5506, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][17], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][17], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][17], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][17], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][17], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][17], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][17], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][17], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][17], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][17], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][18] = CreatePlayerTextDraw(playerid, 405.8718, 239.6672, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][18], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][18], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][18], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][18], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][18], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][18], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][18], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][18], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][18], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][18], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][19] = CreatePlayerTextDraw(playerid, 443.2221, 239.6672, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][19], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][19], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][19], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][19], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][19], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][19], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][19], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][19], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][19], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][19], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][20] = CreatePlayerTextDraw(playerid, 292.5470, 279.0672, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][20], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][20], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][20], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][20], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][20], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][20], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][20], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][20], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][20], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][20], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][21] = CreatePlayerTextDraw(playerid, 330.2659, 279.0672, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][21], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][21], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][21], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][21], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][21], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][21], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][21], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][21], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][21], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][21], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][22] = CreatePlayerTextDraw(playerid, 368.2159, 278.9673, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][22], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][22], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][22], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][22], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][22], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][22], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][22], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][22], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][22], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][22], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][23] = CreatePlayerTextDraw(playerid, 405.9663, 279.0839, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][23], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][23], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][23], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][23], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][23], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][23], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][23], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][23], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][23], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][23], 0.0000, 0.0000, 90.0000, 1.0000);

	SlotParadox_PTD[playerid][24] = CreatePlayerTextDraw(playerid, 443.3166, 279.0839, ""); // пусто
	PlayerTextDrawTextSize(playerid, SlotParadox_PTD[playerid][24], 33.0000, 37.0000);
	PlayerTextDrawAlignment(playerid, SlotParadox_PTD[playerid][24], 1);
	PlayerTextDrawColor(playerid, SlotParadox_PTD[playerid][24], -1);
	PlayerTextDrawBackgroundColor(playerid, SlotParadox_PTD[playerid][24], 1111712255);
	PlayerTextDrawFont(playerid, SlotParadox_PTD[playerid][24], 5);
	PlayerTextDrawSetProportional(playerid, SlotParadox_PTD[playerid][24], 0);
	PlayerTextDrawSetShadow(playerid, SlotParadox_PTD[playerid][24], 0);
	PlayerTextDrawSetSelectable(playerid, SlotParadox_PTD[playerid][24], true);
	PlayerTextDrawSetPreviewModel(playerid, SlotParadox_PTD[playerid][24], 1649);
	PlayerTextDrawSetPreviewRot(playerid, SlotParadox_PTD[playerid][24], 0.0000, 0.0000, 90.0000, 1.0000);
	return Y_HOOKS_CONTINUE_RETURN_1;
}