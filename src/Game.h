
#ifndef BREAKOUT_GAME_H
#define BREAKOUT_GAME_H

#include <SFML/Graphics.hpp>
#include "Vector2.h"
#include "GameObject.h"

class Game
{
 public:
  Game(sf::RenderWindow& window);
  ~Game();
  bool initText();
  bool initGameObjects();
  void update(float dt, sf::Clock game_clock);
  void render();
  void mouseClicked(sf::Event event);
  void keyPressed(sf::Event event);
  void collisionDetection();

 private:
  sf::RenderWindow& window;
  bool start_screen = true;
  bool game_screen = false;
  bool win_screen = false;
  bool lose_screen = false;


  //Text//
  sf::Font font;
  sf::Text start_text;
  sf::Text life_text;
  sf::Text score_text;
  sf::Text win_text;
  sf::Text lose_text;
  sf::Text play_again_text;

  //Game Objects//
  GameObject ball;
  sf::Texture ball_texture;

  GameObject paddle;
  sf::Texture paddle_texture;

  //51 blocks are rendered (maximum per line is 17)
  static const int number_of_blocks = 52;
  GameObject blocks[number_of_blocks];
  sf::Texture block_texture_blue;
  sf::Texture block_texture_yellow;

  //4 gems however only 3 rendered at a time
  static const int number_of_gems = 3;
  GameObject gems[number_of_gems];
  sf::Texture gem_texture_purple;
  sf::Texture gem_texture_red;
  sf::Texture gem_texture_green;

  bool paddle_movement = false;
  bool paddle_move_left = false;
  bool paddle_move_right = false;

  //Additional variables//
  static const int default_lives = 5;
  int lives_left = default_lives;
  int player_score = 0;
  int blocks_broke = 0;
  sf::Time current_time;
};

#endif // BREAKOUT_GAME_H
