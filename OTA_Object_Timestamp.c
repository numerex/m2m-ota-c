/*
 * OTA_Object_String.c
 *
 *  Created on: Sep 2, 2011
 *      Author: wlai
 */

#include <string.h>
#include <OTA_Def.h>
#ifndef EMBEDDED
#include <time.h>
#endif

 time_t get_Time_value (OTA_Object* p)
 {
	 if (p == NULL)
		 return 0.0;
	  return (time_t) p->payload.obj_timestamp.time;
 }


void set_Time_value(OTA_Object* p, ULong value)
{

	if(p == NULL)
		return;

	if(value == 0)
	{
		 time((time_t*)&value);
		 value *= 1000; // in milliseconds.
	}
	p->payload.obj_timestamp.time = value;

}


void 	Object_Timestamp_print(OBJ_TIMESTAMP* p) {

        long timestamp = (p->time)/1000;
#ifndef EMBEDDED
		printf("\t\t<TIME = %s/>\n", ctime(&timestamp));
#else
		printf("\t\t<TIME = %d/>\n", timestamp);
#endif
		printf("\t\t<VALUE = \"%ld\"/>\n", p->time);

}

void populate_Obj_Timestamp_socket (OTA_Object*p, int socketfd, int flags)
{
	ULong d64 = 0;
	
	int n = recv(socketfd, &d64, 8, flags);
	reverseBytes((BYTE*)&d64, 8);

	p->payload.obj_timestamp.time = d64;

}
void populate_Obj_Time(OTA_Object* p, BYTE* data)
{
	// size 2 byte,
	int offset = 0;
	ULong d64 = 0;
	memcpy(&d64, &(data[offset]), sizeof(d64));
	reverseBytes((BYTE*)&d64, 8);
	p->payload.obj_timestamp.set(p,  d64);
}

BYTEARRAY* Timestamp_to_BYTES(OTA_Object* p)
{
	int size = p->size(p);
	BYTEARRAY* array = new_BYTEARRAY(size);
	ULong d64 = p->payload.obj_timestamp.time;
	int offset = 0;
	array->data[offset++] = p->objectid;
	array->data[offset++] = p->objtype;

	reverseBytes((BYTE*)&d64, 8);
	memcpy(&(array->data[offset]), &d64, 8);
	return array;

}
