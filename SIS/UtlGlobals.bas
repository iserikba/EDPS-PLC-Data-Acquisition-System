Attribute VB_Name = "modUtlGlobals"
Option Explicit

'General Info
Declare Function GetUserName Lib "advapi32.dll" Alias "GetUserNameA" ( _
        ByVal lpBuffer As String, nSize As Long) As Long
Declare Function GetComputerName Lib "kernel32" Alias "GetComputerNameA" ( _
        ByVal lpBuffer As String, nSize As Long) As Long

' Read/Write params in ini file
Declare Function GetPrivateProfileString Lib "kernel32" Alias "GetPrivateProfileStringA" _
        (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, _
        ByVal lpReturnedString As String, ByVal nSize As Long, ByVal lpFileName As String) As Long
Declare Function WritePrivateProfileString Lib "kernel32" Alias "WritePrivateProfileStringA" _
    (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As Any, _
    ByVal lpFileName As String) As Long

' Process priority
Declare Function GetCurrentProcess Lib "kernel32" () As Long
Declare Function GetPriorityClass Lib "kernel32" (ByVal hProcess As Long) As Long
Declare Function SetPriorityClass Lib "kernel32" (ByVal hProcess As Long, _
                                            ByVal dwPriorityClass As Long) As Long

' Pause processing
Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)
Private Const strCrLf = vbCrLf

' Queue functions
Public Enum QueueID
    Messages = 0
    ToCalculate = 1
End Enum

Declare Function qm_pop Lib "qm.dll" _
    (ByVal iQNum As QueueID) As String
Declare Function qm_push Lib "qm.dll" _
    (ByVal iQNum As QueueID, ByVal str As String) As Long
Declare Function qm_pushex Lib "qm.dll" _
    (ByVal iQNum As QueueID, ByVal str As String) As Long
Declare Function qm_queues Lib "qm.dll" () As Long
Declare Function qm_length Lib "qm.dll" (ByVal iQNum As QueueID) As Long

' SAPI functions
Declare Function SAPI_Read Lib "OpcS7.dll" (ByVal sCP As String, ByVal sConn As String, ByVal sItem As String, ByVal sType As String, ByRef sData As String) As Long
Declare Function SAPI_Write Lib "OpcS7.dll" (ByVal sCP As String, ByVal sConn As String, ByVal sItem As String, ByVal sType As String, ByVal sData As String) As Long
Declare Function SAPI_MRead Lib "OpcS7.dll" (ByVal sCP As String, ByRef sData As String) As Long
Declare Function SAPI_MWrite Lib "OpcS7.dll" (ByVal sCP As String) As Long
Declare Function SAPI_Shut Lib "OpcS7.dll" () As Long

' MUTEX
Declare Function CreateMutex Lib "kernel32" Alias "CreateMutexA" _
    (lpMutexAttributes As Any, ByVal bInitialOwner As Long, ByVal lpName As String) As Long
Declare Function WaitForSingleObject Lib "kernel32" _
    (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long
Declare Function ReleaseMutex Lib "kernel32" (ByVal hMutex As Long) As Long
Type SECURITY_ATTRIBUTES
        nLength As Long
        lpSecurityDescriptor As Long
        bInheritHandle As Long
End Type
Global Const MutexTimeOut As Long = 100

Function ReadIniKey(ByRef sIniFile As String, _
        ByRef sSection As String, ByRef sKey As String, _
        Optional ByRef sDefault As String = "None") As String

    Dim sReturnedString As String, iSize As Long, iRetCode As Long
    
    iSize = 255
    sReturnedString = Space(iSize)
    
    iRetCode = GetPrivateProfileString(sSection, sKey, sDefault, _
                sReturnedString, iSize, sIniFile)
    ReadIniKey = Left(sReturnedString, iRetCode)

End Function

Sub SaveIniKey(sInFile As String, sSection As String, _
                        sKey As String, sNewVal As String)
    Dim lRetCode As Long
    lRetCode = WritePrivateProfileString(sSection, sKey, sNewVal, sInFile)
    If lRetCode = 0 Then
    
    End If
End Sub


Sub SetPriority(ByVal lNewPriority As Long, Optional ByVal sLogFile As String)
    Dim lProcess As Long, lPriority As Long, lResult As Long

    lProcess = GetCurrentProcess()
    lResult = SetPriorityClass(lProcess, lNewPriority)
    If lResult = 0 Then
        lResult = Err.LastDllError
    End If
    lPriority = GetPriorityClass(lProcess)
    If lPriority <> lNewPriority Then
        Call Str2File(sLogFile, "SetPriority " & _
            Format(Now, "yyyy/MM/dd hh:mm:ss") & " Current priority: " & _
            str(lPriority) & " Unable to set: " & str(lNewPriority) & _
            " DLL error: " & str(lResult))
    End If
End Sub

Function GetPriority() As Long
    Dim lProcess As Long

    lProcess = GetCurrentProcess()
    GetPriority = GetPriorityClass(lProcess)
End Function

Sub Str2File(sFileName As String, sStr As String)
    Dim iFileNumber As Integer, iErrCount As Integer
    
    On Error GoTo Str2File_error
    
    sStr = "SIS:     " & Format(Now, "dd-mmm-yyyy hh:mm:ss ") & sStr
    iErrCount = 0

Str2File_cont:
    iFileNumber = FreeFile
    Open (sFileName) For Append As iFileNumber
    Print #iFileNumber, sStr; strCrLf

    Close #iFileNumber

Str2File_exit:
    Exit Sub

Str2File_error:
    If iErrCount = 0 Then
        iErrCount = iErrCount + 1
        sFileName = Left(sFileName, Len(sFileName) - 4) + _
            Trim(Format(Now(), "yyyyMMddhhmmss")) + _
            Right(sFileName, 4)
        Close
        Resume Str2File_cont
    Else
        Close
        Resume Str2File_exit
    End If
End Sub

Public Function AlwayString(vParam As Variant) As String
    Select Case VarType(vParam)
        Case vbString
            AlwayString = vParam
        Case vbInteger
            AlwayString = Trim(str(vParam))
        Case vbLong
            AlwayString = Trim(str(vParam))
        Case vbSingle
            AlwayString = Trim(str(vParam))
        Case vbDouble
            AlwayString = Trim(str(vParam))
        Case vbDecimal
            AlwayString = Trim(str(vParam))
        Case vbDate
            AlwayString = Format(vParam, "dd-mmm-yyyy hh:nn:ss")
        Case vbBoolean
            AlwayString = IIf(vParam, "True", "False")
        Case vbCurrency
            AlwayString = "$" + Trim(str(vParam))
        Case vbNull
            AlwayString = ""
        Case Else
            AlwayString = ""
    End Select
End Function

Public Sub Str2Log(ByVal sStr As String)
    qm_push Messages, sStr
End Sub
