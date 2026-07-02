VERSION 5.00
Begin VB.Form frmMain 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "EDPS"
   ClientHeight    =   5910
   ClientLeft      =   2370
   ClientTop       =   1620
   ClientWidth     =   9375
   Icon            =   "Main.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   5910
   ScaleWidth      =   9375
   Begin VB.CommandButton cmdRefrSw 
      Caption         =   "Stop Refresh"
      Height          =   375
      Left            =   4080
      TabIndex        =   3
      Top             =   0
      Width           =   1935
   End
   Begin VB.Timer tmrRefresh 
      Enabled         =   0   'False
      Interval        =   1300
      Left            =   3480
      Top             =   0
   End
   Begin VB.TextBox txtLog 
      Height          =   5415
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   480
      Width           =   9375
   End
   Begin VB.Label labQLen 
      Caption         =   "Unknown"
      Height          =   255
      Left            =   2280
      TabIndex        =   2
      Top             =   120
      Width           =   975
   End
   Begin VB.Label labQLength 
      Caption         =   "Length of Requests queue:"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   2055
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Private FDScheduler As sis.cScheduler
Private vCalculator() As sis.cCalculator
Private LDB As LEIS.LEISer
Private DBIS As DBI.DBISer
Private bLog2Scr As Boolean

Private Sub cmdRefrSw_Click()
    If cmdRefrSw.Caption = "Stop Refresh" Then
        bLog2Scr = False
        cmdRefrSw.Caption = "Start Refresh"
    Else
        bLog2Scr = True
        cmdRefrSw.Caption = "Stop Refresh"
    End If
End Sub

Private Sub Form_Load()
    ' Initialise SA
    SA.nLength = 12
    SA.lpSecurityDescriptor = 0
    SA.bInheritHandle = 0
      
    Me.Caption = "EDPS (v. " & Format(App.Major, "###0") & "." & _
                                        Format(App.Minor, "###0") & ")"
    Call Activate
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    If MsgBox("Close EDPS appliction?", vbOKCancel + vbCritical, "EDPS") = vbOK Then
        Call Deactivate
    Else
        Cancel = 1
    End If
End Sub

Private Sub tmrRefresh_Timer()
    Dim sVal As String
    
    labQLen.Caption = Str(FDScheduler.ReqQLength)
    
    If bLog2Scr Then
        sVal = FDScheduler.NextMsg()
        While sVal <> ""
            txtLog.Text = sVal + vbCrLf + txtLog.Text
            sVal = FDScheduler.NextMsg()
        Wend
        txtLog.Text = Left(txtLog.Text, 10000)
    Else
        FDScheduler.TruncateLog
    End If
End Sub

Private Sub Activate()
    Dim lCnt1 As Long, lCnt2 As Long, lCnt3 As Long, vTmp As Variant, _
        vVar As Variant, vVar1 As Variant, vVar2 As Variant, vPar As Variant, _
        iVal As Integer, iVal1 As Integer

    bLog2Scr = True
    Set FDScheduler = New sis.cScheduler
    
    iVal = CInt(ReadIniKey(App.Path + "\" + APPINIFILE, _
        APPINISECT, SCHEDSTEPKEY, "1000"))
    FDScheduler.Interval = iVal
    FDScheduler.VisualLogging = iShowForms

    lCnt1 = CInt(ReadIniKey(App.Path + "\" + APPINIFILE, _
        APPINISECT, WORKTHKEY, "1"))
    iVal = CInt(ReadIniKey(App.Path + "\" + APPINIFILE, _
        APPINISECT, CALCSTEPKEY, "10"))
    iVal1 = CInt(ReadIniKey(App.Path + "\" + APPINIFILE, _
        APPINISECT, AFDRSTEPKEY, "10"))
    
    ReDim vCalculator(1 To lCnt1)
    For lCnt2 = 1 To lCnt1
        Set vCalculator(lCnt2) = New sis.cCalculator
        vCalculator(lCnt2).Interval = iVal
        vCalculator(lCnt2).AFDRLoop = iVal1
        vCalculator(lCnt2).VisualLogging = iShowForms
        vCalculator(lCnt2).CalculatorID = lCnt2
    Next lCnt2
    
    Set LDB = New LEIS.LEISer
    Set DBIS = New DBI.DBISer
    
    vVar = ReadIniKey(App.Path + "\" + APPINIFILE, _
        COMMINISECT, DATASRCNAMKEY, "EUPMIKZALASQL07")
    DBIS.SetOLEDBProp DATASRCNAMP, vVar
    
    vVar = ReadIniKey(App.Path + "\" + APPINIFILE, _
        COMMINISECT, DBNAMEKEY, "PSS_Reports")
    DBIS.SetOLEDBProp DBNAMEP, vVar
    
    vVar = ReadIniKey(App.Path + "\" + APPINIFILE, _
        COMMINISECT, USERNAMEKEY, "PSS")
    DBIS.SetOLEDBProp USERNAMEP, vVar
    
    vVar = ReadIniKey(App.Path + "\" + APPINIFILE, _
        COMMINISECT, USERPWDKEY, "")
    DBIS.SetOLEDBProp USERPWDP, vVar
    
    vVar = ReadIniKey(App.Path + "\" + APPINIFILE, _
        COMMINISECT, NETLIBKEY, "dbmssocn")
    DBIS.SetOLEDBProp NETLIBP, vVar
    
    On Error GoTo LoginFailed
    DBIS.ReConnect
    
    ReDim vPar(0 To 0)
    vPar(0) = Trim(Str(lEDPSid))
    
    vVar = DBIS.RetrieveData("PhysParams", Null)
    vVar1 = DBIS.RetrieveData("Operations", vPar)
    vVar2 = DBIS.RetrieveData("Formulas", vPar)
    
    If Not LDB.WriteSISInfo(vVar, vVar2, vVar1) Then
        Str2File sLogFile, "Can't perform LDB.WriteSISInfo(vVar, vVar2, vVar1)"
        Exit Sub
    End If
    
LoginFailed:
    Resume ReadLocally

ReadLocally:
    On Error GoTo NextError

    vTmp = LDB.ReadParameters
    lCnt2 = LBound(vTmp, 1)
    For lCnt1 = LBound(vTmp, 2) To UBound(vTmp, 2)
        Select Case vTmp(lCnt2 + 1, lCnt1)
        Case "integer"
            vVar = CInt("0")
        Case "float"
            vVar = CSng("0")
        Case "long"
            vVar = CLng("0")
        Case Else
            vVar = ""
        End Select
        FDScheduler.AddParameter _
            vTmp(lCnt2, lCnt1), vTmp(lCnt2 + 1, lCnt1), vTmp(lCnt2 + 2, lCnt1), _
            vTmp(lCnt2 + 3, lCnt1), vTmp(lCnt2 + 4, lCnt1), vTmp(lCnt2 + 5, lCnt1), _
            vTmp(lCnt2 + 6, lCnt1), vTmp(lCnt2 + 7, lCnt1), vTmp(lCnt2 + 8, lCnt1), _
            vTmp(lCnt2 + 9, lCnt1), Now, Now, _
            vVar, 0, sis.FDAction.FDUndefigned
    Next lCnt1
    
    vTmp = LDB.ReadFormulas
    lCnt2 = LBound(vTmp, 1)
    For lCnt1 = LBound(vTmp, 2) To UBound(vTmp, 2)
        FDScheduler.AddSchedule vTmp(lCnt2, lCnt1), vTmp(lCnt2 + 1, lCnt1), _
                                vTmp(lCnt2 + 2, lCnt1)
    Next lCnt1

    vTmp = LDB.ReadOperations
    
    For lCnt3 = LBound(vCalculator, 1) To LBound(vCalculator, 1)
        lCnt2 = LBound(vTmp, 1)
        For lCnt1 = LBound(vTmp, 2) To UBound(vTmp, 2)
            vCalculator(lCnt3).AddFormula _
                vTmp(lCnt2, lCnt1), vTmp(lCnt2 + 1, lCnt1), vTmp(lCnt2 + 2, lCnt1), _
                vTmp(lCnt2 + 3, lCnt1), vTmp(lCnt2 + 4, lCnt1), _
                vTmp(lCnt2 + 5, lCnt1), vTmp(lCnt2 + 6, lCnt1), _
                vTmp(lCnt2 + 7, lCnt1), vTmp(lCnt2 + 8, lCnt1), _
                vTmp(lCnt2 + 9, lCnt1)
        Next lCnt1
        vCalculator(lCnt3).StartProcessing
    Next lCnt3
    
    FDScheduler.StartProcessing
    If iShowForms > 0 Then
        Me.tmrRefresh.Enabled = True
    Else
        Me.tmrRefresh.Enabled = False
    End If
    
ActivateExit:
    Exit Sub
    
NextError:
    Str2File sLogFile, "frmMain:Activate: Unable to initialise operations. Error: " & _
            Str(Err.Number) & " Descr: " & Err.Description
    MsgBox "frmMain:Activate: Unable to initialise operations. Error: " & _
            Str(Err.Number) & " Descr: " & Err.Description
    Resume ActivateExit
End Sub

Private Sub Deactivate()
    Dim lMutex As Long, lMutexResult As Long, lCnt3 As Long, sMName As String

    ' Stop updating of textbox
    tmrRefresh.Enabled = False

    ' Stop all the calculators (wait pending ones)
    For lCnt3 = LBound(vCalculator, 1) To LBound(vCalculator, 1)
        vCalculator(lCnt3).StopProcessing
        
        ' Wait for possible pending calculation
        sMName = "MUTEX_" + Trim(Str(vCalculator(lCnt3).CalculatorID))
        lMutex = CreateMutex(SA, 0, sMName)
        lMutexResult = WaitForSingleObject(lMutex, MutexTimeOut)
        If lMutexResult = 0 Then
            Sleep (4000)
            lMutexResult = ReleaseMutex(lMutex)
        Else
            Str2File sLogFile, "Time out waiting for FDI completion: " + sMName
        End If
        
        Set vCalculator(lCnt3) = Nothing
    Next lCnt3
    ReDim vCalculator(0)
    
    ' Stop scheduling
    FDScheduler.StopProcessing
    FDScheduler.TruncateLog
    Set FDScheduler = Nothing
    
    Set LDB = Nothing
    
    DBIS.ClearConnection
    Set DBIS = Nothing
End Sub
