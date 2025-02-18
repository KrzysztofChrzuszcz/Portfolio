#ifndef DATAPROVIDER_H
#define DATAPROVIDER_H

#include "rapidjson/document.h"

#include <fstream>
#include <list>
#include <string>


/**
 * \brief DataProvider class to storage and access data between application runs.
 */
template <typename T>
class IDataProvider
{
public:
    bool                dump(T& input);
    bool                load(T& output);

};

typedef std::list<std::string> StringList;
class CSVDataProvider : public IDataProvider<StringList>
{
public:
                        CSVDataProvider(const std::string& filePath);
                        ~CSVDataProvider();

    bool                dump(StringList& input);
    bool                load(StringList& output);

private:
    const char*         m_FilePath;
    std::fstream        m_File;
};

/// THOUGHT
/// CSV storage is sufficient to that simple case but has a little inconvenience.
/// We have to retain order while adding or removing in serialization and deserialization.
/// There also can be problem with loading a file from the old version.
/// So better approach is to use JSON as map of variables with their values.
class JSONDataProvider : public IDataProvider<std::string>
{
public:
                        JSONDataProvider(const std::string& filePath);
                        ~JSONDataProvider();


    bool                dump(std::string& input);
    bool                load(std::string& output);

private:
    const char*         m_FilePath;
};

#endif // DATAPROVIDER_H