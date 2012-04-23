/*
 * OTA_Object_Byte_Array.c
 *
 *  Created on: Sep 6, 2011
 *      Author: wlai
 */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include <OTA_Def.h>




int get_bytes_array_size (OTA_Object* p)
{
	if (p == NULL)
		return 0;
	return p->payload.obj_bytes.data_size;
}

BYTE* get_bytes_array (OTA_Object* p )
{
	BYTE* buf;
	if (p == NULL)
		return 0;
	buf = malloc(p->payload.obj_bytes.data_size);
	memcpy(buf, p->payload.obj_bytes.obj_data, p->payload.obj_bytes.data_size);
	return buf;
}


void set_Bytes_value(OTA_Object*p, BYTE* data, int size)
{
	if(p == NULL)
			return;
	p->payload.obj_bytes.data_size = size;
	p->payload.obj_bytes.obj_data = malloc(size);
	memcpy(p->payload.obj_bytes.obj_data, data, size);
}



void Object_Bytes_print(OBJ_BYTE_ARRAY* p)
{

	int i=0;
	
	printf("\t\t<BYTES SIZE = %d/>\n", p->data_size);
	
	printf("\t\t<HEX>");
	for(i=0; i< (p->data_size); i++)
	{
		printf(" 0x%02X", (BYTE) p->obj_data[i]);
		if(i < (p->data_size -1))
			printf(", ");
	}
	printf("\n\t\t</HEX>\n");
}

void populate_Obj_Bytes_socket (OTA_Object* p, int socketfd, int flags)
{


	UInt16 datasize = 0;
	BYTE* buff;

	// reading byte size
	int n  = recv(socketfd, &datasize, 2, flags);
	reverseBytes((BYTE*)&datasize, 2);
	p->payload.obj_bytes.data_size = datasize;

	// total size
	buff = malloc(datasize);
	n = recv(socketfd, buff, datasize, flags);
	p->payload.obj_bytes.obj_data = buff;
	return;
}

void populate_Obj_Bytes (OTA_Object* p, BYTE* data)
{
	int offset = 0;
	UInt16 datasize;

	memcpy(&datasize, data, 2);
	offset += 2;
	reverseBytes((BYTE*)&datasize, 2);

	p->payload.obj_bytes.data_size = datasize;
	p->payload.obj_bytes.obj_data = malloc (datasize);

	memcpy(p->payload.obj_bytes.obj_data, &(data[offset]), datasize);

    return;

}

BYTEARRAY* Bytes_to_BYTES (OTA_Object* p)
{
	int size = p->size(p);
	BYTEARRAY* array = new_BYTEARRAY(size);
	UInt16 datasize;

	int offset = 0;
	array->data[offset++] = p->objectid;
	array->data[offset++] = p->objtype;




	datasize = p->payload.obj_bytes.data_size;
	reverseBytes((BYTE*)&datasize, 2);
	memcpy(&(array->data[offset]), &datasize, 2);
	offset += 2;
	memcpy(&(array->data[offset]), p->payload.obj_bytes.obj_data, p->payload.obj_bytes.data_size);

	return array;
}
