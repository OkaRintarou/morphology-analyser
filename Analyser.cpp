#define _CRT_SECURE_NO_WARNINGS
#include "Analyser.h"

Analyser::Analyser(string inFile, string outFile) :state(0), C(0), iskey(-1),
token{ 0 }, lexemebegin(0), line(1), row(0), lastPtr(0),
char_count(0), ID_count(0), NUM_count(0), keywd_count(0), op_count(0),
isFirst(true), lastRow(0)
{
    ifs.open(inFile);
    if (!ifs)
    {
        cerr << "输入文件打开失败，文件名：" << inFile << endl;
        exit(-1);
    }
    ofs.open(outFile);
    if (!ofs)
    {
        cerr << "输出文件打开失败，文件名：" << outFile << endl;
        exit(-1);
    }
    memset(buffer, 0, buffer_size);
    ifs.read(buffer, buffer_size / 2 - 1);
    forward = buffer;
}



Analyser::~Analyser()
{
    ifs.close();
    ofs.close();
}



void Analyser::run()
{
    cout << "---------------------------------------------" << endl;
    cout << "开始词法分析" << endl;
    cout << "---------------------------------------------" << endl;
    state = 0;
    do
    {
        switch (state)
        {
        case 0:         // 初始状态
            memset(token, 0, token_size);
            get_char();
            get_nbc();
            switch (C)
            {
            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f':
            case 'g':
            case 'h':
            case 'i':
            case 'j':
            case 'k':
            case 'l':
            case 'm':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 's':
            case 't':
            case 'u':
            case 'v':
            case 'w':
            case 'x':
            case 'y':
            case 'z':
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'I':
            case 'J':
            case 'K':
            case 'L':
            case 'M':
            case 'N':
            case 'O':
            case 'P':
            case 'Q':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'Z':
            case '_':state = 1; break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':state = 2; break;
            case '<':state = 8; break;
            case '>':state = 9; break;
            case ':':state = 0; _return(":", "-"); break;
            case '/':state = 11; break;
            case '=':state = 16; break;
            case '+':state = 14; break;
            case '-':state = 15; break;
            case '*':state = 17; break;
            case '&':state = 10; break;
            case '|':state = 22; break;
            case '%':state = 18; break;
            case '^':state = 19; break;
            case '(':state = 0; _return("(", "-"); break;
            case ')':state = 0; _return(")", "-"); break;
            case '[':state = 0; _return("[", "-"); break;
            case ']':state = 0; _return("]", "-"); break;
            case '{':state = 0; _return("{", "-"); break;
            case '}':state = 0; _return("}", "-"); break;
            case '\"':state = 27; break;
            case '\'':state = 28; break;
            case '~':state = 0; _return("~", "-"); break;
            case ';':state = 0; _return(";", "-"); break;
            case ',':state = 0; _return(",", "-"); break;
            case '.':state = 0; _return(".", "-"); break;
            case '?':state = 0; _return("?", "-"); break;
            case '!':state = 24; break;
            case '\\':state = 0; _return("\\", "-"); break;
            case '#':state = 25; break;
            default:state = 13; break;
            }
            break;
        case 1:     // 标识符状态
            cat();
            get_char();
            if (letter() || digit() || C == '_')state = 1;
            else {
                retract();
                state = 0;
                iskey = reserve();
                if (iskey != -1)_return(token, "-");
                else {
                    _return("ID", tableinsert());
                }
            }
            break;
        case 2:     // 常数状态
            cat();
            get_char();
            switch (C) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':state = 2; break;
            case '.':state = 3; break;
            case 'e':
            case 'E':state = 5; break;
            default:
                retract();
                state = 0;
                _return("NUM", SToI());
                break;
            }
            break;
        case 3:     // 小数点状态
            cat();
            get_char();
            if (digit())state = 4;
            else {
                error();
                state = 0;
            }
            break;
        case 4:     //小数状态
            cat();
            get_char();
            switch (C)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':state = 4; break;
            case 'e':
            case 'E':state = 5; break;
            default:
                retract();
                state = 0;
                _return("NUM", SToF());
                break;
            }
            break;
        case 5:     // 指数状态
            cat();
            get_char();
            switch (C) {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':state = 7; break;
            case '+':
            case '-':state = 6; break;
            default:
                retract();
                error();
                state = 0;
                break;
            }
            break;
        case 6:
            cat();
            get_char();
            if (digit())state = 7;
            else {
                retract();
                error();
                state = 0;
            }
            break;
        case 7:
            cat();
            get_char();
            if (digit())state = 7;
            else {
                retract();
                state = 0;
                _return("NUM", SToF());
            }
            break;
        case 8:     // '<'状态
            cat();
            get_char();
            switch (C)
            {
            case'=':cat(); state = 0; _return("relop", "LE"); break;
            case'<':cat(); state = 20; break;
            default:
                retract();
                state = 0;
                _return("relop", "LT");
                break;
            }
            break;
        case 9:     // '>'状态
            cat();
            get_char();
            switch (C)
            {
            case'=':cat(); state = 0; _return("relop", "GE"); break;
            case'>':cat(); state = 21; break;
            default:
                retract();
                state = 0;
                _return("relop", "GT");
                break;
            }
            break;
        case 10:     // '&'状态
            cat();
            get_char();
            switch (C)
            {
            case'&':cat(); state = 0; _return("&&", "-"); break;
            case'=':cat(); state = 0; _return("&=", "-"); break;
            default:
                retract();
                state = 0;
                _return("&", "-");
                break;
            }
            break;
        case 11:       // '/'状态
            cat();
            get_char();
            switch (C)
            {
            case'*':cat(); state = 12; break;
            case'/':cat(); state = 23; break;
            case'=':cat(); state = 0; _return("/=", "-"); break;
            default:
                retract();
                state = 0;
                _return("/", "-");
                break;
            }
            break;


        case 12:        // 注释处理状态
            get_char();
            while (C != '*')get_char();
            get_char();
            if (C == '/')state = 0;
            else state = 12;
            break;
        case 13:        //错误处理状态
            error();
            state = 0;
            break;
        case 14:        // '+'状态
            cat();
            get_char();
            switch (C)
            {
            case'+':cat(); state = 0; _return("++", "-"); break;
            case'=':cat(); state = 0; _return("+=", "-"); break;
            default:
                retract();
                state = 0;
                _return("+", "-");
                break;
            }
            break;
        case 15:        // '-'状态
            cat();
            get_char();
            switch (C)
            {
            case'-':cat(); state = 0; _return("--", "-"); break;
            case'>':cat(); state = 0; _return("->", "-"); break;
            case'=':cat(); state = 0; _return("-=", "-"); break;
            default:
                retract();
                state = 0;
                _return("-", "-");
                break;
            }
            break;
        case 16:        // '='状态
            cat();
            get_char();
            if (C == '=') {
                cat();
                state = 0;
                _return("relop", "EQ");
            }
            else {
                retract();
                state = 0;
                _return("assign-op", "-");
            }
            break;
        case 17:        // '*'状态
            cat();
            get_char();
            if (C == '=') {
                cat();
                state = 0;
                _return("*=", "-");
            }
            else {
                retract();
                state = 0;
                _return("*", "-");
            }
            break;
        case 18:        // '%'状态
            cat();
            get_char();
            if (C == '=') {
                cat();
                state = 0;
                _return("%=", "-");
            }
            else {
                retract();
                state = 0;
                _return("%", "-");
            }
            break;
        case 19:        // '^'状态
            cat();
            get_char();
            if (C == '=') {
                cat();
                state = 0;
                _return("^=", "-");
            }
            else {
                retract();
                state = 0;
                _return("^", "-");
            }
            break;
        case 20:        // '<<'状态
            cat();
            get_char();
            if (C == '=') {
                cat();
                state = 0;
                _return("<<=", "-");
            }
            else {
                retract();
                state = 0;
                _return("<<", "-");
            }
            break;
        case 21:        // '>>'状态
            cat();
            get_char();
            if (C == '=') {
                cat();
                state = 0;
                _return(">>=", "-");
            }
            else {
                retract();
                state = 0;
                _return(">>", "-");
            }
            break;
        case 22:        // '|'状态
            cat();
            get_char();
            switch (C)
            {
            case'|':cat(); state = 0; _return("||", "-"); break;
            case'=':cat(); state = 0; _return("|=", "-"); break;
            default:
                retract();
                state = 0;
                _return("|", "-");
                break;
            }
            break;
        case 23:        // '//'注释
            get_char();
            while (C != '\n' && C != 0)get_char();
            state = 0;
            if (C == 0)
                retract();
            break;
        case 24:        // '!'状态
            cat();
            get_char();
            if (C == '=') {
                cat();
                state = 0;
                _return("!=", "-");
            }
            else {
                retract();
                state = 0;
                _return("!", "-");
            }
            break;
        case 25:        // '#'状态
            if (isFirst)
            {
                state = 26;
            }
            else
            {
                state = 13;
            }
            break;
        case 26:        // 处理预编译
            get_char();
            while (C != '\n' && C != 0)get_char();
            state = 0;
            if (C == 0)
                retract();
            break;
        case 27:        // "状态
            cat();
            get_char();
            while (C != '\"' && C != 0)
            {
                cat();
                get_char();
            }
            cat();
            _return("ID", tableinsert());
            state = 0;
            break;
        case 28:        // '状态
            cat();
            get_char();
            while (C != '\'' && C != 0)
            {
                cat();
                get_char();
            }
            cat();
            _return("ID", tableinsert());
            state = 0;
            break;
        }
    } while (C != 0);
    cout << "---------------------------------------------" << endl;
    cout << "词法分析结束" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "用户自定义标识符：" << endl;
    for (int i = 0; i < tokentable.size(); i++) {
        cout << i << '\t' << tokentable[i] << endl;
    }
    cout << "---------------------------------------------" << endl;
    cout << "语句行数：" << line << endl
        << "字符总数：" << char_count << endl
        << "标识符：" << ID_count << endl
        << "常数：" << NUM_count << endl
        << "关键字：" << keywd_count << endl
        << "运算符及其他符号：" << op_count << endl;
    cout << "---------------------------------------------" << endl;
    cout << "错误数：" << errors.size() << endl;
    for (int i = 0; i < errors.size(); i++)
    {
        cout << errors[i];
    }

    ofs << "---------------------------------------------" << endl;
    ofs << "用户自定义标识符：" << endl;
    for (int i = 0; i < tokentable.size(); i++) {
        ofs << i << '\t' << tokentable[i] << endl;
    }

    ofs << "---------------------------------------------" << endl;
    ofs << "语句行数：" << line << endl
        << "字符总数：" << char_count << endl
        << "标识符：" << ID_count << endl
        << "常数：" << NUM_count << endl
        << "关键字：" << keywd_count << endl
        << "运算符及其他符号：" << op_count << endl;
    ofs << "---------------------------------------------" << endl;
    ofs << "错误数：" << errors.size() << endl;
    for (int i = 0; i < errors.size(); i++)
    {
        ofs << errors[i];
    }

}

void Analyser::get_char()
{
    char_count++;
    lastRow = row;
    row++;
    C = *forward;
    if (C == '\n')
    {
        isFirst = true;
        line++;
        row = 0;
    }
    else if (isFirst && C == '#')
        isFirst = true;
    else if (C != '\t' && C != ' ')
        isFirst = false;
    lastPtr = forward;
    forward++;
    if (*forward == 0)
    {
        if (forward - buffer == bufferEndL)
        {
            forward++;
            memset(forward, 0, buffer_size / 2);
            lastFilePtr = ifs.tellg();
            ifs.read(forward, buffer_size / 2 - 1);
        }
        else if (forward - buffer == bufferEndR)
        {
            forward = buffer;
            memset(forward, 0, buffer_size / 2);
            lastFilePtr = ifs.tellg();
            ifs.read(forward, buffer_size / 2 - 1);
        }
    }
}

void Analyser::get_nbc()
{
    while (C == ' ' || C == '\n' || C == '\t')
    {
        get_char();
    }
}

void Analyser::cat()
{
    for (int i = 0; i < token_size - 1; i++)
    {
        if (token[i] == 0) {
            token[i] = C;
            return;
        }
    }
}

bool Analyser::letter()
{
    return (C >= 'a' && C <= 'z') || (C >= 'A' && C <= 'Z');
}

bool Analyser::digit()
{
    return C >= '0' && C <= '9';
}

void Analyser::retract()
{
    char_count--;
    row = lastRow;
    forward = lastPtr;
    if (forward + 1 == buffer + bufferEndL || forward + 1 == buffer + bufferEndR)
        ifs.seekg(lastFilePtr);
    if (*forward == '\n')
        line--;
}



int Analyser::reserve()
{
    for (int i = 0; i < keyword_size; i++)
    {
        if (keywordtable[i].compare(token) == 0)
            return i;
    }
    return -1;
}

int Analyser::SToI()
{
    int result;
    if (sscanf(token, "%d", &result) == 1)
        return result;
    return 0;
}

float Analyser::SToF()
{
    float result;
    if (sscanf(token, "%f", &result) == 1)
        return result;
    return 0;
}

int Analyser::tableinsert()
{
    int result = 0;
    bool isExist = false;
    for (; result < tokentable.size(); result++)
    {
        if (tokentable[result].compare(token) == 0)
        {
            isExist = true;
            break;
        }
    }
    if (!isExist)
        tokentable.push_back(token);
    return result;
}

void Analyser::error()
{
    ostr.clear();
    ostr.str("");
    ostr << "Error happened at line:" << line << " row:" << row << " token is \"" << token << C << "\"" << endl;
    errors.push_back(ostr.str());
}
