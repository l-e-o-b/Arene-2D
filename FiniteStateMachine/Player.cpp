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

    hp = 20;
    maxHp = hp;


    hpBarBackground.setSize({ 50.f, 6.f });

    hpBarBackground.setOrigin(hpBarBackground.getSize() / 2.f);

    hpBarFill.setSize({ 50.f, 6.f });
    hpBarFill.setFillColor(sf::Color::Green);
    hpBarFill.setOrigin(hpBarFill.getSize() / 2.f);



    speed = 200.f;
    atk_speed = 1.6f;
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

bool Player::isAlive() const
{
    return alive;
}


void Player::update(float dt)
{
    if (!alive)
        return;

    if (damaged)
    {
        invincibilityTimer += sf::seconds(dt);
        if (invincibilityTimer >= invincibilityDuration)
        {
            damaged = false;
            invincibilityTimer = sf::Time::Zero;
        }
    }

    movement(dt);
    sprite.setPosition(shape.getPosition());
    if (!atk_state) {
        Attack(dt);
    }

    // Position au-dessus du joueur
    sf::Vector2f pos = sprite.getPosition();
    hpBarBackground.setPosition({ pos.x, pos.y - 40.f });
    hpBarFill.setPosition({ pos.x, pos.y - 40.f });

    // Ajuster la largeur selon HP
    float hpRatio = static_cast<float>(hp) / maxHp;

    hpBarFill.setSize({ 50.f * hpRatio, 6.f });
    hpBarFill.setOrigin({ (50.f * hpRatio) / 2.f, 3.f });
    if (hpRatio > 0.5f)
        hpBarFill.setFillColor(sf::Color::Green);
    else if (hpRatio > 0.25f)
        hpBarFill.setFillColor(sf::Color::Yellow);
    else
        hpBarFill.setFillColor(sf::Color::Red);


}

void Player::movement(float dt) {
    if (!alive)
        return;

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
    shape.move(movement * dt);
}
sf::Vector2f Player::getForwardVector() const
{
    switch (currentDirection)
    {
    case Direction::Up:    return { 0.f, -1.f };
    case Direction::Down:  return { 0.f, 1.f };
    case Direction::Left:  return { -1.f, 0.f };
    case Direction::Right: return { 1.f, 0.f };
    }
    return { 0.f, 1.f };
}
float Player::getAttackRange() const {
    return attackRange;
}
float Player::getAttackAngle() const {
    return attackAngle;
}


void Player::Attack(float dt) {
    if (!alive)
        return;

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

    pos.x = std::clamp(pos.x,
        bounds.position.x + half.x,
        bounds.position.x + bounds.size.x - half.x);

    pos.y = std::clamp(pos.y,
        bounds.position.y + half.y,
        bounds.position.y + bounds.size.y - half.y);

    shape.setPosition(pos);
    sprite.setPosition(pos);
}


void Player::render(sf::RenderWindow& window)
{
    if (!alive)
        return;
    window.draw(hpBarBackground);
    window.draw(hpBarFill);

    window.draw(sprite);
}

float Player::getCollisionRadius() const
{
    return sprite.getGlobalBounds().size.x * 0.4f;
}

void Player::takeDamage(int dmg)
{
    if (!alive || damaged)
        return;

    hp -= dmg;
    damaged = true;
    invincibilityTimer = sf::Time::Zero;

    std::cout << "Player takes " << dmg << " dmg, hp = " << hp << std::endl;

    if (hp <= 0)
    {
        hp = 0;
        alive = false;
        std::cout << "Player is DEAD\n";
    }
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


