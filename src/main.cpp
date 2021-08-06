#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"


int main()
{
  std::cout << "You should see a window that opens as well as this writing to console..."
            << std::endl;

  // create window and set up
  sf::RenderWindow window(sf::VideoMode(1080, 720), "Breakout!");
  window.setFramerateLimit(60);

  // A Clock starts counting as soon as it's created
  sf::Clock clock;
  //
  sf::Clock game_clock;

  //initialise an instance of the game class
  Game game(window);

  //run the init functions of the game class and check it all initialises ok
  if (!game.initText())
  {
    return 0;
  }
  if (!game.initGameObjects())
  {
    return 0;
  }


  // Game loop: run the program as long as the window is open
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    sf::Event event;

    //calculate delta time
    sf::Time time = clock.restart();
    float dt = time.asSeconds();

    //'process inputs' element of the game loop
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
      {
        game.keyPressed(event);
      }
      // "close requested" event: we close the window
      if (event.type == sf::Event::Closed)
        window.close();
    }


    //'update' element of the game loop
    game.update(dt, game_clock);

    window.clear(sf::Color(38, 38, 38, 0));

    //'render' element of the game loop
    game.render();
    window.display();
  }

  return 0;
}
