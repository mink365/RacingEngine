#ifndef UNCOPYABLE_H
#define UNCOPYABLE_H

namespace re {

class Uncopyable {
protected:
    Uncopyable() {};
    ~Uncopyable() {};

private:
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

} // end namespace

#endif // UNCOPYABLE_H
