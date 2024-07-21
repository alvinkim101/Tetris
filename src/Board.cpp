#include "Board.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

Board::Board(SDL_Window& window, SDL_Renderer& renderer) : m_window(window), m_renderer(renderer)
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
