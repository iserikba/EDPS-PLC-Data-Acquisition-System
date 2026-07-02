VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "DB"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private s As String * 100
Private lResult As Long
Private n As Long

Private UserName As String
Private ComputerName As String

Private WithEvents Conn As ADODB.Connection
Attribute Conn.VB_VarHelpID = -1
Private rstMain As ADODB.Recordset
Private cmdSQL As ADODB.Command
Private errLoop As ADODB.Error

Private vTa As Variant
Private iI As Integer

Private strCMD As String
Private strSQL As String

Private bConnStatus As Boolean
Private bConnPermission As Boolean 'local copy

Public Property Let ConnPermission(ByVal vData As Boolean)
    Dim dTPoint As Date
    
    If vData Then
        bConnPermission = vData
        Call Str2Log("Connection allowed.")
    Else
        If bConnStatus Then
            dTPoint = Now
            Do While True
                If Conn.State <> adStateConnecting And _
                    Conn.State <> adStateExecuting And _
                    Conn.State <> adStateFetching Then
                    Call Str2Log("Connection's not pending, could close / deny it.")
                    Exit Do
                End If
                If DateDiff("n", dTPoint, Now) > 2 Then
                    Str2Log "Connection probably died, could not deny connection."
                    GoTo PityLabel
                End If
                DoEvents
                Sleep (100)
                DoEvents
            Loop
            DoEvents
            If Conn.State <> adStateClosed Then Conn.Close
            bConnStatus = False
            Call Str2Log("Connection denied and closed.")
        Else
            Call Str2Log("Connection denied.")
        End If
        bConnPermission = vData
    End If
PityLabel:
End Property

Public Property Get ConnPermission() As Boolean
    ConnPermission = bConnPermission
End Property
  
Public Sub MantainDatabase()
    Dim je As New JRO.JetEngine
    Dim sSour As String, sDest As String
    
    Call Str2Log("MaintenanceDatabase started.")
    
    Me.ConnPermission = False
    If Me.ConnPermission Then   ' Didn't work, sorry
        Call Str2Log("Could not close connection, something went wrong.")
        GoTo MExit
    End If
   
    On Error Resume Next
    
    ' Make sure that a file doesn't exist with the name of the compacted database.
    If Dir(sDataBase + ".bak") <> "" Then Kill (sDataBase + ".bak")

    On Error GoTo JRO_Error
    
    Call Str2Log("MaintenanceDatabase:Compact & Repare started.")
    ' Compacts and repair database.
    sSour = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" & sDataBase & ";Jet OLEDB:Engine Type=4"
    sDest = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" & sDataBase & ".bak" & ";Jet OLEDB:Engine Type=4"
    Call je.CompactDatabase(sSour, sDest)
    
    Call Str2Log("MaintenanceDatabase: Change started.")
    Call Kill(sDataBase)
    Call FileCopy(sDataBase + ".bak", sDataBase)
    Call Kill(sDataBase + ".bak")

MExit:
    Call Str2Log("MaintenanceDatabase finished.")
    Me.ConnPermission = True
    Exit Sub
    
JRO_Error:
    Resume MExit
End Sub

Function SQLExecute(strCmdText As String) As Variant
    On Error GoTo SError

    If Not bConnPermission Then     '   Connection prohibited !!!
        SQLExecute = "Connection temporarily prohibited !!!"
        Exit Function
    End If
    
    If Not bConnStatus Then
        iI = 0
        Do Until bConnStatus
            ReConnect
            iI = iI + 1
            If iI > iRepQty Then
                SQLExecute = "Connection is broken!"
                Exit Function
            End If
        Loop
    End If
    
    Set cmdSQL = New ADODB.Command
    cmdSQL.ActiveConnection = Conn
    cmdSQL.CommandType = adCmdText
    cmdSQL.CommandText = strCmdText
    
    On Error GoTo SError
    Conn.Errors.Clear
    
    Set SQLExecute = cmdSQL.Execute

SExit:
    Set cmdSQL = Nothing
    Exit Function
    
SError:
    SQLExecute = "Error: " & Err.Description & " *** Query: " & strCmdText
    Str2Log (SQLExecute)
    If Conn.Errors.Count > 0 Then
        For Each errLoop In Conn.Errors
            Call Str2Log("   +++ ADO Error: " & errLoop.Number & _
            "   " & errLoop.Description & _
            "   (Source: " & errLoop.Source & ")" & _
            "   (SQL State: " & errLoop.SQLState & ")" & _
            "   (Native Error: " & errLoop.NativeError & ")")
            'RaiseEvent SomeError(errLoop.Description)
        Next
    End If
    Resume SExit
End Function

Private Sub Class_Initialize()
    Dim lResult As Long
    If Not SysIni Then
        MsgBox "Error loading LEIS system. Please check EDPS.INI file", vbCritical
        Exit Sub
    End If
    
    bConnPermission = True
    Set rstMain = New ADODB.Recordset
    Set Conn = New ADODB.Connection
    bConnStatus = False
    ReConnect

    n = 100: lResult = GetUserName(s, n): UserName = Left(s, n - 1)
    n = 100: lResult = GetComputerName(s, n): ComputerName = Left(s, n)
    
    Call Str2Log("LEIS.DB exampler created.")
End Sub

Private Sub Class_Terminate()
    If Not (rstMain Is Nothing) Then Set rstMain = Nothing
    If Not (Conn Is Nothing) Then
        If Conn.State <> adStateClosed Then Conn.Close
        Set Conn = Nothing
    End If
    
    Call Str2Log("LEIS.DB exampler destroyed.")
End Sub

Private Sub Conn_ConnectComplete(ByVal pError As ADODB.Error, adStatus As ADODB.EventStatusEnum, ByVal pConnection As ADODB.Connection)
    bConnStatus = True
    'Str2Log "Connection to SQL has been established!"
End Sub

Private Sub ReConnect()
    On Error GoTo RC_Error
    
    If Not bConnStatus Then
        Conn.Provider = "Microsoft.Jet.OLEDB.4.0"
        Conn.Open sDataBase, "admin", ""
    End If
    
    bConnStatus = True
RC_Exit:
    Exit Sub

RC_Error:
    bConnStatus = False
    Str2Log (Err.Description)
    If Conn.Errors.Count > 0 Then
        For Each errLoop In Conn.Errors
            Call Str2Log("   +++ ADO Error: " & errLoop.Number & _
            "   " & errLoop.Description & _
            "   (Source: " & errLoop.Source & ")" & _
            "   (SQL State: " & errLoop.SQLState & ")" & _
            "   (Native Error: " & errLoop.NativeError & ")")
            'RaiseEvent SomeError(errLoop.Description)
        Next
    End If
    Resume RC_Exit
End Sub

Private Sub Conn_Disconnect(adStatus As ADODB.EventStatusEnum, ByVal pConnection As ADODB.Connection)
    bConnStatus = False
    'Str2Log "Connection to SQL has been broken!"
End Sub

Private Sub Conn_InfoMessage(ByVal pError As ADODB.Error, adStatus As ADODB.EventStatusEnum, ByVal pConnection As ADODB.Connection)
    Str2Log "Some strange information from ADO connection, status: " + Str(adStatus) + ". Error: " + pError.Description
End Sub

Public Sub BeginTran()
    Conn.BeginTrans
End Sub

Public Sub CommitTran()
    Conn.CommitTrans
End Sub

Public Sub RollbackTran()
    Conn.RollbackTrans
End Sub
