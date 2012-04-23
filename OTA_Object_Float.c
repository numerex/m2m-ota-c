/*
 * Object_Byte.c
 *
 *  Created on: Aug 25, 2011
 *      Author: wlai
 */
#include <stdio.h>
#include <stdlib.h>
#include <OTA_Def.h>



float get_float_value(OTA_Object* p)
{
	if (p == NULL)
		return 0.0;

	return p->payload.obj_float.obj_data;
}




BYTEARRAY* Float_to_BYTES(OTA_Object* p)
{
	float temp;
	int size = p->size(p);
	BYTE* p2;
	BYTEARRAY* array = new_BYTEARRAY(size);
	int offset = 0;
	array->data[offset++] = p->objectid;
	array->data[offset++] = p->objtype;

	temp = p->payload.obj_float.obj_data;
	p2 = (BYTE*)&temp;

	reverseBytes(p2, 4);

	memcpy(&(array->data[offset]),p2, sizeof(float));
	return array;

}

void set_Float_value (OTA_Object*p, float data)
{
	if(p)
		p->payload.obj_float.obj_data =  data;
}

void OBJ_FLOAT_print(OBJ_FLOAT* p )
{
	if(p)
		printf("\t\t<VALUE = %f/>\n", p->obj_data);
}


void Object_Float_print(OBJ_FLOAT* p)
{
	printf("\t\t<PAYLOAD = %f/>\n", p->obj_data);
}

void populate_Obj_Float_socket (OTA_Object* p, int socketfd, int flags)
{

	// reading 4 bytes data
	//float* f;
	BYTE buff[sizeof(float)];
	int n  = recv(socketfd, &buff, sizeof(float), flags);
	reverseBytes (buff, sizeof(float));
	//f = &buff;	
	//p->payload.obj_float.obj_data = *f;
	memcpy(&p->payload.obj_float.obj_data, buff, sizeof(float));
	return;
}

void populate_Obj_Float_bytes (OTA_Object* p, BYTE* data)
{
	float b = 0.0;
	memcpy(&b, data, sizeof(float));
	reverseBytes((void *)&b, sizeof(float));
	p->payload.obj_float.obj_data = b;

    return;

}
