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
                                    TinyDataLoader(std::shared_ptr<ILogger> logger);
    virtual                         ~TinyDataLoader() = default;
    virtual bool                    loadXml(const char* path) override;

private:
    string                          depolonise(const std::wstring& text);   //!< Changes all polish characters in given text to its Latin form.
};
#endif //TINYDATALOADER_H