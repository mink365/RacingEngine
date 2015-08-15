#ifndef UNCOPYABLE_H
#define UNCOPYABLE_H

namespace re {

class Uncopyable {
protected:
    Uncopyable() = default;
    ~Uncopyable() = default;

    Uncopyable(const Uncopyable&) = delete;
    Uncopyable& operator=(const Uncopyable&) = delete;
};

} // end namespace

#endif // UNCOPYABLE_H
