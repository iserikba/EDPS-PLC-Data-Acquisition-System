VERSION 5.00
Begin VB.Form frmScheduler 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Form1"
   ClientHeight    =   705
   ClientLeft      =   6660
   ClientTop       =   1605
   ClientWidth     =   1560
   ControlBox      =   0   'False
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   705
   ScaleWidth      =   1560
   ShowInTaskbar   =   0   'False
   Begin VB.Timer tTimer 
      Enabled         =   0   'False
      Interval        =   1000
      Left            =   360
      Top             =   0
   End
End
Attribute VB_Name = "frmScheduler"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
