#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <stdlib.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glext.h>

// Pieced together from several websites:
// https://gist.github.com/dlivingstone/628320
// https://vec.io/posts/head-to-opengl-hello-triangle
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/

// For nVidia Optimus laptops, try to use the nVidia graphics.
extern _declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

// These are not defined in any of our headers
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094

static const char* vertex_shader =
"#version 330\n"
"in vec3 vp;"
"void main () {"
"  gl_Position = vec4 (vp, 1.0);"
"}";

static const char* fragment_shader =
"#version 330\n"
"out vec4 frag_colour;"
"void main () {"
"  frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
"}";

// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
};

static GLuint g_program;
static HDC g_hDC;
static HGLRC g_context;
static GLuint g_vertexArrayID;
static GLuint g_vertexBuffer;

static 

void prepareScene(void)
{
    PFNGLCREATESHADERPROC glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");

    if (glCreateShader == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glCreateShader.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLSHADERSOURCEPROC glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");

    if (glShaderSource == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glShaderSource.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLCOMPILESHADERPROC glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");

    if (glCompileShader == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glCompileShader.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLCREATEPROGRAMPROC glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");

    if (glCreateProgram == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glCreateProgram.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLATTACHSHADERPROC glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");

    if (glAttachShader == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glAttachShader.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLLINKPROGRAMPROC glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");

    if (glLinkProgram == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glLinkProgram.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");

    if (glCreateShader == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glGetAttribLocation.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLGENBUFFERSPROC glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");

    if (glGenBuffers == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glGenBuffers.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLBINDBUFFERPROC glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");

    if (glBindBuffer == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glBindBuffer.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLBUFFERDATAPROC glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");

    if (glBufferData == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glBufferData.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLGETSHADERIVPROC glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");

    if (glGetShaderiv == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glGetShaderiv.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)wglGetProcAddress("glGetShaderInfoLog");

    if (glGetShaderInfoLog == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glGetShaderInfoLog.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLGETPROGRAMIVPROC glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");

    if (glGetProgramiv == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glGetProgramiv.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)wglGetProcAddress("glGetProgramInfoLog");

    if (glGetProgramInfoLog == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glGetProgramInfoLog.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLUSEPROGRAMPROC glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
    if (glUseProgram == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glUseProgram.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)wglGetProcAddress("glGenVertexArrays");
    if (glGenVertexArrays == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glGenVertexArrays.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLBINDVERTEXARRAYPROC glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)wglGetProcAddress("glBindVertexArray");
    if (glBindVertexArray == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glBindVertexArray.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    GLint status;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, (const GLchar **)&vertex_shader, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        char *infoLog = malloc(logLength * sizeof(char));
        glGetShaderInfoLog(vertexShader, logLength, NULL, infoLog);
        MessageBox(NULL, (char*)infoLog, "Vertex Shader Status", MB_OK | MB_ICONERROR);
        free(infoLog);
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, (const GLchar **)&fragment_shader, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        char *infoLog = malloc(logLength * sizeof(char));
        glGetShaderInfoLog(fragmentShader, logLength, NULL, infoLog);
        MessageBox(NULL, (char*)infoLog, "Fragment Shader Status", MB_OK | MB_ICONERROR);
        free(infoLog);
    }

    g_program = glCreateProgram();
    glAttachShader(g_program, vertexShader);
    glAttachShader(g_program, fragmentShader);
    glLinkProgram(g_program);

    glGetProgramiv(g_program, GL_LINK_STATUS, &status);

    if (status == GL_FALSE)
    {
        GLint logLength;
        glGetProgramiv(g_program, GL_INFO_LOG_LENGTH, &logLength);
        char *infoLog = malloc(logLength * sizeof(char));
        glGetProgramInfoLog(g_program, logLength, NULL, infoLog);
        //fprintf(stderr, "%d: %d, %s\n", __LINE__, S, infoLog);
        MessageBox(NULL, (char*)infoLog, "Program Status", MB_OK | MB_ICONERROR);
        free(infoLog);
    }

    glUseProgram(g_program);

    // create VAO and set it as the current one  
    glGenVertexArrays(1, &g_vertexArrayID);
    glBindVertexArray(g_vertexArrayID);

    // Generate 1 buffer, put the resulting identifier in g_vertexBuffer
    glGenBuffers(1, &g_vertexBuffer);
    // The following commands will talk about our 'g_vertexBuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    for (GLenum err = glGetError(); err != GL_NO_ERROR; err = glGetError()) {
        MessageBox(NULL, (char*)gluErrorString(err), "Error during init", MB_OK | MB_ICONERROR);
    }
}

void display(void)
{
    PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");

    if (glEnableVertexAttribArray == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glEnableVertexAttribArray.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

    if (glVertexAttribPointer == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glVertexAttribPointer.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glDisableVertexAttribArray");

    if (glDisableVertexAttribArray == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glDisableVertexAttribArray.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    PFNGLBINDBUFFERPROC glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");

    if (glBindBuffer == NULL)
    {
        //GetLastError();
        MessageBox(NULL, "Unable to load glBindBuffer.", "Error", MB_OK | MB_ICONERROR);
        return;
    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, g_vertexBuffer);
    glVertexAttribPointer(
         0,                                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
         3,                                  // size
         GL_FLOAT,                   // type
         GL_FALSE,                   // normalized?
         0,                                  // stride
         (void*)0                        // array buffer offset
    );

    // Draw the triangle
    glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
    glDisableVertexAttribArray(0);
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    wc.cbSize           = sizeof(WNDCLASSEX);
    wc.style            = CS_OWNDC;
    wc.lpfnWndProc      = WndProc;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = NULL; //(HBRUSH)(COLOR_BACKGROUND);
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "hellotriangle";
    wc.hIconSm          = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wc))
    {
        // Window registration failed
        return 1;
    }

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        wc.lpszClassName,
        "HelloTriangle: OpenGL Test",
        WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        // Window creation failed
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (1) // GetMessage(&msg, NULL, 0, 0) > 0
    { 
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                break;
            }
        }
        else
        {
            display();
            SwapBuffers(g_hDC);
        }
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static PAINTSTRUCT ps;

    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hwnd, &ps);
            EndPaint(hwnd, &ps);
        break;
        case WM_SIZE:
            reshape(LOWORD(lParam), HIWORD(lParam));
            PostMessage(hwnd, WM_PAINT, 0, 0);
        break;
        case WM_DESTROY:
            // Perform cleanup tasks.
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(g_context);
            ReleaseDC(hwnd, g_hDC);
            PostQuitMessage(0);
        break;
        case WM_CREATE:
        {
            PIXELFORMATDESCRIPTOR pfd =
            {
                sizeof(PIXELFORMATDESCRIPTOR),
                1,
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,      //Flags
                PFD_TYPE_RGBA,                      //The kind of framebuffer. RGBA or palette.
                32,                                              //Colordepth of the framebuffer.
                0, 0, 0, 0, 0, 0,
                0,
                0,
                0,
                0, 0, 0, 0,
                24,                                              //Number of bits for the depthbuffer
                8,                                              //Number of bits for the stencilbuffer
                0,                                              //Number of Aux buffers in the framebuffer.
                PFD_MAIN_PLANE,
                0,
                0, 0, 0
            };

            g_hDC = GetDC(hwnd);

            if (g_hDC == NULL)
            {
                MessageBox(NULL, "GetDC failed.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            int iPixelFormat = ChoosePixelFormat(g_hDC, &pfd);

            if (iPixelFormat == 0)
            {
                //GetLastError();
                MessageBox(NULL, "ChoosePixelFormat failed.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            BOOL result = SetPixelFormat(g_hDC, iPixelFormat, &pfd);

            if (result == FALSE)
            {
                //GetLastError();
                MessageBox(NULL, "SetPixelFormat failed.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            HGLRC tempContext = wglCreateContext(g_hDC);

            if (tempContext == NULL)
            {
                //GetLastError();
                MessageBox(NULL, "wglCreateContext failed.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            result = wglMakeCurrent(g_hDC, tempContext);

            if (result == FALSE)
            {
                //GetLastError();
                MessageBox(NULL, "wglMakeCurrent failed.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            int attribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_FLAGS_ARB, 0,
                0
            };

            PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");

            g_context = wglCreateContextAttribsARB(g_hDC, 0, attribs);
            result = wglMakeCurrent(NULL,NULL);

            if (result == FALSE)
            {
                //GetLastError();
                MessageBox(NULL, "wglMakeCurrent failed.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            result = wglDeleteContext(tempContext);

            if (result == FALSE)
            {
                //GetLastError();
                MessageBox(NULL, "wglDeleteContext failed.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            result = wglMakeCurrent(g_hDC, g_context);

            if (result == FALSE)
            {
                //GetLastError();
                MessageBox(NULL, "wglMakeCurrent failed.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            MessageBox(NULL, (char*)glGetString(GL_VERSION), "OpenGL Version", MB_OK);

            if (!g_context)
            {
                MessageBox(NULL, "g_context empty.", "Error", MB_OK | MB_ICONERROR);
                break;
            }

            prepareScene();
        }
        break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;

}
