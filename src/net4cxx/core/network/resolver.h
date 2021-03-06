//
// Created by yuwenyong on 17-11-22.
//

#ifndef NET4CXX_CORE_NETWORK_RESOLVER_H
#define NET4CXX_CORE_NETWORK_RESOLVER_H

#include "net4cxx/common/common.h"
#include "net4cxx/common/global/loggers.h"
#include <boost/asio.hpp>

NS_BEGIN

class Reactor;

class NET4CXX_COMMON_API Resolver: public std::enable_shared_from_this<Resolver> {
public:
    friend Reactor;
    friend class DelayedResolve;
    using ResolverType = boost::asio::ip::tcp::resolver;
    using ResolverIterator = ResolverType::iterator;

    explicit Resolver(Reactor *reactor);

    Resolver(const Resolver&) = delete;

    Resolver& operator=(const Resolver&) = delete;
protected:
    template <typename CallbackT>
    void start(const std::string &host, CallbackT &&callback) {
        ResolverType::query query(host, "");
        _resolver.async_resolve(query, [callback = std::forward<CallbackT>(callback), resolver = shared_from_this()](
                const boost::system::error_code &ec, ResolverIterator iterator) {
            StringVector addresses;
            if (ec) {
                if (ec == boost::asio::error::operation_aborted) {
                    return;
                }
                NET4CXX_LOG_ERROR(gGenLog, "Resolve error %d: %s", ec.value(), ec.message().c_str());
            } else {
                ResolverIterator end;
                while (iterator != end) {
                    addresses.push_back(iterator->endpoint().address().to_string());
                    ++iterator;
                }
            }
            callback(std::move(addresses));
        });
    }

    void cancel() {
        _resolver.cancel();
    }

    ResolverType _resolver;
};


class NET4CXX_COMMON_API DelayedResolve {
public:
    DelayedResolve() = default;

    explicit DelayedResolve(std::weak_ptr<Resolver> resolver)
            : _resolver(std::move(resolver)) {

    }

    bool cancelled() const {
        return _resolver.expired();
    }

    void cancel();
protected:
    std::weak_ptr<Resolver> _resolver;
};

NS_END

#endif //NET4CXX_CORE_NETWORK_RESOLVER_H
