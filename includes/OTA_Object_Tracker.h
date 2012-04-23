#ifndef OTA_OBJECT_TRACKER_H
#define OTA_OBJECT_TRACKER_H

#include "OTA_Def.h"



typedef enum {
	DEVICE_ID = 0,
	LATITUDE,
	LONGITUDE,
	ALTITUDE,
	SPEED,
	HEADING,
	TIME_OF_FIX,
	TIME_TO_FIX,
	HDOP,
	PDOP,
	NUM_OF_SATELLITES_USED_IN_FIX,
	GPS_FIX_TYPE, 					//1=VALID, 2=LAST KNOWN, 0=INVALID
	LAC,							//Location Area Code  					(AT+CGED=6)
	MNC,							//Mobile Network Code					(AT+CGED=6)
	MCC,						    //Mobile Country Code					(AT+CGED=6)
	CID,							//Cell Tower ID							(AT+CGED=6)
	RSSI_AND_BIT_ERROR_RATE,		//Relative Signal Strength Indicator	(AT+CSQ?)
	FW_VER,
	PROFILE_ID,						//Which predefined profile we are using -- use some default for custom
	RADIO_ID,						//IMEI or other uniquely identifier in the case of CDMA, etc
	TIMING_ADVANCE,					//GPRS Session statistics
	ARFCN,							//Broadcast Channel						(AT+CGED=6)
	RX_LEV,							//RxLevel Received signal level on the cell, range 0-63; please refer to 3GPP TS 05.08 [28]
	BATTERY_LEVEL_ESTIMATE,			//% Remaining 0-100
	NEIGHBOR_CELL_1,				//extended AT_CGED data
	NEIGHBOR_CELL_2,				//extended AT_CGED data
	NEIGHBOR_CELL_3,				//extended AT_CGED data
	NEIGHBOR_CELL_4,				//extended AT_CGED data
	NEIGHBOR_CELL_5,				//extended AT_CGED data
	NEIGHBOR_CELL_6,				//extended AT_CGED data
	NEIGHBOR_CELL_7,				//extended AT_CGED data
	NEIGHBOR_CELL_8,				//extended AT_CGED data
	NEIGHBOR_CELL_9,				//extended AT_CGED data
	NEIGHBOR_CELL_10,				//extended AT_CGED data
	NEIGHBOR_CELL_11,				//extended AT_CGED data
	NEIGHBOR_CELL_12,				//extended AT_CGED data
	EVENT_CODE,
	SENSOR_DATA,
	SENSOR_ID,
	SENSOR_TYPE,

} ota_object_tracker_id_types;

//this lookup table will simplify generating the sub object types
//Make sure to syncronize changes or you'll get some odd stuff!
const unsigned long ota_tracker_id_to_type[] = {
	_OBJTYPE_STRING,					//DEVICE_ID
	_OBJTYPE_FLOAT,						//LATITUDE ,
	_OBJTYPE_FLOAT,						//LONGITUDE,
	_OBJTYPE_UINT,						//ALTITUDE,
	_OBJTYPE_UINT,						//SPEED,
	_OBJTYPE_UINT,						//HEADING,
	_OBJTYPE_STRING,					//TIME_OF_FIX,
	_OBJTYPE_UINT,						//TIME_TO_FIX,
	_OBJTYPE_FLOAT,						//HDOP,
	_OBJTYPE_FLOAT,						//PDOP,
	_OBJTYPE_UINT,						//NUM_OF_SATELLITES_USED_IN_FIX,
	_OBJTYPE_UINT,						//GPS_FIX_TYPE, 					//1=VALID, 2=LAST KNOWN, 0=INVALID
	_OBJTYPE_STRING,					//LAC,							//Location Area Code
	_OBJTYPE_STRING,					//MNC,							//Mobile Network Code
	_OBJTYPE_UINT,						//MCC,						    //Mobile Country Code
	_OBJTYPE_STRING,					//CID,							//Cell Tower ID
	_OBJTYPE_STRING,					//RSSI,							//Relative Signal Strength Indicator
	_OBJTYPE_STRING,					//FW_VER,
	_OBJTYPE_UINT,						//PROFILE_ID,						//Which predefined profile we are using -- use some default for custom
	_OBJTYPE_STRING,					//RADIO_ID,						//IMEI or other uniquely identifier in the case of CDMA, etc
	_OBJTYPE_UINT,						//TIMING_ADVANCE,					//GPRS Session statistics
	_OBJTYPE_UINT,						//ARFCN							//Broadcast Channel
	_OBJTYPE_UINT,						//RX_LEV
	_OBJTYPE_UINT,						//BATTERY_LEVEL_ESTIMATE
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_1,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_2,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_3,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_4,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_5,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_6,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_7,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_8,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_9,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_10,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_11,				//extended AT_CGED data_OBJTYPE_STRING,					//NEIGHBOR_CELL_1,				//extended AT_CGED data
	_OBJTYPE_STRING,					//NEIGHBOR_CELL_12,				//extended AT_CGED data
    _OBJTYPE_UINT,						//EVENT_CODE
	_OBJTYPE_ARRAY_BYTE,				//SENSOR_DATA
    _OBJTYPE_UINT,						//SENSOR_ID,
	_OBJTYPE_UINT,						//SENSOR_TYPE,
};


typedef enum {
	
	REGULAR_POSITION_REPORT,		//0
	START_MOTION_REPORT,			//1
	IN_MOTION_REPORT,			//2
	END_MOTION_REPORT,			//3
	POLLED_REPORT,				//4
	ENGINEERING,				//5
	SENSOR,					//6
	EXCEPTION,				//7
	CACHE_FORWARD,				//8
	MISC,					//9
	BH_TANK_LEVEL_REPORT,			//10
	BH_TANK_LEVEL_EXCEPTION_LOW,		//11
	BH_TANK_LEVEL_EXCEPTION_LOW_LOW,	//12
	BH_TANK_LEVEL_EXCEPTION_SUDDEN_DROP,	//13
	BH_TANK_LEVEL_HARDWARE_CHECK,		//14

} t_evt_codes;
#endif
								
