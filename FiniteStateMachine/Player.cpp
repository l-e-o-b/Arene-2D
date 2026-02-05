#include "Player.h"
Player::Player():sprite(texture)
{
    if (!texture.loadFromFile("Assets/player.png"))
    {
        std::cerr << "Erreur chargement sprite player\n";
    }

    sprite.setTextureRect(
        sf::IntRect({ 0, 0 }, frameSize)
    );

    sprite.setOrigin(
        { frameSize.x / 2.f, frameSize.y / 2.f }
    );
    sprite.setScale({2.f, 2.f});
    sprite.setPosition({400.f, 300.f});
    
    atkCircle.setRadius(40.f);
    atkCircle.setOrigin({ 40.f, 40.f });
    atkCircle.setFillColor(sf::Color::Transparent);


    speed = 250.f;
    atk_speed = 4;
    atk_state = false;
    atkDuration = sf::Time::Zero;
    atkAcc = sf::Time::Zero;
    hp = 20;
    dmg = 2;
    currentDirection = Direction::Down;
}


const sf::Vector2f& Player::getPosition() const
{
    return sprite.getPosition();
}


void Player::update(float dt)
{
    sf::Vector2f movement{ 0.f, 0.f };

    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
    {
        movement.y -= speed;
        currentDirection = Direction::Up;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        movement.y += speed;
        currentDirection = Direction::Down;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
    {
        movement.x -= speed;
        currentDirection = Direction::Left;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        movement.x += speed;
        currentDirection = Direction::Right;
        isMoving = true;
    }

    int directionRow = 0;

    switch (currentDirection)
    {
    case Direction::Down:  directionRow = 0; break;
    case Direction::Up:  directionRow = 1; break;
    case Direction::Right:    directionRow = 2; break;
    case Direction::Left: directionRow = 3; break;
    }

    float movementLength = std::sqrt(movement.x * movement.x + movement.y * movement.y);

    if (atk_state)
    {
        attackTimer += dt;

        if (attackTimer >= attackFrameTime)
        {
            attackTimer -= attackFrameTime;
            currentAttackFrame++;

            int row = attackStartRow + directionRow;

            if (currentAttackFrame >= attackFrameCount)
            {
                currentAttackFrame = 0;
                attackTimer = 0.f;
                atk_state = false;
            }

            sprite.setTextureRect(
                sf::IntRect(
                    { currentAttackFrame * frameSize.x,
                      row * frameSize.y },
                    frameSize
                )
            );
        }
    }
    else if (!isMoving)
    {
      idleTimer += dt;
      if (idleTimer >= idleFrameTime)
      {         
          idleTimer = 0.f;
          currentIdleFrame = (currentIdleFrame + 1) % idleFrameCount;    
          sprite.setTextureRect(      
          sf::IntRect(
          { currentIdleFrame * frameSize.x,
              directionRow * frameSize.y },
              frameSize)
          );
      }
    }
    else
    {
        walkTimer += dt;

        if (walkTimer >= walkFrameTime)
        {
            walkTimer -= walkFrameTime;
            currentWalkFrame = (currentWalkFrame + 1) % walkFrameCount;

            int row = walkStartRow + directionRow;

            sprite.setTextureRect(
                sf::IntRect(
                    { currentWalkFrame * frameSize.x,
                      row * frameSize.y },
                    frameSize
                )
            );
        }

        // reset idle pour éviter les sauts
        idleTimer = 0.f;
        currentIdleFrame = 0;
    }
    sprite.move(movement * dt);


    atkAcc += sf::seconds(dt);
    if (atk_state && canDealDamage)
    {
        atkCircle.setPosition(sprite.getPosition());

        atkDuration += sf::seconds(dt);
        if (atkDuration >= sf::seconds(0.1f))
        {
            canDealDamage = false;
            atkDuration = sf::Time::Zero; 
        }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && !atk_state)
    {
        sf::Time interval = sf::seconds(1.f / atk_speed);

        if (atkAcc >= interval)
        {
            atkCircle.setPosition(sprite.getPosition());
            currentAttackFrame = 0;
            canDealDamage = true;
            attackTimer = 0.f;
            atk_state = true;
            atkAcc -= interval;
        }
    }
}
const sf::CircleShape& Player::getAtkCircle() const
{
    return atkCircle;
}

bool Player::isAttacking() const
{
    return atk_state;
}
int Player::gethp() {
    return hp;
}
int Player::getdmg() {
    return dmg;
}
void Player ::sethp(int new_hp) {
    hp = new_hp;
}
void Player::clampToMap(const sf::FloatRect& bounds)
{
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f half = shape.getSize() / 2.f;

    pos.x = std::clamp(pos.x, (bounds.position.x + half.x), bounds.position.x + (bounds.size.x - half.x));
    pos.y = std::clamp(pos.y, (bounds.position.y + half.y), bounds.position.y + (bounds.size.y - half.y));

    shape.setPosition(pos);
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(sprite);
}
