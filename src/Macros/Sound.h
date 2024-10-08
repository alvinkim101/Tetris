// https://ph3at.github.io/posts/X-Macros/

#ifndef SOUND_EFFECT
#define SOUND_EFFECT(name)
#endif

SOUND_EFFECT(Translate)
SOUND_EFFECT(Rotate)
SOUND_EFFECT(Land) // Also before line clear
SOUND_EFFECT(LineClear)
SOUND_EFFECT(Tetris) // Four line clear
SOUND_EFFECT(Fall) // After line clear
SOUND_EFFECT(LevelUp)
SOUND_EFFECT(GameOver)

#undef SOUND_EFFECT
