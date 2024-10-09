#pragma once

#include "Pieces.h"

#include <array>
#include <span>
#include <chrono>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class SDL_Window;
class SDL_Renderer;
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
        using Piece = const std::pair<glm::uvec3, std::vector<std::vector<glm::ivec2>>>*;

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

        static constexpr size_t Size = 4;

    private:
        void Reset(); // Select random shape and set the coordinates to the top middle of the board

        void Translate(Translation translation);
        void Rotate(Rotation rotation);

        void Insert(std::span<const Coordinates> coordinates, const Color& color);
        void LineClear();

        bool IsValid(std::span<const Coordinates> coordinates) const;

        const std::vector<Coordinates>& GetBaseCoordinates(Orientation orientation) const; // Coordinates centered at origin at a specified orientation
        const Color& GetColor() const;

        Pieces m_pieces;
        Piece m_piece;

        // TODO: Change m_coordinates to m_offset
        std::array<Coordinates, Size> m_coordinates;
        Orientation m_orientation; // Index for current orientation

        std::chrono::steady_clock::time_point m_dropTime;

        SDL_Window& m_window;
        SDL_Renderer& m_renderer;
        Board& m_board;
        Audio& m_audio;
};
