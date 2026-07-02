/****************************************************************************/
/*     Copyright (C) Siemens AG 1995  All Rights Reserved.                  */
/****************************************************************************/
/*                                                                          */
/*  version: @(#1)SAPI_S7.H    V 1.00                                       */
/*                                                                          */
/*  This header file contains all defines, typedefinitions and prototypings */
/*  for using the S7 library of Siemens AG. It is the only file that must   */
/*  be included in all source files of user applications that use S7        */
/*  calls.                                                                  */
/*                                                                          */
/*  For standard calling convention use define S732STD_DLL.                 */
/*                                                                          */
/****************************************************************************/

#ifndef SAPI_S7_H
#define SAPI_S7_H

/*
+-------------------------------------------------------------------+
| returncodes for S7 calls                                          |
+-------------------------------------------------------------------+
*/

/******* Important messages *****************************/

#define S7_OK                                0
#define S7_ERR_RETRY                      (-1)
#define S7_ERR                            (-2)

#define S7_NO_MSG                            0
#define S7_UNKNOWN_MSG                       1

/*
+-------------------------------------------------------------------+
| IEC errors                                                        |
+-------------------------------------------------------------------+
*/

#define S7_ERR_IEC_NO                        0
#define S7_ERR_IEC_LOWER_LAYER               1
#define S7_ERR_IEC_NEG_RESPONSE              2
#define S7_ERR_IEC_INVALID_REF               3
#define S7_ERR_IEC_DATA_TYPE_MISMATCH        4
#define S7_ERR_IEC_RESET_RECEIVED            5
#define S7_ERR_IEC_RECEIVER_DISABLED         6
#define S7_ERR_IEC_PARTNER_IN_WRONG_STATE    7
#define S7_ERR_IEC_NO_ACCESS_TO_REM_OBJECT   8
#define S7_ERR_IEC_RECEIVER_OVERRUN          9

/*
+-------------------------------------------------------------------+
| detailed errors                                                   |
+-------------------------------------------------------------------+
*/


/******** general errors *******************************************/

#define S7_ERR_NO_ERROR                      0
#define S7_ERR_UNKNOWN_ERROR                 1
#define S7_ERR_WRONG_CP_DESCR                2
#define S7_ERR_NO_RESOURCE                   3
#define S7_ERR_INVALID_PARAMETER             7
#define S7_ERR_TOO_LONG_DATA                 8
#define S7_ERR_TOO_MANY_DLL_USERS            9

/******** invalid service ******************************************/

#define S7_ERR_WRONG_IND_CNF                10
#define S7_ERR_SERVICE_NOT_SUPPORTED        11


/******** invalid communication reference (cref) or connection name */

#define S7_ERR_INVALID_CREF                 20
#define S7_ERR_CONN_NAME_NOT_FOUND          23
#define S7_ERR_NO_LDB_XDB_FILE              25

/******** invalid order_id    **************************************/

#define S7_ERR_INVALID_ORDERID              30
#define S7_ERR_ORDERID_USED                 31


/******** invalid r_id    ******************************************/

#define S7_ERR_INVALID_R_ID                 40
#define S7_ERR_R_ID_USED                    41
#define S7_ERR_NO_R_ID						42


/******** invalid object *******************************************/

#define S7_ERR_OBJ_UNDEFINED                50
#define S7_ERR_OBJ_ATTR_INCONSISTENT        51
#define S7_ERR_OBJ_ACCESS_DENIED            53

/******** invalid data transfer ************************************/

#define S7_ERR_INVALID_DATA_SIZE            80
#define S7_ERR_RECEIVE_BUFFER_FULL          81
#define S7_ERR_INVALID_DATARANGE_OR_TYPE    82
#define S7_ERR_INVALID_SEGMENT			    83
#define S7_ERR_INVALID_WRITE_BUFFER         84
#define S7_ERR_INVALID_FILENAME             85
#define S7_ERR_FILE_NOT_FOUND               86
#define S7_ERR_INVALID_FILETYPE             87


/******** controller or CMI or FW problems *************************/

#define S7_ERR_FW_ERROR                     90

/******** s7_mini_db_set() / s7_mini_db_get() errors ***************/

#define S7_ERR_MINI_DB_TYPE                 100
#define S7_ERR_MINI_DB_VALUE                101

/******** multi user errors ****************************************/

#define S7_ERR_SERVICE_VFD_ALREADY_USED     112
#define S7_ERR_SERVICE_CONN_ALREADY_USED    113

/******** connection handling **************************************/

#define S7_ERR_CONN_ABORTED                 120
#define S7_ERR_INVALID_CONN_STATE           121
#define S7_ERR_MAX_REQ                      122
#define S7_ERR_CONN_CNF                     123

/******** state of cyclic read *************************************/

#define S7_ERR_INVALID_CYCL_READ_STATE      130

/******** SINEC problems *******************************************/

#define S7_ERR_INSTALL                      140
#define S7_ERR_INTERNAL_ERROR               141
#define S7_ERR_NO_SIN_SERV                  142
#define S7_ERR_NO_LICENCE                   143

/******** symbolic address problems ********************************/

#define S7_ERR_SYMB_ADDRESS                 150
#define S7_ERR_SYMB_ADDRESS_INCONSISTENT    151

/******** remote block problems ************************************/

#define S7_ERR_REM_BRCV                     160
#define S7_ERR_REM_BSEND                    161
#define S7_ERR_REM_BSEND_CANCEL             162
#define S7_ERR_REM_DATABASE_TOO_SMALL       163

/******** pmc problems ********************************************/

#define S7_ERR_NO_RCV_BLOCK                 170
#define S7_ERR_ADD_VALUE_PDU                171

/******** domain errors *******************************************/

#define S7_ERR_FEW_PROT_LEVEL       180
#define S7_ERR_INVALID_BLOCK        272 
#define S7_ERR_INVALID_BLOCK_TYPE   275
#define S7_ERR_BLOCK_NOT_FOUND      276 
#define S7_ERR_BLOCK_ALREADY_EXISTS	277
#define S7_ERR_BLOCK_PROTECTED 		278
#define S7_ERR_BLOCK_TOO_LARGE 		279
#define S7_ERR_INVALID_BLOCK_NR     280 
#define S7_ERR_NOT_ENOUGH_AS_RAM    281
#define S7_ERR_COORD_RULE           282
#define S7_ERR_INVALID_PASSWORD     290   
#define S7_ERR_CONN_ALREADY_PASSED  291
#define S7_ERR_CONN_ALREADY_FREE    292
#define S7_ERR_NO_PASSWORD          293


/*
+-------------------------------------------------------------------+
| defines for s7_mini_db_set, s7_mini_db_get                        |
+-------------------------------------------------------------------+
*/

/* for trace target (type and values) */
#define S7_MINI_DB_TRACE_TARGET             0        /* type  */
#define S7_TRACE_TARGET_BUFFER             "0"       /* value */
#define S7_TRACE_TARGET_NEW_FILE           "1"       /* value */
#define S7_TRACE_TARGET_OLD_FILE           "2"       /* value */
#define S7_TRACE_TARGET_CONSOLE            "3"       /* value */

/* for trace select (type and values) */
#define S7_MINI_DB_TRACE_SELECT             1        /* type  */
#define S7_TRACE_SELECT_ADMIN_SERVICES     "1"       /* value */
#define S7_TRACE_SELECT_CONN_SERVICES      "2"       /* value */
#define S7_TRACE_SELECT_CRL_SERVICES       "4"       /* value */
#define S7_TRACE_SELECT_OD_SERVICES        "8"       /* value */
#define S7_TRACE_SELECT_VAR_SERVICES       "16"      /* value */
#define S7_TRACE_SELECT_VFD_SERVICES       "32"      /* value */
#define S7_TRACE_SELECT_DE2_SERVICES       "64"      /* value */
#define S7_TRACE_SELECT_CYCL_VAR_SERVICES  "128"     /* value */
#define S7_TRACE_SELECT_RECEIVE_SERVICES   "256"     /* value */
#define S7_TRACE_SELECT_OTHER_SERVICES     "512"     /* value */
#define S7_TRACE_SELECT_PBK_SERVICES       "1024"    /* value */
#define S7_TRACE_SELECT_PMC_SERVICES       "2048"    /* value */
#define S7_TRACE_SELECT_DOM_SERVICES       "4096"    /* value */
#define S7_TRACE_SELECT_ALL                "65535"   /* value */

/* for trace depth (type and values) */
#define S7_MINI_DB_TRACE_DEPTH              2        /* type  */
#define S7_TRACE_DEPTH_OFF                 "0"       /* value */
#define S7_TRACE_DEPTH_USER                "1"       /* range "1" to "99"  */
                                                     /* can be used by     */
                                                     /* user               */
#define S7_TRACE_DEPTH_EXCEPT              "100"     /* value */
#define S7_TRACE_DEPTH_INTERFACE           "101"     /* value */
#define S7_TRACE_DEPTH_OTHER               "102"     /* value */
#define S7_TRACE_DEPTH_LIB0                "103"     /* value for internal */
                                                     /* use only           */
#define S7_TRACE_DEPTH_LIB0_PDU            "104"     /* value for internal */
                                                     /* use only           */
#define S7_TRACE_DEPTH_S7_BUF              S7_TRACE_DEPTH_LIB0
#define S7_TRACE_DEPTH_SCP_BUF             S7_TRACE_DEPTH_LIB0_PDU

/* for trace maxFiles and maxLines */
#define S7_MINI_DB_TRACE_MAXFILES           99       /* type */
#define S7_MINI_DB_TRACE_MAXLINES           32768    /* type */

/* for trace filename (type only) */
#define S7_MINI_DB_TRACE_FILENAME           3        /* type  */

/* for number of trace lines (type only) */
#define S7_MINI_DB_TRACE_NO_LINES           4        /* type  */

/* for initiate request (type only) */
#define S7_MINI_DB_INIT_REQ_AMQ_CALLING    10    /* only set value */
#define S7_MINI_DB_INIT_REQ_AMQ_CALLED     11    /* only set value */
#define S7_MINI_DB_INIT_REQ_PDU_SIZE       12    /* only set value */

/* for initiate response (type only) */
#define S7_MINI_DB_INIT_RSP_AMQ_CALLING    20    /* only set value */
#define S7_MINI_DB_INIT_RSP_AMQ_CALLED     21    /* only set value */
#define S7_MINI_DB_INIT_RSP_PDU_SIZE       22    /* only set value */

/* for initiate indication (type only) */
#define S7_MINI_DB_INIT_IND_AMQ_CALLING    50    /* only get value */
#define S7_MINI_DB_INIT_IND_AMQ_CALLED     51    /* only get value */
#define S7_MINI_DB_INIT_IND_PDU_SIZE       52    /* only get value */

/* for initiate confirmation (type only) */
#define S7_MINI_DB_INIT_CNF_AMQ_CALLING    30    /* only get value */
#define S7_MINI_DB_INIT_CNF_AMQ_CALLED     31    /* only get value */
#define S7_MINI_DB_INIT_CNF_PDU_SIZE       32    /* only get value */

/* for any message */
#define S7_MINI_DB_RESPONSE_CODE           40    /* only get value */
#define S7_MINI_DB_ERROR_CLASS             41    /* only get value */
#define S7_MINI_DB_ERROR_CODE              42    /* only get value */

/* for connection establishment */
#define S7_MINI_DB_NEGOTIATION_OPTIONS     60    /* only set value */
#define S7_MINI_DB_PERSISTANCE_COUNT       61    /* only set value */
#define S7_MINI_DB_ABORT_TIMEOUT           62    /* only set value */

/* for reading library version (type only) */
#define S7_MINI_DB_VERSION                 70    /* only get value */

/*
+-------------------------------------------------------------------+
| defines for diagnosis                                             |
+-------------------------------------------------------------------+
*/

// values for state of connection if pbl_id = STD_TYPE
#define S7_DIAG_STD_DOWN				0x11	/* disconnected by intention */
#define S7_DIAG_STD_ABORT				0x12	/* disconnected by failure   */
#define S7_DIAG_STD_NOT_USED			0x13	/* was never established     */
#define S7_DIAG_STD_OK					0x14	

// values for state of connection if pbl_id = H_TYPE 
#define S7_DIAG_H_OK_RED				0x20	/* established and redundant            */
#define S7_DIAG_H_OK_RED_PATH_CHG		0x21	/* est. and redundancy channel switched */
#define S7_DIAG_H_OK_NOT_RED			0x22	/* established and not redundant        */ 
#define S7_DIAG_H_ABORT					0x23	/* RED disconnected by failure          */
#define S7_DIAG_H_NOT_USED				0x24	/* was never established                */
#define S7_DIAG_H_DOWN					0x25	/* established                          */

// values for state of ways if pbl_id = H_TYPE 
#define S7_DIAG_HW_PROD					0x30	/* this way is active connection    */
#define S7_DIAG_HW_STBY					0x31	/* this way is standby connection   */
#define S7_DIAG_HW_ABORT				0x32	/* doesn't work, see err code       */
#define S7_DIAG_HW_NOT_USED				0x33	/* was never established            */
#define S7_DIAG_HW_DOWN					0x34	/* disconnected by intention        */
#define S7_DIAG_HW_CN_BREAK				0x35	/* this way couldn't be established */

// PBL_ID definitions (only for diagnosis)
#define S7D_PC_HEADER_CON				1
#define S7D_STD_TYPE					2
#define S7D_H_TYPE						3
#define S7D_WAY_INFO_INDEX				4
#define S7D_DIAG_CONN_INFO				5

// maximum number of ways per connection
#define S7_MAX_WAYS						4

// SCAN Alarm messages definitions
#define S7_SCAN_ALL_ABORT				0x0000    /* abort code SCAN functions       */
#define S7_SCAN_ALL_INITIATE			0x0001    /* initiate code SCAN functions    */
#define S7_ALARM_ALL_ABORT				0x0004    /* abort code ALARM functions      */
#define S7_ALARM_ALL_INITIATE			0x0005    /* initiate code ALARM functions   */


/* values for the parameter state of S7_SCAN_OBJECT structure */

#define S7_SCAN_MSG_EXIST               0x0
#define S7_SCAN_NO_MSG                  0x81


/*
+-------------------------------------------------------------------+
| general defines                                                   |
+-------------------------------------------------------------------+
*/

/* max. length of names */
#define S7_MAX_NAMLEN                    32
#define S7_MAX_DEVICELEN                128

/* max. length of address */                  
#define S7_MAX_ADDRESSLEN         32

/* max. length of buffer */
#define S7_MAX_BUFLEN             256
#define S7_MAX_BUFLEN_LONG        932
#define S7_MAX_BUFLEN_MULTIPLE    256 
#define S7_MAX_BSEND_BRCV_DATA    65534

/* max. message additional values */
#define S7_MAX_ADD_VALUES               10
#define S7_MAX_SCAN_ADD_VALUE_LEN        4
#define S7_MAX_ALARM_ADD_VALUE_LEN     432

/* max. SCAN objects */
#define S7_MAX_SCAN_OBJECT				80

/* defines for selection of data base file */
#define S7_NO_FILE                   0
#define S7_LDB_FILE                  1
#define S7_XDB_FILE                  2

/* defines for access */
#define S7_ACCESS_NAME                   1
#define S7_ACCESS_SYMB_ADDRESS           2
#define S7_ACCESS_ADDRESS                3

/* defines for storage_mode */
#define S7_MEMORY                        0
#define S7_FILE                          1

/* defines for logical states */
#define S7_STATE_CHANGES_ALLOWED         0

/* defines for physical states */
#define S7_OPERATIONAL                   0x10
#define S7_NEEDS_COMMISSIONING           0x13

/* defines for initiate_rsp */
#define S7_ACCEPT                        1
#define S7_NON_ACCEPT                    0

/* defines for result */
#define S7_RESULT_OK                     0x00ff
#define S7_RESULT_HW_ERROR               0x0001
#define S7_RESULT_OBJ_ACCESS_DENIED      0x0003
#define S7_RESULT_OBJ_ADDRESS_INVALID    0x0005
#define S7_RESULT_OBJ_TYPE_NOT_SUPPORTED 0x0006
#define S7_RESULT_OBJ_TYPE_INCONSISTENT  0x0007
#define S7_RESULT_OBJ_NOT_EXISTS         0x000a

/* defines for block list maximum */
#define S7_MAX_BLOCKS                   1

/* defines for block parameter  */
#define S7_BLOCK_PASSIV                 0x00000001
#define S7_BLOCK_LINKED_IN              0x00000040
#define S7_BLOCK_WRITE_UNCONDITIONAL    0x00001000
#define S7_BLOCK_WRITE_CONDITIONAL      0x00000000

/* defines for block types  */
#define S7_BLOCK_OB                     0x0008 
#define S7_BLOCK_DB                     0x000A 
#define S7_BLOCK_SDB                    0x000B 
#define S7_BLOCK_FC                     0x000C 
#define S7_BLOCK_SFC                    0x000D 
#define S7_BLOCK_FB                     0x000E 
#define S7_BLOCK_SFB                    0x000F 

/* defines for data type */
#define S7_DATATYPE_ERROR               0x00
#define S7_DATATYPE_STRUCTURE           0x02
#define S7_DATATYPE_BOOLEAN             0x03
#define S7_DATATYPE_BITSTRING           0x04
#define S7_DATATYPE_INTEGER             0x05
#define S7_DATATYPE_UNSIGNED            0x06
#define S7_DATATYPE_FLOAT               0x07
#define S7_DATATYPE_OCTET_STRING        0x09

#define S7_DATATYPE_DATE                0x30
#define S7_DATATYPE_TIME_OF_DAY         0x31
#define S7_DATATYPE_TIME                0x32
#define S7_DATATYPE_S5TIME              0x33
 
/* values for the parameter data_type of S7_ADD_VALUE structure */
#define S7_DATATYPE_DATE                0x30
#define S7_DATATYPE_TIME_OF_DAY         0x31
#define S7_DATATYPE_TIME                0x32
#define S7_DATATYPE_S5_TIME             0x33

/* defines for password */
#define S7_PASSWORD_LEN      8

#define S7_ENTER_PASSWORD    0x00000001
#define S7_SET_PASSWORD      0x00000001
#define S7_RESET_PASSWORD    0x00000000


/*
+-------------------------------------------------------------------+
| defines for services                                              |
+-------------------------------------------------------------------+
*/

/* initiate, abort */
#define  S7_INITIATE_CNF                      0x0101 
#define  S7_INITIATE_IND                      0x0102
#define  S7_ABORT_IND                         0x0103
#define  S7_AWAIT_INITIATE_CNF                0x0104 

/* VFD */
#define  S7_VFD_STATE_CNF                     0x0201
#define  S7_VFD_USTATE_IND                    0x0202

/* variables */
#define  S7_READ_CNF                          0x0301
#define  S7_WRITE_CNF                         0x0302
#define  S7_MULTIPLE_READ_CNF                 0x0303
#define  S7_MULTIPLE_WRITE_CNF                0x0304

/* cyclic variables */
#define  S7_CYCL_READ_ABORT_IND               0x0401
#define  S7_CYCL_READ_DELETE_CNF              0x0402
#define  S7_CYCL_READ_IND                     0x0403
#define  S7_CYCL_READ_INIT_CNF                0x0404
#define  S7_CYCL_READ_START_CNF               0x0405
#define  S7_CYCL_READ_STOP_CNF                0x0406

/* PBK variables */
#define  S7_BSEND_CNF							0x0601
#define  S7_BRCV_IND							0x0602
#define  S7_BRCV_CANCEL_IND						0x0603
#define  S7_BRCV_STOP							0x0604

/* DIAG variables */
#define  S7_DIAG_IND							0x0701

/* Domain variables */
#define  S7_BLOCK_READ_CNF		                0x0801
#define  S7_BLOCK_WRITE_CNF		                0x0802
#define  S7_BLOCK_DELETE_CNF	                0x0803
#define  S7_BLOCK_LINK_IN_CNF	                0x0804
#define  S7_BLOCK_COMPRESS_CNF	                0x0805
#define  S7_PASSWORD_CNF                        0x0806


/* PMC variables */
#define  S7_MSG_INITIATE_CNF	    			0x4401
#define  S7_MSG_ABORT_CNF			    		0x4402
#define  S7_SCAN_IND							0x4403
#define  S7_ALARM_IND							0x4404


/*
+-------------------------------------------------------------------+
| typedefinitions                                                   |
+-------------------------------------------------------------------+
*/

/* The following typedefinitions are necessary for using S7 library.  */
/* Including other header files in user applications using the same   */
/* typedefinitions results in compiler errors or warnings. To avoid   */
/* these warnings or errors the basic typedefinitions of the S7       */
/* library can be switched off.                                       */

#ifndef S7_BASIC_TYPES

/* basic types */
typedef char int8;                  /* signed char                 */
typedef unsigned char ord8;         /* unsigned char               */
typedef short int int16;            /* signed short integer        */
typedef unsigned short int ord16;   /* unsigned short integer      */
typedef long int32;                 /* signed long integer         */
typedef unsigned long ord32;        /* unsigned long integer       */

#endif

struct S7_READ_PARA
{
   ord16 access;
   char  var_name[S7_MAX_NAMLEN+2];
   ord16 index;
   ord16 subindex;
   ord16 address_len;
   ord8  address[S7_MAX_ADDRESSLEN];
};

struct S7_WRITE_PARA
{
   ord16 access;
   char  var_name[S7_MAX_NAMLEN+2];
   ord16 index;
   ord16 subindex;
   ord16 address_len;
   ord8  address[S7_MAX_ADDRESSLEN];
   ord16 var_length;
   ord8  value[S7_MAX_BUFLEN];
};

struct S7_WRITE_PARA_LONG 
{
   ord16 access;
   char  var_name[S7_MAX_NAMLEN+2];
   ord16 index;
   ord16 subindex;
   ord16 address_len;
   ord8  address[S7_MAX_ADDRESSLEN];
   ord16 var_length;
   ord8  value[S7_MAX_BUFLEN_LONG];
};


typedef struct {
    ord16			cref;			// connection ID
    ord16			conn_type;		// connection type
    ord16			conn_state;		// connection state
	ord16			way_state[S7_MAX_WAYS];	// way state
} CONN_INFO;

struct S7_TIME_STAMP
{
	ord16	year;	
	ord16	month;	
	ord16	day;	
	ord16	week_day;
	ord16	hour;	
	ord16	minute;	
	ord16	second;	
	ord16	millisecond;	
};

struct S7_SCAN_OBJECT
{
	ord16	state;	
	ord16	ack_state;	
	ord16	event_state;	
	ord32	event_id;	
	ord16	no_add_value;	
	struct 	
	{		ord16	data_type;				
			ord16	add_value_len;				
			ord8	value[S7_MAX_SCAN_ADD_VALUE_LEN+2];	
	}add_value[S7_MAX_ADD_VALUES];
};

struct  S7_ADD_VALUE				
{	ord16	data_type;				
    ord16	add_value_len;				
    ord8	value[S7_MAX_ALARM_ADD_VALUE_LEN+2];	
};

struct  S7_BLOCK_LIST	 
{	ord16	block_type;	 
	ord16	block_nr;	 
};


/*
+-------------------------------------------------------------------+
| Prototypings for S7 calls                                         |
+-------------------------------------------------------------------+
*/

/* define for using DLL (only for Windows 3.xx) */
#ifdef S732STD_DLL
#define S7_FAR_PASCAL    _stdcall
#define S7_FAR_DATA         
#else
#ifdef S7_DLL
#define S7_FAR_PASCAL    far pascal
#define S7_FAR_DATA      far
#else
#define S7_FAR_PASCAL
#define S7_FAR_DATA
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

int32 S7_FAR_PASCAL s7_abort(ord32 cp_descr,ord16 cref);
int32 S7_FAR_PASCAL s7_await_initiate_req(ord32 cp_descr,ord16 cref);
int32 S7_FAR_PASCAL s7_cycl_read(ord32 cp_descr,ord16 cref,ord16 orderid,
                                 ord16 cycl_time,ord16 number,
                                 struct S7_READ_PARA 
                                        S7_FAR_DATA *read_para_array);
int32 S7_FAR_PASCAL s7_cycl_read_delete_req(ord32 cp_descr,ord16 cref,
                                            ord16 orderid);
int32 S7_FAR_PASCAL s7_cycl_read_init_req(ord32 cp_descr,ord16 cref,
                                          ord16 orderid,ord16 cycl_time,
                                          ord16 number,
                                          struct S7_READ_PARA 
                                                 S7_FAR_DATA *read_para_array);
int32 S7_FAR_PASCAL s7_cycl_read_start_req(ord32 cp_descr,ord16 cref,
                                           ord16 orderid);
int32 S7_FAR_PASCAL s7_cycl_read_stop_req(ord32 cp_descr,ord16 cref,
                                          ord16 orderid);
void  S7_FAR_PASCAL s7_discard_msg(void);
int32 S7_FAR_PASCAL s7_get_abort_ind(void);
int32 S7_FAR_PASCAL s7_get_await_initiate_cnf(void);
int32 S7_FAR_PASCAL s7_get_conn(ord32 cp_descr,ord16 index,
                                ord16 S7_FAR_DATA *number_ptr,
                                ord16 S7_FAR_DATA *cref_ptr,
                                char S7_FAR_DATA *conn_name);
int32 S7_FAR_PASCAL s7_get_cref(ord32 cp_descr,
                                char S7_FAR_DATA *conn_name,
                                ord16 S7_FAR_DATA *cref_ptr);
int32 S7_FAR_PASCAL s7_get_cycl_read_abort_ind(void);
int32 S7_FAR_PASCAL s7_get_cycl_read_delete_cnf(void);
int32 S7_FAR_PASCAL s7_get_cycl_read_ind(void S7_FAR_DATA *od_ptr,
                                         ord16 S7_FAR_DATA *result_array,
                                         ord16 S7_FAR_DATA *var_length_array,
                                         void S7_FAR_DATA *value_array);
int32 S7_FAR_PASCAL s7_get_cycl_read_init_cnf(void);
int32 S7_FAR_PASCAL s7_get_cycl_read_start_cnf(void);
int32 S7_FAR_PASCAL s7_get_cycl_read_stop_cnf(void);
int32 S7_FAR_PASCAL s7_get_device(ord16 index,ord16 S7_FAR_DATA *number_ptr,
                                  char S7_FAR_DATA *dev_name);
int32 S7_FAR_PASCAL s7_get_initiate_cnf(void);
int32 S7_FAR_PASCAL s7_get_initiate_ind(void);
int32 S7_FAR_PASCAL s7_get_multiple_read_cnf(void S7_FAR_DATA *od_ptr,
                                             ord16 S7_FAR_DATA *result_array,
                                             ord16 S7_FAR_DATA *var_length_array,
                                             void S7_FAR_DATA *value_array);
int32 S7_FAR_PASCAL s7_get_multiple_write_cnf(ord16 S7_FAR_DATA *result_array);
int32 S7_FAR_PASCAL s7_get_read_cnf(void S7_FAR_DATA *od_ptr,
                                    ord16 S7_FAR_DATA *var_length_ptr,
                                    void S7_FAR_DATA *value_ptr);
int32 S7_FAR_PASCAL s7_get_vfd(char S7_FAR_DATA *dev_name,ord16 index,
                               ord16 S7_FAR_DATA *number_ptr,
                               char S7_FAR_DATA *vfd_name);
int32 S7_FAR_PASCAL s7_get_vfd_state_cnf(ord16 S7_FAR_DATA *log_state_ptr,
                                         ord16 S7_FAR_DATA *phy_state_ptr, 
                                         ord8 S7_FAR_DATA *local_detail_ptr);
int32 S7_FAR_PASCAL s7_get_vfd_ustate_ind(ord16 S7_FAR_DATA *log_state_ptr,
                                          ord16 S7_FAR_DATA *phy_state_ptr, 
                                          ord8 S7_FAR_DATA *local_detail_ptr);
int32 S7_FAR_PASCAL s7_get_write_cnf(void);
int32 S7_FAR_PASCAL s7_init(char S7_FAR_DATA *cp_name,
                            char S7_FAR_DATA *vfd_name,
                            ord32 S7_FAR_DATA *cp_descr_ptr);
int32 S7_FAR_PASCAL s7_initiate_req(ord32 cp_descr,ord16 cref);
int32 S7_FAR_PASCAL s7_initiate_rsp(ord32 cp_descr,ord16 cref,ord16 accept);
const char S7_FAR_DATA * S7_FAR_PASCAL s7_last_iec_err_msg(void);
ord16 S7_FAR_PASCAL s7_last_iec_err_no(void);
const char S7_FAR_DATA * S7_FAR_PASCAL s7_last_detailed_err_msg(void);
ord16 S7_FAR_PASCAL s7_last_detailed_err_no(void);
const char S7_FAR_DATA * S7_FAR_PASCAL s7_mini_db_get(ord16 type);
int32 S7_FAR_PASCAL s7_mini_db_set(ord16 type, char S7_FAR_DATA *value);
int32 S7_FAR_PASCAL s7_multiple_read_req(ord32 cp_descr,ord16 cref,
                                         ord16 orderid,ord16 number,
                                         struct S7_READ_PARA 
                                                S7_FAR_DATA *read_para_array);
int32 S7_FAR_PASCAL s7_multiple_write_req(ord32 cp_descr,ord16 cref,
                                          ord16 orderid,ord16 number,
                                          struct S7_WRITE_PARA 
                                                 S7_FAR_DATA *write_para_array,
                                          void S7_FAR_DATA *od_ptr);
int32 S7_FAR_PASCAL s7_read_req(ord32 cp_descr,ord16 cref,ord16 orderid,
                                struct S7_READ_PARA 
                                       S7_FAR_DATA *read_para_ptr);
int32 S7_FAR_PASCAL s7_receive(ord32 cp_descr, ord16 S7_FAR_DATA *cref_ptr,
                               ord16 S7_FAR_DATA *orderid_ptr);
int32 S7_FAR_PASCAL s7_set_window_handle_msg(ord32 cp_descr,
                                             ord32 hWnd,
                                             ord32 msgID);

#define s7_set_window_handle_msg(a,b,c) s7_set_window_handle_msg(a,          \
                                                                 (ord32)(b), \
                                                                 c)

int32 S7_FAR_PASCAL s7_shut(ord32 cp_descr);

void  S7_FAR_PASCAL s7_trace(char S7_FAR_DATA *msg);

int32 S7_FAR_PASCAL s7_vfd_state_req(ord32 cp_descr,
									 ord16 cref,
									 ord16 orderid);

int32 S7_FAR_PASCAL s7_write_req(ord32 cp_descr,
								 ord16 cref,
								 ord16 orderid,
                                 struct S7_WRITE_PARA S7_FAR_DATA *write_para_ptr,
                                 void S7_FAR_DATA *od_ptr);

int32 S7_FAR_PASCAL s7_write_long_req(ord32 cp_descr,
								 ord16 cref,
								 ord16 orderid,
                                 struct S7_WRITE_PARA_LONG S7_FAR_DATA *write_para_ptr,
                                 void S7_FAR_DATA *od_ptr);

void  S7_FAR_PASCAL s7_write_trace_buffer(char S7_FAR_DATA *filename);

int32	S7_FAR_PASCAL	s7_bsend_req(	ord32 cp_descr,
										ord16 cref,
										ord16 orderid,
										ord32 r_id,
										void *buffer_ptr,
										ord32 buffer_len);

int32	S7_FAR_PASCAL	s7_get_bsend_cnf(void);

int32	S7_FAR_PASCAL	s7_brcv_init(	ord32 cp_descr,
										ord16 cref,
										ord32 r_id);

int32	S7_FAR_PASCAL	s7_get_brcv_ind(	void  *buffer_ptr,
											ord32 buffer_len,
											ord32 *r_id_ptr,
											ord32 *rec_buffer_len_ptr);

int32	S7_FAR_PASCAL	s7_brcv_stop(	ord32 cp_descr,
										ord16 cref,
										ord32 r_id);

int32	S7_FAR_PASCAL	s7_msg_initiate_req(	ord32 cp_descr,
												ord16 cref,
												ord16 orderid,
												ord16 fct_code);

int32   S7_FAR_PASCAL   s7_get_msg_initiate_cnf(void);

int32	S7_FAR_PASCAL	s7_msg_abort_req(	ord32 cp_descr,
											ord16 cref,
											ord16 orderid,
											ord16 fct_code);

int32   S7_FAR_PASCAL   s7_get_msg_abort_cnf(void);

int32	S7_FAR_PASCAL   s7_get_scan_ind (	void    *od_ptr,
                                            ord16	*no_scan_objects_ptr,						
											struct  S7_TIME_STAMP 	*time_stamp_ptr,
											struct  S7_SCAN_OBJECT 	*scan_objects_array);

int32	S7_FAR_PASCAL   s7_get_alarm_ind (	void    *od_ptr,
                                            ord16	*state_ptr,
										    ord16	*ack_state_ptr,	
											ord16	*event_state_ptr,
											ord32	*event_id_ptr,
											struct 	S7_TIME_STAMP *time_stamp_ptr,
											ord16	*no_add_value_ptr,	
											struct 	S7_ADD_VALUE  *add_value_ptr);

int32	S7_FAR_PASCAL	s7_diag_init(		ord32	cp_descr);

int32	S7_FAR_PASCAL	s7_get_diag_ind(	ord16	number,
											CONN_INFO *info_ptr);

int32	S7_FAR_PASCAL	s7_diag_stop(		ord32	cp_descr);

int32   S7_FAR_PASCAL   s7_block_read_req ( ord32	cp_descr, 	
                                            ord16	cref, 	    
                                            ord16	orderid, 	
                                            ord32	block_flags, 
                                            ord16	block_type, 	
                                            ord16	block_nr, 	
                                            char	*block_file_name);

int32   S7_FAR_PASCAL   s7_block_write_req (ord32	cp_descr, 	
                                            ord16	cref, 	    
                                            ord16	orderid, 	
                                            ord32	block_flags, 
                                            char	*block_file_name);

int32   S7_FAR_PASCAL   s7_block_delete_req (ord32	cp_descr, 	
                                            ord16	cref, 	    
                                            ord16	orderid, 	
                                            ord32	block_flags, 
                                            ord16	block_num, 	
                                            struct S7_BLOCK_LIST *block_list_ptr);

int32   S7_FAR_PASCAL   s7_block_link_in_req ( ord32 cp_descr, 	
                                            ord16 cref, 	    
                                            ord16 orderid, 	
                                            ord16 block_num, 	
                                            struct S7_BLOCK_LIST *block_list_ptr);

int32   S7_FAR_PASCAL   s7_block_compress_req ( ord32 cp_descr, 	
                                            ord16 cref, 	    
                                            ord16 orderid);

int32   S7_FAR_PASCAL   s7_get_block_read_cnf (ord32 *block_len_ptr);

int32   S7_FAR_PASCAL   s7_get_block_write_cnf ( void );

int32   S7_FAR_PASCAL   s7_get_block_delete_cnf ( void );

int32   S7_FAR_PASCAL   s7_get_block_link_in_cnf ( void );

int32   S7_FAR_PASCAL   s7_get_block_compress_cnf ( void );

int32	S7_FAR_PASCAL   s7_password_req ( ord32	cp_descr, 
                                          ord16	cref, 
                                          ord16	orderid, 
                                          ord32	password_flags,
                                          ord8	*password_ptr); 	

int32	S7_FAR_PASCAL   s7_get_password_cnf ( void ) ;


#ifdef __cplusplus
}
#endif

#endif
 
/****************************************************************************/
/*     Copyright (C) Siemens AG 1995  All Rights Reserved.                  */
/****************************************************************************/
