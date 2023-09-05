//
//  ZipWapper.cpp
//  MutiThreadZiper
//
//  Created by 张成 on 2023/9/4.
//

#include "ZipWapper.hpp"
#include <iostream>
#include "Util.h"

bool ZipWapper::OpenZipFile(){
    if(m_zip){
        zip_close(m_zip);
    }
    
    m_zip = zip_open(m_sZipFile.c_str(), 0, NULL);
    if (!m_zip) {
        std::cerr << "Error opening zip file: " << m_sZipFile << std::endl;
        return false;
    }
    
    return true;
}

int ZipWapper::GetNumsOfFile(){
    if (!m_zip) {
        std::cerr << "Error opening zip file: " << m_sZipFile << std::endl;
        return 0;
    }
    
    return static_cast<int>(zip_get_num_entries(m_zip, 0));
}

bool ZipWapper::UnzipByIndex(int theadID, int index){
    struct zip_stat stat;
    if (zip_stat_index(m_zip, index, 0, &stat) == 0) {
        std::string entryName = stat.name;
        std::string entryOutputPath = m_sOutputPath+ "/" + entryName;
        
        if (stat.valid) {
            if (stat.name[strlen(stat.name) - 1] == '/') {
                Util::CreateDirIfNotExisted(entryOutputPath);
                return true;
            }
        }
        else {
            Util::printMessage("Failed to get information for entry" + std::to_string(index));
            return false;
        }

        struct zip_file* zipFile = zip_fopen_index(m_zip, index, 0);
        if (!zipFile) {
            Util::printMessage("Error opening entry:" + entryName);
            return false;
        }
        
        FILE* outputFile = fopen(entryOutputPath.c_str(), "wb");
        if (!outputFile) {
            Util::printMessage("####Error creating output file:" + entryOutputPath);
            zip_fclose(zipFile);
            return false;
        }

        char buffer[1024];
        int bytesRead;
        while ((bytesRead = static_cast<int>(zip_fread(zipFile, buffer, sizeof(buffer)))) > 0) {
            fwrite(buffer, 1, bytesRead, outputFile);
        }

        fclose(outputFile);
        zip_fclose(zipFile);
        Util::printMessage("thread " + std::to_string(theadID) + " Extracted: " + entryName);
        return true;
    }
    else{
        return false;
    }
}
