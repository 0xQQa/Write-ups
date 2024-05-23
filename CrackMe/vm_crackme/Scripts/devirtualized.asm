format PE console
include 'win32ax.inc'

.code
        start:
                call    POC_GetArg1
                mov     esi, eax
                call    POC_DevirtualizedCode
                cmp     ebx, 0
                jnz     sad_result
                call    POC_ValidPass
        sad_result:
                invoke  ExitProcess, 0

        POC_DevirtualizedCode:
                file    'devirtualized.bin'

        POC_GetArg1:
                invoke  GetCommandLineA
                mov     edx, eax
                invoke  lstrlenA, eax
                add     eax, edx
        look_prev:
                dec     eax
                movzx   ebx, byte [eax-1]
                cmp     ebx, ' '
                jnz     look_prev
                ret

        POC_ValidPass:
                invoke  GetStdHandle, -11
                invoke  WriteConsoleA, eax, msg, msg_len, 0, 0
                ret
                msg db  "Nice!!! U got it!", 10, 0
                msg_len = $ - msg

.end start
