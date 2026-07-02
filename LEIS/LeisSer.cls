VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "LEISer"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Attribute VB_Ext_KEY = "SavedWithClassBuilder" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit

Private LDB As DB
Private strCMD As String
Private vTV As Variant
Private iI As Integer
Private iJ As Integer

Public Event SomeError(sDesc As String)

Public Sub Maintenance()
    Call KillMasterSent
    Call LDB.MantainDatabase
End Sub

Public Sub DenyConnection()
    LDB.ConnPermission = False
End Sub

Public Sub AllowConnection()
    LDB.ConnPermission = True
End Sub

Public Function MasterSQLDone(vHistID As Variant) As Variant
    strCMD = "UPDATE tblHistData SET tblHistData.MasterDone = True " + _
            "WHERE tblHistData.Hist_ID = " + AlwayString(vHistID)

    vTV = LDB.SQLExecute(strCMD)
    MasterSQLDone = IIf(VarType(vTV) <> vbString, "", vTV)
End Function

Public Function KillMasterSent() As Variant
    strCMD = "DELETE tblHistData.* FROM tblHistData WHERE tblHistData.MasterDone = True"

    vTV = LDB.SQLExecute(strCMD)
    KillMasterSent = IIf(VarType(vTV) <> vbString, "", vTV)
End Function

Private Function WriteFormulas(vParameters As Variant) As Boolean
    On Error Resume Next
    Dim vTa As Variant
    
    On Error GoTo Wf_err
    strCMD = "DELETE * FROM tblFormula"
    vTa = LDB.SQLExecute(strCMD)
    
    iJ = LBound(vParameters)
    iI = LBound(vParameters, 2)
    
    For iI = LBound(vParameters, 2) To UBound(vParameters, 2)
        strCMD = "INSERT INTO tblFormula (Formula_ID, TimeLoop, Description) " + _
                "VALUES (" + _
                AlwayString1(vParameters(iJ, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 1, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 2, iI)) + ")"
        
        vTa = LDB.SQLExecute(strCMD)
        If VarType(vTa) = vbString Then
            If Left(vTa, 6) = "Error:" Then
                WriteFormulas = False
                GoTo Wf_exit
            End If
        End If
    Next iI
    WriteFormulas = True
    
Wf_exit:
    Exit Function
    
Wf_err:
    WriteFormulas = False
    Resume Wf_exit
    
End Function

Private Function WriteOperations(vParameters As Variant) As Boolean
    On Error Resume Next
    Dim vTa As Variant
    
    On Error GoTo Wo_error
    strCMD = "DELETE * FROM tblOperation"
    vTa = LDB.SQLExecute(strCMD)
    
    iJ = LBound(vParameters)
    For iI = LBound(vParameters, 2) To UBound(vParameters, 2)
        strCMD = "INSERT INTO tblOperation (Formula_ID, Operation_ID, Function, " + _
                "Oper1, Type1, Oper2, Type2, Oper3, Type3, Description) " + _
                "VALUES (" + _
                AlwayString1(vParameters(iJ, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 1, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 2, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 3, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 4, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 5, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 6, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 7, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 8, iI)) + ", " + _
                AlwayString1(vParameters(iJ + 9, iI)) + ")"
        
        vTV = LDB.SQLExecute(strCMD)
        If VarType(vTV) = vbString Then
            If Left(vTV, 6) = "Error:" Then
                WriteOperations = False
                GoTo Wo_exit
            End If
        End If
    Next iI
    WriteOperations = True
    
Wo_exit:
    Exit Function
    
Wo_error:
    WriteOperations = False
    Resume Wo_exit
    
End Function

Private Function WriteParameters(vParameters As Variant) As Boolean
    On Error Resume Next
    Dim bEmpty As Boolean
    Dim vTa As Variant
    
    ' Obtain previous parameters' information.
    vTa = ReadParameters
    bEmpty = (VarType(vTa) = vbEmpty)
    
    On Error GoTo Wp_err
    iJ = LBound(vParameters)
    For iI = LBound(vParameters, 2) To UBound(vParameters, 2)
        If Not bEmpty And CheckPres(vTa, iJ, vParameters(iJ, iI)) Then
            strCMD = "UPDATE tblPhysPar SET " + _
                    "Type = " + AlwayString1(vParameters(iJ + 1, iI)) + ", " + _
                    "PhysType = " + AlwayString1(vParameters(iJ + 2, iI)) + ", " + _
                    "DataSetName = " + AlwayString1(vParameters(iJ + 3, iI)) + ", " + _
                    "AddrStr1 = " + AlwayString1(vParameters(iJ + 4, iI)) + ", " + _
                    "AddrStr2 = " + AlwayString1(vParameters(iJ + 5, iI)) + ", " + _
                    "AddrStr3 = " + AlwayString1(vParameters(iJ + 6, iI)) + ", " + _
                    "AddrStr4 = " + AlwayString1(vParameters(iJ + 7, iI)) + ", " + _
                    "AddrStr5 = " + AlwayString1(vParameters(iJ + 8, iI)) + ", " + _
                    "AddrStr6 = " + AlwayString1(vParameters(iJ + 9, iI)) + _
                    " WHERE tblPhysPar.PhysPar_ID = " + AlwayString1(vParameters(iJ, iI))
        Else
            strCMD = "INSERT INTO tblPhysPar (PhysPar_ID, Type, PhysType, DataSetName, " + _
                    "AddrStr1, AddrStr2, AddrStr3, AddrStr4, AddrStr5, AddrStr6) VALUES (" + _
                    AlwayString1(vParameters(iJ, iI)) + ", " + _
                    AlwayString1(vParameters(iJ + 1, iI)) + ", " + _
                    AlwayString1(vParameters(iJ + 2, iI)) + ", " + _
                    AlwayString1(vParameters(iJ + 3, iI)) + ", " + _
                    AlwayString1(vParameters(iJ + 4, iI)) + ", " + _
                    AlwayString1(vParameters(iJ + 5, iI)) + ", " + _
                    AlwayString1(vParameters(iJ + 6, iI)) + ", " + _
                    AlwayString1(vParameters(iJ + 7, iI)) + ", " + _
                    AlwayString1(vParameters(iJ + 8, iI)) + ", " + _
                    AlwayString1(vParameters(iJ + 9, iI)) + ")"
        End If
        
        vTV = LDB.SQLExecute(strCMD)
        If VarType(vTV) = vbString Then
            If Left(vTV, 6) = "Error:" Then
                WriteParameters = False
                GoTo Wp_exit
            End If
        End If
    Next iI
    WriteParameters = True
    
Wp_exit:
    Exit Function
    
Wp_err:
    WriteParameters = False
    Resume Wp_exit
    
End Function

Public Function SlaveSQLDone(vHistID As Variant) As Variant
    strCMD = "UPDATE tblHistData SET tblHistData.SlaveDone = True " + _
            "WHERE tblHistData.Hist_ID = " + Trim(Str(vHistID))

    vTV = LDB.SQLExecute(strCMD)
    SlaveSQLDone = IIf(VarType(vTV) <> vbString, "", vTV)
End Function

Public Function WriteQueue(iParamID As Integer, dProcTime As Date, _
    sActValue As String, iRunID As Long, Optional dEvenTime As Date = #2/18/68#) As Variant
    
'    Str2Log sActValue
'
    If dEvenTime = #2/18/68# Then
        strCMD = "INSERT INTO tblHistData (PhysPar_ID, ProcTime, ActValue, RunID) VALUES (" + _
        AlwayString(iParamID) + ", '" + AlwayString(dProcTime) + "', '" + AlwayString(sActValue) + "', '" + _
        AlwayString(iRunID) + "')"
    Else
        strCMD = "INSERT INTO tblHistData (PhysPar_ID, ProcTime, ActValue, RunID, EvenTime) VALUES (" + _
        AlwayString(iParamID) + ", '" + AlwayString(dProcTime) + "', '" + _
        AlwayString(sActValue) + "', '" + AlwayString(iRunID) + "', '" + _
        AlwayString(dEvenTime) + "')"
    End If
    
    vTV = LDB.SQLExecute(strCMD)
    WriteQueue = IIf(VarType(vTV) <> vbString, "", vTV)
End Function

Public Function ReadQueue(sSQL As String) As Variant
    On Error Resume Next
    
    strCMD = "SELECT TOP " + Trim(Str(iQueLength)) + " A1.Hist_ID, A1.PhysPar_ID, " + _
            "A1.ProcTime, A1.ActValue, " + _
            "A1.RunID, A1.EvenTime, A1.MasterDone, A1.SlaveDone, A2.DataSetName " + _
            "FROM tblHistData as A1, tblPhysPar as A2 WHERE " + _
            "A1.PhysPar_ID = A2.PhysPar_ID AND " + _
            IIf(UCase(sSQL) = "MASTER", "MasterDone = False", "SlaveDone = False")
    ReadQueue = LDB.SQLExecute(strCMD).GetRows
End Function

Public Function ReadParameters() As Variant
    On Error Resume Next
    
    strCMD = "SELECT PhysPar_ID, Type, PhysType, DataSetName, AddrStr1, " + _
            "AddrStr2, AddrStr3, AddrStr4, AddrStr5, AddrStr6 " + _
            "FROM tblPhysPar ORDER BY PhysPar_ID"
    ReadParameters = LDB.SQLExecute(strCMD).GetRows
End Function

Public Function ReadFormulas() As Variant
    On Error Resume Next
    
    strCMD = "SELECT Formula_ID, TimeLoop, Description " + _
            "FROM tblFormula"
    ReadFormulas = LDB.SQLExecute(strCMD).GetRows
End Function

Public Function ReadOperations() As Variant
    On Error Resume Next
    
    strCMD = "SELECT o.Formula_ID, o.Operation_ID, o.Function, " + _
            "o.Oper1, o.Type1, o.Oper2, o.Type2, o.Oper3, o.Type3, o.Description " + _
            "FROM tblOperation o, tblFormula f WHERE f.Formula_ID = o.Formula_ID " + _
            "ORDER BY o.Formula_ID, o.Operation_ID"
    ReadOperations = LDB.SQLExecute(strCMD).GetRows
End Function

Public Function WriteSISInfo(vParameters As Variant, vFormulas As Variant, _
                            vOperations As Variant) As Boolean
    On Error GoTo Us_error
    
    LDB.BeginTran
    
    On Error GoTo Roll
    
    If Not WriteParameters(vParameters) Then
        LDB.RollbackTran
        WriteSISInfo = False
        GoTo WriteS_exit
    End If
    
    If Not WriteFormulas(vFormulas) Then
        LDB.RollbackTran
        WriteSISInfo = False
        GoTo WriteS_exit
    End If
    
    If Not WriteOperations(vOperations) Then
        LDB.RollbackTran
        WriteSISInfo = False
        GoTo WriteS_exit
    End If
    
    LDB.CommitTran
    WriteSISInfo = True
    
WriteS_exit:
    Exit Function
    
Us_error:
    WriteSISInfo = False
    Resume WriteS_exit
    
Roll:
    LDB.RollbackTran
    WriteSISInfo = False
    Resume WriteS_exit
End Function

Private Sub Class_Initialize()
    Set LDB = DBInt
    Call Str2Log("LEISer exampler created.")
End Sub

Private Sub Class_Terminate()
    If Not (LDB Is Nothing) Then Set LDB = Nothing
    Call Str2Log("LEISer exampler destroyed.")
End Sub

