

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __boardrfididl_h__
#define __boardrfididl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___Dboardrfid_FWD_DEFINED__
#define ___Dboardrfid_FWD_DEFINED__
typedef interface _Dboardrfid _Dboardrfid;
#endif 	/* ___Dboardrfid_FWD_DEFINED__ */


#ifndef ___DboardrfidEvents_FWD_DEFINED__
#define ___DboardrfidEvents_FWD_DEFINED__
typedef interface _DboardrfidEvents _DboardrfidEvents;
#endif 	/* ___DboardrfidEvents_FWD_DEFINED__ */


#ifndef __boardrfid_FWD_DEFINED__
#define __boardrfid_FWD_DEFINED__

#ifdef __cplusplus
typedef class boardrfid boardrfid;
#else
typedef struct boardrfid boardrfid;
#endif /* __cplusplus */

#endif 	/* __boardrfid_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __boardrfidLib_LIBRARY_DEFINED__
#define __boardrfidLib_LIBRARY_DEFINED__

/* library boardrfidLib */
/* [control][version][uuid] */ 


EXTERN_C const IID LIBID_boardrfidLib;

#ifndef ___Dboardrfid_DISPINTERFACE_DEFINED__
#define ___Dboardrfid_DISPINTERFACE_DEFINED__

/* dispinterface _Dboardrfid */
/* [uuid] */ 


EXTERN_C const IID DIID__Dboardrfid;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("2E04D3BE-200F-4CA8-9CF9-8A7E5AB7645A")
    _Dboardrfid : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DboardrfidVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _Dboardrfid * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _Dboardrfid * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _Dboardrfid * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _Dboardrfid * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _Dboardrfid * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _Dboardrfid * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _Dboardrfid * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DboardrfidVtbl;

    interface _Dboardrfid
    {
        CONST_VTBL struct _DboardrfidVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _Dboardrfid_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _Dboardrfid_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _Dboardrfid_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _Dboardrfid_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _Dboardrfid_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _Dboardrfid_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _Dboardrfid_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___Dboardrfid_DISPINTERFACE_DEFINED__ */


#ifndef ___DboardrfidEvents_DISPINTERFACE_DEFINED__
#define ___DboardrfidEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DboardrfidEvents */
/* [uuid] */ 


EXTERN_C const IID DIID__DboardrfidEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("65647ACC-BC07-449E-9FBC-A299082EECDF")
    _DboardrfidEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DboardrfidEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DboardrfidEvents * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DboardrfidEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DboardrfidEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DboardrfidEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DboardrfidEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DboardrfidEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DboardrfidEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DboardrfidEventsVtbl;

    interface _DboardrfidEvents
    {
        CONST_VTBL struct _DboardrfidEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DboardrfidEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DboardrfidEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DboardrfidEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DboardrfidEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DboardrfidEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DboardrfidEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DboardrfidEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DboardrfidEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_boardrfid;

#ifdef __cplusplus

class DECLSPEC_UUID("41B20F36-8986-40D4-9D16-E3466B13F386")
boardrfid;
#endif
#endif /* __boardrfidLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


