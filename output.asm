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
section .data
	D0 db `OpenGL Triangle`, 0 
	D1 db `GLFW init failed`, 0 
	D2 db `Hello Triangle`, 0 
	D3 db `GLEW init failed`, 0 
	D4 db `settings background color..`, 0 
	D5 db 1.000000
	D6 db 1.000000
	D7 db 1.000000
	D8 db 1.000000
	D9 db `Starting loop..`, 0 
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	MOV RAX, D0; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	CALL _glfwInit
	CMP AL, 0
	SETE AL
	CMP AL, 0
	JE .L1
	MOV RAX, D1; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	JMP .L1
.L0:
.L1:
	MOV EAX, 512
	PUSH RAX
	MOV EAX, 512
	PUSH RAX
	MOV RAX, D2; string_ref
	PUSH RAX
	MOV EAX, 0
	PUSH RAX
	MOV EAX, 0
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
	CALL _glewInit
	PUSH RAX
	MOV EAX, 0
	POP RCX
	CMP RCX, RAX
	SETNE AL
	CMP AL, 0
	JE .L3
	MOV RAX, D3; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	JMP .L3
.L2:
.L3:
	MOV RAX, D4; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV RAX, D5; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSD [RSP], XMM0
	MOV RAX, D6; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSD [RSP], XMM0
	MOV RAX, D7; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSD [RSP], XMM0
	MOV RAX, D8; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSD [RSP], XMM0
	MOVSD XMM0, [RSP]
	ADD RSP, 8
	MOVSD XMM1, [RSP]
	ADD RSP, 8
	MOVSD XMM2, [RSP]
	ADD RSP, 8
	MOVSD XMM3, [RSP]
	ADD RSP, 8
	CALL _glClearColor
	MOV RAX, D9; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
.L4:
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	CALL _glfwWindowShouldClose
	CMP AL, 0
	SETE AL
	CMP AL, 0
	JE .L5
	CALL _glfwPollEvents
	MOV EAX, 16384
	PUSH RAX
	POP RDI
	CALL _glClear
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	CALL _glfwSwapBuffers
	JMP .L4
.L5:
.L7:
	JMP .L6
.L6:
	MOV EAX, 1
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
