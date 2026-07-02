VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "cCalculator"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

'Private MyFD As fdis.cAFDR
Private MyFDPTR As Sis.cFDPTR
Private WithEvents tmrScheduler As Timer
Attribute tmrScheduler.VB_VarHelpID = -1
Private vFormulas() As Sis.cFormula
Private vLocSched As Sis.cSchedPTR
Private iVisualLogging As Integer ' from -32,768 to 32,767
Private sLogFile As String
Private dtDate As Date
Private bConnAllowed As Boolean

Private lCalculatorID As Long
Private sMutexName As String
Private lMutex As Long
Private SA As SECURITY_ATTRIBUTES

Private lRunCount As Long ' from -2,147,483,648 to 2,147,483,647
Private lRunID As Long

Private Const QUEUEEMPTY As String = "Queue empty!"

Private vOp1 As New Sis.cFDPTR, vOp2 As New Sis.cFDPTR, _
        vOp3 As New Sis.cFDPTR, iCurrent As Integer

Public Property Let VisualLogging(ByVal vNewValue As Integer)
    iVisualLogging = vNewValue
End Property

Public Property Get VisualLogging() As Integer
    VisualLogging = iVisualLogging
End Property

Public Property Get ConnAllowed() As Boolean
    ConnAllowed = bConnAllowed
End Property

Public Property Let ConnAllowed(ByVal vNewValue As Boolean)
    bConnAllowed = vNewValue
    vOp1.LEISConnAllowed = bConnAllowed
    vOp1.LEISConnAllowed = bConnAllowed
    vOp1.LEISConnAllowed = bConnAllowed
    MyFDPTR.LEISConnAllowed = bConnAllowed
End Property

Public Sub StartProcessing()
' Create x-ref betw. vFormulas snd vParameters
    Dim lCnt1 As Long, lCnt2 As Long
        
    For lCnt1 = LBound(vFormulas, 1) + 1 To UBound(vFormulas, 1)
        For lCnt2 = vFormulas(lCnt1).MinLine To vFormulas(lCnt1).MaxLine
            vFormulas(lCnt1).Current = lCnt2
            If vFormulas(lCnt1).Type1 = "PARAM" Then
                vFormulas(lCnt1).Line1 = CInt(vFormulas(lCnt1).Operand1)
            End If
            If vFormulas(lCnt1).Type2 = "PARAM" Then
                vFormulas(lCnt1).Line2 = CInt(vFormulas(lCnt1).Operand2)
            End If
            If vFormulas(lCnt1).Type3 = "PARAM" Then
                vFormulas(lCnt1).Line3 = CInt(vFormulas(lCnt1).Operand3)
            End If
        Next lCnt2
        
        If UBound(vFormulas, 1) > lCnt1 Then
            If vFormulas(lCnt1 + 1) Is Nothing Then
                lCnt1 = lCnt1 + 1
            End If
        End If
    Next lCnt1
    
    ' Start Processing
    Set MFDPTR = New Sis.cFDPTR
    Me.Enabled = True
End Sub

Public Sub AddFormula( _
    ByVal Formula_ID As Variant, _
    ByVal Line_ID As Variant, _
    ByVal FunctionCode As Variant, _
    ByVal Operand1 As Variant, _
    ByVal Type1 As Variant, _
    ByVal Operand2 As Variant, _
    ByVal Type2 As Variant, _
    ByVal Operand3 As Variant, _
    ByVal Type3 As Variant, _
    ByVal Descr As Variant)

    Dim lCnt As Long

    lCnt = UBound(vFormulas, 1)
    If lCnt < Formula_ID Then
        ReDim Preserve vFormulas(0 To Formula_ID)
        lCnt = Formula_ID
        Set vFormulas(lCnt) = New Sis.cFormula
        vFormulas(lCnt).FormulaID = Formula_ID
    Else
        lCnt = Formula_ID
    End If

    vFormulas(lCnt).CreateLine _
            Line_ID, FunctionCode, Operand1, _
            Type1, 0, Operand2, Type2, _
            0, Operand3, Type3, 0, Descr
End Sub

Private Function LocalRWFDatum(ByRef FDPTR As cFDPTR) As Boolean
    Dim lRetCode As Long, sStr As String, lPos As Long, vPar As Variant, vRes As Variant

    On Error GoTo RW_FDatum_error
    
    Select Case FDPTR.LinkType
    Case "SGR"
        If FDPTR.Action = FDRead Then
            lRetCode = SAPI_MRead(FDPTR.Addr1, sStr)
            lPos = InStr(1, sStr, Chr(0))
            If lPos > 0 Then
                sStr = Left(sStr, lPos - 1)
            End If
                        
           If lRetCode < 0 Then
                FDPTR.ShareValue vbNullString
                Str2File sLogFile, "RW_FDatum SGR: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
                    " Error: " + FDPTR.Value + " Request: " + FDPTR.PoolString
            Else
                LocalRWFDatum = True
                FDPTR.ShareValue sStr
            End If
            FDPTR.Date2 = Now()
        ElseIf FDPTR.Action = FDWrite Then
            lRetCode = SAPI_MWrite(FDPTR.Addr1)
            FDPTR.Date2 = Now()
            If lRetCode <> 0 Then
                Str2File sLogFile, "RW_FDatum: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
                    " Error: " + FDPTR.Value + " Request: " + FDPTR.PoolString
            Else
                LocalRWFDatum = True
            End If
        End If
    Case "SAP"
        If FDPTR.Action = FDRead Then
            lRetCode = SAPI_Read(FDPTR.Addr1, FDPTR.Addr2, _
                            FDPTR.Addr3, FDPTR.Addr4, sStr)
            lPos = InStr(1, sStr, Chr(0))
            If lPos > 0 Then
                sStr = Left(sStr, lPos - 1)
            End If
                        
            If lRetCode < 0 Then
                FDPTR.ShareValue vbNullString
                Str2File sLogFile, "RW_FDatum: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
                    " Error: " + FDPTR.Value + " Request: " + FDPTR.PoolString
            Else
                FDPTR.ShareValue sStr
                LocalRWFDatum = True
            End If
            FDPTR.Date2 = Now()
        ElseIf FDPTR.Action = FDWrite Then
            lRetCode = SAPI_Write(FDPTR.Addr1, FDPTR.Addr2, _
                            FDPTR.Addr3, FDPTR.Addr4, FDPTR.Value)
            FDPTR.Date2 = Now()
            If lRetCode <> 0 Then
                FDPTR.Value = "FdIs_ErRoR: SAPI_write return code " + str(lRetCode) + "!"
                Str2File sLogFile, "RW_FDatum: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
                    " Error: " + FDPTR.Value + " Request: " + FDPTR.PoolString
            Else
                LocalRWFDatum = True
            End If
        End If
    Case Else

    End Select

RW_FDatum_exit:
    Exit Function

RW_FDatum_error:
    Str2File sLogFile, "RW_FDatum: " + Format(Now(), "yyyy/MMM/dd hh:nn:ss ") + _
        " Error: " + Err.Description + " Request: " + FDPTR.PoolString
End Function

Private Sub Calculate(ByVal lLinNum As Long)
    On Error GoTo Calculate_error
    
    dtDate = Now
    lRunID = RunID()
    
    For iCurrent = vFormulas(lLinNum).First To vFormulas(lLinNum).Last
        vFormulas(lLinNum).Current = iCurrent
        If vFormulas(lLinNum).Type1 = "CONST" Then
            vOp1.CValue = vFormulas(lLinNum).Operand1
        ElseIf vFormulas(lLinNum).Type1 = "PARAM" Then
            vOp1.GetData CInt(vFormulas(lLinNum).Operand1)
        End If
        If vFormulas(lLinNum).Type2 = "CONST" Then
            vOp2.CValue = vFormulas(lLinNum).Operand2
        ElseIf vFormulas(lLinNum).Type2 = "PARAM" Then
            vOp2.GetData CInt(vFormulas(lLinNum).Operand2)
        End If
        If vFormulas(lLinNum).Type3 = "CONST" Or vFormulas(lLinNum).Type3 = "LINE" Then
            vOp3.CValue = vFormulas(lLinNum).Operand3
        ElseIf vFormulas(lLinNum).Type3 = "PARAM" Then
            vOp3.GetData CInt(vFormulas(lLinNum).Operand3)
        End If
        
        vOp1.ID = lRunID
        vOp2.ID = lRunID
        vOp3.ID = lRunID
        
        If iVisualLogging = 1 Then
            If Len(vFormulas(lLinNum).Descr) > 1 Then
                Str2Log vFormulas(lLinNum).Descr
            End If
        ElseIf iVisualLogging >= 2 Then
            Str2Log "Formula: " & str(vFormulas(lLinNum).FormulaID) & " " & _
                "Line: " & str(vFormulas(lLinNum).Line_ID) & " " & _
                vFormulas(lLinNum).Descr & " " & _
                vFormulas(lLinNum).FunctionCode & " Operators:|" & _
                vFormulas(lLinNum).Type1 & ":" & vOp1.Value & "|" & _
                vFormulas(lLinNum).Type2 & ":" & vOp2.Value & "|" & _
                vFormulas(lLinNum).Type3 & ":" & vOp3.Value & "|"
        End If
        
        Select Case vFormulas(lLinNum).FunctionCode
        
        Case "FOR"
            vOp3.Value = vOp3.Value + 1
            If CDbl(vOp3.Value) > CDbl(vOp2.Value) Then
                vOp3.Value = vOp1.Value
            End If
            vOp3.ShareData vOp3.Tag
        
        Case "+"
            vOp3.Value = AlwayString(CDbl(vOp1.Value) + CDbl(vOp2.Value))
            vOp3.Date1 = dtDate
            vOp3.ShareData vOp3.Tag
        
        Case "-"
            vOp3.Value = AlwayString(CDbl(vOp1.Value) - CDbl(vOp2.Value))
            vOp3.Date1 = dtDate
            vOp3.ShareData vOp3.Tag
        
        Case "&"
            vOp3.Value = vOp1.Value & vOp2.Value
            vOp3.Date1 = dtDate
            vOp3.ShareData vOp3.Tag
        
        Case "INC"
            vOp3.Value = vOp3.Value + 1
            vOp3.Date1 = dtDate
            vOp3.ShareData vOp3.Tag
        
        Case "INSTR"
            vOp3.Value = Trim(str(InStr(vOp1.Value, vOp2.Value)))
        
        Case "LEFT"
            vOp3.Value = Left(vOp1.Value, CLng(vOp2.Value))
        
        Case "RIGHT"
            vOp3.Value = Right(vOp1.Value, CLng(vOp2.Value))
            
        Case "IF"
            If Trim(vOp1.Value) = Trim(vOp2.Value) Then
                iCurrent = CLng(vOp3.Value) - 1
            End If
        
        Case "IFNOT"
            If Trim(vOp1.Value) <> Trim(vOp2.Value) Then
                iCurrent = CLng(vOp3.Value) - 1
            End If
        
        Case "IF>"
            If CDbl(vOp1.Value) > CDbl(vOp2.Value) Then
                iCurrent = CLng(vOp3.Value) - 1
            End If
        
        Case "IF<="
            If CDbl(vOp1.Value) <= CDbl(vOp2.Value) Then
                iCurrent = CLng(vOp3.Value) - 1
            End If
        
        Case "GOTO"
            iCurrent = CLng(vOp1.Value) - 1
        
        Case "SET"
            vOp3.Date1 = dtDate
            vOp3.Value = vOp1.Value
            vOp3.ShareData vOp3.Tag
        
        Case "SHARE"
            vOp3.Date1 = dtDate
            vOp3.sValue = vOp1.Value
            vOp3.ShareData vOp3.Tag
        
        Case "MREAD"    ' Multiple read
            vOp1.Action = FDRead
            vOp1.Addr1 = CombineToRead(vOp1.Addr6)
            If Not LocalRWFDatum(vOp1) Then Err.Raise 0, "MREAD", "Eror Reading Multidata"
'            vOp1.PoolString = MyFD.RW_FDatum(vOp1.PoolString)
            vOp1.Date1 = dtDate
            vOp1.Value = vOp1.Value
            vOp1.ShareData vOp1.Tag

        Case "SMREAD"   ' SimpleMultiple read
            vOp1.Action = FDRead
            vOp1.Addr1 = CombineToRead(vOp1.Addr6)
            If Not LocalRWFDatum(vOp1) Then Err.Raise 0, "SMREAD", "Eror Reading Multidata"
'            vOp1.PoolString = MyFD.RW_FDatum(vOp1.PoolString)
            SpreadResuts vOp1.Value
            vOp1.Date1 = dtDate
            vOp1.ShareData vOp1.Tag
        
        Case "READ"
            vOp1.Action = FDRead
            If Not LocalRWFDatum(vOp1) Then Err.Raise 0, "READ", "Eror Reading datum"
'            vOp1.PoolString = MyFD.RW_FDatum(vOp1.PoolString)
            vOp1.Date1 = dtDate
            vOp1.Value = vOp1.Value
            vOp1.ShareData vOp1.Tag

        Case "SREAD" ' Read data with no check id data to be stored in DB.
            vOp1.Action = FDRead
            If Not LocalRWFDatum(vOp1) Then Err.Raise 0, "SREAD", "Eror Reading datum"
'            vOp1.PoolString = MyFD.RW_FDatum(vOp1.PoolString)
            vOp1.Date1 = dtDate
            vOp1.ShareData vOp1.Tag
        
        Case "WRITE"
            vOp3.Date1 = dtDate
            vOp3.Action = FDWrite
            If Not LocalRWFDatum(vOp3) Then Err.Raise 0, "WRITE", "Eror write data"
'            vOp3.PoolString = MyFD.RW_FDatum(vOp3.PoolString)
            vOp3.ShareData vOp3.Tag
        
'        Case "AREAD"
'            vOp1.Action = FDRead
'            MyFD.DoItNow vOp1.PoolString
'
'        Case "AWRITE"
'            vOp3.Action = FDWrite
'            vOp3.Value = vOp1.Value
'            MyFD.DoItNow vOp3.PoolString
            
        Case "LHSTR2VAL" 'LoadHexSTRing2VALue
            ' Parameters:   1 - HexString
            '               2 - Start position in HexString
            '               3 - Dest parameter (cFDPTR).
            vOp3.Hex = Mid(vOp1.Value, CInt(vOp2.Value), vOp3.HexLen)
            vOp3.Date1 = dtDate
            vOp3.Date2 = vOp1.Date2
            vOp3.Value = vOp3.Value
            vOp3.ShareData vOp3.Tag
            
        Case "SAVE"
            vOp3.Value = vOp3.Value
            
        Case Else
            Str2File sLogFile, _
                    "Unknown OPERATION: " & vFormulas(lLinNum).FunctionCode & _
                    " Formula: " & str(vFormulas(lLinNum).FormulaID) & " Line: " & _
                    str(vFormulas(lLinNum).Current)
            If iVisualLogging >= 2 Then
                Str2Log "Unknown OPERATION: " & vFormulas(lLinNum).FunctionCode & _
                    " Formula: " & str(vFormulas(lLinNum).FormulaID) & " Line: " & _
                    str(vFormulas(lLinNum).Current)
            End If
        End Select
    Next iCurrent

Calculate_exit:
    Exit Sub

Calculate_error:
    Str2File sLogFile, "Error during calculation!" & _
        " Formula: " & str(vFormulas(lLinNum).FormulaID) & _
        " Line: " & str(vFormulas(lLinNum).Current) & _
        " VB Error: " & str(Err.Number) & _
        " Err descr.: " & Err.Description
    If iVisualLogging >= 2 Then
        Str2Log "Error during calculation!" & _
        " Formula: " & str(vFormulas(lLinNum).FormulaID) & _
        " Line: " & str(vFormulas(lLinNum).Current) & _
        " VB Error: " & str(Err.Number) & _
        " Err descr.: " & Err.Description
    End If
    Resume Calculate_exit
End Sub

Private Sub Class_Initialize()
    ' Initialise SA
    SA.nLength = 12
    SA.lpSecurityDescriptor = 0
    SA.bInheritHandle = 0
      
    If sLogFile = "" Then
        sLogFile = App.Path & "\EDPS.log"
    End If
    
    iVisualLogging = 0
    lRunCount = CLng(Format(Now(), "mmss")) * 100
    
    ReDim vFormulas(0 To 0)
    
    Set vLocSched = New Sis.cSchedPTR
    
    Set MyFDPTR = New Sis.cFDPTR
    
'    Set MyFD = New fdis.cAFDR
    Load frmCalcTimer
    Set tmrScheduler = frmCalcTimer.tTimer
    tmrScheduler.Enabled = False
    tmrScheduler.Interval = 10
'    MyFD.Interval = 10
'    MyFD.Enabled = True
    
    Call Str2File(sLogFile, "cCalculator exampler created.")
End Sub

Public Property Let LogPath(ByVal vNewValue As Variant)
    Dim iI As Integer
    
    sLogFile = vNewValue
    vLocSched.LogPath = sLogFile
    MyFDPTR.LogPath = sLogFile
    vOp1.LogPath = sLogFile
    vOp2.LogPath = sLogFile
    vOp3.LogPath = sLogFile
    For iI = 0 To UBound(vFormulas)
        vFormulas(iI).LogPath = sLogFile
    Next iI
End Property

Public Property Get LogPath() As Variant
    LogPath = sLogFile
End Property

Private Sub Class_Terminate()
    Dim lCnt As Long
    
'    MyFD.Enabled = False
'    Set MyFD = Nothing
    tmrScheduler.Enabled = False
    Set tmrScheduler = Nothing
    Unload frmCalcTimer
    Set MyFDPTR = Nothing
    Set vLocSched = Nothing
    
    For lCnt = 0 To UBound(vFormulas)
        Set vFormulas(lCnt) = Nothing
    Next lCnt
    ReDim vFormulas(0 To 0)
    
    Set vOp1 = Nothing
    Set vOp2 = Nothing
    Set vOp3 = Nothing

    Call Str2File(sLogFile, "cCalculator exampler destroyed.")
End Sub

Public Property Get Interval() As Variant
    Interval = tmrScheduler.Interval
End Property

Public Property Let Interval(ByVal vNewValue As Variant)
    tmrScheduler.Interval = vNewValue
'    MyFD.Interval = vNewValue
End Property

Public Property Get Enabled() As Variant
    Enabled = tmrScheduler.Enabled
End Property

Public Property Let Enabled(ByVal vNewValue As Variant)
    tmrScheduler.Enabled = vNewValue
'    MyFD.Enabled = vNewValue
End Property

Private Sub tmrScheduler_Timer()
    Dim sStr As String, lL2 As Long, lL1 As Long, lMutex As Long, lMutexResult As Long
    
    sStr = qm_pop(ToCalculate)
    If sStr <> QUEUEEMPTY Then
    ' Run formula
        vLocSched.PoolString = sStr
        lL2 = vLocSched.Formula_ID
        For lL1 = LBound(vFormulas, 1) + 1 To UBound(vFormulas, 1)
            If vFormulas(lL1).FormulaID = lL2 Then Exit For
        Next lL1
        If iVisualLogging >= 2 Then
            Str2Log "Starting calculation of formula: " & str(lL1)
        End If
        
        ' Test for StopProcessing blocking
        lMutex = CreateMutex(SA, 0, sMutexName)
        lMutexResult = WaitForSingleObject(lMutex, MutexTimeOut)
        If lMutexResult = 0 Then
            Calculate lL1
            lMutexResult = ReleaseMutex(lMutex)
        Else ' Stop processing
            Me.Enabled = False
            Str2File sLogFile, "Time out in timer event waiting for FDI completion: " + sMutexName
        End If
    End If
End Sub

Public Property Get AFDRLoop() As Variant
'    AFDRLoop = MyFD.Interval
End Property

Public Property Let AFDRLoop(ByVal vNewValue As Variant)
'     MyFD.Interval = vNewValue
End Property

Public Sub StopProcessing()
    Me.Enabled = False
    Set MFDPTR = Nothing
End Sub

Public Property Get CalculatorID() As Variant
    CalculatorID = lCalculatorID
End Property

Public Property Let CalculatorID(ByVal vNewValue As Variant)
    lCalculatorID = vNewValue
    sMutexName = "MUTEX_" + Trim(str(vNewValue))
End Property

Private Function RunID() As Long
    If lRunCount < 999999 Then
        lRunCount = lRunCount + 1
    Else
        lRunCount = 0
    End If
    RunID = lCalculatorID * 1000000 + lRunCount
End Function
