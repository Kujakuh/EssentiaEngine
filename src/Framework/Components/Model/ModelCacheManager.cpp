#include <Framework/Components/Model/ModelCacheManager.hpp>

namespace Essentia
{
    ModelCacheManager::ModelCacheManager() = default;
    ModelCacheManager::~ModelCacheManager() = default;

    ModelCacheManager& ModelCacheManager::getInstance()
    {
        static ModelCacheManager instance;
        return instance;
    }

    std::shared_ptr<Model> ModelCacheManager::loadModel(const std::string& path, bool inverseUvY)
    {
        auto it = modelCache.find(path);
        if (it != modelCache.end()) {
            return it->second;
        }

        auto model = std::make_shared<Model>();
        model->initializeShader();

        model->loadModelInner(path, inverseUvY);
        modelCache[path] = model;

        return model;
    }

    bool ModelCacheManager::isLoaded(const std::string& path)
    {
        auto it = modelCache.find(path);
        return it != modelCache.end();
    }

    void ModelCacheManager::unloadModel(const std::string& path)
    {
        auto it = modelCache.find(path);
        if (it != modelCache.end()) {
            modelCache.erase(it);
        }
    }

    void ModelCacheManager::clearCache()
    {
        modelCache.clear();
    }

    size_t ModelCacheManager::getCacheSize() const
    {
        return modelCache.size();
    }
}