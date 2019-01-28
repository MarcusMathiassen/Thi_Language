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
	d0 dq `GLFW init failed.`, 0 
	d1 dq `Hello Triangle`, 0 
	d2 dq `GLEW init failed.`, 0 
	d3 dd 1.000000
	d4 dd 0.010000
	d5 dd 0.010000
	d6 dd 0.010000
	d7 dd 0.000000
	d8 dd 0.000000
	d9 dd 1.000000
	d10 dd 0.001000
	d11 dd 0.001000
	d12 dd 0.001000
	d13 dd 0.001000
	d14 dd 0.001000
	d15 dd 0.001000
	d16 dd 0.000000
	d17 dd 0.000000
	d18 dd 1.000000
	d19 dd 0.500000
	d20 dd 0.500000
	d21 dd 0.000000
	d22 dd 1.000000
	d23 dd 0.000000
	d24 dd 0.500000
	d25 dd 0.000000
	d26 dd 1.000000
	d27 dd 0.000000
	d28 dd 0.000000
	d29 dd 0.500000
	d30 dd 0.500000
	d31 dd 0.500000
	d32 dd 0.000000
	d33 dd 0.000000
	d34 dd 0.000000
global _main
section .text
_main:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 24 alloc, 8 padding
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
	call _glfwWindowHint
	mov eax, 4
	push rax
	mov eax, 135181
	push rax
	pop rdi
	pop rsi
	call _glfwWindowHint
	mov eax, 8
	push rax
	mov eax, 135169
	push rax
	pop rdi
	pop rsi
	call _glfwWindowHint
	mov eax, 8
	push rax
	mov eax, 135170
	push rax
	pop rdi
	pop rsi
	call _glfwWindowHint
	mov eax, 8
	push rax
	mov eax, 135171
	push rax
	pop rdi
	pop rsi
	call _glfwWindowHint
	mov eax, 8
	push rax
	mov eax, 135172
	push rax
	pop rdi
	pop rsi
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
	mov rax, qword [rbp-8]; load
	pop rax
	mov [rbp-8], rax; store
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
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
	mov rax, d3; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d4; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d5; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d6; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rsp]
	add rsp, 8
	movss xmm1, dword [rsp]
	add rsp, 8
	movss xmm2, dword [rsp]
	add rsp, 8
	movss xmm3, dword [rsp]
	add rsp, 8
	call _glClearColor
	mov eax, 36281
	push rax
	pop rdi
	call _glEnable
	mov rax, d7; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movss [rbp-12], xmm0; store
	mov rax, d8; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-16]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movss [rbp-16], xmm0; store
	mov rax, d9; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-20]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movss [rbp-20], xmm0; store
	mov eax, 1
	push rax
	mov eax, dword [rbp-24]; load
	pop rax
	mov [rbp-24], eax; store
.l4:
	mov eax, dword [rbp-24]; load
	cmp al, 0
	JE .l5
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
	call _glfwWindowShouldClose
	cmp al, 0
	je .l7
	mov eax, 0
	push rax
	mov eax, dword [rbp-24]; load
	pop rax
	mov [rbp-24], eax; store
	jmp .l7
.l6:
.l7:
	mov eax, 256
	push rax
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 0
	je .l9
	mov eax, 0
	push rax
	mov eax, dword [rbp-24]; load
	pop rax
	mov [rbp-24], eax; store
	jmp .l9
.l8:
.l9:
	mov eax, 16384
	push rax
	pop rdi
	call _glClear
	call _glfwPollEvents
	mov eax, 263
	push rax
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, dword [rbp-28]; load
	pop rax
	mov [rbp-28], eax; store
	call _glfwGetTime
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-36]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movsd [rbp-36], xmm0; store
	mov eax, dword [rbp-28]; load
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 1
	je .l12
	jmp .l11
.l12:
	mov rax, d10; float_ref
	movss xmm0, [rax]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movss [rbp-12], xmm0; store
	jmp .l11
.l11:
	mov eax, 262
	push rax
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 0
	je .l14
	mov rax, d11; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movss [rbp-12], xmm0; store
	jmp .l14
.l13:
.l14:
	mov eax, 264
	push rax
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 0
	je .l16
	mov rax, d12; float_ref
	movss xmm0, [rax]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-16]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-16]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movss [rbp-16], xmm0; store
	jmp .l16
.l15:
.l16:
	mov eax, 265
	push rax
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 0
	je .l18
	mov rax, d13; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-16]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-16]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movss [rbp-16], xmm0; store
	jmp .l18
.l17:
.l18:
	mov eax, 88
	push rax
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 0
	je .l20
	mov rax, d14; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-20]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-20]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movss [rbp-20], xmm0; store
	jmp .l20
.l19:
.l20:
	mov eax, 90
	push rax
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
	pop rsi
	call _glfwGetKey
	push rax
	mov eax, 1
	pop rcx
	cmp rcx, rax
	sete al
	cmp al, 0
	je .l22
	mov rax, d15; float_ref
	movss xmm0, [rax]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-20]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-20]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movss [rbp-20], xmm0; store
	jmp .l22
.l21:
.l22:
	movss xmm0, dword [rbp-20]; load
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-16]; load
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rsp]
	add rsp, 8
	movss xmm1, dword [rsp]
	add rsp, 8
	movss xmm2, dword [rsp]
	add rsp, 8
	call _draw_triangle
	mov rax, qword [rbp-8]; load
	push rax
	pop rdi
	call _glfwSwapBuffers
	JMP .l4
.l5:
.l24:
	call _glfwTerminate
	jmp .l23
.l23:
	mov eax, 1
	jmp .end
.end:
	add rsp, 32; 24 alloc, 8 padding
	leave
	ret
_draw_triangle:
	push rbp
	mov rbp, rsp
	sub rsp, 16; 12 alloc, 4 padding
.begin:
	movss [rbp-4], xmm0; store
	movss [rbp-8], xmm1; store
	movss [rbp-12], xmm2; store
	mov eax, 4
	push rax
	pop rdi
	call _glBegin
	mov rax, d16; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d17; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d18; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rsp]
	add rsp, 8
	movss xmm1, dword [rsp]
	add rsp, 8
	movss xmm2, dword [rsp]
	add rsp, 8
	call _glColor3f
	mov rax, d19; float_ref
	movss xmm0, [rax]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-8]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d20; float_ref
	movss xmm0, [rax]; float_ref
	movd ecx, xmm0
	xor ecx, 2147483648
	movd xmm0, ecx
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-4]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rsp]
	add rsp, 8
	movss xmm1, dword [rsp]
	add rsp, 8
	call _glVertex2f
	mov rax, d21; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d22; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d23; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rsp]
	add rsp, 8
	movss xmm1, dword [rsp]
	add rsp, 8
	movss xmm2, dword [rsp]
	add rsp, 8
	call _glColor3f
	mov rax, d24; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-8]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d25; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-4]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rsp]
	add rsp, 8
	movss xmm1, dword [rsp]
	add rsp, 8
	call _glVertex2f
	mov rax, d26; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d27; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d28; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rsp]
	add rsp, 8
	movss xmm1, dword [rsp]
	add rsp, 8
	movss xmm2, dword [rsp]
	add rsp, 8
	call _glColor3f
	mov rax, d29; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	mov rax, d30; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-8]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	subss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	mov rax, d31; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-12]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	mulss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rbp-4]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	addss xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movss xmm0, dword [rsp]
	add rsp, 8
	movss xmm1, dword [rsp]
	add rsp, 8
	call _glVertex2f
	call _glEnd
.end:
	add rsp, 16; 12 alloc, 4 padding
	leave
	ret
_sqrt:
	push rbp
	mov rbp, rsp
	sub rsp, 32; 32 alloc, 0 padding
.begin:
	movsd [rbp-8], xmm0; store
	mov rax, d32; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-16]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movsd [rbp-16], xmm0; store
	mov rax, d33; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-24]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movsd [rbp-24], xmm0; store
	mov rax, d34; float_ref
	movss xmm0, [rax]; float_ref
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-32]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movsd [rbp-32], xmm0; store
	mov eax, 0
	push rax
	mov eax, dword [rbp-36]; load
	pop rax
	mov [rbp-36], eax; store
.l0:
	mov eax, dword [rbp-36]; load
	push rax
	mov eax, 1000
	pop rcx
	cmp rcx, rax
	SETL AL
	CMP AL, 0
	JE .l2
	movsd xmm0, qword [rbp-32]; load
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-32]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-8]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	ucomisd xmm1, xmm0
	sete al
	cmp al, 0
	je .l4
.l6:
	jmp .l5
.l5:
	movsd xmm0, qword [rbp-32]; load
	jmp .end
	jmp .l4
.l3:
.l4:
	movsd xmm0, qword [rbp-32]; load
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-32]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	mulsd xmm0, xmm1
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-8]; load
	movss xmm1, dword [rsp]
	add rsp, 8
	ucomisd xmm1, xmm0
	setg al
	cmp al, 0
	je .l7
	movsd xmm0, qword [rbp-32]; load
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-24]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movsd [rbp-24], xmm0; store
	jmp .l8
.l7:
	movsd xmm0, qword [rbp-32]; load
	sub rsp, 8
	movss [rsp], xmm0
	movsd xmm0, qword [rbp-16]; load
	movss xmm0, dword [rsp]
	add rsp, 8
	movsd [rbp-16], xmm0; store
.l8:
.l1:
	mov eax, 1
	push rax
	mov eax, dword [rbp-36]; load
	pop rcx
	add rax, rcx
	push rax
	mov eax, dword [rbp-36]; load
	pop rax
	mov [rbp-36], eax; store
	JMP .l0
.l2:
.l10:
	jmp .l9
.l9:
	movsd xmm0, qword [rbp-32]; load
	jmp .end
.end:
	add rsp, 32; 32 alloc, 0 padding
	leave
	ret
