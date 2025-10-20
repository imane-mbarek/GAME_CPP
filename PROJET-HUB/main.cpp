#include <SFML/Graphics.hpp>
#include <iostream>

#include "definition.h"
#include "Animation.h"
#include "Layer.h"
#include "Obstacle.h"

#define ANIMATION_SPF 0.0969

float quad(float t,float a, float b, float c)
{
    
    return a*t*t + b*t + c;
}

/* case: jumping in 2D:
* when t = 0, then f(t) = c so c is the starting pos(and end pos in this case)
* when t = duration(T), then 0 = a t^2+b t then at = -b
* 2at+b = 0, then when t = -b/2a = T/2, then f(T/2) = c + h , -b = Ta
* a/4 T^2 + b T/2 = h, then a = -4h/T^2, b =4h/T
*/
// NOTE: jumping is moving in -Y in SFML
void jump(sf::RectangleShape* player, sf::Clock* clock, float* startY, float h = -50, float duration = 1.0f)
{
    float a =  (h / duration)* (- 4.0f / duration);
    float b = 4.0f* (h/ duration);
    float t = clock->getElapsedTime().asSeconds();

    player->setPosition(player->getPosition().x, quad(t, a, b, *startY));
}

Obstacle createRandomObstacle(
    sf::Texture& FirstTex, sf::Texture& secondTex,
    sf::Vector2u FirstFrames, sf::Vector2u SecondFrames,
    float switchTime
) {
    int random = rand() % 2;
    if (random == 0) {
        return Obstacle(&FirstTex, FirstFrames, switchTime);
    } else {
        return Obstacle(&secondTex, SecondFrames, switchTime);
    }
}

void switchToJump(sf::RectangleShape &player, sf::Texture &Jump, float groundLevel) {
    player.setTexture(&Jump);
    player.setSize(sf::Vector2f(Jump.getSize().x / 3, Jump.getSize().y)); // 3 frames pour jump
}

void switchToRun(sf::RectangleShape &player, sf::Texture &Run, float groundLevel) {
    player.setTexture(&Run);
    player.setSize(sf::Vector2f(Run.getSize().x / 5, Run.getSize().y)); // 5 frames pour run
    player.setPosition(player.getPosition().x, groundLevel - player.getSize().y);
}


bool playerDead = false;

int main()
{
    // Graphical window 
    sf::RenderWindow window(sf::VideoMode(Window_W, Window_H), "Main Window");

    // Load player textures
    sf::Texture Run;
    Run.loadFromFile(RUN_PATH);
    sf::Texture Jump;
    Jump.loadFromFile(JUMP_PATH);

    //creates a rectangular shape that will represent the player in your game.
    sf::RectangleShape player(sf::Vector2f(Run.getSize().x/5, Run.getSize().y/1));
    player.setTexture(&Run);
    float groundLevel = Window_H - 68;
    float playerHeight = player.getSize().y;
    player.setPosition(0, playerHeight - groundLevel);
    

    // Initialize animations
    Animation running(&Run, sf::Vector2u(5, 1), ANIMATION_SPF);
    Animation jumping(&Jump, sf::Vector2u(3, 1), 0.3334);

    float deltaTime = 0.0f;
    sf::Clock clock;
    sf::Clock jumpingClock;
    bool isJumping = false;
    float jumpingStartPos = groundLevel - playerHeight ;

    // Initialize layers
    Layer layer1_1(LAYER_1, LAYER_SPEED * 0.2f);
    Layer layer1_2(LAYER_2, LAYER_SPEED * 0.4f);
    Layer layer1_3(LAYER_3, LAYER_SPEED * 0.6f);
    Layer layer1_4(LAYER_4, LAYER_SPEED * 0.8f);
    Layer layer1_5(LAYER_5, LAYER_SPEED * 1.0f);
    Layer layer1_6(LAYER_6, LAYER_SPEED * 1.6f);  // présent uniquement dans layers1
    Layer layer1_7(LAYER_7, LAYER_SPEED * 1.4f); 
    Layer layer1_8(LAYER_8, LAYER_SPEED * 1.6f);




    


    Layer* layers1[] = {&layer1_1, &layer1_2, &layer1_3, &layer1_4, &layer1_5, &layer1_6, &layer1_7, &layer1_8}; // avec layer6
    Layer* layers2[] = {&layer1_1, &layer1_2, &layer1_3, &layer1_4, &layer1_5, &layer1_7, &layer1_8}; // avec layer6
    
    
    // Obtenir la taille de la fenêtre
    sf::Vector2u windowSize = window.getSize();

    // Appliquer resize sur chaque layer

    for (Layer* layer : layers1) {
        layer->resizeToWindow(windowSize);
    }
    
    for (Layer* layer : layers2) {
        layer->resizeToWindow(windowSize);
    }



    // Load obstacle texture
    sf::Texture obstacleTexture1;
    obstacleTexture1.loadFromFile(OBSTACLE_1);

    sf::Texture obstacleTexture2;
    obstacleTexture2.loadFromFile(OBSTACLE_2);


    // Create the obstacle
    sf::Vector2u imageCount(7, 1); // 7 frames horizontales, 1 ligne
    float switchTime = 0.15f;      // temps entre chaque frame (ajuste pour la vitesse de l’animation)


    srand(static_cast<unsigned>(time(nullptr))); // Seed random

    // Initial obstacle
    Obstacle obstacle = createRandomObstacle(obstacleTexture1, obstacleTexture2, imageCount, imageCount, switchTime);


    // Create the Game Over text
    sf::Font font;
    if (!font.loadFromFile("resources/PlayfairDisplay-Bold.ttf")) {
        // Handle error if font doesn't load
        std::cout << "Error loading font" << std::endl;
    }

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setString("Game Over\nPress R to Restart");
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    gameOverText.setPosition(Window_W / 3, Window_H / 3);

    while(window.isOpen()){
        deltaTime = clock.restart().asSeconds();
        // Check for all window events
        sf::Event event;
        while (window.pollEvent(event)){
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code)
                    {
                        case sf::Keyboard::Space:
                        {
                            if (isJumping == true) break;
                            else
                            {
                                switchToJump(player, Jump, groundLevel);
                                isJumping = true;
                                jumpingStartPos = player.getPosition().y;
                                player.setTexture(&Jump);
                                jumpingClock.restart();
                            }
                            break;
                        }
                        case sf::Keyboard::R:{
                            if (playerDead) { // Only restart if the player is dead
                                // Reset game elements
                                player.setPosition(0, playerHeight - groundLevel);
                                obstacle = createRandomObstacle(obstacleTexture1, obstacleTexture2, imageCount, imageCount, switchTime);                                playerDead = false;  // Reset death flag
                            }
                        }
                    }
                default:
                    break;
            }
        }
        if (isJumping)
        {
            if (jumpingClock.getElapsedTime().asSeconds() >= 1)
            {
                isJumping = false;
                switchToRun(player, Run, groundLevel);
            }
            jump(&player, &jumpingClock, &jumpingStartPos);
            jumping.Update(0, deltaTime);
            player.setTextureRect(jumping.uvRect);
        }
        else
        {
            running.Update(0, deltaTime);
            player.setTextureRect(running.uvRect);
        }

        // Update obstacle position
        obstacle.update(deltaTime, 150.0f);

        // Reset obstacle position when it moves off-screen
        if (obstacle.getGlobalBounds().left + obstacle.getGlobalBounds().width < 0) {
            float newX = Window_W + 50; // Reset to just outside the right edge of the window
            float newY = Window_H - 50; // Keep it on the same vertical level as the player (or randomize)
            obstacle.setPosition(newX, newY); // Use a custom method to set the position of the obstacle
        }


        // Check for collision with the obstacle
        if (!playerDead) {
            sf::FloatRect playerBounds = player.getGlobalBounds();
            sf::FloatRect obstacleBounds = obstacle.getGlobalBounds();

            playerBounds.width *= 0.3f;
            playerBounds.height *= 0.9f;
            obstacleBounds.width *= 0.3f;
            obstacleBounds.height *= 0.9f;

            if (playerBounds.intersects(obstacleBounds)) {
                playerDead = true;
            }
        }
        // Restart the game if the player is dead
        if (playerDead) {
            // Reset the player position and obstacle position
            window.clear();
            window.draw(gameOverText);  // Draw Game Over text
            window.display();
            continue;  // Skip the rest of the game loop until restart
        }

        window.clear(sf::Color::Magenta);

        bool apparaitre=true ;

        // Update layers
        while (apparaitre)
        {
            for (int i = 0; i < 7; i++)
                {
                    layers2[i]->draw(&window, deltaTime);
                }
                apparaitre=false;
        } 
        while (!apparaitre)
        {
            for (int i = 0; i < 8; i++) {
                layers1[i]->draw(&window, deltaTime);
            }
            apparaitre=true;
        }
        
        
        
        
       
        
        // Draw obstacle
        obstacle.draw(window);

        // Draw player
        window.draw(player);
        window.display();
    }
    return 0;
}