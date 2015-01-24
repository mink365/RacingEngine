#ifndef RE_AUDIOENGINE_H
#define RE_AUDIOENGINE_H


namespace re {

class AudioEngine
{
public:
    AudioEngine();
    ~AudioEngine();

    void Pause();
    void Resume();
};

} // namespace re

#endif // RE_AUDIOENGINE_H
