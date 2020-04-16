set /a index=0

:loop

set /a index=%index%+1

fc data%index%.ans test%index%.ans
::pause
if %index%==10 goto end
goto loop

:end