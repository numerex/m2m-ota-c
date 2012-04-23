/*
 * OTA_Object_String.c
 *
 *  Created on: Sep 2, 2011
 *      Author: wlai
 */

#include <string.h>
#include <OTA_Def.h>

char* get_String_value (OTA_Object* p)
{

	if (p == NULL)
		return NULL;

	return (char*) (p->payload.obj_string.obj_data);
}


void set_String_value(OTA_Object* p, char* string)
{

	char* loc;
	int len;

	loc = (char*) (p->payload.obj_string.obj_data);
	if(loc != NULL)
		free(loc);

	if(p == NULL || string == NULL)
		return;

    	len = strlen(string);

	p->payload.obj_string.obj_data = malloc(len + 1);
	strcpy(p->payload.obj_string.obj_data, string);
	p->payload.obj_string.data_size = len;
}


void 	Object_String_print(OBJ_STRING* p) {


		printf("\t\t<DATA SIZE = %d/>\n", p->data_size);
		printf("\t\t<STRING = \"%s\"/>\n", p->obj_data);
}

void populate_Obj_String_socket (OTA_Object*p, int socketfd, int flags)
{
	UInt16 d16 = 0;
	int n = recv(socketfd, &d16, 2, flags);
	BYTE* buffer;

	reverseBytes((BYTE*)&d16, 4);
	p->payload.obj_string.data_size = d16;
	if(d16 <= 0)
		return;

	buffer = malloc (d16+1);
	n = recv(socketfd, buffer, d16, flags);
	p->payload.obj_string.set(p, (char*) buffer);
	free(buffer);


}
void populate_Obj_String(OTA_Object* p, BYTE* data)
{



	// size 2 byte,
	int offset = 0;
	UInt16 d16 = 0;
	memcpy(&d16, &(data[offset]), sizeof(d16));
	offset += sizeof(d16);
	reverseBytes((BYTE*)&d16, 2);
	p->payload.obj_string.data_size = d16;
	
        if(d16 > 0)
	{
		if(p->payload.obj_string.obj_data != NULL)
			free(p->payload.obj_string.obj_data);
        	p->payload.obj_string.obj_data = malloc(d16 + 1);
        	strncpy(p->payload.obj_string.obj_data, &(data[offset]), d16);
		p->payload.obj_string.obj_data[d16] = 0; 
		
	}	
	else	
		p->payload.obj_string.set(p, NULL);

}

BYTEARRAY* String_to_BYTES(OTA_Object* p)
{
	int size = p->size(p);
	BYTEARRAY* array = new_BYTEARRAY(size);
	UInt16 d16 = p->payload.obj_string.data_size;
	int offset = 0;
	array->data[offset++] = p->objectid;
	array->data[offset++] = p->objtype;

	reverseBytes((BYTE*)&d16, 2);
	memcpy(&(array->data[offset]), &d16, 2);
	offset += 2;

	memcpy(&(array->data[offset]), p->payload.obj_string.obj_data, p->payload.obj_string.data_size);

	return array;

}
