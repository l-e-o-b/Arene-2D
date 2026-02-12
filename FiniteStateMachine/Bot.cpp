#include "Bot.h"

Bot::Bot(const sf::Vector2f& startPos, BotType type)
    : type(type) ,sprite(texture)
{
    if (!texture.loadFromFile("Assets/VampIdle.png"))
    {
        std::cerr << "Erreur chargement sprite player\n";
    }

    sprite.setTextureRect(
        sf::IntRect({ 0, 0 }, frameSize)
    );

    attackHitbox.setSize({ 40.f, 30.f });
    attackHitbox.setFillColor(sf::Color(255, 0, 0, 120)); // debug visuel
    attackHitbox.setOrigin(attackHitbox.getSize() / 2.f);


    sprite.setOrigin(
        { frameSize.x / 2.f, frameSize.y / 2.f }
    );
    sprite.setScale({ 1.5f, 1.5f });
    sprite.setPosition(startPos);
    shape.setSize({25.f, 25.f });
    shape.setFillColor(sf::Color::Yellow);
    shape.setOrigin(shape.getSize() / 2.f);
    shape.setPosition(startPos);
    hp = 20;
    dmg = 1;
    damaged = false;
    attacking = false;

    framerowcount = texture.getSize().x / frameSize.x;
    currentFrame = 0;
    currentRow = 0;
    animTimer = sf::Time::Zero;
    frameDuration = sf::seconds(1.f / framerowcount);
    // FSM pas encore initialisée

    context.bot = this;
}

void Bot::startAttackCooldown()
{
    attackTimer = attackCooldown;
}

bool Bot::canAttack() const
{
    return attackTimer <= 0.f;
}


void Bot::move(const sf::Vector2f& direction, float dt)
{
    shape.move(direction * speed * dt);
}
sf::RectangleShape& Bot::getHitbox()
{
    return shape;
}


void Bot::startAttackAnim()
{
    attackAnimFinished = false;
    currentFrame = 0;
    animTimer = sf::Time::Zero;
}


void Bot::Attacked() {
    attacking = true;
}
const sf::Vector2f& Bot::getPosition() const
{
    return shape.getPosition();
}

NpcContext& Bot::getContext()
{
    return context;
}

BotType Bot::getType() const
{
    return type;
}
void Bot::markHit()
{
    hitThisFrame = true;
}

bool Bot::wasJustHit()
{
    if (hitThisFrame)
    {
        hitThisFrame = false;
        return true;
    }
    return false;
}

bool Bot::isHurtFinished() const
{
    return hurtFinished;
}

void Bot::setHurt(bool hurt) {
    hurtFinished = hurt;
}

void Bot::Init()
{
    auto* idle = fsm.CreateState<IdleState>();
    auto* chase = fsm.CreateState<ChaseState>();
    auto* attack = fsm.CreateState<AttackState>();
    auto* hurt = fsm.CreateState<HurtState>();

    // --- Transitions Idle → Chase ---
    if (type == BotType::Aggressive)
    {
        idle->AddTransition(Conditions::AlwaysTrue, chase);
    }
    else // ZoneGuard
    {
        idle->AddTransition(Conditions::IsSeeingPlayer, chase);

        chase->AddTransition(
            [](const NpcContext& ctx)
            {
                return !Conditions::IsSeeingPlayer(ctx);
            },
            idle
        );
    }

    // --- Transitions Chase → Attack ---
    chase->AddTransition(Conditions::CanEnterAttack, attack);

    // --- Attack → Chase ---
    attack->AddTransition(
        [](const NpcContext& ctx)
        {
            return ctx.bot->isAttackFinished();
        },
        chase
    );

    idle->AddTransition(Conditions::IsHit, hurt);
    chase->AddTransition(Conditions::IsHit, hurt);
    attack->AddTransition(Conditions::IsHit, hurt);
    hurt->AddTransition(
        [](const NpcContext& ctx)
        {
            return ctx.bot->isHurtFinished();
        },
        idle
    );
    fsm.Init(idle, context);
}

bool Bot::checkHit(const sf::CircleShape& atkCircle) const
{
    sf::Vector2f cPos = atkCircle.getPosition();
    float radius = atkCircle.getRadius();

    sf::Vector2f rPos = shape.getPosition();
    sf::Vector2f half = shape.getSize() / 2.f;

    float closestX = std::clamp(cPos.x, rPos.x - half.x, rPos.x + half.x);
    float closestY = std::clamp(cPos.y, rPos.y - half.y, rPos.y + half.y);

    float dx = cPos.x - closestX;
    float dy = cPos.y - closestY;

    return (dx * dx + dy * dy) <= (radius * radius);
}
void Bot::clampToMap(const sf::FloatRect& bounds)
{
    sf::Vector2f pos = shape.getPosition();
    sf::Vector2f half = shape.getSize() / 2.f;

    pos.x = std::clamp(pos.x, (bounds.position.x + half.x), bounds.position.x + (bounds.size.x - half.x));
    pos.y = std::clamp(pos.y, (bounds.position.y + half.y), bounds.position.y + (bounds.size.y - half.y));

    shape.setPosition(pos);
}
bool Bot::canBeHit() const
{
    return !damaged;
}

void Bot::resetHit()
{
    damaged = false;
}

void Bot::setHit() {
    damaged = true;
}
int Bot::gethp() {
    return hp;
}
int Bot::getdmg() {
    return dmg;
}
void Bot::sethp(int new_hp) {
    hp = new_hp;
}
void Bot::setDirection(const sf::Vector2f& dir)
{
    if (std::abs(dir.x) > std::abs(dir.y))
    {
        currentRow = (dir.x > 0.f) ? 3 : 2; // right : left
    }
    else
    {
        currentRow = (dir.y > 0.f) ? 0 : 1; // down : up
    }
}
void Bot::setAnimation(const std::string& file)
{
    (void)texture.loadFromFile(file);
    sprite.setTexture(texture);

    framerowcount = texture.getSize().x / frameSize.x;;

    currentFrame = 0;
    currentRow = 0;
    frameDuration = sf::seconds(1.f / framerowcount);
    animTimer = sf::Time::Zero;
}

sf::RectangleShape& Bot::getAttackHitbox()
{
    return attackHitbox;
}

void Bot::Update(float dt)
{
    // Mise à jour du timer d’attaque
    if (attackTimer > 0.f)
        attackTimer -= dt;

    sprite.setPosition(shape.getPosition());
    context.BotPosition = shape.getPosition();
    context.dt = dt;
    fsm.Update(context);
    animTimer += sf::seconds(dt);

    sf::Vector2f pos = shape.getPosition();
    float offset = 35.f;

    switch (currentRow)
    {
    case 0: // down
        attackHitbox.setPosition({ pos.x, pos.y + offset });
        break;
    case 1: // up
        attackHitbox.setPosition({ pos.x, pos.y - offset });
        break;
    case 2: // left
        attackHitbox.setPosition({ pos.x - offset, pos.y });
        break;
    case 3: // right
        attackHitbox.setPosition({ pos.x + offset, pos.y });
        break;
    }



    if (animTimer >= frameDuration)
    {
        animTimer -= frameDuration;
        currentFrame++;

        if (attacking && currentFrame >= attackFrameCount)
        {
            attackAnimFinished = true;
            attacking = false;
            currentFrame = attackFrameCount - 1; // hold last frame
        }
        else
        {
            currentFrame %= framerowcount;
        }
    }


    sprite.setTextureRect(sf::IntRect{
        { currentFrame * frameSize.x, currentRow * frameSize.y },
        frameSize
        });
}

void Bot::Render(sf::RenderWindow& window)
{
    window.draw(sprite);
    if (attacking)
        window.draw(attackHitbox);

}
