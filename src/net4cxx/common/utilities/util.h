//
// Created by yuwenyong on 17-9-13.
//

#ifndef NET4CXX_COMMON_UTILITIES_UTIL_H
#define NET4CXX_COMMON_UTILITIES_UTIL_H

#include "net4cxx/common/common.h"
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>


NS_BEGIN

inline std::string BytesToString(const ByteArray &bytes) {
    return {(const char *)bytes.data(), bytes.size()};
}

inline std::string BytesToString(const Byte *bytes, size_t length) {
    return {(const char *)bytes, length};
}

inline ByteArray StringToBytes(const std::string &s) {
    return ByteArray((const Byte *)s.data(), (const Byte *)s.data() + s.size());
}

inline ByteArray StringToBytes(const char *s) {
    return ByteArray((const Byte *)s, (const Byte *)s + strlen(s));
}

inline bool StringToBool(const std::string &s) {
    std::string lowerStr = boost::to_lower_copy(s);
    return lowerStr == "1" || lowerStr == "true" || lowerStr == "yes";
}

NET4CXX_COMMON_API std::string StringMapToString(const StringMap &m);

inline bool IsIntegral(double d) {
    double integralPart;
    return modf(d, &integralPart) == 0.0;
}

inline void ConcatBuffer(ByteArray &b1, ByteArray &&b2) {
    if (b1.empty()) {
        b1 = std::move(b2);
    } else {
        b1.insert(b1.end(), b2.begin(), b2.end());
    }
}

inline void ConcatBuffer(ByteArray &b1, const ByteArray &b2) {
    b1.insert(b1.end(), b2.begin(), b2.end());
}


class NET4CXX_COMMON_API JsonUtil {
public:
    static std::string encode(const boost::property_tree::ptree &doc) {
        std::ostringstream buffer;
        boost::property_tree::write_json(buffer, doc);
        return buffer.str();
    }

    static void decode(const std::string &s, boost::property_tree::ptree &doc) {
        std::istringstream buffer(s);
        boost::property_tree::read_json(s, doc);
    }
};


class NET4CXX_COMMON_API DateTimeUtil {
public:
    static std::string formatDate(bool usegmt=false) {
        DateTime now = boost::posix_time::second_clock::universal_time();
        return formatDate(now, usegmt);
    }

    static std::string formatDate(const DateTime &timeval, bool usegmt=false);

    static std::string formatUTCDate(const DateTime &ts);

    static DateTime parseUTCDate(const std::string &date);
};


class NET4CXX_COMMON_API BinAscii {
public:
    static std::string hexlify(const ByteArray &s, bool reverse= false) {
        return hexlify(s.data(), s.size(), reverse);
    }

    static std::string hexlify(const Byte *s, size_t len, bool reverse= false);

    static ByteArray unhexlify(const std::string &s, bool reverse= false) {
        return unhexlify(s.c_str(), s.length(), reverse);
    }

    static ByteArray unhexlify(const char *s, size_t len, bool reverse= false);
};


template <typename ValueT>
using PtrVector = std::vector<std::unique_ptr<ValueT>>;

template <typename KeyT, typename ValueT>
using PtrMap = std::map<KeyT, std::unique_ptr<ValueT>>;


template <typename KeyT, typename ValueT>
class PtrMapCreator {
public:
    PtrMapCreator& operator()(KeyT key, std::unique_ptr<ValueT> &&value) {
        _container.emplace(std::move(key), std::move(value));
        return *this;
    }

    PtrMap<KeyT, ValueT>&& operator()() {
        return std::move(_container);
    }
protected:
    PtrMap<KeyT, ValueT> _container;
};


NS_END

#endif //NET4CXX_COMMON_UTILITIES_UTIL_H
