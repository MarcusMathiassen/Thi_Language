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
	D0 db `Hello Triangle`, 0 
	D1 db 1.000000
	D2 db 0.020000
	D3 db 1.000000
	D4 db 1.000000
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 8 alloc, 8 padding
.BEGIN:
	CALL _glfwInit
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
	MOV RAX, D0; string_ref
	PUSH RAX
	MOV EAX, 512
	PUSH RAX
	MOV EAX, 512
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
	MOV RAX, D1; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSD [RSP], XMM0
	MOV RAX, D2; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSD [RSP], XMM0
	MOV RAX, D3; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSD [RSP], XMM0
	MOV RAX, D4; float_ref
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
.L0:
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	CALL _glfwWindowShouldClose
	CMP AL, 0
	SETE AL
	CMP AL, 0
	JE .L1
	MOV EAX, 16384
	PUSH RAX
	POP RDI
	CALL _glClear
	CALL _glfwPollEvents
	MOV RAX, QWORD [RBP-8]; load
	PUSH RAX
	POP RDI
	CALL _glfwSwapBuffers
	JMP .L0
.L1:
	CALL _glfwTerminate
.L3:
	JMP .L2
.L2:
	MOV EAX, 1
	JMP .END
.END:
	ADD RSP, 16; 8 alloc, 8 padding
	LEAVE
	RET
