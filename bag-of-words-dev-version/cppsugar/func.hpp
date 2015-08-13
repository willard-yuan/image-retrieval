#ifndef __FUNC_SUGAR__
#define __FUNC_SUGAR__

#include <stdexcept>
#include <vector>
#include <string>
#include <functional>
#include <algorithm>
#include <numeric>
#include <set>

namespace func {
    // Similar to python's range
     std::vector<int> Range(int start, int step, int end) {
        std::vector<int> result;
        result.reserve((end - start) / step + 1);
        for (int i = start; i < end; i += step) {
            result.push_back(i);
        }
        return result;
    }

     std::vector<int> Range(int start, int end) {
        std::vector<int> result(end - start);
        std::iota(result.begin(), result.end(), start);
        return result;
    }

     std::vector<int> Range(int end) {
        return Range(0, end);
    }

    template<typename TIn, typename TOut>
         std::vector<TOut> Map(const std::vector<TIn> &in, std::function<TOut(TIn)> f) {
            std::vector<TOut> result(in.size());
            std::transform(in.begin(), in.end(), result.begin(), f);
            return result;
        }

    template<typename TIn, typename TOut>
         TOut Reduce(const std::vector<TIn> &in, std::function<TOut(TIn, TIn)> f) {
            TIn init = in[0];
            for (int i = 1; i < in.size(); i++) {
                init = f(init, in[i]);
            }
            return init;
        }

    template<typename TIn>
         TIn Sum(const std::vector<TIn> &in) {
            return Reduce<TIn, TIn>(in, [](const TIn &a, const TIn &b) { return a + b; });
        }

    template<typename TIn, typename TOut>
         TOut Sum(const std::vector<TIn> &in, std::function<TOut(TIn)> f) {
             return Sum(Map<TIn, TOut>(in, f));
        }

    template<typename TIn>
         TIn Average(const std::vector<TIn> &in) {
            return Sum(in) / in.size();
        }

    template<typename TIn>
         TIn Min(const std::vector<TIn> &in) {
            return Reduce<TIn, TIn>(in, [](const TIn &a, const TIn &b) { return a < b ? a : b; });
        }

    template<typename TIn>
         TIn Max(const std::vector<TIn> &in) {
            return Reduce<TIn, TIn>(in, [](const TIn &a, const TIn &b) { return a > b ? a : b; });
        }

    template<typename TIn, typename TOut>
         TOut Min(const std::vector<TIn> &in, const std::function<TOut(TIn)> f) {
            return Min(Map(in, f));
        }

    template<typename TIn, typename TOut>
         TOut Max(const std::vector<TIn> &in, const std::function<TOut(TIn)> f) {
            return Max(Map(in, f));
        }

    template<typename TIn>
         size_t ArgMin(const std::vector<TIn> &in) {
            if (in.empty()) throw std::runtime_error("No element in the sequence.");
            size_t mini = 0;
            for (size_t i = 1; i < in.size(); i++) {
                if (in[i] < in[mini]) {
                    mini = i;
                }
            }
            return mini;
        }

    template<typename TIn, typename TOut>
         size_t ArgMin(const std::vector<TIn> &in, const std::function<TOut(TIn)> f) {
            return ArgMin(Map<TIn, TOut>(in, f));
        }

    template<typename TIn>
         size_t ArgMax(const std::vector<TIn> &in) {
            if (in.empty()) throw std::runtime_error("No element in the sequence.");
            size_t maxi = 0;
            for (size_t i = 1; i < in.size(); i++) {
                if (in[i] > in[maxi]) {
                    maxi = i;
                }
            }
            return maxi;
        }

    template<typename TIn, typename TOut>
         size_t ArgMax(const std::vector<TIn> &in, const std::function<TOut(TIn)> f) {
            return ArgMax(Map<TIn, TOut>(in, f));
        }

    template<typename TIn>
         TIn First(const std::vector<TIn> &in, std::function<bool(TIn)> f, const TIn &def = TIn()) {
            for (auto i : in) {
                if (f(i)) {
                    return i;
                }
            }
            return def;
        }

    template<typename TIn, typename TOut>
         std::vector<TOut> Zip(const std::vector<TIn> &in1, const std::vector<TIn> &in2, std::function<TOut(TIn, TIn)> f) {
            if (in1.size() != in2.size()) {
                throw std::runtime_error("Inconsistent size.");
            }
            std::vector<TOut> result(in1.size());
            for (size_t i = 0; i < in1.size(); i++) {
                result[i] = f(in1[i], in2[i]);
            }
            return result;
        }

    template<typename T>
         std::vector<T> Unique(const std::vector<T> &in) {
            std::set<T> s(in.begin(), in.end());
            return std::vector<T>(s.begin(), s.end());
        }

    template<typename T>
         std::vector<T> Filter(const std::vector<T> &in, const std::function<bool(T)> f) {
            std::vector<T> result;
            result.reserve(in.size());
            for (auto i : in) {
                if (f(i)) {
                    result.push_back(i);
                }
            }
            return result;
        }

    template<typename T>
         size_t Count(const std::vector<T> &in, const std::function<bool(T)> f) {
            size_t count;
            for (auto i : in) {
                if (f(i)) {
                    count++;
                }
            }
            return count;
        }
    
    template<typename T>
         bool Any(const std::vector<T> &in, std::function<bool(const T &)> f) {
            for (auto i : in) {
                if (f(i)) {
                    return true;
                }
            }
            return false;
        }

    template<typename T>
         bool All(const std::vector<T> &in, const std::function<bool(T)> f) {
             for (auto i : in) {
                 if (!f(i)) return false;
             }
             return true;
        }
};

#endif
