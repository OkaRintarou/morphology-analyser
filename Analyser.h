#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

class Analyser
{
public:
    Analyser(string inFile, string outFile);                // inFile:输入文件名     outFile:输出文件名
    ~Analyser();
    void run();                 // 启动词法分析对外接口

private:
    const static int token_size = 1024;                     // 存放正在识别单词字符串缓冲区的大小
    const static int buffer_size = 2048;                    // 对称缓冲区大小
    const static int bufferEndL = buffer_size / 2 - 1;      // 缓冲区左部末尾
    const static int bufferEndR = buffer_size - 1;          // 缓冲区右部末尾

    int state;                  // 当前状态
    char C;                     // 当前读入字符
    int iskey;                  // -1为用户自定义标识符，否则值为关键字记号
    char token[token_size];     // 存放正在识别单词字符串
    char* lexemebegin;          // 指向输入缓冲区中当前单词的开始位置
    char* forward;              // 字符指针，向前
    char buffer[buffer_size];   // 输入缓冲区


    void get_char();            // forward的字符放入C，移动forward
    void get_nbc();             // 检查C是否为空格，若是，反复调用get_char
    void cat();                 // 把C中字符连接在token后面
    bool letter();              // 判断C中字符是否为字母
    bool digit();               // 判断C中字符是否为数字
    void retract();             // forward后退
    int reserve();              // 根据token查关键字表，返回值为关键字记号，否则为-1
    int SToI();                 // 将token中的字符串转换成整数
    float SToF();               // 将token中的字符串转换成浮点数
    int tableinsert();          // 将自定义标识符插入符号表，返回在符号表的位置指针
    void error();               // 错误处理

    template<typename T>
    void _return(string sign, T property)   // 将识别出的记号返回给调用程序
    {
        ofs << "<" << sign << "," << property << ">" << endl;
        cout << "<" << sign << "," << property << ">" << endl;
        for (int i = 0; i < keyword_size; i++)
        {
            if (keywordtable[i].compare(sign) == 0)
            {
                keywd_count++;
                return;
            }
        }
        if (sign.compare("ID") == 0)
            ID_count++;
        else if (sign.compare("NUM") == 0)
            NUM_count++;
        else
            op_count++;

    }

    const static int keyword_size = 32;     // 关键字列表大小

    vector<string> tokentable;              // 符号表
    string keywordtable[keyword_size] =     // 关键字表
    {
        "char","double","enum","float","int",
        "long","short","signed","struct","union",
        "unsigned","void","for","do","while",
        "break","continue","if","else","goto",
        "switch","case","default","return","auto",
        "extern","register","static","const","sizeof",
        "typedef","volatile"
    };

    ifstream ifs;                          // 输入文件流
    ofstream ofs;                          // 输出文件流


    int line;                              // 当前行数
    int row;                               // 当前列数
    char* lastPtr;                         // 上一个forward备份

    int char_count;                        // 字符计数

    int ID_count;                          // 标识符计数
    int NUM_count;                         // 常数值计数
    int keywd_count;                       // 关键字计数
    int op_count;                          // 运算符及其他符号计数

    bool isFirst;                          // 行首
    int lastRow;                           // 上一个列值

    vector<string> errors;                 // 错误表
    ostringstream ostr;                    // 字符串流

    streampos lastFilePtr;                 // 上一次文件指针值
};

