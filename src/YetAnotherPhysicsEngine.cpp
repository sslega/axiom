#include <iostream>
#include "GLFW/glfw3.h"

int main() {
    // std::cout << "Hello, World!" << std::endl;
    if (!glfwInit()) 
    {
        std::cerr << "Nie udało się zainicjalizować GLFW\n";
        return -1;
    }

    // Tworzenie okna
    GLFWwindow* window = glfwCreateWindow(800, 600, "Moje Okno", NULL, NULL);
    if (!window) {
        std::cerr << "Nie udało się stworzyć okna\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // Główna pętla
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Renderowanie obiektów

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}