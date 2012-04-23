#ifndef OTA_MESSAGE_EMBEDDED_H
#define OTA_MESSAGE_EMBEDDED_H

#include <string.h>

#ifndef PC_HOST
#ifdef EMBEDDED

#ifndef C8051
typedef unsigned long long time_t;
#else
typedef unsigned int time_t;
#endif
//#define SELF_MALLOC

#ifdef SELF_MALLOC
#define MAX_PSEUDO_ALLOC_SIZE		0x0C00		//2k bytes -- wow
#define MAX_OTA_PACKED_OBJECT_SIZE	0x0200
#define MAX_OBJ_SIZE				0x0080

extern unsigned char pseudo_malloc_buf[MAX_PSEUDO_ALLOC_SIZE];
extern unsigned char* pseudo_malloc_ptr;
void* malloc(size_t size);
void free(void* to_free);
#endif
/*
// ___________________________________________________________
//	Below are the functioned provided for an embedded system
//	that the OS typically provides.	 Set the EMBEDDED define
//  accordingly
// ___________________________________________________________
*/

time_t time(time_t* t);
size_t recv(int socketfd, void* data, size_t length, int flags);
size_t send(int socket, const void *buffer, size_t length, int flags);



#endif //end EMBEDDED

#endif // end OTA_MESSAGE_EMBEDDED_H
#endif //PC_HOST
