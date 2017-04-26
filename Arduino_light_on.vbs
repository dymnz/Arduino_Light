Set WshShell = CreateObject("WScript.Shell")
WshShell.Run chr(34) & "E:\Ethereun\PWM_light\Arduino_light_on.bat" & Chr(34), 0
Set WshShell = Nothing