#include "define.h"

#define FILE_PATH "test.txt"
#define LINELEG 100		//�����ļ�ÿ�е�����ȡ�ַ���
#define CODENUM 100
char line[LINELEG];
int CH;	//�кű���
//*************************************************
void GetChar(char &ch) {					//��ȡ��һ���ַ�
    ch = line[CH];
    CH++;
}
//*************************************************
void GetBC(char &ch) {					//��ȡ��һ���ǿ��ַ�
    ch = line[CH];
    CH++;
    while (ch == ' ' || ch == '\t') {
        ch = line[CH];
        CH++;
    }
}
//*************************************************
void Concat(char *strToken, char ch) {	//���ַ�������ʱ�ַ���ĩβ
    char *str = strToken;
    while (*strToken != '\0') {
        strToken++;
    }
    *strToken = ch;
    strToken = str;
}
//*************************************************
bool IsLetter(char ch) {					//�ж��ַ��Ƿ�����ĸ
    if (ch >= 'a'&&ch <= 'z') {
        return true;
    }
    else if (ch >= 'A'&&ch <= 'Z') {
        return true;
    }
    else
        return false;
}
//*************************************************
bool IsDigit(char ch) {					//�ж��ַ��Ƿ�������
    if (ch >= '0'&&ch <= '9') {
        return true;
    }
    else
        return false;
}
//*************************************************
void Reserve(char *strToken, word &lex) {	//�ж϶�������ʱ�ַ����Ƿ��Ǳ����ֻ��ʶ��
    if (strcmp(strToken, "program") == 0)
        lex.SYM = programS;
    else if (strcmp(strToken, "var") == 0)
        lex.SYM = varS;
    else if (strcmp(strToken, "procedure") == 0)
        lex.SYM = procedureS;
    else if (strcmp(strToken, "begin") == 0)
        lex.SYM = beginS;
    else if (strcmp(strToken, "end") == 0)
        lex.SYM = endS;
    else if (strcmp(strToken, "if") == 0)
        lex.SYM = ifS;
    else if (strcmp(strToken, "then") == 0)
        lex.SYM = thenS;
    else if (strcmp(strToken, "else") == 0)
        lex.SYM = elseS;
    else if (strcmp(strToken, "while") == 0)
        lex.SYM = whileS;
    else if (strcmp(strToken, "do") == 0)
        lex.SYM = doS;
    else if (strcmp(strToken, "call") == 0)
        lex.SYM = callS;
    else if (strcmp(strToken, "read") == 0)
        lex.SYM = readS;
    else if (strcmp(strToken, "write") == 0)
        lex.SYM = writeS;
    else if (strcmp(strToken, "odd") == 0)
        lex.SYM = oddS;
    else if (strcmp(strToken, "const") == 0)
        lex.SYM = constS;
    else {
        lex.SYM = ident;
        strcpy(lex.ID, strToken);//�û��Զ����ʶ��
    }
}
//*************************************************
void Retract(char &ch) {					//���ض��ַ�ǰ��λ��
    CH--;
    ch = NULL;
}
//*************************************************
void lexical() {
    char ch;
    int lineN = 0;
    char strToken[30] = { "" };	//��ʱ�ַ���
    FILE *fp2 = NULL;
    FILE *fp1 = NULL;
    fp1 = fopen(FILE_PATH, "r");	//��Դ�����ļ�
    if (fp1 == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        exit(0);
    }
    fp2 = fopen("lexical.txt", "wb");
    if (fp2 == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        exit(0);
    }
    while (!feof(fp1)) {			//ʶ��Դ�ļ��и������ʡ��ַ�������
        fgets(line, LINELEG, fp1);	//���ļ��ж���һ�д�������
        lineN++;
        tab.lineN = lineN;
        CH = 0;
        GetBC(ch);
        while (ch != '\0'&&ch != '\n') {
            if (IsLetter(ch)) {		//���ַ�����ĸ�������ȡ
                while (IsLetter(ch) || IsDigit(ch)) {
                    Concat(strToken, ch);
                    GetChar(ch);
                }
                Retract(ch);
                Reserve(strToken, tab);	//�жϵ����Ǳ����ֻ��Ǳ�ʶ��
            }
            else if (IsDigit(ch)) {	//���ַ��������������ȡ
                while (IsDigit(ch)) {
                    Concat(strToken, ch);
                    GetChar(ch);
                }
                Retract(ch);
                tab.SYM = number;
                tab.NUM = atoi(strToken);	//����ֵ�������
            }
            else if (ch == '=')		//�����ж��ַ�����
                tab.SYM = equal;//�Ⱥ�
            else if (ch == '+')
                tab.SYM = plus;//�Ӻ�
            else if (ch == '-')
                tab.SYM = minus;//����
            else if (ch == '*') {
                GetChar(ch);
                if (ch == '*')
                    tab.SYM = involution;//�˷���
                else {
                    Retract(ch);
                    tab.SYM = multiply;//�˺�
                }
            }
            else if (ch == '/') {
                tab.SYM = divide;//����
            }
            else if (ch == '>') {
                GetChar(ch);
                if (ch == '=')
                    tab.SYM = notless;//���ڵ��ں�
                else {
                    Retract(ch);
                    tab.SYM = more;//���ں�
                }
            }
            else if (ch == '<') {
                GetChar(ch);
                if (ch == '=')
                    tab.SYM = notmore;//С�ڵ��ں�
                else if (ch == '>')
                    tab.SYM = notequal;//�����ں�
                else {
                    Retract(ch);
                    tab.SYM = less;//С�ں�
                }
            }
            else if (ch == ':') {
                GetChar(ch);
                if (ch == '=') {
                    tab.SYM = assignment;//��ֵ��
                }
                else
                    Retract(ch);
            }
            else if (ch == ',')
                tab.SYM = comma;//����
            else if (ch == ';')
                tab.SYM = semicolon;//�ֺ�
            else if (ch == '(')
                tab.SYM = leftbrackets;//������
            else if (ch == ')')
                tab.SYM = rightbrackets;//������
            else {
                while (ch != '\0'&&ch != 10 && ch != 9) {
                    Concat(strToken, ch);
                    GetBC(ch);
                }
                Retract(ch);
                tab.SYM = undefine;//δ�����ַ�
                printf("\n���󣡵�%d�д���δ�����ַ�'%s'\n", lineN, strToken);
            }
            fwrite(&tab, sizeof(struct word), 1, fp2);
            memset(strToken, 0, sizeof(strToken));
            GetBC(ch);
        }
        memset(line, 0, LINELEG);
    }
    printf("\n�ʷ���������\n");
    fclose(fp1);
    fclose(fp2);
}
//*************************************************
void savelex() {							//����ʷ��������
    FILE *fp1 = NULL, *fp2 = NULL;
    char line[5];
    fp1 = fopen("lexical.txt", "rb");
    if (fp1 == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        exit;
    }
    fp2 = fopen("result.txt", "w");
    if (fp2 == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        exit;
    }
    fputs("����", fp2);
    fputs("\t\t", fp2);
    fputs("����", fp2);
    fputs("\t\t\t", fp2);
    fputs("�к�", fp2);
    fputs("\t\t\n", fp2);
    while (!feof(fp1)) {
        fread(&tab, sizeof(struct word), 1, fp1);
        switch (tab.SYM) {
            case ident:
                fputs(tab.ID, fp2);
                fputs("\t\t", fp2);
                fputs("ident", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case programS:
                fputs("program", fp2);
                fputs("\t\t", fp2);
                fputs("programS", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case varS:
                fputs("var", fp2);
                fputs("\t\t", fp2);
                fputs("varS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case procedureS:
                fputs("procedure", fp2);
                fputs("\t", fp2);
                fputs("procedureS", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case beginS:
                fputs("begin", fp2);
                fputs("\t\t", fp2);
                fputs("beginS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case endS:
                fputs("end", fp2);
                fputs("\t\t", fp2);
                fputs("endS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case ifS:
                fputs("if", fp2);
                fputs("\t\t", fp2);
                fputs("ifS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case thenS:
                fputs("then", fp2);
                fputs("\t\t", fp2);
                fputs("thenS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case elseS:
                fputs("else", fp2);
                fputs("\t\t", fp2);
                fputs("elseS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case whileS:
                fputs("while", fp2);
                fputs("\t\t", fp2);
                fputs("whileS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case doS:
                fputs("do", fp2);
                fputs("\t\t", fp2);
                fputs("doS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case callS:
                fputs("call", fp2);
                fputs("\t\t", fp2);
                fputs("callS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case readS:
                fputs("read", fp2);
                fputs("\t\t", fp2);
                fputs("readS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case writeS:
                fputs("write", fp2);
                fputs("\t\t", fp2);
                fputs("writeS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case oddS:
                fputs("odd", fp2);
                fputs("\t\t", fp2);
                fputs("oddS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case constS:
                fputs("const", fp2);
                fputs("\t\t", fp2);
                fputs("constS", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case number:
                char str[10];
                itoa(tab.NUM, str, 10);
                fputs(str, fp2);
                fputs("\t\t", fp2);
                fputs("number", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case equal:
                fputs("=", fp2);
                fputs("\t\t", fp2);
                fputs("equal", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case plus:
                fputs("+", fp2);
                fputs("\t\t", fp2);
                fputs("plus", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case minus:
                fputs("-", fp2);
                fputs("\t\t", fp2);
                fputs("minus", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case multiply:
                fputs("*", fp2);
                fputs("\t\t", fp2);
                fputs("multiply", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case involution:
                fputs("**", fp2);
                fputs("\t\t", fp2);
                fputs("involution", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case divide:
                fputs("/", fp2);
                fputs("\t\t", fp2);
                fputs("divide", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case more:
                fputs(">", fp2);
                fputs("\t\t", fp2);
                fputs("more", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case notless:
                fputs(">=", fp2);
                fputs("\t\t", fp2);
                fputs("notless", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case less:
                fputs("<", fp2);
                fputs("\t\t", fp2);
                fputs("less", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case notmore:
                fputs("<=", fp2);
                fputs("\t\t", fp2);
                fputs("notmore", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case notequal:
                fputs("<>", fp2);
                fputs("\t\t", fp2);
                fputs("notequal", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case assignment:
                fputs(":=", fp2);
                fputs("\t\t", fp2);
                fputs("assignment", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case comma:
                fputs(",", fp2);
                fputs("\t\t", fp2);
                fputs("comma", fp2);
                fputs("\t\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case semicolon:
                fputs(";", fp2);
                fputs("\t\t", fp2);
                fputs("semicolon", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case leftbrackets:
                fputs("(", fp2);
                fputs("\t\t", fp2);
                fputs("leftbrackets", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case rightbrackets:
                fputs(")", fp2);
                fputs("\t\t", fp2);
                fputs("rightbrackets", fp2);
                fputs("\t\t", fp2);
                itoa(tab.lineN, line, 10);
                fputs(line, fp2);
                fputs("\t\t\n", fp2);
                break;
            case undefine:
                break;
        }
    }
    fclose(fp1);
    fclose(fp2);
}
