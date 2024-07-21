#include "Tetris.h"
#include "Color.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

constexpr int FREQUENCY = 48000;
constexpr int FORMAT = AUDIO_F32SYS;
constexpr int CHANNELS = 2;
constexpr int CHUNKSIZE = 2048;

constexpr const char* BACKGROUND_MUSIC = "../assets/A-Type.ogg";
constexpr const char* MOVE_SOUND = "../assets/Move.wav";
constexpr const char* ROTATE_SOUND = "../assets/Rotate.wav";
constexpr const char* LAND_SOUND = "../assets/Land.wav";
constexpr const char* LINE_CLEAR_SOUND = "../assets/LineClear.wav";
constexpr const char* FOUR_LINE_CLEAR_SOUND = "../assets/Tetris.wav";
constexpr const char* LEVEL_UP_SOUND = "../assets/LevelUp.wav";
constexpr const char* GAME_OVER_SOUND = "../assets/GameOver.wav";

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

    // Loading music and sound effects
    m_backgroundMusic = Mix_LoadMUS(BACKGROUND_MUSIC);
    if (m_backgroundMusic == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_LoadMUS failure: %s", Mix_GetError());
        return;
    }

    m_moveSound = Mix_LoadWAV(MOVE_SOUND);
    if (m_moveSound == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_LoadWAV failure: %s", Mix_GetError());
        return;
    }

    m_rotateSound = Mix_LoadWAV(ROTATE_SOUND);
    if (m_rotateSound == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_LoadWAV failure: %s", Mix_GetError());
        return;
    }

    m_landSound = Mix_LoadWAV(LAND_SOUND);
    if (m_landSound == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_LoadWAV failure: %s", Mix_GetError());
        return;
    }

    m_lineClearSound = Mix_LoadWAV(LINE_CLEAR_SOUND);
    if (m_lineClearSound == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_LoadWAV failure: %s", Mix_GetError());
        return;
    }

    m_fourLineClearSound = Mix_LoadWAV(FOUR_LINE_CLEAR_SOUND);
    if (m_fourLineClearSound == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_LoadWAV failure: %s", Mix_GetError());
        return;
    }

    m_levelUpSound = Mix_LoadWAV(LEVEL_UP_SOUND);
    if (m_levelUpSound == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_LoadWAV failure: %s", Mix_GetError());
        return;
    }

    m_gameOverSound = Mix_LoadWAV(GAME_OVER_SOUND);
    if (m_gameOverSound == nullptr)
    {
        SDL_LogCritical(SDL_LOG_CATEGORY_AUDIO, "Mix_LoadWAV failure: %s", Mix_GetError());
        return;
    }

    // Create board
    m_board = std::make_unique<Board>(*m_window, *m_renderer);

    m_initialized = true;
}

void Tetris::Deinitialize()
{
    #define X(soundEffect) Mix_FreeChunk(m_##soundEffect##Sound);
    SOUND_EFFECTS
    #undef X
    Mix_FreeMusic(m_backgroundMusic);
    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

void Tetris::RunGame()
{
    if (m_initialized == false)
    {
        return;
    }

    Mix_PlayMusic(m_backgroundMusic, -1);

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
}

void Tetris::Render()
{
    SDL_SetRenderDrawColor(m_renderer, RGB_BLACK, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    m_board->Render();

    SDL_RenderPresent(m_renderer);
}

void Tetris::ParseInput(SDL_Keycode key)
{
    switch (key)
    {
        case SDLK_LEFT:
            break;

        case SDLK_RIGHT:
            break;

        case SDLK_x:
            break;

        case SDLK_z:
            break;

        case SDLK_DOWN:
            break;

        case SDLK_ESCAPE:
            m_running = false;
            break;
    }
}
