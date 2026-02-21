
/*

	Используемые PVar`ы:
	OpenBarriers_Cooldown

*/

//==============================================================================
#define OpenBarriers:%0(%1) 			OpnBarrrs_%0(%1)

//==============================================================================
enum
{
	OB_STATUS_CLOSE,
	OB_STATUS_OPEN
}

//==============================================================================
enum open_barrier_info_struct
{
	ob_model_id,
	Float:ob_pos[6],
	Float:ob_open_pos[6],
	ob_vw,
	ob_int,
	ob_status,
	ob_text[128],

	ob_object_id,
	Text3D:ob_3dtext
}
new open_barrier_info[][open_barrier_info_struct] = {
	{
		1495,
		{-1430.784790, -368.937073, 1249.953125, 0.000045, 0.000000, 89.999863},
		{-1430.784790, -368.937073, 1249.953125, 0.000045, 0.000000, 195.0},
		5,
		5,
		OB_STATUS_CLOSE,
		"Дверь\n"cl_grey"'Left ALT'",

		INVALID_OBJECT_ID,
		Text3D:INVALID_3DTEXT_ID
	}
};

new const BARRIERS_COUNT = sizeof(open_barrier_info);

//==============================================================================
enum open_barrier_material_info_str
{
	obm_index,
	obm_material_index,
	obm_model_id,
	obm_lib_name[32],
	obm_texture_name[32],
	obm_color
}
new const open_barrier_material_info[][open_barrier_material_info_str] = {
	{0, 2, 1560, "7_11_door", "cj_sheetmetal2", 0xFF3D3D3D},
	{0, 1, 10806, "airfence_sfse", "ws_griddyfence", 0},
	{0, 0, 1560, "7_11_door", "CJ_CHROME2", 0xFFFFFFFF}
};

new const BARRIERS_MATERIALS_COUNT = sizeof(open_barrier_material_info);