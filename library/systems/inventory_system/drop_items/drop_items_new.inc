
#define MAX_DROP_ITEMS  				1000

//==============================================================================
enum drop_item_info_struct
{
	drop_item_id,
	drop_item_count,
	Float:drop_item_pos[3],
	drop_item_vw,
	drop_item_int,

	drop_item_value_one,
	drop_item_value_two,
	drop_item_value_str[256],
	drop_item_can_drop,

	drop_item_object,
	Text3D:drop_item_text
}
new drop_item_info[MAX_DROP_ITEMS][drop_item_info_struct];