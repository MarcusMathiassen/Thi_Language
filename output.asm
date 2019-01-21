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
extern _puts
section .data
	D0 db `error initilizing GLFW`, 0 
	D1 db `Triangle`, 0 
	D2 db `error initilizing GLEW`, 0 
global _main
section .text
_main:
	PUSH RBP
	MOV RBP, RSP
	SUB RSP, 16; 16 alloc, 0 padding
.BEGIN:
	CALL _glfwInit
	CMP RAX, 0
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
	MOV RAX, 139266
	PUSH RAX
	MOV RAX, 2
	PUSH RAX
	POP RSI
	POP RDI
	CALL _glfwWindowHint
	MOV RAX, 139267
	PUSH RAX
	MOV RAX, 1
	PUSH RAX
	POP RSI
	POP RDI
	CALL _glfwWindowHint
	MOV RAX, 131077
	PUSH RAX
	MOV RAX, 1
	PUSH RAX
	POP RSI
	POP RDI
	CALL _glfwWindowHint
	MOV RAX, 135181
	PUSH RAX
	MOV RAX, 4
	PUSH RAX
	POP RSI
	POP RDI
	CALL _glfwWindowHint
	MOV RAX, 135169
	PUSH RAX
	MOV RAX, 8
	PUSH RAX
	POP RSI
	POP RDI
	CALL _glfwWindowHint
	MOV RAX, 135170
	PUSH RAX
	MOV RAX, 8
	PUSH RAX
	POP RSI
	POP RDI
	CALL _glfwWindowHint
	MOV RAX, 135171
	PUSH RAX
	MOV RAX, 8
	PUSH RAX
	POP RSI
	POP RDI
	CALL _glfwWindowHint
	MOV RAX, 135172
	PUSH RAX
	MOV RAX, 8
	PUSH RAX
	POP RSI
	POP RDI
	CALL _glfwWindowHint
	MOV RAX, 512
	PUSH RAX
	MOV RAX, 512
	PUSH RAX
	MOV RAX, D1; string_ref
	PUSH RAX
	MOV RAX, 0
	PUSH RAX
	MOV RAX, 0
	PUSH RAX
	POP R8
	POP RCX
	POP RDX
	POP RSI
	POP RDI
	CALL _glfwCreateWindow
	PUSH RAX
	PUSH RAX
	LEA RAX, [RBP-8]; load_ref
	POP RCX
	MOV [RAX], RCX; store
	POP RAX
	LEA RAX, [RBP-8]; load_ref
	PUSH RAX
	POP RDI
	CALL _glfwMakeContextCurrent
	CALL _glewInit
	PUSH RAX
	MOV RAX, 0
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
	MOV RAX, 1
	PUSH RAX
	PUSH RAX
	MOV RAX, QWORD [RBP-16]; load
	POP RCX
	MOV [RBP-16], RCX; store
	POP RAX
.L4:
	LEA RAX, [RBP-8]; load_ref
	PUSH RAX
	POP RDI
	CALL _glfwWindowShouldClose
	CMP RAX, 0
	SETE AL
	CMP AL, 0
	JE .L5
	CALL _glfwPollEvents
	MOV RAX, 16384
	PUSH RAX
	POP RDI
	CALL _glClear
	LEA RAX, [RBP-8]; load_ref
	PUSH RAX
	MOV RAX, 256
	PUSH RAX
	POP RSI
	POP RDI
	CALL _glfwGetKey
	PUSH RAX
	MOV RAX, 1
	POP RCX
	CMP RCX, RAX
	SETE AL
	CMP AL, 0
	JE .L7
	MOV RAX, 0
	PUSH RAX
	PUSH RAX
	MOV RAX, QWORD [RBP-16]; load
	POP RCX
	MOV [RBP-16], RCX; store
	POP RAX
	JMP .L7
.L6:
.L7:
	LEA RAX, [RBP-8]; load_ref
	PUSH RAX
	POP RDI
	CALL _glfwSwapBuffers
	JMP .L4
.L5:
.L9:
	JMP .L8
.L8:
	MOV RAX, 1
	JMP .END
.END:
	ADD RSP, 16; 16 alloc, 0 padding
	LEAVE
	RET
