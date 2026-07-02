VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
END
Attribute VB_Name = "cFDI"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
Option Explicit

Private Const Queue As Long = 5
Private sLogFile As String

Public Function DoItNow(vFDPTR As String) As Boolean
    DoItNow = (qm_push(Queue, vFDPTR) = 0)
End Function

Public Function GetResult() As String
    GetResult = qm_pop(RESULTQUEUE)
End Function

Public Property Get PriorityRun() As Variant
    PriorityRun = GetPriority()
End Property

Public Property Let PriorityRun(ByVal vNewValue As Variant)
    Call SetPriority(vNewValue, sLogFile)
End Property

Public Property Get LogPath() As Variant
    LogPath = sLogFile
End Property

Public Property Let LogPath(ByVal vNewValue As Variant)
    sLogFile = vNewValue + App.EXEName
End Property
