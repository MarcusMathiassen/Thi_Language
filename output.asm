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
extern _getc
extern _getchar
extern _ungetc
extern _fputc
extern _putc
extern _putchar
extern _fgets
extern _fputs
extern _puts
extern _printf
extern _fprintf
extern _sprintf
extern _snprintf
extern _scanf
extern _fscanf
extern _sscanf
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
extern _acos
extern _asin
extern _atan
extern _atan2
extern _cos
extern _cosh
extern _sin
extern _sinh
extern _tanh
extern _exp
extern _frexp
extern _ldexp
extern _log
extern _log10
extern _modf
extern _pow
extern _sqrt
extern _ceil
extern _fabs
extern _floor
extern _fmod
section .data
	d0: dq `GLFW init failed.`, 0 
	d1: dq `Hello Triangle`, 0 
	d2: dq `GLEW init failed.`, 0 
	d3: dd 0.000000
	d4: dd 0.000000
	d5: dd 1.000000
	d6: dd 1.000000
	d7: dd 0.000000
	d8: dd 1.000000
	d9: dd 0.000000
	d10: dd 0.000000
	d11: dd 0.000000
	d12: dd 1.000000
	d13: dd 1.000000
	d14: dd 1.000000
	d15: dd 1.000000
	d16: dd 0.010000
	d17: dd 0.010000
	d18: dd 0.010000
	d19: dd 0.010000
	d20: dd 0.010000
	d21: dd 0.010000
	d22: dd 0.010000
	d23: dd 0.010000
	d24: dq `Closing..`, 0 
	d25: dd 0.000000
	d26: dd 0.000000
	d27: dd 1.000000
	d28: dd 0.500000
	d29: dd 0.500000
	d30: dd 0.000000
	d31: dd 1.000000
	d32: dd 0.000000
	d33: dd 0.500000
	d34: dd 0.000000
	d35: dd 1.000000
	d36: dd 0.000000
	d37: dd 0.000000
	d38: dd 0.500000
	d39: dd 0.500000
	d40: dd 0.500000
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 48; 33 alloc, 15 padding
.begin:
	call _glfwInit
	cmp al, 0
	sete al
	cmp al, 0
	je .l1
	mov rax, d0; string_ref
	push rax
	pop rdi
	call _puts
	jmp .l1
.l0:
.l1:
	mov eax, 2
	push rax
	mov eax, 139266
	push rax
	pop rdi
	pop rsi
	call _glfwWindowHint
	mov eax, 1
	push rax
	mov eax, 139267
	push rax
	pop rdi
	pop rsi
	mov rax, 2; var_arg_count
	call _glfwWindowHint
	mov eax, 4
	push rax
	mov eax, 135181
	push rax
	pop rdi
	pop rsi
	mov rax, 2; var_arg_count
	call _glfwWindowHint
	mov eax, 8
	push rax
	mov eax, 135169
	push rax
	pop rdi
	pop rsi
	mov rax, 2; var_arg_count
	call _glfwWindowHint
	mov eax, 8
	push rax
	mov eax, 135170
	push rax
	pop rdi
	pop rsi
	mov rax, 2; var_arg_count
	call _glfwWindowHint
	mov eax, 8
	push rax
	mov eax, 135171
	push rax
	pop rdi
	pop rsi
	mov rax, 2; var_arg_count
	call _glfwWindowHint
	mov eax, 8
	push rax
	mov eax, 135172
	push rax
	pop rdi
	pop rsi
	mov rax, 2; var_arg_count
	call _glfwWindowHint
	mov eax, 0
	push rax
	mov eax, 0
	push rax
	mov rax, d1; string_ref
	push rax
	mov eax, 480
	push rax
	mov eax, 640
	push rax
	pop rdi
	pop rsi
	pop rdx
	pop rcx
	pop r8
	call _glfwCreateWindow
	push rax
	push rax
	mov rax, [rbp-8]; load
	lea rax, [rbp-8]
	pop rcx
	mov [rax], rcx; store
	pop rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	mov rax, 1; var_arg_count
	call _glfwMakeContextCurrent
	call _glfwInit
	push rax
	mov eax, 0
	pop rcx
	cmp rcx, rax
	setne al
	cmp al, 0
	je .l3
	mov rax, d2; string_ref
	push rax
	pop rdi
	call _puts
	jmp .l3
.l2:
.l3:
	mov eax, 0
	push rax
	push rax
	mov al, [rbp-9]; load
	pop rcx
	mov [rbp-9], cl; store
	pop rax
	mov eax, 0
	push rax
	push rax
	mov eax, [rbp-13]; load
	pop rcx
	mov [rbp-13], ecx; store
	pop rax
	movss xmm0, [rel d3]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-17]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-17], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	movss xmm0, [rel d4]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-21]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-21], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	movss xmm0, [rel d5]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-25]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-25], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	mov eax, 1
	push rax
	push rax
	mov eax, [rbp-29]; load
	pop rcx
	mov [rbp-29], ecx; store
	pop rax
	mov eax, 1
	push rax
	push rax
	mov eax, [rbp-33]; load
	pop rcx
	mov [rbp-33], ecx; store
	pop rax
.l4:
	mov eax, [rbp-29]; load
	cmp al, 0
	je .l5
	movss xmm0, [rel d6]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-37]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-37], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	movss xmm0, [rel d7]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-41]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-41], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	movss xmm0, [rel d8]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-45]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-45], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	mov eax, 77
	push rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l8
	jmp .l7
.l8:
	mov eax, [rbp-33]; load
	push rax
	mov eax, 0
	pop rcx
	cmp rcx, rax
	sete al
	cmp eax, 0
	je .l9
	mov eax, 1
	jmp .l10
.l9:
	mov eax, 0
.l10:
	push rax
	push rax
	mov eax, [rbp-33]; load
	pop rcx
	mov [rbp-33], ecx; store
	pop rax
	jmp .l7
.l7:
	mov eax, [rbp-33]; load
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l13
	mov eax, [rbp-33]; load
	push rax
	mov eax, 0
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l14
	jmp .l12
.l13:
	movss xmm0, [rel d9]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-37]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-37], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	movss xmm0, [rel d10]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-41]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-41], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	movss xmm0, [rel d11]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-45]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-45], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	jmp .l12
.l14:
	movss xmm0, [rel d12]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-37]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-37], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	movss xmm0, [rel d13]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-41]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-41], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	movss xmm0, [rel d14]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-45]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-45], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	jmp .l12
.l12:
	movss xmm0, [rel d15]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-45]; load
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-41]; load
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-37]; load
	sub rsp, 8
	movsd [rsp], xmm0
	movsd xmm0, [rsp]
	add rsp, 8
	movsd xmm1, [rsp]
	add rsp, 8
	movsd xmm2, [rsp]
	add rsp, 8
	movsd xmm3, [rsp]
	add rsp, 8
	mov rax, 4; var_arg_count
	call _glClearColor
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	call _glfwWindowShouldClose
	cmp al, 0
	je .l16
	mov eax, 0
	push rax
	push rax
	mov eax, [rbp-29]; load
	pop rcx
	mov [rbp-29], ecx; store
	pop rax
	jmp .l16
.l15:
.l16:
	mov eax, 256
	push rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l19
	jmp .l18
.l19:
	mov eax, 0
	push rax
	push rax
	mov eax, [rbp-29]; load
	pop rcx
	mov [rbp-29], ecx; store
	pop rax
	jmp .l18
.l18:
	mov eax, 16384
	push rax
	pop rdi
	mov rax, 1; var_arg_count
	call _glClear
	mov rax, 0; var_arg_count
	call _glfwPollEvents
	mov eax, 263
	push rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	push rax
	mov eax, [rbp-49]; load
	pop rcx
	mov [rbp-49], ecx; store
	pop rax
	mov eax, [rbp-49]; load
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l22
	jmp .l21
.l22:
	movss xmm0, [rel d16]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-17]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-17]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-17], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	jmp .l21
.l21:
	mov eax, 262
	push rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l25
	jmp .l24
.l25:
	movss xmm0, [rel d17]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-17]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-17]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-17], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	jmp .l24
.l24:
	mov eax, 264
	push rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l28
	jmp .l27
.l28:
	movss xmm0, [rel d18]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-21]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-21]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-21], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	jmp .l27
.l27:
	mov eax, 265
	push rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l31
	jmp .l30
.l31:
	movss xmm0, [rel d19]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-21]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-21]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-21], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	jmp .l30
.l30:
	mov eax, 88
	push rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l34
	jmp .l33
.l34:
	movss xmm0, [rel d20]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-25]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-25]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-25], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	jmp .l33
.l33:
	mov eax, 90
	push rax
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l37
	jmp .l36
.l37:
	movss xmm0, [rel d21]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-25]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-25]; load
	movsd xmm1, [rsp]
	add rsp, 8
	movss [rbp-25], xmm1; store
	movsd xmm0, [rsp]
	add rsp, 8
	jmp .l36
.l36:
	movss xmm0, [rbp-25]; load
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-21]; load
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d22]; float_ref
	movsd xmm1, [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-17]; load
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d23]; float_ref
	movsd xmm1, [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movsd xmm0, [rsp]
	add rsp, 8
	movsd xmm1, [rsp]
	add rsp, 8
	movsd xmm2, [rsp]
	add rsp, 8
	mov rax, 3; var_arg_count
	call _drawTriangle
	mov rax, [rbp-8]; load
	push rax
	pop rdi
	mov rax, 1; var_arg_count
	call _glfwSwapBuffers
	jmp .l4
.l5:
	mov eax, 1
	jmp .end
.end:
	mov rax, d24; string_ref
	push rax
	pop rdi
	call _puts
	mov rax, 0; var_arg_count
	call _glfwTerminate
	add rsp, 48; 33 alloc, 15 padding
	leave
	ret
_drawTriangle:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 12 alloc, 4 padding
.begin:
	movss [rbp-4], xmm0; store_r
	movss [rbp-8], xmm1; store_r
	movss [rbp-12], xmm2; store_r
	mov eax, 4
	push rax
	pop rdi
	mov rax, 1; var_arg_count
	call _glBegin
	movss xmm0, [rel d25]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d26]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d27]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movsd xmm0, [rsp]
	add rsp, 8
	movsd xmm1, [rsp]
	add rsp, 8
	movsd xmm2, [rsp]
	add rsp, 8
	mov rax, 3; var_arg_count
	call _glColor3f
	movss xmm0, [rel d28]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-12]; load
	movsd xmm1, [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-8]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d29]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-12]; load
	movsd xmm1, [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-4]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movsd xmm0, [rsp]
	add rsp, 8
	movsd xmm1, [rsp]
	add rsp, 8
	mov rax, 2; var_arg_count
	call _glVertex2f
	movss xmm0, [rel d30]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d31]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d32]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movsd xmm0, [rsp]
	add rsp, 8
	movsd xmm1, [rsp]
	add rsp, 8
	movsd xmm2, [rsp]
	add rsp, 8
	mov rax, 3; var_arg_count
	call _glColor3f
	movss xmm0, [rel d33]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-12]; load
	movsd xmm1, [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-8]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d34]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-12]; load
	movsd xmm1, [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-4]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movsd xmm0, [rsp]
	add rsp, 8
	movsd xmm1, [rsp]
	add rsp, 8
	call _glVertex2f
	movss xmm0, [rel d35]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d36]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d37]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movsd xmm0, [rsp]
	add rsp, 8
	movsd xmm1, [rsp]
	add rsp, 8
	movsd xmm2, [rsp]
	add rsp, 8
	call _glColor3f
	movss xmm0, [rel d38]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-12]; load
	movsd xmm1, [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	movss xmm0, [rel d39]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-12]; load
	movsd xmm1, [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-8]; load
	movsd xmm1, [rsp]
	add rsp, 8
	subss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rel d40]; float_ref
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-12]; load
	movsd xmm1, [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movss xmm0, [rbp-4]; load
	movsd xmm1, [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movsd [rsp], xmm0
	movsd xmm0, [rsp]
	add rsp, 8
	movsd xmm1, [rsp]
	add rsp, 8
	mov rax, 2; var_arg_count
	call _glVertex2f
.end:
	call _glEnd
	add rsp, 16; 12 alloc, 4 padding
	leave
	ret
