//
//  Util.h
//  MutiThreadZiper
//
//  Created by 张成 on 2023/9/5.
//

#ifndef Util_h
#define Util_h
#include <filesystem>
#include <iostream>
#include <mutex>
#include <ctime>
#include <chrono>

namespace fs = std::filesystem;
static std::mutex coutMutex; // 创建一个互斥锁
class Util{
public:
    static bool CreateDirIfNotExisted(const std::string& pathToCheck){
        if (fs::exists(pathToCheck)) {
            return true;
        }
        else {
            //std::cout << "Path does not exist. Creating it..." << std::endl;
            try
            {
                fs::create_directories(pathToCheck);
                printMessage("Path created: " + pathToCheck);
                return true;
            }
            catch (const std::exception& e) {
                //std::cerr << "Failed to create path: " << e.what() << std::endl;
                printMessage("Failed to create path: " + pathToCheck);
                return false;
            }
        }
    }
    
    static std::string GetCurrentTimeWithMilliseconds() {
        // 获取当前时间点（毫秒级别）
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

        // 计算毫秒偏移量
        auto ms_offset = now - ms;

        // 将时间点转换为时间戳
        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        // 使用strftime函数将时间戳转换为可读的时间字符串
        char timeStr[100];
        std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", std::localtime(&currentTime));

        // 获取毫秒部分并拼接到时间字符串
        char result[200];
        std::sprintf(result, "%s.%03lld", timeStr, ms_offset.count());

        return std::string(result);
    }
    
    static void printMessage(const std::string& message) {
        std::lock_guard<std::mutex> lock(coutMutex); // 在输出前锁住互斥锁
        std::cout << GetCurrentTimeWithMilliseconds() << " " << message << std::endl;
    }
};

#endif /* Util_h */
