Attribute VB_Name = "modMain"
Option Explicit

' Queue functions
Declare Function qm_pop Lib "qm.dll" _
    (ByVal iQNum As Long) As String
Declare Function qm_push Lib "qm.dll" _
    (ByVal iQNum As Long, ByVal Str As String) As Long

Global Const sQUEUEMPTY As String = "Queue empty!"

Global Const RESULTQUEUE As Integer = 5

