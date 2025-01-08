#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

// 1) Define your states
enum class PlayerState {
    IDLE,
    RUNNING,
    ATTACKING
};

enum class Direction {
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// 2) A Player class
class Player
{
public:
    Player();

    // The “update” function that decides which animation frame, etc.
    // This can be called each frame in the main loop.
    void Update(GLfloat deltaTime);

    // Possibly a function to handle input
    // (Key states or something else)
    void HandleKeyboard(bool* keys);

    // Setters/getters for states
    void SetState(PlayerState newState);
    PlayerState GetState() const;

    void SetDirection(Direction newDirection);
    Direction GetDirection() const;

    glm::vec3 GetPosition() const;

    bool IsRunning() const;
    bool IsAttacking() const;

    // Returns the current frame for rendering
    int GetCurrentFrame() const;
    // Returns the number of frames in the current animation
    int GetNumFrames() const;

private:
    // Current state of the player
    PlayerState state;
    Direction direction;
    bool isRunning;
    bool isAttacking;

    glm::vec3 position;    // Player's current position
    GLfloat speed;         // Player's movement speed

    // For time-based animation
    float totalTime;
    float frameDuration;
    int currentFrame;
    int numFrames; // depends on whether we are IDLE, RUNNING, etc.

    // Attack animation specifics
    static constexpr int attackAnimationFrames = 8;
    static constexpr float attackFrameDuration = 0.1f; // e.g.

    // Idle animation specifics
    static constexpr int idleAnimationFrames = 4;
    static constexpr float idleFrameDuration = 0.15f;

    // Running animation specifics
    static constexpr int runAnimationFrames = 8;
    static constexpr float runFrameDuration = 0.1f;

    // A function that updates the “currentFrame” and “numFrames” based on current state
    void updateAnimation(GLfloat deltaTime);
};
