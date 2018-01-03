#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

/**
 * �����ֱ�
 */
const string key[14] = {"", "program", "const", "var", "procedure", "begin",
                        "if", "then", "while", "call", "read", "write", "end", "do"};
int line;
int column;

/**
 * �ж��ַ��Ƿ�Ϊ�հ��ַ�
 * @param ch
 * @return trueΪ��
 */
bool isBlank(char ch);

/**
 * �ж��ַ��Ƿ�Ϊ����
 * @param ch
 * @return trueΪ��
 */
bool isDigit(char ch);

/**
 * �ж��ַ��Ƿ�Ϊ��ĸ
 * @param ch
 * @return trueΪ��
 */
bool isLetter(char ch);

/**
 * �ж�strToken�ǹؼ��ֻ����û��Զ���ı�ʶ��
 * @param strToken
 * @return ��Ϊ0�����ʾstrToken
 */
int Reserve(string strToken);

/**
 * ������ָʾ���ص�һ���ַ�λ��
 */
void Retract(fstream &source);

/**
 * ��ch����������strToken
 * @param str
 * @param ch
 * @return
 */
void Concat(string &str, char ch);

int LA();