

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed Jul 22 09:27:39 2015
 */
/* Compiler settings for boardrfid.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_boardrfidLib,0x256872EE,0x2BDC,0x43A9,0xA1,0xAB,0xAD,0x66,0xE2,0xE2,0x41,0x7C);


MIDL_DEFINE_GUID(IID, DIID__Dboardrfid,0x2E04D3BE,0x200F,0x4CA8,0x9C,0xF9,0x8A,0x7E,0x5A,0xB7,0x64,0x5A);


MIDL_DEFINE_GUID(IID, DIID__DboardrfidEvents,0x65647ACC,0xBC07,0x449E,0x9F,0xBC,0xA2,0x99,0x08,0x2E,0xEC,0xDF);


MIDL_DEFINE_GUID(CLSID, CLSID_boardrfid,0x41B20F36,0x8986,0x40D4,0x9D,0x16,0xE3,0x46,0x6B,0x13,0xF3,0x86);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



