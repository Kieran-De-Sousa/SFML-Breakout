
#include "GameObject.h"
#include <iostream>

GameObject::GameObject()
{

}

GameObject::~GameObject()
{

}

bool GameObject::initialiseSprite(sf::Texture& texture, std::string filename)
{
  if (!texture.loadFromFile(filename))
  {
    std::cout << "Texture could not be loaded\n";
  }
  sprite = new sf::Sprite();
  sprite->setTexture(texture);

  return true;
}

void* GameObject::setVisibility(bool visibility)
{
  is_visible = visibility;
}

void* GameObject::setDirection(Vector2 object_direction)
{
  direction = object_direction;
}

void* GameObject::setSpeed(float object_speed)
{
  speed = object_speed;
}

sf::Sprite* GameObject::getSprite()
{
  return sprite;
}

bool GameObject::getVisibility()
{
  return is_visible;
}

Vector2 GameObject::getDirection()
{
  return direction;
}

float GameObject::getSpeed()
{
  return speed;
}
