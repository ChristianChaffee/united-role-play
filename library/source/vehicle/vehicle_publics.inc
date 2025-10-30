
forward VehicleFuelTimer();
public VehicleFuelTimer(){
	for(new i, j = GetVehiclePoolSize(); i <= j; i++){
		if(!IsValidVehicle(i)) continue;
		if(!GetVehicleEngine(i)) continue;

		switch(GetVehicleSpeed(i)){
			case 0..20: SetVehicleFuel(i, GetVehicleData(i, v_fuel) - 1.0);
			case 21..100: SetVehicleFuel(i, GetVehicleData(i, v_fuel) - 1.5);
			default: SetVehicleFuel(i, GetVehicleData(i, v_fuel) - 2.0);
		}

		if(GetVehicleData(i, v_fuel) <= 0.0){
			SetVehicleEngine(i, false);
			SetVehicleFuel(i, 0.0);
		}
	}
}