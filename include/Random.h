#pragma once

#include <random>

namespace Tetris
{

class Random
{
    public:
        Random()
        {
            std::random_device rd;
            m_rng.seed(rd());
        }

        std::default_random_engine& operator() ()
        {
            return m_rng;
        }

    private:
        std::default_random_engine m_rng;
};

}
