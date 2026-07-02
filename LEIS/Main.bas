Attribute VB_Name = "modMain"
Option Explicit
Global DBInt As DB
Global sDataBase As String, sLogPath As String, iLogFile As Integer, iQueLength As Integer
Private Const strCrLf = vbCrLf
Private Const sIniFile = "EDPS.INI"
Private Const sApplicationName = "LEIS"
Global iRepQty As Integer

Declare Function GetUserName Lib "advapi32.dll" Alias "GetUserNameA" ( _
        ByVal lpBuffer As String, nSize As Long) As Long

Declare Function GetComputerName Lib "kernel32" Alias "GetComputerNameA" ( _
        ByVal lpBuffer As String, nSize As Long) As Long

Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" _
        (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, _
        ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long

Declare Function GetCurrentProcess Lib "kernel32" () As Long

Declare Function GetPriorityClass Lib "kernel32" (ByVal hProcess As Long) As Long

Declare Function SetPriorityClass Lib "kernel32" (ByVal hProcess As Long, _
                                            ByVal dwPriorityClass As Long) As Long
' Pause processing
Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

Public Sub Main()
    Set DBInt = New DB
End Sub

Function AlwayString(vParam As Variant) As String
    Dim iI As Integer, sTmp As String, sT As String
    
    Select Case VarType(vParam)
        Case vbString
            If InStr(1, vParam, "'") <> 0 Then  'Bloody hell!!!
                sTmp = ""
                For iI = 1 To Len(vParam)
                    sT = Mid(vParam, iI, 1)
                    If sT <> "'" Then
                        sTmp = sTmp + sT
                    End If
                Next iI
                vParam = sTmp
            End If
            AlwayString = Trim(vParam)
        Case vbInteger
            AlwayString = Trim(Str(vParam))
        Case vbLong
            AlwayString = Trim(Str(vParam))
        Case vbSingle
            AlwayString = Trim(Str(vParam))
        Case vbDouble
            AlwayString = Trim(Str(vParam))
        Case vbDecimal
            AlwayString = Trim(Str(vParam))
        Case vbDate
'            AlwayString = Format(vParam, "General Date")
'            AlwayString = Format(vParam, "mm-dd-yyyy hh:nn:ss")
            AlwayString = Format(vParam, "dd-mmm-yyyy hh:nn:ss")
        Case vbBoolean
            AlwayString = IIf(vParam, "True", "False")
        Case vbCurrency
            AlwayString = "$" + Trim(Str(vParam))
        Case vbNull
            AlwayString = "Null"
        Case Else
            AlwayString = "BLABALBLABLALABLA!!!!!!"
    End Select
End Function

Function AlwayString1(vParam As Variant) As String
    Select Case VarType(vParam)
        Case vbString
            AlwayString1 = "'" + Trim(vParam) + "'"
        Case vbInteger
            AlwayString1 = Trim(Str(vParam))
        Case vbLong
            AlwayString1 = Trim(Str(vParam))
        Case vbSingle
            AlwayString1 = Trim(Str(vParam))
        Case vbDouble
            AlwayString1 = Trim(Str(vParam))
        Case vbDecimal
            AlwayString1 = Trim(Str(vParam))
        Case vbDate
            AlwayString1 = Format(vParam, "mm-dd-yyyy hh:nn:ss")
        Case vbBoolean
            AlwayString1 = IIf(vParam, "True", "False")
        Case vbCurrency
            AlwayString1 = "$" + Trim(Str(vParam))
        Case vbNull
            AlwayString1 = "Null"
        Case Else
            AlwayString1 = ""
    End Select
End Function

Sub Str2Log(sStr As String)
On Error Resume Next
    iLogFile = FreeFile
    Open (sLogPath) For Append As iLogFile
    Print #iLogFile, "LEIS:    " & Format(Now, "dd-mmm-yyyy hh:nn:ss"); Spc(2); sStr; strCrLf
    Close #iLogFile
End Sub

Function ReadIniKey(ByRef sIniFile As String, ByRef sSection As String, ByRef sKey As String) As String
    
    Dim sDefault As String, sReturnedString As String, iSize As Long, iRetCode As Long
    
    sDefault = "None"
    iSize = 255
    sReturnedString = Space(iSize)
    
    iRetCode = GetPrivateProfileString(sSection, sKey, sDefault, _
                sReturnedString, iSize, sIniFile)
    ReadIniKey = Left(sReturnedString, iRetCode)
End Function

Function SysIni() As Boolean
    Dim sFileName As String, iTmp As Integer
    
    On Error GoTo SysIni_error
    
    SysIni = True
    sFileName = App.Path & "\" & sIniFile
    sLogPath = App.Path & "\EDPS.LOG"

    sDataBase = Trim(ReadIniKey(sFileName, sApplicationName, "DataBase"))

    iRepQty = CInt(ReadIniKey(sFileName, sApplicationName, "TryRepeats"))

    iQueLength = CInt(ReadIniKey(sFileName, sApplicationName, "QueueLength"))

    iTmp = CInt(ReadIniKey(sFileName, sApplicationName, "PriorityRun"))
    If iTmp <> 0 Then
        Call SetPriority(iTmp)
    End If

Endapp:
Exit Function

SysIni_error:
    Call Str2Log("SysIni " & Format(Now, "dd-mmm-yyyy hh:nn:ss") & " Error: " & _
            Str(Err.Number) & " Descr: " & Err.Description)
    SysIni = False
    Resume Endapp
End Function

Sub SetPriority(ByVal lNewPriority As Long)
Dim lProcess As Long, lPriority As Long, lResult As Long

    lProcess = GetCurrentProcess()
    lResult = SetPriorityClass(lProcess, lNewPriority)
    If lResult = 0 Then
        lResult = Err.LastDllError
    End If
    lPriority = GetPriorityClass(lProcess)
    If lPriority <> lNewPriority Then
        Call Str2Log("SetPriority " & Format(Now, "dd-mmm-yyyy hh:nn:ss") & " Current priority: " & _
            Str(lPriority) & " Unable to set: " & Str(lNewPriority) & " DLL error: " & Str(lResult))
    End If
End Sub

Function CheckPres(vArray As Variant, iColumn As Integer, vValue As Variant) As Boolean
    Dim iL As Integer
    On Error GoTo Check_error
    
    For iL = LBound(vArray, 2) To UBound(vArray, 2)
        If vArray(iColumn, iL) = vValue Then
            CheckPres = True
            Exit Function
        End If
    Next iL
    CheckPres = False

Check_exit:
    Exit Function

Check_error:
    CheckPres = False
    Resume Check_exit
End Function

