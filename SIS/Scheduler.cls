VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "cScheduler"
Attribute VB_GlobalNameSpace = True
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

'Private FDI As fdis.cFDI
Private bFirstRun As Boolean

Private WithEvents tmrScheduler As Timer
Attribute tmrScheduler.VB_VarHelpID = -1
Private Const QUEUEEMPTY As String = "Queue empty!"

Private vLocFD As Sis.cFDPTR
Private vParameters As Sis.cFDPTR
Private vSchedule() As Sis.cSchedPTR
Private bConnAllowed As Boolean

Private lEDPSID As Long, iVisualLogging As Integer
Private sLogFile As String

Private Sub Class_Initialize()
    bFirstRun = True
    
    If sLogFile = "" Then
        sLogFile = App.Path & "\EDPS.log"
    End If
    
    Set vParameters = New Sis.cFDPTR
    Set vLocFD = New Sis.cFDPTR
    
    ReDim vSchedule(0 To 0)
'    Set FDI = New fdis.cFDI
    Load frmTimer
    Set tmrScheduler = frmTimer.tTimer
    tmrScheduler.Enabled = False
    tmrScheduler.Interval = 1000
    bConnAllowed = True
    
    Call Str2File(sLogFile, "cSheduler exampler created.")
End Sub

Public Property Get ConnAllowed() As Boolean
    ConnAllowed = bConnAllowed
End Property

Public Property Let ConnAllowed(ByVal vNewValue As Boolean)
    bConnAllowed = vNewValue
    vLocFD.LEISConnAllowed = bConnAllowed
    vParameters.LEISConnAllowed = bConnAllowed
End Property

Public Property Get Interval() As Variant
    Interval = tmrScheduler.Interval
End Property

Public Property Let Interval(ByVal vNewValue As Variant)
    tmrScheduler.Interval = vNewValue
End Property

Public Property Get Enabled() As Variant
    Enabled = tmrScheduler.Enabled
End Property

Public Property Let Enabled(ByVal vNewValue As Variant)
    tmrScheduler.Enabled = vNewValue
End Property

Private Sub Class_Terminate()
    Call Str2File(sLogFile, "cSheduler exampler destroyed.")
End Sub

Private Sub tmrScheduler_Timer()
    Dim sStr As String, lCnt As Long
'    Static vLocFD As New sis.cFDPTR
    
'    sStr = FDI.GetResult
'    While sStr <> QUEUEEMPTY
'        vLocFD.PoolString = sStr
'        vParameters.GetData vLocFD.Tag
'        vParameters.Value = vLocFD.Value
'        vParameters.Date1 = vLocFD.Date1
'        vParameters.Date2 = vLocFD.Date2
'        vParameters.ShareData vLocFD.Tag
'        If iVisualLogging > 1 Then
'            Str2Log "Scheduler. Asynch Result: " & sStr
'        End If
'
'        sStr = FDI.GetResult
'    Wend
    
    If bFirstRun Then
        bFirstRun = False
        For lCnt = LBound(vSchedule, 1) + 1 To UBound(vSchedule, 1)
            If vSchedule(lCnt).TimeLoop = -1 Then
                vSchedule(lCnt).TimeLoop = 0
                sStr = vSchedule(lCnt).PoolString
                If iVisualLogging > 0 Then
                    Str2Log "Autoexec: " & vSchedule(lCnt).Descr
                End If
                qm_push ToCalculate, sStr
            End If
        Next lCnt
        While qm_length(ToCalculate) > 0
            DoEvents
            DoEvents
            DoEvents
        Wend
        DoEvents
    End If
    
    For lCnt = LBound(vSchedule, 1) + 1 To UBound(vSchedule, 1)
        If vSchedule(lCnt).TimeLoop > 0 Then
            If vSchedule(lCnt).Left2Run > 0 Then
                vSchedule(lCnt).Left2Run = vSchedule(lCnt).Left2Run - 1
                If vSchedule(lCnt).Left2Run = 0 Then
                    vSchedule(lCnt).Left2Run = vSchedule(lCnt).TimeLoop
                    sStr = vSchedule(lCnt).PoolString
                    If iVisualLogging > 0 Then
                        Str2Log vSchedule(lCnt).Descr
                    End If
                    qm_pushex ToCalculate, sStr
                End If
            End If
        End If
    Next lCnt
End Sub

Public Property Get LogPath() As Variant
    LogPath = sLogFile
End Property

Public Property Let LogPath(ByVal vNewValue As Variant)
    Dim iI As Integer
    
    sLogFile = vNewValue
    vLocFD.LogPath = sLogFile
    vParameters.LogPath = sLogFile
    For iI = 0 To UBound(vSchedule)
        vSchedule(iI).LogPath = sLogFile
    Next iI
End Property

Public Property Get PriorityRun() As Variant
    PriorityRun = GetPriority()
End Property

Public Property Let PriorityRun(ByVal vNewValue As Variant)
    Call SetPriority(vNewValue)
End Property

Public Property Get EDPSID() As Variant
    EDPSID = lEDPSID
End Property

Public Property Let EDPSID(ByVal vNewValue As Variant)
    EDPSID = vNewValue
End Property

Public Property Get VisualLogging() As Integer
    VisualLogging = iVisualLogging
End Property

Public Property Let VisualLogging(ByVal vNewValue As Integer)
    iVisualLogging = vNewValue
End Property

Public Sub AddParameter(ByVal ParamID As Variant, _
    ByVal DataType As Variant, ByVal LinkType As Variant, _
    ByVal DataSetName As String, ByVal Addr1 As Variant, _
    ByVal Addr2 As Variant, ByVal Addr3 As Variant, _
    ByVal Addr4 As Variant, ByVal Addr5 As Variant, _
    ByVal Addr6 As Variant, ByVal Date1 As Variant, _
    ByVal Date2 As Variant, ByVal Value As Variant, _
    ByVal Tag As Variant, ByVal Action As Variant)

    Dim lCnt As Integer

    If ParamID >= vParameters.ArraySize Then
        vParameters.ArraySize = ParamID + 1
    End If
    lCnt = ParamID
'
'    lCnt = vParameters.ArraySize
'    vParameters.ArraySize = lCnt + 1
'
    vParameters.SetAllValues _
            ParamID, DataType, LinkType, DataSetName, _
            Addr1, Addr2, Addr3, Addr4, Addr5, Addr6, _
            Date1, Date2, Value, lCnt, Action
            
    vParameters.ShareData lCnt
End Sub

Public Sub AddSchedule(ByVal Formula_ID As Variant, _
    ByVal TimeLoop As Variant, ByVal Descr As Variant)

    Dim lCnt As Long

    lCnt = UBound(vSchedule, 1) + 1
    ReDim Preserve vSchedule(0 To lCnt)

    Set vSchedule(lCnt) = New Sis.cSchedPTR
    vSchedule(lCnt).SetAllValues Formula_ID, TimeLoop, TimeLoop, 0, Descr
End Sub

Public Sub StartProcessing()
    ' Start Processing
    tmrScheduler.Enabled = True
End Sub

Public Property Get ReqQLength() As Variant
    ReqQLength = qm_length(ToCalculate)
End Property

Public Function NextMsg() As String
    NextMsg = qm_pop(Messages)
    If NextMsg = QUEUEEMPTY Then
        NextMsg = ""
    End If
End Function

Public Sub TruncateLog()
    Dim sTmp As String
    
    sTmp = qm_pop(Messages)
    While sTmp <> QUEUEEMPTY
        sTmp = qm_pop(Messages)
    Wend
End Sub

Public Sub StopProcessing()
    Dim sStr As String
    Dim lCnt As Long
    
    ' Don't add new tasks to be processed
    tmrScheduler.Enabled = False
    
    ' Clear task's queue
    sStr = qm_pop(ToCalculate)
    While sStr <> QUEUEEMPTY
        sStr = qm_pop(ToCalculate)
    Wend
        
    ' Clear async result's queue
'    sStr = FDI.GetResult
'    While sStr <> QUEUEEMPTY
'        sStr = FDI.GetResult
'    Wend
    
    Set vParameters = Nothing
    Set vLocFD = Nothing
    
    For lCnt = 0 To UBound(vSchedule)
        Set vSchedule(lCnt) = Nothing
    Next lCnt
    ReDim vSchedule(0 To 0)
    
'    Set FDI = Nothing
    Set tmrScheduler = Nothing
    Unload frmTimer
End Sub

