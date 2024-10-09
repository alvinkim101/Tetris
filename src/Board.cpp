#include "Board.h"
#include "Tetromino.h"
#include "Audio.h"

#include <vector>
#include <utility>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

namespace Tetris
{

Board::Board(SDL_Window& window, SDL_Renderer& renderer, Audio& audio) : m_window(window), m_renderer(renderer), m_audio(audio)
{
    // Initialize board to white
    for (auto& row : m_board)
    {
        for (auto& col : row)
        {
            col = White;
        }
    }
}

void Board::Render() const
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(&m_window, &window_w, &window_h);

    int ratio_w = window_w / Cols / WidthDivisor;
    int ratio_h = window_h / Rows;
    SDL_Rect block{0, 0, ratio_w, ratio_h};

    for (int row = 0; row < Rows; row++)
    {
        for (int col = 0; col < Cols; col++)
        {
            const auto& square = m_board[row][col];
            SDL_SetRenderDrawColor(&m_renderer, square.r, square.g, square.b, SDL_ALPHA_OPAQUE);

            if (IsOccupied(col, row))
            {
                SDL_RenderFillRect(&m_renderer, &block);
            }
            else
            {
                SDL_RenderDrawRect(&m_renderer, &block);
            }

            block.x += ratio_w;
        }

        block.x = 0;
        block.y += ratio_h;
    }
}

void Board::Insert(std::span<const glm::ivec2> coordinates, const glm::uvec3& color)
{
    m_recentRows.clear();

    for (const auto& coordinate : coordinates)
    {
        m_board[coordinate.y][coordinate.x] = color;
        m_recentRows.insert(coordinate.y);
    }

    m_audio.PlaySound(Audio::Sound::Land);
}

void Board::LineClear()
{
    // TODO: Figure out the condition(s) for blocks hovering above an empty row

    std::vector<uint8_t> fullRows;
    fullRows.reserve(4);

    // Check which of the recently modified rows are full
    for (const auto& row : m_recentRows)
    {
        if (IsRowFull(row))
        {
            fullRows.push_back(row);
        }
    }

    // Early return
    if (fullRows.empty())
    {
        return;
    }

    std::vector<std::pair<uint8_t, uint8_t>> contiguousRows;
    contiguousRows.reserve(2);

    // Store full rows as contiguous rows using pairs, storing start row and end row. First value will be same as second value if not contiguous with anything.
    int start = fullRows[0];
    for (int i = 1; i < fullRows.size(); i++)
    {
        if (fullRows[i] != fullRows[i - 1] + 1)
        {
            contiguousRows.emplace_back(start, fullRows[i - 1]);
            start = i;
        }
    }
    contiguousRows.emplace_back(start, fullRows[fullRows.size() - 1]);

    // Clear full rows
    for (const auto& row : fullRows)
    {
        ClearRow(row);
    }

    // For every nonempty row above a (formerly) contiguous full row, lower it by a specified amount
    for (int i = 0; i < contiguousRows.size(); i++)
    {
        uint8_t row = contiguousRows[i].first - 1; // Start at row before current row

        while (!IsRowEmpty(row)) // Continue while non empty rows appear
        {
            MoveRow(row, row + 1 + contiguousRows[i].second - contiguousRows[i].first); // The point of creating the contiguous row data structure was to optimize moving contiguous full rows by moving them to their final destination
            row--;
        }
    }

    // TODO: Wait for line clear sound to finish
    if (fullRows.size() == Tetromino::Size)
    {
        m_audio.PlaySound(Audio::Sound::Tetris);
    }
    else
    {
        m_audio.PlaySound(Audio::Sound::LineClear);
    }
    m_audio.PlaySound(Audio::Sound::Fall);
}

void Board::MoveRow(uint8_t srcRow, uint8_t dstRow)
{
    CopyRow(srcRow, dstRow);
    ClearRow(srcRow);
}

void Board::CopyRow(uint8_t srcRow, uint8_t dstRow)
{
    m_board[dstRow] = m_board[srcRow];
}

void Board::ClearRow(uint8_t row)
{
    for (auto& square : m_board[row])
    {
        square = White;
    }
}

bool Board::IsRowFull(uint8_t row) const
{
    for (const auto& square : m_board[row])
    {
        if (square == White)
        {
            return false;
        }
    }

    return true;
}

bool Board::IsRowEmpty(uint8_t row) const
{
    if (row < 0 || row >= Rows)
    {
        return true;
    }

    for (const auto& square : m_board[row])
    {
        if (square != White)
        {
            return false;
        }
    }

    return true;
}

bool Board::IsOccupied(int8_t x, int8_t y) const
{
    if (y < 0 || y >= Rows || x < 0 || x >= Cols)
    {
        return true;
    }

    if (m_board[y][x] == White)
    {
        return false;
    }

    return true;
}

}
