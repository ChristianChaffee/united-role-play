
stock EnterExits_EnterPlayer(playerid, enter_exit_index){
	if(enter_exit_index < 0 || enter_exit_index >= TOTAL_ENTER_EXITS) return false;

	SetPlayerPos(playerid,
		GetEnterExitData(enter_exit_index, ee_exit_pos_x),
		GetEnterExitData(enter_exit_index, ee_exit_pos_y),
		GetEnterExitData(enter_exit_index, ee_exit_pos_z));

	SetPlayerFacingAngle(playerid,
		GetEnterExitData(enter_exit_index, ee_exit_pos_angle));

	SetPlayerVirtualWorld(playerid,
		GetEnterExitData(enter_exit_index, ee_exit_vw));

	SetPlayerInterior(playerid, 
		GetEnterExitData(enter_exit_index, ee_exit_int));

	SetCameraBehindPlayer(playerid);
	return true;
}

stock EnterExits:ExitPlayer(playerid, enter_exit_index){
	if(enter_exit_index < 0 || enter_exit_index >= TOTAL_ENTER_EXITS) return false;

	SetPlayerPos(playerid,
		GetEnterExitData(enter_exit_index, ee_enter_pos_x),
		GetEnterExitData(enter_exit_index, ee_enter_pos_y),
		GetEnterExitData(enter_exit_index, ee_enter_pos_z));

	SetPlayerFacingAngle(playerid,
		GetEnterExitData(enter_exit_index, ee_enter_pos_angle));

	SetPlayerVirtualWorld(playerid,
		GetEnterExitData(enter_exit_index, ee_enter_vw));

	SetPlayerInterior(playerid, 
		GetEnterExitData(enter_exit_index, ee_enter_int));

	SetCameraBehindPlayer(playerid);
	return true;
}