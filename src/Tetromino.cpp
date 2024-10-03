#include "Tetromino.h"
#include "Board.h"
#include "Color.h"

#include <vector>
#include <algorithm>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

// Contains all possible tetrominoes
// First value has color, second value has possible orientations
// First coordinates are at origin
// Clockwise ordered
static const std::vector<std::pair<Tetromino::Color, std::vector<std::vector<Tetromino::Coordinates>>>> Tetrominoes =
{
    /* I */ { glm::uvec3(RGB_CYAN), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(-2, 0), glm::ivec2(1, 0)}, {glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, -2), glm::ivec2(0, 1)}}},
    /* O */ { glm::uvec3(RGB_YELLOW), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(0, -1), glm::ivec2(-1, -1) }}},
    /* T */ { glm::uvec3(RGB_PURPLE), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, 0), glm::ivec2(0, 1) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, 0), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(1, 0) }}},
    /* J */ { glm::uvec3(RGB_BLUE), {{ glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, 1), glm::ivec2(0, 1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(-1, -1), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(1, -1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(1, 0), glm::ivec2(1, 1) }}},
    /* L */ { glm::uvec3(RGB_ORANGE), {{ glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(0, 1), glm::ivec2(1, 1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 1), glm::ivec2(-1, 0), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(-1, -1), glm::ivec2(0, 1) }, { glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(1, -1), glm::ivec2(1, 0) }}},
    /* S */ { glm::uvec3(RGB_GREEN), {{ glm::ivec2(0, 0), glm::ivec2(-1, 1), glm::ivec2(0, 1), glm::ivec2(1, 0) }, { glm::ivec2(0, 0), glm::ivec2(0, -1), glm::ivec2(1, 0), glm::ivec2(1, 1) }}},
    /* Z */ { glm::uvec3(RGB_RED), {{ glm::ivec2(0, 0), glm::ivec2(-1, 0), glm::ivec2(0, 1), glm::ivec2(1, 1) }, { glm::ivec2(0, 0), glm::ivec2(0, 1), glm::ivec2(1, -1), glm::ivec2(1, 0) }}}
};

Tetromino::Tetromino(SDL_Window& window, SDL_Renderer& renderer, Board& board) : m_window(window), m_renderer(renderer), m_board(board)
{
    Reset();
    m_dropTime = std::chrono::steady_clock::now();
}

void Tetromino::Reset()
{
    static constexpr int8_t x_offset = m_board.Cols / 2;
    static constexpr int8_t y_offset = 1;

    m_shape = rand() % Tetrominoes.size();
    m_orientation = 0;
    m_color = Tetrominoes[m_shape].first;
    std::copy(Tetrominoes[m_shape].second[m_orientation].begin(), Tetrominoes[m_shape].second[m_orientation].end(), m_coordinates.begin());

    std::transform(m_coordinates.begin(), m_coordinates.end(), m_coordinates.begin(),
        [](Coordinates& coordinate) -> Coordinates
        {
            coordinate.x += x_offset;
            coordinate.y += y_offset;
            return coordinate;
        }
    );
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

    SDL_SetRenderDrawColor(&m_renderer, m_color.r, m_color.g, m_color.b, SDL_ALPHA_OPAQUE);
    for (const auto& coordinate : m_coordinates)
    {
        block.x = coordinate.x * ratio_w;
        block.y = coordinate.y * ratio_h;
        SDL_RenderFillRect(&m_renderer, &block);
    }
}

void Tetromino::Translate(Translation translation)
{
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

    std::transform(m_coordinates.begin(), m_coordinates.end(), m_coordinates.begin(),
        [&offset](Coordinates& coordinate) -> Coordinates
        {
            coordinate += offset;
            return coordinate;
        }
    );

    if (!IsValid(m_coordinates))
    {
        std::transform(m_coordinates.begin(), m_coordinates.end(), m_coordinates.begin(),
            [&offset](Coordinates& coordinate) -> Coordinates
            {
                coordinate -= offset;
                return coordinate;
            }
        );

        /*
        If not valid and translation is down:
            Insert tetromino into board
            Check for line clears
            Reset tetromino
        */
        if (translation == Translation::Down)
        {
            
        }
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
    std::array<Coordinates, Size> coordinates;

    // Calculate new orientation, and assign to a temp variable
    switch (rotation)
    {
        case Rotation::Clockwise:
            orientation = (m_orientation + 1) % Tetrominoes[m_shape].second.size();
            break;
        case Rotation::CounterClockwise:
            orientation = m_orientation - 1;
            if (orientation == -1)
            {
                orientation = Tetrominoes[m_shape].second.size() - 1;
            }
            break;
    }

    // Add new orientation coordinates (centered at the origin) with the offset, and assign to temp variables
    std::transform(Tetrominoes[m_shape].second[orientation].begin(), Tetrominoes[m_shape].second[orientation].end(), coordinates.begin(),
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
    }
}

bool Tetromino::IsValid(std::span<Coordinates> coordinates) const
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

void Tetromino::MoveLeft()
{
    Translate(Translation::Left);
}

void Tetromino::MoveRight()
{
    Translate(Translation::Right);
}

void Tetromino::RotateClockwise()
{
    Rotate(Rotation::Clockwise);
}

void Tetromino::RotateCounterClockwise()
{
    Rotate(Rotation::CounterClockwise);
}

void Tetromino::SoftDrop()
{
    Translate(Translation::Down);
}

void Tetromino::HardDrop()
{

}

void Tetromino::HoldPiece()
{

}
