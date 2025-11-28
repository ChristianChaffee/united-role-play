
/*
Используемые PVar`ы:
	Report_SelectedType
	Report_ID
	Report_Answer
*/

//==============================================================================
#define Report:%0(%1) 					Rprt_%0(%1)

//==============================================================================
enum
{
	REPORT_TYPE_NONE,
	REPORT_TYPE_QUESTION,
	REPORT_TYPE_COMPLAINT
}

enum
{
	REPORT_STATUS_NONE,
	REPORT_STATUS_WAIT,
	REPORT_STATUS_ANSWERED,
	REPORT_STATUS_CHECKED
}

enum
{
	REPORT_CALLBACK_ACTION_LIST,
	REPORT_CALLBACK_ENTER_ANSWER,
	REPORT_CALLBACK_DELETE,
	REPORT_CALLBACK_SEND_ANSWER
}