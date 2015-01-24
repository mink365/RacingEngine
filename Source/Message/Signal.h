#ifndef RE_SIGNAL_H
#define RE_SIGNAL_H

#include <functional>
#include <list>
#include <vector>

#ifdef emit
    #undef emit
#endif

namespace re {

/// ProtoSignal is the template implementation for callback list.
template<typename,typename> class ProtoSignal;

/// CollectorInvocation invokes signal handlers differently depending on return type.
template<typename,typename> class CollectorInvocation;

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
    typedef std::function<R (Args...)> CbFunction;
    typedef typename CbFunction::result_type Result;
    typedef typename Collector::CollectorResult CollectorResult;

    private:
    std::list<CbFunction> callback_ring_; // linked ring of callback nodes
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
    size_t operator+= (const CbFunction &cb)
    {
        callback_ring_.push_back (cb);

        return callback_ring_.size();
    }

    /// Operator to remove a signal handler through it connection ID, returns if a handler was removed.
    bool   operator-= (size_t connection)
    {
        size_t index = 0;
        auto iter = callback_ring_.begin();
        while(iter != callback_ring_.end()) {
            index ++;
            if (index == connection) {
                callback_ring_.erase(iter);
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
        if (!callback_ring_.size())
            return collector.result();

        auto iter = callback_ring_.begin();
        do
        {
            CbFunction& function = *iter;
            const bool continue_emission = this->invoke (collector, function, args...);
            if (!continue_emission)
                break;

            iter ++;
        } while (iter != callback_ring_.end());

        return collector.result();
    }
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
template <typename SignalSignature, class Collector = CollectorDefault<typename std::function<SignalSignature>::result_type> >
struct Signal : ProtoSignal<SignalSignature, Collector>
{
    Signal ()
    {
    }
};

/// This function creates a std::function by binding a object to the member function pointer a method.
template<class Instance, class Class, class R, class... Args>
std::function<R (Args...)> slot (Instance &object, R (Class::*method) (Args...))
{
    return [&object, method] (Args... args) { return (object .* method) (args...); };
}

/// This function creates a std::function by binding a object to the member function pointer a method.
template<class Class, class R, class... Args>
std::function<R (Args...)> slot (Class *object, R (Class::*method) (Args...))
{
    return [object, method] (Args... args) { return (object ->* method) (args...); };
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
