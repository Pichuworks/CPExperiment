#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define TABLENUM 100
#define CODENUM 200
//**************************************************************************
enum symbol {							//�ַ����ͱ�ǩ
    ident, programS, varS, procedureS, beginS, endS, ifS, thenS, elseS,
    whileS, doS, callS, readS, writeS, oddS, constS, number, equal, plus, minus,
    multiply, involution, divide, more, notless, less, notmore, notequal,
    assignment, comma, semicolon, leftbrackets, rightbrackets, undefine
};
//*************************************************
struct word {							//���ʱ�
    enum symbol SYM;//��������
    char ID[15];	//��ʶ������
    int NUM;		//��������
    int lineN;		//�к�
}tab;
//*************************************************
typedef struct tablelink {				//��������ṹ��
    enum symbol SYM;//��������
    char ID[15];	//��ʶ������
    int NUM;		//��������
    int lineN;		//�к�
    struct tablelink *next;//��һ��Ԫ��ַ
}tabnode, *tabl;
//*************************************************
struct  table {							//���ű�
    enum symbol type;//��������
    int level;		//���ڲ��
    char levelname[15];//����
    char ID[15];	//����
    int num;		//����ֵ
    int adr; 		//���ƫ����
}ta[TABLENUM];
//*************************************************
void freelink(tabl &tb) {						//��������
    tabl q;
    while (tb != NULL) {
        q = tb;
        tb = tb->next;
        free(q);
    }
}
//**************************************************************************
enum codesym {							//�����������
    LIT, OPR, LOD, STO, CAL, RET, INT, JMP, JPC, RED, WRT
};
//*************************************************
struct code {							//�����
    enum codesym func;	//��������
    int L;				//���
    int a;				//��Ե�ַ
}cod[CODENUM];
//**************************************************************************
#define STACK_INIT_SIZE 1000	//Ĭ����󳤶�Ϊ100
#define STACKINCREMENT 10	//�ռ�����Ϊ10
#define SElemType int
struct SqStack								//����ջ�ṹ
{
    SElemType *base;//ջ��ָ��
    SElemType *top;	//ջ��ָ��
    int Stacksize;	//��ǰ����Ĵ洢����
}data;
//*************************************************
void InitStack(SqStack &S)					//����յ�ջ
{
    S.base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));//����ռ�
    S.top = S.base;
    S.Stacksize = STACK_INIT_SIZE;
}
//*************************************************
void DestroyStack(SqStack &S)				//����ջ
{
    S.top = S.base;
    S.Stacksize = 0;
    free(S.base);
    S.base = NULL;
    S.top = NULL;
}
//*************************************************
void StackPush(SqStack &S, SElemType e)		//�����µ�ջ��Ԫ��
{
    SElemType *newbase;
    if ((S.top - S.base) == STACK_INIT_SIZE)	//���ռ���������������ռ�
    {
        newbase = (SElemType *)realloc(S.base, (S.Stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!newbase)
            exit(0);
        S.base = newbase;
        S.top = S.base + STACK_INIT_SIZE;
        S.Stacksize += STACKINCREMENT;
    }
    *(S.top) = e;
    S.top++;
}
//*************************************************
void GetESem(SqStack S, SElemType &e)		//����ջ��Ԫ��
{
    if (S.top != S.base)
        e = *(S.top - 1);
    else
    {
        printf("ջΪ�գ�\n");
        exit(0);
    }
}

//���linux��itoa���������ڵ�����
void itoa(int value, char *string, int radix)
{
    if(radix != 10)
        exit(0);
    sprintf(string, "%d", value);
}

//**************************************************************************
void run(int cx, struct code *cod) {					//ִ�д���
    InitStack(data);
    int i, e, B = 0, T = 0, P;
    int a, b, c, Bs[100] = { 0 }, j = 0;
    int count, Bsum = 0;
    int jj = 0;
    for (i = 0; i<cx; i = P) {
        P = i + 1;
        switch (cod[i].func) {
            case LIT:
                StackPush(data, cod[i].a);
                T++;
                break;
            case OPR:
                switch (cod[i + 1].func) {	//��ִ����һ�����룬��ȡ������
                    case LIT:
                        StackPush(data, cod[i + 1].a);
                        T++;
                        break;
                    case LOD:
                        a = *(data.base + Bs[cod[i].L ? 0 : j] + 2 + cod[i + 1].a);
                        StackPush(data, a);
                        T++;
                        break;
                }
                GetESem(data, b);
                data.top--;
                T--;
                GetESem(data, a);
                data.top--;
                T--;
                switch (cod[i].a) {		//�жϲ�������
                    case 0:
                        break;
                    case 1:			//ȡ��
                        StackPush(data, b);
                        T++;
                        StackPush(data, -a);
                        T++;
                        break;
                    case 2:			//���
                        StackPush(data, b + a);
                        T++;
                        break;
                    case 3:			//���
                        StackPush(data, a - b);
                        T++;
                        break;
                    case 4:			//���
                        StackPush(data, b*a);
                        T++;
                        break;
                    case 5:			//���
                        StackPush(data, a / b);
                        T++;
                        break;
                    case 6:			//�ж���ż��
                        StackPush(data, b);
                        T++;
                        StackPush(data, a % 2);
                        T++;
                        break;
                    case 10:		//�����ж�
                        c = b - a;
                        if (!c) {
                            c = 1;
                        }
                        else {
                            c = 0;
                        }
                        StackPush(data, c);
                        T++;
                        break;
                    case 11:		//�������ж�
                        StackPush(data, b - a);
                        T++;
                        break;
                    case 12:		//С���ж�
                        c = b - a;
                        if (c>0) {
                            c = 1;
                        }
                        else {
                            c = 0;
                        }
                        StackPush(data, c);
                        T++;
                        break;
                    case 13:		//С�ڵ����ж�
                        c = b - a;
                        if (c >= 0) {
                            c = 1;
                        }
                        else {
                            c = 0;
                        }
                        StackPush(data, c);
                        T++;
                        break;
                    case 14:		//�����ж�
                        c = b - a;
                        if (c<0) {
                            c = 1;
                        }
                        else {
                            c = 0;
                        }
                        StackPush(data, c);
                        T++;
                        break;
                    case 15:		//���ڵ����ж�
                        c = b - a;
                        if (c <= 0) {
                            c = 1;
                        }
                        else {
                            c = 0;
                        }
                        StackPush(data, c);
                        T++;
                        break;
                }
                P = i + 2;	//������һ��ָ��
                break;
            case LOD:
                a = *(data.base + Bs[cod[i].L ? 0 : j] + 2 + cod[i].a);
                StackPush(data, a);
                T++;
                break;
            case STO:
                GetESem(data, e);
                data.top--;
                T--;
                *(data.base + Bs[cod[i].L ? 0 : j] + 2 + cod[i].a) = e;
                break;
            case CAL:
                P = cod[i].a;
                jj += cod[i].L;
                Bsum = 0;
                for (int k = 0; k <= j; k++)
                {
                    Bsum += *(data.base + Bs[k]);
                }
                count = T - Bsum;
                data.top -= count;
                T -= count;
                int temp[10];
                for (int k = 0; k < count; k++)
                {
                    temp[k] = *(data.top + k);
                }
                *(data.top) = Bs[j];
                *(data.top + 1) = B;
                B = T;
                j++;
                Bs[j] = B;

                i++;
                c = 3;

                while (c < 3 + count) {
                    *(data.top + c) = temp[c - 3];

                    c++;
                }
                *(data.top + 2) = i;
                break;
            case RET:
                data.top = data.base + B;
                T = B;
                P = *(data.top + 2);
                B = *(data.top + 1);
                j--;
                break;
            case INT:
                data.top += cod[i].a;

                *(data.base + T) = cod[i].a;
                T += cod[i].a;
                break;
            case JMP:
                P = cod[i].a;
                break;
            case JPC:
                GetESem(data, e);
                data.top--;
                T--;
                if (!e) {		//������������ʱ��ת
                    P = cod[i].a;
                }
                break;
            case RED:
                printf("����������: ");
                scanf("%d", data.base + B + 2 + cod[i].a);
                break;
            case WRT:
                GetESem(data, e);
                data.top--;
                T--;
                printf("%d\n", e);
                break;
        }
    }
    DestroyStack(data);
}
