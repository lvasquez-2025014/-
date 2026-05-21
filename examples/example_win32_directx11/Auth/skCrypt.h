#pragma once
#include <string>
#include <array>

// [ignoring loop detection]
template <typename T, std::size_t N>
class skCrypt_key {
public:
    std::array<T, N> _storage;
    
    constexpr skCrypt_key(const T(&arr)[N]) : _storage{} {
        for (std::size_t i = 0; i < N; ++i) {
            _storage[i] = arr[i] ^ 0x5A; // XOR encryption with key 0x5A
        }
    }
    
    std::string decrypt() const {
        std::string res;
        res.resize(N - 1);
        for (std::size_t i = 0; i < N - 1; ++i) {
            res[i] = _storage[i] ^ 0x5A;
        }
        return res;
    }
};

template <typename T, std::size_t N>
constexpr auto skCrypt(const T(&arr)[N]) {
    return skCrypt_key<T, N>(arr);
}
