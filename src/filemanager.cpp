#include <iostream>
#include <thread>
#include <mutex>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"
#include "filemanager.h"

static volatile FileManager *instance = nullptr;
static std::mutex mtx;

std::string FileManager::filePath = "Iceland.jpg"; // 默认值

void FileManager::SetFilePath(const std::string &path)
{
    const std::lock_guard<std::mutex> lock(mtx);
    FileManager::filePath = path;
}

std::string FileManager::GetFilePath()
{
    return FileManager::filePath;
}

FileManager *FileManager::GetInstance()
{
    // 双重校验锁
    if (instance == nullptr)
    {
        const std::lock_guard<std::mutex> lock(mtx);
        if (instance == nullptr)
        {
            instance = new FileManager();
        }
    }
    return const_cast<FileManager *>(instance);
}

int FileManager::getDataSerial(int index)
{
    const std::lock_guard<std::mutex> lock(mtx);
    // 从缓存查找
    // if (memData.find(index) != memData.end())
    // {
    //     return memData[index];
    // }
    // 从本地查找
    cv::Mat img = cv::imread(FileManager::filePath, cv::IMREAD_GRAYSCALE);
    int data = -1;
    if (!img.empty() && index >= 0 && index < img.cols * img.rows)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 模拟本地数据读取延迟
        int x = index % img.cols;
        int y = index / img.cols;
        data = static_cast<int>(img.at<unsigned char>(y, x));
        memData[index] = data; // 写入缓存
        return data;
    }
    // 模拟从在线数据库查找
    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 模拟网络延迟
    data = index;
    memData[index] = data; // 写入缓存
    return data;
}

int FileManager::getDataParallel(int index)
{
    // 模拟从在线数据库查找
    auto _getFromWeb = [&](int &res, bool &finished) -> void
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // 模拟网络延迟
        res = index;
        memData[index] = res; // 写入缓存
        finished = true;
    };

    const std::lock_guard<std::mutex> lock(mtx);
    // 从缓存查找
    // if (memData.find(index) != memData.end())
    // {
    //     return memData[index];
    // }
    // 开启一个线程用于在web上获取数据
    int dataWeb = -1;
    bool finished = false;
    std::thread th(_getFromWeb, std::ref(dataWeb), std::ref(finished));
    th.detach();
    // 主线程查找本地数据
    cv::Mat img = cv::imread(FileManager::filePath, cv::IMREAD_GRAYSCALE);
    if (!img.empty() && index >= 0 && index < img.cols * img.rows)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // 模拟本地数据读取延迟
        int x = index % img.cols;
        int y = index / img.cols;
        int dataFile = static_cast<int>(img.at<unsigned char>(y, x));
        memData[index] = dataFile; // 写入缓存
        return dataFile; // 找到本地数据后就直接返回本地数据
    }
    // 本地没有数据，等待线程获取数据
    while (!finished)
    {
        std::this_thread::yield();
    }
    memData[index] = dataWeb; // 写入缓存
    return dataWeb;
}