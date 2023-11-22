#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <string>
#include <unordered_map>

class FileManager
{
private:
    static std::string filePath;
    std::unordered_map<int, int> memData;

    FileManager() = default; // 单列模式将构造函数隐藏

public:
    ~FileManager() = default;

    static void SetFilePath(const std::string &path);
    static std::string GetFilePath();
    static FileManager *GetInstance();

    int getDataSerial(int index);
    int getDataParallel(int index);
};

#endif // FILEMANAGER_H