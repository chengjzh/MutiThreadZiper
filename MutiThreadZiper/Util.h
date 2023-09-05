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
    
    static void printMessage(const std::string& message) {
        std::lock_guard<std::mutex> lock(coutMutex); // 在输出前锁住互斥锁
        std::cout << message << std::endl;
    }
};

#endif /* Util_h */
