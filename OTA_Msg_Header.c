/*
 * OTA_Msg_Header.c
 *
 *  Created on: Aug 30, 2011
 *      Author: wlai
 */


#include <string.h>
#include <OTA_Def.h>
#ifndef EMBEDDED
#include <sys/socket.h>
#endif

BYTEARRAY* Header_getBytes(Msg_Header* h);
void print_header(Msg_Header* header);
void header_set_eventcode(Msg_Header* header, BYTE event_code);
BYTE header_get_eventcode(Msg_Header* header);
void header_set_seqid(Msg_Header* header, UInt16 seqid);
UInt16 header_get_seqid(Msg_Header* header);
void header_set_time(Msg_Header* header, ULong timestamp);
ULong header_get_time(Msg_Header* header);
int header_send(Msg_Header*, int, int);
int header_size();
Msg_Header*  new_OTA_MSG_Header( Msg_Header* p, BYTE type, BYTE event_code, UInt16 seq_id);
Msg_Header* createHeaderFromBytes( Msg_Header* header, BYTE* bytes);




void Header_set_type	(Msg_Header* p, MESSAGE_TYPE e)
{
	p->type = e;
}

MESSAGE_TYPE Header_get_type (Msg_Header* p)
{
	return (MESSAGE_TYPE) p->type;

}


int header_size()
{
	return HEADER_SIZE;

}

Msg_Header* createHeaderFromBytes( Msg_Header* header, BYTE* bytes)
{
	
	int offset = 0;
	BYTE type;
	BYTE version;
	BYTE event_code;
	UInt16 seqid = 0;
	ULong time = 0;

	if(bytes == NULL)
		return NULL;
	
	type = bytes[offset++];
	version = bytes[offset++];
	event_code = bytes[offset++];

	memcpy(&seqid, &(bytes[offset]), 2);
	offset += 2;
	reverseBytes((BYTE*)&seqid, 2);

	if(header == NULL)
	    header = new_OTA_MSG_Header(NULL, type, event_code, seqid);
	else
	{
		header->type = type;
		header->event_code = event_code;
		header->seq_id = seqid;
	}

	
	memcpy(&time, &(bytes[offset]), sizeof(ULong));
	reverseBytes((BYTE*)&time, sizeof(time));
	header->setTime(header, time);
	return header;

}

Msg_Header*  new_OTA_MSG_Header(Msg_Header* p, BYTE type, BYTE event_code, UInt16 seq_id)
{

	time_t now;
	
	if(p == NULL)
		p = malloc (sizeof(Msg_Header));

	p->type = type;
	p->event_code = event_code;
	p->seq_id = seq_id;

    p->version = _MAJOR_VERSION | _MINOR_VERSION;



	time(&now);
    p->timestamp = (ULong) now * 1000; // milliseconds.

    p->getBytes = Header_getBytes;
    p->print = print_header;

    p->set_Seq_ID = header_set_seqid;
    p->get_Seq_ID = header_get_seqid;

    p->set_Event_Code = header_set_eventcode;
    p->get_Event_Code =  header_get_eventcode;
    p->send	= header_send;

    p->setTime = header_set_time;
    p->getTime = header_get_time;
    p->size = header_size;
    p->set_type = Header_set_type;
    p->get_type = Header_get_type;
    return p;
}

int header_send(Msg_Header* h, int socketfd , int flags)
{
	BYTEARRAY* b = h->getBytes(h);
	int sentbytes = send(socketfd, b->data, b->size, flags);

	if(sentbytes != b->size)
	{
		perror("Failed to send header....");
	}
	b->Delete(b);
	return sentbytes;
}
void header_set_eventcode(Msg_Header* header, BYTE event_code)
{
	header->event_code = event_code;
}
BYTE header_get_eventcode(Msg_Header* header)
{
	return header->event_code;
}
void header_set_seqid(Msg_Header* header, UInt16 seqid )
{
	header->seq_id = seqid;
}

UInt16 header_get_seqid(Msg_Header* header)
{
	return header->seq_id;
}

void header_set_time(Msg_Header* header, ULong timestamp)
{
	if(timestamp == 0)
		timestamp = time((time_t*)&timestamp);
	header->timestamp = timestamp;
}
ULong header_get_time(Msg_Header* header)
{
	return header->timestamp;
}

void print_header(Msg_Header* header)
{
	int major = header->version >> 4;
    int minor = header->version & 0x0F;
	ULong timestamp;

	printf("\t<HEADER>\n");
	printf("\t\t<TYPE = ");
	switch(header->type)
	{
	case MOBILE_TERMINATED_EVENT:
		printf("MOBILE_TERMINATED_EVENT");
		break;
	case MOBILE_ORIGINATED_EVENT:
		printf("MOBILE_ORIGINATED_EVENT");
		break;
	case MOBILE_ORIGINATED_ACK:
		printf("MOBILE_ORIGINATED_ACK");
		break;
	case MOBILE_TERMINATED_ACK:
		printf("MOBILE_TERMINATED_ACK");
		break;
	default:
		printf("???");
		break;

	}

    printf(">\n");

   
    printf("\t\t<VERSION=%d.%d/>\n", major, minor);
    printf("\t\t<EVEN CODE=%d/>\n", header->event_code);
    printf("\t\t<SEQ ID=%d/>\n", header->seq_id);
    timestamp = (header->timestamp) / 1000;
#ifndef EMBEDDED
    printf("\t\t<TIMESTAMP=%s", ctime((time_t)&timestamp));
#else
	printf("\t\t<TIMESTAMP=%d", timestamp);
#endif
    printf("\t\t</TIMESTAMP>\n");
    printf("\t</HEADER>\n");
}

BYTEARRAY* Header_getBytes(Msg_Header* h)
{

	BYTEARRAY* p = new_BYTEARRAY(HEADER_SIZE);
	UInt16 seqid;
	int offset = 0;
	ULong timestamp;

	p->data[offset++] = h->type;
	p->data[offset++] = h->version;
	p->data[offset++] = h->event_code;
	seqid = h->seq_id;
	reverseBytes((BYTE*)&seqid, sizeof(UInt16));
	memcpy(&(p->data[offset]), &seqid, sizeof(UInt16));
	offset += sizeof(UInt16);
	timestamp = h->timestamp;
	reverseBytes((BYTE*)&timestamp, sizeof(ULong));
	memcpy(&(p->data[offset]), &timestamp, sizeof(ULong));
	return p;
}


