#ifndef MODELCACHEMANAGER_HPP
#define MODELCACHEMANAGER_HPP

#include <unordered_map>
#include <memory>
#include <string>
#include <Framework/Components/Model/Model.hpp>

namespace Essentia
{
    class ModelCacheManager 
    {
        private:
            std::unordered_map<std::string, std::shared_ptr<Model>> modelCache;
            ModelCacheManager();

        public:
            ~ModelCacheManager();
            static ModelCacheManager& getInstance();

            std::shared_ptr<Model> loadModel(const std::string& path, bool inverseUvY = true);

            void unloadModel(const std::string& path);
            void clearCache();

            size_t getCacheSize() const;
    };
}

#endif // MODELCACHEMANAGER_HPP