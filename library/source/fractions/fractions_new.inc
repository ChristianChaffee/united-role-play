
#define Fraction:%0(%1) 				Frctn_%0(%1)

//==============================================================================
#define GetFractionData(%0,%1) 			fraction_info[%0][%1]
#define SetFractionData(%0,%1,%2) 		fraction_info[%0][%1] = %2
#define AddFractionData(%0,%1,%2)  		fraction_info[%0][%1] += %2

//==============================================================================
#define MAX_FRACTIONS 				30
#define FRACTIONS_TYPES_COUNT 		5

//==============================================================================
new const fraction_type_names[FRACTIONS_TYPES_COUNT][64] = {
	"Правительство",
	"Правоохранительные органы",
	"Организация здравоохранения",
	"Средство массовой информации",
	"Криминальная структура"
};

//==============================================================================
enum fraction_info_struct
{
	f_id,
	f_name[64],
	f_tag[16],
	f_type,
	f_leader_account_id,
	f_color_str[16],
	f_color_value
}
new fraction_info[MAX_FRACTIONS][fraction_info_struct];
