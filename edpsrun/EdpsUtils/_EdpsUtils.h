

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0361 */
/* at Thu Feb 16 16:25:50 2006
 */
/* Compiler settings for _EdpsUtils.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

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

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef ___EdpsUtils_h__
#define ___EdpsUtils_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IEdpsTimer_FWD_DEFINED__
#define __IEdpsTimer_FWD_DEFINED__
typedef interface IEdpsTimer IEdpsTimer;
#endif 	/* __IEdpsTimer_FWD_DEFINED__ */


#ifndef ___IEdpsTimerEvents_FWD_DEFINED__
#define ___IEdpsTimerEvents_FWD_DEFINED__
typedef interface _IEdpsTimerEvents _IEdpsTimerEvents;
#endif 	/* ___IEdpsTimerEvents_FWD_DEFINED__ */


#ifndef __CEdpsTimer_FWD_DEFINED__
#define __CEdpsTimer_FWD_DEFINED__

#ifdef __cplusplus
typedef class CEdpsTimer CEdpsTimer;
#else
typedef struct CEdpsTimer CEdpsTimer;
#endif /* __cplusplus */

#endif 	/* __CEdpsTimer_FWD_DEFINED__ */


/* header files for imported files */
#include "prsht.h"
#include "mshtml.h"
#include "mshtmhst.h"
#include "exdisp.h"
#include "objsafe.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IEdpsTimer_INTERFACE_DEFINED__
#define __IEdpsTimer_INTERFACE_DEFINED__

/* interface IEdpsTimer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IEdpsTimer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5A1EA625-128E-48A7-A506-9E2824F76798")
    IEdpsTimer : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Interval( 
            /* [retval][out] */ LONG *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Interval( 
            /* [in] */ LONG newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HitsCount( 
            /* [retval][out] */ LONG *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEdpsTimerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEdpsTimer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEdpsTimer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEdpsTimer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEdpsTimer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEdpsTimer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEdpsTimer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEdpsTimer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IEdpsTimer * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IEdpsTimer * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Interval )( 
            IEdpsTimer * This,
            /* [retval][out] */ LONG *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Interval )( 
            IEdpsTimer * This,
            /* [in] */ LONG newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HitsCount )( 
            IEdpsTimer * This,
            /* [retval][out] */ LONG *pVal);
        
        END_INTERFACE
    } IEdpsTimerVtbl;

    interface IEdpsTimer
    {
        CONST_VTBL struct IEdpsTimerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEdpsTimer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEdpsTimer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEdpsTimer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEdpsTimer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEdpsTimer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEdpsTimer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEdpsTimer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEdpsTimer_get_Enabled(This,pVal)	\
    (This)->lpVtbl -> get_Enabled(This,pVal)

#define IEdpsTimer_put_Enabled(This,newVal)	\
    (This)->lpVtbl -> put_Enabled(This,newVal)

#define IEdpsTimer_get_Interval(This,pVal)	\
    (This)->lpVtbl -> get_Interval(This,pVal)

#define IEdpsTimer_put_Interval(This,newVal)	\
    (This)->lpVtbl -> put_Interval(This,newVal)

#define IEdpsTimer_get_HitsCount(This,pVal)	\
    (This)->lpVtbl -> get_HitsCount(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEdpsTimer_get_Enabled_Proxy( 
    IEdpsTimer * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IEdpsTimer_get_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IEdpsTimer_put_Enabled_Proxy( 
    IEdpsTimer * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IEdpsTimer_put_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEdpsTimer_get_Interval_Proxy( 
    IEdpsTimer * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IEdpsTimer_get_Interval_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IEdpsTimer_put_Interval_Proxy( 
    IEdpsTimer * This,
    /* [in] */ LONG newVal);


void __RPC_STUB IEdpsTimer_put_Interval_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEdpsTimer_get_HitsCount_Proxy( 
    IEdpsTimer * This,
    /* [retval][out] */ LONG *pVal);


void __RPC_STUB IEdpsTimer_get_HitsCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEdpsTimer_INTERFACE_DEFINED__ */



#ifndef __EdpsUtils_LIBRARY_DEFINED__
#define __EdpsUtils_LIBRARY_DEFINED__

/* library EdpsUtils */
/* [helpstring][uuid][version] */ 


EXTERN_C const IID LIBID_EdpsUtils;

#ifndef ___IEdpsTimerEvents_DISPINTERFACE_DEFINED__
#define ___IEdpsTimerEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IEdpsTimerEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IEdpsTimerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("B12F0FB5-245A-415A-844E-38B0B6EF6164")
    _IEdpsTimerEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IEdpsTimerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IEdpsTimerEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IEdpsTimerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IEdpsTimerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IEdpsTimerEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IEdpsTimerEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IEdpsTimerEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IEdpsTimerEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IEdpsTimerEventsVtbl;

    interface _IEdpsTimerEvents
    {
        CONST_VTBL struct _IEdpsTimerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IEdpsTimerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IEdpsTimerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IEdpsTimerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IEdpsTimerEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IEdpsTimerEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IEdpsTimerEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IEdpsTimerEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IEdpsTimerEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CEdpsTimer;

#ifdef __cplusplus

class DECLSPEC_UUID("E08597E3-A6D1-4549-BD7C-5EDBAEF3094C")
CEdpsTimer;
#endif
#endif /* __EdpsUtils_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


