// Autor: ML
// // Opis: RADIO

#define _CRT_SECURE_NO_WARNINGS
#define CRES 40

#include <iostream>
#include <fstream>
#include <sstream>


#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const float M_PI = 3.14159265358979323846;

float membraneSize = 0.22f;
float volume = 0.5f;
// Slider parameters
float sliderWidth = 0.4f;
float sliderHeight = 0.1f;
float buttonWidth = 0.02f;

//---------------- COMPONENTS ----------------------

void drawLoudspeaker(float xOffset, float yOffset) {
    std::vector<float> loudspeakerVertices;
    std::vector<unsigned int> loudspeakerIndices;

    // Loudspeaker parameters
    int numPoints = 100;
    float radius = membraneSize + 0.02f;
    float dotSpacing = 0.05f;

    // Center of the protective net
    loudspeakerVertices.push_back(-0.8f + xOffset);
    loudspeakerVertices.push_back(0.0f + yOffset);

    for (int i = 0; i <= numPoints; ++i) {
        float angle = static_cast<float>(i) / numPoints * 2.0f * M_PI;
        float x_protective = -0.8f + xOffset + radius * cos(angle);
        float y_protective = yOffset + radius * sin(angle);

        // Protective net points
        loudspeakerVertices.push_back(x_protective);
        loudspeakerVertices.push_back(y_protective);

        float x_membrane = -0.8f + xOffset + membraneSize * cos(angle);
        float y_membrane = yOffset + membraneSize * sin(angle);

        // Loudspeaker membrane points
        loudspeakerVertices.push_back(x_membrane);
        loudspeakerVertices.push_back(y_membrane);

        // Line indices for the protective net
        loudspeakerIndices.push_back(0);
        loudspeakerIndices.push_back(i * 2 + 1);

        // Line indices for the loudspeaker membrane
        if (i % 5 == 0) {
            // Draw dotted lines by skipping every 5th point
            loudspeakerIndices.push_back(i * 2 + 1);
            loudspeakerIndices.push_back((i * 2 + 3) % (numPoints * 2 + 2));
        }
    }

    // Draw the loudspeaker
    unsigned int loudspeakerVAO, loudspeakerVBO, loudspeakerEBO;
    glGenVertexArrays(1, &loudspeakerVAO);
    glBindVertexArray(loudspeakerVAO);

    glGenBuffers(1, &loudspeakerVBO);
    glBindBuffer(GL_ARRAY_BUFFER, loudspeakerVBO);
    glBufferData(GL_ARRAY_BUFFER, loudspeakerVertices.size() * sizeof(float), loudspeakerVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &loudspeakerEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, loudspeakerEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loudspeakerIndices.size() * sizeof(unsigned int), loudspeakerIndices.data(), GL_STATIC_DRAW);

    glDrawElements(GL_LINES, loudspeakerIndices.size(), GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &loudspeakerVBO);
    glDeleteVertexArrays(1, &loudspeakerVAO);
    glDeleteBuffers(1, &loudspeakerEBO);
}

void drawPowerButton(float xOffset, float yOffset, bool isPowerOn) {
    // Power button body
    std::vector<float> buttonVertices;
    std::vector<unsigned int> buttonIndices;
    //glColor3f(0.4f, 0.4f, 0.4f); // Dark gray color for the power button body

    // Adjust size based on power state
    float buttonRadius = isPowerOn ? 0.04f : 0.05f;

    buttonVertices.push_back(-0.3f + xOffset);
    buttonVertices.push_back(0.05f + yOffset);

    for (int i = 0; i <= 360; ++i) {
        float angle = static_cast<float>(i) * M_PI / 180.0f;
        float x = -0.3f + xOffset + buttonRadius * cos(angle);
        float y = 0.05f + yOffset + buttonRadius * sin(angle);

        buttonVertices.push_back(x);
        buttonVertices.push_back(y);
    }

    // Convert vertices to indices for GL_TRIANGLE_FAN
    for (unsigned int i = 1; i <= 360; ++i) {
        buttonIndices.push_back(0);
        buttonIndices.push_back(i);
        buttonIndices.push_back(i + 1);
    }

    // Draw the power button
    unsigned int buttonVAO, buttonVBO, buttonEBO;
    glGenVertexArrays(1, &buttonVAO);
    glBindVertexArray(buttonVAO);

    glGenBuffers(1, &buttonVBO);
    glBindBuffer(GL_ARRAY_BUFFER, buttonVBO);
    glBufferData(GL_ARRAY_BUFFER, buttonVertices.size() * sizeof(float), buttonVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &buttonEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buttonEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, buttonIndices.size() * sizeof(unsigned int), buttonIndices.data(), GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, buttonIndices.size(), GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &buttonVBO);
    glDeleteVertexArrays(1, &buttonVAO);
    glDeleteBuffers(1, &buttonEBO);
}

void drawAntenna(float xOffset, float yOffset, bool isAntennaOut) {
    if (isAntennaOut) {
        std::vector<float> antennaVertices;
        std::vector<unsigned int> antennaIndices;

        int numPoints = 100;
        float antennaLength = 0.2f; // Adjust the antenna length as needed
        float radius = 0.025f; // Adjust the radius as needed

        // Antenna line
        antennaVertices.push_back(0.25f + xOffset);
        antennaVertices.push_back(0.1f + yOffset);
        antennaVertices.push_back(0.25f + xOffset);
        antennaVertices.push_back(0.1f + antennaLength + yOffset);

        // Antenna circle
        for (int i = 0; i <= numPoints; ++i) {
            float angle = static_cast<float>(i) / numPoints * 2.0f * M_PI;
            float x = 0.25f + xOffset + radius * cos(angle);
            float y = (0.1f + antennaLength) + yOffset + radius * sin(angle);

            antennaVertices.push_back(x);
            antennaVertices.push_back(y);
        }

        // Convert vertices to indices for GL_LINES and GL_TRIANGLE_FAN
        for (unsigned int i = 0; i < antennaVertices.size() / 2 - 2; ++i) {
            antennaIndices.push_back(i);
            antennaIndices.push_back(i + 1);
        }

        // Draw the antenna
        unsigned int antennaVAO, antennaVBO, antennaEBO;
        glGenVertexArrays(1, &antennaVAO);
        glBindVertexArray(antennaVAO);

        glGenBuffers(1, &antennaVBO);
        glBindBuffer(GL_ARRAY_BUFFER, antennaVBO);
        glBufferData(GL_ARRAY_BUFFER, antennaVertices.size() * sizeof(float), antennaVertices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glGenBuffers(1, &antennaEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, antennaEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, antennaIndices.size() * sizeof(unsigned int), antennaIndices.data(), GL_STATIC_DRAW);

        glDrawElements(GL_LINES, antennaIndices.size(), GL_UNSIGNED_INT, 0);

        // Cleanup
        glDeleteBuffers(1, &antennaVBO);
        glDeleteVertexArrays(1, &antennaVAO);
        glDeleteBuffers(1, &antennaEBO);
    }
}

void drawStationIndicator(float xOffset, float yOffset, float indicatorPosition) {
    std::vector<float> indicatorVertices;
    std::vector<unsigned int> indicatorIndices;

    // Draw the scale divisions
    glColor3f(0.0f, 0.0f, 0.0f); // Black color for scale divisions
    for (float angle = 0.0f; angle <= 180.0f; angle += 10.0f) {
        float startX = xOffset - 0.2f * cosf(angle * (M_PI / 180.0f));
        float startY = yOffset + 0.2f * sinf(angle * (M_PI / 180.0f));
        float endX = xOffset - 0.15f * cosf(angle * (M_PI / 180.0f));
        float endY = yOffset + 0.15f * sinf(angle * (M_PI / 180.0f));

        indicatorVertices.push_back(startX);
        indicatorVertices.push_back(startY);

        indicatorVertices.push_back(endX);
        indicatorVertices.push_back(endY);
    }

    // Draw the movable indicator pointer
    glColor3f(1.0f, 0.0f, 0.0f); // Red color for the indicator pointer
    float indicatorX = xOffset - 0.13f * cosf(indicatorPosition);
    float indicatorY = yOffset + 0.13f * sinf(indicatorPosition);

    indicatorVertices.push_back(xOffset);
    indicatorVertices.push_back(yOffset);

    indicatorVertices.push_back(indicatorX);
    indicatorVertices.push_back(indicatorY);

    indicatorVertices.push_back(xOffset);
    indicatorVertices.push_back(yOffset + 0.02f);

    // Convert vertices to indices for GL_LINES
    for (unsigned int i = 0; i < indicatorVertices.size() / 2 - 1; ++i) {
        indicatorIndices.push_back(i);
        indicatorIndices.push_back(i + 1);
    }

    // Draw the station indicator
    unsigned int indicatorVAO, indicatorVBO, indicatorEBO;
    glGenVertexArrays(1, &indicatorVAO);
    glBindVertexArray(indicatorVAO);

    glGenBuffers(1, &indicatorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, indicatorVBO);
    glBufferData(GL_ARRAY_BUFFER, indicatorVertices.size() * sizeof(float), indicatorVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &indicatorEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicatorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicatorIndices.size() * sizeof(unsigned int), indicatorIndices.data(), GL_STATIC_DRAW);

    glDrawElements(GL_LINES, indicatorIndices.size(), GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &indicatorVBO);
    glDeleteVertexArrays(1, &indicatorVAO);
    glDeleteBuffers(1, &indicatorEBO);
}

void drawSliderBody(float xOffset, float yOffset) {
    std::vector<float> bodyVertices = {
        -0.6f + xOffset, -0.3f + yOffset,
        -0.2f + xOffset, -0.3f + yOffset,
        -0.2f + xOffset, -0.2f + yOffset,
        -0.6f + xOffset, -0.2f + yOffset
    };

    unsigned int bodyIndices[] = { 0, 1, 2, 0, 2, 3 };

    // Draw the slider body
    unsigned int bodyVAO, bodyVBO, bodyEBO;
    glGenVertexArrays(1, &bodyVAO);
    glBindVertexArray(bodyVAO);

    glGenBuffers(1, &bodyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bodyVBO);
    glBufferData(GL_ARRAY_BUFFER, bodyVertices.size() * sizeof(float), bodyVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &bodyEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodyEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bodyIndices), bodyIndices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &bodyVBO);
    glDeleteVertexArrays(1, &bodyVAO);
    glDeleteBuffers(1, &bodyEBO);
}

void drawProgressBar(float xOffset, float yOffset, float volume) {
    std::vector<float> barVertices = {
        -0.6f + xOffset, -0.3f + yOffset,
        -0.6f + 0.4f * volume + xOffset, -0.3f + yOffset,
        -0.6f + 0.4f * volume + xOffset, -0.2f + yOffset,
        -0.6f + xOffset, -0.2f + yOffset
    };

    unsigned int barIndices[] = { 0, 1, 2, 0, 2, 3 };

    // Draw the progress bar
    unsigned int barVAO, barVBO, barEBO;
    glGenVertexArrays(1, &barVAO);
    glBindVertexArray(barVAO);

    glGenBuffers(1, &barVBO);
    glBindBuffer(GL_ARRAY_BUFFER, barVBO);
    glBufferData(GL_ARRAY_BUFFER, barVertices.size() * sizeof(float), barVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &barEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, barEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(barIndices), barIndices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &barVBO);
    glDeleteVertexArrays(1, &barVAO);
    glDeleteBuffers(1, &barEBO);
}

void drawDraggableButton(float xOffset, float yOffset, float volume) {
    std::vector<float> buttonVertices = {
        -0.6f + 0.4f * volume - buttonWidth / 2 + xOffset, -0.35f + yOffset,
        -0.6f + 0.4f * volume + buttonWidth / 2 + xOffset, -0.35f + yOffset,
        -0.6f + 0.4f * volume + buttonWidth / 2 + xOffset, -0.15f + yOffset,
        -0.6f + 0.4f * volume - buttonWidth / 2 + xOffset, -0.15f + yOffset
    };

    unsigned int buttonIndices[] = { 0, 1, 2, 0, 2, 3 };

    // Draw the draggable button
    unsigned int buttonVAO, buttonVBO, buttonEBO;
    glGenVertexArrays(1, &buttonVAO);
    glBindVertexArray(buttonVAO);

    glGenBuffers(1, &buttonVBO);
    glBindBuffer(GL_ARRAY_BUFFER, buttonVBO);
    glBufferData(GL_ARRAY_BUFFER, buttonVertices.size() * sizeof(float), buttonVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &buttonEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buttonEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(buttonIndices), buttonIndices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &buttonVBO);
    glDeleteVertexArrays(1, &buttonVAO);
    glDeleteBuffers(1, &buttonEBO);
}

void drawSlider(float xOffset, float yOffset, float volume) {
    // Draw the slider components
    drawSliderBody(xOffset, yOffset);
    drawProgressBar(xOffset, yOffset, volume);
    drawDraggableButton(xOffset, yOffset, volume);
}

void drawSwitchBody(float xOffset, float yOffset) {
    std::vector<float> bodyVertices = {
        -0.1f + xOffset, 0.2f + yOffset,
        0.1f + xOffset, 0.2f + yOffset,
        0.1f + xOffset, 0.3f + yOffset,
        -0.1f + xOffset, 0.3f + yOffset
    };

    unsigned int bodyIndices[] = { 0, 1, 2, 0, 2, 3 };

    // Draw the switch body
    unsigned int bodyVAO, bodyVBO, bodyEBO;
    glGenVertexArrays(1, &bodyVAO);
    glBindVertexArray(bodyVAO);

    glGenBuffers(1, &bodyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bodyVBO);
    glBufferData(GL_ARRAY_BUFFER, bodyVertices.size() * sizeof(float), bodyVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &bodyEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodyEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bodyIndices), bodyIndices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &bodyVBO);
    glDeleteVertexArrays(1, &bodyVAO);
    glDeleteBuffers(1, &bodyEBO);
}

void drawSwitchIndicator(float xOffset, float yOffset, bool isAMMode) {
    std::vector<float> indicatorVertices = {
        (isAMMode ? -0.1f : 0.1f) + xOffset, 0.2f + yOffset,
        (isAMMode ? 0.1f : 0.2f) + xOffset, 0.2f + yOffset,
        (isAMMode ? 0.1f : 0.2f) + xOffset, 0.3f + yOffset,
        (isAMMode ? -0.1f : 0.1f) + xOffset, 0.3f + yOffset
    };

    unsigned int indicatorIndices[] = { 0, 1, 2, 0, 2, 3 };

    // Draw the switch indicator
    unsigned int indicatorVAO, indicatorVBO, indicatorEBO;
    glGenVertexArrays(1, &indicatorVAO);
    glBindVertexArray(indicatorVAO);

    glGenBuffers(1, &indicatorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, indicatorVBO);
    glBufferData(GL_ARRAY_BUFFER, indicatorVertices.size() * sizeof(float), indicatorVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &indicatorEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicatorEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicatorIndices), indicatorIndices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &indicatorVBO);
    glDeleteVertexArrays(1, &indicatorVAO);
    glDeleteBuffers(1, &indicatorEBO);
}

void drawModeSwitch(float xOffset, float yOffset, bool isAMMode) {
    // Draw the switch components
    drawSwitchBody(xOffset, yOffset);
    drawSwitchIndicator(xOffset, yOffset, isAMMode);
}

void drawLightBody(float xOffset, float yOffset) {
    std::vector<float> bodyVertices = {
        -0.45f + xOffset, 0.0f + yOffset,
        -0.35f + xOffset, 0.0f + yOffset,
        -0.35f + xOffset, 0.1f + yOffset,
        -0.45f + xOffset, 0.1f + yOffset
    };

    unsigned int bodyIndices[] = { 0, 1, 2, 0, 2, 3 };

    // Draw the light body
    unsigned int bodyVAO, bodyVBO, bodyEBO;
    glGenVertexArrays(1, &bodyVAO);
    glBindVertexArray(bodyVAO);

    glGenBuffers(1, &bodyVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bodyVBO);
    glBufferData(GL_ARRAY_BUFFER, bodyVertices.size() * sizeof(float), bodyVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &bodyEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bodyEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bodyIndices), bodyIndices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &bodyVBO);
    glDeleteVertexArrays(1, &bodyVAO);
    glDeleteBuffers(1, &bodyEBO);
}

void drawLightGlow(float xOffset, float yOffset, bool isPowerOn) {
    std::vector<float> glowVertices = {
        -0.45f + xOffset, 0.0f + yOffset,
        -0.35f + xOffset, 0.0f + yOffset,
        -0.35f + xOffset, 0.1f + yOffset,
        -0.45f + xOffset, 0.1f + yOffset
    };

    unsigned int glowIndices[] = { 0, 1, 2, 0, 2, 3 };

    // Adjust color based on power state
    if (isPowerOn) {
        glColor3f(1.0f, 0.0f, 0.0f); // Bright red color when the light is on
    }
    else {
        glColor3f(0.5f, 0.0f, 0.0f); // Dim red color when the light is off
    }

    // Draw the light glow
    unsigned int glowVAO, glowVBO, glowEBO;
    glGenVertexArrays(1, &glowVAO);
    glBindVertexArray(glowVAO);

    glGenBuffers(1, &glowVBO);
    glBindBuffer(GL_ARRAY_BUFFER, glowVBO);
    glBufferData(GL_ARRAY_BUFFER, glowVertices.size() * sizeof(float), glowVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &glowEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glowEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glowIndices), glowIndices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Cleanup
    glDeleteBuffers(1, &glowVBO);
    glDeleteVertexArrays(1, &glowVAO);
    glDeleteBuffers(1, &glowEBO);
}

void drawLight(float xOffset, float yOffset, bool isPowerOn) {
    // Draw the light components
    drawLightBody(xOffset, yOffset);
    drawLightGlow(xOffset, yOffset, isPowerOn);
}






// -------------------------------------------------------


unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);

int main(void)
{    
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 750;
    unsigned int wHeight = 750;
    const char wTitle[] = "Radio Interface, Miljan Lazic RA212/2020";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);
    
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }


    float vertices[] = {
        // Rectangle
        -0.9, -0.65, 0.37, 0.2, 0.13, 1.0,  // Vertex 0
        0.9, -0.65, 0.37, 0.2, 0.13, 1.0,   // Vertex 1
        0.9, 0.65, 0.37, 0.2, 0.13, 1.0,    // Vertex 2
        -0.9, 0.65, 0.37, 0.2, 0.13, 1.0,   // Vertex 3
    };
    unsigned int stride = (2 + 4) * sizeof(float);

    unsigned int indices[] = {
        0, 1, 2, // First triangle (part of the rectangle)
        2, 3, 0, // Second triangle (part of the rectangle)
    };


    //Vertex Array Object - Kontejner za VBO, EBO i pokazivace na njihove atribute i tjemena
    unsigned int VAO;
    glGenVertexArrays(1, &VAO); //Generisi 1 kontejner na adresi od promjenljive "VAO"
    glBindVertexArray(VAO); //Povezi VAO za aktivni kontekst - Sve sto radimo ispod ovoga ce se odnositi na kontejner "VAO"

    //Vertex Buffer Object - Nase figure koje crtamo
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //Element Buffer Object
    //Radimo identicne stvari kao za VBO, samo sto saljemo podatke o indeksima tjemena - na Element array buffer
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");
    int uColLoc = glGetUniformLocation(unifiedShader, "uCol"); //Nadji adresu uniforme - obavezno NAKON sto je napravljen sejder
    //glGetUniformLocation(objedinjeni sejder, ime uniforme kako je definisana unutar sejdera) - vraca lokaciju uniforme unutar sejder programa

    glClearColor(1.0, 1.0, 1.0, 1.0);
    while (!glfwWindowShouldClose(window)) 
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        glClear(GL_COLOR_BUFFER_BIT);

        // [KOD ZA CRTANJE]
        glUseProgram(unifiedShader);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(0 * sizeof(unsigned int)));

        //drawLoudspeaker(0.35f, 0.0f);
        //drawLoudspeaker(1.25f, 0.0f);
        //
        //drawPowerButton(-0.54f,0.54f, true);
        //
        //drawAntenna(0.55f, 0.55f, true);
        //
        //drawStationIndicator(-0.02f,-0.35f,0.0f);
        //
        //drawSlider(0.38f, -0.28f, 0.5);
        //
        //drawModeSwitch(0.03f,0.3f, true);

        //drawLight(1.22f, 0.52f, true);
        //glDrawElements(Tip primitive, koliko indeksa se koristi, koji su tip indeksi, pomjeraj sa pocetka niza indeksa da bi dosli do prvog indeksa koji koristimo

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(unifiedShader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    //Uzima kod u fajlu na putanji "source", kompajlira ga i vraca sejder tipa "type"
    //Citanje izvornog koda iz fajla
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str(); //Izvorni kod sejdera koji citamo iz fajla na putanji "source"

    int shader = glCreateShader(type); //Napravimo prazan sejder odredjenog tipa (vertex ili fragment)
    
    int success; //Da li je kompajliranje bilo uspjesno (1 - da)
    char infoLog[512]; //Poruka o gresci (Objasnjava sta je puklo unutar sejdera)
    glShaderSource(shader, 1, &sourceCode, NULL); //Postavi izvorni kod sejdera
    glCompileShader(shader); //Kompajliraj sejder

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); //Provjeri da li je sejder uspjesno kompajliran
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog); //Pribavi poruku o gresci
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    //Pravi objedinjeni sejder program koji se sastoji od Vertex sejdera ciji je kod na putanji vsSource

    unsigned int program; //Objedinjeni sejder
    unsigned int vertexShader; //Verteks sejder (za prostorne podatke)
    unsigned int fragmentShader; //Fragment sejder (za boje, teksture itd)

    program = glCreateProgram(); //Napravi prazan objedinjeni sejder program

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource); //Napravi i kompajliraj vertex sejder
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource); //Napravi i kompajliraj fragment sejder

    //Zakaci verteks i fragment sejdere za objedinjeni program
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program); //Povezi ih u jedan objedinjeni sejder program
    glValidateProgram(program); //Izvrsi provjeru novopecenog programa

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success); //Slicno kao za sejdere
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    //Posto su kodovi sejdera u objedinjenom sejderu, oni pojedinacni programi nam ne trebaju, pa ih brisemo zarad ustede na memoriji
    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}

