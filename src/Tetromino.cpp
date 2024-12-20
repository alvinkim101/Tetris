#include "Tetromino.h"

#include "Board.h"
#include "Audio.h"
#include "macro/Color.h"

#include <vector>
#include <algorithm>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

namespace Tetris
{

Tetromino::Tetromino(SDL_Window& window, SDL_Renderer& renderer, Board& board, Audio& audio) : m_window(window), m_renderer(renderer), m_board(board), m_audio(audio)
{
    Reset();
    m_dropTime = std::chrono::steady_clock::now();
}

void Tetromino::Reset()
{
    static constexpr int8_t x_offset = m_board.Cols / 2;
    static constexpr int8_t y_offset = 1;

    m_piece = m_pieceSelector.Select();
    m_orientation = 0;
    std::copy(GetBaseCoordinates(m_orientation).begin(), GetBaseCoordinates(m_orientation).end(), m_coordinates.begin());

    std::transform(m_coordinates.begin(), m_coordinates.end(), m_coordinates.begin(),
        [](Coordinates& coordinate) -> Coordinates
        {
            coordinate.x += x_offset;
            coordinate.y += y_offset;
            return coordinate;
        }
    );

    // If not valid -> game over
    if (!IsValid(m_coordinates))
    {

    }
}

void Tetromino::Update()
{
    static constexpr std::chrono::seconds interval(1);

    // Gravity logic
    if (std::chrono::steady_clock::now() - m_dropTime > interval)
    {
        Translate(Translation::Down);
    }
}

void Tetromino::Render() const
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(&m_window, &window_w, &window_h);

    int ratio_w = window_w / m_board.Cols / m_board.WidthDivisor;
    int ratio_h = window_h / m_board.Rows;
    SDL_Rect block{0, 0, ratio_w, ratio_h};

    const Color& color = GetColor();
    SDL_SetRenderDrawColor(&m_renderer, color.r, color.g, color.b, SDL_ALPHA_OPAQUE);
    for (const auto& coordinate : m_coordinates)
    {
        block.x = coordinate.x * ratio_w;
        block.y = coordinate.y * ratio_h;
        SDL_RenderFillRect(&m_renderer, &block);
    }
}

void Tetromino::Translate(Translation translation)
{
    std::array<Coordinates, Piece::Size> coordinates;
    Coordinates offset(0, 0);

    switch (translation)
    {
        case Translation::Left:
            offset.x = -1;
            break;
        case Translation::Right:
            offset.x = 1;
            break;
        case Translation::Up:
            offset.y = -1;
            break;
        case Translation::Down:
            offset.y = 1;
            break;
    }

    std::transform(m_coordinates.begin(), m_coordinates.end(), coordinates.begin(),
        [&offset](Coordinates coordinate) -> Coordinates
        {
            coordinate += offset;
            return coordinate;
        }
    );

    if (IsValid(coordinates))
    {
        m_coordinates = coordinates;

        if (translation != Translation::Down)
        {
            m_audio.PlaySound(Audio::Sound::Translate);
        }
    }
    else if (translation == Translation::Down)
    {
        Insert(m_coordinates, GetColor());
        LineClear();
        Reset();
    }

    // Reset tetromino drop time after a down translation
    if (translation == Translation::Down)
    {
        m_dropTime = std::chrono::steady_clock::now();
    }
}

void Tetromino::Rotate(Rotation rotation)
{
    Orientation orientation;
    auto offset = m_coordinates[0]; // First coordinate is relative to the origin
    std::array<Coordinates, Piece::Size> coordinates;

    // Calculate new orientation, and assign to a temp variable
    switch (rotation)
    {
        case Rotation::Clockwise:
            orientation = (m_orientation + 1) % m_piece->orientations.size();
            break;
        case Rotation::CounterClockwise:
            orientation = m_orientation - 1;
            if (orientation == -1)
            {
                orientation = m_piece->orientations.size() - 1;
            }
            break;
    }

    // Add new orientation coordinates (centered at the origin) with the offset, and assign to temp variables
    std::transform(GetBaseCoordinates(orientation).begin(), GetBaseCoordinates(orientation).end(), coordinates.begin(),
        [&offset](Coordinates coordinate) -> Coordinates
        {
            coordinate += offset;
            return coordinate;
        }
    );

    // If valid, assign temp variables to member variables
    if (IsValid(coordinates))
    {
        m_coordinates = coordinates;
        m_orientation = orientation;
        m_audio.PlaySound(Audio::Sound::Rotate);
    }
}

void Tetromino::Insert(std::span<const Coordinates> coordinates, const Color& color)
{
    m_board.Insert(coordinates, color);
}

void Tetromino::LineClear()
{
    m_board.LineClear();
}

bool Tetromino::IsValid(std::span<const Coordinates> coordinates) const
{
    for (const auto& coordinate : coordinates)
    {
        if (m_board.IsOccupied(coordinate.x, coordinate.y))
        {
            return false;
        }
    }

    return true;
}

const std::array<Tetromino::Coordinates, Piece::Size>& Tetromino::GetBaseCoordinates(Orientation orientation) const
{
    return m_piece->orientations[orientation];
}

const Tetromino::Color& Tetromino::GetColor() const
{
    return m_piece->color;
}

void Tetromino::HardDrop()
{

}

void Tetromino::HoldPiece()
{

}

}
