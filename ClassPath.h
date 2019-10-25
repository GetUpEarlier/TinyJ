//
// Created by chenjs on 2019/10/24.
//

#ifndef TINYJ_CLASSPATH_H
#define TINYJ_CLASSPATH_H

#include <string>
#include <fstream>
#include <unzip.h>
#include <vector>
#include "Primitive.h"

namespace TinyJ{

using std::string;
using std::ifstream;
using std::ios;
using std::vector;

class ClassPath{
public:
    virtual U8* findClassFile(string className) = 0;
};

class DirClassPath: public ClassPath{
private:
    string _classPath;
public:
    explicit DirClassPath(string classPath){
        _classPath = std::move(classPath);
    }

    U8* findClassFile(string className) override {
        ifstream file(_classPath + "/" + className, ios::binary | ios::in);
        if(!file.is_open()){
            return nullptr;
        }
        size_t size = file.tellg();
        U8* buffer = new U8[size];
        file.read((char*)buffer, size);
        return buffer;
    }
};

/*class ZipClassPath: public ClassPath{
private:
    string _classPath;
public:
    explicit ZipClassPath(string classPath){
        _classPath = std::move(classPath);
    }

    U8* findClassFile(string className) override {
        vector<string> split = splitClassName(className);
        unzFile file = unzOpen(_classPath.c_str());
        if(file == nullptr){
            return nullptr;
        }
        for(string& dir: split){
            if(unzLocateFile(file, dir.c_str(), 2) == UNZ_OK){

            }
        }
    }

    vector<string> splitClassName(string className){
        string buffer;
        vector<string> split;
        for(char ch: className){
            if(ch == '/'){
                split.push_back(buffer);
                buffer.clear();
            }else{
                buffer.push_back(ch);
            }
        }
        split.push_back(buffer);
        return split;
    }
};*/

}

#endif //TINYJ_CLASSPATH_H
