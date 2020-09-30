#ifndef FILECONTAINER_H
#define FILECONTAINER_H

#include <set>
#include <string>
#include <algorithm>

// File container contains unique files.
// Uniqueness determined by absolute file path.
template <class T>
class FileContainer
{
public:
    typename std::multiset<T>::iterator AddFile(std::string filePath, T file)
    {
        _filePaths.insert(filePath);
        return _files.insert(file);
    }

    bool Contains(std::string filePath)
    {
        return _filePaths.find(filePath) != _filePaths.end();
    }

    typename std::multiset<T>::iterator Begin()
    {
        return _files.begin();
    }

    typename std::multiset<T>::iterator End()
    {
        return _files.end();
    }

private:
    std::multiset<T> _files;
    std::set<std::string> _filePaths;
};


#endif // FILECONTAINER_H
