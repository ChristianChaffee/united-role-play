
#define Business:%0(%1) 					Bsnss_%0(%1)

//==============================================================================
#define GetBusinessData(%0,%1) 				business_info[%0][%1]
#define SetBusinessData(%0,%1,%2) 			business_info[%0][%1] = %2
#define AddBusinessData(%0,%1,%2) 			business_info[%0][%1] += %2

#define GetBusinessInteriorData(%0,%1) 		business_interior_info[%0][%1]

//==============================================================================
#define MAX_BUSINESS 						1000
#define BUSINESS_ENTER_PICKUP_MODEL_ID		19132

//==============================================================================
enum
{
	BUSINESS_TYPE_NONE,
	BUSINESS_TYPE_PIZZA,
	BUSINESS_TYPES_COUNT
}
//==============================================================================
new const business_type_names[BUSINESS_TYPES_COUNT][32] = {
	"None",
	"Пиццерия"
};

//==============================================================================
enum business_interior_info_struct
{
	Float:bi_exit_pos_x,
	Float:bi_exit_pos_y,
	Float:bi_exit_pos_z,
	Float:bi_exit_pos_angle,
	bi_int,
	Float:bi_cashbox_pos_x,
	Float:bi_cashbox_pos_y,
	Float:bi_cashbox_pos_z,
	bi_cashbox_pickup_model_id,
	bi_cashbox_text[128],
}
new const business_interior_info[BUSINESS_TYPES_COUNT][business_interior_info_struct] = {
	{0.0, 0.0, 0.0, 0.0, 0, 0.0, 0.0, 0.0, 0, "None"},

	{ /* Пиццерия */
		372.34, -133.42, 1001.49, 0.0,
		5,
		373.75, -118.80, 1001.49,
		1582,
		""cl_lime"[Меню пиццерии]\n"cl_grey"'Left ALT'"
	}
};

//==============================================================================
enum business_info_struct
{
	b_id,
	b_type,
	b_name[64],
	b_owner_id,
	Float:b_pos_x,
	Float:b_pos_y,
	Float:b_pos_z,
	b_vw,
	b_int,

	Text3D:b_enter_text,
	b_enter_pickup,
	Text3D:b_exit_text,
	b_exit_pickup,
	Text3D:b_cashbox_text,
	b_cashbox_pickup,
}
new business_info[MAX_BUSINESS][business_info_struct];