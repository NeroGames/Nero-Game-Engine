#include <NERO/model/AnimationSequence.h>

#include <easyloggingpp/easylogging++.h>
#include <Nero/utility/LogUtility.h>

namespace nero
{
    AnimationSequence::AnimationSequence():
        m_FrameTable()
        ,m_FrameRate(1/5.f)
        ,m_CurrentFrame(1)
        ,m_Loop(true)
    {
        //ctor
    }

    void AnimationSequence::setFrameRate(const float& frameRate)
    {
        m_FrameRate = frameRate;
    }

    int AnimationSequence::getFrameCount() const
    {
        return m_FrameTable.size();
    }


    const sf::IntRect AnimationSequence::getNextFrame()
    {
        auto frame = m_FrameTable[m_CurrentFrame];

        if(m_CurrentFrame == m_FrameTable.size()-1 && !m_Loop)
            return frame;

        m_CurrentFrame++;

        if(m_CurrentFrame == m_FrameTable.size() && m_Loop)
            m_CurrentFrame = 0;

        return frame;
    }

    void AnimationSequence::setFrameTable(std::vector<sf::IntRect> frameTable)
    {
        m_FrameTable = frameTable;
    }

    void AnimationSequence::setLoop(bool flag)
    {
        m_Loop = flag;
    }

    float   AnimationSequence::getFrameRate() const
    {
        return m_FrameRate;
    }

    bool AnimationSequence::getLoop() const
    {
        return m_Loop;
    }



}
