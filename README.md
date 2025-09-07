# OpenGL
Computer Graphics, using OpenGL, covering basic rendering, shaders, and transformations.

## Requirements
- MSYS2 32-bit terminal (for MinGW 32-bit toolchain)
- OpenGL library
- GLFW and GLEW libraries
- GLUT (OpenGL Utility Toolkit) library
- **`make`** (for building the project)

## Setup Instructions

### Step 1: Install MSYS2 and MinGW
Make sure you have MSYS2 installed with the MinGW 32-bit toolchain. You can install it using the following command in your MSYS2 terminal:

```bash
pacman -S mingw-w64-i686-toolchain mingw-w64-i686-freeglut make
```

## About GLUT GLFW AND GLEW

### **What is GLUT?**

**GLUT** (OpenGL Utility Toolkit) is a library that provides a simplified interface for creating and managing windows that use OpenGL for rendering. It also handles input from devices like the keyboard and mouse.

#### Main Features of GLUT:
1. **Window Creation**: GLUT abstracts the complexities of window creation across different platforms, making it easier to create OpenGL contexts.
2. **Input Handling**: It manages user input, including keyboard and mouse events, in a cross-platform way.
3. **Utility Functions**: GLUT includes functions for creating shapes like spheres and cubes, managing time, and handling animation loops.

**Use Case**: GLUT is ideal for beginners learning OpenGL, as it simplifies a lot of the boilerplate code necessary to set up windows and handle input. However, it is somewhat outdated and lacks advanced features for more complex or performance-critical applications.

---

### **What is GLEW?**

**GLEW** (OpenGL Extension Wrangler Library) is a utility that helps developers access OpenGL extensions, which are new features and capabilities introduced by graphics card manufacturers. OpenGL itself doesn’t always expose these extensions directly, so GLEW simplifies the process of checking for available extensions and using them.

#### Main Features of GLEW:
1. **Extension Loading**: GLEW automatically loads and makes OpenGL extensions available.
2. **Compatibility**: GLEW works across multiple platforms, ensuring that OpenGL applications can access the latest features of the installed hardware.

**Use Case**: GLEW is necessary when you're working with more modern OpenGL versions and you need access to functionality beyond the base OpenGL API, like newer shaders, buffer objects, or advanced rendering techniques.

---

### **What is GLFW?**

**GLFW** is another library, similar to **GLUT**, used for managing window creation, input, and context handling for OpenGL applications. However, GLFW is a more modern alternative to GLUT and provides better control and flexibility, particularly for game development or performance-critical applications.

#### Main Features of GLFW:
1. **Window and OpenGL Context Creation**: Like GLUT, GLFW makes it easy to create windows that support OpenGL rendering. However, it gives you more control over how you set up your OpenGL context (e.g., OpenGL version, core or compatibility profile, etc.).
2. **Input Handling**: GLFW handles keyboard, mouse, and even joystick input in a cross-platform way. It provides more detailed control over input, including handling multiple input devices.
3. **Multiple Window Support**: GLFW allows you to open multiple windows with different OpenGL contexts, which is something GLUT does not support well.
4. **Advanced Features**: It supports modern game development features like fullscreen modes, high-DPI monitors, multiple monitors, and fine-tuned control over refresh rates and screen resolution.
5. **Thread Safety and Modern OpenGL**: GLFW is thread-safe and supports modern OpenGL features better than GLUT.

---

### **How They Relate to OpenGL**

- **OpenGL**: OpenGL is a cross-platform API used for rendering 2D and 3D vector graphics. It is powerful but very low-level, so it doesn’t handle window management or input devices.

- **GLUT**: Acts as a higher-level toolkit that makes it easier to work with OpenGL by managing window creation, input, and basic utility functions. It's mainly useful for learning OpenGL or for simple OpenGL applications.

- **GLFW**: Provides more advanced and flexible options than GLUT for creating OpenGL contexts, handling input, and working with multiple monitors and windows. It is better suited for performance-critical applications, like games.

- **GLEW**: GLEW is used to access modern OpenGL extensions that aren’t always available in the base OpenGL API. It works with both GLUT and GLFW to load the latest OpenGL features supported by the hardware.

### **Comparison**:

| Feature               | GLUT                       | GLFW                       | GLEW                          |
|-----------------------|----------------------------|----------------------------|-------------------------------|
| **Window Creation**    | Yes                        | Yes (more advanced)         | No (used with windowing libraries) |
| **Input Handling**     | Basic (keyboard, mouse)     | Advanced (keyboard, mouse, joystick, multi-monitor) | No |
| **Multiple Windows**   | No                         | Yes                        | No |
| **Modern OpenGL**      | Limited support             | Full support                | Accesses modern OpenGL extensions |
| **Extension Handling** | No                         | No                         | Yes (loads OpenGL extensions) |
| **Best for**           | Learning, prototyping       | Games, complex applications | Accessing advanced OpenGL features |

---

### **When to Use Each:**

- **Use GLUT** when you want an easy way to create windows and handle input for basic OpenGL applications. It’s perfect for tutorials, educational projects, or quick prototypes.
- **Use GLFW** when you need more control and flexibility for creating windows, handling input, or working with multiple monitors and modern OpenGL versions. It’s particularly useful for game development or high-performance applications.
- **Use GLEW** when you’re working with advanced OpenGL features, especially when targeting different hardware or when using modern OpenGL versions that require extensions not available in the basic API.

In many real-world OpenGL projects, **GLFW + GLEW** is a common combination: GLFW handles windowing and input, while GLEW ensures that your application can access the latest OpenGL features. 

---

## 🔑 What an **OpenGL Context** is

* OpenGL by itself is **just a specification** — it describes a huge set of functions (`glClearColor`, `glDrawArrays`, etc.), but it doesn’t exist as a normal library you just call into.
* Those functions are actually implemented in your **GPU driver**.
* To talk to that driver, you need to create a **context**:
  👉 A context is basically a "state machine" object that holds *all* of OpenGL’s state — buffers, textures, shaders, current settings.
  Without a context, there’s no OpenGL state to call functions against, so `gl*` functions would do nothing (or crash).

Think of it as:

* The **context = OpenGL’s workspace**.
* You *must* have one before you can issue any draw calls.

---

## 🔎 Who creates the context?

OpenGL itself doesn’t create contexts — it relies on **windowing systems or helper libraries**:

* On **Windows**, contexts are created via the Win32 API (`wglCreateContext`).
* On **Linux**, contexts are created with GLX (X11) or EGL (Wayland).
* On **macOS**, via CGL or NSOpenGL.

Since that’s a pain to do manually, we use libraries:

* **GLFW / GLUT / SDL** → create a **window** *and* an **OpenGL context**.
* **GLEW / GLAD** → *do not* create contexts. They just **load function pointers** for modern OpenGL features *after* a context exists.

---

## ⚡ So in terms:

* **The context is not GLEW.**
* **The context is created by GLFW** (or GLUT, or SDL, etc.).

Sequence looks like this:

1. You ask GLFW to open a window (`glfwCreateWindow`).
2. GLFW creates a **window + OpenGL context**.
3. You make that context “current” (`glfwMakeContextCurrent`).
4. Only *after that* you initialize **GLEW** → `glewInit()`.

   * Because GLEW queries the driver for all the `gl*` functions, which won’t exist until a context is active.

---

## ✅ Analogy

Think of it like this:

* **OpenGL context** = the actual *workspace/desk* where you do your work.
* **GLFW** = the carpenter who builds the desk (and the room around it = the window).
* **GLEW** = the catalog that tells you which tools are available on that desk.

No desk (context) → nowhere to work.
No carpenter (GLFW/GLUT) → no desk gets built.
No catalog (GLEW) → you have a desk but don’t know which tools are in the drawers.

---
