
#define VEHICLE_MODELS_COUNT 						411

#define GetVehicleData(%0,%1)  						vehicle_info[%0][%1]
#define SetVehicleData(%0,%1,%2)  					vehicle_info[%0][%1] = %2

//==============================================================================
enum vehicle_params_info_struct
{
	vp_model_id,
	vp_name[64],
	vp_max_speed,
	vp_acceleration,
	vp_fuel_type,
	vp_max_fuel,
	vp_type,
}
/*new const vehicle_params_info[VEHICLE_MODELS_COUNT][vehicle_params_info_struct] = {

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
}
new vehicle_info[MAX_VEHICLES][vehicle_info_struct];