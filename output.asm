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
extern _atoi
extern _atol
extern _atoll
extern _atof
extern _strtof
extern _strtod
extern _strtold
extern _strtol
extern _strtoul
extern _strtoll
extern _strtoull
extern _rand
extern _srand
extern _malloc
extern _calloc
extern _realloc
extern _free
extern _aligned_alloc
extern _abort
extern _exit
extern __Exit
extern _quick_exit
extern _getenv
extern _system
extern _abs
extern _labs
extern _llabs
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
extern _glewInit
extern _glewIsSupported
extern _glewGetExtension
extern _glewGetErrorString
extern _glewGetString
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
section .data
	D0 DQ `Hello, world!`, 0 
	D1 DD 3.100000
	D2 DD 5.030000
	D3 DD 5.030000
	D4 DD 64.300000
	D5 DD 64.300000
	D6 DQ `Hello`, 0 
	D7 DQ `you`, 0 
	D8 DQ `you`, 0 
	D9 DQ `echo hello you`, 0 
global _test_no_main
section .text
_test_no_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 96; 89 alloc, 7 padding
.BEGIN:
	MOV [RBP-4], EDI; store
	MOV [RBP-12], RSI; store
	MOV RAX, D0; string_ref
	PUSH RAX
	POP RDI
	CALL _puts
	MOV EAX, 0
	PUSH RAX
	MOV EAX, DWORD [RBP-16]; load
	POP RAX
	MOV [RBP-16], EAX; store
	MOV EAX, 0
	PUSH RAX
	MOV EAX, DWORD [RBP-20]; load
	POP RAX
	MOV [RBP-20], EAX; store
	MOV EAX, 0
	PUSH RAX
	MOV EAX, DWORD [RBP-24]; load
	POP RAX
	MOV [RBP-24], EAX; store
	MOV EAX, 4
	PUSH RAX
	MOV RAX, QWORD [RBP-32]; load
	POP RAX
	MOV [RBP-32], RAX; store
	MOV RAX, D1; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-36]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-36], XMM0; store
	MOV RAX, D2; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-40]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-40], XMM0; store
	MOV RAX, D3; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSS XMM0, DWORD [RBP-44]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSS [RBP-44], XMM0; store
	MOV RAX, D4; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-52]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSD [RBP-52], XMM0; store
	MOV RAX, D5; float_ref
	MOVSS XMM0, [RAX]; float_ref
	SUB RSP, 8
	MOVSS [RSP], XMM0
	MOVSD XMM0, QWORD [RBP-60]; load
	MOVSS XMM0, DWORD [RSP]
	ADD RSP, 8
	MOVSD [RBP-60], XMM0; store
	MOV RAX, D6; string_ref
	PUSH RAX
	MOV RAX, QWORD [RBP-68]; load
	POP RAX
	MOV [RBP-68], RAX; store
	MOV RAX, D7; string_ref
	PUSH RAX
	MOV RAX, QWORD [RBP-76]; load
	POP RAX
	MOV [RBP-76], RAX; store
	MOV RAX, D8; string_ref
	PUSH RAX
	MOV RAX, QWORD [RBP-84]; load
	POP RAX
	MOV [RBP-84], RAX; store
	MOV EAX, 0
	PUSH RAX
	MOV EAX, DWORD [RBP-88]; load
	POP RAX
	MOV [RBP-88], EAX; store
	MOV EAX, 1
	PUSH RAX
	MOV AL, BYTE [RBP-89]; load
	POP RAX
	MOV [RBP-89], AL; store
	MOV RAX, D9; string_ref
	PUSH RAX
	POP RDI
	CALL _system
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	ADD RSP, 96; 89 alloc, 7 padding
	LEAVE
	RET
_cstdio:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	LEAVE
	RET
_cstdlib:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	LEAVE
	RET
_glfw:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	LEAVE
	RET
_glew:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	LEAVE
	RET
_opengl:
	PUSH RBP
	MOV RBP, RSP
.BEGIN:
.L1:
	JMP .L0
.L0:
	MOV EAX, 1
	JMP .END
.END:
	LEAVE
	RET
