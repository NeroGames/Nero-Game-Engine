////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2019 SANOU A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
//NERO
#include <Nero/core/cpp/model/AnimationSequence.h>

//EASYLOG
//#include <easyloggingpp/easylogging++.h>
////////////////////////////////////////////////////////////
namespace nero
{
    ////////////////////////////////////////////////////////////
    AnimationSequence::AnimationSequence():
         m_FrameTable()
        ,m_FrameRate(1.f/5.f)
        ,m_CurrentFrame(0)
        ,m_Loop(true)
    {
        //Empty
    }

    ////////////////////////////////////////////////////////////
    void AnimationSequence::setFrameRate(const float& frameRate)
    {
        m_FrameRate = frameRate;
    }

    ////////////////////////////////////////////////////////////
    int AnimationSequence::getFrameCount() const
    {
        return m_FrameTable.size();
    }

    ////////////////////////////////////////////////////////////
    void AnimationSequence::setFrameTable(const std::vector<sf::IntRect>& frameTable)
    {
        m_FrameTable = frameTable;
    }

    ////////////////////////////////////////////////////////////
    void AnimationSequence::setLoop(bool flag)
    {
        m_Loop = flag;
    }

    ////////////////////////////////////////////////////////////
    float   AnimationSequence::getFrameRate() const
    {
        return m_FrameRate;
    }

    ////////////////////////////////////////////////////////////
    bool AnimationSequence::getLoop() const
    {
        return m_Loop;
    }

    ////////////////////////////////////////////////////////////
    void AnimationSequence::reset()
    {
        m_CurrentFrame = 0;
    }

    ////////////////////////////////////////////////////////////
    void AnimationSequence::lock()
    {
        m_CurrentFrame = m_FrameTable.size() - 1;
    }

    ////////////////////////////////////////////////////////////
    const sf::IntRect AnimationSequence::getNextFrame()
    {
        auto frame = m_FrameTable[m_CurrentFrame++];

        if(m_CurrentFrame >= m_FrameTable.size())
        {
            m_Loop ? reset() : lock();
        }

        return frame;
    }
}
