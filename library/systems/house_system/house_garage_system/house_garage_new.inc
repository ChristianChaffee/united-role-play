
#define GetGarageData(%0,%1) 					garage_info[%0][%1]
#define SetGarageData(%0,%1,%2) 				garage_info[%0][%1] = %2

//==============================================================================
#define MAX_GARAGES 							MAX_HOUSES

#define GARAGE_ENTER_PICKUP_MODEL 				19197

#define GARAGE_TP_POS 							945.6981, 2129.2825, 1004.4070
#define GARAGE_TP_ANGLE 						270.0
#define GARAGE_TP_INT 							1

//==============================================================================
enum garage_info_struct
{
	g_id,
	g_house_id,
	Float:g_pos_x,
	Float:g_pos_z,
	Float:g_pos_y,

	g_enter_pickup,
	Text3D:g_enter_text,
	Text3D:g_exit_text,
}
new garage_info[MAX_GARAGES][garage_info_struct];