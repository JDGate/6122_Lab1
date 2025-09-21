#include <SFML/Graphics.hpp>
#include <optional>
#include <list>
#include <iostream>
#include "../include/ECE_Buzzy.hpp"
#include "../include/ECE_Enemy.hpp"
#include "../include/ECE_LaserBlast.hpp"

#include <Windows.h>

bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2);
std::string getEventName(const sf::Event& ev); // For live-viewing events in console

const int windowWidth = 1200;
const int windowHeight = 1000;

float playerSpeed = 200.0f;
float gatorSpeed = 250.0f;
float wendysSpeed = 200.0f;
float borgerSpeed = 200.0f;

std::list<ECE_LaserBlast> gatorArmy; // List to hold traveling gators
std::list<ECE_LaserBlast> borgerArmy; // List to hold traveling borgers
std::list<ECE_Enemy> wendysFranchises; // List to hold moving Wendy's

sf::Clock movementClock;
sf::Clock gatorClock; // Clock to manage player firing rate
sf::Clock borgerClock; // Clock to manage Wendy's firing rate

sf::Time gatorFireDelay = sf::seconds(0.75f); // Time delay in seconds between firing gators
sf::Time borgerFireDelay = sf::seconds(1.5f); // Time delay in seconds between firing borgers

std::string gameState = "splashscreen";


int main()
{
	// ------------------------------ SETUP --------------------------------- //

	// For debugging: open a console window to receive cout outputs
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "Console enabled!" << std::endl;

	// Create the main window
    sf::RenderWindow window(sf::VideoMode({ windowWidth, windowHeight }), "Florida Man vs. Wendy's");

    // Initialize srand
	std::srand(static_cast<unsigned>(time(nullptr)));

    // Set up background
    sf::Texture textureBackground("graphics/grass.png");
    sf::Sprite spriteBackground(textureBackground);
    spriteBackground.setPosition({ 0, 0 });
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2u textureSize = textureBackground.getSize();
    spriteBackground.setScale({
    static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
    static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
        });

    // Set up player (Buzzy)
    sf::Texture texturePlayer("graphics/culprit.png");
    ECE_Buzzy player(texturePlayer);

    // Set up Gator and Borger texture (LaserBlast)
    sf::Texture textureGator("graphics/alligator.png");
    sf::Texture textureBorger("graphics/borger.png");

    // Set up Wendy's (Enemy)
    sf::Texture textureWendys("graphics/wendys.png");

    // ------------------------------ END SETUP --------------------------------- //

    while (window.isOpen())
    {
        const float dt = movementClock.restart().asSeconds();

        // Load font
        sf::Font font;
        if (!font.openFromFile("fonts/KOMIKAP_.ttf")) {
            std::cerr << "Failed to load font\n";
        }

        while (const std::optional event = window.pollEvent())
        {
            // Debug: print the event type to the console
            //std::cout << "Event: " << getEventName(*event) << '\n';

            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

        }

        if (gameState == "splashscreen")
        {
            //std::cout << "In splashscreen state\n";

            // Get window size
            sf::Vector2u windowSize = window.getSize();

            // Load splashscreen textures
            sf::Texture textureSplashscreen;
            if (!textureSplashscreen.loadFromFile("graphics/asphalt.png")) {
                std::cerr << "Failed to load splashscreen texture\n";
            }
            sf::Texture textureNews1("graphics/news1.png");
            sf::Sprite spriteNews1(textureNews1);
            spriteNews1.setPosition({ 50, 100 });
            sf::Texture textureNews2("graphics/news2.png");
            sf::Sprite spriteNews2(textureNews2);
            spriteNews2.setPosition({ 550, 200 });

            sf::Sprite spriteSplashscreen(textureSplashscreen);
            spriteSplashscreen.setPosition({ 0.f, 0.f });

            sf::Vector2u textureSize = textureSplashscreen.getSize();
            spriteSplashscreen.setScale(
            {
                static_cast<float>(windowSize.x) / static_cast<float>(textureSize.x),
                static_cast<float>(windowSize.y) / static_cast<float>(textureSize.y)
            });

            // Create text objects
            sf::Text messageText(font);

            messageText.setString("Press Space...");

            messageText.setCharacterSize(75);

            messageText.setFillColor(sf::Color::White);

            // Center message text
            sf::FloatRect textRect = messageText.getLocalBounds();
            messageText.setOrigin({
                textRect.size.x / 2.f,
                textRect.size.y / 2.f
                });
            messageText.setPosition({ windowWidth / 2.f, windowHeight - 100.f });

            // Draw splashscreen
            window.draw(spriteSplashscreen);
            window.draw(spriteNews1);
            window.draw(spriteNews2);
            window.draw(messageText);
            window.display();

            // Transition to playing state
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                gameState = "start";
            }
        }
        else if(gameState == "start")
        {
			wendysFranchises.clear();
			borgerArmy.clear();
			gatorArmy.clear();
            player.setPosition({ 550.f, 50.f });
            player.setRotation(sf::degrees(0));
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 5; j++) {
                    ECE_Enemy newWendys(textureWendys);
                    newWendys.setPosition({ static_cast<float>(j * 200 + 100 + i), static_cast<float>(400 + i * 200) });
                    wendysFranchises.push_back(newWendys);
                }
            }

            sf::Text startMessageText(font);
            startMessageText.setString("   Gator Offender\nPress Enter to Start");
            startMessageText.setCharacterSize(75);
            startMessageText.setFillColor(sf::Color::White);
            sf::FloatRect textRect = startMessageText.getLocalBounds();
            startMessageText.setOrigin({
                textRect.size.x / 2.f,
                textRect.size.y / 2.f
                });
            startMessageText.setPosition({ windowWidth / 2.f, 250.f });

            window.clear();
            window.draw(spriteBackground);
            for(ECE_Enemy wendys : wendysFranchises)
            {
                window.draw(wendys);
			}
            window.draw(player);
			window.draw(startMessageText);
            window.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter)) {
                gameState = "play";
            }
        }
        else if (gameState == "win")
        {
            //std::cout << "You win!" << std::endl;

            sf::Text winMessageText(font);
            winMessageText.setString("  You win!\nPress Down");
            winMessageText.setCharacterSize(75);
            winMessageText.setFillColor(sf::Color::White);
            sf::FloatRect textRect = winMessageText.getLocalBounds();
            winMessageText.setOrigin({
                textRect.size.x / 2.f,
                textRect.size.y / 2.f
                });
            winMessageText.setPosition({ windowWidth / 2.f, 250.f });

            window.clear();
            window.draw(spriteBackground);
            for (ECE_Enemy wendys : wendysFranchises)
            {
                window.draw(wendys);
            }
            window.draw(winMessageText);
            window.draw(player);
            window.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                gameState = "start";
            }
        }
        else if(gameState == "gameover")
        {
			//std::cout << "Game Over!" << std::endl;

            sf::Text loseMessageText(font);
            loseMessageText.setString(" Try again...\nPress Down");
            loseMessageText.setCharacterSize(75);
            loseMessageText.setFillColor(sf::Color::Red);
            sf::FloatRect textRect = loseMessageText.getLocalBounds();
            loseMessageText.setOrigin({
                textRect.size.x / 2.f,
                textRect.size.y / 2.f
                });
            loseMessageText.setPosition({ windowWidth / 2.f, 250.f });

            window.clear();
			window.draw(spriteBackground);
            for (ECE_Enemy wendys : wendysFranchises)
            {
                window.draw(wendys);
            }
            window.draw(loseMessageText);
            window.draw(player);
            window.display();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
                gameState = "start";
            }
		}
        else
        {

            // ------------------------------ EVENT LOGIC --------------------------------- //
            // ********** Controls for player ********** //
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                player.move({ -playerSpeed * dt, 0.f });
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                player.move({ playerSpeed * dt, 0.f });
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                if (gatorClock.getElapsedTime() >= gatorFireDelay) {
                    ECE_LaserBlast newGator(textureGator);
                    newGator.setPosition({ player.getPosition().x, player.getPosition().y + 50.f });
                    gatorArmy.push_back(newGator);
                    gatorClock.restart(); // Reset timer after firing
                }
            }
            // ********** End Controls for player ********** //

            // ********** Wendy's Attack ********** //
            if (borgerClock.getElapsedTime() >= borgerFireDelay)
            {
                // Pick a random index
                int randomIndex = std::rand() % wendysFranchises.size();

                // Advance iterator to the random index
                auto it = wendysFranchises.begin();
                std::advance(it, randomIndex);

                // Now *it is your chosen Wendy's
                ECE_Enemy& chosenWendys = *it;

                // Get its position
                sf::Vector2f pos = chosenWendys.getPosition();

                ECE_LaserBlast newBorger(textureBorger);
                newBorger.setScale({ 0.125f, 0.125f });
                newBorger.setPosition({ pos.x, pos.y });
                borgerArmy.push_back(newBorger);
                borgerClock.restart(); // Reset timer after firing
            }
            // ********** End Wendy's Attack ********** //

            // ********** Collision Logic ********** //
            for (auto wendysIt = wendysFranchises.begin(); wendysIt != wendysFranchises.end();)
            {
                bool erased = false;
                for (auto gatorIt = gatorArmy.begin(); gatorIt != gatorArmy.end(); )
                {
                    if (checkCollision(*wendysIt, *gatorIt))
                    {
                        std::cout << "Collision detected! Wendy's hit by Gator!" << std::endl;
                        gatorIt = gatorArmy.erase(gatorIt); // erase returns next valid iterator
                        wendysIt = wendysFranchises.erase(wendysIt); // erase returns next valid iterator
                        erased = true;
                        break; // exit inner loop, wendysIt already updated
                    }
                    else
                    {
                        ++gatorIt;
                    }
                }
                if (!erased)
                {
                    ++wendysIt;
                }
            }

            for (int ii = 0; ii < borgerArmy.size(); ii++) {
                auto borgerIt = std::next(borgerArmy.begin(), ii);
                if (checkCollision(player, *borgerIt)) {
                    // Collision detected, remove the borger
                    borgerArmy.erase(borgerIt);
                    player.setRotation(sf::degrees(90));
                    std::cout << "Collision detected! Player hit by Borger!" << std::endl;
					gameState = "gameover";
                    break; // Exit loop to avoid invalid iterator
                }
            }

            for (int ii = 0; ii < wendysFranchises.size(); ii++)
            {
                auto wendysIt = std::next(wendysFranchises.begin(), ii);
                if (checkCollision(player, *wendysIt))
                {
                    // Collision detected, remove the borger
                    player.setRotation(sf::degrees(90));
                    std::cout << "Collision detected! Player hit by Wendy's!" << std::endl;
					gameState = "gameover";
                }
            }
            // ********** End Collision Logic ********** //

            if(wendysFranchises.empty())
            {
                std::cout << "All Wendy's eliminated! You win!" << std::endl;
                gameState = "win";
			}

            // ------------------------------ END EVENT LOGIC --------------------------------- //

            // ------------------------------ ANIMATION --------------------------------- //

            window.clear();
            window.draw(spriteBackground);
            window.draw(player);

            for (ECE_LaserBlast& gator : gatorArmy)
            {
                gator.move({ 0.f, gatorSpeed * dt });
                window.draw(gator);
            }
            gatorArmy.erase(
                std::remove_if(gatorArmy.begin(), gatorArmy.end(),
                    [](const ECE_LaserBlast& g) {
                        return g.getPosition().y > windowHeight;
                    }),
                gatorArmy.end()
            );

            for (ECE_LaserBlast& borger : borgerArmy)
            {
                borger.move({ 0.f, -borgerSpeed * dt });
                window.draw(borger);
            }
            borgerArmy.erase(
                std::remove_if(borgerArmy.begin(), borgerArmy.end(),
                    [](const ECE_LaserBlast& b) {
                        return b.getPosition().y < -200;
                    }),
                borgerArmy.end()
            );

            for (ECE_Enemy& wendys : wendysFranchises)
            {
                if (wendys.getPosition().x < 0 || wendys.getPosition().x + wendys.getGlobalBounds().size.x > windowWidth)
                {
                    for (ECE_Enemy& eachWendys : wendysFranchises)
                    {
                        eachWendys.move({ 0.f, -30.f }); // Move all Wendy's up
                        if (eachWendys.getPosition().x < 0 || eachWendys.getPosition().x + eachWendys.getGlobalBounds().size.x > windowWidth)
                        {
                            eachWendys.setPosition({ std::clamp(eachWendys.getPosition().x, 0.f, static_cast<float>(windowWidth) - eachWendys.getGlobalBounds().size.x), eachWendys.getPosition().y }); // Prevent Wendy's from sticking to the edge of the window
                        }
                    }
                    wendysSpeed = -wendysSpeed; // Reverse direction
                }
                wendys.move({ wendysSpeed * dt, 0.f });
                window.draw(wendys);
            }
            window.display();

            // ------------------------------ END ANIMATION --------------------------------- //
        }
    }

}

bool checkCollision(const sf::Sprite& a, const sf::Sprite& b) {
    return a.getGlobalBounds().findIntersection(b.getGlobalBounds()).has_value();
}


// Fun debug function that returns the name of an event to the console
std::string getEventName(const sf::Event& ev)
{
    if (ev.is<sf::Event::Closed>())               return "Closed";
    if (ev.is<sf::Event::Resized>())              return "Resized";
    if (ev.is<sf::Event::FocusLost>())            return "LostFocus";
    if (ev.is<sf::Event::FocusGained>())          return "GainedFocus";
    if (ev.is<sf::Event::TextEntered>())          return "TextEntered";
    if (ev.is<sf::Event::KeyPressed>())           return "KeyPressed";
    if (ev.is<sf::Event::KeyReleased>())          return "KeyReleased";
    if (ev.is<sf::Event::MouseWheelScrolled>())   return "MouseWheelScrolled";
    if (ev.is<sf::Event::MouseButtonPressed>())   return "MouseButtonPressed";
    if (ev.is<sf::Event::MouseButtonReleased>())  return "MouseButtonReleased";
    if (ev.is<sf::Event::MouseMoved>())           return "MouseMoved";
    if (ev.is<sf::Event::MouseEntered>())         return "MouseEntered";
    if (ev.is<sf::Event::MouseLeft>())            return "MouseLeft";

    return "Unknown";
}