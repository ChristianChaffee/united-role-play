
#define EnterExits:%0(%1)							EntrExts_%0(%1)

#define GetEnterExitData(%0,%1)						enter_exit_info[%0][%1]

#define MAX_ENTER_EXITS_COUNT 						25
#define ENTER_EXIT_PICKUP_MODEL_ID 					19132
#define ENTER_EXIT_3D_TEXT_DD						15.0

//==============================================================================
enum
{
	EE_TYPE_FOR_ALL,
}

//==============================================================================
enum enter_exit_info_struct
{
	ee_name[32],

	Float:ee_enter_pos_x,
	Float:ee_enter_pos_y,
	Float:ee_enter_pos_z,
	Float:ee_enter_pos_angle,
	ee_enter_vw,
	ee_enter_int,

	Float:ee_exit_pos_x,
	Float:ee_exit_pos_y,
	Float:ee_exit_pos_z,
	Float:ee_exit_pos_angle,
	ee_exit_vw,
	ee_exit_int,

	ee_type
}
new enter_exit_info[MAX_ENTER_EXITS_COUNT][enter_exit_info_struct];
new TOTAL_ENTER_EXITS;