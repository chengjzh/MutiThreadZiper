//
//  IZip.h
//  MutiThreadZiper
//
//  Created by 张成 on 2023/9/5.
//

#ifndef IZip_h
#define IZip_h

class IZip{
public:
    //打开一个zip 文件，并且检查可用性
    virtual bool OpenZipFile() = 0;
    
    //获取这个zip包里面有多少文件
    virtual int GetNumsOfFile() = 0;
    
    //根据序号解压其中一个文件
    virtual bool UnzipByIndex(int theadID, int index) = 0;
};
#endif /* IZip_h */
