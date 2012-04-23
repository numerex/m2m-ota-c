/*
 * Object_Byte.c
 *
 *  Created on: Aug 25, 2011
 *      Author: wlai
 */
#include <stdio.h>
#include <stdlib.h>
#include <OTA_Def.h>



//void OBJ_BYTE_setValue(OBJ_BYTE* p, BYTE data);
//BYTE OBJ_BYTE_getValue(OBJ_BYTE* p);
void OBJ_BYTE_print(OBJ_BYTE* p );



BYTE get_byte_value (OTA_Object* p)
{
		if(p == NULL)
			return 0;

		return p->payload.obj_byte.obj_data;

}

BYTEARRAY* Byte_to_BYTES(OTA_Object* p)
{
	int size = p->size(p);
	BYTEARRAY* array = new_BYTEARRAY(size);
	int offset = 0;
	array->data[offset++] = p->objectid;
	array->data[offset++] = p->objtype;
	array->data[offset++] = p->payload.obj_byte.obj_data;
	return array;

}

void set_Byte_value (OTA_Object*p, BYTE data)
{
	if(p)
		p->payload.obj_byte.obj_data =  data;
}

void OBJ_BYTE_print(OBJ_BYTE* p )
{
	if(p)
		printf("\t\t<VALUE = %x/>\n", p->obj_data);
}



void Object_Byte_print(OBJ_BYTE* p)
{
	if(p)
	{
		printf("\t\t<PAYLOAD = 0x%02X/>\n", p->obj_data);
		printf("\t\t<VALUE = %d/>\n", p->obj_data);
	}
}

void Object_Byte_print_unsigned(OBJ_BYTE* p)
{

	if(p)
		{
			printf("\t\t<PAYLOAD = 0x%02X/>\n", p->obj_data);
			printf("\t\t<VALUE = %d/>\n", (unsigned) (p->obj_data));
		}




}




void populate_Obj_Byte_socket (OTA_Object* p, int socketfd, int flags)
{


	BYTE buff;
	int n  = recv(socketfd, &buff, 1, flags);
	p->payload.obj_byte.obj_data = buff;
	return;
}

void populate_Obj_Byte_bytes (OTA_Object* p, BYTE* data)
{
	p->payload.obj_byte.obj_data = data[0];
    return;

}
