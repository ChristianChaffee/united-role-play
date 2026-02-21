
enum fraction_commands_info_struct
{
	fc_command[16],
	fc_types[FRACTIONS_TYPES_COUNT],
	fc_description[128],
	fc_access_rank,
	fc_on_duty_check
}

//==============================================================================
new const fraction_commands_info[][fraction_commands_info_struct] = {
	{
		"r", 
		{FRAC_TYPE_GOVERMENT, FRAC_TYPE_POLICE, FRAC_TYPE_MEDIC, FRAC_TYPE_NEWS, -1},
		"Рация (чат) фракции.",
		1,
		true
	},
	{
		"rb",
		{FRAC_TYPE_GOVERMENT, FRAC_TYPE_POLICE, FRAC_TYPE_MEDIC, FRAC_TYPE_NEWS, -1},
		"OOC чат организации.",
		1,
		true
	},
	{
		"members",
		{FRAC_TYPE_GOVERMENT, FRAC_TYPE_POLICE, FRAC_TYPE_MEDIC, FRAC_TYPE_NEWS, FRAC_TYPE_CRIMINAL},
		"Список членов фракции онлайн.",
		1,
		false
	},
	{
		"taxes",
		{FRAC_TYPE_GOVERMENT, -1, -1, -1, -1},
		"Управление налогооблажением.",
		10,
		true
	}
};

new const FRACTION_COMMANDS_COUNT = sizeof(fraction_commands_info);