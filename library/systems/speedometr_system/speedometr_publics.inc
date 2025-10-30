
forward SpeedometrTimer();
public SpeedometrTimer(){
	for(new i, j = GetPlayerPoolSize(); i <= j; i++){
		if(!player_logged[i]) continue;
		if(GetPlayerState(i) != PLAYER_STATE_DRIVER) continue;
		if(!GetPVarType(i, "Speedometr_IsShowed")) continue;

		Speedometr:UpdateSpeed(i);
	}
}