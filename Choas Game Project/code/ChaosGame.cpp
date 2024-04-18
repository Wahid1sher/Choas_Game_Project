// Include important C++ libraries here
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <iostream>
#include <sstream>
#include <vector>

// Make code easier to type with "using namespace"
using namespace sf;
using namespace std;

bool showBackground = true;
bool showWelcomeMessage = true;

int main()
{
    // Create a video mode object
    VideoMode vm(1920, 1080);
    // Create and open a window for the game
    RenderWindow window(vm, "Chaos Game", Style::Default);

    // Load background
    Texture backgroundTexture;
    Sprite backgroundSprite;

    if (!backgroundTexture.loadFromFile("./backgournd/background.jpg")) {
        cerr << "Could not load background texture\n";
        return 1;
    }
    
    backgroundSprite.setTexture(backgroundTexture);

    // Calculate the scale factors needed to stretch the sprite to cover the entire window
    Vector2u textureSize = backgroundTexture.getSize();
    float scaleX = static_cast<float>(vm.width) / textureSize.x;
    float scaleY = static_cast<float>(vm.height) / textureSize.y;
    backgroundSprite.setScale(scaleX, scaleY);

    // Load a font
    Font font;
    if (!font.loadFromFile("./fonts/KOMIKAP_.ttf")) { // Replace with the path to your font file
        cerr << "Could not load font\n";
        return 1; // Exit if the font cannot be loaded
    }

    // Create a text object
    Text instructionText;
    instructionText.setFont(font);
    instructionText.setString("Click to set the vertices of the triangle");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(Color::White);
    instructionText.setPosition(600, 100);

    // After loading the background texture and creating the sprite
    FloatRect backgroundBounds = backgroundSprite.getLocalBounds();
    FloatRect textBounds = instructionText.getLocalBounds();

    instructionText.setCharacterSize(48);

    // Center the text on the background sprite
    FloatRect textRect = instructionText.getLocalBounds();
    instructionText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    instructionText.setPosition(vm.width / 2.0f, vm.height / 2.0f);

    // Define the second Text object for interaction instructions
    Text interactionText;
    interactionText.setFont(font);
    interactionText.setCharacterSize(24);
    interactionText.setFillColor(Color::White);
    interactionText.setString("");
    interactionText.setPosition(100, 100);

    vector<Vector2f> vertices;
    vector<Vector2f> points;

    while (window.isOpen())
    {
        if (showBackground && showWelcomeMessage) {
            instructionText.setString("Welcome to the Chaos Game! Click to start.");
        }
        else if (!showBackground) {
            if (vertices.size() < 3) {
                interactionText.setString("Click to set vertex " + to_string(vertices.size() + 1));
            }
            else if (points.empty()) {
                interactionText.setString("Click to set the starting point of the Chaos Game.");
            }
            else {
                interactionText.setString("Press ESC to exit.");
            }
        }
        /*
        ****************************************
        Handle the players input
        ****************************************
        */
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (showBackground) {
                        // On the first click, remove the welcome message and the background
                        showBackground = false;
                        showWelcomeMessage = false;
                    }
                    else {
                        // logic for setting vertices
                        Vector2f newPoint(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
                        if (vertices.size() < 3) {
                            vertices.push_back(newPoint);
                        }
                        else if (points.empty()) {
                            points.push_back(newPoint); // Capture the initial point for the Chaos Game
                        }
                    }
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }

        }

        /*
        ****************************************
        Update
        ****************************************
        */

        int pointsPerFrame = 50; // Example: generate 100 points per frame.
        if (points.size() > 0 && vertices.size() == 3) {
            for (int i = 0; i < pointsPerFrame; ++i) {
                int randomVertexIndex = rand() % 3; // Choose a random vertex index
                Vector2f lastPoint = points.back();
                Vector2f vertex = vertices[randomVertexIndex];
                Vector2f midpoint((lastPoint.x + vertex.x) / 2, (lastPoint.y + vertex.y) / 2);
                points.push_back(midpoint);
            }
        }


        /*
        ****************************************
        Draw
        ****************************************
        */
        window.clear();

        // Draw background
        if (showBackground) {
            window.draw(backgroundSprite);
            // Draw text
            window.draw(instructionText);
        }    
        else {
            window.clear();
            // Draw interaction instructions only if the background is not showing   
            window.draw(interactionText);
    
        }

        
        
        // Draw vertices
        for (const auto& vertex : vertices) {
            CircleShape vertexShape(5); // Radius of 5 units for the vertices
            vertexShape.setPosition(vertex.x - vertexShape.getRadius(), vertex.y - vertexShape.getRadius()); // Center the shape on the vertex point
            vertexShape.setFillColor(Color::Red); // Color for the vertices
            window.draw(vertexShape);
        }

        // Draw points
        for (const auto& point : points) {
            CircleShape pointShape(2); // Radius of 2 units for the points
            pointShape.setPosition(point.x - pointShape.getRadius(), point.y - pointShape.getRadius()); // Center the shape on the point
            pointShape.setFillColor(Color::White); // Color for the points
            window.draw(pointShape);
        }


        // Finally, display the rendered frame
        window.display();

    }
}