#ifndef WEAKPTR_WRAP_H
#define WEAKPTR_WRAP_H

#include <iostream>
#include <memory>
#include <stdexcept>

namespace Essentia
{
    template<typename T>
    class WeakptrWrapper {
        public:
            WeakptrWrapper(std::weak_ptr<T> weakPtr) : weakPtr_(weakPtr) {}

            T* operator->() {
                auto sharedPtr = weakPtr_.lock();
                if (!sharedPtr) {
                    throw std::runtime_error("Invalid access to expired pointer");
                }
                return sharedPtr.get();
            }

        private:
            std::weak_ptr<T> weakPtr_;
    };
}

#endif //!WEAKPTR_WRAP_H