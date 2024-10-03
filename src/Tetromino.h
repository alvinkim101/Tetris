#pragma once

#include <array>
#include <span>
#include <chrono>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class SDL_Window;
class SDL_Renderer;
class Board;

class Tetromino
{
    public:
        enum class Translation
        {
            Left,
            Right,
            Up,
            Down
        };

        enum class Rotation
        {
            Clockwise,
            CounterClockwise
        };

        using Coordinates = glm::ivec2;
        using Color = glm::uvec3;
        using Shape = uint8_t;
        using Orientation = int8_t;

        Tetromino(SDL_Window& window, SDL_Renderer& renderer, Board& board);

        void Update();
        void Render() const;

        void MoveLeft();
        void MoveRight();
        void RotateClockwise();
        void RotateCounterClockwise();
        void SoftDrop();
        void HardDrop();
        void HoldPiece();

    private:
        void Reset(); // Select random shape and set the coordinates to the top middle of the board

        void Translate(Translation translation);
        void Rotate(Rotation rotation);

        bool IsValid(std::span<Coordinates> coordinates) const;

        static constexpr size_t Size = 4;

        std::array<Coordinates, Size> m_coordinates;
        Color m_color;
        Shape m_shape; // Index for current shape
        Orientation m_orientation; // Index for current orientation

        std::chrono::steady_clock::time_point m_dropTime;

        SDL_Window& m_window;
        SDL_Renderer& m_renderer;
        Board& m_board;
};
