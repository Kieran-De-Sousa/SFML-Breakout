
#include "Game.h"
#include "GameObject.h"
#include <iostream>

Game::Game(sf::RenderWindow& game_window)
  : window(game_window)
{
  srand(time(NULL));
}

Game::~Game()
{

}

bool Game::initText()
{
  //Checks for file loads
  if(!font.loadFromFile("Data/Fonts/open-sans/OpenSans-Bold.ttf"))
  {
    std::cout << "Font could not be loaded\n";
  }
  start_text.setString("Press enter to start the game:");
  start_text.setFont(font);
  start_text.setCharacterSize(30);
  start_text.setFillColor(sf::Color(255, 255, 255, 255));
  start_text.setPosition(window.getSize().x/2 - start_text.getGlobalBounds().width/2,
                         window.getSize().y/2 - start_text.getGlobalBounds().height/2);

  life_text.setString("Lives remaining: " + std::to_string(lives_left));
  life_text.setFont(font);
  life_text.setCharacterSize(30);
  life_text.setFillColor(sf::Color(255, 255, 255, 155));
  life_text.setPosition(window.getSize().x/2 - life_text.getGlobalBounds().width/2,
                         window.getSize().y/2 - life_text.getGlobalBounds().height/2);

  score_text.setString("Score: " + std::to_string(player_score));
  score_text.setFont(font);
  score_text.setCharacterSize(30);
  score_text.setFillColor(sf::Color(255, 255, 255, 155));
  score_text.setPosition(window.getSize().x/2 - score_text.getGlobalBounds().width/2,
                        window.getSize().y/1.5 - score_text.getGlobalBounds().height/2);

  win_text.setString("CONGRATULATIONS!");
  win_text.setFont(font);
  win_text.setCharacterSize(30);
  win_text.setFillColor(sf::Color(50, 255, 50, 255));
  win_text.setPosition(window.getSize().x/2 - win_text.getGlobalBounds().width/2,
                         window.getSize().y/2 - win_text.getGlobalBounds().height/2);

  lose_text.setString("YOU LOSE!");
  lose_text.setFont(font);
  lose_text.setCharacterSize(30);
  lose_text.setFillColor(sf::Color(255, 50, 50, 255));
  lose_text.setPosition(window.getSize().x/2 - lose_text.getGlobalBounds().width/2,
                         window.getSize().y/2 - lose_text.getGlobalBounds().height/2);

  play_again_text.setString("Press 'P' to play again.");
  play_again_text.setFont(font);
  play_again_text.setCharacterSize(30);
  play_again_text.setFillColor(sf::Color(255, 255, 255, 255));
  play_again_text.setPosition(window.getSize().x/2 - play_again_text.getGlobalBounds().width/2,
                        window.getSize().y/1.5 - play_again_text.getGlobalBounds().height/2);

  return true;
}

bool Game::initGameObjects()
{
  //Blocks
  int second_line = 0;
  int third_line = 0;

  for (int i = 0; i < number_of_blocks; i++)
  {
    if (i % 2 == 0)
    {
      blocks[i].initialiseSprite(
        block_texture_blue, "Data/images/element_blue_rectangle.png");
    }
    else
    {
      blocks[i].initialiseSprite(
        block_texture_yellow, "Data/images/element_yellow_rectangle.png");
    }
    blocks[i].setVisibility(true);
    int spacing = blocks[i].getSprite()->getGlobalBounds().width;

    if (i <= 17)
    {
      blocks[i].getSprite()->setPosition(spacing * i, 0);
    }
    else if (i <= 34)
    {
      blocks[i].getSprite()->setPosition(
        spacing * second_line, blocks[i].getSprite()->getGlobalBounds().height);
      second_line = second_line + 1;
    }
    else if (i <= 51)
    {
      blocks[i].getSprite()->setPosition(
        spacing * third_line,
        blocks[i].getSprite()->getGlobalBounds().height * 2);
      third_line = third_line + 1;
    }
  }

  //Gems
  for (int j = 0; j < number_of_gems; j++)
  {
    gems[j].setVisibility(false);
    gems[j].setDirection({0,350});
    if (j == 0)
    {
      gems[j].initialiseSprite(gem_texture_purple, "Data/Images/element_purple_diamond_glossy.png");
      gems[j].getSprite()->setPosition(window.getSize().x/2 - gems[j].getSprite()->getGlobalBounds().width/2,
                                       0 - gems[j].getSprite()->getGlobalBounds().height);
    }
    else if (j == 1)
    {
      gems[j].initialiseSprite(gem_texture_red, "Data/Images/element_red_diamond_glossy.png");
      gems[j].getSprite()->setPosition(window.getSize().x/6 - gems[j].getSprite()->getGlobalBounds().width/2,
                                       0 - gems[j].getSprite()->getGlobalBounds().height);
    }
    else if (j == 2)
    {
      gems[j].initialiseSprite(gem_texture_green, "Data/Images/element_green_diamond_glossy.png");
      gems[j].getSprite()->setPosition(window.getSize().x/1.5 - gems[j].getSprite()->getGlobalBounds().width/2,
                                       0 - gems[j].getSprite()->getGlobalBounds().height);
    }
  }

  //Paddle
  paddle.initialiseSprite(paddle_texture, "Data/Images/PaddleBlue.png");
  paddle.getSprite()->setPosition(window.getSize().x/2 - paddle.getSprite()->getGlobalBounds().width/2,
                                  window.getSize().y/1.05 - paddle.getSprite()->getGlobalBounds().height/1.05);
  paddle.setDirection({0,0});

  //Ball
  ball.initialiseSprite(ball_texture, "Data/images/ballGrey.png");
  ball.getSprite()->setPosition(window.getSize().x/2 - ball.getSprite()->getGlobalBounds().width/2,
                                window.getSize().y/2 - ball.getSprite()->getGlobalBounds().height/2);
  ball.setDirection({130, 130});
  ball.setSpeed(1);

  //Game variables
  blocks_broke = 0;
  player_score = 0;
  current_time = sf::seconds(0.0f);
  return true;
}

void Game::update(float dt, sf::Clock game_clock)
{
  if (!start_screen)
  {
    if (game_screen)
    {
      collisionDetection();
      // Paddle movement
      if (paddle_movement)
      {
        if (paddle_move_left)
        {
          paddle.setDirection({ -600, 0 });
          paddle.getSprite()->move(
            paddle.getDirection().x * dt, paddle.getDirection().y * dt);
        }
        else if (paddle_move_right)
        {
          paddle.setDirection({ 600, 0 });
          paddle.getSprite()->move(
            paddle.getDirection().x * dt, paddle.getDirection().y * dt);
        }
      }
      // Ball movement
      ball.getDirection().normalise();
      ball.getSprite()->move(
        ball.getDirection().x * ball.getSpeed() * dt,
        ball.getDirection().y * ball.getSpeed() * dt);

      //Checks if player has won the game
      if (blocks_broke == (number_of_blocks - 1))
      {
        game_screen = false;
        lose_screen = false;
        win_screen = true;
      }

      //Checks if player has lost the game
      if (lives_left <= 0)
      {
        game_screen = false;
        lose_screen = true;
        win_screen  = false;
      }

      current_time = game_clock.getElapsedTime();
      if (current_time > sf::seconds(12.0f) && current_time < sf::seconds(15.0f))
      {
        gems[0].setVisibility(true);
      }
      //40 seconds
      else if (current_time > sf::seconds(20.0f) && current_time < sf::seconds(23.0f))
      {
        gems[1].setVisibility(true);
      }
      //60 seconds
      else if (current_time >= sf::seconds(34.0f) && current_time < sf::seconds(37.0f))
      {
        gems[2].setVisibility(true);
      }

      for (int j = 0; j < number_of_gems; j++)
      {
        if (gems[j].getVisibility())
        {
          gems[j].getSprite()->move(gems[j].getDirection().x * dt, gems[j].getDirection().y * dt);
        }
      }
    }
    else if (win_screen || lose_screen)
    {
      game_clock.restart();
    }
  }
  //Updating text in game
  life_text.setString("Lives remaining: " + std::to_string(lives_left));
  score_text.setString("Score: " + std::to_string(player_score));
}

void Game::render()
{
  if (start_screen)
  {
    window.draw(start_text);
  }
  else if (!start_screen)
  {
    if (game_screen)
    {
      window.draw(life_text);
      window.draw(score_text);
      window.draw(*paddle.getSprite());
      window.draw(*ball.getSprite());
      for (int i = 0; i < number_of_blocks; i++)
      {
        if (blocks[i].getVisibility())
        {
          window.draw(*blocks[i].getSprite());
        }
      }
      for (int j = 0; j < number_of_gems; j++)
      {
        if (gems[j].getVisibility())
        {
          window.draw(*gems[j].getSprite());
        }
      }
    }
    else if (win_screen || lose_screen)
    {
      //Both screens given the option to play the game again
      window.draw(play_again_text);
      if (win_screen)
      {
        window.draw(win_text);
      }
      else if (lose_screen)
      {
        window.draw(lose_text);
      }
    }
  }
}

void Game::mouseClicked(sf::Event event)
{
  //get the click position
  sf::Vector2i click = sf::Mouse::getPosition(window);
}

void Game::keyPressed(sf::Event event)
{
  if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Q)
  {
    window.close();
  }

  if (start_screen)
  {
    if (event.key.code == sf::Keyboard::Enter)
    {
      start_screen = false;
      game_screen = true;
    }
  }

  if (!start_screen)
  {
    //Main game keys
    if (game_screen)
    {
      if (event.type == sf::Event::KeyPressed)
      {
        if (event.key.code == sf::Keyboard::A)
        {
          paddle_movement = true;
          paddle_move_left = true;
          paddle_move_right = false;
        }
        if (event.key.code == sf::Keyboard::D)
        {
          paddle_movement = true;
          paddle_move_left = false;
          paddle_move_right = true;
        }
      }
      if (event.type == sf::Event::KeyReleased)
      {
        if (event.key.code == sf::Keyboard::A)
        {
          paddle_movement = false;
          paddle_move_left = false;
        }
        if (event.key.code == sf::Keyboard::D)
        {
          paddle_movement = false;
          paddle_move_right = false;
        }
      }
    }
    //Win screen and lose screen keys
    else if (win_screen || lose_screen)
    {
      if (event.key.code == sf::Keyboard::P)
      {
        win_screen = false;
        lose_screen = false;
        game_screen = true;
        lives_left = default_lives;
        Game::initGameObjects();
      }
    }
  }
}
void Game::collisionDetection()
{
  //Detection for ball hitting walls of window
  if ((ball.getSprite()->getPosition().x >
      (window.getSize().x - ball.getSprite()->getGlobalBounds().width))
       || ball.getSprite()->getPosition().x < 0)
  {
    ball.setDirection({ball.getDirection().x * - 1, ball.getDirection().y});
  }

  //Detection for ball hitting ceiling of window
  if (ball.getSprite()->getPosition().y < 0)
  {
    ball.setDirection({ball.getDirection().x, ball.getDirection().y * - 1});
  }

  //Detection for ball hitting floor of window
  if ((ball.getSprite()->getPosition().y > (window.getSize().y - ball.getSprite()->getGlobalBounds().height)))
  {
    std::cout << "Ball exited arena!\n";
    lives_left -= 1;
    initGameObjects();
  }

  //Detection for ball hitting paddle
  if (ball.getSprite()->getGlobalBounds().intersects(paddle.getSprite()->getGlobalBounds())
      && ball.getDirection().y > 0)
  {
    ball.setDirection({ball.getDirection().x, ball.getDirection().y * - 1});
  }

  //Detection for ball hitting blocks
  for (int i = 0; i < number_of_blocks; i++)
  {
    //Checks if ball collides with blocks that have a visibility set to true
    if (ball.getSprite()->getGlobalBounds().intersects((blocks[i].getSprite()->getGlobalBounds()))
        && blocks[i].getVisibility())
    {
      //Detection for hitting left of block
      if (ball.getSprite()->getPosition().x <
          blocks[i].getSprite()->getPosition().x + blocks[i].getSprite()->getGlobalBounds().width &&
          ball.getSprite()->getPosition().x >
          blocks[i].getSprite()->getPosition().x + blocks[i].getSprite()->getGlobalBounds().width * 0.9)
      {
        ball.setDirection({ball.getDirection().x * -1, ball.getDirection().y});
        ball.setSpeed(ball.getSpeed() + 0.08);
        blocks[i].setVisibility(false);
        blocks_broke += 1;
        player_score += 1;
      }
      //Detection for hitting right of block
      else if (ball.getSprite()->getPosition().x + ball.getSprite()->getGlobalBounds().width >
               blocks[i].getSprite()->getPosition().x
               && ball.getSprite()->getPosition().x + ball.getSprite()->getGlobalBounds().width <
               blocks[i].getSprite()->getPosition().x + blocks[i].getSprite()->getGlobalBounds().width * 0.1)
      {
        ball.setDirection({ball.getDirection().x * -1, ball.getDirection().y});
        ball.setSpeed(ball.getSpeed() + 0.08);
        blocks[i].setVisibility(false);
        blocks_broke += 1;
        player_score += 1;
      }
      //Detection for hitting bottom of block
      else if (ball.getSprite()->getPosition().y <
               blocks[i].getSprite()->getPosition().y + blocks[i].getSprite()->getGlobalBounds().height &&
               ball.getSprite()->getPosition().y >
               blocks[i].getSprite()->getPosition().y + blocks[i].getSprite()->getGlobalBounds().height * 0.9)
      {
        ball.setDirection({ball.getDirection().x, ball.getDirection().y * -1});
        ball.setSpeed(ball.getSpeed() + 0.08);
        blocks[i].setVisibility(false);
        blocks_broke += 1;
        player_score += 1;
      }
      //Detection for hitting top of block
      else if (ball.getSprite()->getPosition().y + ball.getSprite()->getGlobalBounds().height >
               blocks[i].getSprite()->getPosition().y &&
               ball.getSprite()->getPosition().y + ball.getSprite()->getGlobalBounds().height <
               blocks[i].getSprite()->getPosition().y + blocks[i].getSprite()->getGlobalBounds().height * 0.1)
      {
        ball.setDirection({ball.getDirection().x, ball.getDirection().y * -1});
        ball.setSpeed(ball.getSpeed() + 0.08);
        blocks[i].setVisibility(false);
        blocks_broke += 1;
        player_score += 1;
      }
    }
  }

  //Detection for paddle hitting sides of screen
  if(paddle.getSprite()->getPosition().x > window.getSize().x - paddle.getSprite()->getGlobalBounds().width)
  {
    paddle.getSprite()->setPosition(window.getSize().x - paddle.getSprite()->getGlobalBounds().width, paddle.getSprite()->getPosition().y);
  }
  else if (paddle.getSprite()->getPosition().x < 0)
  {
    paddle.getSprite()->setPosition(0, paddle.getSprite()->getPosition().y);
  }

  //Detection for paddle hitting gems
  for (int j = 0; j < number_of_gems; j++)
  {
    //Checks if paddle collides with gems that have a visibility set to true (avoids
    //score going significantly up due to paddle constantly colliding with gems)
    if (paddle.getSprite()->getGlobalBounds().intersects((gems[j].getSprite()->getGlobalBounds()))
               && gems[j].getVisibility())
    {
      gems[j].setVisibility(false);
      gems[j].getSprite()->setPosition(window.getSize().x + gems[j].getSprite()->getGlobalBounds().width,
                                       window.getSize().y + gems[j].getSprite()->getGlobalBounds().height);
      player_score += 3;
    }
  }
}



