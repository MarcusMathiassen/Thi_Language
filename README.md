# Thi_Language

# Basic example
load "includes/cstdio"
load "includes/cstdlib"
load "includes/opengl"
load "includes/glew"
load "includes/glfw"
load "includes/cmath"

        enum Theme
            dark
            light

        main()
            if !glfwInit() puts("GLFW init failed.")
            defer glfwTerminate()

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2)
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1)

            glfwWindowHint(GLFW_SAMPLES, 4)
            glfwWindowHint(GLFW_RED_BITS, 8)
            glfwWindowHint(GLFW_GREEN_BITS, 8)
            glfwWindowHint(GLFW_BLUE_BITS, 8)
            glfwWindowHint(GLFW_ALPHA_BITS, 8)

            window := glfwCreateWindow(640, 480, "Hello Triangle", 0, 0)
            glfwMakeContextCurrent(window)

            if glfwInit() != GLEW_OK puts("GLEW init failed.")

            r := 0.0
            g := 0.0
            b := 0.0

            mode: Theme = .dark

            if mode

                is .dark
                    r = 30.0/255.0
                    g = 30.0/255.0
                    b = 30.0/255.0

                is .light
                    r = 245.0/255.0
                    g = 245.0/255.0
                    b = 245.0/255.0

            glClearColor(r, g, b, 1.0)

            shorten_filenames_in_error_messages : bool = false
            warnings_is_errors := false

            x := 0.0
            y := 0.0
            scale := 1.0

            running := true


            while running

                if glfwWindowShouldClose(window) running = false
                if glfwGetKey(window, GLFW_KEY_ESCAPE) is GLFW_PRESS  running = false

                glClear(GL_COLOR_BUFFER_BIT)
                glfwPollEvents()

                key := glfwGetKey(window, GLFW_KEY_LEFT)

                # This is a switch, kinda..
                if key is GLFW_PRESS x += -0.01

                if glfwGetKey(window, GLFW_KEY_RIGHT) is GLFW_PRESS x +=  0.01
                if glfwGetKey(window, GLFW_KEY_DOWN) is GLFW_PRESS y += -0.01
                if glfwGetKey(window, GLFW_KEY_UP) is GLFW_PRESS y +=  0.01

                if glfwGetKey(window, GLFW_KEY_X) is GLFW_PRESS scale +=  0.01
                if glfwGetKey(window, GLFW_KEY_Z) is GLFW_PRESS scale +=  -0.01

                drawTriangle(x, y, scale)

                glfwSwapBuffers(window)

            return 1

        drawTriangle(x: f32, y: f32, s: f32)
            glBegin(GL_TRIANGLES)
            glColor3f(1.0, 0.0, 0.0)
            glVertex2f(x + -0.5 *s, y + -0.5*s )
            glColor3f(0.0, 1.0, 0.0)
            glVertex2f(x + 0.0*s, y + 0.5*s)
            glColor3f(0.0, 0.0, 1.0)
            glVertex2f(x + 0.5*s, y -0.5*s)
            glEnd()
# Syntax
    Constant variable def <=>        <ident> :: <expr>
    Function def <=> def <ident> (<variable_decl>..) -> <ret_type>
    Variable decl <=>                <ident> : <type>
    Variable decl with assignmen <=> <ident> : <type> = <expr>
    Variable decl type inf <=>       <ident> := <expr>

    Control flow
    if <=> if <cond>
    for <=> for <init>, <cond>, <inc>
    while <=> while <cond>

# Not actually a readme but a todo scratchpad
- [x] integer literals
- [x] string literals
- [x] char literals
- [x] functions
- [x] function calls
- [x] multiple compilation files using 'load'
- [x] C stdlib interop

- [x] 'link' for adding linking options for the linker

- [x] variables

- [x] s32
- [x] char
- [x] pointer

- [ ] subscript access

- [x] structs
- [ ] field access

- [x] enums

- [x] if
- [x] for
- [x] 'do' lets you do single line statements 
- [x] defer
- [x] while
- [x] break
- [x] return
- [x] continue

- [x] switch
- [x] case
