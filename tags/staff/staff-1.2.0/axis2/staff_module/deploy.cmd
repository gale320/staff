set projectname=staff_axis2_module

set deploydir=%cd%\..\..\deploy\win32
set libdir=%deploydir%\axis2\modules\staff

if not EXIST %libdir% mkdir %libdir%

xcopy /Y /S debug\*.dll %libdir%
xcopy /Y /S config\*.xml %libdir%

xcopy /Y /S debug\*.dll %AXIS2C_HOME%\modules\staff\
xcopy /Y /S config\*.xml %AXIS2C_HOME%\modules\staff\
