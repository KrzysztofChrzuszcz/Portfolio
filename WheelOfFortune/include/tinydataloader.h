#ifndef TINYDATALOADER_H
#define TINYDATALOADER_H

#include "dataloader.h""
#include "tinyxml2.h"

#include <vector>
#include <string>

using std::vector;
using std::string;

/**
 * \brief TinyDataLoader TinyXML2 implementation data loader
 */
class TinyDataLoader : public DataLoader
{
public:
    virtual bool                        loadXml(const char* path) override;
};
#endif //TINYDATALOADER_H