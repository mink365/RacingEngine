#ifndef _ViewFactory_h
#define _ViewFactory_h

#include <string>
#include <memory>
#include <vector>

namespace re {

class Window;
class Scene;
class LogicalScene;

using namespace std;

template<class T>
class ViewFactory
{
public:
    std::shared_ptr<T> getView(const string& name);

protected:
    virtual std::shared_ptr<T> createView(const string& name) = 0;
    std::shared_ptr<T> registerView(std::shared_ptr<T>& view);
    std::shared_ptr<T> getRegisteredView(const string& name);
private:
    std::vector<std::shared_ptr<T>> list;
};

template<class T>
inline std::shared_ptr<T> ViewFactory<T>::getView(const string& name)
{
    std::shared_ptr<T> view = this->getRegisteredView(name);

    if (view == nullptr) {
        view = this->createView(name);

        if (view != nullptr) {
//            this->registerView(view);
        }
    }

    return view;
}

template<class T>
inline std::shared_ptr<T> ViewFactory<T>::registerView(std::shared_ptr<T>& view)
{
    this->list.push_back(view);

    return view;
}

template<class T>
inline std::shared_ptr<T> ViewFactory<T>::getRegisteredView(const string& name)
{
    for (auto view : list) {
        if (view->getName() == name) {
            return view;
        }
    }

    return nullptr;
}

class IWindowFactory : public ViewFactory<Window>
{
};


class ISceneFactory : public ViewFactory<Scene>
{
};

class ILayerFactory : public ViewFactory<LogicalScene>
{
};

}

#endif
