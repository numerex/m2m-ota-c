/*
 * OTA_Msg_MO.c
 *
 *  Created on: Aug 26, 2011
 *      Author: wlai
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <OTA_Def.h>
#ifndef EMBEDDED
#include <time.h>
#endif

void Msg_set_Seq_ID(OTA_Message* p, UInt16 seqid);
UInt16 Msg_get_Seq_ID(OTA_Message* p);
void Msg_set_Event_Code(OTA_Message* p, BYTE event_code);
BYTE Msg_get_Event_Code(OTA_Message*p);
BYTE Msg_get_Obj_Cnt(OTA_Message* p);
void Msg_Add_Object( OTA_Message* p, OTA_Object* o);
void print_Message(OTA_Message* p);
BYTEARRAY*  Msg_getBytes(OTA_Message* p);
int send_Msg(OTA_Message* p, int socketfd, int flags);
void delete_Msg(OTA_Message* p);
BYTE Msg_updateCRC (OTA_Message* p);
void Msg_Add_OID( OTA_Message* p, BYTE oid);
MESSAGE_TYPE getMessageType(OTA_Message* p);
ULong getMsgTime(OTA_Message* p);
int MsgObjCnt(OTA_Message* p);
int get_Msg_Size(OTA_Message* p);

OBJ_ID* getMsgOids		(struct OTA_MSG*);
OTA_Object* getMsgObjs		(struct OTA_MSG*);

BYTE _getMailCount 		(struct OTA_MSG*);
void _setMTRoute		(struct OTA_MSG*, MT_ROUTE_TYPE route_type);





extern Msg_Header*  new_OTA_MSG_Header( Msg_Header* p, BYTE type, BYTE event_code, UInt16 seq_id);
extern Msg_Header* createHeaderFromBytes( Msg_Header* header, BYTE* bytes);
OTA_Message* new_OTA_Message (MESSAGE_TYPE msgtype)
{
//#ifndef EMBEDDED
	    OTA_Message* p = malloc (sizeof(OTA_Message));
//#else
//		OTA_Message* p = obj_malloc (sizeof(OTA_Message));
//#endif
	    new_OTA_MSG_Header(&(p->header), msgtype, 0, 0);

		p->crc = 0;
		p->obj_cnt = 0;
		p->mb_command_count = 0; 
		p->mb_command = 0;
                p->mb_route_type = 0;
		p->mb_mail_count = 0; 
		p->print = print_Message;
		p->set_Seq_ID = Msg_set_Seq_ID;
		p->get_Seq_ID = Msg_get_Seq_ID;
		p->set_Event_Code = Msg_set_Event_Code;
		p->get_Event_Code = Msg_get_Event_Code;
		p->Delete = delete_Msg;
		p->send = send_Msg;
		p->getBytes = Msg_getBytes;
		p->updateCRC = Msg_updateCRC;
		p->type = getMessageType;
		p->getTime = getMsgTime;
		p->objcnt = Msg_get_Obj_Cnt;

		p->objlist.obj = NULL;
		p->objlist.oid = NULL;
		switch (msgtype)
		{
		case _MOBILE_ORIGINATED_EVENT:
		case _MOBILE_ORIGINATED_ACK:
			p->add_obj =   Msg_Add_Object;
			p->getObjs = getMsgObjs;
			p->add_objid = NULL;
			p->getOids = NULL;
			p->setMTRoute = NULL; 
			p->getMailCount = NULL;
			break;

		case _MOBILE_TERMINATED_EVENT:
			p->add_objid = Msg_Add_OID;
			p->getOids = getMsgOids;
			p->getObjs = NULL;
			p->add_obj = NULL;
			p->setMTRoute = NULL; 
			p->getMailCount = NULL;
			break;

		case _MOBILE_TERMINATED_ACK:
			p->add_objid = NULL;
			p->add_obj = NULL;
			p->getOids = NULL;
			p->getObjs = NULL;
			p->setMTRoute = NULL; 
			p->getMailCount = NULL;
			break;

		case _MOBILE_ORIGINATED_MAILBOX_REQ:
                        p->add_objid = NULL;
                        p->add_obj = NULL;
                        p->getOids = NULL;
                        p->getObjs = NULL;
			p->setMTRoute = _setMTRoute; 
			p->getMailCount = NULL;
                        break;

		case _MOBILE_ORIGINATED_MAILBOX_ACK:
                        p->add_objid = NULL;
                        p->add_obj = NULL;
                        p->getOids = NULL;
                        p->getObjs = NULL;
			p->setMTRoute = NULL; 
			p->getMailCount = _getMailCount;
                        break;

		}
		p->updateCRC(p);
		return p;

}

BYTE _getMailCount (struct OTA_MSG* m )
{
	if(m->type(m) != _MOBILE_ORIGINATED_MAILBOX_ACK)
		return 0;

	return m->mb_mail_count;
}
 
void _setMTRoute  (struct OTA_MSG* m, MT_ROUTE_TYPE route_type)
{

	if(m->type(m) != _MOBILE_ORIGINATED_MAILBOX_REQ)
		return;
	m->mb_command_count++; 
        m->mb_command = SET_MT_ROUTE;
	m->mb_route_type = route_type; 

}

OBJ_ID* getMsgOids			(struct OTA_MSG* m )
{

	if(m->type(m) != _MOBILE_TERMINATED_EVENT)
		return 0;

	if(m->objcnt(m) > 0)
		return m->objlist.oid;
	else
		return 0;
}
OTA_Object* getMsgObjs		(struct OTA_MSG* m)
{
	if(m->type(m) != _MOBILE_ORIGINATED_EVENT && m->type(m) != _MOBILE_ORIGINATED_ACK)
		return 0;

	if(m->objcnt(m) > 0)
		return m->objlist.obj;
	else
		return 0;




}
MESSAGE_TYPE getMessageType(OTA_Message* p)
{
	return (MESSAGE_TYPE) (p->header.type);

}

ULong getMsgTime(OTA_Message* p)
{
	return p->header.timestamp;
}


void delete_Msg(OTA_Message* p)
{
	MESSAGE_TYPE type;
	
	OTA_Object* obj = NULL;
	OTA_Object* n = NULL;

	OBJ_ID* id = NULL;
	OBJ_ID* ni = NULL;
		
	if (p == NULL)
		return;

	type = p->type(p);

	switch(type)
	{
	case _MOBILE_ORIGINATED_EVENT:
	case _MOBILE_ORIGINATED_ACK:
		obj = p->objlist.obj;
		while(obj != NULL)
		{
			n = obj->next;
			obj->Delete(obj);
			obj = n;
		}
		break;

	case _MOBILE_TERMINATED_EVENT:
		id = p->objlist.oid;
		while(id != NULL)
		{
			ni = id->next;
			free(id);
			id = ni;
		}
		break;
	default:
		break;
	}
	free(p);
	p = NULL;
}

void Msg_set_Seq_ID(OTA_Message* p, UInt16 seqid)
{
	p->header.seq_id = seqid;
}

UInt16 Msg_get_Seq_ID(OTA_Message* p)
{
	return p->header.seq_id;
}

void Msg_set_Event_Code(OTA_Message* p, BYTE event_code)
{
	p->header.event_code = event_code;
}

BYTE Msg_get_Event_Code(OTA_Message* p)
{
	return p->header.event_code;
}

int MsgObjCnt(OTA_Message* p)
{
	return p->obj_cnt;
}

BYTE Msg_get_Obj_Cnt(OTA_Message* p)
{
	return p->obj_cnt;
}

void Msg_Add_Object( OTA_Message* p, OTA_Object* o)
{
	
	OTA_Object *obj;

	if(p == NULL || o == NULL)
		return;

	if(o->isEmpty(o) == 1)
	{
		printf("Error: trying to add an empty object\n");
		o->print(o); 
		return; 
	}

	obj = p->objlist.obj;

	if(obj == NULL)
		p->objlist.obj = o;

	else {
		while(obj->next != NULL)
			obj = obj->next;
		obj->next = o;
	}
	p->obj_cnt++;
	p->updateCRC(p);
}


void print_Message(OTA_Message* p){
	
	MESSAGE_TYPE type;
	char* msghead = "UNKNOWN";
	OTA_Object* o = NULL;
	OBJ_ID* id = NULL;

	if(p == NULL)
		return;

	type = p->type(p);
	
	switch(type)
	{
	case _MOBILE_ORIGINATED_EVENT:
		msghead = "MOBILE ORIGINATED EVENT";
		break;
	case _MOBILE_ORIGINATED_ACK:
		msghead = "MOBILE ORIGINATED ACK";
		break;
	case _MOBILE_TERMINATED_EVENT:
		msghead = "MOBILE TERMINATED EVENT";
		break;
	case _MOBILE_TERMINATED_ACK:
		msghead = "MOBILE TERMINATED ACK";
		break;
	case _MOBILE_ORIGINATED_MAILBOX_REQ:
		msghead = "MOBILE ORIGINATED MAILBOX REQUEST";
		break;
	case _MOBILE_ORIGINATED_MAILBOX_ACK:
		msghead = "MOBILE ORIGINATED MAILBOX ACK";
		break;
	}
	printf("<%s>\n", msghead);
	p->header.print (&(p->header));

	if(type != _MOBILE_TERMINATED_ACK && type != _MOBILE_ORIGINATED_MAILBOX_REQ &&
		type != _MOBILE_ORIGINATED_MAILBOX_ACK)
		printf("\t<OBJ CNT=%d/>\n", p->obj_cnt);



	switch(type)
	{
	case _MOBILE_ORIGINATED_EVENT:
	case _MOBILE_ORIGINATED_ACK:
		o = p->objlist.obj;
		while(o != NULL)
		{
				o->print(o);
				o = o->next;
		}
		break;

	case _MOBILE_TERMINATED_EVENT:
		id = p->objlist.oid;
		while(id != NULL)
		{
			printf("\t\t<OID = %d/>\n", id->id);
			id = id->next;
		}
		break;

	case _MOBILE_ORIGINATED_MAILBOX_REQ:
		if(p->mb_command_count > 0)
		{
			printf("\t\t<MB_COMMAND_COUNT = %d/>\n", p->mb_command_count); 
			printf("\t\t<MB_COMMAND = %d/>\n", p->mb_command);
			printf("\t\t<MT_ROUTE = %d/>\n", p->mb_route_type); 
		}
		break;

	case _MOBILE_ORIGINATED_MAILBOX_ACK:
		printf("\t\t<MAIL_COUNT = %d/>\n", p->mb_mail_count); 
		break;

	default:
		break;

	}

	p->updateCRC(p);
    printf("\t<CRC = 0x%02X/>\n", p->crc);

    printf("</%s>\n", msghead);
    return;
}


BYTE getCRC8(BYTE* data, int length)
{
	   
	   	short _register = 0;
		short bitMask = 0;
		short poly = 0;
		int i = 1;
	   
	    if(data == NULL)
	    	return 0;


	
		_register = data[0];
        
		for ( i=1; i<length; i++)  {
			_register = (short)((_register << 8) | (data[i] & 0x00ff));
			poly = (short)(0x0107 << 7);
			bitMask = (short)0x8000;

			while (bitMask != 0x0080)  {
				if ((_register & bitMask) != 0) {
					_register ^= poly;
				}
				poly = (short) ((poly&0x0000ffff) >> 1);
				poly &= 0x7FFF;
				bitMask = (short)((bitMask&0x0000ffff) >> 1);
				bitMask &= 0x7FFF;
			}
		}
		return (char)_register;
}



BYTE Msg_updateCRC (OTA_Message* p)
{

	BYTEARRAY* b = p->getBytes(p);
	b->Delete(b);
	return p->crc;

}



BYTEARRAY*  Msg_getBytes(OTA_Message* p)
{

    BYTEARRAY* header = p->header.getBytes(&(p->header));
    BYTEARRAY* buff = NULL;
	BYTEARRAY* temp = NULL;
	BYTEARRAY* n;
	OBJ_ID* id;
	OTA_Object* o;
	BYTEARRAY* obj_ptr;
	int offset = 0;
	BYTE crc;
    MESSAGE_TYPE type = p->type(p);
    switch(type)
    {
    case _MOBILE_ORIGINATED_EVENT:
    case _MOBILE_ORIGINATED_ACK:
    	buff = new_BYTEARRAY(HEADER_SIZE + 1);
    	buff->put(buff, header, 0);
    	header->Delete(header);
    	buff->data[HEADER_SIZE] = p->obj_cnt;
    	o  = p->objlist.obj;
    	obj_ptr = NULL;
    	while( o != NULL)
    		{
    			obj_ptr = o->getBytes(o);
    			buff->append(buff, obj_ptr);
    			obj_ptr->Delete(obj_ptr);
    			o = o->next;
    		}
    break;

    case _MOBILE_TERMINATED_EVENT:
    	buff = new_BYTEARRAY(HEADER_SIZE + 1);
    	buff->put(buff, header, 0);
    	header->Delete(header);
    	buff->data[HEADER_SIZE] = p->obj_cnt;
    	
    	if(p->obj_cnt > 0)
    	{
    		n = new_BYTEARRAY(p->obj_cnt);
    		id = p->objlist.oid;
    		while(id != NULL)
    		{
    			  n->data[offset++] = id->id;
    			  id = id->next;
    		}
    		buff->append(buff, n);
    		n->Delete(n);
    	}
    break;


    case _MOBILE_TERMINATED_ACK:
    	buff = header;
    break;


    case _MOBILE_ORIGINATED_MAILBOX_REQ:
	if(p->mb_command_count > 0)
           buff = new_BYTEARRAY(HEADER_SIZE+3); // 	
	else
	   buff = new_BYTEARRAY(HEADER_SIZE+1);

    	buff->put(buff, header, 0);
    	header->Delete(header);
    	buff->data[HEADER_SIZE] = p->mb_command_count;
	if(p->mb_command_count > 0)
        {
		buff->data[HEADER_SIZE+1] = p->mb_command;
		buff->data[HEADER_SIZE+2] = p->mb_route_type; 
	}
	break;

    case _MOBILE_ORIGINATED_MAILBOX_ACK:
        buff = new_BYTEARRAY(HEADER_SIZE+1);
        buff->put(buff, header, 0);
        header->Delete(header);
        buff->data[HEADER_SIZE] = p->mb_mail_count;
        break;

    default:
    	break;
    }


	crc = getCRC8(buff->data, buff->size);

	p->crc = crc;

	if(buff != NULL)
	{
		temp = new_BYTEARRAY(buff->size + 1);
		temp->put(temp, buff, 0);
		temp->data[temp->size-1] = crc;
		buff->Delete(buff);
		buff = temp;
	}

	return buff;

}


int send_Msg(OTA_Message* p, int socketfd, int flags)
{

	BYTEARRAY* bytes = p->getBytes(p);
	int sentbytes = send(socketfd, bytes->data, bytes->size, flags);

	if( bytes->size != sentbytes)
	{
		DieWithError("Failed to send to socket");

	}
	bytes->Delete(bytes);
	return (sentbytes);

}
void Msg_Add_OID( OTA_Message* p, BYTE oid)
{
	  OBJ_ID* o;
	  OBJ_ID* n;

	  if(p == NULL)
			  return;

		  o = malloc(sizeof(OBJ_ID));
		  o->id = oid;
		  o->next = NULL;

		  n = p->objlist.oid;
		  if(n == NULL)
			  p->objlist.oid = o;
		  else
		  {
			  while(n->next != NULL)
				  n = n->next;
			  n->next = o;
		  }
		  p->obj_cnt++;
		  p->updateCRC(p);

}


OTA_Message*  recvFromSocket(int socketfd, int flags)
{
	BYTE h_buff [HEADER_SIZE];
	BYTE type;
	int byte = recv(socketfd, &h_buff, HEADER_SIZE, flags);
	BYTE crc;

	OTA_Message* p;
	BYTE objcnt = 0;
	int n = 0;
	BYTE oid = 0;

	int i = 0;
	BYTE recvcrc  = 0;

	Msg_Header* header = createHeaderFromBytes(NULL, h_buff);
	if(header == NULL)
	{
		DieWithError ("Failed to read header");
		return NULL;
	}


	type = header->type;
	if( ( type != _MOBILE_TERMINATED_EVENT) && (type != _MOBILE_TERMINATED_ACK) &&
			(type != _MOBILE_ORIGINATED_EVENT) && (type != _MOBILE_ORIGINATED_ACK) && 
			(type != _MOBILE_ORIGINATED_MAILBOX_REQ) && (type != _MOBILE_ORIGINATED_MAILBOX_ACK))
	{

		DieWithError("Not recognized  message type");
		return NULL;
	}

	p = new_OTA_Message(header->type);
	memcpy(&(p->header), header, HEADER_SIZE);
	free(header);

	switch (p->type(p))
	 {
	 case _MOBILE_ORIGINATED_EVENT:
	 case _MOBILE_ORIGINATED_ACK:
		n = recv(socketfd, &objcnt, 1, flags);
		p->obj_cnt = 0;
		for( i=0; i<objcnt; i++)
		{
			 OTA_Object* obj = new_OTA_Object_Socket(socketfd, flags);
			 p->add_obj(p, obj);
		}
		break;

	 case _MOBILE_TERMINATED_EVENT:
		   n = recv(socketfd, &objcnt, 1, flags);
		   p->obj_cnt = 0;
	 		for(i=0; i<objcnt; i++)
	 		{
	 			n = recv(socketfd, &oid,1,flags);
	 			p->add_objid(p, oid);
	 		}
	 		break;

	 case _MOBILE_TERMINATED_ACK:
	 		break;

	 case _MOBILE_ORIGINATED_MAILBOX_REQ:
		  n = recv(socketfd, &objcnt, 1, flags);
                  p->mb_command_count = objcnt;
		  n = recv(socketfd, &objcnt, 1, flags);
                  p->mb_command = objcnt; 
		  n = recv(socketfd, &objcnt, 1, flags);
                  p->mb_route_type = objcnt;
                  break;
 
                 
	 case _MOBILE_ORIGINATED_MAILBOX_ACK:
                  n = recv(socketfd, &objcnt, 1, flags);
                  p->mb_mail_count = objcnt;
                  break;

	 	}

	 crc = p->updateCRC(p);
	 
	 n = recv(socketfd, &recvcrc, 1, flags);
	 if(crc != recvcrc)
	 {
		 DieWithError("CRC check failed.");
	 }
	 return p;
}



OTA_Message*  createMessageFromBytes(BYTE* bytes)
{
	
	BYTE msgtype;
	OTA_Message* p;
	Msg_Header* header;
	int offset;
	BYTE objcnt = 0;
	int i=0;
	BYTE crc;
	if(bytes == NULL)
		return NULL;

	msgtype = bytes[0];

	p = new_OTA_Message((MESSAGE_TYPE) msgtype);
	header = createHeaderFromBytes(&(p->header), bytes);

	offset = header->size();
	
	switch(msgtype)
	{

	case _MOBILE_ORIGINATED_EVENT:
	case _MOBILE_ORIGINATED_ACK:
		objcnt = bytes[offset++];
		p->obj_cnt = 0;
		for( i=0; i<objcnt; i++)
		{
			OTA_Object* obj = new_OTA_Object_Bytes(&bytes[offset]);
			offset += obj->size(obj);
			p->add_obj(p, obj);
		}
		break;

	case _MOBILE_TERMINATED_EVENT:
		objcnt = bytes[offset++];
		p->obj_cnt = 0;
		for(i=0; i<objcnt; i++)
		{
			p->add_objid(p, bytes[offset++]);
		}
		break;

	case _MOBILE_TERMINATED_ACK:
		break;


	case _MOBILE_ORIGINATED_MAILBOX_REQ:
		p->mb_command_count = bytes[offset++];
                if(p->mb_command_count > 0)
		{
			p->mb_command = bytes[offset++];
  			p->mb_route_type = bytes[offset++];
		}
		break;

	case _MOBILE_ORIGINATED_MAILBOX_ACK:
		p->mb_mail_count = bytes[offset++];
		break;

	}

	 crc = p->updateCRC(p);
	 if(crc != bytes[offset])
		 DieWithError("CRC check failed.");

	 return p;


}

int get_Msg_Size(OTA_Message* p) {

   	int size;
	OTA_Object* o;
	MESSAGE_TYPE type;

	if(p == NULL)
		return 0;

	 size = HEADER_SIZE + 1; // header + crc
	 type = p->type(p);
	    switch(type)
	    {
	    case _MOBILE_TERMINATED_ACK:
	    	return size;

	    case _MOBILE_TERMINATED_EVENT:
	    	return size + p->objcnt(p);

	    case _MOBILE_ORIGINATED_EVENT:
	    case _MOBILE_ORIGINATED_ACK:
	    	size ++;
	     	o  = p->objlist.obj;
	        	while( o != NULL)
	        		{
	        			size += o->size(o);
	        			o = o->next;
	        		}
	        break;

	    case _MOBILE_ORIGINATED_MAILBOX_REQ:
		size ++; 
                if(p->mb_command_count != 0)
			size = size + 2;
		break;

	    case _MOBILE_ORIGINATED_MAILBOX_ACK:
		size ++;
		break;
	    }
	    return size;

}




void DieWithError(char *errorMessage) {
#ifndef EMBEDDED
	perror(errorMessage);
	 /*
	  *  exit(1);
	 */
#else
	printf("Error: %s", errorMessage);
#endif


}
