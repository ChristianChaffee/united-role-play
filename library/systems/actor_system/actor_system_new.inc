
#define Actor:%0(%1)					Actr_%0(%1)

#define GetActorInfo(%0,%1)  			actor_info[%0][%1]

//==============================================================================
enum actor_info_struct
{
	a_name[32],
	a_model_id,
	Float:a_pos_x,
	Float:a_pos_y,
	Float:a_pos_z,
	Float:a_pos_angle,
	a_invulnerable,
	a_vw,
	a_int,
	a_anim_lib[32],
	a_anim_name[32]
}
new actor_info[MAX_ACTORS][actor_info_struct];