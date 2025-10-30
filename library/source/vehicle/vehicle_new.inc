
#define VEHICLE_MODELS_COUNT 						411
#define VEHICLE_FUEL_TIME 							60000

#define GetVehicleData(%0,%1)  						vehicle_info[%0][%1]
#define SetVehicleData(%0,%1,%2)  					vehicle_info[%0][%1] = %2

//==============================================================================
enum
{
	FUEL_TYPE_NONE,
	FUEL_TYPE_92,
	FUEL_TYPE_95,
	FUEL_TYPE_98,
	FUEL_TYPE_DISEL
}

enum
{
	VEHICLE_TYPE_CAR,
	VEHICLE_TYPE_TRUCK,
	VEHICLE_TYPE_BUS,
	VEHICLE_TYPE_MOTORBIKE,
	VEHICLE_TYPE_BOAT,
	VEHICLE_TYPE_HELICOPTER,
	VEHICLE_TYPE_PLANE,
	VEHICLE_TYPE_BIKE,
	VEHICLE_TYPE_TRAILER,
	VEHICLE_TYPE_TOY,
	VEHICLE_TYPE_TRAIN,
}

//==============================================================================
enum vehicle_params_info_struct
{
	vp_model_id,
	vp_name[64],
	vp_fuel_type,
	vp_type,
	Float:vp_acceleration
	vp_max_speed,
}
/*new const vehicle_params_info[VEHICLE_MODELS_COUNT][vehicle_params_info_struct] = {
	{400, "Landstalker", FUEL_TYPE_92, VEHICLE_TYPE_CAR, 2.7, 157},
	{401, "Bravura", FUEL_TYPE_92, VEHICLE_TYPE_CAR, 4.9, 147},
	{402, "Buffalo", FUEL_TYPE_95, VEHICLE_TYPE_CAR, 2.7, 186},
	{403, "Linerunner", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 5.7, 109},
	{404, "Perenniel", FUEL_TYPE_92, VEHICLE_TYPE_CAR, 5.7, 133},
	{405, "Sentinel", FUEL_TYPE_95, VEHICLE_TYPE_CAR, 3.6, 163},
	{406, "Dumper", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 4.9, 110},
	{407, "Firetruck", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 3.9, 148},
	{408, "Trashmaster", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 8.3, 100},
	{409, "Stretch", FUEL_TYPE_DISEL, VEHICLE_TYPE_CAR, 4.0, 158},
	{410, "Manana", FUEL_TYPE_92, VEHICLE_TYPE_CAR, 5.8, 129},
	{411, "Infernus", FUEL_TYPE_98, VEHICLE_TYPE_CAR, 1.9, 221},
	{412, "Voodoo", FUEL_TYPE_92, VEHICLE_TYPE_CAR, 3.3, 168},
	{413, "Pony", FUEL_TYPE_DISEL, VEHICLE_TYPE_CAR, 8.0, 110},
	{414, "Mule", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 8.0, 105},
	{415, "Cheetah", FUEL_TYPE_98, VEHICLE_TYPE_CAR, 2.1, 192},
	{416, "Ambulance", FUEL_TYPE_DISEL, VEHICLE_TYPE_CAR, 3.6, 154},
	{417, "Leviathan", FUEL_TYPE_98, VEHICLE_TYPE_HELICOPTER, 0.0, 0},
	{418, "Moonbeam", FUEL_TYPE_DISEL, VEHICLE_TYPE_CAR, 8.4, 115},
	{419, "Esperanto", FUEL_TYPE_95, VEHICLE_TYPE_CAR, 5.0, 149},
	{420, "Taxi", FUEL_TYPE_DISEL, VEHICLE_TYPE_CAR, 4.9, 145},
	{421, "Washington", FUEL_TYPE_95, VEHICLE_TYPE_CAR, 3.8, 153},
	{422, "Bobcat", FUEL_TYPE_DISEL, VEHICLE_TYPE_CAR, 4.9, 140},
	{423, "Mr. Whoopee", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 11.5, 98},
	{424, "BF Injection", FUEL_TYPE_95, VEHICLE_TYPE_CAR, 3.2, 135},
	{425, "Hunter", FUEL_TYPE_98, VEHICLE_TYPE_HELICOPTER, 0.0, 0},
	{426, "Premier", FUEL_TYPE_95, VEHICLE_TYPE_CAR, 3.5, 173},
	{427, "Enforcer", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 4.0, 165},
	{428, "Securicar", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 5.8, 157},
	{429, "Banshee", FUEL_TYPE_98, VEHICLE_TYPE_CAR, 2.2, 201},
	{430, "Predator", FUEL_TYPE_98, VEHICLE_TYPE_BOAT, 5.4, 130},
	{431, "Bus", FUEL_TYPE_DISEL, VEHICLE_TYPE_BUS, 7.7, 130},
	{432, "Rhino", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 4.9, 94},
	{433, "Barracks", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 6.9, 110},
	{434, "Hotknife", FUEL_TYPE_95, VEHICLE_TYPE_CAR, 3.1, 167},
	{435, "Article Trailer", FUEL_TYPE_NONE, VEHICLE_TYPE_TRAILER, 0.0, 0},
	{436, "Previon", FUEL_TYPE_92, VEHICLE_TYPE_CAR, 5.0, 149},
	{437, "Coach", FUEL_TYPE_DISEL, VEHICLE_TYPE_BUS, 4.6, 157},
	{438, "Cabbie", FUEL_TYPE_DISEL, VEHICLE_TYPE_CAR, 3.9, 142},
	{439, "Stallion", FUEL_TYPE_95, VEHICLE_TYPE_CAR, 3.5, 168},
	{440, "Rumpo", FUEL_TYPE_DISEL, VEHICLE_TYPE_CAR, 5.5, 136},
	{441, "RC Bandit", FUEL_TYPE_NONE, VEHICLE_TYPE_TOY, 3.5, 75},
	{442, "Romero", FUEL_TYPE_DISEL, VEHICLE_TYPE_CAR, 6.1, 139},
	{443, "Packer", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 8.5, 126},
	{444, "Monster", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRUCK, 2.3, 110},
	{445, "Admiral", FUEL_TYPE_92, VEHICLE_TYPE_CAR, 3.8, 163},
	{446, "Squallo", FUEL_TYPE_98, VEHICLE_TYPE_BOAT, 2.6, 100},
	{447, "Seasparrow", FUEL_TYPE_98, VEHICLE_TYPE_HELICOPTER, 0.0, 0},
	{448, "Pizzaboy", FUEL_TYPE_92, VEHICLE_TYPE_MOTORBIKE, 12.0, 110},
	{449, "Tram", FUEL_TYPE_DISEL, VEHICLE_TYPE_TRAIN, 0.0, 0},
	{450, "Article Trailer 2", FUEL_TYPE_NONE, VEHICLE_TYPE_TRAILER, 0.0, 0},
};*/

enum vehicle_info_struct
{
	v_id,

	v_model_id,
	Float:v_pos_spawn_x,
	Float:v_pos_spawn_y,
	Float:v_pos_spawn_z,
	Float:v_pos_spawn_angle,
	v_vw,
	v_int,

	v_color_1,
	v_color_2,

	v_siren,
	v_respawn_delay,
	Float:v_health,
	Float:v_fuel,
}
new vehicle_info[MAX_VEHICLES][vehicle_info_struct];
new const vehicle_info_empty[vehicle_info_struct] = {
	0,
	0,

	0.0,
	0.0,
	0.0,
	0.0,

	0,
	0,

	0,
	0,

	0,
	0,
	0.0,
	0.0
};