extern _fopen
extern _freopen
extern _fclose
extern _remove
extern _rename
extern _feof
extern _ferror
extern _fflush
extern _clearerr
extern _fseek
extern _ftell
extern _rewind
extern _fread
extern _fwrite
extern _fgetc
extern _getchar
extern _ungetc
extern _fputc
extern _putc
extern _putchar
extern _fgets
extern _fputs
extern _puts
extern _perror
extern _glClear
extern _glClearColor
extern _glBegin
extern _glEnd
extern _glFinish
extern _glBlendFunc
extern _glEnable
extern _glDisable
extern _glGenTextures
extern _glDeleteTextures
extern _glTexParameteri
extern _glTexParameterf
extern _glBindTexture
extern _glLoadIdentity
extern _glViewport
extern _glOrtho
extern _glColor3f
extern _glColor4f
extern _glVertex2f
extern _glVertex3f
extern _glGetError
extern _glGetString
extern _glGetIntegerv
extern _glTexCoord2f
extern _glTexImage2D
extern _glGenBuffers
extern _glGenVertexArrays
extern _glGenSamplers
extern _glDeleteBuffers
extern _glBindBuffer
extern _glBindVertexArray
extern _glDeleteVertexArrays
extern _glBindSampler
extern _glBufferData
extern _glBufferSubData
extern _glDrawArrays
extern _glDrawElements
extern _glMapBuffer
extern _glUnmapBuffer
extern _VertexAttribPointer
extern _EnableVertexAttribArray
extern _glCreateShader
extern _glShaderSource
extern _glCompileShader
extern _glCreateProgram
extern _glAttachShader
extern _glDetachShader
extern _glDeleteShader
extern _glLinkProgram
extern _glUseProgram
extern _glDeleteProgram
extern _glGetShaderiv
extern _glGetProgramiv
extern _glGetShaderInfoLog
extern _glGetProgramInfoLog
extern _glActiveTexture
extern _glGenerateMipmap
extern _glSamplerParameteri
extern _glSamplerParameterf
extern _glSamplerParameteriv
extern _glSamplerParameterfv
extern _glSamplerParameterIiv
extern _glSamplerParameterIuiv
extern _glUniform1i
extern _glUniform2i
extern _glUniform3i
extern _glUniform4i
extern _glUniform1f
extern _glUniform2f
extern _glUniform3f
extern _glUniform4f
extern _glUniformMatrix4fv
extern _glGetUniformLocation
extern _glewInit
extern _glewIsSupported
extern _glewGetExtension
extern _glewGetErrorString
extern _glewGetString
extern _glfwCreateWindow
extern _glfwInit
extern _glfwTerminate
extern _glfwGetVersion
extern _glfwGetPrimaryMonitor
extern _glfwGetMonitors
extern _glfwGetMonitorPos
extern _glfwGetMonitorPhysicalSize
extern _glfwGetVideoMode
extern _glfwSetGamma
extern _glfwGetGammaRamp
extern _glfwSetGammaRamp
extern _glfwDestroyWindow
extern _glfwWindowHint
extern _glfwDefaultWindowHints
extern _glfwWindowShouldClose
extern _glfwSetWindowShouldClose
extern _glfwSwapInterval
extern _glfwSwapBuffers
extern _glfwMakeContextCurrent
extern _glfwGetCurrentContext
extern _glfwGetProcAddress
extern _glfwExtensionSupported
extern _glfwGetKey
extern _glfwSetWindowTitle
extern _glfwSetWindowIcon
extern _glfwSetWindowPos
extern _glfwSetWindowSizeLimits
extern _glfwSetWindowAspectRatio
extern _glfwSetWindowSize
extern _glfwGetWindowPos
extern _glfwGetWindowSize
extern _glfwGetFramebufferSize
extern _glfwGetWindowFrameSize
extern _glfwIconifyWindow
extern _glfwRestoreWindow
extern _glfwMaximizeWindow
extern _glfwShowWindow
extern _glfwHideWindow
extern _glfwFocusWindow
extern _glfwGetWindowMonitor
extern _glfwSetWindowMonitor
extern _glfwGetWindowAttrib
extern _glfwSetWindowUserPointer
extern _glfwGetWindowUserPointer
extern _glfwPollEvents
extern _glfwWaitEvents
extern _glfwWaitEventsTimeout
extern _glfwPostEmptyEvent
extern _glfwGetInputMode
extern _glfwSetInputMode
extern _glfwGetMouseButton
extern _glfwGetCursorPos
extern _glfwSetCursorPos
extern _glfwCreateCursor
extern _glfwDestroyCursor
extern _glfwSetCursor
extern _glfwCreateStandardCursor
extern _glfwGetJoystickAxes
extern _glfwGetJoystickButtons
extern _glfwSetClipboardString
extern _glfwGetTime
extern _glfwSetTime
extern _glfwGetTimerValue
extern _glfwGetTimerFrequency
extern _glfwGetRequiredInstanceExtensions
section .data
	D0 DQ `GLFW init failed.`, 0 
	D1 DQ `Hello Triangle`, 0 
	D2 DQ `GLEW init failed.`, 0 
	D3 DD 1.000000
	D4 DD 0.010000
	D5 DD 0.010000
	D6 DD 0.010000
	D7 DD 0.000000
	D8 DD 0.000000
	D9 DD 1.000000
	D10 DD 0.001000
	D11 DD 0.001000
	D12 DD 0.001000
	D13 DD 0.001000
	D14 DD 0.001000
	D15 DD 0.001000
	D16 DD 0.000000
	D17 DD 0.000000
	D18 DD 1.000000
	D19 DD 0.500000
	D20 DD 0.500000
	D21 DD 0.000000
	D22 DD 1.000000
	D23 DD 0.000000
	D24 DD 0.500000
	D25 DD 0.000000
	D26 DD 1.000000
	D27 DD 0.000000
	D28 DD 0.000000
	D29 DD 0.500000
	D30 DD 0.500000
	D31 DD 0.500000
	D32 DD 0.000000
	D33 DD 0.000000
	D34 DD 0.000000
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 32; 24 alloc, 8 padding
.BEGIN:
	CALL _glfwInit
	CMP AL, 0
	SETE AL
	CMP AL, 0
	JE .L1
	MOV RAX, D0; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	JMP .L1
.L0:
.L1:
	MOV EAX, 2
	PUSH RAX
	MOV EAX, 139266
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwWindowHint
	MOV EAX, 1
	PUSH RAX
	MOV EAX, 139267
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwWindowHint
	MOV EAX, 4
	PUSH RAX
	MOV EAX, 135181
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwWindowHint
	MOV EAX, 8
	PUSH RAX
	MOV EAX, 135169
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwWindowHint
	MOV EAX, 8
	PUSH RAX
	MOV EAX, 135170
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwWindowHint
	MOV EAX, 8
	PUSH RAX
	MOV EAX, 135171
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwWindowHint
	MOV EAX, 8
	PUSH RAX
	MOV EAX, 135172
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwWindowHint
	MOV EAX, 0
	PUSH RAX
	MOV EAX, 0
	PUSH RAX
	MOV RAX, D1; string_ref
	PUSH RAX
	MOV EAX, 480
	PUSH RAX
	MOV EAX, 640
	PUSH RAX
	POP RDI
	POP RSI
	POP RDX
	POP RCX
	POP R8
	CALL _glfwCreateWindow
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	POP RAX
	MOV [RBP-8], RAX; store
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	CALL _glfwMakeContextCurrent
	CALL _glfwInit
	PUSH RAX
	MOV EAX, 0
	POP RCX
	CMP RCX, RAX
	SETNE AL
	CMP AL, 0
	JE .L3
	MOV RAX, D2; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	JMP .L3
.L2:
.L3:
	MOV RAX, D3; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D4; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D5; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D6; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM2, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM3, DWORD [RSP]
	ADD RSP, 8
	CALL _glClearColor
	MOV EAX, 36281
	PUSH RAX
	POP RDI
	CALL _glEnable
	MOV RAX, D7; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-12], XMM0; store
	MOV RAX, D8; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-16]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-16], XMM0; store
	MOV RAX, D9; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-20]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-20], XMM0; store
	MOV EAX, 1
	PUSH RAX
	MOV EAX, DWORD [RBP-24]; load
	POP RAX
	MOV [RBP-24], EAX; store
.L4:
	MOV EAX, DWORD [RBP-24]; load
	CMP AL, 0
	JE .L5
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	CALL _glfwWindowShouldClose
	CMP AL, 0
	JE .L7
	MOV EAX, 0
	PUSH RAX
	MOV EAX, DWORD [RBP-24]; load
	POP RAX
	MOV [RBP-24], EAX; store
	JMP .L7
.L6:
.L7:
	MOV EAX, 256
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwGetKey
	PUSH RAX
	MOV EAX, 1
	POP RCX
	CMP RCX, RAX
	SETE AL
	CMP AL, 0
	JE .L9
	MOV EAX, 0
	PUSH RAX
	MOV EAX, DWORD [RBP-24]; load
	POP RAX
	MOV [RBP-24], EAX; store
	JMP .L9
.L8:
.L9:
	MOV EAX, 16384
	PUSH RAX
	POP RDI
	CALL _glClear
	CALL _glfwPollEvents
	MOV EAX, 263
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwGetKey
	PUSH RAX
	MOV EAX, 1
	POP RCX
	CMP RCX, RAX
	SETE AL
	CMP AL, 0
	JE .L11
	MOV RAX, D10; float_ref
	MOVSS XMM0, [RAX]; float_ref
	MOVD ECX, XMM0
	XOR ECX, 2147483648
	MOVD XMM0, ECX
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-12], XMM0; store
	JMP .L11
.L10:
.L11:
	MOV EAX, 262
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwGetKey
	PUSH RAX
	MOV EAX, 1
	POP RCX
	CMP RCX, RAX
	SETE AL
	CMP AL, 0
	JE .L13
	MOV RAX, D11; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-12], XMM0; store
	JMP .L13
.L12:
.L13:
	MOV EAX, 264
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwGetKey
	PUSH RAX
	MOV EAX, 1
	POP RCX
	CMP RCX, RAX
	SETE AL
	CMP AL, 0
	JE .L15
	MOV RAX, D12; float_ref
	MOVSS XMM0, [RAX]; float_ref
	MOVD ECX, XMM0
	XOR ECX, 2147483648
	MOVD XMM0, ECX
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-16]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-16]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-16], XMM0; store
	JMP .L15
.L14:
.L15:
	MOV EAX, 265
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwGetKey
	PUSH RAX
	MOV EAX, 1
	POP RCX
	CMP RCX, RAX
	SETE AL
	CMP AL, 0
	JE .L17
	MOV RAX, D13; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-16]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-16]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-16], XMM0; store
	JMP .L17
.L16:
.L17:
	MOV EAX, 88
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwGetKey
	PUSH RAX
	MOV EAX, 1
	POP RCX
	CMP RCX, RAX
	SETE AL
	CMP AL, 0
	JE .L19
	MOV RAX, D14; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-20]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-20]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-20], XMM0; store
	JMP .L19
.L18:
.L19:
	MOV EAX, 90
	PUSH RAX
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	POP RSI
	CALL _glfwGetKey
	PUSH RAX
	MOV EAX, 1
	POP RCX
	CMP RCX, RAX
	SETE AL
	CMP AL, 0
	JE .L21
	MOV RAX, D15; float_ref
	MOVSS XMM0, [RAX]; float_ref
	MOVD ECX, XMM0
	XOR ECX, 2147483648
	MOVD XMM0, ECX
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-20]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-20]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-20], XMM0; store
	JMP .L21
.L20:
.L21:
	MOVSS XMM0, DWORD [RBP-20]; load
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-16]; load
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM2, DWORD [RSP]
	ADD RSP, 8
	CALL _draw_triangle
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	CALL _glfwSwapBuffers
	JMP .L4
.L5:
.L23:
	CALL _glfwTerminate
	JMP .L22
.L22:
	MOV EAX, 1
	JMP .END
.END:
	ADD RSP, 32; 24 alloc, 8 padding
	LEAVE
	RET
_draw_triangle:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 12 alloc, 4 padding
.BEGIN:
	MOVSS [RBP-4], XMM0; store
	MOVSS [RBP-8], XMM1; store
	MOVSS [RBP-12], XMM2; store
	MOV EAX, 4
	PUSH RAX
	POP RDI
	CALL _glBegin
	MOV RAX, D16; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D17; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D18; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM2, DWORD [RSP]
	ADD RSP, 8
	CALL _glColor3f
	MOV RAX, D19; float_ref
	MOVSS XMM0, [RAX]; float_ref
	MOVD ECX, XMM0
	XOR ECX, 2147483648
	MOVD XMM0, ECX
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MULSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-8]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D20; float_ref
	MOVSS XMM0, [RAX]; float_ref
	MOVD ECX, XMM0
	XOR ECX, 2147483648
	MOVD XMM0, ECX
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MULSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-4]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	CALL _glVertex2f
	MOV RAX, D21; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D22; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D23; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM2, DWORD [RSP]
	ADD RSP, 8
	CALL _glColor3f
	MOV RAX, D24; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MULSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-8]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D25; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MULSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-4]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	CALL _glVertex2f
	MOV RAX, D26; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D27; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D28; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM2, DWORD [RSP]
	ADD RSP, 8
	CALL _glColor3f
	MOV RAX, D29; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MULSS XMM0, XMM1
	MOV RAX, D30; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MULSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-8]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	SUBSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOV RAX, D31; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-12]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MULSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-4]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	ADDSS XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	CALL _glVertex2f
	CALL _glEnd
.END:
	ADD RSP, 16; 12 alloc, 4 padding
	LEAVE
	RET
_sqrt:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 32; 32 alloc, 0 padding
.BEGIN:
	MOVSD [RBP-8], XMM0; store
	MOV RAX, D32; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-16]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSD [RBP-16], XMM0; store
	MOV RAX, D33; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-24]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSD [RBP-24], XMM0; store
	MOV RAX, D34; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-32]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSD [RBP-32], XMM0; store
	MOV EAX, 0
	PUSH RAX
	MOV EAX, DWORD [RBP-36]; load
	POP RAX
	MOV [RBP-36], EAX; store
.L0:
	MOV EAX, DWORD [RBP-36]; load
	PUSH RAX
	MOV EAX, 1000
	POP RCX
	CMP RCX, RAX
	SETL AL
	CMP AL, 0
	JE .L2
	MOVSD XMM0, QWORD [RBP-32]; load
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-32]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MULSD XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-8]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	UCOMISD XMM1, XMM0
	SETE AL
	CMP AL, 0
	JE .L4
.L6:
	JMP .L5
.L5:
	MOVSD XMM0, QWORD [RBP-32]; load
	JMP .END
	JMP .L4
.L3:
.L4:
	MOVSD XMM0, QWORD [RBP-32]; load
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-32]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	MULSD XMM0, XMM1
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-8]; load
	MOVSS XMM1, DWORD [RSP]
	ADD RSP, 8
	UCOMISD XMM1, XMM0
	SETG AL
	CMP AL, 0
	JE .L7
	MOVSD XMM0, QWORD [RBP-32]; load
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-24]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSD [RBP-24], XMM0; store
	JMP .L8
.L7:
	MOVSD XMM0, QWORD [RBP-32]; load
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-16]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSD [RBP-16], XMM0; store
.L8:
.L1:
	MOV EAX, 1
	PUSH RAX
	MOV EAX, DWORD [RBP-36]; load
	POP RCX
	ADD RAX, RCX
	PUSH RAX
	MOV EAX, DWORD [RBP-36]; load
	POP RAX
	MOV [RBP-36], EAX; store
	JMP .L0
.L2:
.L10:
	JMP .L9
.L9:
	MOVSD XMM0, QWORD [RBP-32]; load
	JMP .END
.END:
	ADD RSP, 32; 32 alloc, 0 padding
	LEAVE
	RET
