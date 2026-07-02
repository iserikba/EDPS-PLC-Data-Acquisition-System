Attribute VB_Name = "modMain"
Option Explicit

Public Sub Main()
    Dim sVal As String

    If App.PrevInstance Then
        MsgBox "Application " + App.EXEName + " is already running! Exit.", vbCritical + vbOKOnly
        End
    End If
    sLogFile = App.Path + "\EDPS.LOG"
    
    lEDPSid = CLng(ReadIniKey(App.Path + "\" + APPINIFILE, _
        APPINISECT, EDPSID, "0"))
    
    sVal = ReadIniKey(App.Path + "\" + APPINIFILE, _
        APPINISECT, SHOWFORMSKEY, "0")
    
    iShowForms = CInt(sVal)
    
    If iShowForms > 0 Then
        frmMain.Show
    Else
        Load frmMain
    End If
End Sub
