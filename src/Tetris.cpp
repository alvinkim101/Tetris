#include "Tetris.h"
#include "macro/Color.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

constexpr int FREQUENCY = 48000;
constexpr int FORMAT = AUDIO_F32SYS;
constexpr int CHANNELS = 2;
constexpr int CHUNKSIZE = 2048;

Tetris::Tetris()
{
    Initialize();
}

Tetris::~Tetris()
{
    Deinitialize();
}

void Tetris::Initialize()
{
    // Initializing SDL systems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "SDL_Init failure: %s", SDL_GetError());
        return;
    }

    // Creating window and renderer
    if (SDL_GetCurrentDisplayMode(0, &m_displayMode) < 0)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "SDL_GetCurrentDisplayMode failure: %s", SDL_GetError());
        return;
    }

    m_window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_displayMode.w / 2, m_displayMode.h / 2, SDL_WINDOW_SHOWN);
    if (m_window == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_VIDEO, "SDL_CreateWindow failure: %s", SDL_GetError());
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_RENDER, "SDL_CreateRenderer failure: %s", SDL_GetError());
        return;
    }

    // Initializing mixer
    if (Mix_Init(MIX_INIT_OGG) & MIX_INIT_OGG == 0)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_Init failure: %s", Mix_GetError());
        return;
    }

    if (Mix_OpenAudio(FREQUENCY, FORMAT, CHANNELS, CHUNKSIZE) < 0)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_OpenAudio failure: %s", Mix_GetError());
        return;
    }

    // Create audio player
    m_audio = std::make_unique<Audio>();
    if (m_audio->Load() == false)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_Load failure: %s", Mix_GetError());
        return;
    }

    // Create board
    m_board = std::make_unique<Board>(*m_window, *m_renderer, *m_audio);

    // Create tetromino
    m_tetromino = std::make_unique<Tetromino>(*m_window, *m_renderer, *m_board, *m_audio);

    m_initialized = true;
}

void Tetris::Deinitialize()
{
    m_audio->Free();
    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    m_renderer = nullptr;
    m_window = nullptr;

    SDL_Quit();
}

void Tetris::RunGame()
{
    if (m_initialized == false)
    {
        return;
    }

    m_audio->PlayMusic();

    m_running = true;
    while (m_running)
    {
        Update();
        Render();
    }
}

void Tetris::Update()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            case SDL_QUIT:
                m_running = false;
                break;
            case SDL_KEYDOWN:
                ParseInput(event.key.keysym.sym);
                break;
        }
    }

    m_tetromino->Update();
}

void Tetris::Render()
{
    SDL_SetRenderDrawColor(m_renderer, RGB_BLACK, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    m_board->Render();
    m_tetromino->Render();

    SDL_RenderPresent(m_renderer);
}

void Tetris::ParseInput(SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_LEFT:
            m_tetromino->MoveLeft();
            break;

        case SDLK_RIGHT:
            m_tetromino->MoveRight();
            break;

        case SDLK_UP:
        case SDLK_x:
            m_tetromino->RotateClockwise();
            break;

        case SDLK_LCTRL:
        case SDLK_z:
            m_tetromino->RotateCounterClockwise();
            break;

        case SDLK_DOWN:
            m_tetromino->SoftDrop();
            break;

        case SDLK_SPACE:
            m_tetromino->HardDrop();
            break;

        case SDLK_LSHIFT:
        case SDLK_c:
            m_tetromino->HoldPiece();
            break;

        case SDLK_ESCAPE:
            m_running = false;
            break;
    }
}
