#include "Board.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

Board::Board(SDL_Window* window, SDL_Renderer* renderer) : m_window(window), m_renderer(renderer)
{

}

void Board::RenderBoard() const
{
    int window_w;
    int window_h;
    SDL_GetWindowSize(m_window, &window_w, &window_h);

    int ratio_w = window_w / cols / 3; // Fill a third of the screen width-wise
    int ratio_h = window_h / rows;
    SDL_Rect block{0, 0, ratio_w, ratio_h};

    for (const auto& i : m_board)
    {
        for (const auto& j : i)
        {
            SDL_SetRenderDrawColor(m_renderer, std::get<0>(j.rgb), std::get<1>(j.rgb), std::get<2>(j.rgb), SDL_ALPHA_OPAQUE);

            if (j.occupied == false)
            {
                SDL_RenderDrawRect(m_renderer, &block);
            }
            else
            {
                SDL_RenderFillRect(m_renderer, &block);
            }

            block.x += ratio_w;
        }

        block.x = 0;
        block.y += ratio_h;
    }
}
