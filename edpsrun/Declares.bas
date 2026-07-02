Attribute VB_Name = "modDeclares"
Option Explicit
' Constants
Public Const APPINIFILE As String = "edps.ini"
Public Const APPINISECT As String = "SIS"

Public Const WORKTHKEY As String = "WorkThreads"
Public Const APPPRIORITYKEY As String = "PriorityRun"
Public Const EDPSID As String = "EDPSID"
Public Const SHOWFORMSKEY As String = "ShowLogForms "
Public Const SCHEDSTEPKEY As String = "SchedulerStep"
Public Const CALCSTEPKEY As String = "CalculatorStep"
Public Const AFDRSTEPKEY As String = "AFDRStep"

Public Const COMMINISECT As String = "COMMON"
Public Const DATASRCNAMKEY As String = "DataSource"
Public Const DBNAMEKEY As String = "InitialCatalog"
Public Const USERNAMEKEY As String = "UID"
Public Const USERPWDKEY As String = "PWD"
Public Const NETLIBKEY As String = "NetLib"

Public Const DATASRCNAMP As String = "Data Source"
Public Const DBNAMEP As String = "Initial Catalog"
Public Const USERNAMEP As String = "User Id"
Public Const USERPWDP As String = "Password"
Public Const NETLIBP As String = "Network Library"

' Startup parameters
Public sLogFile As String
Public iShowForms As Integer
Public lEDPSid As Long
