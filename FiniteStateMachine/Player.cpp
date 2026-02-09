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
    sprite.setScale({ 2.f, 2.f });
    sprite.setOrigin({ 24.f ,24.f });
    sprite.setPosition({ 400.f, 300.f });
    shape.setSize({ 40.f, 40.f });
    shape.setFillColor(sf::Color::Cyan);
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition({ 400.f, 300.f });

    atkCircle.setRadius(60.f);
    atkCircle.setOrigin({ 60.f, 60.f });
    atkCircle.setFillColor(sf::Color::Red);


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

sf::RectangleShape& Player::getHitbox()
{
    return shape;
}

void Player::update(float dt)
{
    if (damaged)
    {
        invincibilityTimer += sf::seconds(dt);
        if (invincibilityTimer >= invincibilityDuration)
        {
            damaged = false;
            invincibilityTimer = sf::Time::Zero;
        }
    }

    shape.setPosition(sprite.getPosition());
    movement(dt);
    following_circle(dt);
    if (!atk_state) {
        Attack(dt);
    }
}

void Player::movement(float dt) {
    sf::Vector2f movement{ 0.f, 0.f };

    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) {
        movement.y -= speed;
        isMoving = true;
        if (!atk_state) {
            currentDirection = Direction::Up;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
    {
        movement.y += speed;
        isMoving = true;
        if (!atk_state) {
            currentDirection = Direction::Down;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        movement.x -= speed;
        isMoving = true;
        if (!atk_state) {
            currentDirection = Direction::Left;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
    {
        movement.x += speed;
        isMoving = true;
        if (!atk_state) {
            currentDirection = Direction::Right;
        }
    }


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
                atkAcc = sf::seconds(0);
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
}

void Player::following_circle(float dt) {
    atkCircle.setPosition(sprite.getPosition());
    if (atk_state)
    {
        atkDuration += sf::seconds(dt);
        if (atkDuration >= sf::seconds(0.1f))
        {
            atkDuration = sf::Time::Zero;
        }
    }
}

void Player::Attack(float dt) {
    atkAcc += sf::seconds(dt);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
    {
        sf::Time interval = sf::seconds(1.f / atk_speed);

        if (atkAcc >= interval)
        {
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
sf::Time Player::getAtkAcc() {
    return atkAcc;
}
float Player::getAtkSpeed() {
    return atk_speed;
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
    sf::Vector2f pos1 = sprite.getPosition();

    pos1.x = std::clamp(pos1.x, (bounds.position.x + 24.f), bounds.position.x + (bounds.size.x - 24.f));
    pos1.y = std::clamp(pos1.y, (bounds.position.y + 24.f), bounds.position.y + (bounds.size.y - 24.f));

    sprite.setPosition(pos1);
}

void Player::render(sf::RenderWindow& window)
{
    window.draw(atkCircle);
    window.draw(sprite);
}

float Player::getCollisionRadius() const
{
    return sprite.getGlobalBounds().size.x * 0.4f;
}

void Player::takeDamage(int dmg)
{
    if (damaged) return;

    hp -= dmg;
    damaged = true;
    invincibilityTimer = sf::Time::Zero;

    std::cout << "Player takes " << dmg << " dmg, hp = " << hp << std::endl;
}

bool Player::canBeHit() const
{
    return !damaged;
}

void Player::setHit()
{
    damaged = true;
}

void Player::resetHit()
{
    damaged = false;
}
