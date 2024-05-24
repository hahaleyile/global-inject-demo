

/* this ALWAYS GENERATED file contains the RPC client stubs */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for .\print.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#if defined(_M_AMD64)


#pragma warning( disable: 4049 )  /* more than 64k source lines */
#if _MSC_VER >= 1200
#pragma warning(push)
#endif

#pragma warning( disable: 4211 )  /* redefine extern to static */
#pragma warning( disable: 4232 )  /* dllimport identity*/
#pragma warning( disable: 4024 )  /* array to pointer mapping*/

#include <string.h>

#include "print.h"

#define TYPE_FORMAT_STRING_SIZE   7                                 
#define PROC_FORMAT_STRING_SIZE   37                                
#define EXPR_FORMAT_STRING_SIZE   1                                 
#define TRANSMIT_AS_TABLE_SIZE    0            
#define WIRE_MARSHAL_TABLE_SIZE   0            

typedef struct _print_MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } print_MIDL_TYPE_FORMAT_STRING;

typedef struct _print_MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } print_MIDL_PROC_FORMAT_STRING;

typedef struct _print_MIDL_EXPR_FORMAT_STRING
    {
    long          Pad;
    unsigned char  Format[ EXPR_FORMAT_STRING_SIZE ];
    } print_MIDL_EXPR_FORMAT_STRING;


static const RPC_SYNTAX_IDENTIFIER  _RpcTransferSyntax = 
{{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}};


extern const print_MIDL_TYPE_FORMAT_STRING print__MIDL_TypeFormatString;
extern const print_MIDL_PROC_FORMAT_STRING print__MIDL_ProcFormatString;
extern const print_MIDL_EXPR_FORMAT_STRING print__MIDL_ExprFormatString;

#define GENERIC_BINDING_TABLE_SIZE   0            


/* Standard interface: print, ver. 1.0,
   GUID={0xebdcafb3,0x96aa,0x4c04,{0x9b,0xba,0x45,0x1c,0x29,0xec,0x14,0xf4}} */



static const RPC_CLIENT_INTERFACE print___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0xebdcafb3,0x96aa,0x4c04,{0x9b,0xba,0x45,0x1c,0x29,0xec,0x14,0xf4}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0x00000000
    };
RPC_IF_HANDLE print_v1_0_c_ifspec = (RPC_IF_HANDLE)& print___RpcClientInterface;

extern const MIDL_STUB_DESC print_StubDesc;

static RPC_BINDING_HANDLE print__MIDL_AutoBindHandle;


/* [async] */ void  PrintFile( 
    /* [in] */ PRPC_ASYNC_STATE PrintFile_AsyncHandle,
    /* [in] */ handle_t hBinding,
    /* [string][in] */ wchar_t *content)
{

    NdrAsyncClientCall(
                      ( PMIDL_STUB_DESC  )&print_StubDesc,
                      (PFORMAT_STRING) &print__MIDL_ProcFormatString.Format[0],
                      PrintFile_AsyncHandle,
                      hBinding,
                      content);
    
}


#if !defined(__RPC_WIN64__)
#error  Invalid build platform for this stub.
#endif

static const print_MIDL_PROC_FORMAT_STRING print__MIDL_ProcFormatString =
    {
        0,
        {

	/* Procedure PrintFile */

			0x0,		/* 0 */
			0x48,		/* Old Flags:  */
/*  2 */	NdrFcLong( 0x0 ),	/* 0 */
/*  6 */	NdrFcShort( 0x0 ),	/* 0 */
/*  8 */	NdrFcShort( 0x18 ),	/* X64 Stack size/offset = 24 */
/* 10 */	0x32,		/* FC_BIND_PRIMITIVE */
			0x0,		/* 0 */
/* 12 */	NdrFcShort( 0x8 ),	/* X64 Stack size/offset = 8 */
/* 14 */	NdrFcShort( 0x0 ),	/* 0 */
/* 16 */	NdrFcShort( 0x0 ),	/* 0 */
/* 18 */	0xc2,		/* Oi2 Flags:  clt must size, has ext, has async handle */
			0x1,		/* 1 */
/* 20 */	0xa,		/* 10 */
			0x1,		/* Ext Flags:  new corr desc, */
/* 22 */	NdrFcShort( 0x0 ),	/* 0 */
/* 24 */	NdrFcShort( 0x0 ),	/* 0 */
/* 26 */	NdrFcShort( 0x0 ),	/* 0 */
/* 28 */	NdrFcShort( 0x0 ),	/* 0 */

	/* Parameter content */

/* 30 */	NdrFcShort( 0x10b ),	/* Flags:  must size, must free, in, simple ref, */
/* 32 */	NdrFcShort( 0x10 ),	/* X64 Stack size/offset = 16 */
/* 34 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */

			0x0
        }
    };

static const print_MIDL_TYPE_FORMAT_STRING print__MIDL_TypeFormatString =
    {
        0,
        {
			NdrFcShort( 0x0 ),	/* 0 */
/*  2 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  4 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

static const unsigned short print_FormatStringOffsetTable[] =
    {
    0
    };


static const MIDL_STUB_DESC print_StubDesc = 
    {
    (void *)& print___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &print__MIDL_AutoBindHandle,
    0,
    0,
    0,
    0,
    print__MIDL_TypeFormatString.Format,
    1, /* -error bounds_check flag */
    0x50002, /* Ndr library version */
    0,
    0x801026e, /* MIDL Version 8.1.622 */
    0,
    0,
    0,  /* notify & notify_flag routine table */
    0x1, /* MIDL flag */
    0, /* cs routines */
    0,   /* proxy/server info */
    0
    };
#if _MSC_VER >= 1200
#pragma warning(pop)
#endif


#endif /* defined(_M_AMD64)*/

