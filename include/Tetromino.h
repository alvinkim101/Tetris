#pragma once

#include "PieceSelector.h"

#include <array>
#include <span>
#include <chrono>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct SDL_Window;
struct SDL_Renderer;

namespace Tetris
{

class Board;
class Audio;

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
        using Orientation = int8_t;

        Tetromino(SDL_Window& window, SDL_Renderer& renderer, Board& board, Audio& audio);

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

        void Insert(std::span<const Coordinates> coordinates, const Color& color);
        void LineClear();

        bool IsValid(std::span<const Coordinates> coordinates) const;

        const std::array<Coordinates, Piece::Size>& GetBaseCoordinates(Orientation orientation) const; // Coordinates centered at origin at a specified orientation
        const Color& GetColor() const;

        PieceSelector m_pieceSelector;
        const Piece* m_piece;

        // TODO: Change m_coordinates to m_offset
        std::array<Coordinates, Piece::Size> m_coordinates;
        Orientation m_orientation; // Index for current orientation

        std::chrono::steady_clock::time_point m_dropTime;

        SDL_Window& m_window;
        SDL_Renderer& m_renderer;
        Board& m_board;
        Audio& m_audio;
};

inline void Tetromino::MoveLeft()
{
    Translate(Translation::Left);
}

inline void Tetromino::MoveRight()
{
    Translate(Translation::Right);
}

inline void Tetromino::RotateClockwise()
{
    Rotate(Rotation::Clockwise);
}

inline void Tetromino::RotateCounterClockwise()
{
    Rotate(Rotation::CounterClockwise);
}

inline void Tetromino::SoftDrop()
{
    Translate(Translation::Down);
}

}
