#include <iostream>
#include <chrono>
#include <omp.h>
#include "filemanager.h"

#define IMAGE_SIZE 2499000
#define THREAD_NUM 2
#define TASK_NUM 8

void ompGetDataSerialFromFile()
{
#pragma omp parallel for num_threads(THREAD_NUM)
    for (int i = 0; i < TASK_NUM; ++i)
    {
        int index = i;
        int data = FileManager::GetInstance()->getDataSerial(index);
        std::cout << "Task[" << i << "] -> " << data << std::endl;
    }
}

void ompGetDataSerialFromWeb()
{
#pragma omp parallel for num_threads(THREAD_NUM)
    for (int i = 0; i < TASK_NUM; ++i)
    {
        int index = IMAGE_SIZE + i;
        int data = FileManager::GetInstance()->getDataSerial(index);
        std::cout << "Task[" << i << "] -> " << data << std::endl;
    }
}

void ompGetDataSerial()
{
#pragma omp parallel for num_threads(THREAD_NUM)
    for (int i = 0; i < TASK_NUM; ++i)
    {
        int index = i % 2 == 0 ? i : IMAGE_SIZE + i;
        int data = FileManager::GetInstance()->getDataSerial(index);
        std::cout << "Task[" << i << "] -> " << data << std::endl;
    }
}

void ompGetDataParallelFromFile()
{
#pragma omp parallel for num_threads(THREAD_NUM)
    for (int i = 0; i < TASK_NUM; ++i)
    {
        int index = i;
        int data = FileManager::GetInstance()->getDataParallel(index);
        std::cout << "Task[" << i << "] -> " << data << std::endl;
    }
}

void ompGetDataParallelFromWeb()
{
#pragma omp parallel for num_threads(THREAD_NUM)
    for (int i = 0; i < TASK_NUM; ++i)
    {
        int index = IMAGE_SIZE + i;
        int data = FileManager::GetInstance()->getDataParallel(index);
        std::cout << "Task[" << i << "] -> " << data << std::endl;
    }
}

void ompGetDataParallel()
{
#pragma omp parallel for num_threads(THREAD_NUM)
    for (int i = 0; i < TASK_NUM; ++i)
    {
        int index = i % 2 == 0 ? i : IMAGE_SIZE + i;
        int data = FileManager::GetInstance()->getDataParallel(index);
        std::cout << "Task[" << i << "] -> " << data << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    // 创建测试文件
    FileManager::SetFilePath("data/Iceland.jpg");
    // 并发测试串行计算仅从文件读取
    auto start = std::chrono::steady_clock::now();
    ompGetDataSerialFromFile();
    std::cout << "Just from file Serial time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::steady_clock::now() - start)
                     .count()
              << "ms" << std::endl;
    // 并发测试串行计算仅从网络（模拟）读取
    start = std::chrono::steady_clock::now();
    ompGetDataSerialFromWeb();
    std::cout << "Just from Web Serial time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::steady_clock::now() - start)
                     .count()
              << "ms" << std::endl;
    // 并发测试串行计算
    start = std::chrono::steady_clock::now();
    ompGetDataSerial();
    std::cout << "Serial time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::steady_clock::now() - start)
                     .count()
              << "ms" << std::endl;
    // 并发测试并行计算仅从文件读取
    start = std::chrono::steady_clock::now();
    ompGetDataParallelFromFile();
    std::cout << "Just from file Parallel time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::steady_clock::now() - start)
                     .count()
              << "ms" << std::endl;
    // 并发测试并行计算仅从网络（模拟）读取
    start = std::chrono::steady_clock::now();
    ompGetDataParallelFromWeb();
    std::cout << "Just from Web Parallel time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::steady_clock::now() - start)
                     .count()
              << "ms" << std::endl;
    // 并发测试并行计算
    start = std::chrono::steady_clock::now();
    ompGetDataParallel();
    std::cout << "Parallel time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                     std::chrono::steady_clock::now() - start)
                     .count()
              << "ms" << std::endl;
    return 0;
}