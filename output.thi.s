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
extern _glfwSetGamma
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
	d0: dd 0.000000
	d1: dd 1.000000
	d2: dd 0.500000
	d3: db `Hello Triangle`, 0 
	d4: db `GLFW init failed.`, 0 
	d5: db `GLEW init failed.`, 0 
	d6: dd 0.010000
	d7: dd 0.001000
	d8: dd 0.100000
	d9: db `Closing..`, 0 
global _main
section .text
		mov rax, -1
		push rax
		mov rax, [rbp-8]; load EOF
		pop rcx
		mov [rbp-8], rcx; store EOF
		mov rax, [rbp-8]; load EOF
		mov rax, 0
		push rax
		mov rax, [rbp-16]; load SEEK_SET
		pop rcx
		mov [rbp-16], rcx; store SEEK_SET
		mov rax, [rbp-16]; load SEEK_SET
		mov rax, 1
		push rax
		mov rax, [rbp-24]; load SEEK_CUR
		pop rcx
		mov [rbp-24], rcx; store SEEK_CUR
		mov rax, [rbp-24]; load SEEK_CUR
		mov rax, 2
		push rax
		mov rax, [rbp-32]; load SEEK_END
		pop rcx
		mov [rbp-32], rcx; store SEEK_END
		mov rax, [rbp-32]; load SEEK_END
		mov rax, 0
		push rax
		mov rax, [rbp-40]; load _IOFBF
		pop rcx
		mov [rbp-40], rcx; store _IOFBF
		mov rax, [rbp-40]; load _IOFBF
		mov rax, 1
		push rax
		mov rax, [rbp-48]; load _IOLBF
		pop rcx
		mov [rbp-48], rcx; store _IOLBF
		mov rax, [rbp-48]; load _IOLBF
		mov rax, 2
		push rax
		mov rax, [rbp-56]; load _IONBF
		pop rcx
		mov [rbp-56], rcx; store _IONBF
		mov rax, [rbp-56]; load _IONBF
		mov rax, 1024
		push rax
		mov rax, [rbp-64]; load BUFSIZ
		pop rcx
		mov [rbp-64], rcx; store BUFSIZ
		mov rax, [rbp-64]; load BUFSIZ
		mov rax, 4096
		push rax
		mov rax, [rbp-72]; load FILENAME_MAX
		pop rcx
		mov [rbp-72], rcx; store FILENAME_MAX
		mov rax, [rbp-72]; load FILENAME_MAX
		mov rax, 1000
		push rax
		mov rax, [rbp-80]; load FOPEN_MAX
		pop rcx
		mov [rbp-80], rcx; store FOPEN_MAX
		mov rax, [rbp-80]; load FOPEN_MAX
		mov rax, 10000
		push rax
		mov rax, [rbp-88]; load TMP_MAX
		pop rcx
		mov [rbp-88], rcx; store TMP_MAX
		mov rax, [rbp-88]; load TMP_MAX
		mov rax, 20
		push rax
		mov rax, [rbp-96]; load L_tmpnam
		pop rcx
		mov [rbp-96], rcx; store L_tmpnam
		mov rax, [rbp-96]; load L_tmpnam
		mov rax, 0
		push rax
		mov rax, [rbp-104]; load NULL
		pop rcx
		mov [rbp-104], rcx; store NULL
		mov rax, [rbp-104]; load NULL
		mov rax, 1
		push rax
		mov rax, [rbp-112]; load EXIT_FAILURE
		pop rcx
		mov [rbp-112], rcx; store EXIT_FAILURE
		mov rax, [rbp-112]; load EXIT_FAILURE
		mov rax, 0
		push rax
		mov rax, [rbp-120]; load EXIT_SUCCESS
		pop rcx
		mov [rbp-120], rcx; store EXIT_SUCCESS
		mov rax, [rbp-120]; load EXIT_SUCCESS
		mov rax, 2147483647
		push rax
		mov rax, [rbp-128]; load RAND_MAX
		pop rcx
		mov [rbp-128], rcx; store RAND_MAX
		mov rax, [rbp-128]; load RAND_MAX
		mov rax, 0
		push rax
		mov rax, [rbp-136]; load GL_FALSE
		pop rcx
		mov [rbp-136], rcx; store GL_FALSE
		mov rax, [rbp-136]; load GL_FALSE
		mov rax, 1
		push rax
		mov rax, [rbp-144]; load GL_TRUE
		pop rcx
		mov [rbp-144], rcx; store GL_TRUE
		mov rax, [rbp-144]; load GL_TRUE
		mov rax, 256
		push rax
		mov rax, [rbp-152]; load GL_DEPTH_BUFFER_BIT
		pop rcx
		mov [rbp-152], rcx; store GL_DEPTH_BUFFER_BIT
		mov rax, [rbp-152]; load GL_DEPTH_BUFFER_BIT
		mov rax, 1024
		push rax
		mov rax, [rbp-160]; load GL_STENCIL_BUFFER_BIT
		pop rcx
		mov [rbp-160], rcx; store GL_STENCIL_BUFFER_BIT
		mov rax, [rbp-160]; load GL_STENCIL_BUFFER_BIT
		mov rax, 16384
		push rax
		mov rax, [rbp-168]; load GL_COLOR_BUFFER_BIT
		pop rcx
		mov [rbp-168], rcx; store GL_COLOR_BUFFER_BIT
		mov rax, [rbp-168]; load GL_COLOR_BUFFER_BIT
		mov rax, 0
		push rax
		mov rax, [rbp-176]; load GL_POINTS
		pop rcx
		mov [rbp-176], rcx; store GL_POINTS
		mov rax, [rbp-176]; load GL_POINTS
		mov rax, 1
		push rax
		mov rax, [rbp-184]; load GL_LINES
		pop rcx
		mov [rbp-184], rcx; store GL_LINES
		mov rax, [rbp-184]; load GL_LINES
		mov rax, 2
		push rax
		mov rax, [rbp-192]; load GL_LINE_LOOP
		pop rcx
		mov [rbp-192], rcx; store GL_LINE_LOOP
		mov rax, [rbp-192]; load GL_LINE_LOOP
		mov rax, 3
		push rax
		mov rax, [rbp-200]; load GL_LINE_STRIP
		pop rcx
		mov [rbp-200], rcx; store GL_LINE_STRIP
		mov rax, [rbp-200]; load GL_LINE_STRIP
		mov rax, 4
		push rax
		mov rax, [rbp-208]; load GL_TRIANGLES
		pop rcx
		mov [rbp-208], rcx; store GL_TRIANGLES
		mov rax, [rbp-208]; load GL_TRIANGLES
		mov rax, 5
		push rax
		mov rax, [rbp-216]; load GL_TRIANGLE_STRIP
		pop rcx
		mov [rbp-216], rcx; store GL_TRIANGLE_STRIP
		mov rax, [rbp-216]; load GL_TRIANGLE_STRIP
		mov rax, 6
		push rax
		mov rax, [rbp-224]; load GL_TRIANGLE_FAN
		pop rcx
		mov [rbp-224], rcx; store GL_TRIANGLE_FAN
		mov rax, [rbp-224]; load GL_TRIANGLE_FAN
		mov rax, 7
		push rax
		mov rax, [rbp-232]; load GL_QUADS
		pop rcx
		mov [rbp-232], rcx; store GL_QUADS
		mov rax, [rbp-232]; load GL_QUADS
		mov rax, 512
		push rax
		mov rax, [rbp-240]; load GL_NEVER
		pop rcx
		mov [rbp-240], rcx; store GL_NEVER
		mov rax, [rbp-240]; load GL_NEVER
		mov rax, 513
		push rax
		mov rax, [rbp-248]; load GL_LESS
		pop rcx
		mov [rbp-248], rcx; store GL_LESS
		mov rax, [rbp-248]; load GL_LESS
		mov rax, 514
		push rax
		mov rax, [rbp-256]; load GL_EQUAL
		pop rcx
		mov [rbp-256], rcx; store GL_EQUAL
		mov rax, [rbp-256]; load GL_EQUAL
		mov rax, 515
		push rax
		mov rax, [rbp-264]; load GL_LEQUAL
		pop rcx
		mov [rbp-264], rcx; store GL_LEQUAL
		mov rax, [rbp-264]; load GL_LEQUAL
		mov rax, 516
		push rax
		mov rax, [rbp-272]; load GL_GREATER
		pop rcx
		mov [rbp-272], rcx; store GL_GREATER
		mov rax, [rbp-272]; load GL_GREATER
		mov rax, 517
		push rax
		mov rax, [rbp-280]; load GL_NOTEQUAL
		pop rcx
		mov [rbp-280], rcx; store GL_NOTEQUAL
		mov rax, [rbp-280]; load GL_NOTEQUAL
		mov rax, 518
		push rax
		mov rax, [rbp-288]; load GL_GEQUAL
		pop rcx
		mov [rbp-288], rcx; store GL_GEQUAL
		mov rax, [rbp-288]; load GL_GEQUAL
		mov rax, 519
		push rax
		mov rax, [rbp-296]; load GL_ALWAYS
		pop rcx
		mov [rbp-296], rcx; store GL_ALWAYS
		mov rax, [rbp-296]; load GL_ALWAYS
		mov rax, 0
		push rax
		mov rax, [rbp-304]; load GL_ZERO
		pop rcx
		mov [rbp-304], rcx; store GL_ZERO
		mov rax, [rbp-304]; load GL_ZERO
		mov rax, 1
		push rax
		mov rax, [rbp-312]; load GL_ONE
		pop rcx
		mov [rbp-312], rcx; store GL_ONE
		mov rax, [rbp-312]; load GL_ONE
		mov rax, 768
		push rax
		mov rax, [rbp-320]; load GL_SRC_COLOR
		pop rcx
		mov [rbp-320], rcx; store GL_SRC_COLOR
		mov rax, [rbp-320]; load GL_SRC_COLOR
		mov rax, 769
		push rax
		mov rax, [rbp-328]; load GL_ONE_MINUS_SRC_COLOR
		pop rcx
		mov [rbp-328], rcx; store GL_ONE_MINUS_SRC_COLOR
		mov rax, [rbp-328]; load GL_ONE_MINUS_SRC_COLOR
		mov rax, 770
		push rax
		mov rax, [rbp-336]; load GL_SRC_ALPHA
		pop rcx
		mov [rbp-336], rcx; store GL_SRC_ALPHA
		mov rax, [rbp-336]; load GL_SRC_ALPHA
		mov rax, 771
		push rax
		mov rax, [rbp-344]; load GL_ONE_MINUS_SRC_ALPHA
		pop rcx
		mov [rbp-344], rcx; store GL_ONE_MINUS_SRC_ALPHA
		mov rax, [rbp-344]; load GL_ONE_MINUS_SRC_ALPHA
		mov rax, 772
		push rax
		mov rax, [rbp-352]; load GL_DST_ALPHA
		pop rcx
		mov [rbp-352], rcx; store GL_DST_ALPHA
		mov rax, [rbp-352]; load GL_DST_ALPHA
		mov rax, 773
		push rax
		mov rax, [rbp-360]; load GL_ONE_MINUS_DST_ALPHA
		pop rcx
		mov [rbp-360], rcx; store GL_ONE_MINUS_DST_ALPHA
		mov rax, [rbp-360]; load GL_ONE_MINUS_DST_ALPHA
		mov rax, 774
		push rax
		mov rax, [rbp-368]; load GL_DST_COLOR
		pop rcx
		mov [rbp-368], rcx; store GL_DST_COLOR
		mov rax, [rbp-368]; load GL_DST_COLOR
		mov rax, 775
		push rax
		mov rax, [rbp-376]; load GL_ONE_MINUS_DST_COLOR
		pop rcx
		mov [rbp-376], rcx; store GL_ONE_MINUS_DST_COLOR
		mov rax, [rbp-376]; load GL_ONE_MINUS_DST_COLOR
		mov rax, 776
		push rax
		mov rax, [rbp-384]; load GL_SRC_ALPHA_SATURATE
		pop rcx
		mov [rbp-384], rcx; store GL_SRC_ALPHA_SATURATE
		mov rax, [rbp-384]; load GL_SRC_ALPHA_SATURATE
		mov rax, 0
		push rax
		mov rax, [rbp-392]; load GL_NONE
		pop rcx
		mov [rbp-392], rcx; store GL_NONE
		mov rax, [rbp-392]; load GL_NONE
		mov rax, 1024
		push rax
		mov rax, [rbp-400]; load GL_FRONT_LEFT
		pop rcx
		mov [rbp-400], rcx; store GL_FRONT_LEFT
		mov rax, [rbp-400]; load GL_FRONT_LEFT
		mov rax, 1025
		push rax
		mov rax, [rbp-408]; load GL_FRONT_RIGHT
		pop rcx
		mov [rbp-408], rcx; store GL_FRONT_RIGHT
		mov rax, [rbp-408]; load GL_FRONT_RIGHT
		mov rax, 1026
		push rax
		mov rax, [rbp-416]; load GL_BACK_LEFT
		pop rcx
		mov [rbp-416], rcx; store GL_BACK_LEFT
		mov rax, [rbp-416]; load GL_BACK_LEFT
		mov rax, 1027
		push rax
		mov rax, [rbp-424]; load GL_BACK_RIGHT
		pop rcx
		mov [rbp-424], rcx; store GL_BACK_RIGHT
		mov rax, [rbp-424]; load GL_BACK_RIGHT
		mov rax, 1028
		push rax
		mov rax, [rbp-432]; load GL_FRONT
		pop rcx
		mov [rbp-432], rcx; store GL_FRONT
		mov rax, [rbp-432]; load GL_FRONT
		mov rax, 1029
		push rax
		mov rax, [rbp-440]; load GL_BACK
		pop rcx
		mov [rbp-440], rcx; store GL_BACK
		mov rax, [rbp-440]; load GL_BACK
		mov rax, 1030
		push rax
		mov rax, [rbp-448]; load GL_LEFT
		pop rcx
		mov [rbp-448], rcx; store GL_LEFT
		mov rax, [rbp-448]; load GL_LEFT
		mov rax, 1031
		push rax
		mov rax, [rbp-456]; load GL_RIGHT
		pop rcx
		mov [rbp-456], rcx; store GL_RIGHT
		mov rax, [rbp-456]; load GL_RIGHT
		mov rax, 1032
		push rax
		mov rax, [rbp-464]; load GL_FRONT_AND_BACK
		pop rcx
		mov [rbp-464], rcx; store GL_FRONT_AND_BACK
		mov rax, [rbp-464]; load GL_FRONT_AND_BACK
		mov rax, 0
		push rax
		mov rax, [rbp-472]; load GL_NO_ERROR
		pop rcx
		mov [rbp-472], rcx; store GL_NO_ERROR
		mov rax, [rbp-472]; load GL_NO_ERROR
		mov rax, 1280
		push rax
		mov rax, [rbp-480]; load GL_INVALID_ENUM
		pop rcx
		mov [rbp-480], rcx; store GL_INVALID_ENUM
		mov rax, [rbp-480]; load GL_INVALID_ENUM
		mov rax, 1281
		push rax
		mov rax, [rbp-488]; load GL_INVALID_VALUE
		pop rcx
		mov [rbp-488], rcx; store GL_INVALID_VALUE
		mov rax, [rbp-488]; load GL_INVALID_VALUE
		mov rax, 1282
		push rax
		mov rax, [rbp-496]; load GL_INVALID_OPERATION
		pop rcx
		mov [rbp-496], rcx; store GL_INVALID_OPERATION
		mov rax, [rbp-496]; load GL_INVALID_OPERATION
		mov rax, 1285
		push rax
		mov rax, [rbp-504]; load GL_OUT_OF_MEMORY
		pop rcx
		mov [rbp-504], rcx; store GL_OUT_OF_MEMORY
		mov rax, [rbp-504]; load GL_OUT_OF_MEMORY
		mov rax, 2304
		push rax
		mov rax, [rbp-512]; load GL_CW
		pop rcx
		mov [rbp-512], rcx; store GL_CW
		mov rax, [rbp-512]; load GL_CW
		mov rax, 2305
		push rax
		mov rax, [rbp-520]; load GL_CCW
		pop rcx
		mov [rbp-520], rcx; store GL_CCW
		mov rax, [rbp-520]; load GL_CCW
		mov rax, 2833
		push rax
		mov rax, [rbp-528]; load GL_POINT_SIZE
		pop rcx
		mov [rbp-528], rcx; store GL_POINT_SIZE
		mov rax, [rbp-528]; load GL_POINT_SIZE
		mov rax, 2834
		push rax
		mov rax, [rbp-536]; load GL_POINT_SIZE_RANGE
		pop rcx
		mov [rbp-536], rcx; store GL_POINT_SIZE_RANGE
		mov rax, [rbp-536]; load GL_POINT_SIZE_RANGE
		mov rax, 2835
		push rax
		mov rax, [rbp-544]; load GL_POINT_SIZE_GRANULARITY
		pop rcx
		mov [rbp-544], rcx; store GL_POINT_SIZE_GRANULARITY
		mov rax, [rbp-544]; load GL_POINT_SIZE_GRANULARITY
		mov rax, 2848
		push rax
		mov rax, [rbp-552]; load GL_LINE_SMOOTH
		pop rcx
		mov [rbp-552], rcx; store GL_LINE_SMOOTH
		mov rax, [rbp-552]; load GL_LINE_SMOOTH
		mov rax, 2849
		push rax
		mov rax, [rbp-560]; load GL_LINE_WIDTH
		pop rcx
		mov [rbp-560], rcx; store GL_LINE_WIDTH
		mov rax, [rbp-560]; load GL_LINE_WIDTH
		mov rax, 2850
		push rax
		mov rax, [rbp-568]; load GL_LINE_WIDTH_RANGE
		pop rcx
		mov [rbp-568], rcx; store GL_LINE_WIDTH_RANGE
		mov rax, [rbp-568]; load GL_LINE_WIDTH_RANGE
		mov rax, 2851
		push rax
		mov rax, [rbp-576]; load GL_LINE_WIDTH_GRANULARITY
		pop rcx
		mov [rbp-576], rcx; store GL_LINE_WIDTH_GRANULARITY
		mov rax, [rbp-576]; load GL_LINE_WIDTH_GRANULARITY
		mov rax, 2880
		push rax
		mov rax, [rbp-584]; load GL_POLYGON_MODE
		pop rcx
		mov [rbp-584], rcx; store GL_POLYGON_MODE
		mov rax, [rbp-584]; load GL_POLYGON_MODE
		mov rax, 2881
		push rax
		mov rax, [rbp-592]; load GL_POLYGON_SMOOTH
		pop rcx
		mov [rbp-592], rcx; store GL_POLYGON_SMOOTH
		mov rax, [rbp-592]; load GL_POLYGON_SMOOTH
		mov rax, 2884
		push rax
		mov rax, [rbp-600]; load GL_CULL_FACE
		pop rcx
		mov [rbp-600], rcx; store GL_CULL_FACE
		mov rax, [rbp-600]; load GL_CULL_FACE
		mov rax, 2885
		push rax
		mov rax, [rbp-608]; load GL_CULL_FACE_MODE
		pop rcx
		mov [rbp-608], rcx; store GL_CULL_FACE_MODE
		mov rax, [rbp-608]; load GL_CULL_FACE_MODE
		mov rax, 2886
		push rax
		mov rax, [rbp-616]; load GL_FRONT_FACE
		pop rcx
		mov [rbp-616], rcx; store GL_FRONT_FACE
		mov rax, [rbp-616]; load GL_FRONT_FACE
		mov rax, 2928
		push rax
		mov rax, [rbp-624]; load GL_DEPTH_RANGE
		pop rcx
		mov [rbp-624], rcx; store GL_DEPTH_RANGE
		mov rax, [rbp-624]; load GL_DEPTH_RANGE
		mov rax, 2929
		push rax
		mov rax, [rbp-632]; load GL_DEPTH_TEST
		pop rcx
		mov [rbp-632], rcx; store GL_DEPTH_TEST
		mov rax, [rbp-632]; load GL_DEPTH_TEST
		mov rax, 2930
		push rax
		mov rax, [rbp-640]; load GL_DEPTH_WRITEMASK
		pop rcx
		mov [rbp-640], rcx; store GL_DEPTH_WRITEMASK
		mov rax, [rbp-640]; load GL_DEPTH_WRITEMASK
		mov rax, 2931
		push rax
		mov rax, [rbp-648]; load GL_DEPTH_CLEAR_VALUE
		pop rcx
		mov [rbp-648], rcx; store GL_DEPTH_CLEAR_VALUE
		mov rax, [rbp-648]; load GL_DEPTH_CLEAR_VALUE
		mov rax, 2932
		push rax
		mov rax, [rbp-656]; load GL_DEPTH_FUNC
		pop rcx
		mov [rbp-656], rcx; store GL_DEPTH_FUNC
		mov rax, [rbp-656]; load GL_DEPTH_FUNC
		mov rax, 2960
		push rax
		mov rax, [rbp-664]; load GL_STENCIL_TEST
		pop rcx
		mov [rbp-664], rcx; store GL_STENCIL_TEST
		mov rax, [rbp-664]; load GL_STENCIL_TEST
		mov rax, 2961
		push rax
		mov rax, [rbp-672]; load GL_STENCIL_CLEAR_VALUE
		pop rcx
		mov [rbp-672], rcx; store GL_STENCIL_CLEAR_VALUE
		mov rax, [rbp-672]; load GL_STENCIL_CLEAR_VALUE
		mov rax, 2962
		push rax
		mov rax, [rbp-680]; load GL_STENCIL_FUNC
		pop rcx
		mov [rbp-680], rcx; store GL_STENCIL_FUNC
		mov rax, [rbp-680]; load GL_STENCIL_FUNC
		mov rax, 2963
		push rax
		mov rax, [rbp-688]; load GL_STENCIL_VALUE_MASK
		pop rcx
		mov [rbp-688], rcx; store GL_STENCIL_VALUE_MASK
		mov rax, [rbp-688]; load GL_STENCIL_VALUE_MASK
		mov rax, 2964
		push rax
		mov rax, [rbp-696]; load GL_STENCIL_FAIL
		pop rcx
		mov [rbp-696], rcx; store GL_STENCIL_FAIL
		mov rax, [rbp-696]; load GL_STENCIL_FAIL
		mov rax, 2965
		push rax
		mov rax, [rbp-704]; load GL_STENCIL_PASS_DEPTH_FAIL
		pop rcx
		mov [rbp-704], rcx; store GL_STENCIL_PASS_DEPTH_FAIL
		mov rax, [rbp-704]; load GL_STENCIL_PASS_DEPTH_FAIL
		mov rax, 2966
		push rax
		mov rax, [rbp-712]; load GL_STENCIL_PASS_DEPTH_PASS
		pop rcx
		mov [rbp-712], rcx; store GL_STENCIL_PASS_DEPTH_PASS
		mov rax, [rbp-712]; load GL_STENCIL_PASS_DEPTH_PASS
		mov rax, 2967
		push rax
		mov rax, [rbp-720]; load GL_STENCIL_REF
		pop rcx
		mov [rbp-720], rcx; store GL_STENCIL_REF
		mov rax, [rbp-720]; load GL_STENCIL_REF
		mov rax, 2968
		push rax
		mov rax, [rbp-728]; load GL_STENCIL_WRITEMASK
		pop rcx
		mov [rbp-728], rcx; store GL_STENCIL_WRITEMASK
		mov rax, [rbp-728]; load GL_STENCIL_WRITEMASK
		mov rax, 2978
		push rax
		mov rax, [rbp-736]; load GL_VIEWPORT
		pop rcx
		mov [rbp-736], rcx; store GL_VIEWPORT
		mov rax, [rbp-736]; load GL_VIEWPORT
		mov rax, 3024
		push rax
		mov rax, [rbp-744]; load GL_DITHER
		pop rcx
		mov [rbp-744], rcx; store GL_DITHER
		mov rax, [rbp-744]; load GL_DITHER
		mov rax, 3040
		push rax
		mov rax, [rbp-752]; load GL_BLEND_DST
		pop rcx
		mov [rbp-752], rcx; store GL_BLEND_DST
		mov rax, [rbp-752]; load GL_BLEND_DST
		mov rax, 3041
		push rax
		mov rax, [rbp-760]; load GL_BLEND_SRC
		pop rcx
		mov [rbp-760], rcx; store GL_BLEND_SRC
		mov rax, [rbp-760]; load GL_BLEND_SRC
		mov rax, 3042
		push rax
		mov rax, [rbp-768]; load GL_BLEND
		pop rcx
		mov [rbp-768], rcx; store GL_BLEND
		mov rax, [rbp-768]; load GL_BLEND
		mov rax, 3056
		push rax
		mov rax, [rbp-776]; load GL_LOGIC_OP_MODE
		pop rcx
		mov [rbp-776], rcx; store GL_LOGIC_OP_MODE
		mov rax, [rbp-776]; load GL_LOGIC_OP_MODE
		mov rax, 3058
		push rax
		mov rax, [rbp-784]; load GL_COLOR_LOGIC_OP
		pop rcx
		mov [rbp-784], rcx; store GL_COLOR_LOGIC_OP
		mov rax, [rbp-784]; load GL_COLOR_LOGIC_OP
		mov rax, 3073
		push rax
		mov rax, [rbp-792]; load GL_DRAW_BUFFER
		pop rcx
		mov [rbp-792], rcx; store GL_DRAW_BUFFER
		mov rax, [rbp-792]; load GL_DRAW_BUFFER
		mov rax, 3074
		push rax
		mov rax, [rbp-800]; load GL_READ_BUFFER
		pop rcx
		mov [rbp-800], rcx; store GL_READ_BUFFER
		mov rax, [rbp-800]; load GL_READ_BUFFER
		mov rax, 3088
		push rax
		mov rax, [rbp-808]; load GL_SCISSOR_BOX
		pop rcx
		mov [rbp-808], rcx; store GL_SCISSOR_BOX
		mov rax, [rbp-808]; load GL_SCISSOR_BOX
		mov rax, 3089
		push rax
		mov rax, [rbp-816]; load GL_SCISSOR_TEST
		pop rcx
		mov [rbp-816], rcx; store GL_SCISSOR_TEST
		mov rax, [rbp-816]; load GL_SCISSOR_TEST
		mov rax, 3106
		push rax
		mov rax, [rbp-824]; load GL_COLOR_CLEAR_VALUE
		pop rcx
		mov [rbp-824], rcx; store GL_COLOR_CLEAR_VALUE
		mov rax, [rbp-824]; load GL_COLOR_CLEAR_VALUE
		mov rax, 3107
		push rax
		mov rax, [rbp-832]; load GL_COLOR_WRITEMASK
		pop rcx
		mov [rbp-832], rcx; store GL_COLOR_WRITEMASK
		mov rax, [rbp-832]; load GL_COLOR_WRITEMASK
		mov rax, 3122
		push rax
		mov rax, [rbp-840]; load GL_DOUBLEBUFFER
		pop rcx
		mov [rbp-840], rcx; store GL_DOUBLEBUFFER
		mov rax, [rbp-840]; load GL_DOUBLEBUFFER
		mov rax, 3123
		push rax
		mov rax, [rbp-848]; load GL_STEREO
		pop rcx
		mov [rbp-848], rcx; store GL_STEREO
		mov rax, [rbp-848]; load GL_STEREO
		mov rax, 3154
		push rax
		mov rax, [rbp-856]; load GL_LINE_SMOOTH_HINT
		pop rcx
		mov [rbp-856], rcx; store GL_LINE_SMOOTH_HINT
		mov rax, [rbp-856]; load GL_LINE_SMOOTH_HINT
		mov rax, 3155
		push rax
		mov rax, [rbp-864]; load GL_POLYGON_SMOOTH_HINT
		pop rcx
		mov [rbp-864], rcx; store GL_POLYGON_SMOOTH_HINT
		mov rax, [rbp-864]; load GL_POLYGON_SMOOTH_HINT
		mov rax, 3312
		push rax
		mov rax, [rbp-872]; load GL_UNPACK_SWAP_BYTES
		pop rcx
		mov [rbp-872], rcx; store GL_UNPACK_SWAP_BYTES
		mov rax, [rbp-872]; load GL_UNPACK_SWAP_BYTES
		mov rax, 3313
		push rax
		mov rax, [rbp-880]; load GL_UNPACK_LSB_FIRST
		pop rcx
		mov [rbp-880], rcx; store GL_UNPACK_LSB_FIRST
		mov rax, [rbp-880]; load GL_UNPACK_LSB_FIRST
		mov rax, 3314
		push rax
		mov rax, [rbp-888]; load GL_UNPACK_ROW_LENGTH
		pop rcx
		mov [rbp-888], rcx; store GL_UNPACK_ROW_LENGTH
		mov rax, [rbp-888]; load GL_UNPACK_ROW_LENGTH
		mov rax, 3315
		push rax
		mov rax, [rbp-896]; load GL_UNPACK_SKIP_ROWS
		pop rcx
		mov [rbp-896], rcx; store GL_UNPACK_SKIP_ROWS
		mov rax, [rbp-896]; load GL_UNPACK_SKIP_ROWS
		mov rax, 3316
		push rax
		mov rax, [rbp-904]; load GL_UNPACK_SKIP_PIXELS
		pop rcx
		mov [rbp-904], rcx; store GL_UNPACK_SKIP_PIXELS
		mov rax, [rbp-904]; load GL_UNPACK_SKIP_PIXELS
		mov rax, 3317
		push rax
		mov rax, [rbp-912]; load GL_UNPACK_ALIGNMENT
		pop rcx
		mov [rbp-912], rcx; store GL_UNPACK_ALIGNMENT
		mov rax, [rbp-912]; load GL_UNPACK_ALIGNMENT
		mov rax, 3328
		push rax
		mov rax, [rbp-920]; load GL_PACK_SWAP_BYTES
		pop rcx
		mov [rbp-920], rcx; store GL_PACK_SWAP_BYTES
		mov rax, [rbp-920]; load GL_PACK_SWAP_BYTES
		mov rax, 3329
		push rax
		mov rax, [rbp-928]; load GL_PACK_LSB_FIRST
		pop rcx
		mov [rbp-928], rcx; store GL_PACK_LSB_FIRST
		mov rax, [rbp-928]; load GL_PACK_LSB_FIRST
		mov rax, 3330
		push rax
		mov rax, [rbp-936]; load GL_PACK_ROW_LENGTH
		pop rcx
		mov [rbp-936], rcx; store GL_PACK_ROW_LENGTH
		mov rax, [rbp-936]; load GL_PACK_ROW_LENGTH
		mov rax, 3331
		push rax
		mov rax, [rbp-944]; load GL_PACK_SKIP_ROWS
		pop rcx
		mov [rbp-944], rcx; store GL_PACK_SKIP_ROWS
		mov rax, [rbp-944]; load GL_PACK_SKIP_ROWS
		mov rax, 3332
		push rax
		mov rax, [rbp-952]; load GL_PACK_SKIP_PIXELS
		pop rcx
		mov [rbp-952], rcx; store GL_PACK_SKIP_PIXELS
		mov rax, [rbp-952]; load GL_PACK_SKIP_PIXELS
		mov rax, 3333
		push rax
		mov rax, [rbp-960]; load GL_PACK_ALIGNMENT
		pop rcx
		mov [rbp-960], rcx; store GL_PACK_ALIGNMENT
		mov rax, [rbp-960]; load GL_PACK_ALIGNMENT
		mov rax, 3379
		push rax
		mov rax, [rbp-968]; load GL_MAX_TEXTURE_SIZE
		pop rcx
		mov [rbp-968], rcx; store GL_MAX_TEXTURE_SIZE
		mov rax, [rbp-968]; load GL_MAX_TEXTURE_SIZE
		mov rax, 3386
		push rax
		mov rax, [rbp-976]; load GL_MAX_VIEWPORT_DIMS
		pop rcx
		mov [rbp-976], rcx; store GL_MAX_VIEWPORT_DIMS
		mov rax, [rbp-976]; load GL_MAX_VIEWPORT_DIMS
		mov rax, 3408
		push rax
		mov rax, [rbp-984]; load GL_SUBPIXEL_BITS
		pop rcx
		mov [rbp-984], rcx; store GL_SUBPIXEL_BITS
		mov rax, [rbp-984]; load GL_SUBPIXEL_BITS
		mov rax, 3552
		push rax
		mov rax, [rbp-992]; load GL_TEXTURE_1D
		pop rcx
		mov [rbp-992], rcx; store GL_TEXTURE_1D
		mov rax, [rbp-992]; load GL_TEXTURE_1D
		mov rax, 3553
		push rax
		mov rax, [rbp-1000]; load GL_TEXTURE_2D
		pop rcx
		mov [rbp-1000], rcx; store GL_TEXTURE_2D
		mov rax, [rbp-1000]; load GL_TEXTURE_2D
		mov rax, 10752
		push rax
		mov rax, [rbp-1008]; load GL_POLYGON_OFFSET_UNITS
		pop rcx
		mov [rbp-1008], rcx; store GL_POLYGON_OFFSET_UNITS
		mov rax, [rbp-1008]; load GL_POLYGON_OFFSET_UNITS
		mov rax, 10753
		push rax
		mov rax, [rbp-1016]; load GL_POLYGON_OFFSET_POINT
		pop rcx
		mov [rbp-1016], rcx; store GL_POLYGON_OFFSET_POINT
		mov rax, [rbp-1016]; load GL_POLYGON_OFFSET_POINT
		mov rax, 10754
		push rax
		mov rax, [rbp-1024]; load GL_POLYGON_OFFSET_LINE
		pop rcx
		mov [rbp-1024], rcx; store GL_POLYGON_OFFSET_LINE
		mov rax, [rbp-1024]; load GL_POLYGON_OFFSET_LINE
		mov rax, 32823
		push rax
		mov rax, [rbp-1032]; load GL_POLYGON_OFFSET_FILL
		pop rcx
		mov [rbp-1032], rcx; store GL_POLYGON_OFFSET_FILL
		mov rax, [rbp-1032]; load GL_POLYGON_OFFSET_FILL
		mov rax, 32824
		push rax
		mov rax, [rbp-1040]; load GL_POLYGON_OFFSET_FACTOR
		pop rcx
		mov [rbp-1040], rcx; store GL_POLYGON_OFFSET_FACTOR
		mov rax, [rbp-1040]; load GL_POLYGON_OFFSET_FACTOR
		mov rax, 32872
		push rax
		mov rax, [rbp-1048]; load GL_TEXTURE_BINDING_1D
		pop rcx
		mov [rbp-1048], rcx; store GL_TEXTURE_BINDING_1D
		mov rax, [rbp-1048]; load GL_TEXTURE_BINDING_1D
		mov rax, 32873
		push rax
		mov rax, [rbp-1056]; load GL_TEXTURE_BINDING_2D
		pop rcx
		mov [rbp-1056], rcx; store GL_TEXTURE_BINDING_2D
		mov rax, [rbp-1056]; load GL_TEXTURE_BINDING_2D
		mov rax, 4096
		push rax
		mov rax, [rbp-1064]; load GL_TEXTURE_WIDTH
		pop rcx
		mov [rbp-1064], rcx; store GL_TEXTURE_WIDTH
		mov rax, [rbp-1064]; load GL_TEXTURE_WIDTH
		mov rax, 4097
		push rax
		mov rax, [rbp-1072]; load GL_TEXTURE_HEIGHT
		pop rcx
		mov [rbp-1072], rcx; store GL_TEXTURE_HEIGHT
		mov rax, [rbp-1072]; load GL_TEXTURE_HEIGHT
		mov rax, 4099
		push rax
		mov rax, [rbp-1080]; load GL_TEXTURE_INTERNAL_FORMAT
		pop rcx
		mov [rbp-1080], rcx; store GL_TEXTURE_INTERNAL_FORMAT
		mov rax, [rbp-1080]; load GL_TEXTURE_INTERNAL_FORMAT
		mov rax, 4100
		push rax
		mov rax, [rbp-1088]; load GL_TEXTURE_BORDER_COLOR
		pop rcx
		mov [rbp-1088], rcx; store GL_TEXTURE_BORDER_COLOR
		mov rax, [rbp-1088]; load GL_TEXTURE_BORDER_COLOR
		mov rax, 32860
		push rax
		mov rax, [rbp-1096]; load GL_TEXTURE_RED_SIZE
		pop rcx
		mov [rbp-1096], rcx; store GL_TEXTURE_RED_SIZE
		mov rax, [rbp-1096]; load GL_TEXTURE_RED_SIZE
		mov rax, 32861
		push rax
		mov rax, [rbp-1104]; load GL_TEXTURE_GREEN_SIZE
		pop rcx
		mov [rbp-1104], rcx; store GL_TEXTURE_GREEN_SIZE
		mov rax, [rbp-1104]; load GL_TEXTURE_GREEN_SIZE
		mov rax, 32862
		push rax
		mov rax, [rbp-1112]; load GL_TEXTURE_BLUE_SIZE
		pop rcx
		mov [rbp-1112], rcx; store GL_TEXTURE_BLUE_SIZE
		mov rax, [rbp-1112]; load GL_TEXTURE_BLUE_SIZE
		mov rax, 32863
		push rax
		mov rax, [rbp-1120]; load GL_TEXTURE_ALPHA_SIZE
		pop rcx
		mov [rbp-1120], rcx; store GL_TEXTURE_ALPHA_SIZE
		mov rax, [rbp-1120]; load GL_TEXTURE_ALPHA_SIZE
		mov rax, 4352
		push rax
		mov rax, [rbp-1128]; load GL_DONT_CARE
		pop rcx
		mov [rbp-1128], rcx; store GL_DONT_CARE
		mov rax, [rbp-1128]; load GL_DONT_CARE
		mov rax, 4353
		push rax
		mov rax, [rbp-1136]; load GL_FASTEST
		pop rcx
		mov [rbp-1136], rcx; store GL_FASTEST
		mov rax, [rbp-1136]; load GL_FASTEST
		mov rax, 4354
		push rax
		mov rax, [rbp-1144]; load GL_NICEST
		pop rcx
		mov [rbp-1144], rcx; store GL_NICEST
		mov rax, [rbp-1144]; load GL_NICEST
		mov rax, 5120
		push rax
		mov rax, [rbp-1152]; load GL_BYTE
		pop rcx
		mov [rbp-1152], rcx; store GL_BYTE
		mov rax, [rbp-1152]; load GL_BYTE
		mov rax, 5121
		push rax
		mov rax, [rbp-1160]; load GL_UNSIGNED_BYTE
		pop rcx
		mov [rbp-1160], rcx; store GL_UNSIGNED_BYTE
		mov rax, [rbp-1160]; load GL_UNSIGNED_BYTE
		mov rax, 5122
		push rax
		mov rax, [rbp-1168]; load GL_SHORT
		pop rcx
		mov [rbp-1168], rcx; store GL_SHORT
		mov rax, [rbp-1168]; load GL_SHORT
		mov rax, 5123
		push rax
		mov rax, [rbp-1176]; load GL_UNSIGNED_SHORT
		pop rcx
		mov [rbp-1176], rcx; store GL_UNSIGNED_SHORT
		mov rax, [rbp-1176]; load GL_UNSIGNED_SHORT
		mov rax, 5124
		push rax
		mov rax, [rbp-1184]; load GL_INT
		pop rcx
		mov [rbp-1184], rcx; store GL_INT
		mov rax, [rbp-1184]; load GL_INT
		mov rax, 5125
		push rax
		mov rax, [rbp-1192]; load GL_UNSIGNED_INT
		pop rcx
		mov [rbp-1192], rcx; store GL_UNSIGNED_INT
		mov rax, [rbp-1192]; load GL_UNSIGNED_INT
		mov rax, 5126
		push rax
		mov rax, [rbp-1200]; load GL_FLOAT
		pop rcx
		mov [rbp-1200], rcx; store GL_FLOAT
		mov rax, [rbp-1200]; load GL_FLOAT
		mov rax, 5130
		push rax
		mov rax, [rbp-1208]; load GL_DOUBLE
		pop rcx
		mov [rbp-1208], rcx; store GL_DOUBLE
		mov rax, [rbp-1208]; load GL_DOUBLE
		mov rax, 1283
		push rax
		mov rax, [rbp-1216]; load GL_STACK_OVERFLOW
		pop rcx
		mov [rbp-1216], rcx; store GL_STACK_OVERFLOW
		mov rax, [rbp-1216]; load GL_STACK_OVERFLOW
		mov rax, 1284
		push rax
		mov rax, [rbp-1224]; load GL_STACK_UNDERFLOW
		pop rcx
		mov [rbp-1224], rcx; store GL_STACK_UNDERFLOW
		mov rax, [rbp-1224]; load GL_STACK_UNDERFLOW
		mov rax, 5376
		push rax
		mov rax, [rbp-1232]; load GL_CLEAR
		pop rcx
		mov [rbp-1232], rcx; store GL_CLEAR
		mov rax, [rbp-1232]; load GL_CLEAR
		mov rax, 5377
		push rax
		mov rax, [rbp-1240]; load GL_AND
		pop rcx
		mov [rbp-1240], rcx; store GL_AND
		mov rax, [rbp-1240]; load GL_AND
		mov rax, 5378
		push rax
		mov rax, [rbp-1248]; load GL_AND_REVERSE
		pop rcx
		mov [rbp-1248], rcx; store GL_AND_REVERSE
		mov rax, [rbp-1248]; load GL_AND_REVERSE
		mov rax, 5379
		push rax
		mov rax, [rbp-1256]; load GL_COPY
		pop rcx
		mov [rbp-1256], rcx; store GL_COPY
		mov rax, [rbp-1256]; load GL_COPY
		mov rax, 5380
		push rax
		mov rax, [rbp-1264]; load GL_AND_INVERTED
		pop rcx
		mov [rbp-1264], rcx; store GL_AND_INVERTED
		mov rax, [rbp-1264]; load GL_AND_INVERTED
		mov rax, 5381
		push rax
		mov rax, [rbp-1272]; load GL_NOOP
		pop rcx
		mov [rbp-1272], rcx; store GL_NOOP
		mov rax, [rbp-1272]; load GL_NOOP
		mov rax, 5382
		push rax
		mov rax, [rbp-1280]; load GL_XOR
		pop rcx
		mov [rbp-1280], rcx; store GL_XOR
		mov rax, [rbp-1280]; load GL_XOR
		mov rax, 5383
		push rax
		mov rax, [rbp-1288]; load GL_OR
		pop rcx
		mov [rbp-1288], rcx; store GL_OR
		mov rax, [rbp-1288]; load GL_OR
		mov rax, 5384
		push rax
		mov rax, [rbp-1296]; load GL_NOR
		pop rcx
		mov [rbp-1296], rcx; store GL_NOR
		mov rax, [rbp-1296]; load GL_NOR
		mov rax, 5385
		push rax
		mov rax, [rbp-1304]; load GL_EQUIV
		pop rcx
		mov [rbp-1304], rcx; store GL_EQUIV
		mov rax, [rbp-1304]; load GL_EQUIV
		mov rax, 5386
		push rax
		mov rax, [rbp-1312]; load GL_INVERT
		pop rcx
		mov [rbp-1312], rcx; store GL_INVERT
		mov rax, [rbp-1312]; load GL_INVERT
		mov rax, 5387
		push rax
		mov rax, [rbp-1320]; load GL_OR_REVERSE
		pop rcx
		mov [rbp-1320], rcx; store GL_OR_REVERSE
		mov rax, [rbp-1320]; load GL_OR_REVERSE
		mov rax, 5388
		push rax
		mov rax, [rbp-1328]; load GL_COPY_INVERTED
		pop rcx
		mov [rbp-1328], rcx; store GL_COPY_INVERTED
		mov rax, [rbp-1328]; load GL_COPY_INVERTED
		mov rax, 5389
		push rax
		mov rax, [rbp-1336]; load GL_OR_INVERTED
		pop rcx
		mov [rbp-1336], rcx; store GL_OR_INVERTED
		mov rax, [rbp-1336]; load GL_OR_INVERTED
		mov rax, 5390
		push rax
		mov rax, [rbp-1344]; load GL_NAND
		pop rcx
		mov [rbp-1344], rcx; store GL_NAND
		mov rax, [rbp-1344]; load GL_NAND
		mov rax, 5391
		push rax
		mov rax, [rbp-1352]; load GL_SET
		pop rcx
		mov [rbp-1352], rcx; store GL_SET
		mov rax, [rbp-1352]; load GL_SET
		mov rax, 5890
		push rax
		mov rax, [rbp-1360]; load GL_TEXTURE
		pop rcx
		mov [rbp-1360], rcx; store GL_TEXTURE
		mov rax, [rbp-1360]; load GL_TEXTURE
		mov rax, 6144
		push rax
		mov rax, [rbp-1368]; load GL_COLOR
		pop rcx
		mov [rbp-1368], rcx; store GL_COLOR
		mov rax, [rbp-1368]; load GL_COLOR
		mov rax, 6145
		push rax
		mov rax, [rbp-1376]; load GL_DEPTH
		pop rcx
		mov [rbp-1376], rcx; store GL_DEPTH
		mov rax, [rbp-1376]; load GL_DEPTH
		mov rax, 6146
		push rax
		mov rax, [rbp-1384]; load GL_STENCIL
		pop rcx
		mov [rbp-1384], rcx; store GL_STENCIL
		mov rax, [rbp-1384]; load GL_STENCIL
		mov rax, 6401
		push rax
		mov rax, [rbp-1392]; load GL_STENCIL_INDEX
		pop rcx
		mov [rbp-1392], rcx; store GL_STENCIL_INDEX
		mov rax, [rbp-1392]; load GL_STENCIL_INDEX
		mov rax, 6402
		push rax
		mov rax, [rbp-1400]; load GL_DEPTH_COMPONENT
		pop rcx
		mov [rbp-1400], rcx; store GL_DEPTH_COMPONENT
		mov rax, [rbp-1400]; load GL_DEPTH_COMPONENT
		mov rax, 6403
		push rax
		mov rax, [rbp-1408]; load GL_RED
		pop rcx
		mov [rbp-1408], rcx; store GL_RED
		mov rax, [rbp-1408]; load GL_RED
		mov rax, 6404
		push rax
		mov rax, [rbp-1416]; load GL_GREEN
		pop rcx
		mov [rbp-1416], rcx; store GL_GREEN
		mov rax, [rbp-1416]; load GL_GREEN
		mov rax, 6405
		push rax
		mov rax, [rbp-1424]; load GL_BLUE
		pop rcx
		mov [rbp-1424], rcx; store GL_BLUE
		mov rax, [rbp-1424]; load GL_BLUE
		mov rax, 6406
		push rax
		mov rax, [rbp-1432]; load GL_ALPHA
		pop rcx
		mov [rbp-1432], rcx; store GL_ALPHA
		mov rax, [rbp-1432]; load GL_ALPHA
		mov rax, 6407
		push rax
		mov rax, [rbp-1440]; load GL_RGB
		pop rcx
		mov [rbp-1440], rcx; store GL_RGB
		mov rax, [rbp-1440]; load GL_RGB
		mov rax, 6408
		push rax
		mov rax, [rbp-1448]; load GL_RGBA
		pop rcx
		mov [rbp-1448], rcx; store GL_RGBA
		mov rax, [rbp-1448]; load GL_RGBA
		mov rax, 6912
		push rax
		mov rax, [rbp-1456]; load GL_POINT
		pop rcx
		mov [rbp-1456], rcx; store GL_POINT
		mov rax, [rbp-1456]; load GL_POINT
		mov rax, 6913
		push rax
		mov rax, [rbp-1464]; load GL_LINE
		pop rcx
		mov [rbp-1464], rcx; store GL_LINE
		mov rax, [rbp-1464]; load GL_LINE
		mov rax, 6914
		push rax
		mov rax, [rbp-1472]; load GL_FILL
		pop rcx
		mov [rbp-1472], rcx; store GL_FILL
		mov rax, [rbp-1472]; load GL_FILL
		mov rax, 7680
		push rax
		mov rax, [rbp-1480]; load GL_KEEP
		pop rcx
		mov [rbp-1480], rcx; store GL_KEEP
		mov rax, [rbp-1480]; load GL_KEEP
		mov rax, 7681
		push rax
		mov rax, [rbp-1488]; load GL_REPLACE
		pop rcx
		mov [rbp-1488], rcx; store GL_REPLACE
		mov rax, [rbp-1488]; load GL_REPLACE
		mov rax, 7682
		push rax
		mov rax, [rbp-1496]; load GL_INCR
		pop rcx
		mov [rbp-1496], rcx; store GL_INCR
		mov rax, [rbp-1496]; load GL_INCR
		mov rax, 7683
		push rax
		mov rax, [rbp-1504]; load GL_DECR
		pop rcx
		mov [rbp-1504], rcx; store GL_DECR
		mov rax, [rbp-1504]; load GL_DECR
		mov rax, 7936
		push rax
		mov rax, [rbp-1512]; load GL_VENDOR
		pop rcx
		mov [rbp-1512], rcx; store GL_VENDOR
		mov rax, [rbp-1512]; load GL_VENDOR
		mov rax, 7937
		push rax
		mov rax, [rbp-1520]; load GL_RENDERER
		pop rcx
		mov [rbp-1520], rcx; store GL_RENDERER
		mov rax, [rbp-1520]; load GL_RENDERER
		mov rax, 7938
		push rax
		mov rax, [rbp-1528]; load GL_VERSION
		pop rcx
		mov [rbp-1528], rcx; store GL_VERSION
		mov rax, [rbp-1528]; load GL_VERSION
		mov rax, 7939
		push rax
		mov rax, [rbp-1536]; load GL_EXTENSIONS
		pop rcx
		mov [rbp-1536], rcx; store GL_EXTENSIONS
		mov rax, [rbp-1536]; load GL_EXTENSIONS
		mov rax, 9728
		push rax
		mov rax, [rbp-1544]; load GL_NEAREST
		pop rcx
		mov [rbp-1544], rcx; store GL_NEAREST
		mov rax, [rbp-1544]; load GL_NEAREST
		mov rax, 9729
		push rax
		mov rax, [rbp-1552]; load GL_LINEAR
		pop rcx
		mov [rbp-1552], rcx; store GL_LINEAR
		mov rax, [rbp-1552]; load GL_LINEAR
		mov rax, 9984
		push rax
		mov rax, [rbp-1560]; load GL_NEAREST_MIPMAP_NEAREST
		pop rcx
		mov [rbp-1560], rcx; store GL_NEAREST_MIPMAP_NEAREST
		mov rax, [rbp-1560]; load GL_NEAREST_MIPMAP_NEAREST
		mov rax, 9985
		push rax
		mov rax, [rbp-1568]; load GL_LINEAR_MIPMAP_NEAREST
		pop rcx
		mov [rbp-1568], rcx; store GL_LINEAR_MIPMAP_NEAREST
		mov rax, [rbp-1568]; load GL_LINEAR_MIPMAP_NEAREST
		mov rax, 9986
		push rax
		mov rax, [rbp-1576]; load GL_NEAREST_MIPMAP_LINEAR
		pop rcx
		mov [rbp-1576], rcx; store GL_NEAREST_MIPMAP_LINEAR
		mov rax, [rbp-1576]; load GL_NEAREST_MIPMAP_LINEAR
		mov rax, 9987
		push rax
		mov rax, [rbp-1584]; load GL_LINEAR_MIPMAP_LINEAR
		pop rcx
		mov [rbp-1584], rcx; store GL_LINEAR_MIPMAP_LINEAR
		mov rax, [rbp-1584]; load GL_LINEAR_MIPMAP_LINEAR
		mov rax, 10240
		push rax
		mov rax, [rbp-1592]; load GL_TEXTURE_MAG_FILTER
		pop rcx
		mov [rbp-1592], rcx; store GL_TEXTURE_MAG_FILTER
		mov rax, [rbp-1592]; load GL_TEXTURE_MAG_FILTER
		mov rax, 10241
		push rax
		mov rax, [rbp-1600]; load GL_TEXTURE_MIN_FILTER
		pop rcx
		mov [rbp-1600], rcx; store GL_TEXTURE_MIN_FILTER
		mov rax, [rbp-1600]; load GL_TEXTURE_MIN_FILTER
		mov rax, 10242
		push rax
		mov rax, [rbp-1608]; load GL_TEXTURE_WRAP_S
		pop rcx
		mov [rbp-1608], rcx; store GL_TEXTURE_WRAP_S
		mov rax, [rbp-1608]; load GL_TEXTURE_WRAP_S
		mov rax, 10243
		push rax
		mov rax, [rbp-1616]; load GL_TEXTURE_WRAP_T
		pop rcx
		mov [rbp-1616], rcx; store GL_TEXTURE_WRAP_T
		mov rax, [rbp-1616]; load GL_TEXTURE_WRAP_T
		mov rax, 32867
		push rax
		mov rax, [rbp-1624]; load GL_PROXY_TEXTURE_1D
		pop rcx
		mov [rbp-1624], rcx; store GL_PROXY_TEXTURE_1D
		mov rax, [rbp-1624]; load GL_PROXY_TEXTURE_1D
		mov rax, 32868
		push rax
		mov rax, [rbp-1632]; load GL_PROXY_TEXTURE_2D
		pop rcx
		mov [rbp-1632], rcx; store GL_PROXY_TEXTURE_2D
		mov rax, [rbp-1632]; load GL_PROXY_TEXTURE_2D
		mov rax, 10497
		push rax
		mov rax, [rbp-1640]; load GL_REPEAT
		pop rcx
		mov [rbp-1640], rcx; store GL_REPEAT
		mov rax, [rbp-1640]; load GL_REPEAT
		mov rax, 10768
		push rax
		mov rax, [rbp-1648]; load GL_R3_G3_B2
		pop rcx
		mov [rbp-1648], rcx; store GL_R3_G3_B2
		mov rax, [rbp-1648]; load GL_R3_G3_B2
		mov rax, 32847
		push rax
		mov rax, [rbp-1656]; load GL_RGB4
		pop rcx
		mov [rbp-1656], rcx; store GL_RGB4
		mov rax, [rbp-1656]; load GL_RGB4
		mov rax, 32848
		push rax
		mov rax, [rbp-1664]; load GL_RGB5
		pop rcx
		mov [rbp-1664], rcx; store GL_RGB5
		mov rax, [rbp-1664]; load GL_RGB5
		mov rax, 32849
		push rax
		mov rax, [rbp-1672]; load GL_RGB8
		pop rcx
		mov [rbp-1672], rcx; store GL_RGB8
		mov rax, [rbp-1672]; load GL_RGB8
		mov rax, 32850
		push rax
		mov rax, [rbp-1680]; load GL_RGB10
		pop rcx
		mov [rbp-1680], rcx; store GL_RGB10
		mov rax, [rbp-1680]; load GL_RGB10
		mov rax, 32851
		push rax
		mov rax, [rbp-1688]; load GL_RGB12
		pop rcx
		mov [rbp-1688], rcx; store GL_RGB12
		mov rax, [rbp-1688]; load GL_RGB12
		mov rax, 32852
		push rax
		mov rax, [rbp-1696]; load GL_RGB16
		pop rcx
		mov [rbp-1696], rcx; store GL_RGB16
		mov rax, [rbp-1696]; load GL_RGB16
		mov rax, 32853
		push rax
		mov rax, [rbp-1704]; load GL_RGBA2
		pop rcx
		mov [rbp-1704], rcx; store GL_RGBA2
		mov rax, [rbp-1704]; load GL_RGBA2
		mov rax, 32854
		push rax
		mov rax, [rbp-1712]; load GL_RGBA4
		pop rcx
		mov [rbp-1712], rcx; store GL_RGBA4
		mov rax, [rbp-1712]; load GL_RGBA4
		mov rax, 32855
		push rax
		mov rax, [rbp-1720]; load GL_RGB5_A1
		pop rcx
		mov [rbp-1720], rcx; store GL_RGB5_A1
		mov rax, [rbp-1720]; load GL_RGB5_A1
		mov rax, 32856
		push rax
		mov rax, [rbp-1728]; load GL_RGBA8
		pop rcx
		mov [rbp-1728], rcx; store GL_RGBA8
		mov rax, [rbp-1728]; load GL_RGBA8
		mov rax, 32857
		push rax
		mov rax, [rbp-1736]; load GL_RGB10_A2
		pop rcx
		mov [rbp-1736], rcx; store GL_RGB10_A2
		mov rax, [rbp-1736]; load GL_RGB10_A2
		mov rax, 32858
		push rax
		mov rax, [rbp-1744]; load GL_RGBA12
		pop rcx
		mov [rbp-1744], rcx; store GL_RGBA12
		mov rax, [rbp-1744]; load GL_RGBA12
		mov rax, 32859
		push rax
		mov rax, [rbp-1752]; load GL_RGBA16
		pop rcx
		mov [rbp-1752], rcx; store GL_RGBA16
		mov rax, [rbp-1752]; load GL_RGBA16
		mov rax, 32884
		push rax
		mov rax, [rbp-1760]; load GL_VERTEX_ARRAY
		pop rcx
		mov [rbp-1760], rcx; store GL_VERTEX_ARRAY
		mov rax, [rbp-1760]; load GL_VERTEX_ARRAY
		mov rax, 32818
		push rax
		mov rax, [rbp-1768]; load GL_UNSIGNED_BYTE_3_3_2
		pop rcx
		mov [rbp-1768], rcx; store GL_UNSIGNED_BYTE_3_3_2
		mov rax, [rbp-1768]; load GL_UNSIGNED_BYTE_3_3_2
		mov rax, 32819
		push rax
		mov rax, [rbp-1776]; load GL_UNSIGNED_SHORT_4_4_4_4
		pop rcx
		mov [rbp-1776], rcx; store GL_UNSIGNED_SHORT_4_4_4_4
		mov rax, [rbp-1776]; load GL_UNSIGNED_SHORT_4_4_4_4
		mov rax, 32820
		push rax
		mov rax, [rbp-1784]; load GL_UNSIGNED_SHORT_5_5_5_1
		pop rcx
		mov [rbp-1784], rcx; store GL_UNSIGNED_SHORT_5_5_5_1
		mov rax, [rbp-1784]; load GL_UNSIGNED_SHORT_5_5_5_1
		mov rax, 32821
		push rax
		mov rax, [rbp-1792]; load GL_UNSIGNED_INT_8_8_8_8
		pop rcx
		mov [rbp-1792], rcx; store GL_UNSIGNED_INT_8_8_8_8
		mov rax, [rbp-1792]; load GL_UNSIGNED_INT_8_8_8_8
		mov rax, 32822
		push rax
		mov rax, [rbp-1800]; load GL_UNSIGNED_INT_10_10_10_2
		pop rcx
		mov [rbp-1800], rcx; store GL_UNSIGNED_INT_10_10_10_2
		mov rax, [rbp-1800]; load GL_UNSIGNED_INT_10_10_10_2
		mov rax, 32874
		push rax
		mov rax, [rbp-1808]; load GL_TEXTURE_BINDING_3D
		pop rcx
		mov [rbp-1808], rcx; store GL_TEXTURE_BINDING_3D
		mov rax, [rbp-1808]; load GL_TEXTURE_BINDING_3D
		mov rax, 32875
		push rax
		mov rax, [rbp-1816]; load GL_PACK_SKIP_IMAGES
		pop rcx
		mov [rbp-1816], rcx; store GL_PACK_SKIP_IMAGES
		mov rax, [rbp-1816]; load GL_PACK_SKIP_IMAGES
		mov rax, 32876
		push rax
		mov rax, [rbp-1824]; load GL_PACK_IMAGE_HEIGHT
		pop rcx
		mov [rbp-1824], rcx; store GL_PACK_IMAGE_HEIGHT
		mov rax, [rbp-1824]; load GL_PACK_IMAGE_HEIGHT
		mov rax, 32877
		push rax
		mov rax, [rbp-1832]; load GL_UNPACK_SKIP_IMAGES
		pop rcx
		mov [rbp-1832], rcx; store GL_UNPACK_SKIP_IMAGES
		mov rax, [rbp-1832]; load GL_UNPACK_SKIP_IMAGES
		mov rax, 32878
		push rax
		mov rax, [rbp-1840]; load GL_UNPACK_IMAGE_HEIGHT
		pop rcx
		mov [rbp-1840], rcx; store GL_UNPACK_IMAGE_HEIGHT
		mov rax, [rbp-1840]; load GL_UNPACK_IMAGE_HEIGHT
		mov rax, 32879
		push rax
		mov rax, [rbp-1848]; load GL_TEXTURE_3D
		pop rcx
		mov [rbp-1848], rcx; store GL_TEXTURE_3D
		mov rax, [rbp-1848]; load GL_TEXTURE_3D
		mov rax, 32880
		push rax
		mov rax, [rbp-1856]; load GL_PROXY_TEXTURE_3D
		pop rcx
		mov [rbp-1856], rcx; store GL_PROXY_TEXTURE_3D
		mov rax, [rbp-1856]; load GL_PROXY_TEXTURE_3D
		mov rax, 32881
		push rax
		mov rax, [rbp-1864]; load GL_TEXTURE_DEPTH
		pop rcx
		mov [rbp-1864], rcx; store GL_TEXTURE_DEPTH
		mov rax, [rbp-1864]; load GL_TEXTURE_DEPTH
		mov rax, 32882
		push rax
		mov rax, [rbp-1872]; load GL_TEXTURE_WRAP_R
		pop rcx
		mov [rbp-1872], rcx; store GL_TEXTURE_WRAP_R
		mov rax, [rbp-1872]; load GL_TEXTURE_WRAP_R
		mov rax, 32883
		push rax
		mov rax, [rbp-1880]; load GL_MAX_3D_TEXTURE_SIZE
		pop rcx
		mov [rbp-1880], rcx; store GL_MAX_3D_TEXTURE_SIZE
		mov rax, [rbp-1880]; load GL_MAX_3D_TEXTURE_SIZE
		mov rax, 33634
		push rax
		mov rax, [rbp-1888]; load GL_UNSIGNED_BYTE_2_3_3_REV
		pop rcx
		mov [rbp-1888], rcx; store GL_UNSIGNED_BYTE_2_3_3_REV
		mov rax, [rbp-1888]; load GL_UNSIGNED_BYTE_2_3_3_REV
		mov rax, 33635
		push rax
		mov rax, [rbp-1896]; load GL_UNSIGNED_SHORT_5_6_5
		pop rcx
		mov [rbp-1896], rcx; store GL_UNSIGNED_SHORT_5_6_5
		mov rax, [rbp-1896]; load GL_UNSIGNED_SHORT_5_6_5
		mov rax, 33636
		push rax
		mov rax, [rbp-1904]; load GL_UNSIGNED_SHORT_5_6_5_REV
		pop rcx
		mov [rbp-1904], rcx; store GL_UNSIGNED_SHORT_5_6_5_REV
		mov rax, [rbp-1904]; load GL_UNSIGNED_SHORT_5_6_5_REV
		mov rax, 33637
		push rax
		mov rax, [rbp-1912]; load GL_UNSIGNED_SHORT_4_4_4_4_REV
		pop rcx
		mov [rbp-1912], rcx; store GL_UNSIGNED_SHORT_4_4_4_4_REV
		mov rax, [rbp-1912]; load GL_UNSIGNED_SHORT_4_4_4_4_REV
		mov rax, 33638
		push rax
		mov rax, [rbp-1920]; load GL_UNSIGNED_SHORT_1_5_5_5_REV
		pop rcx
		mov [rbp-1920], rcx; store GL_UNSIGNED_SHORT_1_5_5_5_REV
		mov rax, [rbp-1920]; load GL_UNSIGNED_SHORT_1_5_5_5_REV
		mov rax, 33639
		push rax
		mov rax, [rbp-1928]; load GL_UNSIGNED_INT_8_8_8_8_REV
		pop rcx
		mov [rbp-1928], rcx; store GL_UNSIGNED_INT_8_8_8_8_REV
		mov rax, [rbp-1928]; load GL_UNSIGNED_INT_8_8_8_8_REV
		mov rax, 33640
		push rax
		mov rax, [rbp-1936]; load GL_UNSIGNED_INT_2_10_10_10_REV
		pop rcx
		mov [rbp-1936], rcx; store GL_UNSIGNED_INT_2_10_10_10_REV
		mov rax, [rbp-1936]; load GL_UNSIGNED_INT_2_10_10_10_REV
		mov rax, 32992
		push rax
		mov rax, [rbp-1944]; load GL_BGR
		pop rcx
		mov [rbp-1944], rcx; store GL_BGR
		mov rax, [rbp-1944]; load GL_BGR
		mov rax, 32993
		push rax
		mov rax, [rbp-1952]; load GL_BGRA
		pop rcx
		mov [rbp-1952], rcx; store GL_BGRA
		mov rax, [rbp-1952]; load GL_BGRA
		mov rax, 33000
		push rax
		mov rax, [rbp-1960]; load GL_MAX_ELEMENTS_VERTICES
		pop rcx
		mov [rbp-1960], rcx; store GL_MAX_ELEMENTS_VERTICES
		mov rax, [rbp-1960]; load GL_MAX_ELEMENTS_VERTICES
		mov rax, 33001
		push rax
		mov rax, [rbp-1968]; load GL_MAX_ELEMENTS_INDICES
		pop rcx
		mov [rbp-1968], rcx; store GL_MAX_ELEMENTS_INDICES
		mov rax, [rbp-1968]; load GL_MAX_ELEMENTS_INDICES
		mov rax, 33071
		push rax
		mov rax, [rbp-1976]; load GL_CLAMP_TO_EDGE
		pop rcx
		mov [rbp-1976], rcx; store GL_CLAMP_TO_EDGE
		mov rax, [rbp-1976]; load GL_CLAMP_TO_EDGE
		mov rax, 33082
		push rax
		mov rax, [rbp-1984]; load GL_TEXTURE_MIN_LOD
		pop rcx
		mov [rbp-1984], rcx; store GL_TEXTURE_MIN_LOD
		mov rax, [rbp-1984]; load GL_TEXTURE_MIN_LOD
		mov rax, 33083
		push rax
		mov rax, [rbp-1992]; load GL_TEXTURE_MAX_LOD
		pop rcx
		mov [rbp-1992], rcx; store GL_TEXTURE_MAX_LOD
		mov rax, [rbp-1992]; load GL_TEXTURE_MAX_LOD
		mov rax, 33084
		push rax
		mov rax, [rbp-2000]; load GL_TEXTURE_BASE_LEVEL
		pop rcx
		mov [rbp-2000], rcx; store GL_TEXTURE_BASE_LEVEL
		mov rax, [rbp-2000]; load GL_TEXTURE_BASE_LEVEL
		mov rax, 33085
		push rax
		mov rax, [rbp-2008]; load GL_TEXTURE_MAX_LEVEL
		pop rcx
		mov [rbp-2008], rcx; store GL_TEXTURE_MAX_LEVEL
		mov rax, [rbp-2008]; load GL_TEXTURE_MAX_LEVEL
		mov rax, 2834
		push rax
		mov rax, [rbp-2016]; load GL_SMOOTH_POINT_SIZE_RANGE
		pop rcx
		mov [rbp-2016], rcx; store GL_SMOOTH_POINT_SIZE_RANGE
		mov rax, [rbp-2016]; load GL_SMOOTH_POINT_SIZE_RANGE
		mov rax, 2835
		push rax
		mov rax, [rbp-2024]; load GL_SMOOTH_POINT_SIZE_GRANULARITY
		pop rcx
		mov [rbp-2024], rcx; store GL_SMOOTH_POINT_SIZE_GRANULARITY
		mov rax, [rbp-2024]; load GL_SMOOTH_POINT_SIZE_GRANULARITY
		mov rax, 2850
		push rax
		mov rax, [rbp-2032]; load GL_SMOOTH_LINE_WIDTH_RANGE
		pop rcx
		mov [rbp-2032], rcx; store GL_SMOOTH_LINE_WIDTH_RANGE
		mov rax, [rbp-2032]; load GL_SMOOTH_LINE_WIDTH_RANGE
		mov rax, 2851
		push rax
		mov rax, [rbp-2040]; load GL_SMOOTH_LINE_WIDTH_GRANULARITY
		pop rcx
		mov [rbp-2040], rcx; store GL_SMOOTH_LINE_WIDTH_GRANULARITY
		mov rax, [rbp-2040]; load GL_SMOOTH_LINE_WIDTH_GRANULARITY
		mov rax, 33902
		push rax
		mov rax, [rbp-2048]; load GL_ALIASED_LINE_WIDTH_RANGE
		pop rcx
		mov [rbp-2048], rcx; store GL_ALIASED_LINE_WIDTH_RANGE
		mov rax, [rbp-2048]; load GL_ALIASED_LINE_WIDTH_RANGE
		mov rax, 33984
		push rax
		mov rax, [rbp-2056]; load GL_TEXTURE0
		pop rcx
		mov [rbp-2056], rcx; store GL_TEXTURE0
		mov rax, [rbp-2056]; load GL_TEXTURE0
		mov rax, 33985
		push rax
		mov rax, [rbp-2064]; load GL_TEXTURE1
		pop rcx
		mov [rbp-2064], rcx; store GL_TEXTURE1
		mov rax, [rbp-2064]; load GL_TEXTURE1
		mov rax, 33986
		push rax
		mov rax, [rbp-2072]; load GL_TEXTURE2
		pop rcx
		mov [rbp-2072], rcx; store GL_TEXTURE2
		mov rax, [rbp-2072]; load GL_TEXTURE2
		mov rax, 33987
		push rax
		mov rax, [rbp-2080]; load GL_TEXTURE3
		pop rcx
		mov [rbp-2080], rcx; store GL_TEXTURE3
		mov rax, [rbp-2080]; load GL_TEXTURE3
		mov rax, 33988
		push rax
		mov rax, [rbp-2088]; load GL_TEXTURE4
		pop rcx
		mov [rbp-2088], rcx; store GL_TEXTURE4
		mov rax, [rbp-2088]; load GL_TEXTURE4
		mov rax, 33989
		push rax
		mov rax, [rbp-2096]; load GL_TEXTURE5
		pop rcx
		mov [rbp-2096], rcx; store GL_TEXTURE5
		mov rax, [rbp-2096]; load GL_TEXTURE5
		mov rax, 33990
		push rax
		mov rax, [rbp-2104]; load GL_TEXTURE6
		pop rcx
		mov [rbp-2104], rcx; store GL_TEXTURE6
		mov rax, [rbp-2104]; load GL_TEXTURE6
		mov rax, 33991
		push rax
		mov rax, [rbp-2112]; load GL_TEXTURE7
		pop rcx
		mov [rbp-2112], rcx; store GL_TEXTURE7
		mov rax, [rbp-2112]; load GL_TEXTURE7
		mov rax, 33992
		push rax
		mov rax, [rbp-2120]; load GL_TEXTURE8
		pop rcx
		mov [rbp-2120], rcx; store GL_TEXTURE8
		mov rax, [rbp-2120]; load GL_TEXTURE8
		mov rax, 33993
		push rax
		mov rax, [rbp-2128]; load GL_TEXTURE9
		pop rcx
		mov [rbp-2128], rcx; store GL_TEXTURE9
		mov rax, [rbp-2128]; load GL_TEXTURE9
		mov rax, 33994
		push rax
		mov rax, [rbp-2136]; load GL_TEXTURE10
		pop rcx
		mov [rbp-2136], rcx; store GL_TEXTURE10
		mov rax, [rbp-2136]; load GL_TEXTURE10
		mov rax, 33995
		push rax
		mov rax, [rbp-2144]; load GL_TEXTURE11
		pop rcx
		mov [rbp-2144], rcx; store GL_TEXTURE11
		mov rax, [rbp-2144]; load GL_TEXTURE11
		mov rax, 33996
		push rax
		mov rax, [rbp-2152]; load GL_TEXTURE12
		pop rcx
		mov [rbp-2152], rcx; store GL_TEXTURE12
		mov rax, [rbp-2152]; load GL_TEXTURE12
		mov rax, 33997
		push rax
		mov rax, [rbp-2160]; load GL_TEXTURE13
		pop rcx
		mov [rbp-2160], rcx; store GL_TEXTURE13
		mov rax, [rbp-2160]; load GL_TEXTURE13
		mov rax, 33998
		push rax
		mov rax, [rbp-2168]; load GL_TEXTURE14
		pop rcx
		mov [rbp-2168], rcx; store GL_TEXTURE14
		mov rax, [rbp-2168]; load GL_TEXTURE14
		mov rax, 33999
		push rax
		mov rax, [rbp-2176]; load GL_TEXTURE15
		pop rcx
		mov [rbp-2176], rcx; store GL_TEXTURE15
		mov rax, [rbp-2176]; load GL_TEXTURE15
		mov rax, 34000
		push rax
		mov rax, [rbp-2184]; load GL_TEXTURE16
		pop rcx
		mov [rbp-2184], rcx; store GL_TEXTURE16
		mov rax, [rbp-2184]; load GL_TEXTURE16
		mov rax, 34001
		push rax
		mov rax, [rbp-2192]; load GL_TEXTURE17
		pop rcx
		mov [rbp-2192], rcx; store GL_TEXTURE17
		mov rax, [rbp-2192]; load GL_TEXTURE17
		mov rax, 34002
		push rax
		mov rax, [rbp-2200]; load GL_TEXTURE18
		pop rcx
		mov [rbp-2200], rcx; store GL_TEXTURE18
		mov rax, [rbp-2200]; load GL_TEXTURE18
		mov rax, 34003
		push rax
		mov rax, [rbp-2208]; load GL_TEXTURE19
		pop rcx
		mov [rbp-2208], rcx; store GL_TEXTURE19
		mov rax, [rbp-2208]; load GL_TEXTURE19
		mov rax, 34004
		push rax
		mov rax, [rbp-2216]; load GL_TEXTURE20
		pop rcx
		mov [rbp-2216], rcx; store GL_TEXTURE20
		mov rax, [rbp-2216]; load GL_TEXTURE20
		mov rax, 34005
		push rax
		mov rax, [rbp-2224]; load GL_TEXTURE21
		pop rcx
		mov [rbp-2224], rcx; store GL_TEXTURE21
		mov rax, [rbp-2224]; load GL_TEXTURE21
		mov rax, 34006
		push rax
		mov rax, [rbp-2232]; load GL_TEXTURE22
		pop rcx
		mov [rbp-2232], rcx; store GL_TEXTURE22
		mov rax, [rbp-2232]; load GL_TEXTURE22
		mov rax, 34007
		push rax
		mov rax, [rbp-2240]; load GL_TEXTURE23
		pop rcx
		mov [rbp-2240], rcx; store GL_TEXTURE23
		mov rax, [rbp-2240]; load GL_TEXTURE23
		mov rax, 34008
		push rax
		mov rax, [rbp-2248]; load GL_TEXTURE24
		pop rcx
		mov [rbp-2248], rcx; store GL_TEXTURE24
		mov rax, [rbp-2248]; load GL_TEXTURE24
		mov rax, 34009
		push rax
		mov rax, [rbp-2256]; load GL_TEXTURE25
		pop rcx
		mov [rbp-2256], rcx; store GL_TEXTURE25
		mov rax, [rbp-2256]; load GL_TEXTURE25
		mov rax, 34010
		push rax
		mov rax, [rbp-2264]; load GL_TEXTURE26
		pop rcx
		mov [rbp-2264], rcx; store GL_TEXTURE26
		mov rax, [rbp-2264]; load GL_TEXTURE26
		mov rax, 34011
		push rax
		mov rax, [rbp-2272]; load GL_TEXTURE27
		pop rcx
		mov [rbp-2272], rcx; store GL_TEXTURE27
		mov rax, [rbp-2272]; load GL_TEXTURE27
		mov rax, 34012
		push rax
		mov rax, [rbp-2280]; load GL_TEXTURE28
		pop rcx
		mov [rbp-2280], rcx; store GL_TEXTURE28
		mov rax, [rbp-2280]; load GL_TEXTURE28
		mov rax, 34013
		push rax
		mov rax, [rbp-2288]; load GL_TEXTURE29
		pop rcx
		mov [rbp-2288], rcx; store GL_TEXTURE29
		mov rax, [rbp-2288]; load GL_TEXTURE29
		mov rax, 34014
		push rax
		mov rax, [rbp-2296]; load GL_TEXTURE30
		pop rcx
		mov [rbp-2296], rcx; store GL_TEXTURE30
		mov rax, [rbp-2296]; load GL_TEXTURE30
		mov rax, 34015
		push rax
		mov rax, [rbp-2304]; load GL_TEXTURE31
		pop rcx
		mov [rbp-2304], rcx; store GL_TEXTURE31
		mov rax, [rbp-2304]; load GL_TEXTURE31
		mov rax, 34016
		push rax
		mov rax, [rbp-2312]; load GL_ACTIVE_TEXTURE
		pop rcx
		mov [rbp-2312], rcx; store GL_ACTIVE_TEXTURE
		mov rax, [rbp-2312]; load GL_ACTIVE_TEXTURE
		mov rax, 32925
		push rax
		mov rax, [rbp-2320]; load GL_MULTISAMPLE
		pop rcx
		mov [rbp-2320], rcx; store GL_MULTISAMPLE
		mov rax, [rbp-2320]; load GL_MULTISAMPLE
		mov rax, 32926
		push rax
		mov rax, [rbp-2328]; load GL_SAMPLE_ALPHA_TO_COVERAGE
		pop rcx
		mov [rbp-2328], rcx; store GL_SAMPLE_ALPHA_TO_COVERAGE
		mov rax, [rbp-2328]; load GL_SAMPLE_ALPHA_TO_COVERAGE
		mov rax, 32927
		push rax
		mov rax, [rbp-2336]; load GL_SAMPLE_ALPHA_TO_ONE
		pop rcx
		mov [rbp-2336], rcx; store GL_SAMPLE_ALPHA_TO_ONE
		mov rax, [rbp-2336]; load GL_SAMPLE_ALPHA_TO_ONE
		mov rax, 32928
		push rax
		mov rax, [rbp-2344]; load GL_SAMPLE_COVERAGE
		pop rcx
		mov [rbp-2344], rcx; store GL_SAMPLE_COVERAGE
		mov rax, [rbp-2344]; load GL_SAMPLE_COVERAGE
		mov rax, 32936
		push rax
		mov rax, [rbp-2352]; load GL_SAMPLE_BUFFERS
		pop rcx
		mov [rbp-2352], rcx; store GL_SAMPLE_BUFFERS
		mov rax, [rbp-2352]; load GL_SAMPLE_BUFFERS
		mov rax, 32937
		push rax
		mov rax, [rbp-2360]; load GL_SAMPLES
		pop rcx
		mov [rbp-2360], rcx; store GL_SAMPLES
		mov rax, [rbp-2360]; load GL_SAMPLES
		mov rax, 32938
		push rax
		mov rax, [rbp-2368]; load GL_SAMPLE_COVERAGE_VALUE
		pop rcx
		mov [rbp-2368], rcx; store GL_SAMPLE_COVERAGE_VALUE
		mov rax, [rbp-2368]; load GL_SAMPLE_COVERAGE_VALUE
		mov rax, 32939
		push rax
		mov rax, [rbp-2376]; load GL_SAMPLE_COVERAGE_INVERT
		pop rcx
		mov [rbp-2376], rcx; store GL_SAMPLE_COVERAGE_INVERT
		mov rax, [rbp-2376]; load GL_SAMPLE_COVERAGE_INVERT
		mov rax, 34067
		push rax
		mov rax, [rbp-2384]; load GL_TEXTURE_CUBE_MAP
		pop rcx
		mov [rbp-2384], rcx; store GL_TEXTURE_CUBE_MAP
		mov rax, [rbp-2384]; load GL_TEXTURE_CUBE_MAP
		mov rax, 34068
		push rax
		mov rax, [rbp-2392]; load GL_TEXTURE_BINDING_CUBE_MAP
		pop rcx
		mov [rbp-2392], rcx; store GL_TEXTURE_BINDING_CUBE_MAP
		mov rax, [rbp-2392]; load GL_TEXTURE_BINDING_CUBE_MAP
		mov rax, 34069
		push rax
		mov rax, [rbp-2400]; load GL_TEXTURE_CUBE_MAP_POSITIVE_X
		pop rcx
		mov [rbp-2400], rcx; store GL_TEXTURE_CUBE_MAP_POSITIVE_X
		mov rax, [rbp-2400]; load GL_TEXTURE_CUBE_MAP_POSITIVE_X
		mov rax, 34070
		push rax
		mov rax, [rbp-2408]; load GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		pop rcx
		mov [rbp-2408], rcx; store GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		mov rax, [rbp-2408]; load GL_TEXTURE_CUBE_MAP_NEGATIVE_X
		mov rax, 34071
		push rax
		mov rax, [rbp-2416]; load GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		pop rcx
		mov [rbp-2416], rcx; store GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		mov rax, [rbp-2416]; load GL_TEXTURE_CUBE_MAP_POSITIVE_Y
		mov rax, 34072
		push rax
		mov rax, [rbp-2424]; load GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		pop rcx
		mov [rbp-2424], rcx; store GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		mov rax, [rbp-2424]; load GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
		mov rax, 34073
		push rax
		mov rax, [rbp-2432]; load GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		pop rcx
		mov [rbp-2432], rcx; store GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		mov rax, [rbp-2432]; load GL_TEXTURE_CUBE_MAP_POSITIVE_Z
		mov rax, 34074
		push rax
		mov rax, [rbp-2440]; load GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		pop rcx
		mov [rbp-2440], rcx; store GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		mov rax, [rbp-2440]; load GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		mov rax, 34075
		push rax
		mov rax, [rbp-2448]; load GL_PROXY_TEXTURE_CUBE_MAP
		pop rcx
		mov [rbp-2448], rcx; store GL_PROXY_TEXTURE_CUBE_MAP
		mov rax, [rbp-2448]; load GL_PROXY_TEXTURE_CUBE_MAP
		mov rax, 34076
		push rax
		mov rax, [rbp-2456]; load GL_MAX_CUBE_MAP_TEXTURE_SIZE
		pop rcx
		mov [rbp-2456], rcx; store GL_MAX_CUBE_MAP_TEXTURE_SIZE
		mov rax, [rbp-2456]; load GL_MAX_CUBE_MAP_TEXTURE_SIZE
		mov rax, 34029
		push rax
		mov rax, [rbp-2464]; load GL_COMPRESSED_RGB
		pop rcx
		mov [rbp-2464], rcx; store GL_COMPRESSED_RGB
		mov rax, [rbp-2464]; load GL_COMPRESSED_RGB
		mov rax, 34030
		push rax
		mov rax, [rbp-2472]; load GL_COMPRESSED_RGBA
		pop rcx
		mov [rbp-2472], rcx; store GL_COMPRESSED_RGBA
		mov rax, [rbp-2472]; load GL_COMPRESSED_RGBA
		mov rax, 34031
		push rax
		mov rax, [rbp-2480]; load GL_TEXTURE_COMPRESSION_HINT
		pop rcx
		mov [rbp-2480], rcx; store GL_TEXTURE_COMPRESSION_HINT
		mov rax, [rbp-2480]; load GL_TEXTURE_COMPRESSION_HINT
		mov rax, 34464
		push rax
		mov rax, [rbp-2488]; load GL_TEXTURE_COMPRESSED_IMAGE_SIZE
		pop rcx
		mov [rbp-2488], rcx; store GL_TEXTURE_COMPRESSED_IMAGE_SIZE
		mov rax, [rbp-2488]; load GL_TEXTURE_COMPRESSED_IMAGE_SIZE
		mov rax, 34465
		push rax
		mov rax, [rbp-2496]; load GL_TEXTURE_COMPRESSED
		pop rcx
		mov [rbp-2496], rcx; store GL_TEXTURE_COMPRESSED
		mov rax, [rbp-2496]; load GL_TEXTURE_COMPRESSED
		mov rax, 34466
		push rax
		mov rax, [rbp-2504]; load GL_NUM_COMPRESSED_TEXTURE_FORMATS
		pop rcx
		mov [rbp-2504], rcx; store GL_NUM_COMPRESSED_TEXTURE_FORMATS
		mov rax, [rbp-2504]; load GL_NUM_COMPRESSED_TEXTURE_FORMATS
		mov rax, 34467
		push rax
		mov rax, [rbp-2512]; load GL_COMPRESSED_TEXTURE_FORMATS
		pop rcx
		mov [rbp-2512], rcx; store GL_COMPRESSED_TEXTURE_FORMATS
		mov rax, [rbp-2512]; load GL_COMPRESSED_TEXTURE_FORMATS
		mov rax, 33069
		push rax
		mov rax, [rbp-2520]; load GL_CLAMP_TO_BORDER
		pop rcx
		mov [rbp-2520], rcx; store GL_CLAMP_TO_BORDER
		mov rax, [rbp-2520]; load GL_CLAMP_TO_BORDER
		mov rax, 32968
		push rax
		mov rax, [rbp-2528]; load GL_BLEND_DST_RGB
		pop rcx
		mov [rbp-2528], rcx; store GL_BLEND_DST_RGB
		mov rax, [rbp-2528]; load GL_BLEND_DST_RGB
		mov rax, 32969
		push rax
		mov rax, [rbp-2536]; load GL_BLEND_SRC_RGB
		pop rcx
		mov [rbp-2536], rcx; store GL_BLEND_SRC_RGB
		mov rax, [rbp-2536]; load GL_BLEND_SRC_RGB
		mov rax, 32970
		push rax
		mov rax, [rbp-2544]; load GL_BLEND_DST_ALPHA
		pop rcx
		mov [rbp-2544], rcx; store GL_BLEND_DST_ALPHA
		mov rax, [rbp-2544]; load GL_BLEND_DST_ALPHA
		mov rax, 32971
		push rax
		mov rax, [rbp-2552]; load GL_BLEND_SRC_ALPHA
		pop rcx
		mov [rbp-2552], rcx; store GL_BLEND_SRC_ALPHA
		mov rax, [rbp-2552]; load GL_BLEND_SRC_ALPHA
		mov rax, 33064
		push rax
		mov rax, [rbp-2560]; load GL_POINT_FADE_THRESHOLD_SIZE
		pop rcx
		mov [rbp-2560], rcx; store GL_POINT_FADE_THRESHOLD_SIZE
		mov rax, [rbp-2560]; load GL_POINT_FADE_THRESHOLD_SIZE
		mov rax, 33189
		push rax
		mov rax, [rbp-2568]; load GL_DEPTH_COMPONENT16
		pop rcx
		mov [rbp-2568], rcx; store GL_DEPTH_COMPONENT16
		mov rax, [rbp-2568]; load GL_DEPTH_COMPONENT16
		mov rax, 33190
		push rax
		mov rax, [rbp-2576]; load GL_DEPTH_COMPONENT24
		pop rcx
		mov [rbp-2576], rcx; store GL_DEPTH_COMPONENT24
		mov rax, [rbp-2576]; load GL_DEPTH_COMPONENT24
		mov rax, 33191
		push rax
		mov rax, [rbp-2584]; load GL_DEPTH_COMPONENT32
		pop rcx
		mov [rbp-2584], rcx; store GL_DEPTH_COMPONENT32
		mov rax, [rbp-2584]; load GL_DEPTH_COMPONENT32
		mov rax, 33648
		push rax
		mov rax, [rbp-2592]; load GL_MIRRORED_REPEAT
		pop rcx
		mov [rbp-2592], rcx; store GL_MIRRORED_REPEAT
		mov rax, [rbp-2592]; load GL_MIRRORED_REPEAT
		mov rax, 34045
		push rax
		mov rax, [rbp-2600]; load GL_MAX_TEXTURE_LOD_BIAS
		pop rcx
		mov [rbp-2600], rcx; store GL_MAX_TEXTURE_LOD_BIAS
		mov rax, [rbp-2600]; load GL_MAX_TEXTURE_LOD_BIAS
		mov rax, 34049
		push rax
		mov rax, [rbp-2608]; load GL_TEXTURE_LOD_BIAS
		pop rcx
		mov [rbp-2608], rcx; store GL_TEXTURE_LOD_BIAS
		mov rax, [rbp-2608]; load GL_TEXTURE_LOD_BIAS
		mov rax, 34055
		push rax
		mov rax, [rbp-2616]; load GL_INCR_WRAP
		pop rcx
		mov [rbp-2616], rcx; store GL_INCR_WRAP
		mov rax, [rbp-2616]; load GL_INCR_WRAP
		mov rax, 34056
		push rax
		mov rax, [rbp-2624]; load GL_DECR_WRAP
		pop rcx
		mov [rbp-2624], rcx; store GL_DECR_WRAP
		mov rax, [rbp-2624]; load GL_DECR_WRAP
		mov rax, 34890
		push rax
		mov rax, [rbp-2632]; load GL_TEXTURE_DEPTH_SIZE
		pop rcx
		mov [rbp-2632], rcx; store GL_TEXTURE_DEPTH_SIZE
		mov rax, [rbp-2632]; load GL_TEXTURE_DEPTH_SIZE
		mov rax, 34892
		push rax
		mov rax, [rbp-2640]; load GL_TEXTURE_COMPARE_MODE
		pop rcx
		mov [rbp-2640], rcx; store GL_TEXTURE_COMPARE_MODE
		mov rax, [rbp-2640]; load GL_TEXTURE_COMPARE_MODE
		mov rax, 34893
		push rax
		mov rax, [rbp-2648]; load GL_TEXTURE_COMPARE_FUNC
		pop rcx
		mov [rbp-2648], rcx; store GL_TEXTURE_COMPARE_FUNC
		mov rax, [rbp-2648]; load GL_TEXTURE_COMPARE_FUNC
		mov rax, 32774
		push rax
		mov rax, [rbp-2656]; load GL_FUNC_ADD
		pop rcx
		mov [rbp-2656], rcx; store GL_FUNC_ADD
		mov rax, [rbp-2656]; load GL_FUNC_ADD
		mov rax, 32778
		push rax
		mov rax, [rbp-2664]; load GL_FUNC_SUBTRACT
		pop rcx
		mov [rbp-2664], rcx; store GL_FUNC_SUBTRACT
		mov rax, [rbp-2664]; load GL_FUNC_SUBTRACT
		mov rax, 32779
		push rax
		mov rax, [rbp-2672]; load GL_FUNC_REVERSE_SUBTRACT
		pop rcx
		mov [rbp-2672], rcx; store GL_FUNC_REVERSE_SUBTRACT
		mov rax, [rbp-2672]; load GL_FUNC_REVERSE_SUBTRACT
		mov rax, 32775
		push rax
		mov rax, [rbp-2680]; load GL_MIN
		pop rcx
		mov [rbp-2680], rcx; store GL_MIN
		mov rax, [rbp-2680]; load GL_MIN
		mov rax, 32776
		push rax
		mov rax, [rbp-2688]; load GL_MAX
		pop rcx
		mov [rbp-2688], rcx; store GL_MAX
		mov rax, [rbp-2688]; load GL_MAX
		mov rax, 32769
		push rax
		mov rax, [rbp-2696]; load GL_CONSTANT_COLOR
		pop rcx
		mov [rbp-2696], rcx; store GL_CONSTANT_COLOR
		mov rax, [rbp-2696]; load GL_CONSTANT_COLOR
		mov rax, 32770
		push rax
		mov rax, [rbp-2704]; load GL_ONE_MINUS_CONSTANT_COLOR
		pop rcx
		mov [rbp-2704], rcx; store GL_ONE_MINUS_CONSTANT_COLOR
		mov rax, [rbp-2704]; load GL_ONE_MINUS_CONSTANT_COLOR
		mov rax, 32771
		push rax
		mov rax, [rbp-2712]; load GL_CONSTANT_ALPHA
		pop rcx
		mov [rbp-2712], rcx; store GL_CONSTANT_ALPHA
		mov rax, [rbp-2712]; load GL_CONSTANT_ALPHA
		mov rax, 32772
		push rax
		mov rax, [rbp-2720]; load GL_ONE_MINUS_CONSTANT_ALPHA
		pop rcx
		mov [rbp-2720], rcx; store GL_ONE_MINUS_CONSTANT_ALPHA
		mov rax, [rbp-2720]; load GL_ONE_MINUS_CONSTANT_ALPHA
		mov rax, 34660
		push rax
		mov rax, [rbp-2728]; load GL_BUFFER_SIZE
		pop rcx
		mov [rbp-2728], rcx; store GL_BUFFER_SIZE
		mov rax, [rbp-2728]; load GL_BUFFER_SIZE
		mov rax, 34661
		push rax
		mov rax, [rbp-2736]; load GL_BUFFER_USAGE
		pop rcx
		mov [rbp-2736], rcx; store GL_BUFFER_USAGE
		mov rax, [rbp-2736]; load GL_BUFFER_USAGE
		mov rax, 34916
		push rax
		mov rax, [rbp-2744]; load GL_QUERY_COUNTER_BITS
		pop rcx
		mov [rbp-2744], rcx; store GL_QUERY_COUNTER_BITS
		mov rax, [rbp-2744]; load GL_QUERY_COUNTER_BITS
		mov rax, 34917
		push rax
		mov rax, [rbp-2752]; load GL_CURRENT_QUERY
		pop rcx
		mov [rbp-2752], rcx; store GL_CURRENT_QUERY
		mov rax, [rbp-2752]; load GL_CURRENT_QUERY
		mov rax, 34918
		push rax
		mov rax, [rbp-2760]; load GL_QUERY_RESULT
		pop rcx
		mov [rbp-2760], rcx; store GL_QUERY_RESULT
		mov rax, [rbp-2760]; load GL_QUERY_RESULT
		mov rax, 34919
		push rax
		mov rax, [rbp-2768]; load GL_QUERY_RESULT_AVAILABLE
		pop rcx
		mov [rbp-2768], rcx; store GL_QUERY_RESULT_AVAILABLE
		mov rax, [rbp-2768]; load GL_QUERY_RESULT_AVAILABLE
		mov rax, 34962
		push rax
		mov rax, [rbp-2776]; load GL_ARRAY_BUFFER
		pop rcx
		mov [rbp-2776], rcx; store GL_ARRAY_BUFFER
		mov rax, [rbp-2776]; load GL_ARRAY_BUFFER
		mov rax, 34963
		push rax
		mov rax, [rbp-2784]; load GL_ELEMENT_ARRAY_BUFFER
		pop rcx
		mov [rbp-2784], rcx; store GL_ELEMENT_ARRAY_BUFFER
		mov rax, [rbp-2784]; load GL_ELEMENT_ARRAY_BUFFER
		mov rax, 34964
		push rax
		mov rax, [rbp-2792]; load GL_ARRAY_BUFFER_BINDING
		pop rcx
		mov [rbp-2792], rcx; store GL_ARRAY_BUFFER_BINDING
		mov rax, [rbp-2792]; load GL_ARRAY_BUFFER_BINDING
		mov rax, 34965
		push rax
		mov rax, [rbp-2800]; load GL_ELEMENT_ARRAY_BUFFER_BINDING
		pop rcx
		mov [rbp-2800], rcx; store GL_ELEMENT_ARRAY_BUFFER_BINDING
		mov rax, [rbp-2800]; load GL_ELEMENT_ARRAY_BUFFER_BINDING
		mov rax, 34975
		push rax
		mov rax, [rbp-2808]; load GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING
		pop rcx
		mov [rbp-2808], rcx; store GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING
		mov rax, [rbp-2808]; load GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING
		mov rax, 35000
		push rax
		mov rax, [rbp-2816]; load GL_READ_ONLY
		pop rcx
		mov [rbp-2816], rcx; store GL_READ_ONLY
		mov rax, [rbp-2816]; load GL_READ_ONLY
		mov rax, 35001
		push rax
		mov rax, [rbp-2824]; load GL_WRITE_ONLY
		pop rcx
		mov [rbp-2824], rcx; store GL_WRITE_ONLY
		mov rax, [rbp-2824]; load GL_WRITE_ONLY
		mov rax, 35002
		push rax
		mov rax, [rbp-2832]; load GL_READ_WRITE
		pop rcx
		mov [rbp-2832], rcx; store GL_READ_WRITE
		mov rax, [rbp-2832]; load GL_READ_WRITE
		mov rax, 35003
		push rax
		mov rax, [rbp-2840]; load GL_BUFFER_ACCESS
		pop rcx
		mov [rbp-2840], rcx; store GL_BUFFER_ACCESS
		mov rax, [rbp-2840]; load GL_BUFFER_ACCESS
		mov rax, 35004
		push rax
		mov rax, [rbp-2848]; load GL_BUFFER_MAPPED
		pop rcx
		mov [rbp-2848], rcx; store GL_BUFFER_MAPPED
		mov rax, [rbp-2848]; load GL_BUFFER_MAPPED
		mov rax, 35005
		push rax
		mov rax, [rbp-2856]; load GL_BUFFER_MAP_POINTER
		pop rcx
		mov [rbp-2856], rcx; store GL_BUFFER_MAP_POINTER
		mov rax, [rbp-2856]; load GL_BUFFER_MAP_POINTER
		mov rax, 35040
		push rax
		mov rax, [rbp-2864]; load GL_STREAM_DRAW
		pop rcx
		mov [rbp-2864], rcx; store GL_STREAM_DRAW
		mov rax, [rbp-2864]; load GL_STREAM_DRAW
		mov rax, 35041
		push rax
		mov rax, [rbp-2872]; load GL_STREAM_READ
		pop rcx
		mov [rbp-2872], rcx; store GL_STREAM_READ
		mov rax, [rbp-2872]; load GL_STREAM_READ
		mov rax, 35042
		push rax
		mov rax, [rbp-2880]; load GL_STREAM_COPY
		pop rcx
		mov [rbp-2880], rcx; store GL_STREAM_COPY
		mov rax, [rbp-2880]; load GL_STREAM_COPY
		mov rax, 35044
		push rax
		mov rax, [rbp-2888]; load GL_STATIC_DRAW
		pop rcx
		mov [rbp-2888], rcx; store GL_STATIC_DRAW
		mov rax, [rbp-2888]; load GL_STATIC_DRAW
		mov rax, 35045
		push rax
		mov rax, [rbp-2896]; load GL_STATIC_READ
		pop rcx
		mov [rbp-2896], rcx; store GL_STATIC_READ
		mov rax, [rbp-2896]; load GL_STATIC_READ
		mov rax, 35046
		push rax
		mov rax, [rbp-2904]; load GL_STATIC_COPY
		pop rcx
		mov [rbp-2904], rcx; store GL_STATIC_COPY
		mov rax, [rbp-2904]; load GL_STATIC_COPY
		mov rax, 35048
		push rax
		mov rax, [rbp-2912]; load GL_DYNAMIC_DRAW
		pop rcx
		mov [rbp-2912], rcx; store GL_DYNAMIC_DRAW
		mov rax, [rbp-2912]; load GL_DYNAMIC_DRAW
		mov rax, 35049
		push rax
		mov rax, [rbp-2920]; load GL_DYNAMIC_READ
		pop rcx
		mov [rbp-2920], rcx; store GL_DYNAMIC_READ
		mov rax, [rbp-2920]; load GL_DYNAMIC_READ
		mov rax, 35050
		push rax
		mov rax, [rbp-2928]; load GL_DYNAMIC_COPY
		pop rcx
		mov [rbp-2928], rcx; store GL_DYNAMIC_COPY
		mov rax, [rbp-2928]; load GL_DYNAMIC_COPY
		mov rax, 35092
		push rax
		mov rax, [rbp-2936]; load GL_SAMPLES_PASSED
		pop rcx
		mov [rbp-2936], rcx; store GL_SAMPLES_PASSED
		mov rax, [rbp-2936]; load GL_SAMPLES_PASSED
		mov rax, 34185
		push rax
		mov rax, [rbp-2944]; load GL_SRC1_ALPHA
		pop rcx
		mov [rbp-2944], rcx; store GL_SRC1_ALPHA
		mov rax, [rbp-2944]; load GL_SRC1_ALPHA
		mov rax, 32777
		push rax
		mov rax, [rbp-2952]; load GL_BLEND_EQUATION_RGB
		pop rcx
		mov [rbp-2952], rcx; store GL_BLEND_EQUATION_RGB
		mov rax, [rbp-2952]; load GL_BLEND_EQUATION_RGB
		mov rax, 34338
		push rax
		mov rax, [rbp-2960]; load GL_VERTEX_ATTRIB_ARRAY_ENABLED
		pop rcx
		mov [rbp-2960], rcx; store GL_VERTEX_ATTRIB_ARRAY_ENABLED
		mov rax, [rbp-2960]; load GL_VERTEX_ATTRIB_ARRAY_ENABLED
		mov rax, 34339
		push rax
		mov rax, [rbp-2968]; load GL_VERTEX_ATTRIB_ARRAY_SIZE
		pop rcx
		mov [rbp-2968], rcx; store GL_VERTEX_ATTRIB_ARRAY_SIZE
		mov rax, [rbp-2968]; load GL_VERTEX_ATTRIB_ARRAY_SIZE
		mov rax, 34340
		push rax
		mov rax, [rbp-2976]; load GL_VERTEX_ATTRIB_ARRAY_STRIDE
		pop rcx
		mov [rbp-2976], rcx; store GL_VERTEX_ATTRIB_ARRAY_STRIDE
		mov rax, [rbp-2976]; load GL_VERTEX_ATTRIB_ARRAY_STRIDE
		mov rax, 34341
		push rax
		mov rax, [rbp-2984]; load GL_VERTEX_ATTRIB_ARRAY_TYPE
		pop rcx
		mov [rbp-2984], rcx; store GL_VERTEX_ATTRIB_ARRAY_TYPE
		mov rax, [rbp-2984]; load GL_VERTEX_ATTRIB_ARRAY_TYPE
		mov rax, 34342
		push rax
		mov rax, [rbp-2992]; load GL_CURRENT_VERTEX_ATTRIB
		pop rcx
		mov [rbp-2992], rcx; store GL_CURRENT_VERTEX_ATTRIB
		mov rax, [rbp-2992]; load GL_CURRENT_VERTEX_ATTRIB
		mov rax, 34370
		push rax
		mov rax, [rbp-3000]; load GL_VERTEX_PROGRAM_POINT_SIZE
		pop rcx
		mov [rbp-3000], rcx; store GL_VERTEX_PROGRAM_POINT_SIZE
		mov rax, [rbp-3000]; load GL_VERTEX_PROGRAM_POINT_SIZE
		mov rax, 34373
		push rax
		mov rax, [rbp-3008]; load GL_VERTEX_ATTRIB_ARRAY_POINTER
		pop rcx
		mov [rbp-3008], rcx; store GL_VERTEX_ATTRIB_ARRAY_POINTER
		mov rax, [rbp-3008]; load GL_VERTEX_ATTRIB_ARRAY_POINTER
		mov rax, 34816
		push rax
		mov rax, [rbp-3016]; load GL_STENCIL_BACK_FUNC
		pop rcx
		mov [rbp-3016], rcx; store GL_STENCIL_BACK_FUNC
		mov rax, [rbp-3016]; load GL_STENCIL_BACK_FUNC
		mov rax, 34817
		push rax
		mov rax, [rbp-3024]; load GL_STENCIL_BACK_FAIL
		pop rcx
		mov [rbp-3024], rcx; store GL_STENCIL_BACK_FAIL
		mov rax, [rbp-3024]; load GL_STENCIL_BACK_FAIL
		mov rax, 34818
		push rax
		mov rax, [rbp-3032]; load GL_STENCIL_BACK_PASS_DEPTH_FAIL
		pop rcx
		mov [rbp-3032], rcx; store GL_STENCIL_BACK_PASS_DEPTH_FAIL
		mov rax, [rbp-3032]; load GL_STENCIL_BACK_PASS_DEPTH_FAIL
		mov rax, 34819
		push rax
		mov rax, [rbp-3040]; load GL_STENCIL_BACK_PASS_DEPTH_PASS
		pop rcx
		mov [rbp-3040], rcx; store GL_STENCIL_BACK_PASS_DEPTH_PASS
		mov rax, [rbp-3040]; load GL_STENCIL_BACK_PASS_DEPTH_PASS
		mov rax, 34852
		push rax
		mov rax, [rbp-3048]; load GL_MAX_DRAW_BUFFERS
		pop rcx
		mov [rbp-3048], rcx; store GL_MAX_DRAW_BUFFERS
		mov rax, [rbp-3048]; load GL_MAX_DRAW_BUFFERS
		mov rax, 34853
		push rax
		mov rax, [rbp-3056]; load GL_DRAW_BUFFER0
		pop rcx
		mov [rbp-3056], rcx; store GL_DRAW_BUFFER0
		mov rax, [rbp-3056]; load GL_DRAW_BUFFER0
		mov rax, 34854
		push rax
		mov rax, [rbp-3064]; load GL_DRAW_BUFFER1
		pop rcx
		mov [rbp-3064], rcx; store GL_DRAW_BUFFER1
		mov rax, [rbp-3064]; load GL_DRAW_BUFFER1
		mov rax, 34855
		push rax
		mov rax, [rbp-3072]; load GL_DRAW_BUFFER2
		pop rcx
		mov [rbp-3072], rcx; store GL_DRAW_BUFFER2
		mov rax, [rbp-3072]; load GL_DRAW_BUFFER2
		mov rax, 34856
		push rax
		mov rax, [rbp-3080]; load GL_DRAW_BUFFER3
		pop rcx
		mov [rbp-3080], rcx; store GL_DRAW_BUFFER3
		mov rax, [rbp-3080]; load GL_DRAW_BUFFER3
		mov rax, 34857
		push rax
		mov rax, [rbp-3088]; load GL_DRAW_BUFFER4
		pop rcx
		mov [rbp-3088], rcx; store GL_DRAW_BUFFER4
		mov rax, [rbp-3088]; load GL_DRAW_BUFFER4
		mov rax, 34858
		push rax
		mov rax, [rbp-3096]; load GL_DRAW_BUFFER5
		pop rcx
		mov [rbp-3096], rcx; store GL_DRAW_BUFFER5
		mov rax, [rbp-3096]; load GL_DRAW_BUFFER5
		mov rax, 34859
		push rax
		mov rax, [rbp-3104]; load GL_DRAW_BUFFER6
		pop rcx
		mov [rbp-3104], rcx; store GL_DRAW_BUFFER6
		mov rax, [rbp-3104]; load GL_DRAW_BUFFER6
		mov rax, 34860
		push rax
		mov rax, [rbp-3112]; load GL_DRAW_BUFFER7
		pop rcx
		mov [rbp-3112], rcx; store GL_DRAW_BUFFER7
		mov rax, [rbp-3112]; load GL_DRAW_BUFFER7
		mov rax, 34861
		push rax
		mov rax, [rbp-3120]; load GL_DRAW_BUFFER8
		pop rcx
		mov [rbp-3120], rcx; store GL_DRAW_BUFFER8
		mov rax, [rbp-3120]; load GL_DRAW_BUFFER8
		mov rax, 34862
		push rax
		mov rax, [rbp-3128]; load GL_DRAW_BUFFER9
		pop rcx
		mov [rbp-3128], rcx; store GL_DRAW_BUFFER9
		mov rax, [rbp-3128]; load GL_DRAW_BUFFER9
		mov rax, 34863
		push rax
		mov rax, [rbp-3136]; load GL_DRAW_BUFFER10
		pop rcx
		mov [rbp-3136], rcx; store GL_DRAW_BUFFER10
		mov rax, [rbp-3136]; load GL_DRAW_BUFFER10
		mov rax, 34864
		push rax
		mov rax, [rbp-3144]; load GL_DRAW_BUFFER11
		pop rcx
		mov [rbp-3144], rcx; store GL_DRAW_BUFFER11
		mov rax, [rbp-3144]; load GL_DRAW_BUFFER11
		mov rax, 34865
		push rax
		mov rax, [rbp-3152]; load GL_DRAW_BUFFER12
		pop rcx
		mov [rbp-3152], rcx; store GL_DRAW_BUFFER12
		mov rax, [rbp-3152]; load GL_DRAW_BUFFER12
		mov rax, 34866
		push rax
		mov rax, [rbp-3160]; load GL_DRAW_BUFFER13
		pop rcx
		mov [rbp-3160], rcx; store GL_DRAW_BUFFER13
		mov rax, [rbp-3160]; load GL_DRAW_BUFFER13
		mov rax, 34867
		push rax
		mov rax, [rbp-3168]; load GL_DRAW_BUFFER14
		pop rcx
		mov [rbp-3168], rcx; store GL_DRAW_BUFFER14
		mov rax, [rbp-3168]; load GL_DRAW_BUFFER14
		mov rax, 34868
		push rax
		mov rax, [rbp-3176]; load GL_DRAW_BUFFER15
		pop rcx
		mov [rbp-3176], rcx; store GL_DRAW_BUFFER15
		mov rax, [rbp-3176]; load GL_DRAW_BUFFER15
		mov rax, 34877
		push rax
		mov rax, [rbp-3184]; load GL_BLEND_EQUATION_ALPHA
		pop rcx
		mov [rbp-3184], rcx; store GL_BLEND_EQUATION_ALPHA
		mov rax, [rbp-3184]; load GL_BLEND_EQUATION_ALPHA
		mov rax, 34921
		push rax
		mov rax, [rbp-3192]; load GL_MAX_VERTEX_ATTRIBS
		pop rcx
		mov [rbp-3192], rcx; store GL_MAX_VERTEX_ATTRIBS
		mov rax, [rbp-3192]; load GL_MAX_VERTEX_ATTRIBS
		mov rax, 34922
		push rax
		mov rax, [rbp-3200]; load GL_VERTEX_ATTRIB_ARRAY_NORMALIZED
		pop rcx
		mov [rbp-3200], rcx; store GL_VERTEX_ATTRIB_ARRAY_NORMALIZED
		mov rax, [rbp-3200]; load GL_VERTEX_ATTRIB_ARRAY_NORMALIZED
		mov rax, 34930
		push rax
		mov rax, [rbp-3208]; load GL_MAX_TEXTURE_IMAGE_UNITS
		pop rcx
		mov [rbp-3208], rcx; store GL_MAX_TEXTURE_IMAGE_UNITS
		mov rax, [rbp-3208]; load GL_MAX_TEXTURE_IMAGE_UNITS
		mov rax, 35632
		push rax
		mov rax, [rbp-3216]; load GL_FRAGMENT_SHADER
		pop rcx
		mov [rbp-3216], rcx; store GL_FRAGMENT_SHADER
		mov rax, [rbp-3216]; load GL_FRAGMENT_SHADER
		mov rax, 35633
		push rax
		mov rax, [rbp-3224]; load GL_VERTEX_SHADER
		pop rcx
		mov [rbp-3224], rcx; store GL_VERTEX_SHADER
		mov rax, [rbp-3224]; load GL_VERTEX_SHADER
		mov rax, 35657
		push rax
		mov rax, [rbp-3232]; load GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-3232], rcx; store GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
		mov rax, [rbp-3232]; load GL_MAX_FRAGMENT_UNIFORM_COMPONENTS
		mov rax, 35658
		push rax
		mov rax, [rbp-3240]; load GL_MAX_VERTEX_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-3240], rcx; store GL_MAX_VERTEX_UNIFORM_COMPONENTS
		mov rax, [rbp-3240]; load GL_MAX_VERTEX_UNIFORM_COMPONENTS
		mov rax, 35659
		push rax
		mov rax, [rbp-3248]; load GL_MAX_VARYING_FLOATS
		pop rcx
		mov [rbp-3248], rcx; store GL_MAX_VARYING_FLOATS
		mov rax, [rbp-3248]; load GL_MAX_VARYING_FLOATS
		mov rax, 35660
		push rax
		mov rax, [rbp-3256]; load GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
		pop rcx
		mov [rbp-3256], rcx; store GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
		mov rax, [rbp-3256]; load GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS
		mov rax, 35661
		push rax
		mov rax, [rbp-3264]; load GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
		pop rcx
		mov [rbp-3264], rcx; store GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
		mov rax, [rbp-3264]; load GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS
		mov rax, 35663
		push rax
		mov rax, [rbp-3272]; load GL_SHADER_TYPE
		pop rcx
		mov [rbp-3272], rcx; store GL_SHADER_TYPE
		mov rax, [rbp-3272]; load GL_SHADER_TYPE
		mov rax, 35664
		push rax
		mov rax, [rbp-3280]; load GL_FLOAT_VEC2
		pop rcx
		mov [rbp-3280], rcx; store GL_FLOAT_VEC2
		mov rax, [rbp-3280]; load GL_FLOAT_VEC2
		mov rax, 35665
		push rax
		mov rax, [rbp-3288]; load GL_FLOAT_VEC3
		pop rcx
		mov [rbp-3288], rcx; store GL_FLOAT_VEC3
		mov rax, [rbp-3288]; load GL_FLOAT_VEC3
		mov rax, 35666
		push rax
		mov rax, [rbp-3296]; load GL_FLOAT_VEC4
		pop rcx
		mov [rbp-3296], rcx; store GL_FLOAT_VEC4
		mov rax, [rbp-3296]; load GL_FLOAT_VEC4
		mov rax, 35667
		push rax
		mov rax, [rbp-3304]; load GL_INT_VEC2
		pop rcx
		mov [rbp-3304], rcx; store GL_INT_VEC2
		mov rax, [rbp-3304]; load GL_INT_VEC2
		mov rax, 35668
		push rax
		mov rax, [rbp-3312]; load GL_INT_VEC3
		pop rcx
		mov [rbp-3312], rcx; store GL_INT_VEC3
		mov rax, [rbp-3312]; load GL_INT_VEC3
		mov rax, 35669
		push rax
		mov rax, [rbp-3320]; load GL_INT_VEC4
		pop rcx
		mov [rbp-3320], rcx; store GL_INT_VEC4
		mov rax, [rbp-3320]; load GL_INT_VEC4
		mov rax, 35670
		push rax
		mov rax, [rbp-3328]; load GL_BOOL
		pop rcx
		mov [rbp-3328], rcx; store GL_BOOL
		mov rax, [rbp-3328]; load GL_BOOL
		mov rax, 35671
		push rax
		mov rax, [rbp-3336]; load GL_BOOL_VEC2
		pop rcx
		mov [rbp-3336], rcx; store GL_BOOL_VEC2
		mov rax, [rbp-3336]; load GL_BOOL_VEC2
		mov rax, 35672
		push rax
		mov rax, [rbp-3344]; load GL_BOOL_VEC3
		pop rcx
		mov [rbp-3344], rcx; store GL_BOOL_VEC3
		mov rax, [rbp-3344]; load GL_BOOL_VEC3
		mov rax, 35673
		push rax
		mov rax, [rbp-3352]; load GL_BOOL_VEC4
		pop rcx
		mov [rbp-3352], rcx; store GL_BOOL_VEC4
		mov rax, [rbp-3352]; load GL_BOOL_VEC4
		mov rax, 35674
		push rax
		mov rax, [rbp-3360]; load GL_FLOAT_MAT2
		pop rcx
		mov [rbp-3360], rcx; store GL_FLOAT_MAT2
		mov rax, [rbp-3360]; load GL_FLOAT_MAT2
		mov rax, 35675
		push rax
		mov rax, [rbp-3368]; load GL_FLOAT_MAT3
		pop rcx
		mov [rbp-3368], rcx; store GL_FLOAT_MAT3
		mov rax, [rbp-3368]; load GL_FLOAT_MAT3
		mov rax, 35676
		push rax
		mov rax, [rbp-3376]; load GL_FLOAT_MAT4
		pop rcx
		mov [rbp-3376], rcx; store GL_FLOAT_MAT4
		mov rax, [rbp-3376]; load GL_FLOAT_MAT4
		mov rax, 35677
		push rax
		mov rax, [rbp-3384]; load GL_SAMPLER_1D
		pop rcx
		mov [rbp-3384], rcx; store GL_SAMPLER_1D
		mov rax, [rbp-3384]; load GL_SAMPLER_1D
		mov rax, 35678
		push rax
		mov rax, [rbp-3392]; load GL_SAMPLER_2D
		pop rcx
		mov [rbp-3392], rcx; store GL_SAMPLER_2D
		mov rax, [rbp-3392]; load GL_SAMPLER_2D
		mov rax, 35679
		push rax
		mov rax, [rbp-3400]; load GL_SAMPLER_3D
		pop rcx
		mov [rbp-3400], rcx; store GL_SAMPLER_3D
		mov rax, [rbp-3400]; load GL_SAMPLER_3D
		mov rax, 35680
		push rax
		mov rax, [rbp-3408]; load GL_SAMPLER_CUBE
		pop rcx
		mov [rbp-3408], rcx; store GL_SAMPLER_CUBE
		mov rax, [rbp-3408]; load GL_SAMPLER_CUBE
		mov rax, 35681
		push rax
		mov rax, [rbp-3416]; load GL_SAMPLER_1D_SHADOW
		pop rcx
		mov [rbp-3416], rcx; store GL_SAMPLER_1D_SHADOW
		mov rax, [rbp-3416]; load GL_SAMPLER_1D_SHADOW
		mov rax, 35682
		push rax
		mov rax, [rbp-3424]; load GL_SAMPLER_2D_SHADOW
		pop rcx
		mov [rbp-3424], rcx; store GL_SAMPLER_2D_SHADOW
		mov rax, [rbp-3424]; load GL_SAMPLER_2D_SHADOW
		mov rax, 35712
		push rax
		mov rax, [rbp-3432]; load GL_DELETE_STATUS
		pop rcx
		mov [rbp-3432], rcx; store GL_DELETE_STATUS
		mov rax, [rbp-3432]; load GL_DELETE_STATUS
		mov rax, 35713
		push rax
		mov rax, [rbp-3440]; load GL_COMPILE_STATUS
		pop rcx
		mov [rbp-3440], rcx; store GL_COMPILE_STATUS
		mov rax, [rbp-3440]; load GL_COMPILE_STATUS
		mov rax, 35714
		push rax
		mov rax, [rbp-3448]; load GL_LINK_STATUS
		pop rcx
		mov [rbp-3448], rcx; store GL_LINK_STATUS
		mov rax, [rbp-3448]; load GL_LINK_STATUS
		mov rax, 35715
		push rax
		mov rax, [rbp-3456]; load GL_VALIDATE_STATUS
		pop rcx
		mov [rbp-3456], rcx; store GL_VALIDATE_STATUS
		mov rax, [rbp-3456]; load GL_VALIDATE_STATUS
		mov rax, 35716
		push rax
		mov rax, [rbp-3464]; load GL_INFO_LOG_LENGTH
		pop rcx
		mov [rbp-3464], rcx; store GL_INFO_LOG_LENGTH
		mov rax, [rbp-3464]; load GL_INFO_LOG_LENGTH
		mov rax, 35717
		push rax
		mov rax, [rbp-3472]; load GL_ATTACHED_SHADERS
		pop rcx
		mov [rbp-3472], rcx; store GL_ATTACHED_SHADERS
		mov rax, [rbp-3472]; load GL_ATTACHED_SHADERS
		mov rax, 35718
		push rax
		mov rax, [rbp-3480]; load GL_ACTIVE_UNIFORMS
		pop rcx
		mov [rbp-3480], rcx; store GL_ACTIVE_UNIFORMS
		mov rax, [rbp-3480]; load GL_ACTIVE_UNIFORMS
		mov rax, 35719
		push rax
		mov rax, [rbp-3488]; load GL_ACTIVE_UNIFORM_MAX_LENGTH
		pop rcx
		mov [rbp-3488], rcx; store GL_ACTIVE_UNIFORM_MAX_LENGTH
		mov rax, [rbp-3488]; load GL_ACTIVE_UNIFORM_MAX_LENGTH
		mov rax, 35720
		push rax
		mov rax, [rbp-3496]; load GL_SHADER_SOURCE_LENGTH
		pop rcx
		mov [rbp-3496], rcx; store GL_SHADER_SOURCE_LENGTH
		mov rax, [rbp-3496]; load GL_SHADER_SOURCE_LENGTH
		mov rax, 35721
		push rax
		mov rax, [rbp-3504]; load GL_ACTIVE_ATTRIBUTES
		pop rcx
		mov [rbp-3504], rcx; store GL_ACTIVE_ATTRIBUTES
		mov rax, [rbp-3504]; load GL_ACTIVE_ATTRIBUTES
		mov rax, 35722
		push rax
		mov rax, [rbp-3512]; load GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
		pop rcx
		mov [rbp-3512], rcx; store GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
		mov rax, [rbp-3512]; load GL_ACTIVE_ATTRIBUTE_MAX_LENGTH
		mov rax, 35723
		push rax
		mov rax, [rbp-3520]; load GL_FRAGMENT_SHADER_DERIVATIVE_HINT
		pop rcx
		mov [rbp-3520], rcx; store GL_FRAGMENT_SHADER_DERIVATIVE_HINT
		mov rax, [rbp-3520]; load GL_FRAGMENT_SHADER_DERIVATIVE_HINT
		mov rax, 35724
		push rax
		mov rax, [rbp-3528]; load GL_SHADING_LANGUAGE_VERSION
		pop rcx
		mov [rbp-3528], rcx; store GL_SHADING_LANGUAGE_VERSION
		mov rax, [rbp-3528]; load GL_SHADING_LANGUAGE_VERSION
		mov rax, 35725
		push rax
		mov rax, [rbp-3536]; load GL_CURRENT_PROGRAM
		pop rcx
		mov [rbp-3536], rcx; store GL_CURRENT_PROGRAM
		mov rax, [rbp-3536]; load GL_CURRENT_PROGRAM
		mov rax, 36000
		push rax
		mov rax, [rbp-3544]; load GL_POINT_SPRITE_COORD_ORIGIN
		pop rcx
		mov [rbp-3544], rcx; store GL_POINT_SPRITE_COORD_ORIGIN
		mov rax, [rbp-3544]; load GL_POINT_SPRITE_COORD_ORIGIN
		mov rax, 36001
		push rax
		mov rax, [rbp-3552]; load GL_LOWER_LEFT
		pop rcx
		mov [rbp-3552], rcx; store GL_LOWER_LEFT
		mov rax, [rbp-3552]; load GL_LOWER_LEFT
		mov rax, 36002
		push rax
		mov rax, [rbp-3560]; load GL_UPPER_LEFT
		pop rcx
		mov [rbp-3560], rcx; store GL_UPPER_LEFT
		mov rax, [rbp-3560]; load GL_UPPER_LEFT
		mov rax, 36003
		push rax
		mov rax, [rbp-3568]; load GL_STENCIL_BACK_REF
		pop rcx
		mov [rbp-3568], rcx; store GL_STENCIL_BACK_REF
		mov rax, [rbp-3568]; load GL_STENCIL_BACK_REF
		mov rax, 36004
		push rax
		mov rax, [rbp-3576]; load GL_STENCIL_BACK_VALUE_MASK
		pop rcx
		mov [rbp-3576], rcx; store GL_STENCIL_BACK_VALUE_MASK
		mov rax, [rbp-3576]; load GL_STENCIL_BACK_VALUE_MASK
		mov rax, 36005
		push rax
		mov rax, [rbp-3584]; load GL_STENCIL_BACK_WRITEMASK
		pop rcx
		mov [rbp-3584], rcx; store GL_STENCIL_BACK_WRITEMASK
		mov rax, [rbp-3584]; load GL_STENCIL_BACK_WRITEMASK
		mov rax, 35051
		push rax
		mov rax, [rbp-3592]; load GL_PIXEL_PACK_BUFFER
		pop rcx
		mov [rbp-3592], rcx; store GL_PIXEL_PACK_BUFFER
		mov rax, [rbp-3592]; load GL_PIXEL_PACK_BUFFER
		mov rax, 35052
		push rax
		mov rax, [rbp-3600]; load GL_PIXEL_UNPACK_BUFFER
		pop rcx
		mov [rbp-3600], rcx; store GL_PIXEL_UNPACK_BUFFER
		mov rax, [rbp-3600]; load GL_PIXEL_UNPACK_BUFFER
		mov rax, 35053
		push rax
		mov rax, [rbp-3608]; load GL_PIXEL_PACK_BUFFER_BINDING
		pop rcx
		mov [rbp-3608], rcx; store GL_PIXEL_PACK_BUFFER_BINDING
		mov rax, [rbp-3608]; load GL_PIXEL_PACK_BUFFER_BINDING
		mov rax, 35055
		push rax
		mov rax, [rbp-3616]; load GL_PIXEL_UNPACK_BUFFER_BINDING
		pop rcx
		mov [rbp-3616], rcx; store GL_PIXEL_UNPACK_BUFFER_BINDING
		mov rax, [rbp-3616]; load GL_PIXEL_UNPACK_BUFFER_BINDING
		mov rax, 35685
		push rax
		mov rax, [rbp-3624]; load GL_FLOAT_MAT2x3
		pop rcx
		mov [rbp-3624], rcx; store GL_FLOAT_MAT2x3
		mov rax, [rbp-3624]; load GL_FLOAT_MAT2x3
		mov rax, 35686
		push rax
		mov rax, [rbp-3632]; load GL_FLOAT_MAT2x4
		pop rcx
		mov [rbp-3632], rcx; store GL_FLOAT_MAT2x4
		mov rax, [rbp-3632]; load GL_FLOAT_MAT2x4
		mov rax, 35687
		push rax
		mov rax, [rbp-3640]; load GL_FLOAT_MAT3x2
		pop rcx
		mov [rbp-3640], rcx; store GL_FLOAT_MAT3x2
		mov rax, [rbp-3640]; load GL_FLOAT_MAT3x2
		mov rax, 35688
		push rax
		mov rax, [rbp-3648]; load GL_FLOAT_MAT3x4
		pop rcx
		mov [rbp-3648], rcx; store GL_FLOAT_MAT3x4
		mov rax, [rbp-3648]; load GL_FLOAT_MAT3x4
		mov rax, 35689
		push rax
		mov rax, [rbp-3656]; load GL_FLOAT_MAT4x2
		pop rcx
		mov [rbp-3656], rcx; store GL_FLOAT_MAT4x2
		mov rax, [rbp-3656]; load GL_FLOAT_MAT4x2
		mov rax, 35690
		push rax
		mov rax, [rbp-3664]; load GL_FLOAT_MAT4x3
		pop rcx
		mov [rbp-3664], rcx; store GL_FLOAT_MAT4x3
		mov rax, [rbp-3664]; load GL_FLOAT_MAT4x3
		mov rax, 35904
		push rax
		mov rax, [rbp-3672]; load GL_SRGB
		pop rcx
		mov [rbp-3672], rcx; store GL_SRGB
		mov rax, [rbp-3672]; load GL_SRGB
		mov rax, 35905
		push rax
		mov rax, [rbp-3680]; load GL_SRGB8
		pop rcx
		mov [rbp-3680], rcx; store GL_SRGB8
		mov rax, [rbp-3680]; load GL_SRGB8
		mov rax, 35906
		push rax
		mov rax, [rbp-3688]; load GL_SRGB_ALPHA
		pop rcx
		mov [rbp-3688], rcx; store GL_SRGB_ALPHA
		mov rax, [rbp-3688]; load GL_SRGB_ALPHA
		mov rax, 35907
		push rax
		mov rax, [rbp-3696]; load GL_SRGB8_ALPHA8
		pop rcx
		mov [rbp-3696], rcx; store GL_SRGB8_ALPHA8
		mov rax, [rbp-3696]; load GL_SRGB8_ALPHA8
		mov rax, 35912
		push rax
		mov rax, [rbp-3704]; load GL_COMPRESSED_SRGB
		pop rcx
		mov [rbp-3704], rcx; store GL_COMPRESSED_SRGB
		mov rax, [rbp-3704]; load GL_COMPRESSED_SRGB
		mov rax, 35913
		push rax
		mov rax, [rbp-3712]; load GL_COMPRESSED_SRGB_ALPHA
		pop rcx
		mov [rbp-3712], rcx; store GL_COMPRESSED_SRGB_ALPHA
		mov rax, [rbp-3712]; load GL_COMPRESSED_SRGB_ALPHA
		mov rax, 34894
		push rax
		mov rax, [rbp-3720]; load GL_COMPARE_REF_TO_TEXTURE
		pop rcx
		mov [rbp-3720], rcx; store GL_COMPARE_REF_TO_TEXTURE
		mov rax, [rbp-3720]; load GL_COMPARE_REF_TO_TEXTURE
		mov rax, 12288
		push rax
		mov rax, [rbp-3728]; load GL_CLIP_DISTANCE0
		pop rcx
		mov [rbp-3728], rcx; store GL_CLIP_DISTANCE0
		mov rax, [rbp-3728]; load GL_CLIP_DISTANCE0
		mov rax, 12289
		push rax
		mov rax, [rbp-3736]; load GL_CLIP_DISTANCE1
		pop rcx
		mov [rbp-3736], rcx; store GL_CLIP_DISTANCE1
		mov rax, [rbp-3736]; load GL_CLIP_DISTANCE1
		mov rax, 12290
		push rax
		mov rax, [rbp-3744]; load GL_CLIP_DISTANCE2
		pop rcx
		mov [rbp-3744], rcx; store GL_CLIP_DISTANCE2
		mov rax, [rbp-3744]; load GL_CLIP_DISTANCE2
		mov rax, 12291
		push rax
		mov rax, [rbp-3752]; load GL_CLIP_DISTANCE3
		pop rcx
		mov [rbp-3752], rcx; store GL_CLIP_DISTANCE3
		mov rax, [rbp-3752]; load GL_CLIP_DISTANCE3
		mov rax, 12292
		push rax
		mov rax, [rbp-3760]; load GL_CLIP_DISTANCE4
		pop rcx
		mov [rbp-3760], rcx; store GL_CLIP_DISTANCE4
		mov rax, [rbp-3760]; load GL_CLIP_DISTANCE4
		mov rax, 12293
		push rax
		mov rax, [rbp-3768]; load GL_CLIP_DISTANCE5
		pop rcx
		mov [rbp-3768], rcx; store GL_CLIP_DISTANCE5
		mov rax, [rbp-3768]; load GL_CLIP_DISTANCE5
		mov rax, 12294
		push rax
		mov rax, [rbp-3776]; load GL_CLIP_DISTANCE6
		pop rcx
		mov [rbp-3776], rcx; store GL_CLIP_DISTANCE6
		mov rax, [rbp-3776]; load GL_CLIP_DISTANCE6
		mov rax, 12295
		push rax
		mov rax, [rbp-3784]; load GL_CLIP_DISTANCE7
		pop rcx
		mov [rbp-3784], rcx; store GL_CLIP_DISTANCE7
		mov rax, [rbp-3784]; load GL_CLIP_DISTANCE7
		mov rax, 3378
		push rax
		mov rax, [rbp-3792]; load GL_MAX_CLIP_DISTANCES
		pop rcx
		mov [rbp-3792], rcx; store GL_MAX_CLIP_DISTANCES
		mov rax, [rbp-3792]; load GL_MAX_CLIP_DISTANCES
		mov rax, 33307
		push rax
		mov rax, [rbp-3800]; load GL_MAJOR_VERSION
		pop rcx
		mov [rbp-3800], rcx; store GL_MAJOR_VERSION
		mov rax, [rbp-3800]; load GL_MAJOR_VERSION
		mov rax, 33308
		push rax
		mov rax, [rbp-3808]; load GL_MINOR_VERSION
		pop rcx
		mov [rbp-3808], rcx; store GL_MINOR_VERSION
		mov rax, [rbp-3808]; load GL_MINOR_VERSION
		mov rax, 33309
		push rax
		mov rax, [rbp-3816]; load GL_NUM_EXTENSIONS
		pop rcx
		mov [rbp-3816], rcx; store GL_NUM_EXTENSIONS
		mov rax, [rbp-3816]; load GL_NUM_EXTENSIONS
		mov rax, 33310
		push rax
		mov rax, [rbp-3824]; load GL_CONTEXT_FLAGS
		pop rcx
		mov [rbp-3824], rcx; store GL_CONTEXT_FLAGS
		mov rax, [rbp-3824]; load GL_CONTEXT_FLAGS
		mov rax, 33317
		push rax
		mov rax, [rbp-3832]; load GL_COMPRESSED_RED
		pop rcx
		mov [rbp-3832], rcx; store GL_COMPRESSED_RED
		mov rax, [rbp-3832]; load GL_COMPRESSED_RED
		mov rax, 33318
		push rax
		mov rax, [rbp-3840]; load GL_COMPRESSED_RG
		pop rcx
		mov [rbp-3840], rcx; store GL_COMPRESSED_RG
		mov rax, [rbp-3840]; load GL_COMPRESSED_RG
		mov rax, 1
		push rax
		mov rax, [rbp-3848]; load GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT
		pop rcx
		mov [rbp-3848], rcx; store GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT
		mov rax, [rbp-3848]; load GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT
		mov rax, 34836
		push rax
		mov rax, [rbp-3856]; load GL_RGBA32F
		pop rcx
		mov [rbp-3856], rcx; store GL_RGBA32F
		mov rax, [rbp-3856]; load GL_RGBA32F
		mov rax, 34837
		push rax
		mov rax, [rbp-3864]; load GL_RGB32F
		pop rcx
		mov [rbp-3864], rcx; store GL_RGB32F
		mov rax, [rbp-3864]; load GL_RGB32F
		mov rax, 34842
		push rax
		mov rax, [rbp-3872]; load GL_RGBA16F
		pop rcx
		mov [rbp-3872], rcx; store GL_RGBA16F
		mov rax, [rbp-3872]; load GL_RGBA16F
		mov rax, 34843
		push rax
		mov rax, [rbp-3880]; load GL_RGB16F
		pop rcx
		mov [rbp-3880], rcx; store GL_RGB16F
		mov rax, [rbp-3880]; load GL_RGB16F
		mov rax, 35069
		push rax
		mov rax, [rbp-3888]; load GL_VERTEX_ATTRIB_ARRAY_INTEGER
		pop rcx
		mov [rbp-3888], rcx; store GL_VERTEX_ATTRIB_ARRAY_INTEGER
		mov rax, [rbp-3888]; load GL_VERTEX_ATTRIB_ARRAY_INTEGER
		mov rax, 35071
		push rax
		mov rax, [rbp-3896]; load GL_MAX_ARRAY_TEXTURE_LAYERS
		pop rcx
		mov [rbp-3896], rcx; store GL_MAX_ARRAY_TEXTURE_LAYERS
		mov rax, [rbp-3896]; load GL_MAX_ARRAY_TEXTURE_LAYERS
		mov rax, 35076
		push rax
		mov rax, [rbp-3904]; load GL_MIN_PROGRAM_TEXEL_OFFSET
		pop rcx
		mov [rbp-3904], rcx; store GL_MIN_PROGRAM_TEXEL_OFFSET
		mov rax, [rbp-3904]; load GL_MIN_PROGRAM_TEXEL_OFFSET
		mov rax, 35077
		push rax
		mov rax, [rbp-3912]; load GL_MAX_PROGRAM_TEXEL_OFFSET
		pop rcx
		mov [rbp-3912], rcx; store GL_MAX_PROGRAM_TEXEL_OFFSET
		mov rax, [rbp-3912]; load GL_MAX_PROGRAM_TEXEL_OFFSET
		mov rax, 35100
		push rax
		mov rax, [rbp-3920]; load GL_CLAMP_READ_COLOR
		pop rcx
		mov [rbp-3920], rcx; store GL_CLAMP_READ_COLOR
		mov rax, [rbp-3920]; load GL_CLAMP_READ_COLOR
		mov rax, 35101
		push rax
		mov rax, [rbp-3928]; load GL_FIXED_ONLY
		pop rcx
		mov [rbp-3928], rcx; store GL_FIXED_ONLY
		mov rax, [rbp-3928]; load GL_FIXED_ONLY
		mov rax, 35659
		push rax
		mov rax, [rbp-3936]; load GL_MAX_VARYING_COMPONENTS
		pop rcx
		mov [rbp-3936], rcx; store GL_MAX_VARYING_COMPONENTS
		mov rax, [rbp-3936]; load GL_MAX_VARYING_COMPONENTS
		mov rax, 35864
		push rax
		mov rax, [rbp-3944]; load GL_TEXTURE_1D_ARRAY
		pop rcx
		mov [rbp-3944], rcx; store GL_TEXTURE_1D_ARRAY
		mov rax, [rbp-3944]; load GL_TEXTURE_1D_ARRAY
		mov rax, 35865
		push rax
		mov rax, [rbp-3952]; load GL_PROXY_TEXTURE_1D_ARRAY
		pop rcx
		mov [rbp-3952], rcx; store GL_PROXY_TEXTURE_1D_ARRAY
		mov rax, [rbp-3952]; load GL_PROXY_TEXTURE_1D_ARRAY
		mov rax, 35866
		push rax
		mov rax, [rbp-3960]; load GL_TEXTURE_2D_ARRAY
		pop rcx
		mov [rbp-3960], rcx; store GL_TEXTURE_2D_ARRAY
		mov rax, [rbp-3960]; load GL_TEXTURE_2D_ARRAY
		mov rax, 35867
		push rax
		mov rax, [rbp-3968]; load GL_PROXY_TEXTURE_2D_ARRAY
		pop rcx
		mov [rbp-3968], rcx; store GL_PROXY_TEXTURE_2D_ARRAY
		mov rax, [rbp-3968]; load GL_PROXY_TEXTURE_2D_ARRAY
		mov rax, 35868
		push rax
		mov rax, [rbp-3976]; load GL_TEXTURE_BINDING_1D_ARRAY
		pop rcx
		mov [rbp-3976], rcx; store GL_TEXTURE_BINDING_1D_ARRAY
		mov rax, [rbp-3976]; load GL_TEXTURE_BINDING_1D_ARRAY
		mov rax, 35869
		push rax
		mov rax, [rbp-3984]; load GL_TEXTURE_BINDING_2D_ARRAY
		pop rcx
		mov [rbp-3984], rcx; store GL_TEXTURE_BINDING_2D_ARRAY
		mov rax, [rbp-3984]; load GL_TEXTURE_BINDING_2D_ARRAY
		mov rax, 35898
		push rax
		mov rax, [rbp-3992]; load GL_R11F_G11F_B10F
		pop rcx
		mov [rbp-3992], rcx; store GL_R11F_G11F_B10F
		mov rax, [rbp-3992]; load GL_R11F_G11F_B10F
		mov rax, 35899
		push rax
		mov rax, [rbp-4000]; load GL_UNSIGNED_INT_10F_11F_11F_REV
		pop rcx
		mov [rbp-4000], rcx; store GL_UNSIGNED_INT_10F_11F_11F_REV
		mov rax, [rbp-4000]; load GL_UNSIGNED_INT_10F_11F_11F_REV
		mov rax, 35901
		push rax
		mov rax, [rbp-4008]; load GL_RGB9_E5
		pop rcx
		mov [rbp-4008], rcx; store GL_RGB9_E5
		mov rax, [rbp-4008]; load GL_RGB9_E5
		mov rax, 35902
		push rax
		mov rax, [rbp-4016]; load GL_UNSIGNED_INT_5_9_9_9_REV
		pop rcx
		mov [rbp-4016], rcx; store GL_UNSIGNED_INT_5_9_9_9_REV
		mov rax, [rbp-4016]; load GL_UNSIGNED_INT_5_9_9_9_REV
		mov rax, 35903
		push rax
		mov rax, [rbp-4024]; load GL_TEXTURE_SHARED_SIZE
		pop rcx
		mov [rbp-4024], rcx; store GL_TEXTURE_SHARED_SIZE
		mov rax, [rbp-4024]; load GL_TEXTURE_SHARED_SIZE
		mov rax, 35958
		push rax
		mov rax, [rbp-4032]; load GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH
		pop rcx
		mov [rbp-4032], rcx; store GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH
		mov rax, [rbp-4032]; load GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH
		mov rax, 35967
		push rax
		mov rax, [rbp-4040]; load GL_TRANSFORM_FEEDBACK_BUFFER_MODE
		pop rcx
		mov [rbp-4040], rcx; store GL_TRANSFORM_FEEDBACK_BUFFER_MODE
		mov rax, [rbp-4040]; load GL_TRANSFORM_FEEDBACK_BUFFER_MODE
		mov rax, 35968
		push rax
		mov rax, [rbp-4048]; load GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS
		pop rcx
		mov [rbp-4048], rcx; store GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS
		mov rax, [rbp-4048]; load GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS
		mov rax, 35971
		push rax
		mov rax, [rbp-4056]; load GL_TRANSFORM_FEEDBACK_VARYINGS
		pop rcx
		mov [rbp-4056], rcx; store GL_TRANSFORM_FEEDBACK_VARYINGS
		mov rax, [rbp-4056]; load GL_TRANSFORM_FEEDBACK_VARYINGS
		mov rax, 35972
		push rax
		mov rax, [rbp-4064]; load GL_TRANSFORM_FEEDBACK_BUFFER_START
		pop rcx
		mov [rbp-4064], rcx; store GL_TRANSFORM_FEEDBACK_BUFFER_START
		mov rax, [rbp-4064]; load GL_TRANSFORM_FEEDBACK_BUFFER_START
		mov rax, 35973
		push rax
		mov rax, [rbp-4072]; load GL_TRANSFORM_FEEDBACK_BUFFER_SIZE
		pop rcx
		mov [rbp-4072], rcx; store GL_TRANSFORM_FEEDBACK_BUFFER_SIZE
		mov rax, [rbp-4072]; load GL_TRANSFORM_FEEDBACK_BUFFER_SIZE
		mov rax, 35975
		push rax
		mov rax, [rbp-4080]; load GL_PRIMITIVES_GENERATED
		pop rcx
		mov [rbp-4080], rcx; store GL_PRIMITIVES_GENERATED
		mov rax, [rbp-4080]; load GL_PRIMITIVES_GENERATED
		mov rax, 35976
		push rax
		mov rax, [rbp-4088]; load GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
		pop rcx
		mov [rbp-4088], rcx; store GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
		mov rax, [rbp-4088]; load GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN
		mov rax, 35977
		push rax
		mov rax, [rbp-4096]; load GL_RASTERIZER_DISCARD
		pop rcx
		mov [rbp-4096], rcx; store GL_RASTERIZER_DISCARD
		mov rax, [rbp-4096]; load GL_RASTERIZER_DISCARD
		mov rax, 35978
		push rax
		mov rax, [rbp-4104]; load GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS
		pop rcx
		mov [rbp-4104], rcx; store GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS
		mov rax, [rbp-4104]; load GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS
		mov rax, 35979
		push rax
		mov rax, [rbp-4112]; load GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS
		pop rcx
		mov [rbp-4112], rcx; store GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS
		mov rax, [rbp-4112]; load GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS
		mov rax, 35980
		push rax
		mov rax, [rbp-4120]; load GL_INTERLEAVED_ATTRIBS
		pop rcx
		mov [rbp-4120], rcx; store GL_INTERLEAVED_ATTRIBS
		mov rax, [rbp-4120]; load GL_INTERLEAVED_ATTRIBS
		mov rax, 35981
		push rax
		mov rax, [rbp-4128]; load GL_SEPARATE_ATTRIBS
		pop rcx
		mov [rbp-4128], rcx; store GL_SEPARATE_ATTRIBS
		mov rax, [rbp-4128]; load GL_SEPARATE_ATTRIBS
		mov rax, 35982
		push rax
		mov rax, [rbp-4136]; load GL_TRANSFORM_FEEDBACK_BUFFER
		pop rcx
		mov [rbp-4136], rcx; store GL_TRANSFORM_FEEDBACK_BUFFER
		mov rax, [rbp-4136]; load GL_TRANSFORM_FEEDBACK_BUFFER
		mov rax, 35983
		push rax
		mov rax, [rbp-4144]; load GL_TRANSFORM_FEEDBACK_BUFFER_BINDING
		pop rcx
		mov [rbp-4144], rcx; store GL_TRANSFORM_FEEDBACK_BUFFER_BINDING
		mov rax, [rbp-4144]; load GL_TRANSFORM_FEEDBACK_BUFFER_BINDING
		mov rax, 36208
		push rax
		mov rax, [rbp-4152]; load GL_RGBA32UI
		pop rcx
		mov [rbp-4152], rcx; store GL_RGBA32UI
		mov rax, [rbp-4152]; load GL_RGBA32UI
		mov rax, 36209
		push rax
		mov rax, [rbp-4160]; load GL_RGB32UI
		pop rcx
		mov [rbp-4160], rcx; store GL_RGB32UI
		mov rax, [rbp-4160]; load GL_RGB32UI
		mov rax, 36214
		push rax
		mov rax, [rbp-4168]; load GL_RGBA16UI
		pop rcx
		mov [rbp-4168], rcx; store GL_RGBA16UI
		mov rax, [rbp-4168]; load GL_RGBA16UI
		mov rax, 36215
		push rax
		mov rax, [rbp-4176]; load GL_RGB16UI
		pop rcx
		mov [rbp-4176], rcx; store GL_RGB16UI
		mov rax, [rbp-4176]; load GL_RGB16UI
		mov rax, 36220
		push rax
		mov rax, [rbp-4184]; load GL_RGBA8UI
		pop rcx
		mov [rbp-4184], rcx; store GL_RGBA8UI
		mov rax, [rbp-4184]; load GL_RGBA8UI
		mov rax, 36221
		push rax
		mov rax, [rbp-4192]; load GL_RGB8UI
		pop rcx
		mov [rbp-4192], rcx; store GL_RGB8UI
		mov rax, [rbp-4192]; load GL_RGB8UI
		mov rax, 36226
		push rax
		mov rax, [rbp-4200]; load GL_RGBA32I
		pop rcx
		mov [rbp-4200], rcx; store GL_RGBA32I
		mov rax, [rbp-4200]; load GL_RGBA32I
		mov rax, 36227
		push rax
		mov rax, [rbp-4208]; load GL_RGB32I
		pop rcx
		mov [rbp-4208], rcx; store GL_RGB32I
		mov rax, [rbp-4208]; load GL_RGB32I
		mov rax, 36232
		push rax
		mov rax, [rbp-4216]; load GL_RGBA16I
		pop rcx
		mov [rbp-4216], rcx; store GL_RGBA16I
		mov rax, [rbp-4216]; load GL_RGBA16I
		mov rax, 36233
		push rax
		mov rax, [rbp-4224]; load GL_RGB16I
		pop rcx
		mov [rbp-4224], rcx; store GL_RGB16I
		mov rax, [rbp-4224]; load GL_RGB16I
		mov rax, 36238
		push rax
		mov rax, [rbp-4232]; load GL_RGBA8I
		pop rcx
		mov [rbp-4232], rcx; store GL_RGBA8I
		mov rax, [rbp-4232]; load GL_RGBA8I
		mov rax, 36239
		push rax
		mov rax, [rbp-4240]; load GL_RGB8I
		pop rcx
		mov [rbp-4240], rcx; store GL_RGB8I
		mov rax, [rbp-4240]; load GL_RGB8I
		mov rax, 36244
		push rax
		mov rax, [rbp-4248]; load GL_RED_INTEGER
		pop rcx
		mov [rbp-4248], rcx; store GL_RED_INTEGER
		mov rax, [rbp-4248]; load GL_RED_INTEGER
		mov rax, 36245
		push rax
		mov rax, [rbp-4256]; load GL_GREEN_INTEGER
		pop rcx
		mov [rbp-4256], rcx; store GL_GREEN_INTEGER
		mov rax, [rbp-4256]; load GL_GREEN_INTEGER
		mov rax, 36246
		push rax
		mov rax, [rbp-4264]; load GL_BLUE_INTEGER
		pop rcx
		mov [rbp-4264], rcx; store GL_BLUE_INTEGER
		mov rax, [rbp-4264]; load GL_BLUE_INTEGER
		mov rax, 36248
		push rax
		mov rax, [rbp-4272]; load GL_RGB_INTEGER
		pop rcx
		mov [rbp-4272], rcx; store GL_RGB_INTEGER
		mov rax, [rbp-4272]; load GL_RGB_INTEGER
		mov rax, 36249
		push rax
		mov rax, [rbp-4280]; load GL_RGBA_INTEGER
		pop rcx
		mov [rbp-4280], rcx; store GL_RGBA_INTEGER
		mov rax, [rbp-4280]; load GL_RGBA_INTEGER
		mov rax, 36250
		push rax
		mov rax, [rbp-4288]; load GL_BGR_INTEGER
		pop rcx
		mov [rbp-4288], rcx; store GL_BGR_INTEGER
		mov rax, [rbp-4288]; load GL_BGR_INTEGER
		mov rax, 36251
		push rax
		mov rax, [rbp-4296]; load GL_BGRA_INTEGER
		pop rcx
		mov [rbp-4296], rcx; store GL_BGRA_INTEGER
		mov rax, [rbp-4296]; load GL_BGRA_INTEGER
		mov rax, 36288
		push rax
		mov rax, [rbp-4304]; load GL_SAMPLER_1D_ARRAY
		pop rcx
		mov [rbp-4304], rcx; store GL_SAMPLER_1D_ARRAY
		mov rax, [rbp-4304]; load GL_SAMPLER_1D_ARRAY
		mov rax, 36289
		push rax
		mov rax, [rbp-4312]; load GL_SAMPLER_2D_ARRAY
		pop rcx
		mov [rbp-4312], rcx; store GL_SAMPLER_2D_ARRAY
		mov rax, [rbp-4312]; load GL_SAMPLER_2D_ARRAY
		mov rax, 36291
		push rax
		mov rax, [rbp-4320]; load GL_SAMPLER_1D_ARRAY_SHADOW
		pop rcx
		mov [rbp-4320], rcx; store GL_SAMPLER_1D_ARRAY_SHADOW
		mov rax, [rbp-4320]; load GL_SAMPLER_1D_ARRAY_SHADOW
		mov rax, 36292
		push rax
		mov rax, [rbp-4328]; load GL_SAMPLER_2D_ARRAY_SHADOW
		pop rcx
		mov [rbp-4328], rcx; store GL_SAMPLER_2D_ARRAY_SHADOW
		mov rax, [rbp-4328]; load GL_SAMPLER_2D_ARRAY_SHADOW
		mov rax, 36293
		push rax
		mov rax, [rbp-4336]; load GL_SAMPLER_CUBE_SHADOW
		pop rcx
		mov [rbp-4336], rcx; store GL_SAMPLER_CUBE_SHADOW
		mov rax, [rbp-4336]; load GL_SAMPLER_CUBE_SHADOW
		mov rax, 36294
		push rax
		mov rax, [rbp-4344]; load GL_UNSIGNED_INT_VEC2
		pop rcx
		mov [rbp-4344], rcx; store GL_UNSIGNED_INT_VEC2
		mov rax, [rbp-4344]; load GL_UNSIGNED_INT_VEC2
		mov rax, 36295
		push rax
		mov rax, [rbp-4352]; load GL_UNSIGNED_INT_VEC3
		pop rcx
		mov [rbp-4352], rcx; store GL_UNSIGNED_INT_VEC3
		mov rax, [rbp-4352]; load GL_UNSIGNED_INT_VEC3
		mov rax, 36296
		push rax
		mov rax, [rbp-4360]; load GL_UNSIGNED_INT_VEC4
		pop rcx
		mov [rbp-4360], rcx; store GL_UNSIGNED_INT_VEC4
		mov rax, [rbp-4360]; load GL_UNSIGNED_INT_VEC4
		mov rax, 36297
		push rax
		mov rax, [rbp-4368]; load GL_INT_SAMPLER_1D
		pop rcx
		mov [rbp-4368], rcx; store GL_INT_SAMPLER_1D
		mov rax, [rbp-4368]; load GL_INT_SAMPLER_1D
		mov rax, 36298
		push rax
		mov rax, [rbp-4376]; load GL_INT_SAMPLER_2D
		pop rcx
		mov [rbp-4376], rcx; store GL_INT_SAMPLER_2D
		mov rax, [rbp-4376]; load GL_INT_SAMPLER_2D
		mov rax, 36299
		push rax
		mov rax, [rbp-4384]; load GL_INT_SAMPLER_3D
		pop rcx
		mov [rbp-4384], rcx; store GL_INT_SAMPLER_3D
		mov rax, [rbp-4384]; load GL_INT_SAMPLER_3D
		mov rax, 36300
		push rax
		mov rax, [rbp-4392]; load GL_INT_SAMPLER_CUBE
		pop rcx
		mov [rbp-4392], rcx; store GL_INT_SAMPLER_CUBE
		mov rax, [rbp-4392]; load GL_INT_SAMPLER_CUBE
		mov rax, 36302
		push rax
		mov rax, [rbp-4400]; load GL_INT_SAMPLER_1D_ARRAY
		pop rcx
		mov [rbp-4400], rcx; store GL_INT_SAMPLER_1D_ARRAY
		mov rax, [rbp-4400]; load GL_INT_SAMPLER_1D_ARRAY
		mov rax, 36303
		push rax
		mov rax, [rbp-4408]; load GL_INT_SAMPLER_2D_ARRAY
		pop rcx
		mov [rbp-4408], rcx; store GL_INT_SAMPLER_2D_ARRAY
		mov rax, [rbp-4408]; load GL_INT_SAMPLER_2D_ARRAY
		mov rax, 36305
		push rax
		mov rax, [rbp-4416]; load GL_UNSIGNED_INT_SAMPLER_1D
		pop rcx
		mov [rbp-4416], rcx; store GL_UNSIGNED_INT_SAMPLER_1D
		mov rax, [rbp-4416]; load GL_UNSIGNED_INT_SAMPLER_1D
		mov rax, 36306
		push rax
		mov rax, [rbp-4424]; load GL_UNSIGNED_INT_SAMPLER_2D
		pop rcx
		mov [rbp-4424], rcx; store GL_UNSIGNED_INT_SAMPLER_2D
		mov rax, [rbp-4424]; load GL_UNSIGNED_INT_SAMPLER_2D
		mov rax, 36307
		push rax
		mov rax, [rbp-4432]; load GL_UNSIGNED_INT_SAMPLER_3D
		pop rcx
		mov [rbp-4432], rcx; store GL_UNSIGNED_INT_SAMPLER_3D
		mov rax, [rbp-4432]; load GL_UNSIGNED_INT_SAMPLER_3D
		mov rax, 36308
		push rax
		mov rax, [rbp-4440]; load GL_UNSIGNED_INT_SAMPLER_CUBE
		pop rcx
		mov [rbp-4440], rcx; store GL_UNSIGNED_INT_SAMPLER_CUBE
		mov rax, [rbp-4440]; load GL_UNSIGNED_INT_SAMPLER_CUBE
		mov rax, 36310
		push rax
		mov rax, [rbp-4448]; load GL_UNSIGNED_INT_SAMPLER_1D_ARRAY
		pop rcx
		mov [rbp-4448], rcx; store GL_UNSIGNED_INT_SAMPLER_1D_ARRAY
		mov rax, [rbp-4448]; load GL_UNSIGNED_INT_SAMPLER_1D_ARRAY
		mov rax, 36311
		push rax
		mov rax, [rbp-4456]; load GL_UNSIGNED_INT_SAMPLER_2D_ARRAY
		pop rcx
		mov [rbp-4456], rcx; store GL_UNSIGNED_INT_SAMPLER_2D_ARRAY
		mov rax, [rbp-4456]; load GL_UNSIGNED_INT_SAMPLER_2D_ARRAY
		mov rax, 36371
		push rax
		mov rax, [rbp-4464]; load GL_QUERY_WAIT
		pop rcx
		mov [rbp-4464], rcx; store GL_QUERY_WAIT
		mov rax, [rbp-4464]; load GL_QUERY_WAIT
		mov rax, 36372
		push rax
		mov rax, [rbp-4472]; load GL_QUERY_NO_WAIT
		pop rcx
		mov [rbp-4472], rcx; store GL_QUERY_NO_WAIT
		mov rax, [rbp-4472]; load GL_QUERY_NO_WAIT
		mov rax, 36373
		push rax
		mov rax, [rbp-4480]; load GL_QUERY_BY_REGION_WAIT
		pop rcx
		mov [rbp-4480], rcx; store GL_QUERY_BY_REGION_WAIT
		mov rax, [rbp-4480]; load GL_QUERY_BY_REGION_WAIT
		mov rax, 36374
		push rax
		mov rax, [rbp-4488]; load GL_QUERY_BY_REGION_NO_WAIT
		pop rcx
		mov [rbp-4488], rcx; store GL_QUERY_BY_REGION_NO_WAIT
		mov rax, [rbp-4488]; load GL_QUERY_BY_REGION_NO_WAIT
		mov rax, 37151
		push rax
		mov rax, [rbp-4496]; load GL_BUFFER_ACCESS_FLAGS
		pop rcx
		mov [rbp-4496], rcx; store GL_BUFFER_ACCESS_FLAGS
		mov rax, [rbp-4496]; load GL_BUFFER_ACCESS_FLAGS
		mov rax, 37152
		push rax
		mov rax, [rbp-4504]; load GL_BUFFER_MAP_LENGTH
		pop rcx
		mov [rbp-4504], rcx; store GL_BUFFER_MAP_LENGTH
		mov rax, [rbp-4504]; load GL_BUFFER_MAP_LENGTH
		mov rax, 37153
		push rax
		mov rax, [rbp-4512]; load GL_BUFFER_MAP_OFFSET
		pop rcx
		mov [rbp-4512], rcx; store GL_BUFFER_MAP_OFFSET
		mov rax, [rbp-4512]; load GL_BUFFER_MAP_OFFSET
		mov rax, 36012
		push rax
		mov rax, [rbp-4520]; load GL_DEPTH_COMPONENT32F
		pop rcx
		mov [rbp-4520], rcx; store GL_DEPTH_COMPONENT32F
		mov rax, [rbp-4520]; load GL_DEPTH_COMPONENT32F
		mov rax, 36013
		push rax
		mov rax, [rbp-4528]; load GL_DEPTH32F_STENCIL8
		pop rcx
		mov [rbp-4528], rcx; store GL_DEPTH32F_STENCIL8
		mov rax, [rbp-4528]; load GL_DEPTH32F_STENCIL8
		mov rax, 36269
		push rax
		mov rax, [rbp-4536]; load GL_FLOAT_32_UNSIGNED_INT_24_8_REV
		pop rcx
		mov [rbp-4536], rcx; store GL_FLOAT_32_UNSIGNED_INT_24_8_REV
		mov rax, [rbp-4536]; load GL_FLOAT_32_UNSIGNED_INT_24_8_REV
		mov rax, 1286
		push rax
		mov rax, [rbp-4544]; load GL_INVALID_FRAMEBUFFER_OPERATION
		pop rcx
		mov [rbp-4544], rcx; store GL_INVALID_FRAMEBUFFER_OPERATION
		mov rax, [rbp-4544]; load GL_INVALID_FRAMEBUFFER_OPERATION
		mov rax, 33296
		push rax
		mov rax, [rbp-4552]; load GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING
		pop rcx
		mov [rbp-4552], rcx; store GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING
		mov rax, [rbp-4552]; load GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING
		mov rax, 33297
		push rax
		mov rax, [rbp-4560]; load GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE
		pop rcx
		mov [rbp-4560], rcx; store GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE
		mov rax, [rbp-4560]; load GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE
		mov rax, 33298
		push rax
		mov rax, [rbp-4568]; load GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE
		pop rcx
		mov [rbp-4568], rcx; store GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE
		mov rax, [rbp-4568]; load GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE
		mov rax, 33299
		push rax
		mov rax, [rbp-4576]; load GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE
		pop rcx
		mov [rbp-4576], rcx; store GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE
		mov rax, [rbp-4576]; load GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE
		mov rax, 33300
		push rax
		mov rax, [rbp-4584]; load GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE
		pop rcx
		mov [rbp-4584], rcx; store GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE
		mov rax, [rbp-4584]; load GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE
		mov rax, 33301
		push rax
		mov rax, [rbp-4592]; load GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE
		pop rcx
		mov [rbp-4592], rcx; store GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE
		mov rax, [rbp-4592]; load GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE
		mov rax, 33302
		push rax
		mov rax, [rbp-4600]; load GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE
		pop rcx
		mov [rbp-4600], rcx; store GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE
		mov rax, [rbp-4600]; load GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE
		mov rax, 33303
		push rax
		mov rax, [rbp-4608]; load GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE
		pop rcx
		mov [rbp-4608], rcx; store GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE
		mov rax, [rbp-4608]; load GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE
		mov rax, 33304
		push rax
		mov rax, [rbp-4616]; load GL_FRAMEBUFFER_DEFAULT
		pop rcx
		mov [rbp-4616], rcx; store GL_FRAMEBUFFER_DEFAULT
		mov rax, [rbp-4616]; load GL_FRAMEBUFFER_DEFAULT
		mov rax, 33305
		push rax
		mov rax, [rbp-4624]; load GL_FRAMEBUFFER_UNDEFINED
		pop rcx
		mov [rbp-4624], rcx; store GL_FRAMEBUFFER_UNDEFINED
		mov rax, [rbp-4624]; load GL_FRAMEBUFFER_UNDEFINED
		mov rax, 33306
		push rax
		mov rax, [rbp-4632]; load GL_DEPTH_STENCIL_ATTACHMENT
		pop rcx
		mov [rbp-4632], rcx; store GL_DEPTH_STENCIL_ATTACHMENT
		mov rax, [rbp-4632]; load GL_DEPTH_STENCIL_ATTACHMENT
		mov rax, 34024
		push rax
		mov rax, [rbp-4640]; load GL_MAX_RENDERBUFFER_SIZE
		pop rcx
		mov [rbp-4640], rcx; store GL_MAX_RENDERBUFFER_SIZE
		mov rax, [rbp-4640]; load GL_MAX_RENDERBUFFER_SIZE
		mov rax, 34041
		push rax
		mov rax, [rbp-4648]; load GL_DEPTH_STENCIL
		pop rcx
		mov [rbp-4648], rcx; store GL_DEPTH_STENCIL
		mov rax, [rbp-4648]; load GL_DEPTH_STENCIL
		mov rax, 34042
		push rax
		mov rax, [rbp-4656]; load GL_UNSIGNED_INT_24_8
		pop rcx
		mov [rbp-4656], rcx; store GL_UNSIGNED_INT_24_8
		mov rax, [rbp-4656]; load GL_UNSIGNED_INT_24_8
		mov rax, 35056
		push rax
		mov rax, [rbp-4664]; load GL_DEPTH24_STENCIL8
		pop rcx
		mov [rbp-4664], rcx; store GL_DEPTH24_STENCIL8
		mov rax, [rbp-4664]; load GL_DEPTH24_STENCIL8
		mov rax, 35057
		push rax
		mov rax, [rbp-4672]; load GL_TEXTURE_STENCIL_SIZE
		pop rcx
		mov [rbp-4672], rcx; store GL_TEXTURE_STENCIL_SIZE
		mov rax, [rbp-4672]; load GL_TEXTURE_STENCIL_SIZE
		mov rax, 35856
		push rax
		mov rax, [rbp-4680]; load GL_TEXTURE_RED_TYPE
		pop rcx
		mov [rbp-4680], rcx; store GL_TEXTURE_RED_TYPE
		mov rax, [rbp-4680]; load GL_TEXTURE_RED_TYPE
		mov rax, 35857
		push rax
		mov rax, [rbp-4688]; load GL_TEXTURE_GREEN_TYPE
		pop rcx
		mov [rbp-4688], rcx; store GL_TEXTURE_GREEN_TYPE
		mov rax, [rbp-4688]; load GL_TEXTURE_GREEN_TYPE
		mov rax, 35858
		push rax
		mov rax, [rbp-4696]; load GL_TEXTURE_BLUE_TYPE
		pop rcx
		mov [rbp-4696], rcx; store GL_TEXTURE_BLUE_TYPE
		mov rax, [rbp-4696]; load GL_TEXTURE_BLUE_TYPE
		mov rax, 35859
		push rax
		mov rax, [rbp-4704]; load GL_TEXTURE_ALPHA_TYPE
		pop rcx
		mov [rbp-4704], rcx; store GL_TEXTURE_ALPHA_TYPE
		mov rax, [rbp-4704]; load GL_TEXTURE_ALPHA_TYPE
		mov rax, 35862
		push rax
		mov rax, [rbp-4712]; load GL_TEXTURE_DEPTH_TYPE
		pop rcx
		mov [rbp-4712], rcx; store GL_TEXTURE_DEPTH_TYPE
		mov rax, [rbp-4712]; load GL_TEXTURE_DEPTH_TYPE
		mov rax, 35863
		push rax
		mov rax, [rbp-4720]; load GL_UNSIGNED_NORMALIZED
		pop rcx
		mov [rbp-4720], rcx; store GL_UNSIGNED_NORMALIZED
		mov rax, [rbp-4720]; load GL_UNSIGNED_NORMALIZED
		mov rax, 36006
		push rax
		mov rax, [rbp-4728]; load GL_FRAMEBUFFER_BINDING
		pop rcx
		mov [rbp-4728], rcx; store GL_FRAMEBUFFER_BINDING
		mov rax, [rbp-4728]; load GL_FRAMEBUFFER_BINDING
		mov rax, 36006
		push rax
		mov rax, [rbp-4736]; load GL_DRAW_FRAMEBUFFER_BINDING
		pop rcx
		mov [rbp-4736], rcx; store GL_DRAW_FRAMEBUFFER_BINDING
		mov rax, [rbp-4736]; load GL_DRAW_FRAMEBUFFER_BINDING
		mov rax, 36007
		push rax
		mov rax, [rbp-4744]; load GL_RENDERBUFFER_BINDING
		pop rcx
		mov [rbp-4744], rcx; store GL_RENDERBUFFER_BINDING
		mov rax, [rbp-4744]; load GL_RENDERBUFFER_BINDING
		mov rax, 36008
		push rax
		mov rax, [rbp-4752]; load GL_READ_FRAMEBUFFER
		pop rcx
		mov [rbp-4752], rcx; store GL_READ_FRAMEBUFFER
		mov rax, [rbp-4752]; load GL_READ_FRAMEBUFFER
		mov rax, 36009
		push rax
		mov rax, [rbp-4760]; load GL_DRAW_FRAMEBUFFER
		pop rcx
		mov [rbp-4760], rcx; store GL_DRAW_FRAMEBUFFER
		mov rax, [rbp-4760]; load GL_DRAW_FRAMEBUFFER
		mov rax, 36010
		push rax
		mov rax, [rbp-4768]; load GL_READ_FRAMEBUFFER_BINDING
		pop rcx
		mov [rbp-4768], rcx; store GL_READ_FRAMEBUFFER_BINDING
		mov rax, [rbp-4768]; load GL_READ_FRAMEBUFFER_BINDING
		mov rax, 36011
		push rax
		mov rax, [rbp-4776]; load GL_RENDERBUFFER_SAMPLES
		pop rcx
		mov [rbp-4776], rcx; store GL_RENDERBUFFER_SAMPLES
		mov rax, [rbp-4776]; load GL_RENDERBUFFER_SAMPLES
		mov rax, 36048
		push rax
		mov rax, [rbp-4784]; load GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE
		pop rcx
		mov [rbp-4784], rcx; store GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE
		mov rax, [rbp-4784]; load GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE
		mov rax, 36049
		push rax
		mov rax, [rbp-4792]; load GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME
		pop rcx
		mov [rbp-4792], rcx; store GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME
		mov rax, [rbp-4792]; load GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME
		mov rax, 36050
		push rax
		mov rax, [rbp-4800]; load GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL
		pop rcx
		mov [rbp-4800], rcx; store GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL
		mov rax, [rbp-4800]; load GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL
		mov rax, 36051
		push rax
		mov rax, [rbp-4808]; load GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE
		pop rcx
		mov [rbp-4808], rcx; store GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE
		mov rax, [rbp-4808]; load GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE
		mov rax, 36052
		push rax
		mov rax, [rbp-4816]; load GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER
		pop rcx
		mov [rbp-4816], rcx; store GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER
		mov rax, [rbp-4816]; load GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER
		mov rax, 36053
		push rax
		mov rax, [rbp-4824]; load GL_FRAMEBUFFER_COMPLETE
		pop rcx
		mov [rbp-4824], rcx; store GL_FRAMEBUFFER_COMPLETE
		mov rax, [rbp-4824]; load GL_FRAMEBUFFER_COMPLETE
		mov rax, 36054
		push rax
		mov rax, [rbp-4832]; load GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
		pop rcx
		mov [rbp-4832], rcx; store GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
		mov rax, [rbp-4832]; load GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT
		mov rax, 36055
		push rax
		mov rax, [rbp-4840]; load GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
		pop rcx
		mov [rbp-4840], rcx; store GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
		mov rax, [rbp-4840]; load GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT
		mov rax, 36059
		push rax
		mov rax, [rbp-4848]; load GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
		pop rcx
		mov [rbp-4848], rcx; store GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
		mov rax, [rbp-4848]; load GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
		mov rax, 36060
		push rax
		mov rax, [rbp-4856]; load GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
		pop rcx
		mov [rbp-4856], rcx; store GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
		mov rax, [rbp-4856]; load GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
		mov rax, 36061
		push rax
		mov rax, [rbp-4864]; load GL_FRAMEBUFFER_UNSUPPORTED
		pop rcx
		mov [rbp-4864], rcx; store GL_FRAMEBUFFER_UNSUPPORTED
		mov rax, [rbp-4864]; load GL_FRAMEBUFFER_UNSUPPORTED
		mov rax, 36063
		push rax
		mov rax, [rbp-4872]; load GL_MAX_COLOR_ATTACHMENTS
		pop rcx
		mov [rbp-4872], rcx; store GL_MAX_COLOR_ATTACHMENTS
		mov rax, [rbp-4872]; load GL_MAX_COLOR_ATTACHMENTS
		mov rax, 36064
		push rax
		mov rax, [rbp-4880]; load GL_COLOR_ATTACHMENT0
		pop rcx
		mov [rbp-4880], rcx; store GL_COLOR_ATTACHMENT0
		mov rax, [rbp-4880]; load GL_COLOR_ATTACHMENT0
		mov rax, 36065
		push rax
		mov rax, [rbp-4888]; load GL_COLOR_ATTACHMENT1
		pop rcx
		mov [rbp-4888], rcx; store GL_COLOR_ATTACHMENT1
		mov rax, [rbp-4888]; load GL_COLOR_ATTACHMENT1
		mov rax, 36066
		push rax
		mov rax, [rbp-4896]; load GL_COLOR_ATTACHMENT2
		pop rcx
		mov [rbp-4896], rcx; store GL_COLOR_ATTACHMENT2
		mov rax, [rbp-4896]; load GL_COLOR_ATTACHMENT2
		mov rax, 36067
		push rax
		mov rax, [rbp-4904]; load GL_COLOR_ATTACHMENT3
		pop rcx
		mov [rbp-4904], rcx; store GL_COLOR_ATTACHMENT3
		mov rax, [rbp-4904]; load GL_COLOR_ATTACHMENT3
		mov rax, 36068
		push rax
		mov rax, [rbp-4912]; load GL_COLOR_ATTACHMENT4
		pop rcx
		mov [rbp-4912], rcx; store GL_COLOR_ATTACHMENT4
		mov rax, [rbp-4912]; load GL_COLOR_ATTACHMENT4
		mov rax, 36069
		push rax
		mov rax, [rbp-4920]; load GL_COLOR_ATTACHMENT5
		pop rcx
		mov [rbp-4920], rcx; store GL_COLOR_ATTACHMENT5
		mov rax, [rbp-4920]; load GL_COLOR_ATTACHMENT5
		mov rax, 36070
		push rax
		mov rax, [rbp-4928]; load GL_COLOR_ATTACHMENT6
		pop rcx
		mov [rbp-4928], rcx; store GL_COLOR_ATTACHMENT6
		mov rax, [rbp-4928]; load GL_COLOR_ATTACHMENT6
		mov rax, 36071
		push rax
		mov rax, [rbp-4936]; load GL_COLOR_ATTACHMENT7
		pop rcx
		mov [rbp-4936], rcx; store GL_COLOR_ATTACHMENT7
		mov rax, [rbp-4936]; load GL_COLOR_ATTACHMENT7
		mov rax, 36072
		push rax
		mov rax, [rbp-4944]; load GL_COLOR_ATTACHMENT8
		pop rcx
		mov [rbp-4944], rcx; store GL_COLOR_ATTACHMENT8
		mov rax, [rbp-4944]; load GL_COLOR_ATTACHMENT8
		mov rax, 36073
		push rax
		mov rax, [rbp-4952]; load GL_COLOR_ATTACHMENT9
		pop rcx
		mov [rbp-4952], rcx; store GL_COLOR_ATTACHMENT9
		mov rax, [rbp-4952]; load GL_COLOR_ATTACHMENT9
		mov rax, 36074
		push rax
		mov rax, [rbp-4960]; load GL_COLOR_ATTACHMENT10
		pop rcx
		mov [rbp-4960], rcx; store GL_COLOR_ATTACHMENT10
		mov rax, [rbp-4960]; load GL_COLOR_ATTACHMENT10
		mov rax, 36075
		push rax
		mov rax, [rbp-4968]; load GL_COLOR_ATTACHMENT11
		pop rcx
		mov [rbp-4968], rcx; store GL_COLOR_ATTACHMENT11
		mov rax, [rbp-4968]; load GL_COLOR_ATTACHMENT11
		mov rax, 36076
		push rax
		mov rax, [rbp-4976]; load GL_COLOR_ATTACHMENT12
		pop rcx
		mov [rbp-4976], rcx; store GL_COLOR_ATTACHMENT12
		mov rax, [rbp-4976]; load GL_COLOR_ATTACHMENT12
		mov rax, 36077
		push rax
		mov rax, [rbp-4984]; load GL_COLOR_ATTACHMENT13
		pop rcx
		mov [rbp-4984], rcx; store GL_COLOR_ATTACHMENT13
		mov rax, [rbp-4984]; load GL_COLOR_ATTACHMENT13
		mov rax, 36078
		push rax
		mov rax, [rbp-4992]; load GL_COLOR_ATTACHMENT14
		pop rcx
		mov [rbp-4992], rcx; store GL_COLOR_ATTACHMENT14
		mov rax, [rbp-4992]; load GL_COLOR_ATTACHMENT14
		mov rax, 36079
		push rax
		mov rax, [rbp-5000]; load GL_COLOR_ATTACHMENT15
		pop rcx
		mov [rbp-5000], rcx; store GL_COLOR_ATTACHMENT15
		mov rax, [rbp-5000]; load GL_COLOR_ATTACHMENT15
		mov rax, 36080
		push rax
		mov rax, [rbp-5008]; load GL_COLOR_ATTACHMENT16
		pop rcx
		mov [rbp-5008], rcx; store GL_COLOR_ATTACHMENT16
		mov rax, [rbp-5008]; load GL_COLOR_ATTACHMENT16
		mov rax, 36081
		push rax
		mov rax, [rbp-5016]; load GL_COLOR_ATTACHMENT17
		pop rcx
		mov [rbp-5016], rcx; store GL_COLOR_ATTACHMENT17
		mov rax, [rbp-5016]; load GL_COLOR_ATTACHMENT17
		mov rax, 36082
		push rax
		mov rax, [rbp-5024]; load GL_COLOR_ATTACHMENT18
		pop rcx
		mov [rbp-5024], rcx; store GL_COLOR_ATTACHMENT18
		mov rax, [rbp-5024]; load GL_COLOR_ATTACHMENT18
		mov rax, 36083
		push rax
		mov rax, [rbp-5032]; load GL_COLOR_ATTACHMENT19
		pop rcx
		mov [rbp-5032], rcx; store GL_COLOR_ATTACHMENT19
		mov rax, [rbp-5032]; load GL_COLOR_ATTACHMENT19
		mov rax, 36084
		push rax
		mov rax, [rbp-5040]; load GL_COLOR_ATTACHMENT20
		pop rcx
		mov [rbp-5040], rcx; store GL_COLOR_ATTACHMENT20
		mov rax, [rbp-5040]; load GL_COLOR_ATTACHMENT20
		mov rax, 36085
		push rax
		mov rax, [rbp-5048]; load GL_COLOR_ATTACHMENT21
		pop rcx
		mov [rbp-5048], rcx; store GL_COLOR_ATTACHMENT21
		mov rax, [rbp-5048]; load GL_COLOR_ATTACHMENT21
		mov rax, 36086
		push rax
		mov rax, [rbp-5056]; load GL_COLOR_ATTACHMENT22
		pop rcx
		mov [rbp-5056], rcx; store GL_COLOR_ATTACHMENT22
		mov rax, [rbp-5056]; load GL_COLOR_ATTACHMENT22
		mov rax, 36087
		push rax
		mov rax, [rbp-5064]; load GL_COLOR_ATTACHMENT23
		pop rcx
		mov [rbp-5064], rcx; store GL_COLOR_ATTACHMENT23
		mov rax, [rbp-5064]; load GL_COLOR_ATTACHMENT23
		mov rax, 36088
		push rax
		mov rax, [rbp-5072]; load GL_COLOR_ATTACHMENT24
		pop rcx
		mov [rbp-5072], rcx; store GL_COLOR_ATTACHMENT24
		mov rax, [rbp-5072]; load GL_COLOR_ATTACHMENT24
		mov rax, 36089
		push rax
		mov rax, [rbp-5080]; load GL_COLOR_ATTACHMENT25
		pop rcx
		mov [rbp-5080], rcx; store GL_COLOR_ATTACHMENT25
		mov rax, [rbp-5080]; load GL_COLOR_ATTACHMENT25
		mov rax, 36090
		push rax
		mov rax, [rbp-5088]; load GL_COLOR_ATTACHMENT26
		pop rcx
		mov [rbp-5088], rcx; store GL_COLOR_ATTACHMENT26
		mov rax, [rbp-5088]; load GL_COLOR_ATTACHMENT26
		mov rax, 36091
		push rax
		mov rax, [rbp-5096]; load GL_COLOR_ATTACHMENT27
		pop rcx
		mov [rbp-5096], rcx; store GL_COLOR_ATTACHMENT27
		mov rax, [rbp-5096]; load GL_COLOR_ATTACHMENT27
		mov rax, 36092
		push rax
		mov rax, [rbp-5104]; load GL_COLOR_ATTACHMENT28
		pop rcx
		mov [rbp-5104], rcx; store GL_COLOR_ATTACHMENT28
		mov rax, [rbp-5104]; load GL_COLOR_ATTACHMENT28
		mov rax, 36093
		push rax
		mov rax, [rbp-5112]; load GL_COLOR_ATTACHMENT29
		pop rcx
		mov [rbp-5112], rcx; store GL_COLOR_ATTACHMENT29
		mov rax, [rbp-5112]; load GL_COLOR_ATTACHMENT29
		mov rax, 36094
		push rax
		mov rax, [rbp-5120]; load GL_COLOR_ATTACHMENT30
		pop rcx
		mov [rbp-5120], rcx; store GL_COLOR_ATTACHMENT30
		mov rax, [rbp-5120]; load GL_COLOR_ATTACHMENT30
		mov rax, 36095
		push rax
		mov rax, [rbp-5128]; load GL_COLOR_ATTACHMENT31
		pop rcx
		mov [rbp-5128], rcx; store GL_COLOR_ATTACHMENT31
		mov rax, [rbp-5128]; load GL_COLOR_ATTACHMENT31
		mov rax, 36096
		push rax
		mov rax, [rbp-5136]; load GL_DEPTH_ATTACHMENT
		pop rcx
		mov [rbp-5136], rcx; store GL_DEPTH_ATTACHMENT
		mov rax, [rbp-5136]; load GL_DEPTH_ATTACHMENT
		mov rax, 36128
		push rax
		mov rax, [rbp-5144]; load GL_STENCIL_ATTACHMENT
		pop rcx
		mov [rbp-5144], rcx; store GL_STENCIL_ATTACHMENT
		mov rax, [rbp-5144]; load GL_STENCIL_ATTACHMENT
		mov rax, 36160
		push rax
		mov rax, [rbp-5152]; load GL_FRAMEBUFFER
		pop rcx
		mov [rbp-5152], rcx; store GL_FRAMEBUFFER
		mov rax, [rbp-5152]; load GL_FRAMEBUFFER
		mov rax, 36161
		push rax
		mov rax, [rbp-5160]; load GL_RENDERBUFFER
		pop rcx
		mov [rbp-5160], rcx; store GL_RENDERBUFFER
		mov rax, [rbp-5160]; load GL_RENDERBUFFER
		mov rax, 36162
		push rax
		mov rax, [rbp-5168]; load GL_RENDERBUFFER_WIDTH
		pop rcx
		mov [rbp-5168], rcx; store GL_RENDERBUFFER_WIDTH
		mov rax, [rbp-5168]; load GL_RENDERBUFFER_WIDTH
		mov rax, 36163
		push rax
		mov rax, [rbp-5176]; load GL_RENDERBUFFER_HEIGHT
		pop rcx
		mov [rbp-5176], rcx; store GL_RENDERBUFFER_HEIGHT
		mov rax, [rbp-5176]; load GL_RENDERBUFFER_HEIGHT
		mov rax, 36164
		push rax
		mov rax, [rbp-5184]; load GL_RENDERBUFFER_INTERNAL_FORMAT
		pop rcx
		mov [rbp-5184], rcx; store GL_RENDERBUFFER_INTERNAL_FORMAT
		mov rax, [rbp-5184]; load GL_RENDERBUFFER_INTERNAL_FORMAT
		mov rax, 36166
		push rax
		mov rax, [rbp-5192]; load GL_STENCIL_INDEX1
		pop rcx
		mov [rbp-5192], rcx; store GL_STENCIL_INDEX1
		mov rax, [rbp-5192]; load GL_STENCIL_INDEX1
		mov rax, 36167
		push rax
		mov rax, [rbp-5200]; load GL_STENCIL_INDEX4
		pop rcx
		mov [rbp-5200], rcx; store GL_STENCIL_INDEX4
		mov rax, [rbp-5200]; load GL_STENCIL_INDEX4
		mov rax, 36168
		push rax
		mov rax, [rbp-5208]; load GL_STENCIL_INDEX8
		pop rcx
		mov [rbp-5208], rcx; store GL_STENCIL_INDEX8
		mov rax, [rbp-5208]; load GL_STENCIL_INDEX8
		mov rax, 36169
		push rax
		mov rax, [rbp-5216]; load GL_STENCIL_INDEX16
		pop rcx
		mov [rbp-5216], rcx; store GL_STENCIL_INDEX16
		mov rax, [rbp-5216]; load GL_STENCIL_INDEX16
		mov rax, 36176
		push rax
		mov rax, [rbp-5224]; load GL_RENDERBUFFER_RED_SIZE
		pop rcx
		mov [rbp-5224], rcx; store GL_RENDERBUFFER_RED_SIZE
		mov rax, [rbp-5224]; load GL_RENDERBUFFER_RED_SIZE
		mov rax, 36177
		push rax
		mov rax, [rbp-5232]; load GL_RENDERBUFFER_GREEN_SIZE
		pop rcx
		mov [rbp-5232], rcx; store GL_RENDERBUFFER_GREEN_SIZE
		mov rax, [rbp-5232]; load GL_RENDERBUFFER_GREEN_SIZE
		mov rax, 36178
		push rax
		mov rax, [rbp-5240]; load GL_RENDERBUFFER_BLUE_SIZE
		pop rcx
		mov [rbp-5240], rcx; store GL_RENDERBUFFER_BLUE_SIZE
		mov rax, [rbp-5240]; load GL_RENDERBUFFER_BLUE_SIZE
		mov rax, 36179
		push rax
		mov rax, [rbp-5248]; load GL_RENDERBUFFER_ALPHA_SIZE
		pop rcx
		mov [rbp-5248], rcx; store GL_RENDERBUFFER_ALPHA_SIZE
		mov rax, [rbp-5248]; load GL_RENDERBUFFER_ALPHA_SIZE
		mov rax, 36180
		push rax
		mov rax, [rbp-5256]; load GL_RENDERBUFFER_DEPTH_SIZE
		pop rcx
		mov [rbp-5256], rcx; store GL_RENDERBUFFER_DEPTH_SIZE
		mov rax, [rbp-5256]; load GL_RENDERBUFFER_DEPTH_SIZE
		mov rax, 36181
		push rax
		mov rax, [rbp-5264]; load GL_RENDERBUFFER_STENCIL_SIZE
		pop rcx
		mov [rbp-5264], rcx; store GL_RENDERBUFFER_STENCIL_SIZE
		mov rax, [rbp-5264]; load GL_RENDERBUFFER_STENCIL_SIZE
		mov rax, 36182
		push rax
		mov rax, [rbp-5272]; load GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
		pop rcx
		mov [rbp-5272], rcx; store GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
		mov rax, [rbp-5272]; load GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
		mov rax, 36183
		push rax
		mov rax, [rbp-5280]; load GL_MAX_SAMPLES
		pop rcx
		mov [rbp-5280], rcx; store GL_MAX_SAMPLES
		mov rax, [rbp-5280]; load GL_MAX_SAMPLES
		mov rax, 36281
		push rax
		mov rax, [rbp-5288]; load GL_FRAMEBUFFER_SRGB
		pop rcx
		mov [rbp-5288], rcx; store GL_FRAMEBUFFER_SRGB
		mov rax, [rbp-5288]; load GL_FRAMEBUFFER_SRGB
		mov rax, 5131
		push rax
		mov rax, [rbp-5296]; load GL_HALF_FLOAT
		pop rcx
		mov [rbp-5296], rcx; store GL_HALF_FLOAT
		mov rax, [rbp-5296]; load GL_HALF_FLOAT
		mov rax, 1
		push rax
		mov rax, [rbp-5304]; load GL_MAP_READ_BIT
		pop rcx
		mov [rbp-5304], rcx; store GL_MAP_READ_BIT
		mov rax, [rbp-5304]; load GL_MAP_READ_BIT
		mov rax, 2
		push rax
		mov rax, [rbp-5312]; load GL_MAP_WRITE_BIT
		pop rcx
		mov [rbp-5312], rcx; store GL_MAP_WRITE_BIT
		mov rax, [rbp-5312]; load GL_MAP_WRITE_BIT
		mov rax, 4
		push rax
		mov rax, [rbp-5320]; load GL_MAP_INVALIDATE_RANGE_BIT
		pop rcx
		mov [rbp-5320], rcx; store GL_MAP_INVALIDATE_RANGE_BIT
		mov rax, [rbp-5320]; load GL_MAP_INVALIDATE_RANGE_BIT
		mov rax, 8
		push rax
		mov rax, [rbp-5328]; load GL_MAP_INVALIDATE_BUFFER_BIT
		pop rcx
		mov [rbp-5328], rcx; store GL_MAP_INVALIDATE_BUFFER_BIT
		mov rax, [rbp-5328]; load GL_MAP_INVALIDATE_BUFFER_BIT
		mov rax, 16
		push rax
		mov rax, [rbp-5336]; load GL_MAP_FLUSH_EXPLICIT_BIT
		pop rcx
		mov [rbp-5336], rcx; store GL_MAP_FLUSH_EXPLICIT_BIT
		mov rax, [rbp-5336]; load GL_MAP_FLUSH_EXPLICIT_BIT
		mov rax, 32
		push rax
		mov rax, [rbp-5344]; load GL_MAP_UNSYNCHRONIZED_BIT
		pop rcx
		mov [rbp-5344], rcx; store GL_MAP_UNSYNCHRONIZED_BIT
		mov rax, [rbp-5344]; load GL_MAP_UNSYNCHRONIZED_BIT
		mov rax, 36283
		push rax
		mov rax, [rbp-5352]; load GL_COMPRESSED_RED_RGTC1
		pop rcx
		mov [rbp-5352], rcx; store GL_COMPRESSED_RED_RGTC1
		mov rax, [rbp-5352]; load GL_COMPRESSED_RED_RGTC1
		mov rax, 36284
		push rax
		mov rax, [rbp-5360]; load GL_COMPRESSED_SIGNED_RED_RGTC1
		pop rcx
		mov [rbp-5360], rcx; store GL_COMPRESSED_SIGNED_RED_RGTC1
		mov rax, [rbp-5360]; load GL_COMPRESSED_SIGNED_RED_RGTC1
		mov rax, 36285
		push rax
		mov rax, [rbp-5368]; load GL_COMPRESSED_RG_RGTC2
		pop rcx
		mov [rbp-5368], rcx; store GL_COMPRESSED_RG_RGTC2
		mov rax, [rbp-5368]; load GL_COMPRESSED_RG_RGTC2
		mov rax, 36286
		push rax
		mov rax, [rbp-5376]; load GL_COMPRESSED_SIGNED_RG_RGTC2
		pop rcx
		mov [rbp-5376], rcx; store GL_COMPRESSED_SIGNED_RG_RGTC2
		mov rax, [rbp-5376]; load GL_COMPRESSED_SIGNED_RG_RGTC2
		mov rax, 33319
		push rax
		mov rax, [rbp-5384]; load GL_RG
		pop rcx
		mov [rbp-5384], rcx; store GL_RG
		mov rax, [rbp-5384]; load GL_RG
		mov rax, 33320
		push rax
		mov rax, [rbp-5392]; load GL_RG_INTEGER
		pop rcx
		mov [rbp-5392], rcx; store GL_RG_INTEGER
		mov rax, [rbp-5392]; load GL_RG_INTEGER
		mov rax, 33321
		push rax
		mov rax, [rbp-5400]; load GL_R8
		pop rcx
		mov [rbp-5400], rcx; store GL_R8
		mov rax, [rbp-5400]; load GL_R8
		mov rax, 33322
		push rax
		mov rax, [rbp-5408]; load GL_R16
		pop rcx
		mov [rbp-5408], rcx; store GL_R16
		mov rax, [rbp-5408]; load GL_R16
		mov rax, 33323
		push rax
		mov rax, [rbp-5416]; load GL_RG8
		pop rcx
		mov [rbp-5416], rcx; store GL_RG8
		mov rax, [rbp-5416]; load GL_RG8
		mov rax, 33324
		push rax
		mov rax, [rbp-5424]; load GL_RG16
		pop rcx
		mov [rbp-5424], rcx; store GL_RG16
		mov rax, [rbp-5424]; load GL_RG16
		mov rax, 33325
		push rax
		mov rax, [rbp-5432]; load GL_R16F
		pop rcx
		mov [rbp-5432], rcx; store GL_R16F
		mov rax, [rbp-5432]; load GL_R16F
		mov rax, 33326
		push rax
		mov rax, [rbp-5440]; load GL_R32F
		pop rcx
		mov [rbp-5440], rcx; store GL_R32F
		mov rax, [rbp-5440]; load GL_R32F
		mov rax, 33327
		push rax
		mov rax, [rbp-5448]; load GL_RG16F
		pop rcx
		mov [rbp-5448], rcx; store GL_RG16F
		mov rax, [rbp-5448]; load GL_RG16F
		mov rax, 33328
		push rax
		mov rax, [rbp-5456]; load GL_RG32F
		pop rcx
		mov [rbp-5456], rcx; store GL_RG32F
		mov rax, [rbp-5456]; load GL_RG32F
		mov rax, 33329
		push rax
		mov rax, [rbp-5464]; load GL_R8I
		pop rcx
		mov [rbp-5464], rcx; store GL_R8I
		mov rax, [rbp-5464]; load GL_R8I
		mov rax, 33330
		push rax
		mov rax, [rbp-5472]; load GL_R8UI
		pop rcx
		mov [rbp-5472], rcx; store GL_R8UI
		mov rax, [rbp-5472]; load GL_R8UI
		mov rax, 33331
		push rax
		mov rax, [rbp-5480]; load GL_R16I
		pop rcx
		mov [rbp-5480], rcx; store GL_R16I
		mov rax, [rbp-5480]; load GL_R16I
		mov rax, 33332
		push rax
		mov rax, [rbp-5488]; load GL_R16UI
		pop rcx
		mov [rbp-5488], rcx; store GL_R16UI
		mov rax, [rbp-5488]; load GL_R16UI
		mov rax, 33333
		push rax
		mov rax, [rbp-5496]; load GL_R32I
		pop rcx
		mov [rbp-5496], rcx; store GL_R32I
		mov rax, [rbp-5496]; load GL_R32I
		mov rax, 33334
		push rax
		mov rax, [rbp-5504]; load GL_R32UI
		pop rcx
		mov [rbp-5504], rcx; store GL_R32UI
		mov rax, [rbp-5504]; load GL_R32UI
		mov rax, 33335
		push rax
		mov rax, [rbp-5512]; load GL_RG8I
		pop rcx
		mov [rbp-5512], rcx; store GL_RG8I
		mov rax, [rbp-5512]; load GL_RG8I
		mov rax, 33336
		push rax
		mov rax, [rbp-5520]; load GL_RG8UI
		pop rcx
		mov [rbp-5520], rcx; store GL_RG8UI
		mov rax, [rbp-5520]; load GL_RG8UI
		mov rax, 33337
		push rax
		mov rax, [rbp-5528]; load GL_RG16I
		pop rcx
		mov [rbp-5528], rcx; store GL_RG16I
		mov rax, [rbp-5528]; load GL_RG16I
		mov rax, 33338
		push rax
		mov rax, [rbp-5536]; load GL_RG16UI
		pop rcx
		mov [rbp-5536], rcx; store GL_RG16UI
		mov rax, [rbp-5536]; load GL_RG16UI
		mov rax, 33339
		push rax
		mov rax, [rbp-5544]; load GL_RG32I
		pop rcx
		mov [rbp-5544], rcx; store GL_RG32I
		mov rax, [rbp-5544]; load GL_RG32I
		mov rax, 33340
		push rax
		mov rax, [rbp-5552]; load GL_RG32UI
		pop rcx
		mov [rbp-5552], rcx; store GL_RG32UI
		mov rax, [rbp-5552]; load GL_RG32UI
		mov rax, 34229
		push rax
		mov rax, [rbp-5560]; load GL_VERTEX_ARRAY_BINDING
		pop rcx
		mov [rbp-5560], rcx; store GL_VERTEX_ARRAY_BINDING
		mov rax, [rbp-5560]; load GL_VERTEX_ARRAY_BINDING
		mov rax, 35683
		push rax
		mov rax, [rbp-5568]; load GL_SAMPLER_2D_RECT
		pop rcx
		mov [rbp-5568], rcx; store GL_SAMPLER_2D_RECT
		mov rax, [rbp-5568]; load GL_SAMPLER_2D_RECT
		mov rax, 35684
		push rax
		mov rax, [rbp-5576]; load GL_SAMPLER_2D_RECT_SHADOW
		pop rcx
		mov [rbp-5576], rcx; store GL_SAMPLER_2D_RECT_SHADOW
		mov rax, [rbp-5576]; load GL_SAMPLER_2D_RECT_SHADOW
		mov rax, 36290
		push rax
		mov rax, [rbp-5584]; load GL_SAMPLER_BUFFER
		pop rcx
		mov [rbp-5584], rcx; store GL_SAMPLER_BUFFER
		mov rax, [rbp-5584]; load GL_SAMPLER_BUFFER
		mov rax, 36301
		push rax
		mov rax, [rbp-5592]; load GL_INT_SAMPLER_2D_RECT
		pop rcx
		mov [rbp-5592], rcx; store GL_INT_SAMPLER_2D_RECT
		mov rax, [rbp-5592]; load GL_INT_SAMPLER_2D_RECT
		mov rax, 36304
		push rax
		mov rax, [rbp-5600]; load GL_INT_SAMPLER_BUFFER
		pop rcx
		mov [rbp-5600], rcx; store GL_INT_SAMPLER_BUFFER
		mov rax, [rbp-5600]; load GL_INT_SAMPLER_BUFFER
		mov rax, 36309
		push rax
		mov rax, [rbp-5608]; load GL_UNSIGNED_INT_SAMPLER_2D_RECT
		pop rcx
		mov [rbp-5608], rcx; store GL_UNSIGNED_INT_SAMPLER_2D_RECT
		mov rax, [rbp-5608]; load GL_UNSIGNED_INT_SAMPLER_2D_RECT
		mov rax, 36312
		push rax
		mov rax, [rbp-5616]; load GL_UNSIGNED_INT_SAMPLER_BUFFER
		pop rcx
		mov [rbp-5616], rcx; store GL_UNSIGNED_INT_SAMPLER_BUFFER
		mov rax, [rbp-5616]; load GL_UNSIGNED_INT_SAMPLER_BUFFER
		mov rax, 35882
		push rax
		mov rax, [rbp-5624]; load GL_TEXTURE_BUFFER
		pop rcx
		mov [rbp-5624], rcx; store GL_TEXTURE_BUFFER
		mov rax, [rbp-5624]; load GL_TEXTURE_BUFFER
		mov rax, 35883
		push rax
		mov rax, [rbp-5632]; load GL_MAX_TEXTURE_BUFFER_SIZE
		pop rcx
		mov [rbp-5632], rcx; store GL_MAX_TEXTURE_BUFFER_SIZE
		mov rax, [rbp-5632]; load GL_MAX_TEXTURE_BUFFER_SIZE
		mov rax, 35884
		push rax
		mov rax, [rbp-5640]; load GL_TEXTURE_BINDING_BUFFER
		pop rcx
		mov [rbp-5640], rcx; store GL_TEXTURE_BINDING_BUFFER
		mov rax, [rbp-5640]; load GL_TEXTURE_BINDING_BUFFER
		mov rax, 35885
		push rax
		mov rax, [rbp-5648]; load GL_TEXTURE_BUFFER_DATA_STORE_BINDING
		pop rcx
		mov [rbp-5648], rcx; store GL_TEXTURE_BUFFER_DATA_STORE_BINDING
		mov rax, [rbp-5648]; load GL_TEXTURE_BUFFER_DATA_STORE_BINDING
		mov rax, 34037
		push rax
		mov rax, [rbp-5656]; load GL_TEXTURE_RECTANGLE
		pop rcx
		mov [rbp-5656], rcx; store GL_TEXTURE_RECTANGLE
		mov rax, [rbp-5656]; load GL_TEXTURE_RECTANGLE
		mov rax, 34038
		push rax
		mov rax, [rbp-5664]; load GL_TEXTURE_BINDING_RECTANGLE
		pop rcx
		mov [rbp-5664], rcx; store GL_TEXTURE_BINDING_RECTANGLE
		mov rax, [rbp-5664]; load GL_TEXTURE_BINDING_RECTANGLE
		mov rax, 34039
		push rax
		mov rax, [rbp-5672]; load GL_PROXY_TEXTURE_RECTANGLE
		pop rcx
		mov [rbp-5672], rcx; store GL_PROXY_TEXTURE_RECTANGLE
		mov rax, [rbp-5672]; load GL_PROXY_TEXTURE_RECTANGLE
		mov rax, 34040
		push rax
		mov rax, [rbp-5680]; load GL_MAX_RECTANGLE_TEXTURE_SIZE
		pop rcx
		mov [rbp-5680], rcx; store GL_MAX_RECTANGLE_TEXTURE_SIZE
		mov rax, [rbp-5680]; load GL_MAX_RECTANGLE_TEXTURE_SIZE
		mov rax, 36756
		push rax
		mov rax, [rbp-5688]; load GL_R8_SNORM
		pop rcx
		mov [rbp-5688], rcx; store GL_R8_SNORM
		mov rax, [rbp-5688]; load GL_R8_SNORM
		mov rax, 36757
		push rax
		mov rax, [rbp-5696]; load GL_RG8_SNORM
		pop rcx
		mov [rbp-5696], rcx; store GL_RG8_SNORM
		mov rax, [rbp-5696]; load GL_RG8_SNORM
		mov rax, 36758
		push rax
		mov rax, [rbp-5704]; load GL_RGB8_SNORM
		pop rcx
		mov [rbp-5704], rcx; store GL_RGB8_SNORM
		mov rax, [rbp-5704]; load GL_RGB8_SNORM
		mov rax, 36759
		push rax
		mov rax, [rbp-5712]; load GL_RGBA8_SNORM
		pop rcx
		mov [rbp-5712], rcx; store GL_RGBA8_SNORM
		mov rax, [rbp-5712]; load GL_RGBA8_SNORM
		mov rax, 36760
		push rax
		mov rax, [rbp-5720]; load GL_R16_SNORM
		pop rcx
		mov [rbp-5720], rcx; store GL_R16_SNORM
		mov rax, [rbp-5720]; load GL_R16_SNORM
		mov rax, 36761
		push rax
		mov rax, [rbp-5728]; load GL_RG16_SNORM
		pop rcx
		mov [rbp-5728], rcx; store GL_RG16_SNORM
		mov rax, [rbp-5728]; load GL_RG16_SNORM
		mov rax, 36762
		push rax
		mov rax, [rbp-5736]; load GL_RGB16_SNORM
		pop rcx
		mov [rbp-5736], rcx; store GL_RGB16_SNORM
		mov rax, [rbp-5736]; load GL_RGB16_SNORM
		mov rax, 36763
		push rax
		mov rax, [rbp-5744]; load GL_RGBA16_SNORM
		pop rcx
		mov [rbp-5744], rcx; store GL_RGBA16_SNORM
		mov rax, [rbp-5744]; load GL_RGBA16_SNORM
		mov rax, 36764
		push rax
		mov rax, [rbp-5752]; load GL_SIGNED_NORMALIZED
		pop rcx
		mov [rbp-5752], rcx; store GL_SIGNED_NORMALIZED
		mov rax, [rbp-5752]; load GL_SIGNED_NORMALIZED
		mov rax, 36765
		push rax
		mov rax, [rbp-5760]; load GL_PRIMITIVE_RESTART
		pop rcx
		mov [rbp-5760], rcx; store GL_PRIMITIVE_RESTART
		mov rax, [rbp-5760]; load GL_PRIMITIVE_RESTART
		mov rax, 36766
		push rax
		mov rax, [rbp-5768]; load GL_PRIMITIVE_RESTART_INDEX
		pop rcx
		mov [rbp-5768], rcx; store GL_PRIMITIVE_RESTART_INDEX
		mov rax, [rbp-5768]; load GL_PRIMITIVE_RESTART_INDEX
		mov rax, 36662
		push rax
		mov rax, [rbp-5776]; load GL_COPY_READ_BUFFER
		pop rcx
		mov [rbp-5776], rcx; store GL_COPY_READ_BUFFER
		mov rax, [rbp-5776]; load GL_COPY_READ_BUFFER
		mov rax, 36663
		push rax
		mov rax, [rbp-5784]; load GL_COPY_WRITE_BUFFER
		pop rcx
		mov [rbp-5784], rcx; store GL_COPY_WRITE_BUFFER
		mov rax, [rbp-5784]; load GL_COPY_WRITE_BUFFER
		mov rax, 35345
		push rax
		mov rax, [rbp-5792]; load GL_UNIFORM_BUFFER
		pop rcx
		mov [rbp-5792], rcx; store GL_UNIFORM_BUFFER
		mov rax, [rbp-5792]; load GL_UNIFORM_BUFFER
		mov rax, 35368
		push rax
		mov rax, [rbp-5800]; load GL_UNIFORM_BUFFER_BINDING
		pop rcx
		mov [rbp-5800], rcx; store GL_UNIFORM_BUFFER_BINDING
		mov rax, [rbp-5800]; load GL_UNIFORM_BUFFER_BINDING
		mov rax, 35369
		push rax
		mov rax, [rbp-5808]; load GL_UNIFORM_BUFFER_START
		pop rcx
		mov [rbp-5808], rcx; store GL_UNIFORM_BUFFER_START
		mov rax, [rbp-5808]; load GL_UNIFORM_BUFFER_START
		mov rax, 35370
		push rax
		mov rax, [rbp-5816]; load GL_UNIFORM_BUFFER_SIZE
		pop rcx
		mov [rbp-5816], rcx; store GL_UNIFORM_BUFFER_SIZE
		mov rax, [rbp-5816]; load GL_UNIFORM_BUFFER_SIZE
		mov rax, 35371
		push rax
		mov rax, [rbp-5824]; load GL_MAX_VERTEX_UNIFORM_BLOCKS
		pop rcx
		mov [rbp-5824], rcx; store GL_MAX_VERTEX_UNIFORM_BLOCKS
		mov rax, [rbp-5824]; load GL_MAX_VERTEX_UNIFORM_BLOCKS
		mov rax, 35372
		push rax
		mov rax, [rbp-5832]; load GL_MAX_GEOMETRY_UNIFORM_BLOCKS
		pop rcx
		mov [rbp-5832], rcx; store GL_MAX_GEOMETRY_UNIFORM_BLOCKS
		mov rax, [rbp-5832]; load GL_MAX_GEOMETRY_UNIFORM_BLOCKS
		mov rax, 35373
		push rax
		mov rax, [rbp-5840]; load GL_MAX_FRAGMENT_UNIFORM_BLOCKS
		pop rcx
		mov [rbp-5840], rcx; store GL_MAX_FRAGMENT_UNIFORM_BLOCKS
		mov rax, [rbp-5840]; load GL_MAX_FRAGMENT_UNIFORM_BLOCKS
		mov rax, 35374
		push rax
		mov rax, [rbp-5848]; load GL_MAX_COMBINED_UNIFORM_BLOCKS
		pop rcx
		mov [rbp-5848], rcx; store GL_MAX_COMBINED_UNIFORM_BLOCKS
		mov rax, [rbp-5848]; load GL_MAX_COMBINED_UNIFORM_BLOCKS
		mov rax, 35375
		push rax
		mov rax, [rbp-5856]; load GL_MAX_UNIFORM_BUFFER_BINDINGS
		pop rcx
		mov [rbp-5856], rcx; store GL_MAX_UNIFORM_BUFFER_BINDINGS
		mov rax, [rbp-5856]; load GL_MAX_UNIFORM_BUFFER_BINDINGS
		mov rax, 35376
		push rax
		mov rax, [rbp-5864]; load GL_MAX_UNIFORM_BLOCK_SIZE
		pop rcx
		mov [rbp-5864], rcx; store GL_MAX_UNIFORM_BLOCK_SIZE
		mov rax, [rbp-5864]; load GL_MAX_UNIFORM_BLOCK_SIZE
		mov rax, 35377
		push rax
		mov rax, [rbp-5872]; load GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-5872], rcx; store GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS
		mov rax, [rbp-5872]; load GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS
		mov rax, 35378
		push rax
		mov rax, [rbp-5880]; load GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-5880], rcx; store GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
		mov rax, [rbp-5880]; load GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS
		mov rax, 35379
		push rax
		mov rax, [rbp-5888]; load GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-5888], rcx; store GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS
		mov rax, [rbp-5888]; load GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS
		mov rax, 35380
		push rax
		mov rax, [rbp-5896]; load GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT
		pop rcx
		mov [rbp-5896], rcx; store GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT
		mov rax, [rbp-5896]; load GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT
		mov rax, 35381
		push rax
		mov rax, [rbp-5904]; load GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH
		pop rcx
		mov [rbp-5904], rcx; store GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH
		mov rax, [rbp-5904]; load GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH
		mov rax, 35382
		push rax
		mov rax, [rbp-5912]; load GL_ACTIVE_UNIFORM_BLOCKS
		pop rcx
		mov [rbp-5912], rcx; store GL_ACTIVE_UNIFORM_BLOCKS
		mov rax, [rbp-5912]; load GL_ACTIVE_UNIFORM_BLOCKS
		mov rax, 35383
		push rax
		mov rax, [rbp-5920]; load GL_UNIFORM_TYPE
		pop rcx
		mov [rbp-5920], rcx; store GL_UNIFORM_TYPE
		mov rax, [rbp-5920]; load GL_UNIFORM_TYPE
		mov rax, 35384
		push rax
		mov rax, [rbp-5928]; load GL_UNIFORM_SIZE
		pop rcx
		mov [rbp-5928], rcx; store GL_UNIFORM_SIZE
		mov rax, [rbp-5928]; load GL_UNIFORM_SIZE
		mov rax, 35385
		push rax
		mov rax, [rbp-5936]; load GL_UNIFORM_NAME_LENGTH
		pop rcx
		mov [rbp-5936], rcx; store GL_UNIFORM_NAME_LENGTH
		mov rax, [rbp-5936]; load GL_UNIFORM_NAME_LENGTH
		mov rax, 35386
		push rax
		mov rax, [rbp-5944]; load GL_UNIFORM_BLOCK_INDEX
		pop rcx
		mov [rbp-5944], rcx; store GL_UNIFORM_BLOCK_INDEX
		mov rax, [rbp-5944]; load GL_UNIFORM_BLOCK_INDEX
		mov rax, 35387
		push rax
		mov rax, [rbp-5952]; load GL_UNIFORM_OFFSET
		pop rcx
		mov [rbp-5952], rcx; store GL_UNIFORM_OFFSET
		mov rax, [rbp-5952]; load GL_UNIFORM_OFFSET
		mov rax, 35388
		push rax
		mov rax, [rbp-5960]; load GL_UNIFORM_ARRAY_STRIDE
		pop rcx
		mov [rbp-5960], rcx; store GL_UNIFORM_ARRAY_STRIDE
		mov rax, [rbp-5960]; load GL_UNIFORM_ARRAY_STRIDE
		mov rax, 35389
		push rax
		mov rax, [rbp-5968]; load GL_UNIFORM_MATRIX_STRIDE
		pop rcx
		mov [rbp-5968], rcx; store GL_UNIFORM_MATRIX_STRIDE
		mov rax, [rbp-5968]; load GL_UNIFORM_MATRIX_STRIDE
		mov rax, 35390
		push rax
		mov rax, [rbp-5976]; load GL_UNIFORM_IS_ROW_MAJOR
		pop rcx
		mov [rbp-5976], rcx; store GL_UNIFORM_IS_ROW_MAJOR
		mov rax, [rbp-5976]; load GL_UNIFORM_IS_ROW_MAJOR
		mov rax, 35391
		push rax
		mov rax, [rbp-5984]; load GL_UNIFORM_BLOCK_BINDING
		pop rcx
		mov [rbp-5984], rcx; store GL_UNIFORM_BLOCK_BINDING
		mov rax, [rbp-5984]; load GL_UNIFORM_BLOCK_BINDING
		mov rax, 35392
		push rax
		mov rax, [rbp-5992]; load GL_UNIFORM_BLOCK_DATA_SIZE
		pop rcx
		mov [rbp-5992], rcx; store GL_UNIFORM_BLOCK_DATA_SIZE
		mov rax, [rbp-5992]; load GL_UNIFORM_BLOCK_DATA_SIZE
		mov rax, 35393
		push rax
		mov rax, [rbp-6000]; load GL_UNIFORM_BLOCK_NAME_LENGTH
		pop rcx
		mov [rbp-6000], rcx; store GL_UNIFORM_BLOCK_NAME_LENGTH
		mov rax, [rbp-6000]; load GL_UNIFORM_BLOCK_NAME_LENGTH
		mov rax, 35394
		push rax
		mov rax, [rbp-6008]; load GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS
		pop rcx
		mov [rbp-6008], rcx; store GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS
		mov rax, [rbp-6008]; load GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS
		mov rax, 35395
		push rax
		mov rax, [rbp-6016]; load GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES
		pop rcx
		mov [rbp-6016], rcx; store GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES
		mov rax, [rbp-6016]; load GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES
		mov rax, 35396
		push rax
		mov rax, [rbp-6024]; load GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER
		pop rcx
		mov [rbp-6024], rcx; store GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER
		mov rax, [rbp-6024]; load GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER
		mov rax, 35397
		push rax
		mov rax, [rbp-6032]; load GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER
		pop rcx
		mov [rbp-6032], rcx; store GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER
		mov rax, [rbp-6032]; load GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER
		mov rax, 35398
		push rax
		mov rax, [rbp-6040]; load GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER
		pop rcx
		mov [rbp-6040], rcx; store GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER
		mov rax, [rbp-6040]; load GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER
		mov rax, -1
		push rax
		mov rax, [rbp-6048]; load GL_INVALID_INDEX
		pop rcx
		mov [rbp-6048], rcx; store GL_INVALID_INDEX
		mov rax, [rbp-6048]; load GL_INVALID_INDEX
		mov rax, 1
		push rax
		mov rax, [rbp-6056]; load GL_CONTEXT_CORE_PROFILE_BIT
		pop rcx
		mov [rbp-6056], rcx; store GL_CONTEXT_CORE_PROFILE_BIT
		mov rax, [rbp-6056]; load GL_CONTEXT_CORE_PROFILE_BIT
		mov rax, 2
		push rax
		mov rax, [rbp-6064]; load GL_CONTEXT_COMPATIBILITY_PROFILE_BIT
		pop rcx
		mov [rbp-6064], rcx; store GL_CONTEXT_COMPATIBILITY_PROFILE_BIT
		mov rax, [rbp-6064]; load GL_CONTEXT_COMPATIBILITY_PROFILE_BIT
		mov rax, 10
		push rax
		mov rax, [rbp-6072]; load GL_LINES_ADJACENCY
		pop rcx
		mov [rbp-6072], rcx; store GL_LINES_ADJACENCY
		mov rax, [rbp-6072]; load GL_LINES_ADJACENCY
		mov rax, 11
		push rax
		mov rax, [rbp-6080]; load GL_LINE_STRIP_ADJACENCY
		pop rcx
		mov [rbp-6080], rcx; store GL_LINE_STRIP_ADJACENCY
		mov rax, [rbp-6080]; load GL_LINE_STRIP_ADJACENCY
		mov rax, 12
		push rax
		mov rax, [rbp-6088]; load GL_TRIANGLES_ADJACENCY
		pop rcx
		mov [rbp-6088], rcx; store GL_TRIANGLES_ADJACENCY
		mov rax, [rbp-6088]; load GL_TRIANGLES_ADJACENCY
		mov rax, 13
		push rax
		mov rax, [rbp-6096]; load GL_TRIANGLE_STRIP_ADJACENCY
		pop rcx
		mov [rbp-6096], rcx; store GL_TRIANGLE_STRIP_ADJACENCY
		mov rax, [rbp-6096]; load GL_TRIANGLE_STRIP_ADJACENCY
		mov rax, 34370
		push rax
		mov rax, [rbp-6104]; load GL_PROGRAM_POINT_SIZE
		pop rcx
		mov [rbp-6104], rcx; store GL_PROGRAM_POINT_SIZE
		mov rax, [rbp-6104]; load GL_PROGRAM_POINT_SIZE
		mov rax, 35881
		push rax
		mov rax, [rbp-6112]; load GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
		pop rcx
		mov [rbp-6112], rcx; store GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
		mov rax, [rbp-6112]; load GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS
		mov rax, 36263
		push rax
		mov rax, [rbp-6120]; load GL_FRAMEBUFFER_ATTACHMENT_LAYERED
		pop rcx
		mov [rbp-6120], rcx; store GL_FRAMEBUFFER_ATTACHMENT_LAYERED
		mov rax, [rbp-6120]; load GL_FRAMEBUFFER_ATTACHMENT_LAYERED
		mov rax, 36264
		push rax
		mov rax, [rbp-6128]; load GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
		pop rcx
		mov [rbp-6128], rcx; store GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
		mov rax, [rbp-6128]; load GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS
		mov rax, 36313
		push rax
		mov rax, [rbp-6136]; load GL_GEOMETRY_SHADER
		pop rcx
		mov [rbp-6136], rcx; store GL_GEOMETRY_SHADER
		mov rax, [rbp-6136]; load GL_GEOMETRY_SHADER
		mov rax, 35094
		push rax
		mov rax, [rbp-6144]; load GL_GEOMETRY_VERTICES_OUT
		pop rcx
		mov [rbp-6144], rcx; store GL_GEOMETRY_VERTICES_OUT
		mov rax, [rbp-6144]; load GL_GEOMETRY_VERTICES_OUT
		mov rax, 35095
		push rax
		mov rax, [rbp-6152]; load GL_GEOMETRY_INPUT_TYPE
		pop rcx
		mov [rbp-6152], rcx; store GL_GEOMETRY_INPUT_TYPE
		mov rax, [rbp-6152]; load GL_GEOMETRY_INPUT_TYPE
		mov rax, 35096
		push rax
		mov rax, [rbp-6160]; load GL_GEOMETRY_OUTPUT_TYPE
		pop rcx
		mov [rbp-6160], rcx; store GL_GEOMETRY_OUTPUT_TYPE
		mov rax, [rbp-6160]; load GL_GEOMETRY_OUTPUT_TYPE
		mov rax, 36319
		push rax
		mov rax, [rbp-6168]; load GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-6168], rcx; store GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
		mov rax, [rbp-6168]; load GL_MAX_GEOMETRY_UNIFORM_COMPONENTS
		mov rax, 36320
		push rax
		mov rax, [rbp-6176]; load GL_MAX_GEOMETRY_OUTPUT_VERTICES
		pop rcx
		mov [rbp-6176], rcx; store GL_MAX_GEOMETRY_OUTPUT_VERTICES
		mov rax, [rbp-6176]; load GL_MAX_GEOMETRY_OUTPUT_VERTICES
		mov rax, 36321
		push rax
		mov rax, [rbp-6184]; load GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS
		pop rcx
		mov [rbp-6184], rcx; store GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS
		mov rax, [rbp-6184]; load GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS
		mov rax, 37154
		push rax
		mov rax, [rbp-6192]; load GL_MAX_VERTEX_OUTPUT_COMPONENTS
		pop rcx
		mov [rbp-6192], rcx; store GL_MAX_VERTEX_OUTPUT_COMPONENTS
		mov rax, [rbp-6192]; load GL_MAX_VERTEX_OUTPUT_COMPONENTS
		mov rax, 37155
		push rax
		mov rax, [rbp-6200]; load GL_MAX_GEOMETRY_INPUT_COMPONENTS
		pop rcx
		mov [rbp-6200], rcx; store GL_MAX_GEOMETRY_INPUT_COMPONENTS
		mov rax, [rbp-6200]; load GL_MAX_GEOMETRY_INPUT_COMPONENTS
		mov rax, 37156
		push rax
		mov rax, [rbp-6208]; load GL_MAX_GEOMETRY_OUTPUT_COMPONENTS
		pop rcx
		mov [rbp-6208], rcx; store GL_MAX_GEOMETRY_OUTPUT_COMPONENTS
		mov rax, [rbp-6208]; load GL_MAX_GEOMETRY_OUTPUT_COMPONENTS
		mov rax, 37157
		push rax
		mov rax, [rbp-6216]; load GL_MAX_FRAGMENT_INPUT_COMPONENTS
		pop rcx
		mov [rbp-6216], rcx; store GL_MAX_FRAGMENT_INPUT_COMPONENTS
		mov rax, [rbp-6216]; load GL_MAX_FRAGMENT_INPUT_COMPONENTS
		mov rax, 37158
		push rax
		mov rax, [rbp-6224]; load GL_CONTEXT_PROFILE_MASK
		pop rcx
		mov [rbp-6224], rcx; store GL_CONTEXT_PROFILE_MASK
		mov rax, [rbp-6224]; load GL_CONTEXT_PROFILE_MASK
		mov rax, 34383
		push rax
		mov rax, [rbp-6232]; load GL_DEPTH_CLAMP
		pop rcx
		mov [rbp-6232], rcx; store GL_DEPTH_CLAMP
		mov rax, [rbp-6232]; load GL_DEPTH_CLAMP
		mov rax, 36428
		push rax
		mov rax, [rbp-6240]; load GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION
		pop rcx
		mov [rbp-6240], rcx; store GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION
		mov rax, [rbp-6240]; load GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION
		mov rax, 36429
		push rax
		mov rax, [rbp-6248]; load GL_FIRST_VERTEX_CONVENTION
		pop rcx
		mov [rbp-6248], rcx; store GL_FIRST_VERTEX_CONVENTION
		mov rax, [rbp-6248]; load GL_FIRST_VERTEX_CONVENTION
		mov rax, 36430
		push rax
		mov rax, [rbp-6256]; load GL_LAST_VERTEX_CONVENTION
		pop rcx
		mov [rbp-6256], rcx; store GL_LAST_VERTEX_CONVENTION
		mov rax, [rbp-6256]; load GL_LAST_VERTEX_CONVENTION
		mov rax, 36431
		push rax
		mov rax, [rbp-6264]; load GL_PROVOKING_VERTEX
		pop rcx
		mov [rbp-6264], rcx; store GL_PROVOKING_VERTEX
		mov rax, [rbp-6264]; load GL_PROVOKING_VERTEX
		mov rax, 34895
		push rax
		mov rax, [rbp-6272]; load GL_TEXTURE_CUBE_MAP_SEAMLESS
		pop rcx
		mov [rbp-6272], rcx; store GL_TEXTURE_CUBE_MAP_SEAMLESS
		mov rax, [rbp-6272]; load GL_TEXTURE_CUBE_MAP_SEAMLESS
		mov rax, 37137
		push rax
		mov rax, [rbp-6280]; load GL_MAX_SERVER_WAIT_TIMEOUT
		pop rcx
		mov [rbp-6280], rcx; store GL_MAX_SERVER_WAIT_TIMEOUT
		mov rax, [rbp-6280]; load GL_MAX_SERVER_WAIT_TIMEOUT
		mov rax, 37138
		push rax
		mov rax, [rbp-6288]; load GL_OBJECT_TYPE
		pop rcx
		mov [rbp-6288], rcx; store GL_OBJECT_TYPE
		mov rax, [rbp-6288]; load GL_OBJECT_TYPE
		mov rax, 37139
		push rax
		mov rax, [rbp-6296]; load GL_SYNC_CONDITION
		pop rcx
		mov [rbp-6296], rcx; store GL_SYNC_CONDITION
		mov rax, [rbp-6296]; load GL_SYNC_CONDITION
		mov rax, 37140
		push rax
		mov rax, [rbp-6304]; load GL_SYNC_STATUS
		pop rcx
		mov [rbp-6304], rcx; store GL_SYNC_STATUS
		mov rax, [rbp-6304]; load GL_SYNC_STATUS
		mov rax, 37141
		push rax
		mov rax, [rbp-6312]; load GL_SYNC_FLAGS
		pop rcx
		mov [rbp-6312], rcx; store GL_SYNC_FLAGS
		mov rax, [rbp-6312]; load GL_SYNC_FLAGS
		mov rax, 37142
		push rax
		mov rax, [rbp-6320]; load GL_SYNC_FENCE
		pop rcx
		mov [rbp-6320], rcx; store GL_SYNC_FENCE
		mov rax, [rbp-6320]; load GL_SYNC_FENCE
		mov rax, 37143
		push rax
		mov rax, [rbp-6328]; load GL_SYNC_GPU_COMMANDS_COMPLETE
		pop rcx
		mov [rbp-6328], rcx; store GL_SYNC_GPU_COMMANDS_COMPLETE
		mov rax, [rbp-6328]; load GL_SYNC_GPU_COMMANDS_COMPLETE
		mov rax, 37144
		push rax
		mov rax, [rbp-6336]; load GL_UNSIGNALED
		pop rcx
		mov [rbp-6336], rcx; store GL_UNSIGNALED
		mov rax, [rbp-6336]; load GL_UNSIGNALED
		mov rax, 37145
		push rax
		mov rax, [rbp-6344]; load GL_SIGNALED
		pop rcx
		mov [rbp-6344], rcx; store GL_SIGNALED
		mov rax, [rbp-6344]; load GL_SIGNALED
		mov rax, 37146
		push rax
		mov rax, [rbp-6352]; load GL_ALREADY_SIGNALED
		pop rcx
		mov [rbp-6352], rcx; store GL_ALREADY_SIGNALED
		mov rax, [rbp-6352]; load GL_ALREADY_SIGNALED
		mov rax, 37147
		push rax
		mov rax, [rbp-6360]; load GL_TIMEOUT_EXPIRED
		pop rcx
		mov [rbp-6360], rcx; store GL_TIMEOUT_EXPIRED
		mov rax, [rbp-6360]; load GL_TIMEOUT_EXPIRED
		mov rax, 37148
		push rax
		mov rax, [rbp-6368]; load GL_CONDITION_SATISFIED
		pop rcx
		mov [rbp-6368], rcx; store GL_CONDITION_SATISFIED
		mov rax, [rbp-6368]; load GL_CONDITION_SATISFIED
		mov rax, 37149
		push rax
		mov rax, [rbp-6376]; load GL_WAIT_FAILED
		pop rcx
		mov [rbp-6376], rcx; store GL_WAIT_FAILED
		mov rax, [rbp-6376]; load GL_WAIT_FAILED
		mov rax, -1
		push rax
		mov rax, [rbp-6384]; load GL_TIMEOUT_IGNORED
		pop rcx
		mov [rbp-6384], rcx; store GL_TIMEOUT_IGNORED
		mov rax, [rbp-6384]; load GL_TIMEOUT_IGNORED
		mov rax, 1
		push rax
		mov rax, [rbp-6392]; load GL_SYNC_FLUSH_COMMANDS_BIT
		pop rcx
		mov [rbp-6392], rcx; store GL_SYNC_FLUSH_COMMANDS_BIT
		mov rax, [rbp-6392]; load GL_SYNC_FLUSH_COMMANDS_BIT
		mov rax, 36432
		push rax
		mov rax, [rbp-6400]; load GL_SAMPLE_POSITION
		pop rcx
		mov [rbp-6400], rcx; store GL_SAMPLE_POSITION
		mov rax, [rbp-6400]; load GL_SAMPLE_POSITION
		mov rax, 36433
		push rax
		mov rax, [rbp-6408]; load GL_SAMPLE_MASK
		pop rcx
		mov [rbp-6408], rcx; store GL_SAMPLE_MASK
		mov rax, [rbp-6408]; load GL_SAMPLE_MASK
		mov rax, 36434
		push rax
		mov rax, [rbp-6416]; load GL_SAMPLE_MASK_VALUE
		pop rcx
		mov [rbp-6416], rcx; store GL_SAMPLE_MASK_VALUE
		mov rax, [rbp-6416]; load GL_SAMPLE_MASK_VALUE
		mov rax, 36441
		push rax
		mov rax, [rbp-6424]; load GL_MAX_SAMPLE_MASK_WORDS
		pop rcx
		mov [rbp-6424], rcx; store GL_MAX_SAMPLE_MASK_WORDS
		mov rax, [rbp-6424]; load GL_MAX_SAMPLE_MASK_WORDS
		mov rax, 37120
		push rax
		mov rax, [rbp-6432]; load GL_TEXTURE_2D_MULTISAMPLE
		pop rcx
		mov [rbp-6432], rcx; store GL_TEXTURE_2D_MULTISAMPLE
		mov rax, [rbp-6432]; load GL_TEXTURE_2D_MULTISAMPLE
		mov rax, 37121
		push rax
		mov rax, [rbp-6440]; load GL_PROXY_TEXTURE_2D_MULTISAMPLE
		pop rcx
		mov [rbp-6440], rcx; store GL_PROXY_TEXTURE_2D_MULTISAMPLE
		mov rax, [rbp-6440]; load GL_PROXY_TEXTURE_2D_MULTISAMPLE
		mov rax, 37122
		push rax
		mov rax, [rbp-6448]; load GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		pop rcx
		mov [rbp-6448], rcx; store GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		mov rax, [rbp-6448]; load GL_TEXTURE_2D_MULTISAMPLE_ARRAY
		mov rax, 37123
		push rax
		mov rax, [rbp-6456]; load GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY
		pop rcx
		mov [rbp-6456], rcx; store GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY
		mov rax, [rbp-6456]; load GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY
		mov rax, 37124
		push rax
		mov rax, [rbp-6464]; load GL_TEXTURE_BINDING_2D_MULTISAMPLE
		pop rcx
		mov [rbp-6464], rcx; store GL_TEXTURE_BINDING_2D_MULTISAMPLE
		mov rax, [rbp-6464]; load GL_TEXTURE_BINDING_2D_MULTISAMPLE
		mov rax, 37125
		push rax
		mov rax, [rbp-6472]; load GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY
		pop rcx
		mov [rbp-6472], rcx; store GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY
		mov rax, [rbp-6472]; load GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY
		mov rax, 37126
		push rax
		mov rax, [rbp-6480]; load GL_TEXTURE_SAMPLES
		pop rcx
		mov [rbp-6480], rcx; store GL_TEXTURE_SAMPLES
		mov rax, [rbp-6480]; load GL_TEXTURE_SAMPLES
		mov rax, 37127
		push rax
		mov rax, [rbp-6488]; load GL_TEXTURE_FIXED_SAMPLE_LOCATIONS
		pop rcx
		mov [rbp-6488], rcx; store GL_TEXTURE_FIXED_SAMPLE_LOCATIONS
		mov rax, [rbp-6488]; load GL_TEXTURE_FIXED_SAMPLE_LOCATIONS
		mov rax, 37128
		push rax
		mov rax, [rbp-6496]; load GL_SAMPLER_2D_MULTISAMPLE
		pop rcx
		mov [rbp-6496], rcx; store GL_SAMPLER_2D_MULTISAMPLE
		mov rax, [rbp-6496]; load GL_SAMPLER_2D_MULTISAMPLE
		mov rax, 37129
		push rax
		mov rax, [rbp-6504]; load GL_INT_SAMPLER_2D_MULTISAMPLE
		pop rcx
		mov [rbp-6504], rcx; store GL_INT_SAMPLER_2D_MULTISAMPLE
		mov rax, [rbp-6504]; load GL_INT_SAMPLER_2D_MULTISAMPLE
		mov rax, 37130
		push rax
		mov rax, [rbp-6512]; load GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE
		pop rcx
		mov [rbp-6512], rcx; store GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE
		mov rax, [rbp-6512]; load GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE
		mov rax, 37131
		push rax
		mov rax, [rbp-6520]; load GL_SAMPLER_2D_MULTISAMPLE_ARRAY
		pop rcx
		mov [rbp-6520], rcx; store GL_SAMPLER_2D_MULTISAMPLE_ARRAY
		mov rax, [rbp-6520]; load GL_SAMPLER_2D_MULTISAMPLE_ARRAY
		mov rax, 37132
		push rax
		mov rax, [rbp-6528]; load GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
		pop rcx
		mov [rbp-6528], rcx; store GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
		mov rax, [rbp-6528]; load GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
		mov rax, 37133
		push rax
		mov rax, [rbp-6536]; load GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
		pop rcx
		mov [rbp-6536], rcx; store GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
		mov rax, [rbp-6536]; load GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY
		mov rax, 37134
		push rax
		mov rax, [rbp-6544]; load GL_MAX_COLOR_TEXTURE_SAMPLES
		pop rcx
		mov [rbp-6544], rcx; store GL_MAX_COLOR_TEXTURE_SAMPLES
		mov rax, [rbp-6544]; load GL_MAX_COLOR_TEXTURE_SAMPLES
		mov rax, 37135
		push rax
		mov rax, [rbp-6552]; load GL_MAX_DEPTH_TEXTURE_SAMPLES
		pop rcx
		mov [rbp-6552], rcx; store GL_MAX_DEPTH_TEXTURE_SAMPLES
		mov rax, [rbp-6552]; load GL_MAX_DEPTH_TEXTURE_SAMPLES
		mov rax, 37136
		push rax
		mov rax, [rbp-6560]; load GL_MAX_INTEGER_SAMPLES
		pop rcx
		mov [rbp-6560], rcx; store GL_MAX_INTEGER_SAMPLES
		mov rax, [rbp-6560]; load GL_MAX_INTEGER_SAMPLES
		mov rax, 35070
		push rax
		mov rax, [rbp-6568]; load GL_VERTEX_ATTRIB_ARRAY_DIVISOR
		pop rcx
		mov [rbp-6568], rcx; store GL_VERTEX_ATTRIB_ARRAY_DIVISOR
		mov rax, [rbp-6568]; load GL_VERTEX_ATTRIB_ARRAY_DIVISOR
		mov rax, 35065
		push rax
		mov rax, [rbp-6576]; load GL_SRC1_COLOR
		pop rcx
		mov [rbp-6576], rcx; store GL_SRC1_COLOR
		mov rax, [rbp-6576]; load GL_SRC1_COLOR
		mov rax, 35066
		push rax
		mov rax, [rbp-6584]; load GL_ONE_MINUS_SRC1_COLOR
		pop rcx
		mov [rbp-6584], rcx; store GL_ONE_MINUS_SRC1_COLOR
		mov rax, [rbp-6584]; load GL_ONE_MINUS_SRC1_COLOR
		mov rax, 35067
		push rax
		mov rax, [rbp-6592]; load GL_ONE_MINUS_SRC1_ALPHA
		pop rcx
		mov [rbp-6592], rcx; store GL_ONE_MINUS_SRC1_ALPHA
		mov rax, [rbp-6592]; load GL_ONE_MINUS_SRC1_ALPHA
		mov rax, 35068
		push rax
		mov rax, [rbp-6600]; load GL_MAX_DUAL_SOURCE_DRAW_BUFFERS
		pop rcx
		mov [rbp-6600], rcx; store GL_MAX_DUAL_SOURCE_DRAW_BUFFERS
		mov rax, [rbp-6600]; load GL_MAX_DUAL_SOURCE_DRAW_BUFFERS
		mov rax, 35887
		push rax
		mov rax, [rbp-6608]; load GL_ANY_SAMPLES_PASSED
		pop rcx
		mov [rbp-6608], rcx; store GL_ANY_SAMPLES_PASSED
		mov rax, [rbp-6608]; load GL_ANY_SAMPLES_PASSED
		mov rax, 35097
		push rax
		mov rax, [rbp-6616]; load GL_SAMPLER_BINDING
		pop rcx
		mov [rbp-6616], rcx; store GL_SAMPLER_BINDING
		mov rax, [rbp-6616]; load GL_SAMPLER_BINDING
		mov rax, 36975
		push rax
		mov rax, [rbp-6624]; load GL_RGB10_A2UI
		pop rcx
		mov [rbp-6624], rcx; store GL_RGB10_A2UI
		mov rax, [rbp-6624]; load GL_RGB10_A2UI
		mov rax, 36418
		push rax
		mov rax, [rbp-6632]; load GL_TEXTURE_SWIZZLE_R
		pop rcx
		mov [rbp-6632], rcx; store GL_TEXTURE_SWIZZLE_R
		mov rax, [rbp-6632]; load GL_TEXTURE_SWIZZLE_R
		mov rax, 36419
		push rax
		mov rax, [rbp-6640]; load GL_TEXTURE_SWIZZLE_G
		pop rcx
		mov [rbp-6640], rcx; store GL_TEXTURE_SWIZZLE_G
		mov rax, [rbp-6640]; load GL_TEXTURE_SWIZZLE_G
		mov rax, 36420
		push rax
		mov rax, [rbp-6648]; load GL_TEXTURE_SWIZZLE_B
		pop rcx
		mov [rbp-6648], rcx; store GL_TEXTURE_SWIZZLE_B
		mov rax, [rbp-6648]; load GL_TEXTURE_SWIZZLE_B
		mov rax, 36421
		push rax
		mov rax, [rbp-6656]; load GL_TEXTURE_SWIZZLE_A
		pop rcx
		mov [rbp-6656], rcx; store GL_TEXTURE_SWIZZLE_A
		mov rax, [rbp-6656]; load GL_TEXTURE_SWIZZLE_A
		mov rax, 36422
		push rax
		mov rax, [rbp-6664]; load GL_TEXTURE_SWIZZLE_RGBA
		pop rcx
		mov [rbp-6664], rcx; store GL_TEXTURE_SWIZZLE_RGBA
		mov rax, [rbp-6664]; load GL_TEXTURE_SWIZZLE_RGBA
		mov rax, 35007
		push rax
		mov rax, [rbp-6672]; load GL_TIME_ELAPSED
		pop rcx
		mov [rbp-6672], rcx; store GL_TIME_ELAPSED
		mov rax, [rbp-6672]; load GL_TIME_ELAPSED
		mov rax, 36392
		push rax
		mov rax, [rbp-6680]; load GL_TIMESTAMP
		pop rcx
		mov [rbp-6680], rcx; store GL_TIMESTAMP
		mov rax, [rbp-6680]; load GL_TIMESTAMP
		mov rax, 36255
		push rax
		mov rax, [rbp-6688]; load GL_INT_2_10_10_10_REV
		pop rcx
		mov [rbp-6688], rcx; store GL_INT_2_10_10_10_REV
		mov rax, [rbp-6688]; load GL_INT_2_10_10_10_REV
		mov rax, 35894
		push rax
		mov rax, [rbp-6696]; load GL_SAMPLE_SHADING
		pop rcx
		mov [rbp-6696], rcx; store GL_SAMPLE_SHADING
		mov rax, [rbp-6696]; load GL_SAMPLE_SHADING
		mov rax, 35895
		push rax
		mov rax, [rbp-6704]; load GL_MIN_SAMPLE_SHADING_VALUE
		pop rcx
		mov [rbp-6704], rcx; store GL_MIN_SAMPLE_SHADING_VALUE
		mov rax, [rbp-6704]; load GL_MIN_SAMPLE_SHADING_VALUE
		mov rax, 36446
		push rax
		mov rax, [rbp-6712]; load GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET
		pop rcx
		mov [rbp-6712], rcx; store GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET
		mov rax, [rbp-6712]; load GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET
		mov rax, 36447
		push rax
		mov rax, [rbp-6720]; load GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET
		pop rcx
		mov [rbp-6720], rcx; store GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET
		mov rax, [rbp-6720]; load GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET
		mov rax, 36873
		push rax
		mov rax, [rbp-6728]; load GL_TEXTURE_CUBE_MAP_ARRAY
		pop rcx
		mov [rbp-6728], rcx; store GL_TEXTURE_CUBE_MAP_ARRAY
		mov rax, [rbp-6728]; load GL_TEXTURE_CUBE_MAP_ARRAY
		mov rax, 36874
		push rax
		mov rax, [rbp-6736]; load GL_TEXTURE_BINDING_CUBE_MAP_ARRAY
		pop rcx
		mov [rbp-6736], rcx; store GL_TEXTURE_BINDING_CUBE_MAP_ARRAY
		mov rax, [rbp-6736]; load GL_TEXTURE_BINDING_CUBE_MAP_ARRAY
		mov rax, 36875
		push rax
		mov rax, [rbp-6744]; load GL_PROXY_TEXTURE_CUBE_MAP_ARRAY
		pop rcx
		mov [rbp-6744], rcx; store GL_PROXY_TEXTURE_CUBE_MAP_ARRAY
		mov rax, [rbp-6744]; load GL_PROXY_TEXTURE_CUBE_MAP_ARRAY
		mov rax, 36876
		push rax
		mov rax, [rbp-6752]; load GL_SAMPLER_CUBE_MAP_ARRAY
		pop rcx
		mov [rbp-6752], rcx; store GL_SAMPLER_CUBE_MAP_ARRAY
		mov rax, [rbp-6752]; load GL_SAMPLER_CUBE_MAP_ARRAY
		mov rax, 36877
		push rax
		mov rax, [rbp-6760]; load GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW
		pop rcx
		mov [rbp-6760], rcx; store GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW
		mov rax, [rbp-6760]; load GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW
		mov rax, 36878
		push rax
		mov rax, [rbp-6768]; load GL_INT_SAMPLER_CUBE_MAP_ARRAY
		pop rcx
		mov [rbp-6768], rcx; store GL_INT_SAMPLER_CUBE_MAP_ARRAY
		mov rax, [rbp-6768]; load GL_INT_SAMPLER_CUBE_MAP_ARRAY
		mov rax, 36879
		push rax
		mov rax, [rbp-6776]; load GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY
		pop rcx
		mov [rbp-6776], rcx; store GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY
		mov rax, [rbp-6776]; load GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY
		mov rax, 36671
		push rax
		mov rax, [rbp-6784]; load GL_DRAW_INDIRECT_BUFFER
		pop rcx
		mov [rbp-6784], rcx; store GL_DRAW_INDIRECT_BUFFER
		mov rax, [rbp-6784]; load GL_DRAW_INDIRECT_BUFFER
		mov rax, 36675
		push rax
		mov rax, [rbp-6792]; load GL_DRAW_INDIRECT_BUFFER_BINDING
		pop rcx
		mov [rbp-6792], rcx; store GL_DRAW_INDIRECT_BUFFER_BINDING
		mov rax, [rbp-6792]; load GL_DRAW_INDIRECT_BUFFER_BINDING
		mov rax, 34943
		push rax
		mov rax, [rbp-6800]; load GL_GEOMETRY_SHADER_INVOCATIONS
		pop rcx
		mov [rbp-6800], rcx; store GL_GEOMETRY_SHADER_INVOCATIONS
		mov rax, [rbp-6800]; load GL_GEOMETRY_SHADER_INVOCATIONS
		mov rax, 36442
		push rax
		mov rax, [rbp-6808]; load GL_MAX_GEOMETRY_SHADER_INVOCATIONS
		pop rcx
		mov [rbp-6808], rcx; store GL_MAX_GEOMETRY_SHADER_INVOCATIONS
		mov rax, [rbp-6808]; load GL_MAX_GEOMETRY_SHADER_INVOCATIONS
		mov rax, 36443
		push rax
		mov rax, [rbp-6816]; load GL_MIN_FRAGMENT_INTERPOLATION_OFFSET
		pop rcx
		mov [rbp-6816], rcx; store GL_MIN_FRAGMENT_INTERPOLATION_OFFSET
		mov rax, [rbp-6816]; load GL_MIN_FRAGMENT_INTERPOLATION_OFFSET
		mov rax, 36444
		push rax
		mov rax, [rbp-6824]; load GL_MAX_FRAGMENT_INTERPOLATION_OFFSET
		pop rcx
		mov [rbp-6824], rcx; store GL_MAX_FRAGMENT_INTERPOLATION_OFFSET
		mov rax, [rbp-6824]; load GL_MAX_FRAGMENT_INTERPOLATION_OFFSET
		mov rax, 36445
		push rax
		mov rax, [rbp-6832]; load GL_FRAGMENT_INTERPOLATION_OFFSET_BITS
		pop rcx
		mov [rbp-6832], rcx; store GL_FRAGMENT_INTERPOLATION_OFFSET_BITS
		mov rax, [rbp-6832]; load GL_FRAGMENT_INTERPOLATION_OFFSET_BITS
		mov rax, 36465
		push rax
		mov rax, [rbp-6840]; load GL_MAX_VERTEX_STREAMS
		pop rcx
		mov [rbp-6840], rcx; store GL_MAX_VERTEX_STREAMS
		mov rax, [rbp-6840]; load GL_MAX_VERTEX_STREAMS
		mov rax, 36860
		push rax
		mov rax, [rbp-6848]; load GL_DOUBLE_VEC2
		pop rcx
		mov [rbp-6848], rcx; store GL_DOUBLE_VEC2
		mov rax, [rbp-6848]; load GL_DOUBLE_VEC2
		mov rax, 36861
		push rax
		mov rax, [rbp-6856]; load GL_DOUBLE_VEC3
		pop rcx
		mov [rbp-6856], rcx; store GL_DOUBLE_VEC3
		mov rax, [rbp-6856]; load GL_DOUBLE_VEC3
		mov rax, 36862
		push rax
		mov rax, [rbp-6864]; load GL_DOUBLE_VEC4
		pop rcx
		mov [rbp-6864], rcx; store GL_DOUBLE_VEC4
		mov rax, [rbp-6864]; load GL_DOUBLE_VEC4
		mov rax, 36678
		push rax
		mov rax, [rbp-6872]; load GL_DOUBLE_MAT2
		pop rcx
		mov [rbp-6872], rcx; store GL_DOUBLE_MAT2
		mov rax, [rbp-6872]; load GL_DOUBLE_MAT2
		mov rax, 36679
		push rax
		mov rax, [rbp-6880]; load GL_DOUBLE_MAT3
		pop rcx
		mov [rbp-6880], rcx; store GL_DOUBLE_MAT3
		mov rax, [rbp-6880]; load GL_DOUBLE_MAT3
		mov rax, 36680
		push rax
		mov rax, [rbp-6888]; load GL_DOUBLE_MAT4
		pop rcx
		mov [rbp-6888], rcx; store GL_DOUBLE_MAT4
		mov rax, [rbp-6888]; load GL_DOUBLE_MAT4
		mov rax, 36681
		push rax
		mov rax, [rbp-6896]; load GL_DOUBLE_MAT2x3
		pop rcx
		mov [rbp-6896], rcx; store GL_DOUBLE_MAT2x3
		mov rax, [rbp-6896]; load GL_DOUBLE_MAT2x3
		mov rax, 36682
		push rax
		mov rax, [rbp-6904]; load GL_DOUBLE_MAT2x4
		pop rcx
		mov [rbp-6904], rcx; store GL_DOUBLE_MAT2x4
		mov rax, [rbp-6904]; load GL_DOUBLE_MAT2x4
		mov rax, 36683
		push rax
		mov rax, [rbp-6912]; load GL_DOUBLE_MAT3x2
		pop rcx
		mov [rbp-6912], rcx; store GL_DOUBLE_MAT3x2
		mov rax, [rbp-6912]; load GL_DOUBLE_MAT3x2
		mov rax, 36684
		push rax
		mov rax, [rbp-6920]; load GL_DOUBLE_MAT3x4
		pop rcx
		mov [rbp-6920], rcx; store GL_DOUBLE_MAT3x4
		mov rax, [rbp-6920]; load GL_DOUBLE_MAT3x4
		mov rax, 36685
		push rax
		mov rax, [rbp-6928]; load GL_DOUBLE_MAT4x2
		pop rcx
		mov [rbp-6928], rcx; store GL_DOUBLE_MAT4x2
		mov rax, [rbp-6928]; load GL_DOUBLE_MAT4x2
		mov rax, 36686
		push rax
		mov rax, [rbp-6936]; load GL_DOUBLE_MAT4x3
		pop rcx
		mov [rbp-6936], rcx; store GL_DOUBLE_MAT4x3
		mov rax, [rbp-6936]; load GL_DOUBLE_MAT4x3
		mov rax, 36325
		push rax
		mov rax, [rbp-6944]; load GL_ACTIVE_SUBROUTINES
		pop rcx
		mov [rbp-6944], rcx; store GL_ACTIVE_SUBROUTINES
		mov rax, [rbp-6944]; load GL_ACTIVE_SUBROUTINES
		mov rax, 36326
		push rax
		mov rax, [rbp-6952]; load GL_ACTIVE_SUBROUTINE_UNIFORMS
		pop rcx
		mov [rbp-6952], rcx; store GL_ACTIVE_SUBROUTINE_UNIFORMS
		mov rax, [rbp-6952]; load GL_ACTIVE_SUBROUTINE_UNIFORMS
		mov rax, 36423
		push rax
		mov rax, [rbp-6960]; load GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS
		pop rcx
		mov [rbp-6960], rcx; store GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS
		mov rax, [rbp-6960]; load GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS
		mov rax, 36424
		push rax
		mov rax, [rbp-6968]; load GL_ACTIVE_SUBROUTINE_MAX_LENGTH
		pop rcx
		mov [rbp-6968], rcx; store GL_ACTIVE_SUBROUTINE_MAX_LENGTH
		mov rax, [rbp-6968]; load GL_ACTIVE_SUBROUTINE_MAX_LENGTH
		mov rax, 36425
		push rax
		mov rax, [rbp-6976]; load GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH
		pop rcx
		mov [rbp-6976], rcx; store GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH
		mov rax, [rbp-6976]; load GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH
		mov rax, 36327
		push rax
		mov rax, [rbp-6984]; load GL_MAX_SUBROUTINES
		pop rcx
		mov [rbp-6984], rcx; store GL_MAX_SUBROUTINES
		mov rax, [rbp-6984]; load GL_MAX_SUBROUTINES
		mov rax, 36328
		push rax
		mov rax, [rbp-6992]; load GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS
		pop rcx
		mov [rbp-6992], rcx; store GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS
		mov rax, [rbp-6992]; load GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS
		mov rax, 36426
		push rax
		mov rax, [rbp-7000]; load GL_NUM_COMPATIBLE_SUBROUTINES
		pop rcx
		mov [rbp-7000], rcx; store GL_NUM_COMPATIBLE_SUBROUTINES
		mov rax, [rbp-7000]; load GL_NUM_COMPATIBLE_SUBROUTINES
		mov rax, 36427
		push rax
		mov rax, [rbp-7008]; load GL_COMPATIBLE_SUBROUTINES
		pop rcx
		mov [rbp-7008], rcx; store GL_COMPATIBLE_SUBROUTINES
		mov rax, [rbp-7008]; load GL_COMPATIBLE_SUBROUTINES
		mov rax, 14
		push rax
		mov rax, [rbp-7016]; load GL_PATCHES
		pop rcx
		mov [rbp-7016], rcx; store GL_PATCHES
		mov rax, [rbp-7016]; load GL_PATCHES
		mov rax, 36466
		push rax
		mov rax, [rbp-7024]; load GL_PATCH_VERTICES
		pop rcx
		mov [rbp-7024], rcx; store GL_PATCH_VERTICES
		mov rax, [rbp-7024]; load GL_PATCH_VERTICES
		mov rax, 36467
		push rax
		mov rax, [rbp-7032]; load GL_PATCH_DEFAULT_INNER_LEVEL
		pop rcx
		mov [rbp-7032], rcx; store GL_PATCH_DEFAULT_INNER_LEVEL
		mov rax, [rbp-7032]; load GL_PATCH_DEFAULT_INNER_LEVEL
		mov rax, 36468
		push rax
		mov rax, [rbp-7040]; load GL_PATCH_DEFAULT_OUTER_LEVEL
		pop rcx
		mov [rbp-7040], rcx; store GL_PATCH_DEFAULT_OUTER_LEVEL
		mov rax, [rbp-7040]; load GL_PATCH_DEFAULT_OUTER_LEVEL
		mov rax, 36469
		push rax
		mov rax, [rbp-7048]; load GL_TESS_CONTROL_OUTPUT_VERTICES
		pop rcx
		mov [rbp-7048], rcx; store GL_TESS_CONTROL_OUTPUT_VERTICES
		mov rax, [rbp-7048]; load GL_TESS_CONTROL_OUTPUT_VERTICES
		mov rax, 36470
		push rax
		mov rax, [rbp-7056]; load GL_TESS_GEN_MODE
		pop rcx
		mov [rbp-7056], rcx; store GL_TESS_GEN_MODE
		mov rax, [rbp-7056]; load GL_TESS_GEN_MODE
		mov rax, 36471
		push rax
		mov rax, [rbp-7064]; load GL_TESS_GEN_SPACING
		pop rcx
		mov [rbp-7064], rcx; store GL_TESS_GEN_SPACING
		mov rax, [rbp-7064]; load GL_TESS_GEN_SPACING
		mov rax, 36472
		push rax
		mov rax, [rbp-7072]; load GL_TESS_GEN_VERTEX_ORDER
		pop rcx
		mov [rbp-7072], rcx; store GL_TESS_GEN_VERTEX_ORDER
		mov rax, [rbp-7072]; load GL_TESS_GEN_VERTEX_ORDER
		mov rax, 36473
		push rax
		mov rax, [rbp-7080]; load GL_TESS_GEN_POINT_MODE
		pop rcx
		mov [rbp-7080], rcx; store GL_TESS_GEN_POINT_MODE
		mov rax, [rbp-7080]; load GL_TESS_GEN_POINT_MODE
		mov rax, 36474
		push rax
		mov rax, [rbp-7088]; load GL_ISOLINES
		pop rcx
		mov [rbp-7088], rcx; store GL_ISOLINES
		mov rax, [rbp-7088]; load GL_ISOLINES
		mov rax, 36475
		push rax
		mov rax, [rbp-7096]; load GL_FRACTIONAL_ODD
		pop rcx
		mov [rbp-7096], rcx; store GL_FRACTIONAL_ODD
		mov rax, [rbp-7096]; load GL_FRACTIONAL_ODD
		mov rax, 36476
		push rax
		mov rax, [rbp-7104]; load GL_FRACTIONAL_EVEN
		pop rcx
		mov [rbp-7104], rcx; store GL_FRACTIONAL_EVEN
		mov rax, [rbp-7104]; load GL_FRACTIONAL_EVEN
		mov rax, 36477
		push rax
		mov rax, [rbp-7112]; load GL_MAX_PATCH_VERTICES
		pop rcx
		mov [rbp-7112], rcx; store GL_MAX_PATCH_VERTICES
		mov rax, [rbp-7112]; load GL_MAX_PATCH_VERTICES
		mov rax, 36478
		push rax
		mov rax, [rbp-7120]; load GL_MAX_TESS_GEN_LEVEL
		pop rcx
		mov [rbp-7120], rcx; store GL_MAX_TESS_GEN_LEVEL
		mov rax, [rbp-7120]; load GL_MAX_TESS_GEN_LEVEL
		mov rax, 36479
		push rax
		mov rax, [rbp-7128]; load GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-7128], rcx; store GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS
		mov rax, [rbp-7128]; load GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS
		mov rax, 36480
		push rax
		mov rax, [rbp-7136]; load GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-7136], rcx; store GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS
		mov rax, [rbp-7136]; load GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS
		mov rax, 36481
		push rax
		mov rax, [rbp-7144]; load GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS
		pop rcx
		mov [rbp-7144], rcx; store GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS
		mov rax, [rbp-7144]; load GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS
		mov rax, 36482
		push rax
		mov rax, [rbp-7152]; load GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS
		pop rcx
		mov [rbp-7152], rcx; store GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS
		mov rax, [rbp-7152]; load GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS
		mov rax, 36483
		push rax
		mov rax, [rbp-7160]; load GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS
		pop rcx
		mov [rbp-7160], rcx; store GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS
		mov rax, [rbp-7160]; load GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS
		mov rax, 36484
		push rax
		mov rax, [rbp-7168]; load GL_MAX_TESS_PATCH_COMPONENTS
		pop rcx
		mov [rbp-7168], rcx; store GL_MAX_TESS_PATCH_COMPONENTS
		mov rax, [rbp-7168]; load GL_MAX_TESS_PATCH_COMPONENTS
		mov rax, 36485
		push rax
		mov rax, [rbp-7176]; load GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS
		pop rcx
		mov [rbp-7176], rcx; store GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS
		mov rax, [rbp-7176]; load GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS
		mov rax, 36486
		push rax
		mov rax, [rbp-7184]; load GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS
		pop rcx
		mov [rbp-7184], rcx; store GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS
		mov rax, [rbp-7184]; load GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS
		mov rax, 36489
		push rax
		mov rax, [rbp-7192]; load GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS
		pop rcx
		mov [rbp-7192], rcx; store GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS
		mov rax, [rbp-7192]; load GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS
		mov rax, 36490
		push rax
		mov rax, [rbp-7200]; load GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS
		pop rcx
		mov [rbp-7200], rcx; store GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS
		mov rax, [rbp-7200]; load GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS
		mov rax, 34924
		push rax
		mov rax, [rbp-7208]; load GL_MAX_TESS_CONTROL_INPUT_COMPONENTS
		pop rcx
		mov [rbp-7208], rcx; store GL_MAX_TESS_CONTROL_INPUT_COMPONENTS
		mov rax, [rbp-7208]; load GL_MAX_TESS_CONTROL_INPUT_COMPONENTS
		mov rax, 34925
		push rax
		mov rax, [rbp-7216]; load GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS
		pop rcx
		mov [rbp-7216], rcx; store GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS
		mov rax, [rbp-7216]; load GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS
		mov rax, 36382
		push rax
		mov rax, [rbp-7224]; load GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-7224], rcx; store GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS
		mov rax, [rbp-7224]; load GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS
		mov rax, 36383
		push rax
		mov rax, [rbp-7232]; load GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-7232], rcx; store GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS
		mov rax, [rbp-7232]; load GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS
		mov rax, 34032
		push rax
		mov rax, [rbp-7240]; load GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER
		pop rcx
		mov [rbp-7240], rcx; store GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER
		mov rax, [rbp-7240]; load GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER
		mov rax, 34033
		push rax
		mov rax, [rbp-7248]; load GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER
		pop rcx
		mov [rbp-7248], rcx; store GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER
		mov rax, [rbp-7248]; load GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER
		mov rax, 36487
		push rax
		mov rax, [rbp-7256]; load GL_TESS_EVALUATION_SHADER
		pop rcx
		mov [rbp-7256], rcx; store GL_TESS_EVALUATION_SHADER
		mov rax, [rbp-7256]; load GL_TESS_EVALUATION_SHADER
		mov rax, 36488
		push rax
		mov rax, [rbp-7264]; load GL_TESS_CONTROL_SHADER
		pop rcx
		mov [rbp-7264], rcx; store GL_TESS_CONTROL_SHADER
		mov rax, [rbp-7264]; load GL_TESS_CONTROL_SHADER
		mov rax, 36386
		push rax
		mov rax, [rbp-7272]; load GL_TRANSFORM_FEEDBACK
		pop rcx
		mov [rbp-7272], rcx; store GL_TRANSFORM_FEEDBACK
		mov rax, [rbp-7272]; load GL_TRANSFORM_FEEDBACK
		mov rax, 36387
		push rax
		mov rax, [rbp-7280]; load GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED
		pop rcx
		mov [rbp-7280], rcx; store GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED
		mov rax, [rbp-7280]; load GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED
		mov rax, 36388
		push rax
		mov rax, [rbp-7288]; load GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE
		pop rcx
		mov [rbp-7288], rcx; store GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE
		mov rax, [rbp-7288]; load GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE
		mov rax, 36389
		push rax
		mov rax, [rbp-7296]; load GL_TRANSFORM_FEEDBACK_BINDING
		pop rcx
		mov [rbp-7296], rcx; store GL_TRANSFORM_FEEDBACK_BINDING
		mov rax, [rbp-7296]; load GL_TRANSFORM_FEEDBACK_BINDING
		mov rax, 36464
		push rax
		mov rax, [rbp-7304]; load GL_MAX_TRANSFORM_FEEDBACK_BUFFERS
		pop rcx
		mov [rbp-7304], rcx; store GL_MAX_TRANSFORM_FEEDBACK_BUFFERS
		mov rax, [rbp-7304]; load GL_MAX_TRANSFORM_FEEDBACK_BUFFERS
		mov rax, 5132
		push rax
		mov rax, [rbp-7312]; load GL_FIXED
		pop rcx
		mov [rbp-7312], rcx; store GL_FIXED
		mov rax, [rbp-7312]; load GL_FIXED
		mov rax, 35738
		push rax
		mov rax, [rbp-7320]; load GL_IMPLEMENTATION_COLOR_READ_TYPE
		pop rcx
		mov [rbp-7320], rcx; store GL_IMPLEMENTATION_COLOR_READ_TYPE
		mov rax, [rbp-7320]; load GL_IMPLEMENTATION_COLOR_READ_TYPE
		mov rax, 35739
		push rax
		mov rax, [rbp-7328]; load GL_IMPLEMENTATION_COLOR_READ_FORMAT
		pop rcx
		mov [rbp-7328], rcx; store GL_IMPLEMENTATION_COLOR_READ_FORMAT
		mov rax, [rbp-7328]; load GL_IMPLEMENTATION_COLOR_READ_FORMAT
		mov rax, 36336
		push rax
		mov rax, [rbp-7336]; load GL_LOW_FLOAT
		pop rcx
		mov [rbp-7336], rcx; store GL_LOW_FLOAT
		mov rax, [rbp-7336]; load GL_LOW_FLOAT
		mov rax, 36337
		push rax
		mov rax, [rbp-7344]; load GL_MEDIUM_FLOAT
		pop rcx
		mov [rbp-7344], rcx; store GL_MEDIUM_FLOAT
		mov rax, [rbp-7344]; load GL_MEDIUM_FLOAT
		mov rax, 36338
		push rax
		mov rax, [rbp-7352]; load GL_HIGH_FLOAT
		pop rcx
		mov [rbp-7352], rcx; store GL_HIGH_FLOAT
		mov rax, [rbp-7352]; load GL_HIGH_FLOAT
		mov rax, 36339
		push rax
		mov rax, [rbp-7360]; load GL_LOW_INT
		pop rcx
		mov [rbp-7360], rcx; store GL_LOW_INT
		mov rax, [rbp-7360]; load GL_LOW_INT
		mov rax, 36340
		push rax
		mov rax, [rbp-7368]; load GL_MEDIUM_INT
		pop rcx
		mov [rbp-7368], rcx; store GL_MEDIUM_INT
		mov rax, [rbp-7368]; load GL_MEDIUM_INT
		mov rax, 36341
		push rax
		mov rax, [rbp-7376]; load GL_HIGH_INT
		pop rcx
		mov [rbp-7376], rcx; store GL_HIGH_INT
		mov rax, [rbp-7376]; load GL_HIGH_INT
		mov rax, 36346
		push rax
		mov rax, [rbp-7384]; load GL_SHADER_COMPILER
		pop rcx
		mov [rbp-7384], rcx; store GL_SHADER_COMPILER
		mov rax, [rbp-7384]; load GL_SHADER_COMPILER
		mov rax, 36344
		push rax
		mov rax, [rbp-7392]; load GL_SHADER_BINARY_FORMATS
		pop rcx
		mov [rbp-7392], rcx; store GL_SHADER_BINARY_FORMATS
		mov rax, [rbp-7392]; load GL_SHADER_BINARY_FORMATS
		mov rax, 36345
		push rax
		mov rax, [rbp-7400]; load GL_NUM_SHADER_BINARY_FORMATS
		pop rcx
		mov [rbp-7400], rcx; store GL_NUM_SHADER_BINARY_FORMATS
		mov rax, [rbp-7400]; load GL_NUM_SHADER_BINARY_FORMATS
		mov rax, 36347
		push rax
		mov rax, [rbp-7408]; load GL_MAX_VERTEX_UNIFORM_VECTORS
		pop rcx
		mov [rbp-7408], rcx; store GL_MAX_VERTEX_UNIFORM_VECTORS
		mov rax, [rbp-7408]; load GL_MAX_VERTEX_UNIFORM_VECTORS
		mov rax, 36348
		push rax
		mov rax, [rbp-7416]; load GL_MAX_VARYING_VECTORS
		pop rcx
		mov [rbp-7416], rcx; store GL_MAX_VARYING_VECTORS
		mov rax, [rbp-7416]; load GL_MAX_VARYING_VECTORS
		mov rax, 36349
		push rax
		mov rax, [rbp-7424]; load GL_MAX_FRAGMENT_UNIFORM_VECTORS
		pop rcx
		mov [rbp-7424], rcx; store GL_MAX_FRAGMENT_UNIFORM_VECTORS
		mov rax, [rbp-7424]; load GL_MAX_FRAGMENT_UNIFORM_VECTORS
		mov rax, 36194
		push rax
		mov rax, [rbp-7432]; load GL_RGB565
		pop rcx
		mov [rbp-7432], rcx; store GL_RGB565
		mov rax, [rbp-7432]; load GL_RGB565
		mov rax, 33367
		push rax
		mov rax, [rbp-7440]; load GL_PROGRAM_BINARY_RETRIEVABLE_HINT
		pop rcx
		mov [rbp-7440], rcx; store GL_PROGRAM_BINARY_RETRIEVABLE_HINT
		mov rax, [rbp-7440]; load GL_PROGRAM_BINARY_RETRIEVABLE_HINT
		mov rax, 34625
		push rax
		mov rax, [rbp-7448]; load GL_PROGRAM_BINARY_LENGTH
		pop rcx
		mov [rbp-7448], rcx; store GL_PROGRAM_BINARY_LENGTH
		mov rax, [rbp-7448]; load GL_PROGRAM_BINARY_LENGTH
		mov rax, 34814
		push rax
		mov rax, [rbp-7456]; load GL_NUM_PROGRAM_BINARY_FORMATS
		pop rcx
		mov [rbp-7456], rcx; store GL_NUM_PROGRAM_BINARY_FORMATS
		mov rax, [rbp-7456]; load GL_NUM_PROGRAM_BINARY_FORMATS
		mov rax, 34815
		push rax
		mov rax, [rbp-7464]; load GL_PROGRAM_BINARY_FORMATS
		pop rcx
		mov [rbp-7464], rcx; store GL_PROGRAM_BINARY_FORMATS
		mov rax, [rbp-7464]; load GL_PROGRAM_BINARY_FORMATS
		mov rax, 1
		push rax
		mov rax, [rbp-7472]; load GL_VERTEX_SHADER_BIT
		pop rcx
		mov [rbp-7472], rcx; store GL_VERTEX_SHADER_BIT
		mov rax, [rbp-7472]; load GL_VERTEX_SHADER_BIT
		mov rax, 2
		push rax
		mov rax, [rbp-7480]; load GL_FRAGMENT_SHADER_BIT
		pop rcx
		mov [rbp-7480], rcx; store GL_FRAGMENT_SHADER_BIT
		mov rax, [rbp-7480]; load GL_FRAGMENT_SHADER_BIT
		mov rax, 4
		push rax
		mov rax, [rbp-7488]; load GL_GEOMETRY_SHADER_BIT
		pop rcx
		mov [rbp-7488], rcx; store GL_GEOMETRY_SHADER_BIT
		mov rax, [rbp-7488]; load GL_GEOMETRY_SHADER_BIT
		mov rax, 8
		push rax
		mov rax, [rbp-7496]; load GL_TESS_CONTROL_SHADER_BIT
		pop rcx
		mov [rbp-7496], rcx; store GL_TESS_CONTROL_SHADER_BIT
		mov rax, [rbp-7496]; load GL_TESS_CONTROL_SHADER_BIT
		mov rax, 16
		push rax
		mov rax, [rbp-7504]; load GL_TESS_EVALUATION_SHADER_BIT
		pop rcx
		mov [rbp-7504], rcx; store GL_TESS_EVALUATION_SHADER_BIT
		mov rax, [rbp-7504]; load GL_TESS_EVALUATION_SHADER_BIT
		mov rax, -1
		push rax
		mov rax, [rbp-7512]; load GL_ALL_SHADER_BITS
		pop rcx
		mov [rbp-7512], rcx; store GL_ALL_SHADER_BITS
		mov rax, [rbp-7512]; load GL_ALL_SHADER_BITS
		mov rax, 33368
		push rax
		mov rax, [rbp-7520]; load GL_PROGRAM_SEPARABLE
		pop rcx
		mov [rbp-7520], rcx; store GL_PROGRAM_SEPARABLE
		mov rax, [rbp-7520]; load GL_PROGRAM_SEPARABLE
		mov rax, 33369
		push rax
		mov rax, [rbp-7528]; load GL_ACTIVE_PROGRAM
		pop rcx
		mov [rbp-7528], rcx; store GL_ACTIVE_PROGRAM
		mov rax, [rbp-7528]; load GL_ACTIVE_PROGRAM
		mov rax, 33370
		push rax
		mov rax, [rbp-7536]; load GL_PROGRAM_PIPELINE_BINDING
		pop rcx
		mov [rbp-7536], rcx; store GL_PROGRAM_PIPELINE_BINDING
		mov rax, [rbp-7536]; load GL_PROGRAM_PIPELINE_BINDING
		mov rax, 33371
		push rax
		mov rax, [rbp-7544]; load GL_MAX_VIEWPORTS
		pop rcx
		mov [rbp-7544], rcx; store GL_MAX_VIEWPORTS
		mov rax, [rbp-7544]; load GL_MAX_VIEWPORTS
		mov rax, 33372
		push rax
		mov rax, [rbp-7552]; load GL_VIEWPORT_SUBPIXEL_BITS
		pop rcx
		mov [rbp-7552], rcx; store GL_VIEWPORT_SUBPIXEL_BITS
		mov rax, [rbp-7552]; load GL_VIEWPORT_SUBPIXEL_BITS
		mov rax, 33373
		push rax
		mov rax, [rbp-7560]; load GL_VIEWPORT_BOUNDS_RANGE
		pop rcx
		mov [rbp-7560], rcx; store GL_VIEWPORT_BOUNDS_RANGE
		mov rax, [rbp-7560]; load GL_VIEWPORT_BOUNDS_RANGE
		mov rax, 33374
		push rax
		mov rax, [rbp-7568]; load GL_LAYER_PROVOKING_VERTEX
		pop rcx
		mov [rbp-7568], rcx; store GL_LAYER_PROVOKING_VERTEX
		mov rax, [rbp-7568]; load GL_LAYER_PROVOKING_VERTEX
		mov rax, 33375
		push rax
		mov rax, [rbp-7576]; load GL_VIEWPORT_INDEX_PROVOKING_VERTEX
		pop rcx
		mov [rbp-7576], rcx; store GL_VIEWPORT_INDEX_PROVOKING_VERTEX
		mov rax, [rbp-7576]; load GL_VIEWPORT_INDEX_PROVOKING_VERTEX
		mov rax, 33376
		push rax
		mov rax, [rbp-7584]; load GL_UNDEFINED_VERTEX
		pop rcx
		mov [rbp-7584], rcx; store GL_UNDEFINED_VERTEX
		mov rax, [rbp-7584]; load GL_UNDEFINED_VERTEX
		mov rax, 36662
		push rax
		mov rax, [rbp-7592]; load GL_COPY_READ_BUFFER_BINDING
		pop rcx
		mov [rbp-7592], rcx; store GL_COPY_READ_BUFFER_BINDING
		mov rax, [rbp-7592]; load GL_COPY_READ_BUFFER_BINDING
		mov rax, 36663
		push rax
		mov rax, [rbp-7600]; load GL_COPY_WRITE_BUFFER_BINDING
		pop rcx
		mov [rbp-7600], rcx; store GL_COPY_WRITE_BUFFER_BINDING
		mov rax, [rbp-7600]; load GL_COPY_WRITE_BUFFER_BINDING
		mov rax, 36388
		push rax
		mov rax, [rbp-7608]; load GL_TRANSFORM_FEEDBACK_ACTIVE
		pop rcx
		mov [rbp-7608], rcx; store GL_TRANSFORM_FEEDBACK_ACTIVE
		mov rax, [rbp-7608]; load GL_TRANSFORM_FEEDBACK_ACTIVE
		mov rax, 36387
		push rax
		mov rax, [rbp-7616]; load GL_TRANSFORM_FEEDBACK_PAUSED
		pop rcx
		mov [rbp-7616], rcx; store GL_TRANSFORM_FEEDBACK_PAUSED
		mov rax, [rbp-7616]; load GL_TRANSFORM_FEEDBACK_PAUSED
		mov rax, 37159
		push rax
		mov rax, [rbp-7624]; load GL_UNPACK_COMPRESSED_BLOCK_WIDTH
		pop rcx
		mov [rbp-7624], rcx; store GL_UNPACK_COMPRESSED_BLOCK_WIDTH
		mov rax, [rbp-7624]; load GL_UNPACK_COMPRESSED_BLOCK_WIDTH
		mov rax, 37160
		push rax
		mov rax, [rbp-7632]; load GL_UNPACK_COMPRESSED_BLOCK_HEIGHT
		pop rcx
		mov [rbp-7632], rcx; store GL_UNPACK_COMPRESSED_BLOCK_HEIGHT
		mov rax, [rbp-7632]; load GL_UNPACK_COMPRESSED_BLOCK_HEIGHT
		mov rax, 37161
		push rax
		mov rax, [rbp-7640]; load GL_UNPACK_COMPRESSED_BLOCK_DEPTH
		pop rcx
		mov [rbp-7640], rcx; store GL_UNPACK_COMPRESSED_BLOCK_DEPTH
		mov rax, [rbp-7640]; load GL_UNPACK_COMPRESSED_BLOCK_DEPTH
		mov rax, 37162
		push rax
		mov rax, [rbp-7648]; load GL_UNPACK_COMPRESSED_BLOCK_SIZE
		pop rcx
		mov [rbp-7648], rcx; store GL_UNPACK_COMPRESSED_BLOCK_SIZE
		mov rax, [rbp-7648]; load GL_UNPACK_COMPRESSED_BLOCK_SIZE
		mov rax, 37163
		push rax
		mov rax, [rbp-7656]; load GL_PACK_COMPRESSED_BLOCK_WIDTH
		pop rcx
		mov [rbp-7656], rcx; store GL_PACK_COMPRESSED_BLOCK_WIDTH
		mov rax, [rbp-7656]; load GL_PACK_COMPRESSED_BLOCK_WIDTH
		mov rax, 37164
		push rax
		mov rax, [rbp-7664]; load GL_PACK_COMPRESSED_BLOCK_HEIGHT
		pop rcx
		mov [rbp-7664], rcx; store GL_PACK_COMPRESSED_BLOCK_HEIGHT
		mov rax, [rbp-7664]; load GL_PACK_COMPRESSED_BLOCK_HEIGHT
		mov rax, 37165
		push rax
		mov rax, [rbp-7672]; load GL_PACK_COMPRESSED_BLOCK_DEPTH
		pop rcx
		mov [rbp-7672], rcx; store GL_PACK_COMPRESSED_BLOCK_DEPTH
		mov rax, [rbp-7672]; load GL_PACK_COMPRESSED_BLOCK_DEPTH
		mov rax, 37166
		push rax
		mov rax, [rbp-7680]; load GL_PACK_COMPRESSED_BLOCK_SIZE
		pop rcx
		mov [rbp-7680], rcx; store GL_PACK_COMPRESSED_BLOCK_SIZE
		mov rax, [rbp-7680]; load GL_PACK_COMPRESSED_BLOCK_SIZE
		mov rax, 37760
		push rax
		mov rax, [rbp-7688]; load GL_NUM_SAMPLE_COUNTS
		pop rcx
		mov [rbp-7688], rcx; store GL_NUM_SAMPLE_COUNTS
		mov rax, [rbp-7688]; load GL_NUM_SAMPLE_COUNTS
		mov rax, 37052
		push rax
		mov rax, [rbp-7696]; load GL_MIN_MAP_BUFFER_ALIGNMENT
		pop rcx
		mov [rbp-7696], rcx; store GL_MIN_MAP_BUFFER_ALIGNMENT
		mov rax, [rbp-7696]; load GL_MIN_MAP_BUFFER_ALIGNMENT
		mov rax, 37568
		push rax
		mov rax, [rbp-7704]; load GL_ATOMIC_COUNTER_BUFFER
		pop rcx
		mov [rbp-7704], rcx; store GL_ATOMIC_COUNTER_BUFFER
		mov rax, [rbp-7704]; load GL_ATOMIC_COUNTER_BUFFER
		mov rax, 37569
		push rax
		mov rax, [rbp-7712]; load GL_ATOMIC_COUNTER_BUFFER_BINDING
		pop rcx
		mov [rbp-7712], rcx; store GL_ATOMIC_COUNTER_BUFFER_BINDING
		mov rax, [rbp-7712]; load GL_ATOMIC_COUNTER_BUFFER_BINDING
		mov rax, 37570
		push rax
		mov rax, [rbp-7720]; load GL_ATOMIC_COUNTER_BUFFER_START
		pop rcx
		mov [rbp-7720], rcx; store GL_ATOMIC_COUNTER_BUFFER_START
		mov rax, [rbp-7720]; load GL_ATOMIC_COUNTER_BUFFER_START
		mov rax, 37571
		push rax
		mov rax, [rbp-7728]; load GL_ATOMIC_COUNTER_BUFFER_SIZE
		pop rcx
		mov [rbp-7728], rcx; store GL_ATOMIC_COUNTER_BUFFER_SIZE
		mov rax, [rbp-7728]; load GL_ATOMIC_COUNTER_BUFFER_SIZE
		mov rax, 37572
		push rax
		mov rax, [rbp-7736]; load GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE
		pop rcx
		mov [rbp-7736], rcx; store GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE
		mov rax, [rbp-7736]; load GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE
		mov rax, 37573
		push rax
		mov rax, [rbp-7744]; load GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS
		pop rcx
		mov [rbp-7744], rcx; store GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS
		mov rax, [rbp-7744]; load GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS
		mov rax, 37574
		push rax
		mov rax, [rbp-7752]; load GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES
		pop rcx
		mov [rbp-7752], rcx; store GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES
		mov rax, [rbp-7752]; load GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES
		mov rax, 37575
		push rax
		mov rax, [rbp-7760]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER
		pop rcx
		mov [rbp-7760], rcx; store GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER
		mov rax, [rbp-7760]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER
		mov rax, 37576
		push rax
		mov rax, [rbp-7768]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER
		pop rcx
		mov [rbp-7768], rcx; store GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER
		mov rax, [rbp-7768]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER
		mov rax, 37577
		push rax
		mov rax, [rbp-7776]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER
		pop rcx
		mov [rbp-7776], rcx; store GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER
		mov rax, [rbp-7776]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER
		mov rax, 37578
		push rax
		mov rax, [rbp-7784]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER
		pop rcx
		mov [rbp-7784], rcx; store GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER
		mov rax, [rbp-7784]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER
		mov rax, 37579
		push rax
		mov rax, [rbp-7792]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER
		pop rcx
		mov [rbp-7792], rcx; store GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER
		mov rax, [rbp-7792]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER
		mov rax, 37580
		push rax
		mov rax, [rbp-7800]; load GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS
		pop rcx
		mov [rbp-7800], rcx; store GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS
		mov rax, [rbp-7800]; load GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS
		mov rax, 37581
		push rax
		mov rax, [rbp-7808]; load GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS
		pop rcx
		mov [rbp-7808], rcx; store GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS
		mov rax, [rbp-7808]; load GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS
		mov rax, 37582
		push rax
		mov rax, [rbp-7816]; load GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS
		pop rcx
		mov [rbp-7816], rcx; store GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS
		mov rax, [rbp-7816]; load GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS
		mov rax, 37583
		push rax
		mov rax, [rbp-7824]; load GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS
		pop rcx
		mov [rbp-7824], rcx; store GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS
		mov rax, [rbp-7824]; load GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS
		mov rax, 37584
		push rax
		mov rax, [rbp-7832]; load GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS
		pop rcx
		mov [rbp-7832], rcx; store GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS
		mov rax, [rbp-7832]; load GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS
		mov rax, 37585
		push rax
		mov rax, [rbp-7840]; load GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS
		pop rcx
		mov [rbp-7840], rcx; store GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS
		mov rax, [rbp-7840]; load GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS
		mov rax, 37586
		push rax
		mov rax, [rbp-7848]; load GL_MAX_VERTEX_ATOMIC_COUNTERS
		pop rcx
		mov [rbp-7848], rcx; store GL_MAX_VERTEX_ATOMIC_COUNTERS
		mov rax, [rbp-7848]; load GL_MAX_VERTEX_ATOMIC_COUNTERS
		mov rax, 37587
		push rax
		mov rax, [rbp-7856]; load GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS
		pop rcx
		mov [rbp-7856], rcx; store GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS
		mov rax, [rbp-7856]; load GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS
		mov rax, 37588
		push rax
		mov rax, [rbp-7864]; load GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS
		pop rcx
		mov [rbp-7864], rcx; store GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS
		mov rax, [rbp-7864]; load GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS
		mov rax, 37589
		push rax
		mov rax, [rbp-7872]; load GL_MAX_GEOMETRY_ATOMIC_COUNTERS
		pop rcx
		mov [rbp-7872], rcx; store GL_MAX_GEOMETRY_ATOMIC_COUNTERS
		mov rax, [rbp-7872]; load GL_MAX_GEOMETRY_ATOMIC_COUNTERS
		mov rax, 37590
		push rax
		mov rax, [rbp-7880]; load GL_MAX_FRAGMENT_ATOMIC_COUNTERS
		pop rcx
		mov [rbp-7880], rcx; store GL_MAX_FRAGMENT_ATOMIC_COUNTERS
		mov rax, [rbp-7880]; load GL_MAX_FRAGMENT_ATOMIC_COUNTERS
		mov rax, 37591
		push rax
		mov rax, [rbp-7888]; load GL_MAX_COMBINED_ATOMIC_COUNTERS
		pop rcx
		mov [rbp-7888], rcx; store GL_MAX_COMBINED_ATOMIC_COUNTERS
		mov rax, [rbp-7888]; load GL_MAX_COMBINED_ATOMIC_COUNTERS
		mov rax, 37592
		push rax
		mov rax, [rbp-7896]; load GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE
		pop rcx
		mov [rbp-7896], rcx; store GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE
		mov rax, [rbp-7896]; load GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE
		mov rax, 37596
		push rax
		mov rax, [rbp-7904]; load GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS
		pop rcx
		mov [rbp-7904], rcx; store GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS
		mov rax, [rbp-7904]; load GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS
		mov rax, 37593
		push rax
		mov rax, [rbp-7912]; load GL_ACTIVE_ATOMIC_COUNTER_BUFFERS
		pop rcx
		mov [rbp-7912], rcx; store GL_ACTIVE_ATOMIC_COUNTER_BUFFERS
		mov rax, [rbp-7912]; load GL_ACTIVE_ATOMIC_COUNTER_BUFFERS
		mov rax, 37594
		push rax
		mov rax, [rbp-7920]; load GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX
		pop rcx
		mov [rbp-7920], rcx; store GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX
		mov rax, [rbp-7920]; load GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX
		mov rax, 37595
		push rax
		mov rax, [rbp-7928]; load GL_UNSIGNED_INT_ATOMIC_COUNTER
		pop rcx
		mov [rbp-7928], rcx; store GL_UNSIGNED_INT_ATOMIC_COUNTER
		mov rax, [rbp-7928]; load GL_UNSIGNED_INT_ATOMIC_COUNTER
		mov rax, 1
		push rax
		mov rax, [rbp-7936]; load GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT
		pop rcx
		mov [rbp-7936], rcx; store GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT
		mov rax, [rbp-7936]; load GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT
		mov rax, 2
		push rax
		mov rax, [rbp-7944]; load GL_ELEMENT_ARRAY_BARRIER_BIT
		pop rcx
		mov [rbp-7944], rcx; store GL_ELEMENT_ARRAY_BARRIER_BIT
		mov rax, [rbp-7944]; load GL_ELEMENT_ARRAY_BARRIER_BIT
		mov rax, 4
		push rax
		mov rax, [rbp-7952]; load GL_UNIFORM_BARRIER_BIT
		pop rcx
		mov [rbp-7952], rcx; store GL_UNIFORM_BARRIER_BIT
		mov rax, [rbp-7952]; load GL_UNIFORM_BARRIER_BIT
		mov rax, 8
		push rax
		mov rax, [rbp-7960]; load GL_TEXTURE_FETCH_BARRIER_BIT
		pop rcx
		mov [rbp-7960], rcx; store GL_TEXTURE_FETCH_BARRIER_BIT
		mov rax, [rbp-7960]; load GL_TEXTURE_FETCH_BARRIER_BIT
		mov rax, 32
		push rax
		mov rax, [rbp-7968]; load GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
		pop rcx
		mov [rbp-7968], rcx; store GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
		mov rax, [rbp-7968]; load GL_SHADER_IMAGE_ACCESS_BARRIER_BIT
		mov rax, 64
		push rax
		mov rax, [rbp-7976]; load GL_COMMAND_BARRIER_BIT
		pop rcx
		mov [rbp-7976], rcx; store GL_COMMAND_BARRIER_BIT
		mov rax, [rbp-7976]; load GL_COMMAND_BARRIER_BIT
		mov rax, 128
		push rax
		mov rax, [rbp-7984]; load GL_PIXEL_BUFFER_BARRIER_BIT
		pop rcx
		mov [rbp-7984], rcx; store GL_PIXEL_BUFFER_BARRIER_BIT
		mov rax, [rbp-7984]; load GL_PIXEL_BUFFER_BARRIER_BIT
		mov rax, 256
		push rax
		mov rax, [rbp-7992]; load GL_TEXTURE_UPDATE_BARRIER_BIT
		pop rcx
		mov [rbp-7992], rcx; store GL_TEXTURE_UPDATE_BARRIER_BIT
		mov rax, [rbp-7992]; load GL_TEXTURE_UPDATE_BARRIER_BIT
		mov rax, 512
		push rax
		mov rax, [rbp-8000]; load GL_BUFFER_UPDATE_BARRIER_BIT
		pop rcx
		mov [rbp-8000], rcx; store GL_BUFFER_UPDATE_BARRIER_BIT
		mov rax, [rbp-8000]; load GL_BUFFER_UPDATE_BARRIER_BIT
		mov rax, 1024
		push rax
		mov rax, [rbp-8008]; load GL_FRAMEBUFFER_BARRIER_BIT
		pop rcx
		mov [rbp-8008], rcx; store GL_FRAMEBUFFER_BARRIER_BIT
		mov rax, [rbp-8008]; load GL_FRAMEBUFFER_BARRIER_BIT
		mov rax, 2048
		push rax
		mov rax, [rbp-8016]; load GL_TRANSFORM_FEEDBACK_BARRIER_BIT
		pop rcx
		mov [rbp-8016], rcx; store GL_TRANSFORM_FEEDBACK_BARRIER_BIT
		mov rax, [rbp-8016]; load GL_TRANSFORM_FEEDBACK_BARRIER_BIT
		mov rax, 4096
		push rax
		mov rax, [rbp-8024]; load GL_ATOMIC_COUNTER_BARRIER_BIT
		pop rcx
		mov [rbp-8024], rcx; store GL_ATOMIC_COUNTER_BARRIER_BIT
		mov rax, [rbp-8024]; load GL_ATOMIC_COUNTER_BARRIER_BIT
		mov rax, -1
		push rax
		mov rax, [rbp-8032]; load GL_ALL_BARRIER_BITS
		pop rcx
		mov [rbp-8032], rcx; store GL_ALL_BARRIER_BITS
		mov rax, [rbp-8032]; load GL_ALL_BARRIER_BITS
		mov rax, 36664
		push rax
		mov rax, [rbp-8040]; load GL_MAX_IMAGE_UNITS
		pop rcx
		mov [rbp-8040], rcx; store GL_MAX_IMAGE_UNITS
		mov rax, [rbp-8040]; load GL_MAX_IMAGE_UNITS
		mov rax, 36665
		push rax
		mov rax, [rbp-8048]; load GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS
		pop rcx
		mov [rbp-8048], rcx; store GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS
		mov rax, [rbp-8048]; load GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS
		mov rax, 36666
		push rax
		mov rax, [rbp-8056]; load GL_IMAGE_BINDING_NAME
		pop rcx
		mov [rbp-8056], rcx; store GL_IMAGE_BINDING_NAME
		mov rax, [rbp-8056]; load GL_IMAGE_BINDING_NAME
		mov rax, 36667
		push rax
		mov rax, [rbp-8064]; load GL_IMAGE_BINDING_LEVEL
		pop rcx
		mov [rbp-8064], rcx; store GL_IMAGE_BINDING_LEVEL
		mov rax, [rbp-8064]; load GL_IMAGE_BINDING_LEVEL
		mov rax, 36668
		push rax
		mov rax, [rbp-8072]; load GL_IMAGE_BINDING_LAYERED
		pop rcx
		mov [rbp-8072], rcx; store GL_IMAGE_BINDING_LAYERED
		mov rax, [rbp-8072]; load GL_IMAGE_BINDING_LAYERED
		mov rax, 36669
		push rax
		mov rax, [rbp-8080]; load GL_IMAGE_BINDING_LAYER
		pop rcx
		mov [rbp-8080], rcx; store GL_IMAGE_BINDING_LAYER
		mov rax, [rbp-8080]; load GL_IMAGE_BINDING_LAYER
		mov rax, 36670
		push rax
		mov rax, [rbp-8088]; load GL_IMAGE_BINDING_ACCESS
		pop rcx
		mov [rbp-8088], rcx; store GL_IMAGE_BINDING_ACCESS
		mov rax, [rbp-8088]; load GL_IMAGE_BINDING_ACCESS
		mov rax, 36940
		push rax
		mov rax, [rbp-8096]; load GL_IMAGE_1D
		pop rcx
		mov [rbp-8096], rcx; store GL_IMAGE_1D
		mov rax, [rbp-8096]; load GL_IMAGE_1D
		mov rax, 36941
		push rax
		mov rax, [rbp-8104]; load GL_IMAGE_2D
		pop rcx
		mov [rbp-8104], rcx; store GL_IMAGE_2D
		mov rax, [rbp-8104]; load GL_IMAGE_2D
		mov rax, 36942
		push rax
		mov rax, [rbp-8112]; load GL_IMAGE_3D
		pop rcx
		mov [rbp-8112], rcx; store GL_IMAGE_3D
		mov rax, [rbp-8112]; load GL_IMAGE_3D
		mov rax, 36943
		push rax
		mov rax, [rbp-8120]; load GL_IMAGE_2D_RECT
		pop rcx
		mov [rbp-8120], rcx; store GL_IMAGE_2D_RECT
		mov rax, [rbp-8120]; load GL_IMAGE_2D_RECT
		mov rax, 36944
		push rax
		mov rax, [rbp-8128]; load GL_IMAGE_CUBE
		pop rcx
		mov [rbp-8128], rcx; store GL_IMAGE_CUBE
		mov rax, [rbp-8128]; load GL_IMAGE_CUBE
		mov rax, 36945
		push rax
		mov rax, [rbp-8136]; load GL_IMAGE_BUFFER
		pop rcx
		mov [rbp-8136], rcx; store GL_IMAGE_BUFFER
		mov rax, [rbp-8136]; load GL_IMAGE_BUFFER
		mov rax, 36946
		push rax
		mov rax, [rbp-8144]; load GL_IMAGE_1D_ARRAY
		pop rcx
		mov [rbp-8144], rcx; store GL_IMAGE_1D_ARRAY
		mov rax, [rbp-8144]; load GL_IMAGE_1D_ARRAY
		mov rax, 36947
		push rax
		mov rax, [rbp-8152]; load GL_IMAGE_2D_ARRAY
		pop rcx
		mov [rbp-8152], rcx; store GL_IMAGE_2D_ARRAY
		mov rax, [rbp-8152]; load GL_IMAGE_2D_ARRAY
		mov rax, 36948
		push rax
		mov rax, [rbp-8160]; load GL_IMAGE_CUBE_MAP_ARRAY
		pop rcx
		mov [rbp-8160], rcx; store GL_IMAGE_CUBE_MAP_ARRAY
		mov rax, [rbp-8160]; load GL_IMAGE_CUBE_MAP_ARRAY
		mov rax, 36949
		push rax
		mov rax, [rbp-8168]; load GL_IMAGE_2D_MULTISAMPLE
		pop rcx
		mov [rbp-8168], rcx; store GL_IMAGE_2D_MULTISAMPLE
		mov rax, [rbp-8168]; load GL_IMAGE_2D_MULTISAMPLE
		mov rax, 36950
		push rax
		mov rax, [rbp-8176]; load GL_IMAGE_2D_MULTISAMPLE_ARRAY
		pop rcx
		mov [rbp-8176], rcx; store GL_IMAGE_2D_MULTISAMPLE_ARRAY
		mov rax, [rbp-8176]; load GL_IMAGE_2D_MULTISAMPLE_ARRAY
		mov rax, 36951
		push rax
		mov rax, [rbp-8184]; load GL_INT_IMAGE_1D
		pop rcx
		mov [rbp-8184], rcx; store GL_INT_IMAGE_1D
		mov rax, [rbp-8184]; load GL_INT_IMAGE_1D
		mov rax, 36952
		push rax
		mov rax, [rbp-8192]; load GL_INT_IMAGE_2D
		pop rcx
		mov [rbp-8192], rcx; store GL_INT_IMAGE_2D
		mov rax, [rbp-8192]; load GL_INT_IMAGE_2D
		mov rax, 36953
		push rax
		mov rax, [rbp-8200]; load GL_INT_IMAGE_3D
		pop rcx
		mov [rbp-8200], rcx; store GL_INT_IMAGE_3D
		mov rax, [rbp-8200]; load GL_INT_IMAGE_3D
		mov rax, 36954
		push rax
		mov rax, [rbp-8208]; load GL_INT_IMAGE_2D_RECT
		pop rcx
		mov [rbp-8208], rcx; store GL_INT_IMAGE_2D_RECT
		mov rax, [rbp-8208]; load GL_INT_IMAGE_2D_RECT
		mov rax, 36955
		push rax
		mov rax, [rbp-8216]; load GL_INT_IMAGE_CUBE
		pop rcx
		mov [rbp-8216], rcx; store GL_INT_IMAGE_CUBE
		mov rax, [rbp-8216]; load GL_INT_IMAGE_CUBE
		mov rax, 36956
		push rax
		mov rax, [rbp-8224]; load GL_INT_IMAGE_BUFFER
		pop rcx
		mov [rbp-8224], rcx; store GL_INT_IMAGE_BUFFER
		mov rax, [rbp-8224]; load GL_INT_IMAGE_BUFFER
		mov rax, 36957
		push rax
		mov rax, [rbp-8232]; load GL_INT_IMAGE_1D_ARRAY
		pop rcx
		mov [rbp-8232], rcx; store GL_INT_IMAGE_1D_ARRAY
		mov rax, [rbp-8232]; load GL_INT_IMAGE_1D_ARRAY
		mov rax, 36958
		push rax
		mov rax, [rbp-8240]; load GL_INT_IMAGE_2D_ARRAY
		pop rcx
		mov [rbp-8240], rcx; store GL_INT_IMAGE_2D_ARRAY
		mov rax, [rbp-8240]; load GL_INT_IMAGE_2D_ARRAY
		mov rax, 36959
		push rax
		mov rax, [rbp-8248]; load GL_INT_IMAGE_CUBE_MAP_ARRAY
		pop rcx
		mov [rbp-8248], rcx; store GL_INT_IMAGE_CUBE_MAP_ARRAY
		mov rax, [rbp-8248]; load GL_INT_IMAGE_CUBE_MAP_ARRAY
		mov rax, 36960
		push rax
		mov rax, [rbp-8256]; load GL_INT_IMAGE_2D_MULTISAMPLE
		pop rcx
		mov [rbp-8256], rcx; store GL_INT_IMAGE_2D_MULTISAMPLE
		mov rax, [rbp-8256]; load GL_INT_IMAGE_2D_MULTISAMPLE
		mov rax, 36961
		push rax
		mov rax, [rbp-8264]; load GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY
		pop rcx
		mov [rbp-8264], rcx; store GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY
		mov rax, [rbp-8264]; load GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY
		mov rax, 36962
		push rax
		mov rax, [rbp-8272]; load GL_UNSIGNED_INT_IMAGE_1D
		pop rcx
		mov [rbp-8272], rcx; store GL_UNSIGNED_INT_IMAGE_1D
		mov rax, [rbp-8272]; load GL_UNSIGNED_INT_IMAGE_1D
		mov rax, 36963
		push rax
		mov rax, [rbp-8280]; load GL_UNSIGNED_INT_IMAGE_2D
		pop rcx
		mov [rbp-8280], rcx; store GL_UNSIGNED_INT_IMAGE_2D
		mov rax, [rbp-8280]; load GL_UNSIGNED_INT_IMAGE_2D
		mov rax, 36964
		push rax
		mov rax, [rbp-8288]; load GL_UNSIGNED_INT_IMAGE_3D
		pop rcx
		mov [rbp-8288], rcx; store GL_UNSIGNED_INT_IMAGE_3D
		mov rax, [rbp-8288]; load GL_UNSIGNED_INT_IMAGE_3D
		mov rax, 36965
		push rax
		mov rax, [rbp-8296]; load GL_UNSIGNED_INT_IMAGE_2D_RECT
		pop rcx
		mov [rbp-8296], rcx; store GL_UNSIGNED_INT_IMAGE_2D_RECT
		mov rax, [rbp-8296]; load GL_UNSIGNED_INT_IMAGE_2D_RECT
		mov rax, 36966
		push rax
		mov rax, [rbp-8304]; load GL_UNSIGNED_INT_IMAGE_CUBE
		pop rcx
		mov [rbp-8304], rcx; store GL_UNSIGNED_INT_IMAGE_CUBE
		mov rax, [rbp-8304]; load GL_UNSIGNED_INT_IMAGE_CUBE
		mov rax, 36967
		push rax
		mov rax, [rbp-8312]; load GL_UNSIGNED_INT_IMAGE_BUFFER
		pop rcx
		mov [rbp-8312], rcx; store GL_UNSIGNED_INT_IMAGE_BUFFER
		mov rax, [rbp-8312]; load GL_UNSIGNED_INT_IMAGE_BUFFER
		mov rax, 36968
		push rax
		mov rax, [rbp-8320]; load GL_UNSIGNED_INT_IMAGE_1D_ARRAY
		pop rcx
		mov [rbp-8320], rcx; store GL_UNSIGNED_INT_IMAGE_1D_ARRAY
		mov rax, [rbp-8320]; load GL_UNSIGNED_INT_IMAGE_1D_ARRAY
		mov rax, 36969
		push rax
		mov rax, [rbp-8328]; load GL_UNSIGNED_INT_IMAGE_2D_ARRAY
		pop rcx
		mov [rbp-8328], rcx; store GL_UNSIGNED_INT_IMAGE_2D_ARRAY
		mov rax, [rbp-8328]; load GL_UNSIGNED_INT_IMAGE_2D_ARRAY
		mov rax, 36970
		push rax
		mov rax, [rbp-8336]; load GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY
		pop rcx
		mov [rbp-8336], rcx; store GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY
		mov rax, [rbp-8336]; load GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY
		mov rax, 36971
		push rax
		mov rax, [rbp-8344]; load GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE
		pop rcx
		mov [rbp-8344], rcx; store GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE
		mov rax, [rbp-8344]; load GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE
		mov rax, 36972
		push rax
		mov rax, [rbp-8352]; load GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY
		pop rcx
		mov [rbp-8352], rcx; store GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY
		mov rax, [rbp-8352]; load GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY
		mov rax, 36973
		push rax
		mov rax, [rbp-8360]; load GL_MAX_IMAGE_SAMPLES
		pop rcx
		mov [rbp-8360], rcx; store GL_MAX_IMAGE_SAMPLES
		mov rax, [rbp-8360]; load GL_MAX_IMAGE_SAMPLES
		mov rax, 36974
		push rax
		mov rax, [rbp-8368]; load GL_IMAGE_BINDING_FORMAT
		pop rcx
		mov [rbp-8368], rcx; store GL_IMAGE_BINDING_FORMAT
		mov rax, [rbp-8368]; load GL_IMAGE_BINDING_FORMAT
		mov rax, 37063
		push rax
		mov rax, [rbp-8376]; load GL_IMAGE_FORMAT_COMPATIBILITY_TYPE
		pop rcx
		mov [rbp-8376], rcx; store GL_IMAGE_FORMAT_COMPATIBILITY_TYPE
		mov rax, [rbp-8376]; load GL_IMAGE_FORMAT_COMPATIBILITY_TYPE
		mov rax, 37064
		push rax
		mov rax, [rbp-8384]; load GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE
		pop rcx
		mov [rbp-8384], rcx; store GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE
		mov rax, [rbp-8384]; load GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE
		mov rax, 37065
		push rax
		mov rax, [rbp-8392]; load GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS
		pop rcx
		mov [rbp-8392], rcx; store GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS
		mov rax, [rbp-8392]; load GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS
		mov rax, 37066
		push rax
		mov rax, [rbp-8400]; load GL_MAX_VERTEX_IMAGE_UNIFORMS
		pop rcx
		mov [rbp-8400], rcx; store GL_MAX_VERTEX_IMAGE_UNIFORMS
		mov rax, [rbp-8400]; load GL_MAX_VERTEX_IMAGE_UNIFORMS
		mov rax, 37067
		push rax
		mov rax, [rbp-8408]; load GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS
		pop rcx
		mov [rbp-8408], rcx; store GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS
		mov rax, [rbp-8408]; load GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS
		mov rax, 37068
		push rax
		mov rax, [rbp-8416]; load GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS
		pop rcx
		mov [rbp-8416], rcx; store GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS
		mov rax, [rbp-8416]; load GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS
		mov rax, 37069
		push rax
		mov rax, [rbp-8424]; load GL_MAX_GEOMETRY_IMAGE_UNIFORMS
		pop rcx
		mov [rbp-8424], rcx; store GL_MAX_GEOMETRY_IMAGE_UNIFORMS
		mov rax, [rbp-8424]; load GL_MAX_GEOMETRY_IMAGE_UNIFORMS
		mov rax, 37070
		push rax
		mov rax, [rbp-8432]; load GL_MAX_FRAGMENT_IMAGE_UNIFORMS
		pop rcx
		mov [rbp-8432], rcx; store GL_MAX_FRAGMENT_IMAGE_UNIFORMS
		mov rax, [rbp-8432]; load GL_MAX_FRAGMENT_IMAGE_UNIFORMS
		mov rax, 37071
		push rax
		mov rax, [rbp-8440]; load GL_MAX_COMBINED_IMAGE_UNIFORMS
		pop rcx
		mov [rbp-8440], rcx; store GL_MAX_COMBINED_IMAGE_UNIFORMS
		mov rax, [rbp-8440]; load GL_MAX_COMBINED_IMAGE_UNIFORMS
		mov rax, 36492
		push rax
		mov rax, [rbp-8448]; load GL_COMPRESSED_RGBA_BPTC_UNORM
		pop rcx
		mov [rbp-8448], rcx; store GL_COMPRESSED_RGBA_BPTC_UNORM
		mov rax, [rbp-8448]; load GL_COMPRESSED_RGBA_BPTC_UNORM
		mov rax, 36493
		push rax
		mov rax, [rbp-8456]; load GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM
		pop rcx
		mov [rbp-8456], rcx; store GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM
		mov rax, [rbp-8456]; load GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM
		mov rax, 36494
		push rax
		mov rax, [rbp-8464]; load GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT
		pop rcx
		mov [rbp-8464], rcx; store GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT
		mov rax, [rbp-8464]; load GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT
		mov rax, 36495
		push rax
		mov rax, [rbp-8472]; load GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
		pop rcx
		mov [rbp-8472], rcx; store GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
		mov rax, [rbp-8472]; load GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT
		mov rax, 37167
		push rax
		mov rax, [rbp-8480]; load GL_TEXTURE_IMMUTABLE_FORMAT
		pop rcx
		mov [rbp-8480], rcx; store GL_TEXTURE_IMMUTABLE_FORMAT
		mov rax, [rbp-8480]; load GL_TEXTURE_IMMUTABLE_FORMAT
		mov rax, 33513
		push rax
		mov rax, [rbp-8488]; load GL_NUM_SHADING_LANGUAGE_VERSIONS
		pop rcx
		mov [rbp-8488], rcx; store GL_NUM_SHADING_LANGUAGE_VERSIONS
		mov rax, [rbp-8488]; load GL_NUM_SHADING_LANGUAGE_VERSIONS
		mov rax, 34638
		push rax
		mov rax, [rbp-8496]; load GL_VERTEX_ATTRIB_ARRAY_LONG
		pop rcx
		mov [rbp-8496], rcx; store GL_VERTEX_ATTRIB_ARRAY_LONG
		mov rax, [rbp-8496]; load GL_VERTEX_ATTRIB_ARRAY_LONG
		mov rax, 37492
		push rax
		mov rax, [rbp-8504]; load GL_COMPRESSED_RGB8_ETC2
		pop rcx
		mov [rbp-8504], rcx; store GL_COMPRESSED_RGB8_ETC2
		mov rax, [rbp-8504]; load GL_COMPRESSED_RGB8_ETC2
		mov rax, 37493
		push rax
		mov rax, [rbp-8512]; load GL_COMPRESSED_SRGB8_ETC2
		pop rcx
		mov [rbp-8512], rcx; store GL_COMPRESSED_SRGB8_ETC2
		mov rax, [rbp-8512]; load GL_COMPRESSED_SRGB8_ETC2
		mov rax, 37494
		push rax
		mov rax, [rbp-8520]; load GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2
		pop rcx
		mov [rbp-8520], rcx; store GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2
		mov rax, [rbp-8520]; load GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2
		mov rax, 37495
		push rax
		mov rax, [rbp-8528]; load GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2
		pop rcx
		mov [rbp-8528], rcx; store GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2
		mov rax, [rbp-8528]; load GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2
		mov rax, 37496
		push rax
		mov rax, [rbp-8536]; load GL_COMPRESSED_RGBA8_ETC2_EAC
		pop rcx
		mov [rbp-8536], rcx; store GL_COMPRESSED_RGBA8_ETC2_EAC
		mov rax, [rbp-8536]; load GL_COMPRESSED_RGBA8_ETC2_EAC
		mov rax, 37497
		push rax
		mov rax, [rbp-8544]; load GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC
		pop rcx
		mov [rbp-8544], rcx; store GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC
		mov rax, [rbp-8544]; load GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC
		mov rax, 37488
		push rax
		mov rax, [rbp-8552]; load GL_COMPRESSED_R11_EAC
		pop rcx
		mov [rbp-8552], rcx; store GL_COMPRESSED_R11_EAC
		mov rax, [rbp-8552]; load GL_COMPRESSED_R11_EAC
		mov rax, 37489
		push rax
		mov rax, [rbp-8560]; load GL_COMPRESSED_SIGNED_R11_EAC
		pop rcx
		mov [rbp-8560], rcx; store GL_COMPRESSED_SIGNED_R11_EAC
		mov rax, [rbp-8560]; load GL_COMPRESSED_SIGNED_R11_EAC
		mov rax, 37490
		push rax
		mov rax, [rbp-8568]; load GL_COMPRESSED_RG11_EAC
		pop rcx
		mov [rbp-8568], rcx; store GL_COMPRESSED_RG11_EAC
		mov rax, [rbp-8568]; load GL_COMPRESSED_RG11_EAC
		mov rax, 37491
		push rax
		mov rax, [rbp-8576]; load GL_COMPRESSED_SIGNED_RG11_EAC
		pop rcx
		mov [rbp-8576], rcx; store GL_COMPRESSED_SIGNED_RG11_EAC
		mov rax, [rbp-8576]; load GL_COMPRESSED_SIGNED_RG11_EAC
		mov rax, 36201
		push rax
		mov rax, [rbp-8584]; load GL_PRIMITIVE_RESTART_FIXED_INDEX
		pop rcx
		mov [rbp-8584], rcx; store GL_PRIMITIVE_RESTART_FIXED_INDEX
		mov rax, [rbp-8584]; load GL_PRIMITIVE_RESTART_FIXED_INDEX
		mov rax, 36202
		push rax
		mov rax, [rbp-8592]; load GL_ANY_SAMPLES_PASSED_CONSERVATIVE
		pop rcx
		mov [rbp-8592], rcx; store GL_ANY_SAMPLES_PASSED_CONSERVATIVE
		mov rax, [rbp-8592]; load GL_ANY_SAMPLES_PASSED_CONSERVATIVE
		mov rax, 36203
		push rax
		mov rax, [rbp-8600]; load GL_MAX_ELEMENT_INDEX
		pop rcx
		mov [rbp-8600], rcx; store GL_MAX_ELEMENT_INDEX
		mov rax, [rbp-8600]; load GL_MAX_ELEMENT_INDEX
		mov rax, 37305
		push rax
		mov rax, [rbp-8608]; load GL_COMPUTE_SHADER
		pop rcx
		mov [rbp-8608], rcx; store GL_COMPUTE_SHADER
		mov rax, [rbp-8608]; load GL_COMPUTE_SHADER
		mov rax, 37307
		push rax
		mov rax, [rbp-8616]; load GL_MAX_COMPUTE_UNIFORM_BLOCKS
		pop rcx
		mov [rbp-8616], rcx; store GL_MAX_COMPUTE_UNIFORM_BLOCKS
		mov rax, [rbp-8616]; load GL_MAX_COMPUTE_UNIFORM_BLOCKS
		mov rax, 37308
		push rax
		mov rax, [rbp-8624]; load GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS
		pop rcx
		mov [rbp-8624], rcx; store GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS
		mov rax, [rbp-8624]; load GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS
		mov rax, 37309
		push rax
		mov rax, [rbp-8632]; load GL_MAX_COMPUTE_IMAGE_UNIFORMS
		pop rcx
		mov [rbp-8632], rcx; store GL_MAX_COMPUTE_IMAGE_UNIFORMS
		mov rax, [rbp-8632]; load GL_MAX_COMPUTE_IMAGE_UNIFORMS
		mov rax, 33378
		push rax
		mov rax, [rbp-8640]; load GL_MAX_COMPUTE_SHARED_MEMORY_SIZE
		pop rcx
		mov [rbp-8640], rcx; store GL_MAX_COMPUTE_SHARED_MEMORY_SIZE
		mov rax, [rbp-8640]; load GL_MAX_COMPUTE_SHARED_MEMORY_SIZE
		mov rax, 33379
		push rax
		mov rax, [rbp-8648]; load GL_MAX_COMPUTE_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-8648], rcx; store GL_MAX_COMPUTE_UNIFORM_COMPONENTS
		mov rax, [rbp-8648]; load GL_MAX_COMPUTE_UNIFORM_COMPONENTS
		mov rax, 33380
		push rax
		mov rax, [rbp-8656]; load GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS
		pop rcx
		mov [rbp-8656], rcx; store GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS
		mov rax, [rbp-8656]; load GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS
		mov rax, 33381
		push rax
		mov rax, [rbp-8664]; load GL_MAX_COMPUTE_ATOMIC_COUNTERS
		pop rcx
		mov [rbp-8664], rcx; store GL_MAX_COMPUTE_ATOMIC_COUNTERS
		mov rax, [rbp-8664]; load GL_MAX_COMPUTE_ATOMIC_COUNTERS
		mov rax, 33382
		push rax
		mov rax, [rbp-8672]; load GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS
		pop rcx
		mov [rbp-8672], rcx; store GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS
		mov rax, [rbp-8672]; load GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS
		mov rax, 37099
		push rax
		mov rax, [rbp-8680]; load GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS
		pop rcx
		mov [rbp-8680], rcx; store GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS
		mov rax, [rbp-8680]; load GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS
		mov rax, 37310
		push rax
		mov rax, [rbp-8688]; load GL_MAX_COMPUTE_WORK_GROUP_COUNT
		pop rcx
		mov [rbp-8688], rcx; store GL_MAX_COMPUTE_WORK_GROUP_COUNT
		mov rax, [rbp-8688]; load GL_MAX_COMPUTE_WORK_GROUP_COUNT
		mov rax, 37311
		push rax
		mov rax, [rbp-8696]; load GL_MAX_COMPUTE_WORK_GROUP_SIZE
		pop rcx
		mov [rbp-8696], rcx; store GL_MAX_COMPUTE_WORK_GROUP_SIZE
		mov rax, [rbp-8696]; load GL_MAX_COMPUTE_WORK_GROUP_SIZE
		mov rax, 33383
		push rax
		mov rax, [rbp-8704]; load GL_COMPUTE_WORK_GROUP_SIZE
		pop rcx
		mov [rbp-8704], rcx; store GL_COMPUTE_WORK_GROUP_SIZE
		mov rax, [rbp-8704]; load GL_COMPUTE_WORK_GROUP_SIZE
		mov rax, 37100
		push rax
		mov rax, [rbp-8712]; load GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER
		pop rcx
		mov [rbp-8712], rcx; store GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER
		mov rax, [rbp-8712]; load GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER
		mov rax, 37101
		push rax
		mov rax, [rbp-8720]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER
		pop rcx
		mov [rbp-8720], rcx; store GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER
		mov rax, [rbp-8720]; load GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER
		mov rax, 37102
		push rax
		mov rax, [rbp-8728]; load GL_DISPATCH_INDIRECT_BUFFER
		pop rcx
		mov [rbp-8728], rcx; store GL_DISPATCH_INDIRECT_BUFFER
		mov rax, [rbp-8728]; load GL_DISPATCH_INDIRECT_BUFFER
		mov rax, 37103
		push rax
		mov rax, [rbp-8736]; load GL_DISPATCH_INDIRECT_BUFFER_BINDING
		pop rcx
		mov [rbp-8736], rcx; store GL_DISPATCH_INDIRECT_BUFFER_BINDING
		mov rax, [rbp-8736]; load GL_DISPATCH_INDIRECT_BUFFER_BINDING
		mov rax, 32
		push rax
		mov rax, [rbp-8744]; load GL_COMPUTE_SHADER_BIT
		pop rcx
		mov [rbp-8744], rcx; store GL_COMPUTE_SHADER_BIT
		mov rax, [rbp-8744]; load GL_COMPUTE_SHADER_BIT
		mov rax, 33346
		push rax
		mov rax, [rbp-8752]; load GL_DEBUG_OUTPUT_SYNCHRONOUS
		pop rcx
		mov [rbp-8752], rcx; store GL_DEBUG_OUTPUT_SYNCHRONOUS
		mov rax, [rbp-8752]; load GL_DEBUG_OUTPUT_SYNCHRONOUS
		mov rax, 33347
		push rax
		mov rax, [rbp-8760]; load GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH
		pop rcx
		mov [rbp-8760], rcx; store GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH
		mov rax, [rbp-8760]; load GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH
		mov rax, 33348
		push rax
		mov rax, [rbp-8768]; load GL_DEBUG_CALLBACK_FUNCTION
		pop rcx
		mov [rbp-8768], rcx; store GL_DEBUG_CALLBACK_FUNCTION
		mov rax, [rbp-8768]; load GL_DEBUG_CALLBACK_FUNCTION
		mov rax, 33349
		push rax
		mov rax, [rbp-8776]; load GL_DEBUG_CALLBACK_USER_PARAM
		pop rcx
		mov [rbp-8776], rcx; store GL_DEBUG_CALLBACK_USER_PARAM
		mov rax, [rbp-8776]; load GL_DEBUG_CALLBACK_USER_PARAM
		mov rax, 33350
		push rax
		mov rax, [rbp-8784]; load GL_DEBUG_SOURCE_API
		pop rcx
		mov [rbp-8784], rcx; store GL_DEBUG_SOURCE_API
		mov rax, [rbp-8784]; load GL_DEBUG_SOURCE_API
		mov rax, 33351
		push rax
		mov rax, [rbp-8792]; load GL_DEBUG_SOURCE_WINDOW_SYSTEM
		pop rcx
		mov [rbp-8792], rcx; store GL_DEBUG_SOURCE_WINDOW_SYSTEM
		mov rax, [rbp-8792]; load GL_DEBUG_SOURCE_WINDOW_SYSTEM
		mov rax, 33352
		push rax
		mov rax, [rbp-8800]; load GL_DEBUG_SOURCE_SHADER_COMPILER
		pop rcx
		mov [rbp-8800], rcx; store GL_DEBUG_SOURCE_SHADER_COMPILER
		mov rax, [rbp-8800]; load GL_DEBUG_SOURCE_SHADER_COMPILER
		mov rax, 33353
		push rax
		mov rax, [rbp-8808]; load GL_DEBUG_SOURCE_THIRD_PARTY
		pop rcx
		mov [rbp-8808], rcx; store GL_DEBUG_SOURCE_THIRD_PARTY
		mov rax, [rbp-8808]; load GL_DEBUG_SOURCE_THIRD_PARTY
		mov rax, 33354
		push rax
		mov rax, [rbp-8816]; load GL_DEBUG_SOURCE_APPLICATION
		pop rcx
		mov [rbp-8816], rcx; store GL_DEBUG_SOURCE_APPLICATION
		mov rax, [rbp-8816]; load GL_DEBUG_SOURCE_APPLICATION
		mov rax, 33355
		push rax
		mov rax, [rbp-8824]; load GL_DEBUG_SOURCE_OTHER
		pop rcx
		mov [rbp-8824], rcx; store GL_DEBUG_SOURCE_OTHER
		mov rax, [rbp-8824]; load GL_DEBUG_SOURCE_OTHER
		mov rax, 33356
		push rax
		mov rax, [rbp-8832]; load GL_DEBUG_TYPE_ERROR
		pop rcx
		mov [rbp-8832], rcx; store GL_DEBUG_TYPE_ERROR
		mov rax, [rbp-8832]; load GL_DEBUG_TYPE_ERROR
		mov rax, 33357
		push rax
		mov rax, [rbp-8840]; load GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR
		pop rcx
		mov [rbp-8840], rcx; store GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR
		mov rax, [rbp-8840]; load GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR
		mov rax, 33358
		push rax
		mov rax, [rbp-8848]; load GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR
		pop rcx
		mov [rbp-8848], rcx; store GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR
		mov rax, [rbp-8848]; load GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR
		mov rax, 33359
		push rax
		mov rax, [rbp-8856]; load GL_DEBUG_TYPE_PORTABILITY
		pop rcx
		mov [rbp-8856], rcx; store GL_DEBUG_TYPE_PORTABILITY
		mov rax, [rbp-8856]; load GL_DEBUG_TYPE_PORTABILITY
		mov rax, 33360
		push rax
		mov rax, [rbp-8864]; load GL_DEBUG_TYPE_PERFORMANCE
		pop rcx
		mov [rbp-8864], rcx; store GL_DEBUG_TYPE_PERFORMANCE
		mov rax, [rbp-8864]; load GL_DEBUG_TYPE_PERFORMANCE
		mov rax, 33361
		push rax
		mov rax, [rbp-8872]; load GL_DEBUG_TYPE_OTHER
		pop rcx
		mov [rbp-8872], rcx; store GL_DEBUG_TYPE_OTHER
		mov rax, [rbp-8872]; load GL_DEBUG_TYPE_OTHER
		mov rax, 37187
		push rax
		mov rax, [rbp-8880]; load GL_MAX_DEBUG_MESSAGE_LENGTH
		pop rcx
		mov [rbp-8880], rcx; store GL_MAX_DEBUG_MESSAGE_LENGTH
		mov rax, [rbp-8880]; load GL_MAX_DEBUG_MESSAGE_LENGTH
		mov rax, 37188
		push rax
		mov rax, [rbp-8888]; load GL_MAX_DEBUG_LOGGED_MESSAGES
		pop rcx
		mov [rbp-8888], rcx; store GL_MAX_DEBUG_LOGGED_MESSAGES
		mov rax, [rbp-8888]; load GL_MAX_DEBUG_LOGGED_MESSAGES
		mov rax, 37189
		push rax
		mov rax, [rbp-8896]; load GL_DEBUG_LOGGED_MESSAGES
		pop rcx
		mov [rbp-8896], rcx; store GL_DEBUG_LOGGED_MESSAGES
		mov rax, [rbp-8896]; load GL_DEBUG_LOGGED_MESSAGES
		mov rax, 37190
		push rax
		mov rax, [rbp-8904]; load GL_DEBUG_SEVERITY_HIGH
		pop rcx
		mov [rbp-8904], rcx; store GL_DEBUG_SEVERITY_HIGH
		mov rax, [rbp-8904]; load GL_DEBUG_SEVERITY_HIGH
		mov rax, 37191
		push rax
		mov rax, [rbp-8912]; load GL_DEBUG_SEVERITY_MEDIUM
		pop rcx
		mov [rbp-8912], rcx; store GL_DEBUG_SEVERITY_MEDIUM
		mov rax, [rbp-8912]; load GL_DEBUG_SEVERITY_MEDIUM
		mov rax, 37192
		push rax
		mov rax, [rbp-8920]; load GL_DEBUG_SEVERITY_LOW
		pop rcx
		mov [rbp-8920], rcx; store GL_DEBUG_SEVERITY_LOW
		mov rax, [rbp-8920]; load GL_DEBUG_SEVERITY_LOW
		mov rax, 33384
		push rax
		mov rax, [rbp-8928]; load GL_DEBUG_TYPE_MARKER
		pop rcx
		mov [rbp-8928], rcx; store GL_DEBUG_TYPE_MARKER
		mov rax, [rbp-8928]; load GL_DEBUG_TYPE_MARKER
		mov rax, 33385
		push rax
		mov rax, [rbp-8936]; load GL_DEBUG_TYPE_PUSH_GROUP
		pop rcx
		mov [rbp-8936], rcx; store GL_DEBUG_TYPE_PUSH_GROUP
		mov rax, [rbp-8936]; load GL_DEBUG_TYPE_PUSH_GROUP
		mov rax, 33386
		push rax
		mov rax, [rbp-8944]; load GL_DEBUG_TYPE_POP_GROUP
		pop rcx
		mov [rbp-8944], rcx; store GL_DEBUG_TYPE_POP_GROUP
		mov rax, [rbp-8944]; load GL_DEBUG_TYPE_POP_GROUP
		mov rax, 33387
		push rax
		mov rax, [rbp-8952]; load GL_DEBUG_SEVERITY_NOTIFICATION
		pop rcx
		mov [rbp-8952], rcx; store GL_DEBUG_SEVERITY_NOTIFICATION
		mov rax, [rbp-8952]; load GL_DEBUG_SEVERITY_NOTIFICATION
		mov rax, 33388
		push rax
		mov rax, [rbp-8960]; load GL_MAX_DEBUG_GROUP_STACK_DEPTH
		pop rcx
		mov [rbp-8960], rcx; store GL_MAX_DEBUG_GROUP_STACK_DEPTH
		mov rax, [rbp-8960]; load GL_MAX_DEBUG_GROUP_STACK_DEPTH
		mov rax, 33389
		push rax
		mov rax, [rbp-8968]; load GL_DEBUG_GROUP_STACK_DEPTH
		pop rcx
		mov [rbp-8968], rcx; store GL_DEBUG_GROUP_STACK_DEPTH
		mov rax, [rbp-8968]; load GL_DEBUG_GROUP_STACK_DEPTH
		mov rax, 33504
		push rax
		mov rax, [rbp-8976]; load GL_BUFFER
		pop rcx
		mov [rbp-8976], rcx; store GL_BUFFER
		mov rax, [rbp-8976]; load GL_BUFFER
		mov rax, 33505
		push rax
		mov rax, [rbp-8984]; load GL_SHADER
		pop rcx
		mov [rbp-8984], rcx; store GL_SHADER
		mov rax, [rbp-8984]; load GL_SHADER
		mov rax, 33506
		push rax
		mov rax, [rbp-8992]; load GL_PROGRAM
		pop rcx
		mov [rbp-8992], rcx; store GL_PROGRAM
		mov rax, [rbp-8992]; load GL_PROGRAM
		mov rax, 33507
		push rax
		mov rax, [rbp-9000]; load GL_QUERY
		pop rcx
		mov [rbp-9000], rcx; store GL_QUERY
		mov rax, [rbp-9000]; load GL_QUERY
		mov rax, 33508
		push rax
		mov rax, [rbp-9008]; load GL_PROGRAM_PIPELINE
		pop rcx
		mov [rbp-9008], rcx; store GL_PROGRAM_PIPELINE
		mov rax, [rbp-9008]; load GL_PROGRAM_PIPELINE
		mov rax, 33510
		push rax
		mov rax, [rbp-9016]; load GL_SAMPLER
		pop rcx
		mov [rbp-9016], rcx; store GL_SAMPLER
		mov rax, [rbp-9016]; load GL_SAMPLER
		mov rax, 33512
		push rax
		mov rax, [rbp-9024]; load GL_MAX_LABEL_LENGTH
		pop rcx
		mov [rbp-9024], rcx; store GL_MAX_LABEL_LENGTH
		mov rax, [rbp-9024]; load GL_MAX_LABEL_LENGTH
		mov rax, 37600
		push rax
		mov rax, [rbp-9032]; load GL_DEBUG_OUTPUT
		pop rcx
		mov [rbp-9032], rcx; store GL_DEBUG_OUTPUT
		mov rax, [rbp-9032]; load GL_DEBUG_OUTPUT
		mov rax, 2
		push rax
		mov rax, [rbp-9040]; load GL_CONTEXT_FLAG_DEBUG_BIT
		pop rcx
		mov [rbp-9040], rcx; store GL_CONTEXT_FLAG_DEBUG_BIT
		mov rax, [rbp-9040]; load GL_CONTEXT_FLAG_DEBUG_BIT
		mov rax, 33390
		push rax
		mov rax, [rbp-9048]; load GL_MAX_UNIFORM_LOCATIONS
		pop rcx
		mov [rbp-9048], rcx; store GL_MAX_UNIFORM_LOCATIONS
		mov rax, [rbp-9048]; load GL_MAX_UNIFORM_LOCATIONS
		mov rax, 37648
		push rax
		mov rax, [rbp-9056]; load GL_FRAMEBUFFER_DEFAULT_WIDTH
		pop rcx
		mov [rbp-9056], rcx; store GL_FRAMEBUFFER_DEFAULT_WIDTH
		mov rax, [rbp-9056]; load GL_FRAMEBUFFER_DEFAULT_WIDTH
		mov rax, 37649
		push rax
		mov rax, [rbp-9064]; load GL_FRAMEBUFFER_DEFAULT_HEIGHT
		pop rcx
		mov [rbp-9064], rcx; store GL_FRAMEBUFFER_DEFAULT_HEIGHT
		mov rax, [rbp-9064]; load GL_FRAMEBUFFER_DEFAULT_HEIGHT
		mov rax, 37650
		push rax
		mov rax, [rbp-9072]; load GL_FRAMEBUFFER_DEFAULT_LAYERS
		pop rcx
		mov [rbp-9072], rcx; store GL_FRAMEBUFFER_DEFAULT_LAYERS
		mov rax, [rbp-9072]; load GL_FRAMEBUFFER_DEFAULT_LAYERS
		mov rax, 37651
		push rax
		mov rax, [rbp-9080]; load GL_FRAMEBUFFER_DEFAULT_SAMPLES
		pop rcx
		mov [rbp-9080], rcx; store GL_FRAMEBUFFER_DEFAULT_SAMPLES
		mov rax, [rbp-9080]; load GL_FRAMEBUFFER_DEFAULT_SAMPLES
		mov rax, 37652
		push rax
		mov rax, [rbp-9088]; load GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS
		pop rcx
		mov [rbp-9088], rcx; store GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS
		mov rax, [rbp-9088]; load GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS
		mov rax, 37653
		push rax
		mov rax, [rbp-9096]; load GL_MAX_FRAMEBUFFER_WIDTH
		pop rcx
		mov [rbp-9096], rcx; store GL_MAX_FRAMEBUFFER_WIDTH
		mov rax, [rbp-9096]; load GL_MAX_FRAMEBUFFER_WIDTH
		mov rax, 37654
		push rax
		mov rax, [rbp-9104]; load GL_MAX_FRAMEBUFFER_HEIGHT
		pop rcx
		mov [rbp-9104], rcx; store GL_MAX_FRAMEBUFFER_HEIGHT
		mov rax, [rbp-9104]; load GL_MAX_FRAMEBUFFER_HEIGHT
		mov rax, 37655
		push rax
		mov rax, [rbp-9112]; load GL_MAX_FRAMEBUFFER_LAYERS
		pop rcx
		mov [rbp-9112], rcx; store GL_MAX_FRAMEBUFFER_LAYERS
		mov rax, [rbp-9112]; load GL_MAX_FRAMEBUFFER_LAYERS
		mov rax, 37656
		push rax
		mov rax, [rbp-9120]; load GL_MAX_FRAMEBUFFER_SAMPLES
		pop rcx
		mov [rbp-9120], rcx; store GL_MAX_FRAMEBUFFER_SAMPLES
		mov rax, [rbp-9120]; load GL_MAX_FRAMEBUFFER_SAMPLES
		mov rax, 33391
		push rax
		mov rax, [rbp-9128]; load GL_INTERNALFORMAT_SUPPORTED
		pop rcx
		mov [rbp-9128], rcx; store GL_INTERNALFORMAT_SUPPORTED
		mov rax, [rbp-9128]; load GL_INTERNALFORMAT_SUPPORTED
		mov rax, 33392
		push rax
		mov rax, [rbp-9136]; load GL_INTERNALFORMAT_PREFERRED
		pop rcx
		mov [rbp-9136], rcx; store GL_INTERNALFORMAT_PREFERRED
		mov rax, [rbp-9136]; load GL_INTERNALFORMAT_PREFERRED
		mov rax, 33393
		push rax
		mov rax, [rbp-9144]; load GL_INTERNALFORMAT_RED_SIZE
		pop rcx
		mov [rbp-9144], rcx; store GL_INTERNALFORMAT_RED_SIZE
		mov rax, [rbp-9144]; load GL_INTERNALFORMAT_RED_SIZE
		mov rax, 33394
		push rax
		mov rax, [rbp-9152]; load GL_INTERNALFORMAT_GREEN_SIZE
		pop rcx
		mov [rbp-9152], rcx; store GL_INTERNALFORMAT_GREEN_SIZE
		mov rax, [rbp-9152]; load GL_INTERNALFORMAT_GREEN_SIZE
		mov rax, 33395
		push rax
		mov rax, [rbp-9160]; load GL_INTERNALFORMAT_BLUE_SIZE
		pop rcx
		mov [rbp-9160], rcx; store GL_INTERNALFORMAT_BLUE_SIZE
		mov rax, [rbp-9160]; load GL_INTERNALFORMAT_BLUE_SIZE
		mov rax, 33396
		push rax
		mov rax, [rbp-9168]; load GL_INTERNALFORMAT_ALPHA_SIZE
		pop rcx
		mov [rbp-9168], rcx; store GL_INTERNALFORMAT_ALPHA_SIZE
		mov rax, [rbp-9168]; load GL_INTERNALFORMAT_ALPHA_SIZE
		mov rax, 33397
		push rax
		mov rax, [rbp-9176]; load GL_INTERNALFORMAT_DEPTH_SIZE
		pop rcx
		mov [rbp-9176], rcx; store GL_INTERNALFORMAT_DEPTH_SIZE
		mov rax, [rbp-9176]; load GL_INTERNALFORMAT_DEPTH_SIZE
		mov rax, 33398
		push rax
		mov rax, [rbp-9184]; load GL_INTERNALFORMAT_STENCIL_SIZE
		pop rcx
		mov [rbp-9184], rcx; store GL_INTERNALFORMAT_STENCIL_SIZE
		mov rax, [rbp-9184]; load GL_INTERNALFORMAT_STENCIL_SIZE
		mov rax, 33399
		push rax
		mov rax, [rbp-9192]; load GL_INTERNALFORMAT_SHARED_SIZE
		pop rcx
		mov [rbp-9192], rcx; store GL_INTERNALFORMAT_SHARED_SIZE
		mov rax, [rbp-9192]; load GL_INTERNALFORMAT_SHARED_SIZE
		mov rax, 33400
		push rax
		mov rax, [rbp-9200]; load GL_INTERNALFORMAT_RED_TYPE
		pop rcx
		mov [rbp-9200], rcx; store GL_INTERNALFORMAT_RED_TYPE
		mov rax, [rbp-9200]; load GL_INTERNALFORMAT_RED_TYPE
		mov rax, 33401
		push rax
		mov rax, [rbp-9208]; load GL_INTERNALFORMAT_GREEN_TYPE
		pop rcx
		mov [rbp-9208], rcx; store GL_INTERNALFORMAT_GREEN_TYPE
		mov rax, [rbp-9208]; load GL_INTERNALFORMAT_GREEN_TYPE
		mov rax, 33402
		push rax
		mov rax, [rbp-9216]; load GL_INTERNALFORMAT_BLUE_TYPE
		pop rcx
		mov [rbp-9216], rcx; store GL_INTERNALFORMAT_BLUE_TYPE
		mov rax, [rbp-9216]; load GL_INTERNALFORMAT_BLUE_TYPE
		mov rax, 33403
		push rax
		mov rax, [rbp-9224]; load GL_INTERNALFORMAT_ALPHA_TYPE
		pop rcx
		mov [rbp-9224], rcx; store GL_INTERNALFORMAT_ALPHA_TYPE
		mov rax, [rbp-9224]; load GL_INTERNALFORMAT_ALPHA_TYPE
		mov rax, 33404
		push rax
		mov rax, [rbp-9232]; load GL_INTERNALFORMAT_DEPTH_TYPE
		pop rcx
		mov [rbp-9232], rcx; store GL_INTERNALFORMAT_DEPTH_TYPE
		mov rax, [rbp-9232]; load GL_INTERNALFORMAT_DEPTH_TYPE
		mov rax, 33405
		push rax
		mov rax, [rbp-9240]; load GL_INTERNALFORMAT_STENCIL_TYPE
		pop rcx
		mov [rbp-9240], rcx; store GL_INTERNALFORMAT_STENCIL_TYPE
		mov rax, [rbp-9240]; load GL_INTERNALFORMAT_STENCIL_TYPE
		mov rax, 33406
		push rax
		mov rax, [rbp-9248]; load GL_MAX_WIDTH
		pop rcx
		mov [rbp-9248], rcx; store GL_MAX_WIDTH
		mov rax, [rbp-9248]; load GL_MAX_WIDTH
		mov rax, 33407
		push rax
		mov rax, [rbp-9256]; load GL_MAX_HEIGHT
		pop rcx
		mov [rbp-9256], rcx; store GL_MAX_HEIGHT
		mov rax, [rbp-9256]; load GL_MAX_HEIGHT
		mov rax, 33408
		push rax
		mov rax, [rbp-9264]; load GL_MAX_DEPTH
		pop rcx
		mov [rbp-9264], rcx; store GL_MAX_DEPTH
		mov rax, [rbp-9264]; load GL_MAX_DEPTH
		mov rax, 33409
		push rax
		mov rax, [rbp-9272]; load GL_MAX_LAYERS
		pop rcx
		mov [rbp-9272], rcx; store GL_MAX_LAYERS
		mov rax, [rbp-9272]; load GL_MAX_LAYERS
		mov rax, 33410
		push rax
		mov rax, [rbp-9280]; load GL_MAX_COMBINED_DIMENSIONS
		pop rcx
		mov [rbp-9280], rcx; store GL_MAX_COMBINED_DIMENSIONS
		mov rax, [rbp-9280]; load GL_MAX_COMBINED_DIMENSIONS
		mov rax, 33411
		push rax
		mov rax, [rbp-9288]; load GL_COLOR_COMPONENTS
		pop rcx
		mov [rbp-9288], rcx; store GL_COLOR_COMPONENTS
		mov rax, [rbp-9288]; load GL_COLOR_COMPONENTS
		mov rax, 33412
		push rax
		mov rax, [rbp-9296]; load GL_DEPTH_COMPONENTS
		pop rcx
		mov [rbp-9296], rcx; store GL_DEPTH_COMPONENTS
		mov rax, [rbp-9296]; load GL_DEPTH_COMPONENTS
		mov rax, 33413
		push rax
		mov rax, [rbp-9304]; load GL_STENCIL_COMPONENTS
		pop rcx
		mov [rbp-9304], rcx; store GL_STENCIL_COMPONENTS
		mov rax, [rbp-9304]; load GL_STENCIL_COMPONENTS
		mov rax, 33414
		push rax
		mov rax, [rbp-9312]; load GL_COLOR_RENDERABLE
		pop rcx
		mov [rbp-9312], rcx; store GL_COLOR_RENDERABLE
		mov rax, [rbp-9312]; load GL_COLOR_RENDERABLE
		mov rax, 33415
		push rax
		mov rax, [rbp-9320]; load GL_DEPTH_RENDERABLE
		pop rcx
		mov [rbp-9320], rcx; store GL_DEPTH_RENDERABLE
		mov rax, [rbp-9320]; load GL_DEPTH_RENDERABLE
		mov rax, 33416
		push rax
		mov rax, [rbp-9328]; load GL_STENCIL_RENDERABLE
		pop rcx
		mov [rbp-9328], rcx; store GL_STENCIL_RENDERABLE
		mov rax, [rbp-9328]; load GL_STENCIL_RENDERABLE
		mov rax, 33417
		push rax
		mov rax, [rbp-9336]; load GL_FRAMEBUFFER_RENDERABLE
		pop rcx
		mov [rbp-9336], rcx; store GL_FRAMEBUFFER_RENDERABLE
		mov rax, [rbp-9336]; load GL_FRAMEBUFFER_RENDERABLE
		mov rax, 33418
		push rax
		mov rax, [rbp-9344]; load GL_FRAMEBUFFER_RENDERABLE_LAYERED
		pop rcx
		mov [rbp-9344], rcx; store GL_FRAMEBUFFER_RENDERABLE_LAYERED
		mov rax, [rbp-9344]; load GL_FRAMEBUFFER_RENDERABLE_LAYERED
		mov rax, 33419
		push rax
		mov rax, [rbp-9352]; load GL_FRAMEBUFFER_BLEND
		pop rcx
		mov [rbp-9352], rcx; store GL_FRAMEBUFFER_BLEND
		mov rax, [rbp-9352]; load GL_FRAMEBUFFER_BLEND
		mov rax, 33420
		push rax
		mov rax, [rbp-9360]; load GL_READ_PIXELS
		pop rcx
		mov [rbp-9360], rcx; store GL_READ_PIXELS
		mov rax, [rbp-9360]; load GL_READ_PIXELS
		mov rax, 33421
		push rax
		mov rax, [rbp-9368]; load GL_READ_PIXELS_FORMAT
		pop rcx
		mov [rbp-9368], rcx; store GL_READ_PIXELS_FORMAT
		mov rax, [rbp-9368]; load GL_READ_PIXELS_FORMAT
		mov rax, 33422
		push rax
		mov rax, [rbp-9376]; load GL_READ_PIXELS_TYPE
		pop rcx
		mov [rbp-9376], rcx; store GL_READ_PIXELS_TYPE
		mov rax, [rbp-9376]; load GL_READ_PIXELS_TYPE
		mov rax, 33423
		push rax
		mov rax, [rbp-9384]; load GL_TEXTURE_IMAGE_FORMAT
		pop rcx
		mov [rbp-9384], rcx; store GL_TEXTURE_IMAGE_FORMAT
		mov rax, [rbp-9384]; load GL_TEXTURE_IMAGE_FORMAT
		mov rax, 33424
		push rax
		mov rax, [rbp-9392]; load GL_TEXTURE_IMAGE_TYPE
		pop rcx
		mov [rbp-9392], rcx; store GL_TEXTURE_IMAGE_TYPE
		mov rax, [rbp-9392]; load GL_TEXTURE_IMAGE_TYPE
		mov rax, 33425
		push rax
		mov rax, [rbp-9400]; load GL_GET_TEXTURE_IMAGE_FORMAT
		pop rcx
		mov [rbp-9400], rcx; store GL_GET_TEXTURE_IMAGE_FORMAT
		mov rax, [rbp-9400]; load GL_GET_TEXTURE_IMAGE_FORMAT
		mov rax, 33426
		push rax
		mov rax, [rbp-9408]; load GL_GET_TEXTURE_IMAGE_TYPE
		pop rcx
		mov [rbp-9408], rcx; store GL_GET_TEXTURE_IMAGE_TYPE
		mov rax, [rbp-9408]; load GL_GET_TEXTURE_IMAGE_TYPE
		mov rax, 33427
		push rax
		mov rax, [rbp-9416]; load GL_MIPMAP
		pop rcx
		mov [rbp-9416], rcx; store GL_MIPMAP
		mov rax, [rbp-9416]; load GL_MIPMAP
		mov rax, 33428
		push rax
		mov rax, [rbp-9424]; load GL_MANUAL_GENERATE_MIPMAP
		pop rcx
		mov [rbp-9424], rcx; store GL_MANUAL_GENERATE_MIPMAP
		mov rax, [rbp-9424]; load GL_MANUAL_GENERATE_MIPMAP
		mov rax, 33429
		push rax
		mov rax, [rbp-9432]; load GL_AUTO_GENERATE_MIPMAP
		pop rcx
		mov [rbp-9432], rcx; store GL_AUTO_GENERATE_MIPMAP
		mov rax, [rbp-9432]; load GL_AUTO_GENERATE_MIPMAP
		mov rax, 33430
		push rax
		mov rax, [rbp-9440]; load GL_COLOR_ENCODING
		pop rcx
		mov [rbp-9440], rcx; store GL_COLOR_ENCODING
		mov rax, [rbp-9440]; load GL_COLOR_ENCODING
		mov rax, 33431
		push rax
		mov rax, [rbp-9448]; load GL_SRGB_READ
		pop rcx
		mov [rbp-9448], rcx; store GL_SRGB_READ
		mov rax, [rbp-9448]; load GL_SRGB_READ
		mov rax, 33432
		push rax
		mov rax, [rbp-9456]; load GL_SRGB_WRITE
		pop rcx
		mov [rbp-9456], rcx; store GL_SRGB_WRITE
		mov rax, [rbp-9456]; load GL_SRGB_WRITE
		mov rax, 33434
		push rax
		mov rax, [rbp-9464]; load GL_FILTER
		pop rcx
		mov [rbp-9464], rcx; store GL_FILTER
		mov rax, [rbp-9464]; load GL_FILTER
		mov rax, 33435
		push rax
		mov rax, [rbp-9472]; load GL_VERTEX_TEXTURE
		pop rcx
		mov [rbp-9472], rcx; store GL_VERTEX_TEXTURE
		mov rax, [rbp-9472]; load GL_VERTEX_TEXTURE
		mov rax, 33436
		push rax
		mov rax, [rbp-9480]; load GL_TESS_CONTROL_TEXTURE
		pop rcx
		mov [rbp-9480], rcx; store GL_TESS_CONTROL_TEXTURE
		mov rax, [rbp-9480]; load GL_TESS_CONTROL_TEXTURE
		mov rax, 33437
		push rax
		mov rax, [rbp-9488]; load GL_TESS_EVALUATION_TEXTURE
		pop rcx
		mov [rbp-9488], rcx; store GL_TESS_EVALUATION_TEXTURE
		mov rax, [rbp-9488]; load GL_TESS_EVALUATION_TEXTURE
		mov rax, 33438
		push rax
		mov rax, [rbp-9496]; load GL_GEOMETRY_TEXTURE
		pop rcx
		mov [rbp-9496], rcx; store GL_GEOMETRY_TEXTURE
		mov rax, [rbp-9496]; load GL_GEOMETRY_TEXTURE
		mov rax, 33439
		push rax
		mov rax, [rbp-9504]; load GL_FRAGMENT_TEXTURE
		pop rcx
		mov [rbp-9504], rcx; store GL_FRAGMENT_TEXTURE
		mov rax, [rbp-9504]; load GL_FRAGMENT_TEXTURE
		mov rax, 33440
		push rax
		mov rax, [rbp-9512]; load GL_COMPUTE_TEXTURE
		pop rcx
		mov [rbp-9512], rcx; store GL_COMPUTE_TEXTURE
		mov rax, [rbp-9512]; load GL_COMPUTE_TEXTURE
		mov rax, 33441
		push rax
		mov rax, [rbp-9520]; load GL_TEXTURE_SHADOW
		pop rcx
		mov [rbp-9520], rcx; store GL_TEXTURE_SHADOW
		mov rax, [rbp-9520]; load GL_TEXTURE_SHADOW
		mov rax, 33442
		push rax
		mov rax, [rbp-9528]; load GL_TEXTURE_GATHER
		pop rcx
		mov [rbp-9528], rcx; store GL_TEXTURE_GATHER
		mov rax, [rbp-9528]; load GL_TEXTURE_GATHER
		mov rax, 33443
		push rax
		mov rax, [rbp-9536]; load GL_TEXTURE_GATHER_SHADOW
		pop rcx
		mov [rbp-9536], rcx; store GL_TEXTURE_GATHER_SHADOW
		mov rax, [rbp-9536]; load GL_TEXTURE_GATHER_SHADOW
		mov rax, 33444
		push rax
		mov rax, [rbp-9544]; load GL_SHADER_IMAGE_LOAD
		pop rcx
		mov [rbp-9544], rcx; store GL_SHADER_IMAGE_LOAD
		mov rax, [rbp-9544]; load GL_SHADER_IMAGE_LOAD
		mov rax, 33445
		push rax
		mov rax, [rbp-9552]; load GL_SHADER_IMAGE_STORE
		pop rcx
		mov [rbp-9552], rcx; store GL_SHADER_IMAGE_STORE
		mov rax, [rbp-9552]; load GL_SHADER_IMAGE_STORE
		mov rax, 33446
		push rax
		mov rax, [rbp-9560]; load GL_SHADER_IMAGE_ATOMIC
		pop rcx
		mov [rbp-9560], rcx; store GL_SHADER_IMAGE_ATOMIC
		mov rax, [rbp-9560]; load GL_SHADER_IMAGE_ATOMIC
		mov rax, 33447
		push rax
		mov rax, [rbp-9568]; load GL_IMAGE_TEXEL_SIZE
		pop rcx
		mov [rbp-9568], rcx; store GL_IMAGE_TEXEL_SIZE
		mov rax, [rbp-9568]; load GL_IMAGE_TEXEL_SIZE
		mov rax, 33448
		push rax
		mov rax, [rbp-9576]; load GL_IMAGE_COMPATIBILITY_CLASS
		pop rcx
		mov [rbp-9576], rcx; store GL_IMAGE_COMPATIBILITY_CLASS
		mov rax, [rbp-9576]; load GL_IMAGE_COMPATIBILITY_CLASS
		mov rax, 33449
		push rax
		mov rax, [rbp-9584]; load GL_IMAGE_PIXEL_FORMAT
		pop rcx
		mov [rbp-9584], rcx; store GL_IMAGE_PIXEL_FORMAT
		mov rax, [rbp-9584]; load GL_IMAGE_PIXEL_FORMAT
		mov rax, 33450
		push rax
		mov rax, [rbp-9592]; load GL_IMAGE_PIXEL_TYPE
		pop rcx
		mov [rbp-9592], rcx; store GL_IMAGE_PIXEL_TYPE
		mov rax, [rbp-9592]; load GL_IMAGE_PIXEL_TYPE
		mov rax, 33452
		push rax
		mov rax, [rbp-9600]; load GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST
		pop rcx
		mov [rbp-9600], rcx; store GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST
		mov rax, [rbp-9600]; load GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST
		mov rax, 33453
		push rax
		mov rax, [rbp-9608]; load GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST
		pop rcx
		mov [rbp-9608], rcx; store GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST
		mov rax, [rbp-9608]; load GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST
		mov rax, 33454
		push rax
		mov rax, [rbp-9616]; load GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE
		pop rcx
		mov [rbp-9616], rcx; store GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE
		mov rax, [rbp-9616]; load GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE
		mov rax, 33455
		push rax
		mov rax, [rbp-9624]; load GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE
		pop rcx
		mov [rbp-9624], rcx; store GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE
		mov rax, [rbp-9624]; load GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE
		mov rax, 33457
		push rax
		mov rax, [rbp-9632]; load GL_TEXTURE_COMPRESSED_BLOCK_WIDTH
		pop rcx
		mov [rbp-9632], rcx; store GL_TEXTURE_COMPRESSED_BLOCK_WIDTH
		mov rax, [rbp-9632]; load GL_TEXTURE_COMPRESSED_BLOCK_WIDTH
		mov rax, 33458
		push rax
		mov rax, [rbp-9640]; load GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT
		pop rcx
		mov [rbp-9640], rcx; store GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT
		mov rax, [rbp-9640]; load GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT
		mov rax, 33459
		push rax
		mov rax, [rbp-9648]; load GL_TEXTURE_COMPRESSED_BLOCK_SIZE
		pop rcx
		mov [rbp-9648], rcx; store GL_TEXTURE_COMPRESSED_BLOCK_SIZE
		mov rax, [rbp-9648]; load GL_TEXTURE_COMPRESSED_BLOCK_SIZE
		mov rax, 33460
		push rax
		mov rax, [rbp-9656]; load GL_CLEAR_BUFFER
		pop rcx
		mov [rbp-9656], rcx; store GL_CLEAR_BUFFER
		mov rax, [rbp-9656]; load GL_CLEAR_BUFFER
		mov rax, 33461
		push rax
		mov rax, [rbp-9664]; load GL_TEXTURE_VIEW
		pop rcx
		mov [rbp-9664], rcx; store GL_TEXTURE_VIEW
		mov rax, [rbp-9664]; load GL_TEXTURE_VIEW
		mov rax, 33462
		push rax
		mov rax, [rbp-9672]; load GL_VIEW_COMPATIBILITY_CLASS
		pop rcx
		mov [rbp-9672], rcx; store GL_VIEW_COMPATIBILITY_CLASS
		mov rax, [rbp-9672]; load GL_VIEW_COMPATIBILITY_CLASS
		mov rax, 33463
		push rax
		mov rax, [rbp-9680]; load GL_FULL_SUPPORT
		pop rcx
		mov [rbp-9680], rcx; store GL_FULL_SUPPORT
		mov rax, [rbp-9680]; load GL_FULL_SUPPORT
		mov rax, 33464
		push rax
		mov rax, [rbp-9688]; load GL_CAVEAT_SUPPORT
		pop rcx
		mov [rbp-9688], rcx; store GL_CAVEAT_SUPPORT
		mov rax, [rbp-9688]; load GL_CAVEAT_SUPPORT
		mov rax, 33465
		push rax
		mov rax, [rbp-9696]; load GL_IMAGE_CLASS_4_X_32
		pop rcx
		mov [rbp-9696], rcx; store GL_IMAGE_CLASS_4_X_32
		mov rax, [rbp-9696]; load GL_IMAGE_CLASS_4_X_32
		mov rax, 33466
		push rax
		mov rax, [rbp-9704]; load GL_IMAGE_CLASS_2_X_32
		pop rcx
		mov [rbp-9704], rcx; store GL_IMAGE_CLASS_2_X_32
		mov rax, [rbp-9704]; load GL_IMAGE_CLASS_2_X_32
		mov rax, 33467
		push rax
		mov rax, [rbp-9712]; load GL_IMAGE_CLASS_1_X_32
		pop rcx
		mov [rbp-9712], rcx; store GL_IMAGE_CLASS_1_X_32
		mov rax, [rbp-9712]; load GL_IMAGE_CLASS_1_X_32
		mov rax, 33468
		push rax
		mov rax, [rbp-9720]; load GL_IMAGE_CLASS_4_X_16
		pop rcx
		mov [rbp-9720], rcx; store GL_IMAGE_CLASS_4_X_16
		mov rax, [rbp-9720]; load GL_IMAGE_CLASS_4_X_16
		mov rax, 33469
		push rax
		mov rax, [rbp-9728]; load GL_IMAGE_CLASS_2_X_16
		pop rcx
		mov [rbp-9728], rcx; store GL_IMAGE_CLASS_2_X_16
		mov rax, [rbp-9728]; load GL_IMAGE_CLASS_2_X_16
		mov rax, 33470
		push rax
		mov rax, [rbp-9736]; load GL_IMAGE_CLASS_1_X_16
		pop rcx
		mov [rbp-9736], rcx; store GL_IMAGE_CLASS_1_X_16
		mov rax, [rbp-9736]; load GL_IMAGE_CLASS_1_X_16
		mov rax, 33471
		push rax
		mov rax, [rbp-9744]; load GL_IMAGE_CLASS_4_X_8
		pop rcx
		mov [rbp-9744], rcx; store GL_IMAGE_CLASS_4_X_8
		mov rax, [rbp-9744]; load GL_IMAGE_CLASS_4_X_8
		mov rax, 33472
		push rax
		mov rax, [rbp-9752]; load GL_IMAGE_CLASS_2_X_8
		pop rcx
		mov [rbp-9752], rcx; store GL_IMAGE_CLASS_2_X_8
		mov rax, [rbp-9752]; load GL_IMAGE_CLASS_2_X_8
		mov rax, 33473
		push rax
		mov rax, [rbp-9760]; load GL_IMAGE_CLASS_1_X_8
		pop rcx
		mov [rbp-9760], rcx; store GL_IMAGE_CLASS_1_X_8
		mov rax, [rbp-9760]; load GL_IMAGE_CLASS_1_X_8
		mov rax, 33474
		push rax
		mov rax, [rbp-9768]; load GL_IMAGE_CLASS_11_11_10
		pop rcx
		mov [rbp-9768], rcx; store GL_IMAGE_CLASS_11_11_10
		mov rax, [rbp-9768]; load GL_IMAGE_CLASS_11_11_10
		mov rax, 33475
		push rax
		mov rax, [rbp-9776]; load GL_IMAGE_CLASS_10_10_10_2
		pop rcx
		mov [rbp-9776], rcx; store GL_IMAGE_CLASS_10_10_10_2
		mov rax, [rbp-9776]; load GL_IMAGE_CLASS_10_10_10_2
		mov rax, 33476
		push rax
		mov rax, [rbp-9784]; load GL_VIEW_CLASS_128_BITS
		pop rcx
		mov [rbp-9784], rcx; store GL_VIEW_CLASS_128_BITS
		mov rax, [rbp-9784]; load GL_VIEW_CLASS_128_BITS
		mov rax, 33477
		push rax
		mov rax, [rbp-9792]; load GL_VIEW_CLASS_96_BITS
		pop rcx
		mov [rbp-9792], rcx; store GL_VIEW_CLASS_96_BITS
		mov rax, [rbp-9792]; load GL_VIEW_CLASS_96_BITS
		mov rax, 33478
		push rax
		mov rax, [rbp-9800]; load GL_VIEW_CLASS_64_BITS
		pop rcx
		mov [rbp-9800], rcx; store GL_VIEW_CLASS_64_BITS
		mov rax, [rbp-9800]; load GL_VIEW_CLASS_64_BITS
		mov rax, 33479
		push rax
		mov rax, [rbp-9808]; load GL_VIEW_CLASS_48_BITS
		pop rcx
		mov [rbp-9808], rcx; store GL_VIEW_CLASS_48_BITS
		mov rax, [rbp-9808]; load GL_VIEW_CLASS_48_BITS
		mov rax, 33480
		push rax
		mov rax, [rbp-9816]; load GL_VIEW_CLASS_32_BITS
		pop rcx
		mov [rbp-9816], rcx; store GL_VIEW_CLASS_32_BITS
		mov rax, [rbp-9816]; load GL_VIEW_CLASS_32_BITS
		mov rax, 33481
		push rax
		mov rax, [rbp-9824]; load GL_VIEW_CLASS_24_BITS
		pop rcx
		mov [rbp-9824], rcx; store GL_VIEW_CLASS_24_BITS
		mov rax, [rbp-9824]; load GL_VIEW_CLASS_24_BITS
		mov rax, 33482
		push rax
		mov rax, [rbp-9832]; load GL_VIEW_CLASS_16_BITS
		pop rcx
		mov [rbp-9832], rcx; store GL_VIEW_CLASS_16_BITS
		mov rax, [rbp-9832]; load GL_VIEW_CLASS_16_BITS
		mov rax, 33483
		push rax
		mov rax, [rbp-9840]; load GL_VIEW_CLASS_8_BITS
		pop rcx
		mov [rbp-9840], rcx; store GL_VIEW_CLASS_8_BITS
		mov rax, [rbp-9840]; load GL_VIEW_CLASS_8_BITS
		mov rax, 33484
		push rax
		mov rax, [rbp-9848]; load GL_VIEW_CLASS_S3TC_DXT1_RGB
		pop rcx
		mov [rbp-9848], rcx; store GL_VIEW_CLASS_S3TC_DXT1_RGB
		mov rax, [rbp-9848]; load GL_VIEW_CLASS_S3TC_DXT1_RGB
		mov rax, 33485
		push rax
		mov rax, [rbp-9856]; load GL_VIEW_CLASS_S3TC_DXT1_RGBA
		pop rcx
		mov [rbp-9856], rcx; store GL_VIEW_CLASS_S3TC_DXT1_RGBA
		mov rax, [rbp-9856]; load GL_VIEW_CLASS_S3TC_DXT1_RGBA
		mov rax, 33486
		push rax
		mov rax, [rbp-9864]; load GL_VIEW_CLASS_S3TC_DXT3_RGBA
		pop rcx
		mov [rbp-9864], rcx; store GL_VIEW_CLASS_S3TC_DXT3_RGBA
		mov rax, [rbp-9864]; load GL_VIEW_CLASS_S3TC_DXT3_RGBA
		mov rax, 33487
		push rax
		mov rax, [rbp-9872]; load GL_VIEW_CLASS_S3TC_DXT5_RGBA
		pop rcx
		mov [rbp-9872], rcx; store GL_VIEW_CLASS_S3TC_DXT5_RGBA
		mov rax, [rbp-9872]; load GL_VIEW_CLASS_S3TC_DXT5_RGBA
		mov rax, 33488
		push rax
		mov rax, [rbp-9880]; load GL_VIEW_CLASS_RGTC1_RED
		pop rcx
		mov [rbp-9880], rcx; store GL_VIEW_CLASS_RGTC1_RED
		mov rax, [rbp-9880]; load GL_VIEW_CLASS_RGTC1_RED
		mov rax, 33489
		push rax
		mov rax, [rbp-9888]; load GL_VIEW_CLASS_RGTC2_RG
		pop rcx
		mov [rbp-9888], rcx; store GL_VIEW_CLASS_RGTC2_RG
		mov rax, [rbp-9888]; load GL_VIEW_CLASS_RGTC2_RG
		mov rax, 33490
		push rax
		mov rax, [rbp-9896]; load GL_VIEW_CLASS_BPTC_UNORM
		pop rcx
		mov [rbp-9896], rcx; store GL_VIEW_CLASS_BPTC_UNORM
		mov rax, [rbp-9896]; load GL_VIEW_CLASS_BPTC_UNORM
		mov rax, 33491
		push rax
		mov rax, [rbp-9904]; load GL_VIEW_CLASS_BPTC_FLOAT
		pop rcx
		mov [rbp-9904], rcx; store GL_VIEW_CLASS_BPTC_FLOAT
		mov rax, [rbp-9904]; load GL_VIEW_CLASS_BPTC_FLOAT
		mov rax, 37601
		push rax
		mov rax, [rbp-9912]; load GL_UNIFORM
		pop rcx
		mov [rbp-9912], rcx; store GL_UNIFORM
		mov rax, [rbp-9912]; load GL_UNIFORM
		mov rax, 37602
		push rax
		mov rax, [rbp-9920]; load GL_UNIFORM_BLOCK
		pop rcx
		mov [rbp-9920], rcx; store GL_UNIFORM_BLOCK
		mov rax, [rbp-9920]; load GL_UNIFORM_BLOCK
		mov rax, 37603
		push rax
		mov rax, [rbp-9928]; load GL_PROGRAM_INPUT
		pop rcx
		mov [rbp-9928], rcx; store GL_PROGRAM_INPUT
		mov rax, [rbp-9928]; load GL_PROGRAM_INPUT
		mov rax, 37604
		push rax
		mov rax, [rbp-9936]; load GL_PROGRAM_OUTPUT
		pop rcx
		mov [rbp-9936], rcx; store GL_PROGRAM_OUTPUT
		mov rax, [rbp-9936]; load GL_PROGRAM_OUTPUT
		mov rax, 37605
		push rax
		mov rax, [rbp-9944]; load GL_BUFFER_VARIABLE
		pop rcx
		mov [rbp-9944], rcx; store GL_BUFFER_VARIABLE
		mov rax, [rbp-9944]; load GL_BUFFER_VARIABLE
		mov rax, 37606
		push rax
		mov rax, [rbp-9952]; load GL_SHADER_STORAGE_BLOCK
		pop rcx
		mov [rbp-9952], rcx; store GL_SHADER_STORAGE_BLOCK
		mov rax, [rbp-9952]; load GL_SHADER_STORAGE_BLOCK
		mov rax, 37608
		push rax
		mov rax, [rbp-9960]; load GL_VERTEX_SUBROUTINE
		pop rcx
		mov [rbp-9960], rcx; store GL_VERTEX_SUBROUTINE
		mov rax, [rbp-9960]; load GL_VERTEX_SUBROUTINE
		mov rax, 37609
		push rax
		mov rax, [rbp-9968]; load GL_TESS_CONTROL_SUBROUTINE
		pop rcx
		mov [rbp-9968], rcx; store GL_TESS_CONTROL_SUBROUTINE
		mov rax, [rbp-9968]; load GL_TESS_CONTROL_SUBROUTINE
		mov rax, 37610
		push rax
		mov rax, [rbp-9976]; load GL_TESS_EVALUATION_SUBROUTINE
		pop rcx
		mov [rbp-9976], rcx; store GL_TESS_EVALUATION_SUBROUTINE
		mov rax, [rbp-9976]; load GL_TESS_EVALUATION_SUBROUTINE
		mov rax, 37611
		push rax
		mov rax, [rbp-9984]; load GL_GEOMETRY_SUBROUTINE
		pop rcx
		mov [rbp-9984], rcx; store GL_GEOMETRY_SUBROUTINE
		mov rax, [rbp-9984]; load GL_GEOMETRY_SUBROUTINE
		mov rax, 37612
		push rax
		mov rax, [rbp-9992]; load GL_FRAGMENT_SUBROUTINE
		pop rcx
		mov [rbp-9992], rcx; store GL_FRAGMENT_SUBROUTINE
		mov rax, [rbp-9992]; load GL_FRAGMENT_SUBROUTINE
		mov rax, 37613
		push rax
		mov rax, [rbp-10000]; load GL_COMPUTE_SUBROUTINE
		pop rcx
		mov [rbp-10000], rcx; store GL_COMPUTE_SUBROUTINE
		mov rax, [rbp-10000]; load GL_COMPUTE_SUBROUTINE
		mov rax, 37614
		push rax
		mov rax, [rbp-10008]; load GL_VERTEX_SUBROUTINE_UNIFORM
		pop rcx
		mov [rbp-10008], rcx; store GL_VERTEX_SUBROUTINE_UNIFORM
		mov rax, [rbp-10008]; load GL_VERTEX_SUBROUTINE_UNIFORM
		mov rax, 37615
		push rax
		mov rax, [rbp-10016]; load GL_TESS_CONTROL_SUBROUTINE_UNIFORM
		pop rcx
		mov [rbp-10016], rcx; store GL_TESS_CONTROL_SUBROUTINE_UNIFORM
		mov rax, [rbp-10016]; load GL_TESS_CONTROL_SUBROUTINE_UNIFORM
		mov rax, 37616
		push rax
		mov rax, [rbp-10024]; load GL_TESS_EVALUATION_SUBROUTINE_UNIFORM
		pop rcx
		mov [rbp-10024], rcx; store GL_TESS_EVALUATION_SUBROUTINE_UNIFORM
		mov rax, [rbp-10024]; load GL_TESS_EVALUATION_SUBROUTINE_UNIFORM
		mov rax, 37617
		push rax
		mov rax, [rbp-10032]; load GL_GEOMETRY_SUBROUTINE_UNIFORM
		pop rcx
		mov [rbp-10032], rcx; store GL_GEOMETRY_SUBROUTINE_UNIFORM
		mov rax, [rbp-10032]; load GL_GEOMETRY_SUBROUTINE_UNIFORM
		mov rax, 37618
		push rax
		mov rax, [rbp-10040]; load GL_FRAGMENT_SUBROUTINE_UNIFORM
		pop rcx
		mov [rbp-10040], rcx; store GL_FRAGMENT_SUBROUTINE_UNIFORM
		mov rax, [rbp-10040]; load GL_FRAGMENT_SUBROUTINE_UNIFORM
		mov rax, 37619
		push rax
		mov rax, [rbp-10048]; load GL_COMPUTE_SUBROUTINE_UNIFORM
		pop rcx
		mov [rbp-10048], rcx; store GL_COMPUTE_SUBROUTINE_UNIFORM
		mov rax, [rbp-10048]; load GL_COMPUTE_SUBROUTINE_UNIFORM
		mov rax, 37620
		push rax
		mov rax, [rbp-10056]; load GL_TRANSFORM_FEEDBACK_VARYING
		pop rcx
		mov [rbp-10056], rcx; store GL_TRANSFORM_FEEDBACK_VARYING
		mov rax, [rbp-10056]; load GL_TRANSFORM_FEEDBACK_VARYING
		mov rax, 37621
		push rax
		mov rax, [rbp-10064]; load GL_ACTIVE_RESOURCES
		pop rcx
		mov [rbp-10064], rcx; store GL_ACTIVE_RESOURCES
		mov rax, [rbp-10064]; load GL_ACTIVE_RESOURCES
		mov rax, 37622
		push rax
		mov rax, [rbp-10072]; load GL_MAX_NAME_LENGTH
		pop rcx
		mov [rbp-10072], rcx; store GL_MAX_NAME_LENGTH
		mov rax, [rbp-10072]; load GL_MAX_NAME_LENGTH
		mov rax, 37623
		push rax
		mov rax, [rbp-10080]; load GL_MAX_NUM_ACTIVE_VARIABLES
		pop rcx
		mov [rbp-10080], rcx; store GL_MAX_NUM_ACTIVE_VARIABLES
		mov rax, [rbp-10080]; load GL_MAX_NUM_ACTIVE_VARIABLES
		mov rax, 37624
		push rax
		mov rax, [rbp-10088]; load GL_MAX_NUM_COMPATIBLE_SUBROUTINES
		pop rcx
		mov [rbp-10088], rcx; store GL_MAX_NUM_COMPATIBLE_SUBROUTINES
		mov rax, [rbp-10088]; load GL_MAX_NUM_COMPATIBLE_SUBROUTINES
		mov rax, 37625
		push rax
		mov rax, [rbp-10096]; load GL_NAME_LENGTH
		pop rcx
		mov [rbp-10096], rcx; store GL_NAME_LENGTH
		mov rax, [rbp-10096]; load GL_NAME_LENGTH
		mov rax, 37626
		push rax
		mov rax, [rbp-10104]; load GL_TYPE
		pop rcx
		mov [rbp-10104], rcx; store GL_TYPE
		mov rax, [rbp-10104]; load GL_TYPE
		mov rax, 37627
		push rax
		mov rax, [rbp-10112]; load GL_ARRAY_SIZE
		pop rcx
		mov [rbp-10112], rcx; store GL_ARRAY_SIZE
		mov rax, [rbp-10112]; load GL_ARRAY_SIZE
		mov rax, 37628
		push rax
		mov rax, [rbp-10120]; load GL_OFFSET
		pop rcx
		mov [rbp-10120], rcx; store GL_OFFSET
		mov rax, [rbp-10120]; load GL_OFFSET
		mov rax, 37629
		push rax
		mov rax, [rbp-10128]; load GL_BLOCK_INDEX
		pop rcx
		mov [rbp-10128], rcx; store GL_BLOCK_INDEX
		mov rax, [rbp-10128]; load GL_BLOCK_INDEX
		mov rax, 37630
		push rax
		mov rax, [rbp-10136]; load GL_ARRAY_STRIDE
		pop rcx
		mov [rbp-10136], rcx; store GL_ARRAY_STRIDE
		mov rax, [rbp-10136]; load GL_ARRAY_STRIDE
		mov rax, 37631
		push rax
		mov rax, [rbp-10144]; load GL_MATRIX_STRIDE
		pop rcx
		mov [rbp-10144], rcx; store GL_MATRIX_STRIDE
		mov rax, [rbp-10144]; load GL_MATRIX_STRIDE
		mov rax, 37632
		push rax
		mov rax, [rbp-10152]; load GL_IS_ROW_MAJOR
		pop rcx
		mov [rbp-10152], rcx; store GL_IS_ROW_MAJOR
		mov rax, [rbp-10152]; load GL_IS_ROW_MAJOR
		mov rax, 37633
		push rax
		mov rax, [rbp-10160]; load GL_ATOMIC_COUNTER_BUFFER_INDEX
		pop rcx
		mov [rbp-10160], rcx; store GL_ATOMIC_COUNTER_BUFFER_INDEX
		mov rax, [rbp-10160]; load GL_ATOMIC_COUNTER_BUFFER_INDEX
		mov rax, 37634
		push rax
		mov rax, [rbp-10168]; load GL_BUFFER_BINDING
		pop rcx
		mov [rbp-10168], rcx; store GL_BUFFER_BINDING
		mov rax, [rbp-10168]; load GL_BUFFER_BINDING
		mov rax, 37635
		push rax
		mov rax, [rbp-10176]; load GL_BUFFER_DATA_SIZE
		pop rcx
		mov [rbp-10176], rcx; store GL_BUFFER_DATA_SIZE
		mov rax, [rbp-10176]; load GL_BUFFER_DATA_SIZE
		mov rax, 37636
		push rax
		mov rax, [rbp-10184]; load GL_NUM_ACTIVE_VARIABLES
		pop rcx
		mov [rbp-10184], rcx; store GL_NUM_ACTIVE_VARIABLES
		mov rax, [rbp-10184]; load GL_NUM_ACTIVE_VARIABLES
		mov rax, 37637
		push rax
		mov rax, [rbp-10192]; load GL_ACTIVE_VARIABLES
		pop rcx
		mov [rbp-10192], rcx; store GL_ACTIVE_VARIABLES
		mov rax, [rbp-10192]; load GL_ACTIVE_VARIABLES
		mov rax, 37638
		push rax
		mov rax, [rbp-10200]; load GL_REFERENCED_BY_VERTEX_SHADER
		pop rcx
		mov [rbp-10200], rcx; store GL_REFERENCED_BY_VERTEX_SHADER
		mov rax, [rbp-10200]; load GL_REFERENCED_BY_VERTEX_SHADER
		mov rax, 37639
		push rax
		mov rax, [rbp-10208]; load GL_REFERENCED_BY_TESS_CONTROL_SHADER
		pop rcx
		mov [rbp-10208], rcx; store GL_REFERENCED_BY_TESS_CONTROL_SHADER
		mov rax, [rbp-10208]; load GL_REFERENCED_BY_TESS_CONTROL_SHADER
		mov rax, 37640
		push rax
		mov rax, [rbp-10216]; load GL_REFERENCED_BY_TESS_EVALUATION_SHADER
		pop rcx
		mov [rbp-10216], rcx; store GL_REFERENCED_BY_TESS_EVALUATION_SHADER
		mov rax, [rbp-10216]; load GL_REFERENCED_BY_TESS_EVALUATION_SHADER
		mov rax, 37641
		push rax
		mov rax, [rbp-10224]; load GL_REFERENCED_BY_GEOMETRY_SHADER
		pop rcx
		mov [rbp-10224], rcx; store GL_REFERENCED_BY_GEOMETRY_SHADER
		mov rax, [rbp-10224]; load GL_REFERENCED_BY_GEOMETRY_SHADER
		mov rax, 37642
		push rax
		mov rax, [rbp-10232]; load GL_REFERENCED_BY_FRAGMENT_SHADER
		pop rcx
		mov [rbp-10232], rcx; store GL_REFERENCED_BY_FRAGMENT_SHADER
		mov rax, [rbp-10232]; load GL_REFERENCED_BY_FRAGMENT_SHADER
		mov rax, 37643
		push rax
		mov rax, [rbp-10240]; load GL_REFERENCED_BY_COMPUTE_SHADER
		pop rcx
		mov [rbp-10240], rcx; store GL_REFERENCED_BY_COMPUTE_SHADER
		mov rax, [rbp-10240]; load GL_REFERENCED_BY_COMPUTE_SHADER
		mov rax, 37644
		push rax
		mov rax, [rbp-10248]; load GL_TOP_LEVEL_ARRAY_SIZE
		pop rcx
		mov [rbp-10248], rcx; store GL_TOP_LEVEL_ARRAY_SIZE
		mov rax, [rbp-10248]; load GL_TOP_LEVEL_ARRAY_SIZE
		mov rax, 37645
		push rax
		mov rax, [rbp-10256]; load GL_TOP_LEVEL_ARRAY_STRIDE
		pop rcx
		mov [rbp-10256], rcx; store GL_TOP_LEVEL_ARRAY_STRIDE
		mov rax, [rbp-10256]; load GL_TOP_LEVEL_ARRAY_STRIDE
		mov rax, 37646
		push rax
		mov rax, [rbp-10264]; load GL_LOCATION
		pop rcx
		mov [rbp-10264], rcx; store GL_LOCATION
		mov rax, [rbp-10264]; load GL_LOCATION
		mov rax, 37647
		push rax
		mov rax, [rbp-10272]; load GL_LOCATION_INDEX
		pop rcx
		mov [rbp-10272], rcx; store GL_LOCATION_INDEX
		mov rax, [rbp-10272]; load GL_LOCATION_INDEX
		mov rax, 37607
		push rax
		mov rax, [rbp-10280]; load GL_IS_PER_PATCH
		pop rcx
		mov [rbp-10280], rcx; store GL_IS_PER_PATCH
		mov rax, [rbp-10280]; load GL_IS_PER_PATCH
		mov rax, 37074
		push rax
		mov rax, [rbp-10288]; load GL_SHADER_STORAGE_BUFFER
		pop rcx
		mov [rbp-10288], rcx; store GL_SHADER_STORAGE_BUFFER
		mov rax, [rbp-10288]; load GL_SHADER_STORAGE_BUFFER
		mov rax, 37075
		push rax
		mov rax, [rbp-10296]; load GL_SHADER_STORAGE_BUFFER_BINDING
		pop rcx
		mov [rbp-10296], rcx; store GL_SHADER_STORAGE_BUFFER_BINDING
		mov rax, [rbp-10296]; load GL_SHADER_STORAGE_BUFFER_BINDING
		mov rax, 37076
		push rax
		mov rax, [rbp-10304]; load GL_SHADER_STORAGE_BUFFER_START
		pop rcx
		mov [rbp-10304], rcx; store GL_SHADER_STORAGE_BUFFER_START
		mov rax, [rbp-10304]; load GL_SHADER_STORAGE_BUFFER_START
		mov rax, 37077
		push rax
		mov rax, [rbp-10312]; load GL_SHADER_STORAGE_BUFFER_SIZE
		pop rcx
		mov [rbp-10312], rcx; store GL_SHADER_STORAGE_BUFFER_SIZE
		mov rax, [rbp-10312]; load GL_SHADER_STORAGE_BUFFER_SIZE
		mov rax, 37078
		push rax
		mov rax, [rbp-10320]; load GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS
		pop rcx
		mov [rbp-10320], rcx; store GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS
		mov rax, [rbp-10320]; load GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS
		mov rax, 37079
		push rax
		mov rax, [rbp-10328]; load GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS
		pop rcx
		mov [rbp-10328], rcx; store GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS
		mov rax, [rbp-10328]; load GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS
		mov rax, 37080
		push rax
		mov rax, [rbp-10336]; load GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS
		pop rcx
		mov [rbp-10336], rcx; store GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS
		mov rax, [rbp-10336]; load GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS
		mov rax, 37081
		push rax
		mov rax, [rbp-10344]; load GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS
		pop rcx
		mov [rbp-10344], rcx; store GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS
		mov rax, [rbp-10344]; load GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS
		mov rax, 37082
		push rax
		mov rax, [rbp-10352]; load GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS
		pop rcx
		mov [rbp-10352], rcx; store GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS
		mov rax, [rbp-10352]; load GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS
		mov rax, 37083
		push rax
		mov rax, [rbp-10360]; load GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS
		pop rcx
		mov [rbp-10360], rcx; store GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS
		mov rax, [rbp-10360]; load GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS
		mov rax, 37084
		push rax
		mov rax, [rbp-10368]; load GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS
		pop rcx
		mov [rbp-10368], rcx; store GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS
		mov rax, [rbp-10368]; load GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS
		mov rax, 37085
		push rax
		mov rax, [rbp-10376]; load GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS
		pop rcx
		mov [rbp-10376], rcx; store GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS
		mov rax, [rbp-10376]; load GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS
		mov rax, 37086
		push rax
		mov rax, [rbp-10384]; load GL_MAX_SHADER_STORAGE_BLOCK_SIZE
		pop rcx
		mov [rbp-10384], rcx; store GL_MAX_SHADER_STORAGE_BLOCK_SIZE
		mov rax, [rbp-10384]; load GL_MAX_SHADER_STORAGE_BLOCK_SIZE
		mov rax, 37087
		push rax
		mov rax, [rbp-10392]; load GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT
		pop rcx
		mov [rbp-10392], rcx; store GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT
		mov rax, [rbp-10392]; load GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT
		mov rax, 8192
		push rax
		mov rax, [rbp-10400]; load GL_SHADER_STORAGE_BARRIER_BIT
		pop rcx
		mov [rbp-10400], rcx; store GL_SHADER_STORAGE_BARRIER_BIT
		mov rax, [rbp-10400]; load GL_SHADER_STORAGE_BARRIER_BIT
		mov rax, 36665
		push rax
		mov rax, [rbp-10408]; load GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES
		pop rcx
		mov [rbp-10408], rcx; store GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES
		mov rax, [rbp-10408]; load GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES
		mov rax, 37098
		push rax
		mov rax, [rbp-10416]; load GL_DEPTH_STENCIL_TEXTURE_MODE
		pop rcx
		mov [rbp-10416], rcx; store GL_DEPTH_STENCIL_TEXTURE_MODE
		mov rax, [rbp-10416]; load GL_DEPTH_STENCIL_TEXTURE_MODE
		mov rax, 37277
		push rax
		mov rax, [rbp-10424]; load GL_TEXTURE_BUFFER_OFFSET
		pop rcx
		mov [rbp-10424], rcx; store GL_TEXTURE_BUFFER_OFFSET
		mov rax, [rbp-10424]; load GL_TEXTURE_BUFFER_OFFSET
		mov rax, 37278
		push rax
		mov rax, [rbp-10432]; load GL_TEXTURE_BUFFER_SIZE
		pop rcx
		mov [rbp-10432], rcx; store GL_TEXTURE_BUFFER_SIZE
		mov rax, [rbp-10432]; load GL_TEXTURE_BUFFER_SIZE
		mov rax, 37279
		push rax
		mov rax, [rbp-10440]; load GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT
		pop rcx
		mov [rbp-10440], rcx; store GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT
		mov rax, [rbp-10440]; load GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT
		mov rax, 33499
		push rax
		mov rax, [rbp-10448]; load GL_TEXTURE_VIEW_MIN_LEVEL
		pop rcx
		mov [rbp-10448], rcx; store GL_TEXTURE_VIEW_MIN_LEVEL
		mov rax, [rbp-10448]; load GL_TEXTURE_VIEW_MIN_LEVEL
		mov rax, 33500
		push rax
		mov rax, [rbp-10456]; load GL_TEXTURE_VIEW_NUM_LEVELS
		pop rcx
		mov [rbp-10456], rcx; store GL_TEXTURE_VIEW_NUM_LEVELS
		mov rax, [rbp-10456]; load GL_TEXTURE_VIEW_NUM_LEVELS
		mov rax, 33501
		push rax
		mov rax, [rbp-10464]; load GL_TEXTURE_VIEW_MIN_LAYER
		pop rcx
		mov [rbp-10464], rcx; store GL_TEXTURE_VIEW_MIN_LAYER
		mov rax, [rbp-10464]; load GL_TEXTURE_VIEW_MIN_LAYER
		mov rax, 33502
		push rax
		mov rax, [rbp-10472]; load GL_TEXTURE_VIEW_NUM_LAYERS
		pop rcx
		mov [rbp-10472], rcx; store GL_TEXTURE_VIEW_NUM_LAYERS
		mov rax, [rbp-10472]; load GL_TEXTURE_VIEW_NUM_LAYERS
		mov rax, 33503
		push rax
		mov rax, [rbp-10480]; load GL_TEXTURE_IMMUTABLE_LEVELS
		pop rcx
		mov [rbp-10480], rcx; store GL_TEXTURE_IMMUTABLE_LEVELS
		mov rax, [rbp-10480]; load GL_TEXTURE_IMMUTABLE_LEVELS
		mov rax, 33492
		push rax
		mov rax, [rbp-10488]; load GL_VERTEX_ATTRIB_BINDING
		pop rcx
		mov [rbp-10488], rcx; store GL_VERTEX_ATTRIB_BINDING
		mov rax, [rbp-10488]; load GL_VERTEX_ATTRIB_BINDING
		mov rax, 33493
		push rax
		mov rax, [rbp-10496]; load GL_VERTEX_ATTRIB_RELATIVE_OFFSET
		pop rcx
		mov [rbp-10496], rcx; store GL_VERTEX_ATTRIB_RELATIVE_OFFSET
		mov rax, [rbp-10496]; load GL_VERTEX_ATTRIB_RELATIVE_OFFSET
		mov rax, 33494
		push rax
		mov rax, [rbp-10504]; load GL_VERTEX_BINDING_DIVISOR
		pop rcx
		mov [rbp-10504], rcx; store GL_VERTEX_BINDING_DIVISOR
		mov rax, [rbp-10504]; load GL_VERTEX_BINDING_DIVISOR
		mov rax, 33495
		push rax
		mov rax, [rbp-10512]; load GL_VERTEX_BINDING_OFFSET
		pop rcx
		mov [rbp-10512], rcx; store GL_VERTEX_BINDING_OFFSET
		mov rax, [rbp-10512]; load GL_VERTEX_BINDING_OFFSET
		mov rax, 33496
		push rax
		mov rax, [rbp-10520]; load GL_VERTEX_BINDING_STRIDE
		pop rcx
		mov [rbp-10520], rcx; store GL_VERTEX_BINDING_STRIDE
		mov rax, [rbp-10520]; load GL_VERTEX_BINDING_STRIDE
		mov rax, 33497
		push rax
		mov rax, [rbp-10528]; load GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET
		pop rcx
		mov [rbp-10528], rcx; store GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET
		mov rax, [rbp-10528]; load GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET
		mov rax, 33498
		push rax
		mov rax, [rbp-10536]; load GL_MAX_VERTEX_ATTRIB_BINDINGS
		pop rcx
		mov [rbp-10536], rcx; store GL_MAX_VERTEX_ATTRIB_BINDINGS
		mov rax, [rbp-10536]; load GL_MAX_VERTEX_ATTRIB_BINDINGS
		mov rax, 36687
		push rax
		mov rax, [rbp-10544]; load GL_VERTEX_BINDING_BUFFER
		pop rcx
		mov [rbp-10544], rcx; store GL_VERTEX_BINDING_BUFFER
		mov rax, [rbp-10544]; load GL_VERTEX_BINDING_BUFFER
		mov rax, 33509
		push rax
		mov rax, [rbp-10552]; load GL_MAX_VERTEX_ATTRIB_STRIDE
		pop rcx
		mov [rbp-10552], rcx; store GL_MAX_VERTEX_ATTRIB_STRIDE
		mov rax, [rbp-10552]; load GL_MAX_VERTEX_ATTRIB_STRIDE
		mov rax, 33313
		push rax
		mov rax, [rbp-10560]; load GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED
		pop rcx
		mov [rbp-10560], rcx; store GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED
		mov rax, [rbp-10560]; load GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED
		mov rax, 35882
		push rax
		mov rax, [rbp-10568]; load GL_TEXTURE_BUFFER_BINDING
		pop rcx
		mov [rbp-10568], rcx; store GL_TEXTURE_BUFFER_BINDING
		mov rax, [rbp-10568]; load GL_TEXTURE_BUFFER_BINDING
		mov rax, 64
		push rax
		mov rax, [rbp-10576]; load GL_MAP_PERSISTENT_BIT
		pop rcx
		mov [rbp-10576], rcx; store GL_MAP_PERSISTENT_BIT
		mov rax, [rbp-10576]; load GL_MAP_PERSISTENT_BIT
		mov rax, 128
		push rax
		mov rax, [rbp-10584]; load GL_MAP_COHERENT_BIT
		pop rcx
		mov [rbp-10584], rcx; store GL_MAP_COHERENT_BIT
		mov rax, [rbp-10584]; load GL_MAP_COHERENT_BIT
		mov rax, 256
		push rax
		mov rax, [rbp-10592]; load GL_DYNAMIC_STORAGE_BIT
		pop rcx
		mov [rbp-10592], rcx; store GL_DYNAMIC_STORAGE_BIT
		mov rax, [rbp-10592]; load GL_DYNAMIC_STORAGE_BIT
		mov rax, 512
		push rax
		mov rax, [rbp-10600]; load GL_CLIENT_STORAGE_BIT
		pop rcx
		mov [rbp-10600], rcx; store GL_CLIENT_STORAGE_BIT
		mov rax, [rbp-10600]; load GL_CLIENT_STORAGE_BIT
		mov rax, 16384
		push rax
		mov rax, [rbp-10608]; load GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT
		pop rcx
		mov [rbp-10608], rcx; store GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT
		mov rax, [rbp-10608]; load GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT
		mov rax, 33311
		push rax
		mov rax, [rbp-10616]; load GL_BUFFER_IMMUTABLE_STORAGE
		pop rcx
		mov [rbp-10616], rcx; store GL_BUFFER_IMMUTABLE_STORAGE
		mov rax, [rbp-10616]; load GL_BUFFER_IMMUTABLE_STORAGE
		mov rax, 33312
		push rax
		mov rax, [rbp-10624]; load GL_BUFFER_STORAGE_FLAGS
		pop rcx
		mov [rbp-10624], rcx; store GL_BUFFER_STORAGE_FLAGS
		mov rax, [rbp-10624]; load GL_BUFFER_STORAGE_FLAGS
		mov rax, 37733
		push rax
		mov rax, [rbp-10632]; load GL_CLEAR_TEXTURE
		pop rcx
		mov [rbp-10632], rcx; store GL_CLEAR_TEXTURE
		mov rax, [rbp-10632]; load GL_CLEAR_TEXTURE
		mov rax, 37706
		push rax
		mov rax, [rbp-10640]; load GL_LOCATION_COMPONENT
		pop rcx
		mov [rbp-10640], rcx; store GL_LOCATION_COMPONENT
		mov rax, [rbp-10640]; load GL_LOCATION_COMPONENT
		mov rax, 37707
		push rax
		mov rax, [rbp-10648]; load GL_TRANSFORM_FEEDBACK_BUFFER_INDEX
		pop rcx
		mov [rbp-10648], rcx; store GL_TRANSFORM_FEEDBACK_BUFFER_INDEX
		mov rax, [rbp-10648]; load GL_TRANSFORM_FEEDBACK_BUFFER_INDEX
		mov rax, 37708
		push rax
		mov rax, [rbp-10656]; load GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE
		pop rcx
		mov [rbp-10656], rcx; store GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE
		mov rax, [rbp-10656]; load GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE
		mov rax, 37266
		push rax
		mov rax, [rbp-10664]; load GL_QUERY_BUFFER
		pop rcx
		mov [rbp-10664], rcx; store GL_QUERY_BUFFER
		mov rax, [rbp-10664]; load GL_QUERY_BUFFER
		mov rax, 32768
		push rax
		mov rax, [rbp-10672]; load GL_QUERY_BUFFER_BARRIER_BIT
		pop rcx
		mov [rbp-10672], rcx; store GL_QUERY_BUFFER_BARRIER_BIT
		mov rax, [rbp-10672]; load GL_QUERY_BUFFER_BARRIER_BIT
		mov rax, 37267
		push rax
		mov rax, [rbp-10680]; load GL_QUERY_BUFFER_BINDING
		pop rcx
		mov [rbp-10680], rcx; store GL_QUERY_BUFFER_BINDING
		mov rax, [rbp-10680]; load GL_QUERY_BUFFER_BINDING
		mov rax, 37268
		push rax
		mov rax, [rbp-10688]; load GL_QUERY_RESULT_NO_WAIT
		pop rcx
		mov [rbp-10688], rcx; store GL_QUERY_RESULT_NO_WAIT
		mov rax, [rbp-10688]; load GL_QUERY_RESULT_NO_WAIT
		mov rax, 34627
		push rax
		mov rax, [rbp-10696]; load GL_MIRROR_CLAMP_TO_EDGE
		pop rcx
		mov [rbp-10696], rcx; store GL_MIRROR_CLAMP_TO_EDGE
		mov rax, [rbp-10696]; load GL_MIRROR_CLAMP_TO_EDGE
		mov rax, 1287
		push rax
		mov rax, [rbp-10704]; load GL_CONTEXT_LOST
		pop rcx
		mov [rbp-10704], rcx; store GL_CONTEXT_LOST
		mov rax, [rbp-10704]; load GL_CONTEXT_LOST
		mov rax, 37726
		push rax
		mov rax, [rbp-10712]; load GL_NEGATIVE_ONE_TO_ONE
		pop rcx
		mov [rbp-10712], rcx; store GL_NEGATIVE_ONE_TO_ONE
		mov rax, [rbp-10712]; load GL_NEGATIVE_ONE_TO_ONE
		mov rax, 37727
		push rax
		mov rax, [rbp-10720]; load GL_ZERO_TO_ONE
		pop rcx
		mov [rbp-10720], rcx; store GL_ZERO_TO_ONE
		mov rax, [rbp-10720]; load GL_ZERO_TO_ONE
		mov rax, 37724
		push rax
		mov rax, [rbp-10728]; load GL_CLIP_ORIGIN
		pop rcx
		mov [rbp-10728], rcx; store GL_CLIP_ORIGIN
		mov rax, [rbp-10728]; load GL_CLIP_ORIGIN
		mov rax, 37725
		push rax
		mov rax, [rbp-10736]; load GL_CLIP_DEPTH_MODE
		pop rcx
		mov [rbp-10736], rcx; store GL_CLIP_DEPTH_MODE
		mov rax, [rbp-10736]; load GL_CLIP_DEPTH_MODE
		mov rax, 36375
		push rax
		mov rax, [rbp-10744]; load GL_QUERY_WAIT_INVERTED
		pop rcx
		mov [rbp-10744], rcx; store GL_QUERY_WAIT_INVERTED
		mov rax, [rbp-10744]; load GL_QUERY_WAIT_INVERTED
		mov rax, 36376
		push rax
		mov rax, [rbp-10752]; load GL_QUERY_NO_WAIT_INVERTED
		pop rcx
		mov [rbp-10752], rcx; store GL_QUERY_NO_WAIT_INVERTED
		mov rax, [rbp-10752]; load GL_QUERY_NO_WAIT_INVERTED
		mov rax, 36377
		push rax
		mov rax, [rbp-10760]; load GL_QUERY_BY_REGION_WAIT_INVERTED
		pop rcx
		mov [rbp-10760], rcx; store GL_QUERY_BY_REGION_WAIT_INVERTED
		mov rax, [rbp-10760]; load GL_QUERY_BY_REGION_WAIT_INVERTED
		mov rax, 36378
		push rax
		mov rax, [rbp-10768]; load GL_QUERY_BY_REGION_NO_WAIT_INVERTED
		pop rcx
		mov [rbp-10768], rcx; store GL_QUERY_BY_REGION_NO_WAIT_INVERTED
		mov rax, [rbp-10768]; load GL_QUERY_BY_REGION_NO_WAIT_INVERTED
		mov rax, 33529
		push rax
		mov rax, [rbp-10776]; load GL_MAX_CULL_DISTANCES
		pop rcx
		mov [rbp-10776], rcx; store GL_MAX_CULL_DISTANCES
		mov rax, [rbp-10776]; load GL_MAX_CULL_DISTANCES
		mov rax, 33530
		push rax
		mov rax, [rbp-10784]; load GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES
		pop rcx
		mov [rbp-10784], rcx; store GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES
		mov rax, [rbp-10784]; load GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES
		mov rax, 4102
		push rax
		mov rax, [rbp-10792]; load GL_TEXTURE_TARGET
		pop rcx
		mov [rbp-10792], rcx; store GL_TEXTURE_TARGET
		mov rax, [rbp-10792]; load GL_TEXTURE_TARGET
		mov rax, 33514
		push rax
		mov rax, [rbp-10800]; load GL_QUERY_TARGET
		pop rcx
		mov [rbp-10800], rcx; store GL_QUERY_TARGET
		mov rax, [rbp-10800]; load GL_QUERY_TARGET
		mov rax, 33363
		push rax
		mov rax, [rbp-10808]; load GL_GUILTY_CONTEXT_RESET
		pop rcx
		mov [rbp-10808], rcx; store GL_GUILTY_CONTEXT_RESET
		mov rax, [rbp-10808]; load GL_GUILTY_CONTEXT_RESET
		mov rax, 33364
		push rax
		mov rax, [rbp-10816]; load GL_INNOCENT_CONTEXT_RESET
		pop rcx
		mov [rbp-10816], rcx; store GL_INNOCENT_CONTEXT_RESET
		mov rax, [rbp-10816]; load GL_INNOCENT_CONTEXT_RESET
		mov rax, 33365
		push rax
		mov rax, [rbp-10824]; load GL_UNKNOWN_CONTEXT_RESET
		pop rcx
		mov [rbp-10824], rcx; store GL_UNKNOWN_CONTEXT_RESET
		mov rax, [rbp-10824]; load GL_UNKNOWN_CONTEXT_RESET
		mov rax, 33366
		push rax
		mov rax, [rbp-10832]; load GL_RESET_NOTIFICATION_STRATEGY
		pop rcx
		mov [rbp-10832], rcx; store GL_RESET_NOTIFICATION_STRATEGY
		mov rax, [rbp-10832]; load GL_RESET_NOTIFICATION_STRATEGY
		mov rax, 33362
		push rax
		mov rax, [rbp-10840]; load GL_LOSE_CONTEXT_ON_RESET
		pop rcx
		mov [rbp-10840], rcx; store GL_LOSE_CONTEXT_ON_RESET
		mov rax, [rbp-10840]; load GL_LOSE_CONTEXT_ON_RESET
		mov rax, 33377
		push rax
		mov rax, [rbp-10848]; load GL_NO_RESET_NOTIFICATION
		pop rcx
		mov [rbp-10848], rcx; store GL_NO_RESET_NOTIFICATION
		mov rax, [rbp-10848]; load GL_NO_RESET_NOTIFICATION
		mov rax, 4
		push rax
		mov rax, [rbp-10856]; load GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT
		pop rcx
		mov [rbp-10856], rcx; store GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT
		mov rax, [rbp-10856]; load GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT
		mov rax, 33531
		push rax
		mov rax, [rbp-10864]; load GL_CONTEXT_RELEASE_BEHAVIOR
		pop rcx
		mov [rbp-10864], rcx; store GL_CONTEXT_RELEASE_BEHAVIOR
		mov rax, [rbp-10864]; load GL_CONTEXT_RELEASE_BEHAVIOR
		mov rax, 33532
		push rax
		mov rax, [rbp-10872]; load GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH
		pop rcx
		mov [rbp-10872], rcx; store GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH
		mov rax, [rbp-10872]; load GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH
		mov rax, 33346
		push rax
		mov rax, [rbp-10880]; load GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB
		pop rcx
		mov [rbp-10880], rcx; store GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB
		mov rax, [rbp-10880]; load GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB
		mov rax, 33347
		push rax
		mov rax, [rbp-10888]; load GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB
		pop rcx
		mov [rbp-10888], rcx; store GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB
		mov rax, [rbp-10888]; load GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB
		mov rax, 33348
		push rax
		mov rax, [rbp-10896]; load GL_DEBUG_CALLBACK_FUNCTION_ARB
		pop rcx
		mov [rbp-10896], rcx; store GL_DEBUG_CALLBACK_FUNCTION_ARB
		mov rax, [rbp-10896]; load GL_DEBUG_CALLBACK_FUNCTION_ARB
		mov rax, 33349
		push rax
		mov rax, [rbp-10904]; load GL_DEBUG_CALLBACK_USER_PARAM_ARB
		pop rcx
		mov [rbp-10904], rcx; store GL_DEBUG_CALLBACK_USER_PARAM_ARB
		mov rax, [rbp-10904]; load GL_DEBUG_CALLBACK_USER_PARAM_ARB
		mov rax, 33350
		push rax
		mov rax, [rbp-10912]; load GL_DEBUG_SOURCE_API_ARB
		pop rcx
		mov [rbp-10912], rcx; store GL_DEBUG_SOURCE_API_ARB
		mov rax, [rbp-10912]; load GL_DEBUG_SOURCE_API_ARB
		mov rax, 33351
		push rax
		mov rax, [rbp-10920]; load GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB
		pop rcx
		mov [rbp-10920], rcx; store GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB
		mov rax, [rbp-10920]; load GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB
		mov rax, 33352
		push rax
		mov rax, [rbp-10928]; load GL_DEBUG_SOURCE_SHADER_COMPILER_ARB
		pop rcx
		mov [rbp-10928], rcx; store GL_DEBUG_SOURCE_SHADER_COMPILER_ARB
		mov rax, [rbp-10928]; load GL_DEBUG_SOURCE_SHADER_COMPILER_ARB
		mov rax, 33353
		push rax
		mov rax, [rbp-10936]; load GL_DEBUG_SOURCE_THIRD_PARTY_ARB
		pop rcx
		mov [rbp-10936], rcx; store GL_DEBUG_SOURCE_THIRD_PARTY_ARB
		mov rax, [rbp-10936]; load GL_DEBUG_SOURCE_THIRD_PARTY_ARB
		mov rax, 33354
		push rax
		mov rax, [rbp-10944]; load GL_DEBUG_SOURCE_APPLICATION_ARB
		pop rcx
		mov [rbp-10944], rcx; store GL_DEBUG_SOURCE_APPLICATION_ARB
		mov rax, [rbp-10944]; load GL_DEBUG_SOURCE_APPLICATION_ARB
		mov rax, 33355
		push rax
		mov rax, [rbp-10952]; load GL_DEBUG_SOURCE_OTHER_ARB
		pop rcx
		mov [rbp-10952], rcx; store GL_DEBUG_SOURCE_OTHER_ARB
		mov rax, [rbp-10952]; load GL_DEBUG_SOURCE_OTHER_ARB
		mov rax, 33356
		push rax
		mov rax, [rbp-10960]; load GL_DEBUG_TYPE_ERROR_ARB
		pop rcx
		mov [rbp-10960], rcx; store GL_DEBUG_TYPE_ERROR_ARB
		mov rax, [rbp-10960]; load GL_DEBUG_TYPE_ERROR_ARB
		mov rax, 33357
		push rax
		mov rax, [rbp-10968]; load GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB
		pop rcx
		mov [rbp-10968], rcx; store GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB
		mov rax, [rbp-10968]; load GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB
		mov rax, 33358
		push rax
		mov rax, [rbp-10976]; load GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB
		pop rcx
		mov [rbp-10976], rcx; store GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB
		mov rax, [rbp-10976]; load GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB
		mov rax, 33359
		push rax
		mov rax, [rbp-10984]; load GL_DEBUG_TYPE_PORTABILITY_ARB
		pop rcx
		mov [rbp-10984], rcx; store GL_DEBUG_TYPE_PORTABILITY_ARB
		mov rax, [rbp-10984]; load GL_DEBUG_TYPE_PORTABILITY_ARB
		mov rax, 33360
		push rax
		mov rax, [rbp-10992]; load GL_DEBUG_TYPE_PERFORMANCE_ARB
		pop rcx
		mov [rbp-10992], rcx; store GL_DEBUG_TYPE_PERFORMANCE_ARB
		mov rax, [rbp-10992]; load GL_DEBUG_TYPE_PERFORMANCE_ARB
		mov rax, 33361
		push rax
		mov rax, [rbp-11000]; load GL_DEBUG_TYPE_OTHER_ARB
		pop rcx
		mov [rbp-11000], rcx; store GL_DEBUG_TYPE_OTHER_ARB
		mov rax, [rbp-11000]; load GL_DEBUG_TYPE_OTHER_ARB
		mov rax, 37187
		push rax
		mov rax, [rbp-11008]; load GL_MAX_DEBUG_MESSAGE_LENGTH_ARB
		pop rcx
		mov [rbp-11008], rcx; store GL_MAX_DEBUG_MESSAGE_LENGTH_ARB
		mov rax, [rbp-11008]; load GL_MAX_DEBUG_MESSAGE_LENGTH_ARB
		mov rax, 37188
		push rax
		mov rax, [rbp-11016]; load GL_MAX_DEBUG_LOGGED_MESSAGES_ARB
		pop rcx
		mov [rbp-11016], rcx; store GL_MAX_DEBUG_LOGGED_MESSAGES_ARB
		mov rax, [rbp-11016]; load GL_MAX_DEBUG_LOGGED_MESSAGES_ARB
		mov rax, 37189
		push rax
		mov rax, [rbp-11024]; load GL_DEBUG_LOGGED_MESSAGES_ARB
		pop rcx
		mov [rbp-11024], rcx; store GL_DEBUG_LOGGED_MESSAGES_ARB
		mov rax, [rbp-11024]; load GL_DEBUG_LOGGED_MESSAGES_ARB
		mov rax, 37190
		push rax
		mov rax, [rbp-11032]; load GL_DEBUG_SEVERITY_HIGH_ARB
		pop rcx
		mov [rbp-11032], rcx; store GL_DEBUG_SEVERITY_HIGH_ARB
		mov rax, [rbp-11032]; load GL_DEBUG_SEVERITY_HIGH_ARB
		mov rax, 37191
		push rax
		mov rax, [rbp-11040]; load GL_DEBUG_SEVERITY_MEDIUM_ARB
		pop rcx
		mov [rbp-11040], rcx; store GL_DEBUG_SEVERITY_MEDIUM_ARB
		mov rax, [rbp-11040]; load GL_DEBUG_SEVERITY_MEDIUM_ARB
		mov rax, 37192
		push rax
		mov rax, [rbp-11048]; load GL_DEBUG_SEVERITY_LOW_ARB
		pop rcx
		mov [rbp-11048], rcx; store GL_DEBUG_SEVERITY_LOW_ARB
		mov rax, [rbp-11048]; load GL_DEBUG_SEVERITY_LOW_ARB
		mov rax, 38225
		push rax
		mov rax, [rbp-11056]; load GL_SHADER_BINARY_FORMAT_SPIR_V
		pop rcx
		mov [rbp-11056], rcx; store GL_SHADER_BINARY_FORMAT_SPIR_V
		mov rax, [rbp-11056]; load GL_SHADER_BINARY_FORMAT_SPIR_V
		mov rax, 38226
		push rax
		mov rax, [rbp-11064]; load GL_SPIR_V_BINARY
		pop rcx
		mov [rbp-11064], rcx; store GL_SPIR_V_BINARY
		mov rax, [rbp-11064]; load GL_SPIR_V_BINARY
		mov rax, 33006
		push rax
		mov rax, [rbp-11072]; load GL_PARAMETER_BUFFER
		pop rcx
		mov [rbp-11072], rcx; store GL_PARAMETER_BUFFER
		mov rax, [rbp-11072]; load GL_PARAMETER_BUFFER
		mov rax, 33007
		push rax
		mov rax, [rbp-11080]; load GL_PARAMETER_BUFFER_BINDING
		pop rcx
		mov [rbp-11080], rcx; store GL_PARAMETER_BUFFER_BINDING
		mov rax, [rbp-11080]; load GL_PARAMETER_BUFFER_BINDING
		mov rax, 8
		push rax
		mov rax, [rbp-11088]; load GL_CONTEXT_FLAG_NO_ERROR_BIT
		pop rcx
		mov [rbp-11088], rcx; store GL_CONTEXT_FLAG_NO_ERROR_BIT
		mov rax, [rbp-11088]; load GL_CONTEXT_FLAG_NO_ERROR_BIT
		mov rax, 33518
		push rax
		mov rax, [rbp-11096]; load GL_VERTICES_SUBMITTED
		pop rcx
		mov [rbp-11096], rcx; store GL_VERTICES_SUBMITTED
		mov rax, [rbp-11096]; load GL_VERTICES_SUBMITTED
		mov rax, 33519
		push rax
		mov rax, [rbp-11104]; load GL_PRIMITIVES_SUBMITTED
		pop rcx
		mov [rbp-11104], rcx; store GL_PRIMITIVES_SUBMITTED
		mov rax, [rbp-11104]; load GL_PRIMITIVES_SUBMITTED
		mov rax, 33520
		push rax
		mov rax, [rbp-11112]; load GL_VERTEX_SHADER_INVOCATIONS
		pop rcx
		mov [rbp-11112], rcx; store GL_VERTEX_SHADER_INVOCATIONS
		mov rax, [rbp-11112]; load GL_VERTEX_SHADER_INVOCATIONS
		mov rax, 33521
		push rax
		mov rax, [rbp-11120]; load GL_TESS_CONTROL_SHADER_PATCHES
		pop rcx
		mov [rbp-11120], rcx; store GL_TESS_CONTROL_SHADER_PATCHES
		mov rax, [rbp-11120]; load GL_TESS_CONTROL_SHADER_PATCHES
		mov rax, 33522
		push rax
		mov rax, [rbp-11128]; load GL_TESS_EVALUATION_SHADER_INVOCATIONS
		pop rcx
		mov [rbp-11128], rcx; store GL_TESS_EVALUATION_SHADER_INVOCATIONS
		mov rax, [rbp-11128]; load GL_TESS_EVALUATION_SHADER_INVOCATIONS
		mov rax, 33523
		push rax
		mov rax, [rbp-11136]; load GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED
		pop rcx
		mov [rbp-11136], rcx; store GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED
		mov rax, [rbp-11136]; load GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED
		mov rax, 33524
		push rax
		mov rax, [rbp-11144]; load GL_FRAGMENT_SHADER_INVOCATIONS
		pop rcx
		mov [rbp-11144], rcx; store GL_FRAGMENT_SHADER_INVOCATIONS
		mov rax, [rbp-11144]; load GL_FRAGMENT_SHADER_INVOCATIONS
		mov rax, 33525
		push rax
		mov rax, [rbp-11152]; load GL_COMPUTE_SHADER_INVOCATIONS
		pop rcx
		mov [rbp-11152], rcx; store GL_COMPUTE_SHADER_INVOCATIONS
		mov rax, [rbp-11152]; load GL_COMPUTE_SHADER_INVOCATIONS
		mov rax, 33526
		push rax
		mov rax, [rbp-11160]; load GL_CLIPPING_INPUT_PRIMITIVES
		pop rcx
		mov [rbp-11160], rcx; store GL_CLIPPING_INPUT_PRIMITIVES
		mov rax, [rbp-11160]; load GL_CLIPPING_INPUT_PRIMITIVES
		mov rax, 33527
		push rax
		mov rax, [rbp-11168]; load GL_CLIPPING_OUTPUT_PRIMITIVES
		pop rcx
		mov [rbp-11168], rcx; store GL_CLIPPING_OUTPUT_PRIMITIVES
		mov rax, [rbp-11168]; load GL_CLIPPING_OUTPUT_PRIMITIVES
		mov rax, 36379
		push rax
		mov rax, [rbp-11176]; load GL_POLYGON_OFFSET_CLAMP
		pop rcx
		mov [rbp-11176], rcx; store GL_POLYGON_OFFSET_CLAMP
		mov rax, [rbp-11176]; load GL_POLYGON_OFFSET_CLAMP
		mov rax, 38227
		push rax
		mov rax, [rbp-11184]; load GL_SPIR_V_EXTENSIONS
		pop rcx
		mov [rbp-11184], rcx; store GL_SPIR_V_EXTENSIONS
		mov rax, [rbp-11184]; load GL_SPIR_V_EXTENSIONS
		mov rax, 38228
		push rax
		mov rax, [rbp-11192]; load GL_NUM_SPIR_V_EXTENSIONS
		pop rcx
		mov [rbp-11192], rcx; store GL_NUM_SPIR_V_EXTENSIONS
		mov rax, [rbp-11192]; load GL_NUM_SPIR_V_EXTENSIONS
		mov rax, 34046
		push rax
		mov rax, [rbp-11200]; load GL_TEXTURE_MAX_ANISOTROPY
		pop rcx
		mov [rbp-11200], rcx; store GL_TEXTURE_MAX_ANISOTROPY
		mov rax, [rbp-11200]; load GL_TEXTURE_MAX_ANISOTROPY
		mov rax, 34047
		push rax
		mov rax, [rbp-11208]; load GL_MAX_TEXTURE_MAX_ANISOTROPY
		pop rcx
		mov [rbp-11208], rcx; store GL_MAX_TEXTURE_MAX_ANISOTROPY
		mov rax, [rbp-11208]; load GL_MAX_TEXTURE_MAX_ANISOTROPY
		mov rax, 33516
		push rax
		mov rax, [rbp-11216]; load GL_TRANSFORM_FEEDBACK_OVERFLOW
		pop rcx
		mov [rbp-11216], rcx; store GL_TRANSFORM_FEEDBACK_OVERFLOW
		mov rax, [rbp-11216]; load GL_TRANSFORM_FEEDBACK_OVERFLOW
		mov rax, 33517
		push rax
		mov rax, [rbp-11224]; load GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW
		pop rcx
		mov [rbp-11224], rcx; store GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW
		mov rax, [rbp-11224]; load GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW
		mov rax, 0
		push rax
		mov rax, [rbp-11232]; load GLEW_OK
		pop rcx
		mov [rbp-11232], rcx; store GLEW_OK
		mov rax, [rbp-11232]; load GLEW_OK
		mov rax, 0
		push rax
		mov rax, [rbp-11240]; load GLEW_NO_ERROR
		pop rcx
		mov [rbp-11240], rcx; store GLEW_NO_ERROR
		mov rax, [rbp-11240]; load GLEW_NO_ERROR
		mov rax, 1
		push rax
		mov rax, [rbp-11248]; load GLEW_ERROR_NO_GL_VERSION
		pop rcx
		mov [rbp-11248], rcx; store GLEW_ERROR_NO_GL_VERSION
		mov rax, [rbp-11248]; load GLEW_ERROR_NO_GL_VERSION
		mov rax, 2
		push rax
		mov rax, [rbp-11256]; load GLEW_ERROR_GL_VERSION_10_ONLY
		pop rcx
		mov [rbp-11256], rcx; store GLEW_ERROR_GL_VERSION_10_ONLY
		mov rax, [rbp-11256]; load GLEW_ERROR_GL_VERSION_10_ONLY
		mov rax, 3
		push rax
		mov rax, [rbp-11264]; load GLEW_ERROR_GLX_VERSION_11_ONLY
		pop rcx
		mov [rbp-11264], rcx; store GLEW_ERROR_GLX_VERSION_11_ONLY
		mov rax, [rbp-11264]; load GLEW_ERROR_GLX_VERSION_11_ONLY
		mov rax, 1
		push rax
		mov rax, [rbp-11272]; load GLEW_VERSION
		pop rcx
		mov [rbp-11272], rcx; store GLEW_VERSION
		mov rax, [rbp-11272]; load GLEW_VERSION
		mov rax, 2
		push rax
		mov rax, [rbp-11280]; load GLEW_VERSION_MAJOR
		pop rcx
		mov [rbp-11280], rcx; store GLEW_VERSION_MAJOR
		mov rax, [rbp-11280]; load GLEW_VERSION_MAJOR
		mov rax, 3
		push rax
		mov rax, [rbp-11288]; load GLEW_VERSION_MINOR
		pop rcx
		mov [rbp-11288], rcx; store GLEW_VERSION_MINOR
		mov rax, [rbp-11288]; load GLEW_VERSION_MINOR
		mov rax, 4
		push rax
		mov rax, [rbp-11296]; load GLEW_VERSION_MICRO
		pop rcx
		mov [rbp-11296], rcx; store GLEW_VERSION_MICRO
		mov rax, [rbp-11296]; load GLEW_VERSION_MICRO
		mov rax, 3
		push rax
		mov rax, [rbp-11304]; load GLFW_VERSION_MAJOR
		pop rcx
		mov [rbp-11304], rcx; store GLFW_VERSION_MAJOR
		mov rax, [rbp-11304]; load GLFW_VERSION_MAJOR
		mov rax, 2
		push rax
		mov rax, [rbp-11312]; load GLFW_VERSION_MINOR
		pop rcx
		mov [rbp-11312], rcx; store GLFW_VERSION_MINOR
		mov rax, [rbp-11312]; load GLFW_VERSION_MINOR
		mov rax, 1
		push rax
		mov rax, [rbp-11320]; load GLFW_VERSION_REVISION
		pop rcx
		mov [rbp-11320], rcx; store GLFW_VERSION_REVISION
		mov rax, [rbp-11320]; load GLFW_VERSION_REVISION
		mov rax, 1
		push rax
		mov rax, [rbp-11328]; load GLFW_TRUE
		pop rcx
		mov [rbp-11328], rcx; store GLFW_TRUE
		mov rax, [rbp-11328]; load GLFW_TRUE
		mov rax, 0
		push rax
		mov rax, [rbp-11336]; load GLFW_FALSE
		pop rcx
		mov [rbp-11336], rcx; store GLFW_FALSE
		mov rax, [rbp-11336]; load GLFW_FALSE
		mov rax, 0
		push rax
		mov rax, [rbp-11344]; load GLFW_RELEASE
		pop rcx
		mov [rbp-11344], rcx; store GLFW_RELEASE
		mov rax, [rbp-11344]; load GLFW_RELEASE
		mov rax, 1
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		mov [rbp-11352], rcx; store GLFW_PRESS
		mov rax, [rbp-11352]; load GLFW_PRESS
		mov rax, 2
		push rax
		mov rax, [rbp-11360]; load GLFW_REPEAT
		pop rcx
		mov [rbp-11360], rcx; store GLFW_REPEAT
		mov rax, [rbp-11360]; load GLFW_REPEAT
		mov rax, -1
		push rax
		mov rax, [rbp-11368]; load GLFW_KEY_UNKNOWN
		pop rcx
		mov [rbp-11368], rcx; store GLFW_KEY_UNKNOWN
		mov rax, [rbp-11368]; load GLFW_KEY_UNKNOWN
		mov rax, 32
		push rax
		mov rax, [rbp-11376]; load GLFW_KEY_SPACE
		pop rcx
		mov [rbp-11376], rcx; store GLFW_KEY_SPACE
		mov rax, [rbp-11376]; load GLFW_KEY_SPACE
		mov rax, 39
		push rax
		mov rax, [rbp-11384]; load GLFW_KEY_APOSTROPHE
		pop rcx
		mov [rbp-11384], rcx; store GLFW_KEY_APOSTROPHE
		mov rax, [rbp-11384]; load GLFW_KEY_APOSTROPHE
		mov rax, 44
		push rax
		mov rax, [rbp-11392]; load GLFW_KEY_COMMA
		pop rcx
		mov [rbp-11392], rcx; store GLFW_KEY_COMMA
		mov rax, [rbp-11392]; load GLFW_KEY_COMMA
		mov rax, 45
		push rax
		mov rax, [rbp-11400]; load GLFW_KEY_MINUS
		pop rcx
		mov [rbp-11400], rcx; store GLFW_KEY_MINUS
		mov rax, [rbp-11400]; load GLFW_KEY_MINUS
		mov rax, 46
		push rax
		mov rax, [rbp-11408]; load GLFW_KEY_PERIOD
		pop rcx
		mov [rbp-11408], rcx; store GLFW_KEY_PERIOD
		mov rax, [rbp-11408]; load GLFW_KEY_PERIOD
		mov rax, 47
		push rax
		mov rax, [rbp-11416]; load GLFW_KEY_SLASH
		pop rcx
		mov [rbp-11416], rcx; store GLFW_KEY_SLASH
		mov rax, [rbp-11416]; load GLFW_KEY_SLASH
		mov rax, 59
		push rax
		mov rax, [rbp-11424]; load GLFW_KEY_SEMICOLON
		pop rcx
		mov [rbp-11424], rcx; store GLFW_KEY_SEMICOLON
		mov rax, [rbp-11424]; load GLFW_KEY_SEMICOLON
		mov rax, 61
		push rax
		mov rax, [rbp-11432]; load GLFW_KEY_EQUAL
		pop rcx
		mov [rbp-11432], rcx; store GLFW_KEY_EQUAL
		mov rax, [rbp-11432]; load GLFW_KEY_EQUAL
		mov rax, 91
		push rax
		mov rax, [rbp-11440]; load GLFW_KEY_LEFT_BRACKET
		pop rcx
		mov [rbp-11440], rcx; store GLFW_KEY_LEFT_BRACKET
		mov rax, [rbp-11440]; load GLFW_KEY_LEFT_BRACKET
		mov rax, 92
		push rax
		mov rax, [rbp-11448]; load GLFW_KEY_BACKSLASH
		pop rcx
		mov [rbp-11448], rcx; store GLFW_KEY_BACKSLASH
		mov rax, [rbp-11448]; load GLFW_KEY_BACKSLASH
		mov rax, 93
		push rax
		mov rax, [rbp-11456]; load GLFW_KEY_RIGHT_BRACKET
		pop rcx
		mov [rbp-11456], rcx; store GLFW_KEY_RIGHT_BRACKET
		mov rax, [rbp-11456]; load GLFW_KEY_RIGHT_BRACKET
		mov rax, 96
		push rax
		mov rax, [rbp-11464]; load GLFW_KEY_GRAVE_ACCENT
		pop rcx
		mov [rbp-11464], rcx; store GLFW_KEY_GRAVE_ACCENT
		mov rax, [rbp-11464]; load GLFW_KEY_GRAVE_ACCENT
		mov rax, 161
		push rax
		mov rax, [rbp-11472]; load GLFW_KEY_WORLD_1
		pop rcx
		mov [rbp-11472], rcx; store GLFW_KEY_WORLD_1
		mov rax, [rbp-11472]; load GLFW_KEY_WORLD_1
		mov rax, 162
		push rax
		mov rax, [rbp-11480]; load GLFW_KEY_WORLD_2
		pop rcx
		mov [rbp-11480], rcx; store GLFW_KEY_WORLD_2
		mov rax, [rbp-11480]; load GLFW_KEY_WORLD_2
		mov rax, 48
		push rax
		mov rax, [rbp-11488]; load GLFW_KEY_0
		pop rcx
		mov [rbp-11488], rcx; store GLFW_KEY_0
		mov rax, [rbp-11488]; load GLFW_KEY_0
		mov rax, 49
		push rax
		mov rax, [rbp-11496]; load GLFW_KEY_1
		pop rcx
		mov [rbp-11496], rcx; store GLFW_KEY_1
		mov rax, [rbp-11496]; load GLFW_KEY_1
		mov rax, 50
		push rax
		mov rax, [rbp-11504]; load GLFW_KEY_2
		pop rcx
		mov [rbp-11504], rcx; store GLFW_KEY_2
		mov rax, [rbp-11504]; load GLFW_KEY_2
		mov rax, 51
		push rax
		mov rax, [rbp-11512]; load GLFW_KEY_3
		pop rcx
		mov [rbp-11512], rcx; store GLFW_KEY_3
		mov rax, [rbp-11512]; load GLFW_KEY_3
		mov rax, 52
		push rax
		mov rax, [rbp-11520]; load GLFW_KEY_4
		pop rcx
		mov [rbp-11520], rcx; store GLFW_KEY_4
		mov rax, [rbp-11520]; load GLFW_KEY_4
		mov rax, 53
		push rax
		mov rax, [rbp-11528]; load GLFW_KEY_5
		pop rcx
		mov [rbp-11528], rcx; store GLFW_KEY_5
		mov rax, [rbp-11528]; load GLFW_KEY_5
		mov rax, 54
		push rax
		mov rax, [rbp-11536]; load GLFW_KEY_6
		pop rcx
		mov [rbp-11536], rcx; store GLFW_KEY_6
		mov rax, [rbp-11536]; load GLFW_KEY_6
		mov rax, 55
		push rax
		mov rax, [rbp-11544]; load GLFW_KEY_7
		pop rcx
		mov [rbp-11544], rcx; store GLFW_KEY_7
		mov rax, [rbp-11544]; load GLFW_KEY_7
		mov rax, 56
		push rax
		mov rax, [rbp-11552]; load GLFW_KEY_8
		pop rcx
		mov [rbp-11552], rcx; store GLFW_KEY_8
		mov rax, [rbp-11552]; load GLFW_KEY_8
		mov rax, 57
		push rax
		mov rax, [rbp-11560]; load GLFW_KEY_9
		pop rcx
		mov [rbp-11560], rcx; store GLFW_KEY_9
		mov rax, [rbp-11560]; load GLFW_KEY_9
		mov rax, 65
		push rax
		mov rax, [rbp-11568]; load GLFW_KEY_A
		pop rcx
		mov [rbp-11568], rcx; store GLFW_KEY_A
		mov rax, [rbp-11568]; load GLFW_KEY_A
		mov rax, 66
		push rax
		mov rax, [rbp-11576]; load GLFW_KEY_B
		pop rcx
		mov [rbp-11576], rcx; store GLFW_KEY_B
		mov rax, [rbp-11576]; load GLFW_KEY_B
		mov rax, 67
		push rax
		mov rax, [rbp-11584]; load GLFW_KEY_C
		pop rcx
		mov [rbp-11584], rcx; store GLFW_KEY_C
		mov rax, [rbp-11584]; load GLFW_KEY_C
		mov rax, 68
		push rax
		mov rax, [rbp-11592]; load GLFW_KEY_D
		pop rcx
		mov [rbp-11592], rcx; store GLFW_KEY_D
		mov rax, [rbp-11592]; load GLFW_KEY_D
		mov rax, 69
		push rax
		mov rax, [rbp-11600]; load GLFW_KEY_E
		pop rcx
		mov [rbp-11600], rcx; store GLFW_KEY_E
		mov rax, [rbp-11600]; load GLFW_KEY_E
		mov rax, 70
		push rax
		mov rax, [rbp-11608]; load GLFW_KEY_F
		pop rcx
		mov [rbp-11608], rcx; store GLFW_KEY_F
		mov rax, [rbp-11608]; load GLFW_KEY_F
		mov rax, 71
		push rax
		mov rax, [rbp-11616]; load GLFW_KEY_G
		pop rcx
		mov [rbp-11616], rcx; store GLFW_KEY_G
		mov rax, [rbp-11616]; load GLFW_KEY_G
		mov rax, 72
		push rax
		mov rax, [rbp-11624]; load GLFW_KEY_H
		pop rcx
		mov [rbp-11624], rcx; store GLFW_KEY_H
		mov rax, [rbp-11624]; load GLFW_KEY_H
		mov rax, 73
		push rax
		mov rax, [rbp-11632]; load GLFW_KEY_I
		pop rcx
		mov [rbp-11632], rcx; store GLFW_KEY_I
		mov rax, [rbp-11632]; load GLFW_KEY_I
		mov rax, 74
		push rax
		mov rax, [rbp-11640]; load GLFW_KEY_J
		pop rcx
		mov [rbp-11640], rcx; store GLFW_KEY_J
		mov rax, [rbp-11640]; load GLFW_KEY_J
		mov rax, 75
		push rax
		mov rax, [rbp-11648]; load GLFW_KEY_K
		pop rcx
		mov [rbp-11648], rcx; store GLFW_KEY_K
		mov rax, [rbp-11648]; load GLFW_KEY_K
		mov rax, 76
		push rax
		mov rax, [rbp-11656]; load GLFW_KEY_L
		pop rcx
		mov [rbp-11656], rcx; store GLFW_KEY_L
		mov rax, [rbp-11656]; load GLFW_KEY_L
		mov rax, 77
		push rax
		mov rax, [rbp-11664]; load GLFW_KEY_M
		pop rcx
		mov [rbp-11664], rcx; store GLFW_KEY_M
		mov rax, [rbp-11664]; load GLFW_KEY_M
		mov rax, 78
		push rax
		mov rax, [rbp-11672]; load GLFW_KEY_N
		pop rcx
		mov [rbp-11672], rcx; store GLFW_KEY_N
		mov rax, [rbp-11672]; load GLFW_KEY_N
		mov rax, 79
		push rax
		mov rax, [rbp-11680]; load GLFW_KEY_O
		pop rcx
		mov [rbp-11680], rcx; store GLFW_KEY_O
		mov rax, [rbp-11680]; load GLFW_KEY_O
		mov rax, 80
		push rax
		mov rax, [rbp-11688]; load GLFW_KEY_P
		pop rcx
		mov [rbp-11688], rcx; store GLFW_KEY_P
		mov rax, [rbp-11688]; load GLFW_KEY_P
		mov rax, 81
		push rax
		mov rax, [rbp-11696]; load GLFW_KEY_Q
		pop rcx
		mov [rbp-11696], rcx; store GLFW_KEY_Q
		mov rax, [rbp-11696]; load GLFW_KEY_Q
		mov rax, 82
		push rax
		mov rax, [rbp-11704]; load GLFW_KEY_R
		pop rcx
		mov [rbp-11704], rcx; store GLFW_KEY_R
		mov rax, [rbp-11704]; load GLFW_KEY_R
		mov rax, 83
		push rax
		mov rax, [rbp-11712]; load GLFW_KEY_S
		pop rcx
		mov [rbp-11712], rcx; store GLFW_KEY_S
		mov rax, [rbp-11712]; load GLFW_KEY_S
		mov rax, 84
		push rax
		mov rax, [rbp-11720]; load GLFW_KEY_T
		pop rcx
		mov [rbp-11720], rcx; store GLFW_KEY_T
		mov rax, [rbp-11720]; load GLFW_KEY_T
		mov rax, 85
		push rax
		mov rax, [rbp-11728]; load GLFW_KEY_U
		pop rcx
		mov [rbp-11728], rcx; store GLFW_KEY_U
		mov rax, [rbp-11728]; load GLFW_KEY_U
		mov rax, 86
		push rax
		mov rax, [rbp-11736]; load GLFW_KEY_V
		pop rcx
		mov [rbp-11736], rcx; store GLFW_KEY_V
		mov rax, [rbp-11736]; load GLFW_KEY_V
		mov rax, 87
		push rax
		mov rax, [rbp-11744]; load GLFW_KEY_W
		pop rcx
		mov [rbp-11744], rcx; store GLFW_KEY_W
		mov rax, [rbp-11744]; load GLFW_KEY_W
		mov rax, 88
		push rax
		mov rax, [rbp-11752]; load GLFW_KEY_X
		pop rcx
		mov [rbp-11752], rcx; store GLFW_KEY_X
		mov rax, [rbp-11752]; load GLFW_KEY_X
		mov rax, 89
		push rax
		mov rax, [rbp-11760]; load GLFW_KEY_Y
		pop rcx
		mov [rbp-11760], rcx; store GLFW_KEY_Y
		mov rax, [rbp-11760]; load GLFW_KEY_Y
		mov rax, 90
		push rax
		mov rax, [rbp-11768]; load GLFW_KEY_Z
		pop rcx
		mov [rbp-11768], rcx; store GLFW_KEY_Z
		mov rax, [rbp-11768]; load GLFW_KEY_Z
		mov rax, 256
		push rax
		mov rax, [rbp-11776]; load GLFW_KEY_ESCAPE
		pop rcx
		mov [rbp-11776], rcx; store GLFW_KEY_ESCAPE
		mov rax, [rbp-11776]; load GLFW_KEY_ESCAPE
		mov rax, 257
		push rax
		mov rax, [rbp-11784]; load GLFW_KEY_ENTER
		pop rcx
		mov [rbp-11784], rcx; store GLFW_KEY_ENTER
		mov rax, [rbp-11784]; load GLFW_KEY_ENTER
		mov rax, 258
		push rax
		mov rax, [rbp-11792]; load GLFW_KEY_TAB
		pop rcx
		mov [rbp-11792], rcx; store GLFW_KEY_TAB
		mov rax, [rbp-11792]; load GLFW_KEY_TAB
		mov rax, 259
		push rax
		mov rax, [rbp-11800]; load GLFW_KEY_BACKSPACE
		pop rcx
		mov [rbp-11800], rcx; store GLFW_KEY_BACKSPACE
		mov rax, [rbp-11800]; load GLFW_KEY_BACKSPACE
		mov rax, 260
		push rax
		mov rax, [rbp-11808]; load GLFW_KEY_INSERT
		pop rcx
		mov [rbp-11808], rcx; store GLFW_KEY_INSERT
		mov rax, [rbp-11808]; load GLFW_KEY_INSERT
		mov rax, 261
		push rax
		mov rax, [rbp-11816]; load GLFW_KEY_DELETE
		pop rcx
		mov [rbp-11816], rcx; store GLFW_KEY_DELETE
		mov rax, [rbp-11816]; load GLFW_KEY_DELETE
		mov rax, 262
		push rax
		mov rax, [rbp-11824]; load GLFW_KEY_RIGHT
		pop rcx
		mov [rbp-11824], rcx; store GLFW_KEY_RIGHT
		mov rax, [rbp-11824]; load GLFW_KEY_RIGHT
		mov rax, 263
		push rax
		mov rax, [rbp-11832]; load GLFW_KEY_LEFT
		pop rcx
		mov [rbp-11832], rcx; store GLFW_KEY_LEFT
		mov rax, [rbp-11832]; load GLFW_KEY_LEFT
		mov rax, 264
		push rax
		mov rax, [rbp-11840]; load GLFW_KEY_DOWN
		pop rcx
		mov [rbp-11840], rcx; store GLFW_KEY_DOWN
		mov rax, [rbp-11840]; load GLFW_KEY_DOWN
		mov rax, 265
		push rax
		mov rax, [rbp-11848]; load GLFW_KEY_UP
		pop rcx
		mov [rbp-11848], rcx; store GLFW_KEY_UP
		mov rax, [rbp-11848]; load GLFW_KEY_UP
		mov rax, 266
		push rax
		mov rax, [rbp-11856]; load GLFW_KEY_PAGE_UP
		pop rcx
		mov [rbp-11856], rcx; store GLFW_KEY_PAGE_UP
		mov rax, [rbp-11856]; load GLFW_KEY_PAGE_UP
		mov rax, 267
		push rax
		mov rax, [rbp-11864]; load GLFW_KEY_PAGE_DOWN
		pop rcx
		mov [rbp-11864], rcx; store GLFW_KEY_PAGE_DOWN
		mov rax, [rbp-11864]; load GLFW_KEY_PAGE_DOWN
		mov rax, 268
		push rax
		mov rax, [rbp-11872]; load GLFW_KEY_HOME
		pop rcx
		mov [rbp-11872], rcx; store GLFW_KEY_HOME
		mov rax, [rbp-11872]; load GLFW_KEY_HOME
		mov rax, 269
		push rax
		mov rax, [rbp-11880]; load GLFW_KEY_END
		pop rcx
		mov [rbp-11880], rcx; store GLFW_KEY_END
		mov rax, [rbp-11880]; load GLFW_KEY_END
		mov rax, 280
		push rax
		mov rax, [rbp-11888]; load GLFW_KEY_CAPS_LOCK
		pop rcx
		mov [rbp-11888], rcx; store GLFW_KEY_CAPS_LOCK
		mov rax, [rbp-11888]; load GLFW_KEY_CAPS_LOCK
		mov rax, 281
		push rax
		mov rax, [rbp-11896]; load GLFW_KEY_SCROLL_LOCK
		pop rcx
		mov [rbp-11896], rcx; store GLFW_KEY_SCROLL_LOCK
		mov rax, [rbp-11896]; load GLFW_KEY_SCROLL_LOCK
		mov rax, 282
		push rax
		mov rax, [rbp-11904]; load GLFW_KEY_NUM_LOCK
		pop rcx
		mov [rbp-11904], rcx; store GLFW_KEY_NUM_LOCK
		mov rax, [rbp-11904]; load GLFW_KEY_NUM_LOCK
		mov rax, 283
		push rax
		mov rax, [rbp-11912]; load GLFW_KEY_PRINT_SCREEN
		pop rcx
		mov [rbp-11912], rcx; store GLFW_KEY_PRINT_SCREEN
		mov rax, [rbp-11912]; load GLFW_KEY_PRINT_SCREEN
		mov rax, 284
		push rax
		mov rax, [rbp-11920]; load GLFW_KEY_PAUSE
		pop rcx
		mov [rbp-11920], rcx; store GLFW_KEY_PAUSE
		mov rax, [rbp-11920]; load GLFW_KEY_PAUSE
		mov rax, 290
		push rax
		mov rax, [rbp-11928]; load GLFW_KEY_F1
		pop rcx
		mov [rbp-11928], rcx; store GLFW_KEY_F1
		mov rax, [rbp-11928]; load GLFW_KEY_F1
		mov rax, 291
		push rax
		mov rax, [rbp-11936]; load GLFW_KEY_F2
		pop rcx
		mov [rbp-11936], rcx; store GLFW_KEY_F2
		mov rax, [rbp-11936]; load GLFW_KEY_F2
		mov rax, 292
		push rax
		mov rax, [rbp-11944]; load GLFW_KEY_F3
		pop rcx
		mov [rbp-11944], rcx; store GLFW_KEY_F3
		mov rax, [rbp-11944]; load GLFW_KEY_F3
		mov rax, 293
		push rax
		mov rax, [rbp-11952]; load GLFW_KEY_F4
		pop rcx
		mov [rbp-11952], rcx; store GLFW_KEY_F4
		mov rax, [rbp-11952]; load GLFW_KEY_F4
		mov rax, 294
		push rax
		mov rax, [rbp-11960]; load GLFW_KEY_F5
		pop rcx
		mov [rbp-11960], rcx; store GLFW_KEY_F5
		mov rax, [rbp-11960]; load GLFW_KEY_F5
		mov rax, 295
		push rax
		mov rax, [rbp-11968]; load GLFW_KEY_F6
		pop rcx
		mov [rbp-11968], rcx; store GLFW_KEY_F6
		mov rax, [rbp-11968]; load GLFW_KEY_F6
		mov rax, 296
		push rax
		mov rax, [rbp-11976]; load GLFW_KEY_F7
		pop rcx
		mov [rbp-11976], rcx; store GLFW_KEY_F7
		mov rax, [rbp-11976]; load GLFW_KEY_F7
		mov rax, 297
		push rax
		mov rax, [rbp-11984]; load GLFW_KEY_F8
		pop rcx
		mov [rbp-11984], rcx; store GLFW_KEY_F8
		mov rax, [rbp-11984]; load GLFW_KEY_F8
		mov rax, 298
		push rax
		mov rax, [rbp-11992]; load GLFW_KEY_F9
		pop rcx
		mov [rbp-11992], rcx; store GLFW_KEY_F9
		mov rax, [rbp-11992]; load GLFW_KEY_F9
		mov rax, 299
		push rax
		mov rax, [rbp-12000]; load GLFW_KEY_F10
		pop rcx
		mov [rbp-12000], rcx; store GLFW_KEY_F10
		mov rax, [rbp-12000]; load GLFW_KEY_F10
		mov rax, 300
		push rax
		mov rax, [rbp-12008]; load GLFW_KEY_F11
		pop rcx
		mov [rbp-12008], rcx; store GLFW_KEY_F11
		mov rax, [rbp-12008]; load GLFW_KEY_F11
		mov rax, 301
		push rax
		mov rax, [rbp-12016]; load GLFW_KEY_F12
		pop rcx
		mov [rbp-12016], rcx; store GLFW_KEY_F12
		mov rax, [rbp-12016]; load GLFW_KEY_F12
		mov rax, 302
		push rax
		mov rax, [rbp-12024]; load GLFW_KEY_F13
		pop rcx
		mov [rbp-12024], rcx; store GLFW_KEY_F13
		mov rax, [rbp-12024]; load GLFW_KEY_F13
		mov rax, 303
		push rax
		mov rax, [rbp-12032]; load GLFW_KEY_F14
		pop rcx
		mov [rbp-12032], rcx; store GLFW_KEY_F14
		mov rax, [rbp-12032]; load GLFW_KEY_F14
		mov rax, 304
		push rax
		mov rax, [rbp-12040]; load GLFW_KEY_F15
		pop rcx
		mov [rbp-12040], rcx; store GLFW_KEY_F15
		mov rax, [rbp-12040]; load GLFW_KEY_F15
		mov rax, 305
		push rax
		mov rax, [rbp-12048]; load GLFW_KEY_F16
		pop rcx
		mov [rbp-12048], rcx; store GLFW_KEY_F16
		mov rax, [rbp-12048]; load GLFW_KEY_F16
		mov rax, 306
		push rax
		mov rax, [rbp-12056]; load GLFW_KEY_F17
		pop rcx
		mov [rbp-12056], rcx; store GLFW_KEY_F17
		mov rax, [rbp-12056]; load GLFW_KEY_F17
		mov rax, 307
		push rax
		mov rax, [rbp-12064]; load GLFW_KEY_F18
		pop rcx
		mov [rbp-12064], rcx; store GLFW_KEY_F18
		mov rax, [rbp-12064]; load GLFW_KEY_F18
		mov rax, 308
		push rax
		mov rax, [rbp-12072]; load GLFW_KEY_F19
		pop rcx
		mov [rbp-12072], rcx; store GLFW_KEY_F19
		mov rax, [rbp-12072]; load GLFW_KEY_F19
		mov rax, 309
		push rax
		mov rax, [rbp-12080]; load GLFW_KEY_F20
		pop rcx
		mov [rbp-12080], rcx; store GLFW_KEY_F20
		mov rax, [rbp-12080]; load GLFW_KEY_F20
		mov rax, 310
		push rax
		mov rax, [rbp-12088]; load GLFW_KEY_F21
		pop rcx
		mov [rbp-12088], rcx; store GLFW_KEY_F21
		mov rax, [rbp-12088]; load GLFW_KEY_F21
		mov rax, 311
		push rax
		mov rax, [rbp-12096]; load GLFW_KEY_F22
		pop rcx
		mov [rbp-12096], rcx; store GLFW_KEY_F22
		mov rax, [rbp-12096]; load GLFW_KEY_F22
		mov rax, 312
		push rax
		mov rax, [rbp-12104]; load GLFW_KEY_F23
		pop rcx
		mov [rbp-12104], rcx; store GLFW_KEY_F23
		mov rax, [rbp-12104]; load GLFW_KEY_F23
		mov rax, 313
		push rax
		mov rax, [rbp-12112]; load GLFW_KEY_F24
		pop rcx
		mov [rbp-12112], rcx; store GLFW_KEY_F24
		mov rax, [rbp-12112]; load GLFW_KEY_F24
		mov rax, 314
		push rax
		mov rax, [rbp-12120]; load GLFW_KEY_F25
		pop rcx
		mov [rbp-12120], rcx; store GLFW_KEY_F25
		mov rax, [rbp-12120]; load GLFW_KEY_F25
		mov rax, 320
		push rax
		mov rax, [rbp-12128]; load GLFW_KEY_KP_0
		pop rcx
		mov [rbp-12128], rcx; store GLFW_KEY_KP_0
		mov rax, [rbp-12128]; load GLFW_KEY_KP_0
		mov rax, 321
		push rax
		mov rax, [rbp-12136]; load GLFW_KEY_KP_1
		pop rcx
		mov [rbp-12136], rcx; store GLFW_KEY_KP_1
		mov rax, [rbp-12136]; load GLFW_KEY_KP_1
		mov rax, 322
		push rax
		mov rax, [rbp-12144]; load GLFW_KEY_KP_2
		pop rcx
		mov [rbp-12144], rcx; store GLFW_KEY_KP_2
		mov rax, [rbp-12144]; load GLFW_KEY_KP_2
		mov rax, 323
		push rax
		mov rax, [rbp-12152]; load GLFW_KEY_KP_3
		pop rcx
		mov [rbp-12152], rcx; store GLFW_KEY_KP_3
		mov rax, [rbp-12152]; load GLFW_KEY_KP_3
		mov rax, 324
		push rax
		mov rax, [rbp-12160]; load GLFW_KEY_KP_4
		pop rcx
		mov [rbp-12160], rcx; store GLFW_KEY_KP_4
		mov rax, [rbp-12160]; load GLFW_KEY_KP_4
		mov rax, 325
		push rax
		mov rax, [rbp-12168]; load GLFW_KEY_KP_5
		pop rcx
		mov [rbp-12168], rcx; store GLFW_KEY_KP_5
		mov rax, [rbp-12168]; load GLFW_KEY_KP_5
		mov rax, 326
		push rax
		mov rax, [rbp-12176]; load GLFW_KEY_KP_6
		pop rcx
		mov [rbp-12176], rcx; store GLFW_KEY_KP_6
		mov rax, [rbp-12176]; load GLFW_KEY_KP_6
		mov rax, 327
		push rax
		mov rax, [rbp-12184]; load GLFW_KEY_KP_7
		pop rcx
		mov [rbp-12184], rcx; store GLFW_KEY_KP_7
		mov rax, [rbp-12184]; load GLFW_KEY_KP_7
		mov rax, 328
		push rax
		mov rax, [rbp-12192]; load GLFW_KEY_KP_8
		pop rcx
		mov [rbp-12192], rcx; store GLFW_KEY_KP_8
		mov rax, [rbp-12192]; load GLFW_KEY_KP_8
		mov rax, 329
		push rax
		mov rax, [rbp-12200]; load GLFW_KEY_KP_9
		pop rcx
		mov [rbp-12200], rcx; store GLFW_KEY_KP_9
		mov rax, [rbp-12200]; load GLFW_KEY_KP_9
		mov rax, 330
		push rax
		mov rax, [rbp-12208]; load GLFW_KEY_KP_DECIMAL
		pop rcx
		mov [rbp-12208], rcx; store GLFW_KEY_KP_DECIMAL
		mov rax, [rbp-12208]; load GLFW_KEY_KP_DECIMAL
		mov rax, 331
		push rax
		mov rax, [rbp-12216]; load GLFW_KEY_KP_DIVIDE
		pop rcx
		mov [rbp-12216], rcx; store GLFW_KEY_KP_DIVIDE
		mov rax, [rbp-12216]; load GLFW_KEY_KP_DIVIDE
		mov rax, 332
		push rax
		mov rax, [rbp-12224]; load GLFW_KEY_KP_MULTIPLY
		pop rcx
		mov [rbp-12224], rcx; store GLFW_KEY_KP_MULTIPLY
		mov rax, [rbp-12224]; load GLFW_KEY_KP_MULTIPLY
		mov rax, 333
		push rax
		mov rax, [rbp-12232]; load GLFW_KEY_KP_SUBTRACT
		pop rcx
		mov [rbp-12232], rcx; store GLFW_KEY_KP_SUBTRACT
		mov rax, [rbp-12232]; load GLFW_KEY_KP_SUBTRACT
		mov rax, 334
		push rax
		mov rax, [rbp-12240]; load GLFW_KEY_KP_ADD
		pop rcx
		mov [rbp-12240], rcx; store GLFW_KEY_KP_ADD
		mov rax, [rbp-12240]; load GLFW_KEY_KP_ADD
		mov rax, 335
		push rax
		mov rax, [rbp-12248]; load GLFW_KEY_KP_ENTER
		pop rcx
		mov [rbp-12248], rcx; store GLFW_KEY_KP_ENTER
		mov rax, [rbp-12248]; load GLFW_KEY_KP_ENTER
		mov rax, 336
		push rax
		mov rax, [rbp-12256]; load GLFW_KEY_KP_EQUAL
		pop rcx
		mov [rbp-12256], rcx; store GLFW_KEY_KP_EQUAL
		mov rax, [rbp-12256]; load GLFW_KEY_KP_EQUAL
		mov rax, 340
		push rax
		mov rax, [rbp-12264]; load GLFW_KEY_LEFT_SHIFT
		pop rcx
		mov [rbp-12264], rcx; store GLFW_KEY_LEFT_SHIFT
		mov rax, [rbp-12264]; load GLFW_KEY_LEFT_SHIFT
		mov rax, 341
		push rax
		mov rax, [rbp-12272]; load GLFW_KEY_LEFT_CONTROL
		pop rcx
		mov [rbp-12272], rcx; store GLFW_KEY_LEFT_CONTROL
		mov rax, [rbp-12272]; load GLFW_KEY_LEFT_CONTROL
		mov rax, 342
		push rax
		mov rax, [rbp-12280]; load GLFW_KEY_LEFT_ALT
		pop rcx
		mov [rbp-12280], rcx; store GLFW_KEY_LEFT_ALT
		mov rax, [rbp-12280]; load GLFW_KEY_LEFT_ALT
		mov rax, 343
		push rax
		mov rax, [rbp-12288]; load GLFW_KEY_LEFT_SUPER
		pop rcx
		mov [rbp-12288], rcx; store GLFW_KEY_LEFT_SUPER
		mov rax, [rbp-12288]; load GLFW_KEY_LEFT_SUPER
		mov rax, 344
		push rax
		mov rax, [rbp-12296]; load GLFW_KEY_RIGHT_SHIFT
		pop rcx
		mov [rbp-12296], rcx; store GLFW_KEY_RIGHT_SHIFT
		mov rax, [rbp-12296]; load GLFW_KEY_RIGHT_SHIFT
		mov rax, 345
		push rax
		mov rax, [rbp-12304]; load GLFW_KEY_RIGHT_CONTROL
		pop rcx
		mov [rbp-12304], rcx; store GLFW_KEY_RIGHT_CONTROL
		mov rax, [rbp-12304]; load GLFW_KEY_RIGHT_CONTROL
		mov rax, 346
		push rax
		mov rax, [rbp-12312]; load GLFW_KEY_RIGHT_ALT
		pop rcx
		mov [rbp-12312], rcx; store GLFW_KEY_RIGHT_ALT
		mov rax, [rbp-12312]; load GLFW_KEY_RIGHT_ALT
		mov rax, 347
		push rax
		mov rax, [rbp-12320]; load GLFW_KEY_RIGHT_SUPER
		pop rcx
		mov [rbp-12320], rcx; store GLFW_KEY_RIGHT_SUPER
		mov rax, [rbp-12320]; load GLFW_KEY_RIGHT_SUPER
		mov rax, 348
		push rax
		mov rax, [rbp-12328]; load GLFW_KEY_MENU
		pop rcx
		mov [rbp-12328], rcx; store GLFW_KEY_MENU
		mov rax, [rbp-12328]; load GLFW_KEY_MENU
		mov rax, 348
		push rax
		mov rax, [rbp-12336]; load GLFW_KEY_LAST
		pop rcx
		mov [rbp-12336], rcx; store GLFW_KEY_LAST
		mov rax, [rbp-12336]; load GLFW_KEY_LAST
		mov rax, 1
		push rax
		mov rax, [rbp-12344]; load GLFW_MOD_SHIFT
		pop rcx
		mov [rbp-12344], rcx; store GLFW_MOD_SHIFT
		mov rax, [rbp-12344]; load GLFW_MOD_SHIFT
		mov rax, 2
		push rax
		mov rax, [rbp-12352]; load GLFW_MOD_CONTROL
		pop rcx
		mov [rbp-12352], rcx; store GLFW_MOD_CONTROL
		mov rax, [rbp-12352]; load GLFW_MOD_CONTROL
		mov rax, 4
		push rax
		mov rax, [rbp-12360]; load GLFW_MOD_ALT
		pop rcx
		mov [rbp-12360], rcx; store GLFW_MOD_ALT
		mov rax, [rbp-12360]; load GLFW_MOD_ALT
		mov rax, 8
		push rax
		mov rax, [rbp-12368]; load GLFW_MOD_SUPER
		pop rcx
		mov [rbp-12368], rcx; store GLFW_MOD_SUPER
		mov rax, [rbp-12368]; load GLFW_MOD_SUPER
		mov rax, 0
		push rax
		mov rax, [rbp-12376]; load GLFW_MOUSE_BUTTON_1
		pop rcx
		mov [rbp-12376], rcx; store GLFW_MOUSE_BUTTON_1
		mov rax, [rbp-12376]; load GLFW_MOUSE_BUTTON_1
		mov rax, 1
		push rax
		mov rax, [rbp-12384]; load GLFW_MOUSE_BUTTON_2
		pop rcx
		mov [rbp-12384], rcx; store GLFW_MOUSE_BUTTON_2
		mov rax, [rbp-12384]; load GLFW_MOUSE_BUTTON_2
		mov rax, 2
		push rax
		mov rax, [rbp-12392]; load GLFW_MOUSE_BUTTON_3
		pop rcx
		mov [rbp-12392], rcx; store GLFW_MOUSE_BUTTON_3
		mov rax, [rbp-12392]; load GLFW_MOUSE_BUTTON_3
		mov rax, 3
		push rax
		mov rax, [rbp-12400]; load GLFW_MOUSE_BUTTON_4
		pop rcx
		mov [rbp-12400], rcx; store GLFW_MOUSE_BUTTON_4
		mov rax, [rbp-12400]; load GLFW_MOUSE_BUTTON_4
		mov rax, 4
		push rax
		mov rax, [rbp-12408]; load GLFW_MOUSE_BUTTON_5
		pop rcx
		mov [rbp-12408], rcx; store GLFW_MOUSE_BUTTON_5
		mov rax, [rbp-12408]; load GLFW_MOUSE_BUTTON_5
		mov rax, 5
		push rax
		mov rax, [rbp-12416]; load GLFW_MOUSE_BUTTON_6
		pop rcx
		mov [rbp-12416], rcx; store GLFW_MOUSE_BUTTON_6
		mov rax, [rbp-12416]; load GLFW_MOUSE_BUTTON_6
		mov rax, 6
		push rax
		mov rax, [rbp-12424]; load GLFW_MOUSE_BUTTON_7
		pop rcx
		mov [rbp-12424], rcx; store GLFW_MOUSE_BUTTON_7
		mov rax, [rbp-12424]; load GLFW_MOUSE_BUTTON_7
		mov rax, 7
		push rax
		mov rax, [rbp-12432]; load GLFW_MOUSE_BUTTON_8
		pop rcx
		mov [rbp-12432], rcx; store GLFW_MOUSE_BUTTON_8
		mov rax, [rbp-12432]; load GLFW_MOUSE_BUTTON_8
		mov rax, 7
		push rax
		mov rax, [rbp-12440]; load GLFW_MOUSE_BUTTON_LAST
		pop rcx
		mov [rbp-12440], rcx; store GLFW_MOUSE_BUTTON_LAST
		mov rax, [rbp-12440]; load GLFW_MOUSE_BUTTON_LAST
		mov rax, 0
		push rax
		mov rax, [rbp-12448]; load GLFW_MOUSE_BUTTON_LEFT
		pop rcx
		mov [rbp-12448], rcx; store GLFW_MOUSE_BUTTON_LEFT
		mov rax, [rbp-12448]; load GLFW_MOUSE_BUTTON_LEFT
		mov rax, 1
		push rax
		mov rax, [rbp-12456]; load GLFW_MOUSE_BUTTON_RIGHT
		pop rcx
		mov [rbp-12456], rcx; store GLFW_MOUSE_BUTTON_RIGHT
		mov rax, [rbp-12456]; load GLFW_MOUSE_BUTTON_RIGHT
		mov rax, 2
		push rax
		mov rax, [rbp-12464]; load GLFW_MOUSE_BUTTON_MIDDLE
		pop rcx
		mov [rbp-12464], rcx; store GLFW_MOUSE_BUTTON_MIDDLE
		mov rax, [rbp-12464]; load GLFW_MOUSE_BUTTON_MIDDLE
		mov rax, 0
		push rax
		mov rax, [rbp-12472]; load GLFW_JOYSTICK_1
		pop rcx
		mov [rbp-12472], rcx; store GLFW_JOYSTICK_1
		mov rax, [rbp-12472]; load GLFW_JOYSTICK_1
		mov rax, 1
		push rax
		mov rax, [rbp-12480]; load GLFW_JOYSTICK_2
		pop rcx
		mov [rbp-12480], rcx; store GLFW_JOYSTICK_2
		mov rax, [rbp-12480]; load GLFW_JOYSTICK_2
		mov rax, 2
		push rax
		mov rax, [rbp-12488]; load GLFW_JOYSTICK_3
		pop rcx
		mov [rbp-12488], rcx; store GLFW_JOYSTICK_3
		mov rax, [rbp-12488]; load GLFW_JOYSTICK_3
		mov rax, 3
		push rax
		mov rax, [rbp-12496]; load GLFW_JOYSTICK_4
		pop rcx
		mov [rbp-12496], rcx; store GLFW_JOYSTICK_4
		mov rax, [rbp-12496]; load GLFW_JOYSTICK_4
		mov rax, 4
		push rax
		mov rax, [rbp-12504]; load GLFW_JOYSTICK_5
		pop rcx
		mov [rbp-12504], rcx; store GLFW_JOYSTICK_5
		mov rax, [rbp-12504]; load GLFW_JOYSTICK_5
		mov rax, 5
		push rax
		mov rax, [rbp-12512]; load GLFW_JOYSTICK_6
		pop rcx
		mov [rbp-12512], rcx; store GLFW_JOYSTICK_6
		mov rax, [rbp-12512]; load GLFW_JOYSTICK_6
		mov rax, 6
		push rax
		mov rax, [rbp-12520]; load GLFW_JOYSTICK_7
		pop rcx
		mov [rbp-12520], rcx; store GLFW_JOYSTICK_7
		mov rax, [rbp-12520]; load GLFW_JOYSTICK_7
		mov rax, 7
		push rax
		mov rax, [rbp-12528]; load GLFW_JOYSTICK_8
		pop rcx
		mov [rbp-12528], rcx; store GLFW_JOYSTICK_8
		mov rax, [rbp-12528]; load GLFW_JOYSTICK_8
		mov rax, 8
		push rax
		mov rax, [rbp-12536]; load GLFW_JOYSTICK_9
		pop rcx
		mov [rbp-12536], rcx; store GLFW_JOYSTICK_9
		mov rax, [rbp-12536]; load GLFW_JOYSTICK_9
		mov rax, 9
		push rax
		mov rax, [rbp-12544]; load GLFW_JOYSTICK_10
		pop rcx
		mov [rbp-12544], rcx; store GLFW_JOYSTICK_10
		mov rax, [rbp-12544]; load GLFW_JOYSTICK_10
		mov rax, 10
		push rax
		mov rax, [rbp-12552]; load GLFW_JOYSTICK_11
		pop rcx
		mov [rbp-12552], rcx; store GLFW_JOYSTICK_11
		mov rax, [rbp-12552]; load GLFW_JOYSTICK_11
		mov rax, 11
		push rax
		mov rax, [rbp-12560]; load GLFW_JOYSTICK_12
		pop rcx
		mov [rbp-12560], rcx; store GLFW_JOYSTICK_12
		mov rax, [rbp-12560]; load GLFW_JOYSTICK_12
		mov rax, 12
		push rax
		mov rax, [rbp-12568]; load GLFW_JOYSTICK_13
		pop rcx
		mov [rbp-12568], rcx; store GLFW_JOYSTICK_13
		mov rax, [rbp-12568]; load GLFW_JOYSTICK_13
		mov rax, 13
		push rax
		mov rax, [rbp-12576]; load GLFW_JOYSTICK_14
		pop rcx
		mov [rbp-12576], rcx; store GLFW_JOYSTICK_14
		mov rax, [rbp-12576]; load GLFW_JOYSTICK_14
		mov rax, 14
		push rax
		mov rax, [rbp-12584]; load GLFW_JOYSTICK_15
		pop rcx
		mov [rbp-12584], rcx; store GLFW_JOYSTICK_15
		mov rax, [rbp-12584]; load GLFW_JOYSTICK_15
		mov rax, 15
		push rax
		mov rax, [rbp-12592]; load GLFW_JOYSTICK_16
		pop rcx
		mov [rbp-12592], rcx; store GLFW_JOYSTICK_16
		mov rax, [rbp-12592]; load GLFW_JOYSTICK_16
		mov rax, 15
		push rax
		mov rax, [rbp-12600]; load GLFW_JOYSTICK_LAST
		pop rcx
		mov [rbp-12600], rcx; store GLFW_JOYSTICK_LAST
		mov rax, [rbp-12600]; load GLFW_JOYSTICK_LAST
		mov rax, 65537
		push rax
		mov rax, [rbp-12608]; load GLFW_NOT_INITIALIZED
		pop rcx
		mov [rbp-12608], rcx; store GLFW_NOT_INITIALIZED
		mov rax, [rbp-12608]; load GLFW_NOT_INITIALIZED
		mov rax, 65538
		push rax
		mov rax, [rbp-12616]; load GLFW_NO_CURRENT_CONTEXT
		pop rcx
		mov [rbp-12616], rcx; store GLFW_NO_CURRENT_CONTEXT
		mov rax, [rbp-12616]; load GLFW_NO_CURRENT_CONTEXT
		mov rax, 65539
		push rax
		mov rax, [rbp-12624]; load GLFW_INVALID_ENUM
		pop rcx
		mov [rbp-12624], rcx; store GLFW_INVALID_ENUM
		mov rax, [rbp-12624]; load GLFW_INVALID_ENUM
		mov rax, 65540
		push rax
		mov rax, [rbp-12632]; load GLFW_INVALID_VALUE
		pop rcx
		mov [rbp-12632], rcx; store GLFW_INVALID_VALUE
		mov rax, [rbp-12632]; load GLFW_INVALID_VALUE
		mov rax, 65541
		push rax
		mov rax, [rbp-12640]; load GLFW_OUT_OF_MEMORY
		pop rcx
		mov [rbp-12640], rcx; store GLFW_OUT_OF_MEMORY
		mov rax, [rbp-12640]; load GLFW_OUT_OF_MEMORY
		mov rax, 65542
		push rax
		mov rax, [rbp-12648]; load GLFW_API_UNAVAILABLE
		pop rcx
		mov [rbp-12648], rcx; store GLFW_API_UNAVAILABLE
		mov rax, [rbp-12648]; load GLFW_API_UNAVAILABLE
		mov rax, 65543
		push rax
		mov rax, [rbp-12656]; load GLFW_VERSION_UNAVAILABLE
		pop rcx
		mov [rbp-12656], rcx; store GLFW_VERSION_UNAVAILABLE
		mov rax, [rbp-12656]; load GLFW_VERSION_UNAVAILABLE
		mov rax, 65544
		push rax
		mov rax, [rbp-12664]; load GLFW_PLATFORM_ERROR
		pop rcx
		mov [rbp-12664], rcx; store GLFW_PLATFORM_ERROR
		mov rax, [rbp-12664]; load GLFW_PLATFORM_ERROR
		mov rax, 65545
		push rax
		mov rax, [rbp-12672]; load GLFW_FORMAT_UNAVAILABLE
		pop rcx
		mov [rbp-12672], rcx; store GLFW_FORMAT_UNAVAILABLE
		mov rax, [rbp-12672]; load GLFW_FORMAT_UNAVAILABLE
		mov rax, 65546
		push rax
		mov rax, [rbp-12680]; load GLFW_NO_WINDOW_CONTEXT
		pop rcx
		mov [rbp-12680], rcx; store GLFW_NO_WINDOW_CONTEXT
		mov rax, [rbp-12680]; load GLFW_NO_WINDOW_CONTEXT
		mov rax, 131073
		push rax
		mov rax, [rbp-12688]; load GLFW_FOCUSED
		pop rcx
		mov [rbp-12688], rcx; store GLFW_FOCUSED
		mov rax, [rbp-12688]; load GLFW_FOCUSED
		mov rax, 131074
		push rax
		mov rax, [rbp-12696]; load GLFW_ICONIFIED
		pop rcx
		mov [rbp-12696], rcx; store GLFW_ICONIFIED
		mov rax, [rbp-12696]; load GLFW_ICONIFIED
		mov rax, 131075
		push rax
		mov rax, [rbp-12704]; load GLFW_RESIZABLE
		pop rcx
		mov [rbp-12704], rcx; store GLFW_RESIZABLE
		mov rax, [rbp-12704]; load GLFW_RESIZABLE
		mov rax, 131076
		push rax
		mov rax, [rbp-12712]; load GLFW_VISIBLE
		pop rcx
		mov [rbp-12712], rcx; store GLFW_VISIBLE
		mov rax, [rbp-12712]; load GLFW_VISIBLE
		mov rax, 131077
		push rax
		mov rax, [rbp-12720]; load GLFW_DECORATED
		pop rcx
		mov [rbp-12720], rcx; store GLFW_DECORATED
		mov rax, [rbp-12720]; load GLFW_DECORATED
		mov rax, 131078
		push rax
		mov rax, [rbp-12728]; load GLFW_AUTO_ICONIFY
		pop rcx
		mov [rbp-12728], rcx; store GLFW_AUTO_ICONIFY
		mov rax, [rbp-12728]; load GLFW_AUTO_ICONIFY
		mov rax, 131079
		push rax
		mov rax, [rbp-12736]; load GLFW_FLOATING
		pop rcx
		mov [rbp-12736], rcx; store GLFW_FLOATING
		mov rax, [rbp-12736]; load GLFW_FLOATING
		mov rax, 131080
		push rax
		mov rax, [rbp-12744]; load GLFW_MAXIMIZED
		pop rcx
		mov [rbp-12744], rcx; store GLFW_MAXIMIZED
		mov rax, [rbp-12744]; load GLFW_MAXIMIZED
		mov rax, 135169
		push rax
		mov rax, [rbp-12752]; load GLFW_RED_BITS
		pop rcx
		mov [rbp-12752], rcx; store GLFW_RED_BITS
		mov rax, [rbp-12752]; load GLFW_RED_BITS
		mov rax, 135170
		push rax
		mov rax, [rbp-12760]; load GLFW_GREEN_BITS
		pop rcx
		mov [rbp-12760], rcx; store GLFW_GREEN_BITS
		mov rax, [rbp-12760]; load GLFW_GREEN_BITS
		mov rax, 135171
		push rax
		mov rax, [rbp-12768]; load GLFW_BLUE_BITS
		pop rcx
		mov [rbp-12768], rcx; store GLFW_BLUE_BITS
		mov rax, [rbp-12768]; load GLFW_BLUE_BITS
		mov rax, 135172
		push rax
		mov rax, [rbp-12776]; load GLFW_ALPHA_BITS
		pop rcx
		mov [rbp-12776], rcx; store GLFW_ALPHA_BITS
		mov rax, [rbp-12776]; load GLFW_ALPHA_BITS
		mov rax, 135173
		push rax
		mov rax, [rbp-12784]; load GLFW_DEPTH_BITS
		pop rcx
		mov [rbp-12784], rcx; store GLFW_DEPTH_BITS
		mov rax, [rbp-12784]; load GLFW_DEPTH_BITS
		mov rax, 135174
		push rax
		mov rax, [rbp-12792]; load GLFW_STENCIL_BITS
		pop rcx
		mov [rbp-12792], rcx; store GLFW_STENCIL_BITS
		mov rax, [rbp-12792]; load GLFW_STENCIL_BITS
		mov rax, 135175
		push rax
		mov rax, [rbp-12800]; load GLFW_ACCUM_RED_BITS
		pop rcx
		mov [rbp-12800], rcx; store GLFW_ACCUM_RED_BITS
		mov rax, [rbp-12800]; load GLFW_ACCUM_RED_BITS
		mov rax, 135176
		push rax
		mov rax, [rbp-12808]; load GLFW_ACCUM_GREEN_BITS
		pop rcx
		mov [rbp-12808], rcx; store GLFW_ACCUM_GREEN_BITS
		mov rax, [rbp-12808]; load GLFW_ACCUM_GREEN_BITS
		mov rax, 135177
		push rax
		mov rax, [rbp-12816]; load GLFW_ACCUM_BLUE_BITS
		pop rcx
		mov [rbp-12816], rcx; store GLFW_ACCUM_BLUE_BITS
		mov rax, [rbp-12816]; load GLFW_ACCUM_BLUE_BITS
		mov rax, 135178
		push rax
		mov rax, [rbp-12824]; load GLFW_ACCUM_ALPHA_BITS
		pop rcx
		mov [rbp-12824], rcx; store GLFW_ACCUM_ALPHA_BITS
		mov rax, [rbp-12824]; load GLFW_ACCUM_ALPHA_BITS
		mov rax, 135179
		push rax
		mov rax, [rbp-12832]; load GLFW_AUX_BUFFERS
		pop rcx
		mov [rbp-12832], rcx; store GLFW_AUX_BUFFERS
		mov rax, [rbp-12832]; load GLFW_AUX_BUFFERS
		mov rax, 135180
		push rax
		mov rax, [rbp-12840]; load GLFW_STEREO
		pop rcx
		mov [rbp-12840], rcx; store GLFW_STEREO
		mov rax, [rbp-12840]; load GLFW_STEREO
		mov rax, 135181
		push rax
		mov rax, [rbp-12848]; load GLFW_SAMPLES
		pop rcx
		mov [rbp-12848], rcx; store GLFW_SAMPLES
		mov rax, [rbp-12848]; load GLFW_SAMPLES
		mov rax, 135182
		push rax
		mov rax, [rbp-12856]; load GLFW_SRGB_CAPABLE
		pop rcx
		mov [rbp-12856], rcx; store GLFW_SRGB_CAPABLE
		mov rax, [rbp-12856]; load GLFW_SRGB_CAPABLE
		mov rax, 135183
		push rax
		mov rax, [rbp-12864]; load GLFW_REFRESH_RATE
		pop rcx
		mov [rbp-12864], rcx; store GLFW_REFRESH_RATE
		mov rax, [rbp-12864]; load GLFW_REFRESH_RATE
		mov rax, 135184
		push rax
		mov rax, [rbp-12872]; load GLFW_DOUBLEBUFFER
		pop rcx
		mov [rbp-12872], rcx; store GLFW_DOUBLEBUFFER
		mov rax, [rbp-12872]; load GLFW_DOUBLEBUFFER
		mov rax, 139265
		push rax
		mov rax, [rbp-12880]; load GLFW_CLIENT_API
		pop rcx
		mov [rbp-12880], rcx; store GLFW_CLIENT_API
		mov rax, [rbp-12880]; load GLFW_CLIENT_API
		mov rax, 139266
		push rax
		mov rax, [rbp-12888]; load GLFW_CONTEXT_VERSION_MAJOR
		pop rcx
		mov [rbp-12888], rcx; store GLFW_CONTEXT_VERSION_MAJOR
		mov rax, [rbp-12888]; load GLFW_CONTEXT_VERSION_MAJOR
		mov rax, 139267
		push rax
		mov rax, [rbp-12896]; load GLFW_CONTEXT_VERSION_MINOR
		pop rcx
		mov [rbp-12896], rcx; store GLFW_CONTEXT_VERSION_MINOR
		mov rax, [rbp-12896]; load GLFW_CONTEXT_VERSION_MINOR
		mov rax, 139268
		push rax
		mov rax, [rbp-12904]; load GLFW_CONTEXT_REVISION
		pop rcx
		mov [rbp-12904], rcx; store GLFW_CONTEXT_REVISION
		mov rax, [rbp-12904]; load GLFW_CONTEXT_REVISION
		mov rax, 139269
		push rax
		mov rax, [rbp-12912]; load GLFW_CONTEXT_ROBUSTNESS
		pop rcx
		mov [rbp-12912], rcx; store GLFW_CONTEXT_ROBUSTNESS
		mov rax, [rbp-12912]; load GLFW_CONTEXT_ROBUSTNESS
		mov rax, 139270
		push rax
		mov rax, [rbp-12920]; load GLFW_OPENGL_FORWARD_COMPAT
		pop rcx
		mov [rbp-12920], rcx; store GLFW_OPENGL_FORWARD_COMPAT
		mov rax, [rbp-12920]; load GLFW_OPENGL_FORWARD_COMPAT
		mov rax, 139271
		push rax
		mov rax, [rbp-12928]; load GLFW_OPENGL_DEBUG_CONTEXT
		pop rcx
		mov [rbp-12928], rcx; store GLFW_OPENGL_DEBUG_CONTEXT
		mov rax, [rbp-12928]; load GLFW_OPENGL_DEBUG_CONTEXT
		mov rax, 139272
		push rax
		mov rax, [rbp-12936]; load GLFW_OPENGL_PROFILE
		pop rcx
		mov [rbp-12936], rcx; store GLFW_OPENGL_PROFILE
		mov rax, [rbp-12936]; load GLFW_OPENGL_PROFILE
		mov rax, 139273
		push rax
		mov rax, [rbp-12944]; load GLFW_CONTEXT_RELEASE_BEHAVIOR
		pop rcx
		mov [rbp-12944], rcx; store GLFW_CONTEXT_RELEASE_BEHAVIOR
		mov rax, [rbp-12944]; load GLFW_CONTEXT_RELEASE_BEHAVIOR
		mov rax, 139274
		push rax
		mov rax, [rbp-12952]; load GLFW_CONTEXT_NO_ERROR
		pop rcx
		mov [rbp-12952], rcx; store GLFW_CONTEXT_NO_ERROR
		mov rax, [rbp-12952]; load GLFW_CONTEXT_NO_ERROR
		mov rax, 139275
		push rax
		mov rax, [rbp-12960]; load GLFW_CONTEXT_CREATION_API
		pop rcx
		mov [rbp-12960], rcx; store GLFW_CONTEXT_CREATION_API
		mov rax, [rbp-12960]; load GLFW_CONTEXT_CREATION_API
		mov rax, 0
		push rax
		mov rax, [rbp-12968]; load GLFW_NO_API
		pop rcx
		mov [rbp-12968], rcx; store GLFW_NO_API
		mov rax, [rbp-12968]; load GLFW_NO_API
		mov rax, 196609
		push rax
		mov rax, [rbp-12976]; load GLFW_OPENGL_API
		pop rcx
		mov [rbp-12976], rcx; store GLFW_OPENGL_API
		mov rax, [rbp-12976]; load GLFW_OPENGL_API
		mov rax, 196610
		push rax
		mov rax, [rbp-12984]; load GLFW_OPENGL_ES_API
		pop rcx
		mov [rbp-12984], rcx; store GLFW_OPENGL_ES_API
		mov rax, [rbp-12984]; load GLFW_OPENGL_ES_API
		mov rax, 0
		push rax
		mov rax, [rbp-12992]; load GLFW_NO_ROBUSTNESS
		pop rcx
		mov [rbp-12992], rcx; store GLFW_NO_ROBUSTNESS
		mov rax, [rbp-12992]; load GLFW_NO_ROBUSTNESS
		mov rax, 200705
		push rax
		mov rax, [rbp-13000]; load GLFW_NO_RESET_NOTIFICATION
		pop rcx
		mov [rbp-13000], rcx; store GLFW_NO_RESET_NOTIFICATION
		mov rax, [rbp-13000]; load GLFW_NO_RESET_NOTIFICATION
		mov rax, 200706
		push rax
		mov rax, [rbp-13008]; load GLFW_LOSE_CONTEXT_ON_RESET
		pop rcx
		mov [rbp-13008], rcx; store GLFW_LOSE_CONTEXT_ON_RESET
		mov rax, [rbp-13008]; load GLFW_LOSE_CONTEXT_ON_RESET
		mov rax, 0
		push rax
		mov rax, [rbp-13016]; load GLFW_OPENGL_ANY_PROFILE
		pop rcx
		mov [rbp-13016], rcx; store GLFW_OPENGL_ANY_PROFILE
		mov rax, [rbp-13016]; load GLFW_OPENGL_ANY_PROFILE
		mov rax, 204801
		push rax
		mov rax, [rbp-13024]; load GLFW_OPENGL_CORE_PROFILE
		pop rcx
		mov [rbp-13024], rcx; store GLFW_OPENGL_CORE_PROFILE
		mov rax, [rbp-13024]; load GLFW_OPENGL_CORE_PROFILE
		mov rax, 204802
		push rax
		mov rax, [rbp-13032]; load GLFW_OPENGL_COMPAT_PROFILE
		pop rcx
		mov [rbp-13032], rcx; store GLFW_OPENGL_COMPAT_PROFILE
		mov rax, [rbp-13032]; load GLFW_OPENGL_COMPAT_PROFILE
		mov rax, 208897
		push rax
		mov rax, [rbp-13040]; load GLFW_CURSOR
		pop rcx
		mov [rbp-13040], rcx; store GLFW_CURSOR
		mov rax, [rbp-13040]; load GLFW_CURSOR
		mov rax, 208898
		push rax
		mov rax, [rbp-13048]; load GLFW_STICKY_KEYS
		pop rcx
		mov [rbp-13048], rcx; store GLFW_STICKY_KEYS
		mov rax, [rbp-13048]; load GLFW_STICKY_KEYS
		mov rax, 208899
		push rax
		mov rax, [rbp-13056]; load GLFW_STICKY_MOUSE_BUTTONS
		pop rcx
		mov [rbp-13056], rcx; store GLFW_STICKY_MOUSE_BUTTONS
		mov rax, [rbp-13056]; load GLFW_STICKY_MOUSE_BUTTONS
		mov rax, 212993
		push rax
		mov rax, [rbp-13064]; load GLFW_CURSOR_NORMAL
		pop rcx
		mov [rbp-13064], rcx; store GLFW_CURSOR_NORMAL
		mov rax, [rbp-13064]; load GLFW_CURSOR_NORMAL
		mov rax, 212994
		push rax
		mov rax, [rbp-13072]; load GLFW_CURSOR_HIDDEN
		pop rcx
		mov [rbp-13072], rcx; store GLFW_CURSOR_HIDDEN
		mov rax, [rbp-13072]; load GLFW_CURSOR_HIDDEN
		mov rax, 212995
		push rax
		mov rax, [rbp-13080]; load GLFW_CURSOR_DISABLED
		pop rcx
		mov [rbp-13080], rcx; store GLFW_CURSOR_DISABLED
		mov rax, [rbp-13080]; load GLFW_CURSOR_DISABLED
		mov rax, 0
		push rax
		mov rax, [rbp-13088]; load GLFW_ANY_RELEASE_BEHAVIOR
		pop rcx
		mov [rbp-13088], rcx; store GLFW_ANY_RELEASE_BEHAVIOR
		mov rax, [rbp-13088]; load GLFW_ANY_RELEASE_BEHAVIOR
		mov rax, 217089
		push rax
		mov rax, [rbp-13096]; load GLFW_RELEASE_BEHAVIOR_FLUSH
		pop rcx
		mov [rbp-13096], rcx; store GLFW_RELEASE_BEHAVIOR_FLUSH
		mov rax, [rbp-13096]; load GLFW_RELEASE_BEHAVIOR_FLUSH
		mov rax, 217090
		push rax
		mov rax, [rbp-13104]; load GLFW_RELEASE_BEHAVIOR_NONE
		pop rcx
		mov [rbp-13104], rcx; store GLFW_RELEASE_BEHAVIOR_NONE
		mov rax, [rbp-13104]; load GLFW_RELEASE_BEHAVIOR_NONE
		mov rax, 221185
		push rax
		mov rax, [rbp-13112]; load GLFW_NATIVE_CONTEXT_API
		pop rcx
		mov [rbp-13112], rcx; store GLFW_NATIVE_CONTEXT_API
		mov rax, [rbp-13112]; load GLFW_NATIVE_CONTEXT_API
		mov rax, 221186
		push rax
		mov rax, [rbp-13120]; load GLFW_EGL_CONTEXT_API
		pop rcx
		mov [rbp-13120], rcx; store GLFW_EGL_CONTEXT_API
		mov rax, [rbp-13120]; load GLFW_EGL_CONTEXT_API
		mov rax, 221185
		push rax
		mov rax, [rbp-13128]; load GLFW_ARROW_CURSOR
		pop rcx
		mov [rbp-13128], rcx; store GLFW_ARROW_CURSOR
		mov rax, [rbp-13128]; load GLFW_ARROW_CURSOR
		mov rax, 221186
		push rax
		mov rax, [rbp-13136]; load GLFW_IBEAM_CURSOR
		pop rcx
		mov [rbp-13136], rcx; store GLFW_IBEAM_CURSOR
		mov rax, [rbp-13136]; load GLFW_IBEAM_CURSOR
		mov rax, 221187
		push rax
		mov rax, [rbp-13144]; load GLFW_CROSSHAIR_CURSOR
		pop rcx
		mov [rbp-13144], rcx; store GLFW_CROSSHAIR_CURSOR
		mov rax, [rbp-13144]; load GLFW_CROSSHAIR_CURSOR
		mov rax, 221188
		push rax
		mov rax, [rbp-13152]; load GLFW_HAND_CURSOR
		pop rcx
		mov [rbp-13152], rcx; store GLFW_HAND_CURSOR
		mov rax, [rbp-13152]; load GLFW_HAND_CURSOR
		mov rax, 221189
		push rax
		mov rax, [rbp-13160]; load GLFW_HRESIZE_CURSOR
		pop rcx
		mov [rbp-13160], rcx; store GLFW_HRESIZE_CURSOR
		mov rax, [rbp-13160]; load GLFW_HRESIZE_CURSOR
		mov rax, 221190
		push rax
		mov rax, [rbp-13168]; load GLFW_VRESIZE_CURSOR
		pop rcx
		mov [rbp-13168], rcx; store GLFW_VRESIZE_CURSOR
		mov rax, [rbp-13168]; load GLFW_VRESIZE_CURSOR
		mov rax, 262145
		push rax
		mov rax, [rbp-13176]; load GLFW_CONNECTED
		pop rcx
		mov [rbp-13176], rcx; store GLFW_CONNECTED
		mov rax, [rbp-13176]; load GLFW_CONNECTED
		mov rax, 262146
		push rax
		mov rax, [rbp-13184]; load GLFW_DISCONNECTED
		pop rcx
		mov [rbp-13184], rcx; store GLFW_DISCONNECTED
		mov rax, [rbp-13184]; load GLFW_DISCONNECTED
		mov rax, -1
		push rax
		mov rax, [rbp-13192]; load GLFW_DONT_CARE
		pop rcx
		mov [rbp-13192], rcx; store GLFW_DONT_CARE
		mov rax, [rbp-13192]; load GLFW_DONT_CARE
_drawTriangle:
		push rbp
		mov rbp, rsp
		sub rsp, 16; 12 alloc, 4 padding
.begin:
		movss [rbp-4], xmm0; store_r x
		movss [rbp-8], xmm1; store_r y
		movss [rbp-12], xmm2; store_r s
		mov rax, [rbp-208]; load GL_TRIANGLES
		push rax
		pop rdi
		call _glBegin
		movss xmm0, [rel d0]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d0]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rsp]
		add rsp, 8
		movsd xmm1, [rsp]
		add rsp, 8
		movsd xmm2, [rsp]
		add rsp, 8
		call _glColor3f
		movss xmm0, [rbp-12]; load s
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d2]; float_ref
		movd ecx, xmm0
		xor ecx, 2147483648
		movd xmm0, ecx
		movsd xmm1, [rsp]
		add rsp, 8
		mulss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-8]; load y
		movsd xmm1, [rsp]
		add rsp, 8
		addss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-12]; load s
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d2]; float_ref
		movd ecx, xmm0
		xor ecx, 2147483648
		movd xmm0, ecx
		movsd xmm1, [rsp]
		add rsp, 8
		mulss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-4]; load x
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
		movss xmm0, [rel d0]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d0]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rsp]
		add rsp, 8
		movsd xmm1, [rsp]
		add rsp, 8
		movsd xmm2, [rsp]
		add rsp, 8
		call _glColor3f
		movss xmm0, [rbp-12]; load s
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d2]; float_ref
		movsd xmm1, [rsp]
		add rsp, 8
		mulss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-8]; load y
		movsd xmm1, [rsp]
		add rsp, 8
		addss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-4]; load x
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rsp]
		add rsp, 8
		movsd xmm1, [rsp]
		add rsp, 8
		call _glVertex2f
		movss xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d0]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d0]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rsp]
		add rsp, 8
		movsd xmm1, [rsp]
		add rsp, 8
		movsd xmm2, [rsp]
		add rsp, 8
		call _glColor3f
		movss xmm0, [rbp-12]; load s
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d2]; float_ref
		movsd xmm1, [rsp]
		add rsp, 8
		mulss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-8]; load y
		movsd xmm1, [rsp]
		add rsp, 8
		subss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-12]; load s
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d2]; float_ref
		movsd xmm1, [rsp]
		add rsp, 8
		mulss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-4]; load x
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
		call _glEnd
.end:
		add rsp, 16; 12 alloc, 4 padding
		leave
		ret
_clamp0to1:
		push rbp
		mov rbp, rsp
		sub rsp, 16; 4 alloc, 12 padding
.begin:
		movss [rbp-4], xmm0; store_r x
		movss xmm0, [rbp-4]; load x
		movss xmm0, xmm0
		jmp .end
.end:
		add rsp, 16; 4 alloc, 12 padding
		leave
		ret
_main:
		push rbp
		mov rbp, rsp
		sub rsp, 64; 52 alloc, 12 padding
.begin:
		mov rax, d3; string_ref
		push rax
		mov rax, [rbp-8]; load WINDOW_TITLE
		pop rcx
		mov [rbp-8], rcx; store WINDOW_TITLE
		mov rax, [rbp-8]; load WINDOW_TITLE
		call _glfwInit
		cmp al, 0
		sete al
		cmp al, 0
		je .l1
		mov rax, d4; string_ref
		push rax
		pop rdi
		call _puts
		jmp .l1
.l0:
.l1:
		mov rax, 2
		push rax
		mov rax, [rbp-12888]; load GLFW_CONTEXT_VERSION_MAJOR
		push rax
		pop rdi
		pop rsi
		call _glfwWindowHint
		mov rax, 1
		push rax
		mov rax, [rbp-12896]; load GLFW_CONTEXT_VERSION_MINOR
		push rax
		pop rdi
		pop rsi
		call _glfwWindowHint
		mov rax, [rbp-104]; load NULL
		push rax
		mov rax, [rbp-104]; load NULL
		push rax
		mov rax, [rbp-8]; load WINDOW_TITLE
		push rax
		mov rax, 480
		push rax
		mov rax, 640
		push rax
		pop rdi
		pop rsi
		pop rdx
		pop rcx
		pop r8
		call _glfwCreateWindow
		push rax
		mov rax, [rbp-13208]; load window
		pop rcx
		mov [rbp-13208], rcx; store window
		mov rax, [rbp-13208]; load window
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		call _glfwMakeContextCurrent
		call _glewInit
		push rax
		mov rax, [rbp-11232]; load GLEW_OK
		pop rcx
		cmp rcx, rax
		setne al
		cmp al, 0
		je .l3
		mov rax, d5; string_ref
		push rax
		pop rdi
		call _puts
		jmp .l3
.l2:
.l3:
		mov rax, 1
		push rax
		mov rax, [rbp-13216]; load running
		pop rcx
		mov [rbp-13216], rcx; store running
		mov rax, [rbp-13216]; load running
		movss xmm0, [rel d6]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13220]; load r
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13220], xmm1; store r
		movss xmm0, [rbp-13220]; load r
		movss xmm0, [rel d6]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13224]; load g
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13224], xmm1; store g
		movss xmm0, [rbp-13224]; load g
		movss xmm0, [rel d6]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13228]; load b
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13228], xmm1; store b
		movss xmm0, [rbp-13228]; load b
		movss xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13232]; load a
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13232], xmm1; store a
		movss xmm0, [rbp-13232]; load a
		movss xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13236]; load x
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13236], xmm1; store x
		movss xmm0, [rbp-13236]; load x
		movss xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13240]; load y
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13240], xmm1; store y
		movss xmm0, [rbp-13240]; load y
		movss xmm0, [rel d1]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13244]; load scale
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13244], xmm1; store scale
		movss xmm0, [rbp-13244]; load scale
.l4:
		mov rax, [rbp-13216]; load running
		cmp al, 0
		je .l5
		mov rax, [rbp-11704]; load GLFW_KEY_R
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l7
		movss xmm0, [rel d7]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13220]; load r
		movsd xmm1, [rsp]
		add rsp, 8
		addss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13220]; load r
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13220], xmm1; store r
		movss xmm0, [rbp-13220]; load r
		jmp .l7
.l6:
.l7:
		mov rax, [rbp-11616]; load GLFW_KEY_G
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l9
		movss xmm0, [rel d7]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13224]; load g
		movsd xmm1, [rsp]
		add rsp, 8
		addss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13224]; load g
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13224], xmm1; store g
		movss xmm0, [rbp-13224]; load g
		jmp .l9
.l8:
.l9:
		mov rax, [rbp-11576]; load GLFW_KEY_B
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l11
		movss xmm0, [rel d7]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13228]; load b
		movsd xmm1, [rsp]
		add rsp, 8
		addss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13228]; load b
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13228], xmm1; store b
		movss xmm0, [rbp-13228]; load b
		jmp .l11
.l10:
.l11:
		mov rax, [rbp-11600]; load GLFW_KEY_E
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l13
		movss xmm0, [rel d7]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13220]; load r
		movsd xmm1, [rsp]
		add rsp, 8
		subss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13220]; load r
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13220], xmm1; store r
		movss xmm0, [rbp-13220]; load r
		jmp .l13
.l12:
.l13:
		mov rax, [rbp-11608]; load GLFW_KEY_F
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l15
		movss xmm0, [rel d7]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13224]; load g
		movsd xmm1, [rsp]
		add rsp, 8
		subss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13224]; load g
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13224], xmm1; store g
		movss xmm0, [rbp-13224]; load g
		jmp .l15
.l14:
.l15:
		mov rax, [rbp-11736]; load GLFW_KEY_V
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l17
		movss xmm0, [rel d7]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13228]; load b
		movsd xmm1, [rsp]
		add rsp, 8
		subss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13228]; load b
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13228], xmm1; store b
		movss xmm0, [rbp-13228]; load b
		jmp .l17
.l16:
.l17:
		movss xmm0, [rbp-13220]; load r
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rsp]
		add rsp, 8
		call _clamp0to1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13220]; load r
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13220], xmm1; store r
		movss xmm0, [rbp-13220]; load r
		movss xmm0, [rbp-13224]; load g
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rsp]
		add rsp, 8
		call _clamp0to1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13224]; load g
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13224], xmm1; store g
		movss xmm0, [rbp-13224]; load g
		movss xmm0, [rbp-13228]; load b
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rsp]
		add rsp, 8
		call _clamp0to1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13228]; load b
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13228], xmm1; store b
		movss xmm0, [rbp-13228]; load b
		movss xmm0, [rbp-13232]; load a
		sub rsp, 8
		movsd [rsp], xmm0
		movsd xmm0, [rsp]
		add rsp, 8
		call _clamp0to1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13232]; load a
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13232], xmm1; store a
		movss xmm0, [rbp-13232]; load a
		movss xmm0, [rbp-13232]; load a
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13228]; load b
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13224]; load g
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13220]; load r
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
		call _glClearColor
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		call _glfwWindowShouldClose
		cmp al, 0
		je .l19
		mov rax, 0
		push rax
		mov rax, [rbp-13216]; load running
		pop rcx
		mov [rbp-13216], rcx; store running
		mov rax, [rbp-13216]; load running
		jmp .l19
.l18:
.l19:
		mov rax, [rbp-11776]; load GLFW_KEY_ESCAPE
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l21
		mov rax, 0
		push rax
		mov rax, [rbp-13216]; load running
		pop rcx
		mov [rbp-13216], rcx; store running
		mov rax, [rbp-13216]; load running
		jmp .l21
.l20:
.l21:
		mov rax, [rbp-168]; load GL_COLOR_BUFFER_BIT
		push rax
		pop rdi
		call _glClear
		call _glfwPollEvents
		mov rax, [rbp-11848]; load GLFW_KEY_UP
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l23
		movss xmm0, [rel d8]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13240]; load y
		movsd xmm1, [rsp]
		add rsp, 8
		addss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13240]; load y
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13240], xmm1; store y
		movss xmm0, [rbp-13240]; load y
		jmp .l23
.l22:
.l23:
		mov rax, [rbp-11840]; load GLFW_KEY_DOWN
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l25
		movss xmm0, [rel d8]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13240]; load y
		movsd xmm1, [rsp]
		add rsp, 8
		subss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13240]; load y
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13240], xmm1; store y
		movss xmm0, [rbp-13240]; load y
		jmp .l25
.l24:
.l25:
		mov rax, [rbp-11824]; load GLFW_KEY_RIGHT
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l27
		movss xmm0, [rel d8]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13236]; load x
		movsd xmm1, [rsp]
		add rsp, 8
		addss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13236]; load x
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13236], xmm1; store x
		movss xmm0, [rbp-13236]; load x
		jmp .l27
.l26:
.l27:
		mov rax, [rbp-11832]; load GLFW_KEY_LEFT
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l29
		movss xmm0, [rel d8]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13236]; load x
		movsd xmm1, [rsp]
		add rsp, 8
		subss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13236]; load x
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13236], xmm1; store x
		movss xmm0, [rbp-13236]; load x
		jmp .l29
.l28:
.l29:
		mov rax, [rbp-11768]; load GLFW_KEY_Z
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l31
		movss xmm0, [rel d6]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13244]; load scale
		movsd xmm1, [rsp]
		add rsp, 8
		subss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13244]; load scale
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13244], xmm1; store scale
		movss xmm0, [rbp-13244]; load scale
		jmp .l31
.l30:
.l31:
		mov rax, [rbp-11696]; load GLFW_KEY_Q
		push rax
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		pop rsi
		call _glfwGetKey
		push rax
		mov rax, [rbp-11352]; load GLFW_PRESS
		pop rcx
		cmp rcx, rax
		sete al
		cmp al, 0
		je .l33
		movss xmm0, [rel d6]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13244]; load scale
		movsd xmm1, [rsp]
		add rsp, 8
		addss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13244]; load scale
		movsd xmm1, [rsp]
		add rsp, 8
		movss [rbp-13244], xmm1; store scale
		movss xmm0, [rbp-13244]; load scale
		jmp .l33
.l32:
.l33:
		movss xmm0, [rbp-13244]; load scale
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d6]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13240]; load y
		movsd xmm1, [rsp]
		add rsp, 8
		mulss xmm0, xmm1
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rel d6]; float_ref
		sub rsp, 8
		movsd [rsp], xmm0
		movss xmm0, [rbp-13236]; load x
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
		call _drawTriangle
		mov rax, [rbp-13208]; load window
		push rax
		pop rdi
		call _glfwSwapBuffers
		jmp .l4
.l5:
		mov rax, 1
		mov rax, rax
		jmp .end
.end:
		mov rax, d9; string_ref
		push rax
		pop rdi
		call _puts
		call _glfwTerminate
		add rsp, 64; 52 alloc, 12 padding
		leave
		ret
