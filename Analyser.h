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
    Analyser(string inFile, string outFile);                // inFile:�����ļ���     outFile:����ļ���
    ~Analyser();
    void run();                 // �����ʷ���������ӿ�

private:
    const static int token_size = 1024;                     // �������ʶ�𵥴��ַ����������Ĵ�С
    const static int buffer_size = 2048;                    // �Գƻ�������С
    const static int bufferEndL = buffer_size / 2 - 1;      // ��������ĩβ
    const static int bufferEndR = buffer_size - 1;          // �������Ҳ�ĩβ

    int state;                  // ��ǰ״̬
    char C;                     // ��ǰ�����ַ�
    int iskey;                  // -1Ϊ�û��Զ����ʶ��������ֵΪ�ؼ��ּǺ�
    char token[token_size];     // �������ʶ�𵥴��ַ���
    char* lexemebegin;          // ָ�����뻺�����е�ǰ���ʵĿ�ʼλ��
    char* forward;              // �ַ�ָ�룬��ǰ
    char buffer[buffer_size];   // ���뻺����


    void get_char();            // forward���ַ�����C���ƶ�forward
    void get_nbc();             // ���C�Ƿ�Ϊ�ո����ǣ���������get_char
    void cat();                 // ��C���ַ�������token����
    bool letter();              // �ж�C���ַ��Ƿ�Ϊ��ĸ
    bool digit();               // �ж�C���ַ��Ƿ�Ϊ����
    void retract();             // forward����
    int reserve();              // ����token��ؼ��ֱ�����ֵΪ�ؼ��ּǺţ�����Ϊ-1
    int SToI();                 // ��token�е��ַ���ת��������
    float SToF();               // ��token�е��ַ���ת���ɸ�����
    int tableinsert();          // ���Զ����ʶ��������ű������ڷ��ű��λ��ָ��
    void error();               // ������

    template<typename T>
    void _return(string sign, T property)   // ��ʶ����ļǺŷ��ظ����ó���
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

    const static int keyword_size = 32;     // �ؼ����б��С

    vector<string> tokentable;              // ���ű�
    string keywordtable[keyword_size] =     // �ؼ��ֱ�
    {
        "char","double","enum","float","int",
        "long","short","signed","struct","union",
        "unsigned","void","for","do","while",
        "break","continue","if","else","goto",
        "switch","case","default","return","auto",
        "extern","register","static","const","sizeof",
        "typedef","volatile"
    };

    ifstream ifs;                          // �����ļ���
    ofstream ofs;                          // ����ļ���


    int line;                              // ��ǰ����
    int row;                               // ��ǰ����
    char* lastPtr;                         // ��һ��forward����

    int char_count;                        // �ַ�����

    int ID_count;                          // ��ʶ������
    int NUM_count;                         // ����ֵ����
    int keywd_count;                       // �ؼ��ּ���
    int op_count;                          // ��������������ż���

    bool isFirst;                          // ����
    int lastRow;                           // ��һ����ֵ

    vector<string> errors;                 // �����
    ostringstream ostr;                    // �ַ�����

    streampos lastFilePtr;                 // ��һ���ļ�ָ��ֵ
};

