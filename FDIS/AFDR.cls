VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "cAFDR"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Private FDB As DBI.DBISer
Private FDPTR As fdis.cFDPTR

Private WithEvents tAFDR As EdpsUtils.CEdpsTimer
Attribute tAFDR.VB_VarHelpID = -1
'Private WithEvents tAFDR As Timer
'Private txtDDEC As TextBox
Private sReq As String
Private sLogFile As String
Private MyQueueNum As Long
Private sDBName As String
Private sSrv As String
Private sUserName As String
Private sPWD As String

Private Sub Class_Initialize()
    sLogFile = App.Path + "\cAFDR" + Format(Now(), "yyyyMMddhhnnss.log")
    MyQueueNum = 0
    
    Set FDB = New DBI.DBISer
 '   Load frmAFDR
    
'    Set txtDDEC = frmAFDR.txtRWDDEVal
'    Set tAFDR = frmAFDR.AsRead
'    tAFDR.Enabled = False
    Set tAFDR = New EdpsUtils.CEdpsTimer
    tAFDR.Interval = 1000
    tAFDR.Enabled = True
    Set FDPTR = New cFDPTR
End Sub

Private Sub Class_Terminate()
    Dim sReq As String
    
    tAFDR.Enabled = False
    
    ' Clean all used queues in case if any
    Do
        sReq = qm_pop(MyQueueNum)
    Loop While sReq <> sQUEUEMPTY
    Do
        sReq = qm_pop(RESULTQUEUE)
    Loop While sReq <> sQUEUEMPTY
    
    Set tAFDR = Nothing
'    Set txtDDEC = Nothing
'    Unload frmAFDR
    
    ' Terminate open connection to SQL
    Set FDB = Nothing
    
    Set FDPTR = Nothing
    
    ' Terminate all open connections to SAPI
    SAPI_Shut
End Sub

Public Property Get Interval() As Variant
    Interval = tAFDR.Interval
End Property

Public Property Let Interval(ByVal vNewValue As Variant)
    tAFDR.Interval = vNewValue
End Property

Private Sub tAFDR_OnTimer()
    Call tAFDR_Timer
End Sub

Private Sub tAFDR_Timer()
    On Error GoTo tAFDR_Timer_error
    
    sReq = qm_pop(MyQueueNum)
    If sReq <> sQUEUEMPTY Then
        qm_push RESULTQUEUE, RW_FDatum(sReq)
    End If
    
tAFDR_Timer_exit:
    Exit Sub

tAFDR_Timer_error:
    Str2File sLogFile, "tAFDR_Timer: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
        "Error: " + Err.Description + " Request: " + sReq
'    Beep
    Resume tAFDR_Timer_exit
End Sub

Public Property Get DBISCatalog() As String
    DBISCatalog = sDBName
End Property

Public Property Let DBISCatalog(ByVal vNewValue As String)
    sDBName = vNewValue
    'FDB.InitialCat = vNewValue
End Property

Public Property Get SQLUserName() As Variant
    SQLUserName = sUserName
End Property

Public Property Let SQLUserName(ByVal vNewValue As Variant)
    sUserName = vNewValue
    'FDB.User = vNewValue
End Property

Public Property Get SQLUserPassword() As Variant
    SQLUserPassword = sPWD
End Property

Public Property Let SQLUserPassword(ByVal vNewValue As Variant)
    sPWD = vNewValue
    'FDB.PWD = vNewValue
End Property

Public Property Get SQLSrv() As Variant
    SQLSrv = sSrv
End Property

Public Property Let SQLSrv(ByVal vNewValue As Variant)
    sSrv = vNewValue
    'FDB.DataSource = vNewValue
End Property

Public Property Get LogFile() As String
    LogFile = sLogFile
End Property

Public Property Let LogFile(ByVal vNewValue As String)
    sLogFile = vNewValue
End Property

Public Function RW_FDatum(ByVal sRequest As String) As String
    Dim lRetCode As Long, sStr As String, lPos As Long, vPar As Variant, vRes As Variant

    On Error GoTo RW_FDatum_error
    
    FDPTR.PoolString = sRequest
    Select Case FDPTR.LinkType
    Case "DDE"
'        txtDDEC.LinkTimeout = 100
'        txtDDEC.LinkTopic = FDPTR.Addr1
'        txtDDEC.LinkItem = FDPTR.Addr2
'        If FDPTR.Action = FDAction.FDRead Then
'            txtDDEC.LinkMode = vbLinkManual
'            txtDDEC.LinkRequest
'            txtDDEC.LinkMode = vbLinkNone
'            FDPTR.Value = txtDDEC.Text
'            FDPTR.Date2 = Now()
'        ElseIf FDPTR.Action = FDAction.FDWrite Then
'            txtDDEC.Text = FDPTR.Value
'            txtDDEC.LinkMode = vbLinkManual
'            txtDDEC.LinkPoke
'            txtDDEC.LinkMode = vbLinkNone
'            FDPTR.Date2 = Now()
'        End If
    Case "SGR"
        If FDPTR.Action = FDAction.FDRead Then
            lRetCode = SAPI_MRead(FDPTR.Addr1, sStr)
            lPos = InStr(1, sStr, Chr(0))
            If lPos > 0 Then
                sStr = Left(sStr, lPos - 1)
            End If
                        
           If lRetCode < 0 Then
                FDPTR.Value = "FdIs_ErRoR: SAPI_MRead return code:" + _
                                            Str(lRetCode) + "! " + sStr
                Str2File sLogFile, "RW_FDatum SGR: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
                    " Error: " + FDPTR.Value + " Request: " + sRequest
            Else
                FDPTR.Value = sStr
            End If
            FDPTR.Date2 = Now()
        ElseIf FDPTR.Action = FDAction.FDWrite Then
            lRetCode = SAPI_MWrite(FDPTR.Addr1)
            FDPTR.Date2 = Now()
            If lRetCode <> 0 Then
                FDPTR.Value = "FdIs_ErRoR: SAPI_MWrite return code " + Str(lRetCode) + "!"
                Str2File sLogFile, "RW_FDatum: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
                    " Error: " + FDPTR.Value + " Request: " + sRequest
            End If
        End If
    Case "SAP"
        If FDPTR.Action = FDAction.FDRead Then
            lRetCode = SAPI_Read(FDPTR.Addr1, FDPTR.Addr2, _
                            FDPTR.Addr3, FDPTR.Addr4, sStr)
            lPos = InStr(1, sStr, Chr(0))
            If lPos > 0 Then
                sStr = Left(sStr, lPos - 1)
            End If
                        
            If lRetCode < 0 Then
                FDPTR.Value = "FdIs_ErRoR: SAPI_read return code:" + _
                                            Str(lRetCode) + "! " + sStr
                Str2File sLogFile, "RW_FDatum: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
                    " Error: " + FDPTR.Value + " Request: " + sRequest
            Else
                FDPTR.Value = sStr
            End If
            FDPTR.Date2 = Now()
        ElseIf FDPTR.Action = FDAction.FDWrite Then
            lRetCode = SAPI_Write(FDPTR.Addr1, FDPTR.Addr2, _
                            FDPTR.Addr3, FDPTR.Addr4, FDPTR.Value)
            FDPTR.Date2 = Now()
            If lRetCode <> 0 Then
                FDPTR.Value = "FdIs_ErRoR: SAPI_write return code " + Str(lRetCode) + "!"
                Str2File sLogFile, "RW_FDatum: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
                    " Error: " + FDPTR.Value + " Request: " + sRequest
            End If
        End If
    Case "DBI"
        ReDim vPar(0 To 4)
        vPar(0) = AlwayString(FDPTR.Addr2)
        vPar(1) = AlwayString(FDPTR.Addr3)
        vPar(2) = AlwayString(FDPTR.Addr4)
        vPar(3) = AlwayString(FDPTR.Addr5)
        vPar(4) = AlwayString(FDPTR.Addr6)
        
        If FDPTR.Action = FDAction.FDRead Then
            vRes = FDB.RetrieveData(FDPTR.Addr1, vPar)
            If VarType(vRes) <> vbString Then
                FDPTR.Value = vRes(0, 0)
            Else
                FDPTR.Value = "FdIs_ErRoR: " + vRes
            End If
            FDPTR.Date2 = Now()
        ElseIf FDPTR.Action = FDAction.FDWrite Then
            FDPTR.Value = FDB.InsertData(FDPTR.Addr1, vPar)
            FDPTR.Date2 = Now()
        End If
    Case Else

    End Select

RW_FDatum_exit:
    RW_FDatum = FDPTR.PoolString
    Exit Function

RW_FDatum_error:
    Str2File sLogFile, "RW_FDatum: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
        " Error: " + Err.Description + " Request: " + sRequest
    Beep
    Resume RW_FDatum_exit
End Function

Public Property Get Enabled() As Boolean
    Enabled = tAFDR.Enabled
End Property

Public Property Let Enabled(ByVal vNewValue As Boolean)
    tAFDR.Enabled = vNewValue
End Property

Public Function DoItNow(vFDPTR As String) As Boolean
    DoItNow = (qm_push(MyQueueNum, vFDPTR) = 0)
End Function

