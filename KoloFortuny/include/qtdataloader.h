#ifndef QTDATALOADER_H
#define QTDATALOADER_H

#include "dataloader.h"

#include <QtXml>

class QtDataLoader : public DataLoader
{
public:
    virtual bool                loadXml(const char* path) override;

};
#endif //QTDATALOADER_H