#ifndef RE_SIGNAL_H
#define RE_SIGNAL_H

#include <functional>
#include <list>
#include <vector>
#include <memory>
#include <algorithm>

#ifdef emit
    #undef emit
#endif

namespace re {

/// ProtoSignal is the template implementation for callback list.
template<typename,typename> class ProtoSignal;

/// CollectorInvocation invokes signal handlers differently depending on return type.
template<typename,typename> class CollectorInvocation;

template<typename> class Slot;

template<typename, typename> class Connection;

/// CollectorLast returns the result of the last signal handler from a signal emission.
template<typename Result>
struct CollectorLast
{
    typedef Result CollectorResult;
    explicit        CollectorLast ()              : last_() {}
    inline bool     operator()    (Result r)      { last_ = r; return true; }
    CollectorResult result        ()              { return last_; }
private:
    Result last_;
};

/// CollectorDefault implements the default signal handler collection behaviour.
template<typename Result>
struct CollectorDefault : CollectorLast<Result>
{};

/// CollectorDefault specialisation for signals with void return type.
template<>
struct CollectorDefault<void>
{
    typedef void CollectorResult;
    void                  result     ()           {}
    inline bool           operator() (void)       { return true; }
};

/// CollectorInvocation specialisation for regular signals.
template<class Collector, class R, class... Args>
class CollectorInvocation<Collector, R (Args...)>
{
protected:
    inline bool invoke (Collector &collector, const std::function<R (Args...)> &cbf, Args... args)
    {
        return collector (cbf (args...));
    }
};

/// CollectorInvocation specialisation for signals with void return type.
template<class Collector, class... Args>
class CollectorInvocation<Collector, void (Args...)>
{
protected:
    inline bool invoke (Collector &collector, const std::function<void (Args...)> &cbf, Args... args)
    {
        cbf (args...);
        return collector();
    }
};

/// ProtoSignal template specialised for the callback signature and collector.
template<class Collector, class R, class... Args>
class ProtoSignal<R (Args...), Collector> : private CollectorInvocation<Collector, R (Args...)>
{
protected:
    typedef Slot<R (Args...)> SlotType;
    typedef Connection<R (Args...), Collector> ConnectionType;
    typedef typename SlotType::CbFunction CbFunction;
    typedef typename CbFunction::result_type Result;
    typedef typename Collector::CollectorResult CollectorResult;

private:
    /*copy-ctor*/
    ProtoSignal (const ProtoSignal&) = delete;
    ProtoSignal&  operator=   (const ProtoSignal&) = delete;

public:
    /// ProtoSignal constructor, connects default callback if non-NULL.
    ProtoSignal ()
    {
    }
    /// ProtoSignal destructor releases all resources associated with this signal.
    ~ProtoSignal ()
    {
    }
    /// Operator to add a new function or lambda as signal handler, returns a handler connection ID.
    ConnectionType operator+= (const SlotType &slot)
    {
        return connect(slot);
    }

    /// Operator to remove a signal handler through it connection ID, returns if a handler was removed.
    bool   operator-= (const ConnectionType& connection)
    {
        return disconnect(connection);
    }

    ConnectionType connect(const SlotType &slot)
    {
        std::shared_ptr<SlotType> slot_ = std::make_shared<SlotType>(slot);

        slot_->connect();
        slot_ring_.push_back (slot_);

        return ConnectionType(*this, slot_);
    }

    bool disconnect(const ConnectionType& connection)
    {
        auto iter = slot_ring_.begin();
        while(iter != slot_ring_.end()) {
            auto& slot = *iter;
            if (connection.containSlot(slot)) {
                slot->disconnect();
                slot_ring_.erase(iter);
                return true;
            }

            iter ++;
        }

        return false;
    }

    /// Emit a signal, i.e. invoke all its callbacks and collect return types with the Collector.
    CollectorResult emit (Args... args)
    {
        Collector collector;
        if (!slot_ring_.size())
            return collector.result();

        std::list<std::shared_ptr<SlotType>> ring = slot_ring_;

        auto iter = ring.begin();
        do
        {
            auto& slot = *iter;
            if (slot->useable()) {
                CbFunction& function = slot->_func;
                const bool continue_emission = this->invoke (collector, function, args...);
                
                if (!continue_emission)
                    break;
            } else {
                slot->disconnect();
            }
            
            iter ++;
        } while (iter != ring.end());
        
        // TODO: should we support recurse?
        clearDisconnected();

        return collector.result();
    }
    
private:
    void clearDisconnected()
    {
        auto iter = std::remove_if(slot_ring_.begin(), slot_ring_.end(), [](const std::shared_ptr<SlotType>& slot){
            return !slot->connected();
        });
        
        slot_ring_.erase(iter, slot_ring_.end());
    }
    
private:
    std::list<std::shared_ptr<SlotType>> slot_ring_; // linked ring of callback nodes
};

/**
 * Signal is a template type providing an interface for arbitrary callback lists.
 * A signal type needs to be declared with the function signature of its callbacks,
 * and optionally a return result collector class type.
 * Signal callbacks can be added with operator+= to a signal and removed with operator-=, using
 * a callback connection ID return by operator+= as argument.
 * The callbacks of a signal are invoked with the emit() method and arguments according to the signature.
 * The result returned by emit() depends on the signal collector class. By default, the result of
 * the last callback is returned from emit(). Collectors can be implemented to accumulate callback
 * results or to halt a running emissions in correspondance to callback results.
 * The signal implementation is safe against recursion, so callbacks may be removed and
 * added during a signal emission and recursive emit() calls are also safe.
 * The overhead of an unused signal is intentionally kept very low, around the size of a single pointer.
 * Note that the Signal template types is non-copyable.
 */
template<typename SignalSignature, class Collector = CollectorDefault<typename std::function<SignalSignature>::result_type> >
struct Signal : ProtoSignal<SignalSignature, Collector>
{
    Signal ()
    {
    }
};

template<class R, class... Args>
class Slot<R (Args...)>
{
public:
    typedef std::function<R (Args...)> CbFunction;
    typedef std::list<std::weak_ptr<void>> TrackedContainer;

public:
    Slot(CbFunction func)
        : _connected(false), _func(func)
    {

    }

    Slot(const Slot& rhs)
    {
        this->_func = rhs._func;
        this->_tracked_objects = rhs._tracked_objects;
    }

    Slot<R (Args...)>& track(const std::weak_ptr<void> &object)
    {
        _tracked_objects.push_back(object);

        return *this;
    }

    Slot<R (Args...)>& trackObjects(const std::vector<std::weak_ptr<void>>& objects)
    {
        _tracked_objects.insert(_tracked_objects.begin(), objects.begin(), objects.end());
        
        return *this;
    }
    
    bool useable() const
    {
        if (_tracked_objects.size() == 0) {
            return true;
        }
        
        for (auto& object : _tracked_objects) {
            if (object.use_count() == 0) {
                return false;
            }
        }
        
        return true;
    }
    
    inline bool connected() const
    {
        return _connected;
    }
    
    inline void connect()
    {
        _connected = true;
    }
    
    inline void disconnect()
    {
        _connected = false;
    }

public:
    bool _connected;
    CbFunction _func;
    TrackedContainer _tracked_objects;
};

template<typename SignalSignature, class Collector>
class Connection
{
protected:
    typedef Slot<SignalSignature> SlotType;
    typedef ProtoSignal<SignalSignature, Collector> SignalType;

public:
    Connection()
        : _signal(nullptr)
    {
    }

    Connection(SignalType& signal, const std::weak_ptr<SlotType>& slot)
        : _signal(&signal), _slot(slot)
    {

    }
    
    Connection& operator=(const Connection<SignalSignature, Collector>& rhs)
    {
        _signal = rhs._signal;
        _slot = rhs._slot;
        
        return *this;
    }

    bool containSlot(const std::shared_ptr<SlotType>& rhs) const
    {
        auto slot = _slot.lock();

        if (slot && slot == rhs) {
            return true;
        }

        return false;
    }

    bool connected()
    {
        auto slot = _slot.lock();
        return slot != nullptr && slot->connected();
    }

    bool disconnect()
    {
        auto slot = _slot.lock();
        if (_signal && slot) {
            return _signal->disconnect(*this);
        }

        return false;
    }

private:
    SignalType* _signal;
    std::weak_ptr<SlotType> _slot;
};

template<typename SignalSignature, class Collector>
class ScopedConnection : public Connection<SignalSignature, Collector>
{
protected:
    typedef Connection<SignalSignature, Collector> ConnectionType;
    
public:
    ScopedConnection(const ConnectionType& rhs)
        : ConnectionType(rhs)
    {
    }
    
    ~ScopedConnection()
    {
        this->disconnect();
    }
    
    ScopedConnection& operator= (const ConnectionType& rhs)
    {
        this->disconnect();
        ConnectionType::operator=(rhs);
        return *this;
    }
};

template<class R, class... Args>
Slot<R (Args...)> slot (const std::function<R (Args...)>& func)
{
    return Slot<R (Args...)>(func);
}
    
/// This function creates a Slot by binding a object to the member function pointer a method.
template<class Instance, class Class, class R, class... Args>
Slot<R (Args...)> slot (Instance &object, R (Class::*method) (Args...))
{
    auto func = [&object, method] (Args... args) { return (object .* method) (args...); };

    return Slot<R (Args...)>(func);
}

/// This function creates a Slot by binding a object to the member function pointer a method.
template<class Class, class R, class... Args>
Slot<R (Args...)> slot (Class *object, R (Class::*method) (Args...))
{
    auto func = [object, method] (Args... args) { return (object ->* method) (args...); };

    return Slot<R (Args...)>(func);
}

/// Keep signal emissions going while all handlers return !0 (true).
template<typename Result>
struct CollectorUntil0 {
    typedef Result CollectorResult;
    explicit                      CollectorUntil0 ()      : result_() {}
    const CollectorResult&        result          ()      { return result_; }
    inline bool operator() (Result r)
    {
        result_ = r;
        return result_ ? true : false;
    }
private:
    CollectorResult result_;
};

/// Keep signal emissions going while all handlers return 0 (false).
template<typename Result>
struct CollectorWhile0
{
    typedef Result CollectorResult;
    explicit                      CollectorWhile0 ()      : result_() {}
    const CollectorResult&        result          ()      { return result_; }
    inline bool operator() (Result r)
    {
        result_ = r;
        return result_ ? false : true;
    }
private:
    CollectorResult result_;
};

/// CollectorVector returns the result of the all signal handlers from a signal emission in a std::vector.
template<typename Result>
struct CollectorVector
{
    typedef std::vector<Result> CollectorResult;
    const CollectorResult&        result ()       { return result_; }
    inline bool operator() (Result r)
    {
        result_.push_back (r);
        return true;
    }
private:
    CollectorResult result_;
};

} // namespace re

#endif // RE_SIGNAL_H
