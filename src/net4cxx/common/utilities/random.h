//
// Created by yuwenyong on 17-9-13.
//

#ifndef NET4CXX_COMMON_UTILITIES_RANDOM_H
#define NET4CXX_COMMON_UTILITIES_RANDOM_H

#include "net4cxx/common/common.h"
#include <random>
#include "net4cxx/common/utilities/errors.h"

NS_BEGIN

class NET4CXX_COMMON_API Random {
public:
    static void seed() {
        std::random_device rd;
        _engine.seed(rd());
    }

    static void seed(unsigned int seed) {
        _engine.seed(seed);
    }

    static int randRange(int stop) {
        std::uniform_int_distribution<int> dist(0, stop - 1);
        return dist(_engine);
    }

    static int randRange(int start, int stop) {
        std::uniform_int_distribution<int> dist(start, stop - 1);
        return dist(_engine);
    }

    static int randInt(int a, int b) {
        std::uniform_int_distribution<int> dist(a, b);
        return dist(_engine);
    }

    template <typename ValueT, typename AllocT>
    ValueT choice(const std::vector<ValueT, AllocT> &seq) {
        if (seq.empty()) {
            NET4CXX_THROW_EXCEPTION(IndexError, "seq is empty");
        }
        std::uniform_int_distribution<size_t> dist(0, seq.size() - 1);
        return seq[dist(_engine)];
    };

    template <typename ValueT, typename AllocT>
    void shuffle(std::vector<ValueT, AllocT> &seq) {
        std::shuffle(seq.begin(), seq.end(), _engine);
    };

    template <typename ValueT, typename AllocT>
    std::vector<ValueT, AllocT> sample(const std::vector<ValueT, AllocT> &seq, size_t n) {
        if (seq.size() > n) {
            NET4CXX_THROW_EXCEPTION(ValueError, "sample larger than population");
        }
        std::vector<ValueT, AllocT> result;
        auto first = seq.begin();
        std::copy(first, std::next(first, n), std::back_inserter(result));
        std::advance(first, n);
        std::uniform_int_distribution<size_t> dist;
        for (size_t k = n; first != seq.end(); ++first, ++k) {
            size_t r = dist(_engine, std::uniform_int_distribution<size_t>::param_type{0, k});
            if (r < n) {
                result[r] = *first;
            }
        }
        return result;
    };

    static double random() {
        std::uniform_real_distribution<double> dist(0.0,1.0);
        return dist(_engine);
    }

    static double uniform(double a, double b) {
        std::uniform_real_distribution<double> dist(a, b);
        return dist(_engine);
    }

    static double normalvariate(double mu, double sigma);

    static void randBytes(Byte *buffer, size_t length) {
        std::random_device rd;
        std::uniform_int_distribution<Byte> dist;
        for (size_t i = 0; i != length; ++i) {
            buffer[i] = dist(rd);
        }
    }

    template <size_t BufLen>
    static void randBytes(std::array<Byte, BufLen> &buffer) {
        std::random_device rd;
        std::uniform_int_distribution<Byte> dist;
        for (size_t i = 0; i != BufLen; ++i) {
            buffer[i] = dist(rd);
        }
    }
protected:
    static thread_local std::default_random_engine _engine;
};

NS_END

#endif //NET4CXX_COMMON_UTILITIES_RANDOM_H
