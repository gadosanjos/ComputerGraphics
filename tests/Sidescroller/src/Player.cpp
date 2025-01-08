#include "Player.h"

Player::Player()
    : position(0.0f, 0.0f, 0.0f), speed(5.0f) // Initialize position and speed
{
    // Initialize
    state        = PlayerState::IDLE;
    direction    = Direction::RIGHT;
    isRunning    = false;
    isAttacking  = false;
    totalTime    = 0.0f;
    currentFrame = 0;
    numFrames    = 1;
    frameDuration = 0.15f;  // default for idle
}

void Player::Update(GLfloat deltaTime)
{
    // Accumulate time
    totalTime += deltaTime;

    if (state == PlayerState::RUNNING) {
        // Update position based on direction and speed
        if (direction == Direction::LEFT) {
            position.x -= speed * deltaTime;
        } 
        if (direction == Direction::RIGHT) {
            position.x += speed * deltaTime;
        }
        if (direction == Direction::UP) {
            position.z -= speed * deltaTime;
        } 
        if (direction == Direction::DOWN) {
            position.z += speed * deltaTime;
        }
    }

    // Decide if we need to update states, or do logic, etc.
    // Possibly we might switch states based on isRunning / isAttacking
    // For now, let's just update the animation each frame
    updateAnimation(deltaTime);
}

void Player::HandleKeyboard(bool* keys)
{
    // Example: if keys[GLFW_KEY_W], we might set state to RUNNING, etc.
    // Or keep it super simple for now.

    if (keys[GLFW_KEY_W])
    {
        SetState(PlayerState::RUNNING);
        direction = Direction::UP;
        isRunning = true;
    }
    else if (keys[GLFW_KEY_A])
    {
        SetState(PlayerState::RUNNING);
        direction = Direction::LEFT;
        isRunning = true;
    }
    else if (keys[GLFW_KEY_D])
    {
        SetState(PlayerState::RUNNING);
        direction = Direction::RIGHT;
        isRunning = true;
    }
    else if (keys[GLFW_KEY_S])
    {
        SetState(PlayerState::RUNNING);
        direction = Direction::DOWN;
        isRunning = true;
    }
    else
    {
        // If no keys are pressed, set state to IDLE
        SetState(PlayerState::IDLE);
    }
    // etc.
}

void Player::SetState(PlayerState newState)
{
    // Possibly reset totalTime when we switch states
    if (state != newState)
    {
        state = newState;
        totalTime = 0.0f; // reset so animation restarts
        currentFrame = 0; 
    }
}

PlayerState Player::GetState() const
{
    return state;
}

void Player::SetDirection(Direction newDirection)
{
    direction = newDirection;
}

Direction Player::GetDirection() const
{
    return direction;
}

bool Player::IsRunning() const
{
    return isRunning;
}

bool Player::IsAttacking() const
{
    return isAttacking;
}

int Player::GetCurrentFrame() const
{
    return currentFrame;
}

int Player::GetNumFrames() const
{
    return numFrames;
}

glm::vec3 Player::GetPosition() const {
    return position;
}

// Private function to update the currentFrame
void Player::updateAnimation(GLfloat deltaTime)
{
    switch (state)
    {
    case PlayerState::IDLE:
        // Idle has 4 frames, each 0.15s
        numFrames     = idleAnimationFrames;
        frameDuration = idleFrameDuration;
        break;
    case PlayerState::RUNNING:
        // Running has 8 frames, each 0.1s
        numFrames     = runAnimationFrames;
        frameDuration = runFrameDuration;
        break;
    case PlayerState::ATTACKING:
        // Attack has 8 frames, each 0.1s
        numFrames     = attackAnimationFrames;
        frameDuration = attackFrameDuration;
        break;
    }

    // compute the currentFrame from totalTime
    currentFrame = static_cast<int>(totalTime / frameDuration) % numFrames;
}
