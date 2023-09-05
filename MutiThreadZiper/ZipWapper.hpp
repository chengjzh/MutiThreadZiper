//
//  ZipWapper.hpp
//  MutiThreadZiper
//
//  Created by 张成 on 2023/9/4.
//

#ifndef ZipWapper_hpp
#define ZipWapper_hpp

#include <zip.h>
#include <string>
#include "IZip.h"

class ZipWapper : public IZip{
public:
    ZipWapper(const std::string& zipFile, const std::string& outputPath){
        m_sZipFile = zipFile;
        m_sOutputPath = outputPath;
    }
    
    ~ZipWapper(){
        if(m_zip){
            zip_close(m_zip);
        }
    }
    
    ZipWapper(ZipWapper&& zip) noexcept:
    m_zip(zip.m_zip),
    m_sZipFile(zip.m_sZipFile),
    m_sOutputPath(zip.m_sOutputPath){
        zip.m_zip = nullptr;
    }
    
    ZipWapper& operator=(ZipWapper&& zip)noexcept{
        if(this == &zip){
            return *this;
        }
        
        m_sZipFile = std::move(zip.m_sZipFile);
        m_sOutputPath = std::move(zip.m_sOutputPath);
        if(m_zip){
            zip_close(m_zip);
        }
        
        m_zip = zip.m_zip;
        zip.m_zip = nullptr;
        return *this;
    }
    
    ZipWapper(const ZipWapper& zip) = delete;
    ZipWapper& operator=(const ZipWapper& zip) = delete;
    
public:
    virtual bool OpenZipFile() override;
    virtual int GetNumsOfFile() override;
    virtual bool UnzipByIndex(int theadID, int index) override;
    
    
private:
    std::string m_sZipFile;
    std::string m_sOutputPath;
    struct zip* m_zip = nullptr;
};

#endif /* ZipWapper_hpp */
