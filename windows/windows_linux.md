# 在windows下开发linux程序需要注意的问题
1. windows和linux的文件换行符是不一样，windows:CRLF,linux:LF,附Wikipedia

        In computing, a newline, also known as a line ending, end of line (EOL), or line break, is a special character or sequence of characters signifying the end of a line of text and the start of a new line. The actual codes representing a newline vary across operating systems, which can be a problem when exchanging text files between systems with different newline representations.

        LF:    Unix and Unix-like systems (Linux, macOS, FreeBSD, Multics, AIX, Xenix, etc.), BeOS, Amiga, RISC OS, and others
        CR+LF: Microsoft Windows, DOS (MS-DOS, PC DOS, etc.), DEC TOPS-10, RT-11, CP/M, MP/M, Atari TOS, OS/2, Symbian OS, Palm OS, Amstrad CPC, and most other early non-Unix and non-IBM OSes
        CR:    Commodore 8-bit machines, Acorn BBC, ZX Spectrum, TRS-80, Apple II family, Oberon, the classic Mac OS up to version 9, MIT Lisp Machine and OS-9
        RS:    QNX pre-POSIX implementation
        0x9B:  Atari 8-bit machines using ATASCII variant of ASCII (155 in decimal)
        LF+CR: Acorn BBC and RISC OS spooled text output.

        The Unicode standard defines a number of characters that conforming applications should recognize as line terminators:

       LF:    Line Feed, U+000A
       VT:    Vertical Tab, U+000B
       FF:    Form Feed, U+000C
       CR:    Carriage Return, U+000D
       CR+LF: CR (U+000D) followed by LF (U+000A)
       NEL:   Next Line, U+0085
       LS:    Line Separator, U+2028
       PS:    Paragraph Separator, U+2029
2. windows和linux的纯文本都是不一样的，windows的会在纯文本中写入一些东西
3. window<=>linux需注意转换（unix2dos、dos2unix）








































































[上一级](base.md)
[上一篇](vmvare_windows.md)
[下一篇](zerobrane_VisualStudio_debug_linux.md)
