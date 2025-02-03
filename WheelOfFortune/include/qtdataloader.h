#ifndef QTDATALOADER_H
#define QTDATALOADER_H

#include "dataloader.h"

#include <QtXml>

/**
 * \brief QtDataLoader Qt implementation data loader
 */
class QtDataLoader : public DataLoader
{
public:
                                QtDataLoader(std::shared_ptr<ILogger> logger);
    virtual                     ~QtDataLoader() noexcept = default;
    virtual bool                loadXml(const char* path) override;

};
#endif //QTDATALOADER_H