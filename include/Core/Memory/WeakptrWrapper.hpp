#ifndef WEAKPTR_WRAP_H
#define WEAKPTR_WRAP_H

#include <iostream>
#include <memory>
#include <stdexcept>

namespace Essentia
{
    class Entity;

    template<typename T>
    class WeakptrWrapper {
        public:
            WeakptrWrapper(std::weak_ptr<T> weakPtr) : weakPtr_(weakPtr) {}
            WeakptrWrapper() : weakPtr_() {}

            std::weak_ptr<T> weakPtr_;

            T* operator->() {
                auto sharedPtr = weakPtr_.lock();
                if (!sharedPtr) {
                    throw std::runtime_error("Invalid access to expired pointer");
                }
                return sharedPtr.get();
            }
    };
}

#endif //!WEAKPTR_WRAP_H