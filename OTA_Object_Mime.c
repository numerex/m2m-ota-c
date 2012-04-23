/*
 * OTA_Object_Mime.c
 *
 *  Created on: Aug 26, 2011
 *      Author: wlai
 */

#include <stdlib.h>
#include <string.h>
#include <OTA_Def.h>

int mime_content_size (OTA_Object* p)
{
	if(p == NULL)
		return 0;
	return p->payload.obj_mime.data_size;

}

_CONTENT_TYPE getMimeContentType (OTA_Object* p )
{
	if (p == NULL)
		return 0;

	return (_CONTENT_TYPE) (p->payload.obj_mime.mime_type & 0xF0);
}

_SUB_CONTENT_TYPE getMimeSubContentType(OTA_Object* p )
{

	if (p == NULL)
		return 0;

	return (_SUB_CONTENT_TYPE) (p->payload.obj_mime.mime_type & 0x0F);

}

BYTE* get_mime_content (struct Object* p)
{
	if (p == NULL)
		return 0;

	return p->payload.obj_mime.obj_data;
}



void 	Object_Mime_setContent(OTA_Object* p,  _CONTENT_TYPE content_type,  _SUB_CONTENT_TYPE subcontent_type)
{
		BYTE mime_type = content_type  | subcontent_type;
		p->payload.obj_mime.mime_type = mime_type;
}

void 	Object_Mime_setData(OTA_Object* p, BYTE* location, int size )
{
	
	BYTE* tmp;

	if(location == NULL)
		return;

    tmp = p->payload.obj_mime.obj_data;
	if(tmp != NULL)
		free(tmp);

	p->payload.obj_mime.obj_data = malloc (size);
	memcpy(p->payload.obj_mime.obj_data, (void*) location, size);

	p->payload.obj_mime.data_size = size;

}

void set_Mime_value(OTA_Object* p, _CONTENT_TYPE content_type,  _SUB_CONTENT_TYPE subcontent_type, BYTE* location, int size)
{
	Object_Mime_setContent(p, content_type, subcontent_type);
	Object_Mime_setData(p, location, size);

}


void 	Object_Mime_print(OBJ_MIME* p) {


	BYTE content_type = p->mime_type & 0xF0;
	BYTE sub_content_type;

	printf("\t\t<DATA SIZE = %d/>\n", p->data_size);
	printf("\t\t<MIME_TYPE = ");


	switch (content_type)
	{
	case APPLICATION :
		printf("APPLICATION/");
		break;

	case AUDIO:
		printf("AUDIO/");
		break;

	case VIDEO:
				printf("VIDEO/");
				break;

	case IMAGE:
				printf("IMAGE/");
				break;

	case MESSAGE:
				printf("MESSAGE/");
				break;

	case TEXT:
				printf("TEXT/");
				break;
	default:
			printf("X/");
			break;
	}

	sub_content_type = p->mime_type & 0x0F;
	switch (sub_content_type)
	{
	case JPG:
		printf("JPG");
		break;
	case GIF:
		printf("GIF");
		break;
	case PNG:
		printf("PNG");
		break;
	default:
		printf("X");
		break;
	}

	printf("/>\n");

}

void populate_Obj_Mime_socket (OTA_Object*p, int socketfd, int flags)
{
	UInt32 d32 = 0;
	BYTE type;
	_CONTENT_TYPE ctype;
	_SUB_CONTENT_TYPE stype;
	BYTE* buffer;

	int n = recv(socketfd, &d32, 4, flags);
	reverseBytes((BYTE*)&d32, 4);
	p->payload.obj_mime.data_size = d32;

	type = 0;
	n = recv(socketfd, &type, 1, flags);
	ctype = type | 0xF0;
	stype = type | 0x0F;


	buffer = malloc (d32);
	n = recv(socketfd, buffer, d32, flags);
	p->payload.obj_mime.set(p, ctype, stype, buffer, d32);
	free(buffer);


}
void populate_Obj_Mime(OTA_Object* p, BYTE* data)
{
	// size 4 byte,
	// type 1 byte
	int offset = 0;
	_CONTENT_TYPE ctype;
	_SUB_CONTENT_TYPE stype;
	BYTE type;
	UInt32 d32 = 0;
	memcpy(&d32, &(data[offset]), sizeof(d32));
	offset += sizeof(d32);
	reverseBytes((BYTE*)&d32, 4);
	p->payload.obj_mime.data_size = d32;

	type = data[offset++];
	ctype = type & 0xF0;
	stype = type & 0x0F;
	p->payload.obj_mime.set(p, ctype, stype, &(data[offset]), d32);

}

BYTEARRAY* Mime_to_BYTES(OTA_Object* p)
{
	int size = p->size(p);
	BYTEARRAY* array = new_BYTEARRAY(size);
	UInt32 d32 = p->payload.obj_mime.data_size;
	int offset = 0;
	array->data[offset++] = p->objectid;
	array->data[offset++] = p->objtype;

	reverseBytes((BYTE*)&d32, 4);
	memcpy(&(array->data[offset]), &d32, 4);
	offset += 4;
	array->data[offset++] = p->payload.obj_mime.mime_type;
	memcpy(&(array->data[offset]), p->payload.obj_mime.obj_data, p->payload.obj_mime.data_size);

	return array;

}
