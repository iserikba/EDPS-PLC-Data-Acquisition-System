VERSION 5.00
Begin VB.Form frmAFDR 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "frmAFDR"
   ClientHeight    =   540
   ClientLeft      =   8490
   ClientTop       =   2190
   ClientWidth     =   1560
   ControlBox      =   0   'False
   Icon            =   "AFDR.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   540
   ScaleWidth      =   1560
   ShowInTaskbar   =   0   'False
   Begin VB.TextBox txtRWDDEVal 
      Height          =   285
      Left            =   720
      TabIndex        =   0
      Top             =   120
      Width           =   615
   End
   Begin VB.Timer AsRead 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   120
      Top             =   0
   End
End
Attribute VB_Name = "frmAFDR"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
