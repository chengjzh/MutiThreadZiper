//
//  main.cpp
//  MutiThreadZiper
//
//  Created by 张成 on 2023/9/4.
//

#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <queue>
#include <memory>
#include "IZip.h"
#include "ZipWapper.hpp"
#include "Util.h"


int main(int argc, const char * argv[]) {
    
    //1. check the paramter
    //command line Usage
    //argv[1] : zip 文件路径
    //argv[2] : 解压后的本地目录
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <zipfile> <output Path> " << std::endl;
        return 1;
    }
    
    std::string zipFilePath = argv[1];
    std::string outputPath =  argv[2];
    
    std::shared_ptr<IZip> pZip(new ZipWapper(zipFilePath, outputPath));
    
    if(!pZip->OpenZipFile()){
        std::cerr << "open file failed:  file name is "<<  zipFilePath << std::endl;
        return 1;
    }
    
    Util::CreateDirIfNotExisted(outputPath);
    
    //获取文件数量
    int numFiles = pZip->GetNumsOfFile(); // 文件总数
    int numThreads = 3; //设定线程数
    if(numFiles <= 0){
        std::cerr << "get entry files failed:  file name is "<<  zipFilePath << std::endl;
        return 1;
    }
    else{
        std::cout << "Zip file has "<< numFiles << " entry" << std::endl;
    }

    if(numFiles < 3){
        numThreads = 1;
    }

    std::vector<std::thread> threads;
    static std::atomic<int> currentIndex(0); // 用于跟踪当前要处理的文件索引
    
    // 启动线程池
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back([=](){
            while (true) {
                int file_index = currentIndex.fetch_add(1, std::memory_order_relaxed);
                if (file_index >= numFiles) {
                    break; // 所有文件都已解压完毕
                }
                
                pZip->UnzipByIndex(i, file_index);
            }
        });
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "All files are unzipped." << std::endl;

    return 0;
}
