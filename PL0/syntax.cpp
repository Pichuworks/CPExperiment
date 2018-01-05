#include "lexical.h"

int err = 0;		//��������
int lev = 1;		//����
char levname[5];//����
int tx = 0;		//�ַ����ڵ��ַ���
int dx[10] = { 0 }; 		//���ƫ����
int cx = 0;		//������ڵĴ�����

//****************************************************************
void getcode() {								//����ַ���ʹ����
    int i;
    char number[5];
    FILE *fp;
    fp = fopen("table.txt", "w");
    if (fp == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        exit;
    }
    fputs("\t����\t����\t      ֵ/�ռ�\t��\tƫ����\n", fp);
    for (i = 0; i<tx; i++) {
        itoa(i, number, 10);
        fputs(number, fp);
        fputs("\t", fp);
        fputs(ta[i].ID, fp);
        fputs("\t", fp);
        switch (ta[i].type) {
            case constS:
                fputs("const\t", fp);
                fputs("\t", fp);
                itoa(ta[i].num, number, 10);
                fputs(number, fp);
                break;
            case varS:
                fputs("var\t", fp);
                fputs("\t", fp);
                break;
            case procedureS:
                fputs("procedure", fp);
                fputs("\t", fp);
                itoa(ta[i].num, number, 10);
                fputs(number, fp);
                break;
        }
        fputs("\t", fp);
        itoa(ta[i].level, number, 10);
        fputs(number, fp);
        fputs("\t", fp);
        itoa(ta[i].adr, number, 10);
        fputs(number, fp);
        fputs("\n", fp);
    }
    fclose(fp);
    fp = fopen("code.txt", "w");
    if (fp == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        exit;
    }
    fputs("\t������\t���\tλ����\n", fp);
    for (i = 0; i<cx; i++) {
        itoa(i, number, 10);
        fputs(number, fp);
        fputs("\t", fp);
        switch (cod[i].func) {
            case LIT:
                fputs("LIT", fp);
                break;
            case OPR:
                fputs("OPR", fp);
                break;
            case LOD:
                fputs("LOD", fp);
                break;
            case STO:
                fputs("STO", fp);
                break;
            case CAL:
                fputs("CAL", fp);
                break;
            case RET:
                fputs("RET", fp);
                break;
            case INT:
                fputs("INT", fp);
                break;
            case JMP:
                fputs("JMP", fp);
                break;
            case JPC:
                fputs("JPC", fp);
                break;
            case RED:
                fputs("RED", fp);
                break;
            case WRT:
                fputs("WRT", fp);
                break;
        }
        fputs("\t", fp);
        itoa(cod[i].L, number, 10);
        fputs(number, fp);
        fputs("\t", fp);
        itoa(cod[i].a, number, 10);
        fputs(number, fp);
        fputs("\n", fp);
    }
    fclose(fp);
}
//****************************************************************
void gen(enum codesym func, int L, int a) {		//���ɴ����������
    if (cx >= CODENUM) {
        printf("����������������!\n");
        exit;
    }
    cod[cx].func = func;
    cod[cx].L = L;
    cod[cx].a = a;
    cx++;
}
//****************************************************************
int position(char *ID) {							//���ַ����в��ҵ�ǰ�ַ�
    for (int i = tx - 1; i >= 0; i--) {
        if (!(strcmp(ta[i].ID, ID))) {
            if (!(strcmp(ta[i].levelname, levname)) || ta[i].level<lev) {	//�жϲ㼶��ϵ�Ƿ���ȷ
                return i;
            }
        }
    }
    return -1;
}
//****************************************************************
void enter(tabl now, enum symbol tp) {			//���ַ������ַ���
    for (int i = 0; i<tx; i++) {		//�жϱ����Ƿ��Ѷ�����ñ���
        if (!(strcmp(ta[i].ID, now->ID)) && ta[i].level == lev && !(strcmp(ta[i].levelname, levname))) {
            printf(" [����] ��%d��	�����Ѷ���\n", now->lineN);
            err++;
            return;
        }
    }
    ta[tx].type = tp;
    ta[tx].level = lev;
    strcpy(ta[tx].levelname, levname);
    strcpy(ta[tx].ID, now->ID);
    ta[tx].num = now->NUM;
    ta[tx].adr = dx[lev];
    tx++;
}
//****************************************************************
void factor_syn(tabl &now) {						//factor����
    void exp_syn(tabl &now);
    int t;
    if (now->SYM != ident&&now->SYM != number&&now->SYM != leftbrackets) {
        printf("[����]��%d��	ȱ������\n", now->lineN);
        err++;
    }
    else if (now->SYM == leftbrackets) {
        if (now->next == NULL) {
            printf("[����]ȱ������\n");
            err++;
            return;
        }
        now = now->next;
        exp_syn(now);
        if (now->SYM != rightbrackets) {
            printf("[����]��%d��	ȱ��������\n", now->lineN);
            err++;
        }
        else {
            if (now->next == NULL) {
                return;
            }
            now = now->next;
        }
    }
    else {
        if (now->SYM == ident) {
            t = position(now->ID);	//�жϱ����Ƿ��ڷ��ű���
            if (t<0) {
                printf("[����]��%d��	����δ����\n", now->lineN);
                err++;
            }
            else {
                switch (ta[t].type) {
                    case constS:
                        gen(LIT, 0, ta[t].num);
                        break;
                    case varS:
                        gen(LOD, lev - ta[t].level, ta[t].adr);
                        break;
                    case procedureS:
                        printf("[����]��%d��	�������ó���\n", now->lineN);
                        err++;
                        break;
                }
            }
        }
        else if (now->SYM == number) {
            gen(LIT, 0, now->NUM);
        }
        if (now->next == NULL) {
            return;
        }
        now = now->next;
    }
}
//****************************************************************
void term_syn(tabl &now) {						//term����
    factor_syn(now);
    while (now->SYM == multiply || now->SYM == divide) {
        if (now->next == NULL) {
            printf("[ ����]ȱ������\n");
            err++;
            return;
        }
        if (now->SYM == multiply) {
            gen(OPR, 0, 4);
        }
        else {
            gen(OPR, 0, 5);
        }
        now = now->next;
        factor_syn(now);
    }
}
//****************************************************************
void exp_syn(tabl &now) {						//exp����
    if (now->SYM == plus || now->SYM == minus) {
        if (now->next == NULL) {
            printf("[���� ]ȱ������\n");
            err++;
            return;
        }
        if (now->SYM == minus) {
            gen(OPR, 0, 1);
        }
        now = now->next;
    }
    term_syn(now);
    while (now->SYM == plus || now->SYM == minus) {
        if (now->next == NULL) {
            printf("[���� ]ȱ������\n");
            err++;
            return;
        }
        if (now->SYM == plus) {
            gen(OPR, 0, 2);
        }
        else {
            gen(OPR, 0, 3);
        }
        now = now->next;
        term_syn(now);
    }
}
//****************************************************************
void lexp_syn(tabl &now) {						//lexp����
    exp_syn(now);
    if (now->SYM != equal&&now->SYM != notequal&&now->SYM != less&&now->SYM != notmore&&now->SYM != more&&now->SYM != notless) {
        printf("[����]��%d��	ȱ�������\n", now->lineN);
        err++;
        return;
    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ������\n");
            err++;
            return;
        }
        switch (now->SYM) {
            case equal:
                gen(OPR, 0, 10);
                break;
            case notequal:
                gen(OPR, 0, 11);
                break;
            case less:
                gen(OPR, 0, 12);
                break;
            case notmore:
                gen(OPR, 0, 13);
                break;
            case more:
                gen(OPR, 0, 14);
                break;
            case notless:
                gen(OPR, 0, 15);
                break;
        }
        now = now->next;
    }
    if (now->SYM == oddS) {
        if (now->next == NULL) {
            printf("[����]ȱ������\n");
            err++;
            return;
        }
        gen(OPR, 0, 6);
        now = now->next;
    }
    exp_syn(now);
}
//****************************************************************
void callexp_syn(tabl &now) {					//call�е�expѭ��
    exp_syn(now);
    if (now->SYM == comma) {
        if (now->next == NULL) {
            printf("[����]ȱ������\n");
            err++;
            return;
        }
        now = now->next;
        callexp_syn(now);
    }
}
//****************************************************************
void statement_syn(tabl &now) {					//statement����
    void id_syn(tabl &now);
    void body_syn(tabl &now);
    int j, t;
    int tcall;
    int cx0, cx1, cx2, cx3;
    tabl s, q;
    if (now->next == NULL) {
        printf("[����]ȱ�ٳ�����\n");
        err++;
        return;
    }
    for (j = 0, s = now; s->next != NULL; s = s->next) {
        if (s->SYM == ident)
        {
            j = 1;
            t = position(now->ID);	//�жϱ����Ƿ��ڷ��ű���
            if (t<0) {
                printf("[����]��%d��	����δ����\n", now->lineN);
                err++;
            }
            else if (ta[t].type != varS) {
                printf("[����]��%d��	�������ó���\n", now->lineN);
                err++;
            }
            break;
        }
        else if (s->SYM == ifS) {
            j = 2;
            break;
        }
        else if (s->SYM == whileS) {
            j = 3;
            break;
        }
        else if (s->SYM == callS) {
            j = 4;
            break;
        }
        else if (s->SYM == beginS) {
            j = 5;
            break;
        }
        else if (s->SYM == readS) {
            j = 6;
            break;
        }
        else if (s->SYM == writeS) {
            j = 7;
            break;
        }
    }
    if (now != s) {
        printf("[����]��%d��	�ж�������\n", now->lineN);
        err++;
        for (; now->next != s; now = now->next) {
            q = now->next;
            printf("[����]��%d��	�ж�������\n", q->lineN);
            err++;
        }
        now = s;
    }
    if (now->next == NULL) {
        printf("[����]ȱ�ٳ�����\n");
        err++;
        return;
    }
    now = now->next;
    switch (j) {
        case 1:	//<id> := <exp>
            if (now->SYM != assignment) {//�ж��Ƿ��и�ֵ��
                printf("[����]��%d��	ȱ�ٸ�ֵ��\n", now->lineN);
                err++;
            }
            else {
                if (now->next == NULL) {
                    printf("[����]ȱ�ٳ�����\n");
                    err++;
                    return;
                }
                if (lev == ta[t].level)
                {
                    ta[t].num = 0;
                }
                now = now->next;
            }
            exp_syn(now);
            gen(STO, lev - ta[t].level, ta[t].adr);
            break;
        case 2:	//if���
            lexp_syn(now);
            if (now->SYM != thenS) {//�ж��Ƿ��и�ֵ��
                printf("[����]��%d��	ȱ��then\n", now->lineN);
                err++;
            }
            else {
                if (now->next == NULL) {
                    printf("[����]ȱ������\n");
                    err++;
                    return;
                }
                cx0 = cx;
                gen(JPC, 0, 0);
                now = now->next;
            }
            statement_syn(now);
            cx1 = cx;
            gen(JMP, 0, 0);
            cod[cx0].a = cx;
            if (now->SYM == elseS) {
                if (now->next == NULL) {
                    printf("[����]��%d��	else����\n", now->lineN);
                    err++;
                    return;
                }
                now = now->next;
                statement_syn(now);
            }
            cod[cx1].a = cx;
            break;
        case 3:	//while���
            cx1 = cx;
            lexp_syn(now);
            if (now->SYM != doS) {//�ж��Ƿ��и�ֵ��
                printf("[����]��%d��	ȱ��do\n", now->lineN);
                err++;
            }
            else {
                cx2 = cx;
                gen(JPC, 0, 0);
                if (now->next == NULL) {
                    printf("[����]ȱ������\n");
                    err++;
                    return;
                }
                now = now->next;
            }
            statement_syn(now);
            gen(JMP, 0, cx1);
            cod[cx2].a = cx;
            break;
        case 4:	//call���
            if (now->SYM != ident) {
                printf("[����]��%d��	ȱ�ٱ�����\n", now->lineN);
                err++;
            }
            else {
                t = position(now->ID);		//�жϱ����Ƿ��ڷ��ű���
                if (t<0) {
                    printf("[����]��%d��	����δ����\n", now->lineN);
                    err++;
                }
                else if (ta[t].type != procedureS) {
                    printf("[����]��%d��	�������ó���\n", now->lineN);
                    err++;
                }
                else {
                    tcall = t;
                    //gen(CAL, lev - ta[t].level, ta[t].adr);
                    //cod[ta[t].num].a=cx;
                }
                if (now->next == NULL) {
                    printf("[����]ȱ������\n");
                    err++;
                    return;
                }
                now = now->next;
            }
            if (now->SYM != leftbrackets) {
                printf("[����]��%d��	ȱ��������\n", now->lineN);
                err++;
            }
            else {
                if (now->next == NULL) {
                    printf("[����]ȱ������\n");
                    err++;
                    return;
                }
                now = now->next;
            }
            if (now->SYM != rightbrackets)
                callexp_syn(now);
            if (now->SYM != rightbrackets) {
                printf("[����]��%d��	ȱ��������\n", now->lineN);
                err++;
            }
            else {
                if (now->next == NULL) {
                    return;
                }
                now = now->next;
            }
            gen(CAL, lev - ta[tcall].level, ta[tcall].adr);
            break;
        case 5:	//<body>
            now = s;
            body_syn(now);
            break;
        case 6:	//read���
            if (now->SYM != leftbrackets) {
                printf("[����]��%d��	ȱ��������\n", now->lineN);
                err++;
            }
            else {
                if (now->next == NULL) {
                    printf("[����]ȱ������\n");
                    err++;
                    return;
                }
                now = now->next;
            }
            while (now->SYM != rightbrackets)
            {
                if (now->SYM != ident) {//�ж��Ƿ���ident
                    printf("[����]��%d��	ȱ��������\n", now->lineN);
                    err++;
                }
                else {
                    t = position(now->ID);	//�жϱ����Ƿ��ڷ��ű���
                    if (t<0) {
                        printf("[����]��%d��	����δ����\n", now->lineN);
                        err++;
                    }
                    else if (ta[t].type != varS) {
                        printf("[����]��%d��	�������ó���\n", now->lineN);
                        err++;
                    }
                    else {
                        ta[t].num = 0;
                        gen(RED, lev - ta[t].level, ta[t].adr);	//���ɴ���
                    }
                    if (now->next == NULL) {
                        printf("[����]ȱ������\n");
                        err++;
                        return;
                    }
                    now = now->next;
                }
                if (now->SYM != comma) {
                    break;
                }
                else {
                    if (now->next == NULL) {
                        printf("[����]ȱ������\n");
                        err++;
                        return;
                    }
                    now = now->next;
                }
            }
            if (now->SYM != rightbrackets) {
                printf("[����]��%d��	ȱ��������\n", now->lineN);
                err++;
            }
            else {
                if (now->next == NULL) {
                    return;
                }
                now = now->next;
            }
            break;
        case 7:	//write���
            if (now->SYM != leftbrackets) {
                printf("[����]��%d��	ȱ��������\n", now->lineN);
                err++;
            }
            else {
                if (now->next == NULL) {
                    printf("[����]ȱ������\n");
                    err++;
                    return;
                }
                now = now->next;
            }
            while (now->SYM != rightbrackets)
            {
                exp_syn(now);
                gen(WRT, 0, 0);
                if (now->SYM != comma) {
                    break;
                }
                else {
                    if (now->next == NULL) {
                        printf("[����]ȱ������\n");
                        err++;
                        return;
                    }
                    now = now->next;
                }
            }
            if (now->SYM != rightbrackets) {
                printf("[����]��%d��	ȱ��������\n", now->lineN);
                err++;
            }
            else {
                if (now->next == NULL) {
                    return;
                }
                now = now->next;
            }
    }
    if (now->SYM == endS) {
        return;
    }
    else if (now->SYM != semicolon) {//�ж��Ƿ��зֺ�
        if (now->next != NULL&&now->SYM != beginS) {
            printf("[����]��%d��	ȱ�ٷֺ�\n", now->lineN);
            err++;
        }
        else
            return;
    }
    else {
        if (now->next == NULL) {
            return;
        }
        now = now->next;
    }
    statement_syn(now);
}
//****************************************************************
void body_syn(tabl &now) {						//body����
    if (now->SYM == beginS) {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    else {
        printf("[����]��%d��	ȱ��begin\n", now->lineN);
        err++;
    }
    statement_syn(now);
    if (now->SYM != endS) {
        printf("[����]��%d��	ȱ��end\n", now->lineN);
        err++;
    }
    else {
        if (now->next == NULL) {
            return;
        }
        now = now->next;
    }
}
//****************************************************************
void id_syn(tabl &now) {						//procedure����ѭ��
    int Noid, Noco = 0;
    if (now->SYM != ident) {//�ж��Ƿ���ident
        Noid = 1;
    }
    else {
        dx[lev]++;
        enter(now, varS);
        Noid = 0;
    }
    if (now->next == NULL) {
        printf("[����]ȱ�ٳ�����\n");
        err++;
        return;
    }
    now = now->next;
    if (now->SYM == comma) {//�ж��Ƿ��Ƕ���
        if (Noid) {
            printf("[����]��%d��	ȱ�ٲ���\n", now->lineN);
            err++;
        }
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
        id_syn(now);
        return;
    }
    else if (now->SYM == ident) {//�ж��Ƿ���ident
        printf("[����]��%d��	ȱ�ٶ���\n", now->lineN);
        err++;
        id_syn(now);
        return;
    }
}
//****************************************************************
void proc_syn(tabl &now) {					//procedure����
    void block_syn(tabl &);
    int tx0;
    char name[5];
    if (now->SYM != ident) {//�ж��Ƿ���ident
        printf("[����]��%d��	ȱ�ٳ�����\n", now->lineN);
        err++;
    }
    else {
        tx0 = tx;
        enter(now, procedureS);
        strcpy(name, levname);
        strcpy(levname, now->ID);
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    if (now->SYM != leftbrackets) {//�ж��Ƿ���������
        printf("[����]��%d��	ȱ��������\n", now->lineN);
        err++;
    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    lev++;
    dx[lev] = 0;

    id_syn(now);//�����������
    if (now->SYM != rightbrackets) {//�ж��Ƿ���������
        printf("[����]��%d��	ȱ��������\n", now->lineN);
        err++;
    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    if (now->SYM != semicolon) {//�ж��Ƿ��зֺ�
        printf("[����]��%d��	ȱ�ٷֺ�\n", now->lineN);
        err++;
    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    ta[tx0].adr = cx;
    block_syn(now);
    strcpy(levname, name);
    ta[tx0].num = cx;

    gen(RET, 0, 0);
    lev--;
    if (now->SYM != semicolon) {//�ж��Ƿ��зֺ�
        return;
    }
    else {
        if (now->next == NULL || now->next->next == NULL) {
            printf("[����]��%d��	�ж�������\n", now->lineN);
            err++;
            return;
        }
        now = now->next;
        if (now->SYM == procedureS) {
            now = now->next;
            proc_syn(now);
        }
        else {
            printf("[����]��%d��	�ж�������\n", now->lineN);
            err++;
            now = now->next;
        }
    }
}
//****************************************************************
void var_syn(tabl &now) {						//var����
    void blockbegin_syn(tabl &);
    if (now->SYM != ident) {//�ж��Ƿ���ident
        printf("[����]��%d��	ȱ�ٱ�����\n", now->lineN);
        err++;
    }
    else {
        dx[lev]++;
        enter(now, varS);
        ta[tx - 1].num = 0xffffffff;
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    if (now->SYM == comma) {//�ж��Ƿ��ж���
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
        var_syn(now);
        return;
    }
    else if (now->SYM != semicolon) {//�ж��Ƿ��зֺ�
        printf("[����]��%d��	ȱ�ٷֺ�\n", now->lineN);
        err++;
    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    blockbegin_syn(now);
}
//****************************************************************
void const_syn(tabl &now) {						//const����
    void blockbegin_syn(tabl &);
    if (now->SYM != ident) {//�ж��Ƿ���ident
        printf("[����]��%d��	ȱ�ٳ�����\n", now->lineN);
        err++;
    }
    else {
        enter(now, constS);
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    if (now->SYM != assignment) {//�ж��Ƿ��и�ֵ��
        printf("[����]��%d��	ȱ�ٸ�ֵ��\n", now->lineN);
        err++;
    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    if (now->SYM != number) {//�ж��Ƿ���number
        printf("[����]��%d��	ȱ�ٳ���\n", now->lineN);
        err++;
    }
    else {
        ta[tx - 1].num = now->NUM;
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    if (now->SYM == comma) {//�ж��Ƿ��ж���
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
        const_syn(now);
        return;
    }
    else if (now->SYM != semicolon) {//�ж��Ƿ��зֺ�
        printf("[����]��%d��	ȱ�ٷֺ�\n", now->lineN);
        err++;
    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    blockbegin_syn(now);
}
//****************************************************************
void blockbegin_syn(tabl &now) {					//blockǰ����ѭ��
    int j;
    tabl s, q;
    if (now->next == NULL) {
        printf("[����]ȱ�ٳ�����\n");
        err++;
        return;
    }
    for (j = 0, s = now; s->next != NULL; s = s->next) {
        if (s->SYM == constS)
        {
            j = 1;
            break;
        }
        else if (s->SYM == varS) {
            j = 2;
            break;
        }
        else if (s->SYM == procedureS) {
            j = 3;
            break;
        }
        else if (s->SYM == beginS) {
            return;
        }
    }
    if (now != s) {
        q = now->next;
        if (now->SYM == ident) {
            if (q->SYM == assignment) {
                if (q->next->SYM == number) {
                    printf("[����]��%d��	ȱ��const\n", now->lineN);
                    err++;
                    j = 1;
                }
                else if (q->next->SYM == ident || q->next->SYM == plus || q->next->SYM == minus) {
                    body_syn(now);
                    return;
                }
            }
            else if (q->SYM == comma || q->next->SYM == number) {
                printf("[����]��%d��	ȱ��var\n", now->lineN);
                err++;
                j = 2;
            }
            else if (q->SYM == leftbrackets) {
                printf("[����]��%d��	ȱ��procedure\n", now->lineN);
                err++;
                j = 3;
            }
        }
        else if (now->SYM == ifS || now->SYM == whileS || now->SYM == callS || now->SYM == readS || now->SYM == writeS) {
            body_syn(now);
            return;
        }
        else {
            printf("[����]��%d��	�ж�������\n", now->lineN);
            err++;
            for (; now->next != s; now = now->next) {
                q = now->next;
                printf("[����]��%d��	�ж�������\n", q->lineN);
                err++;
            }
            if (s->next == NULL) {
                printf("[����]ȱ�ٳ�����\n");
                err++;
                return;
            }
            now = s->next;
        }
    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    switch (j) {
        case 1:	//const����
            const_syn(now);
            break;
        case 2:	//var����
            var_syn(now);
            break;
        case 3:	//procedure����
            proc_syn(now);
            break;
    }
}
//****************************************************************
void block_syn(tabl &now) {						//block����
    int cx0;
    cx0 = cx;
    gen(JMP, 0, 0);
    blockbegin_syn(now);
    cod[cx0].a = cx;
    gen(INT, 0, dx[lev] + 3);
    if (now->next != NULL) {
        body_syn(now);	//ת��body����
    }
}
//****************************************************************
void prog_syn(tabl &now) {						//prog�﷨����
    tabl s, p = now;
    if (now == NULL) {
        return;
    }
    for (s = now; s->next != NULL; s = s->next) {
        if (s->SYM == programS)
            break;
    }if (s->SYM != programS) {	//�ж��Ƿ��С�program��
        printf("[����]ȱ��program\n");
        err++;
    }
    else if (now != s) {
        printf("[����]��%d��	���ڳ�������\n", now->lineN);
        err++;
        for (tabl q; now->next != s; now = now->next) {
            q = now->next;
            if (q->lineN != now->lineN)
                printf("[����]��%d��	���ڳ�������\n", q->lineN);
            err++;
        }
        now = s;
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;

    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        now = now->next;
    }
    if (now->SYM != ident) {	//�ж��Ƿ���ident
        printf("[����]��%d��	ȱ�ٳ�����\n", now->lineN);
        err++;
    }
    else {
        if (now->next == NULL) {
            printf("[����]ȱ�ٳ�����\n");
            err++;
            return;
        }
        strcpy(levname, now->ID);
        now = now->next;
    }
    for (s = now; s->next != NULL&&s->SYM != semicolon; s = s->next) {	//�ж��Ƿ��зֺ�
        if (s->SYM == constS || s->SYM == varS || s->SYM == procedureS || s->SYM == beginS) {
            if (now != s) {
                printf("[����]��%d��	�ж�������\n", now->lineN);
                err++;
                for (tabl q; now->next != s; now = now->next) {
                    q = now->next;
                    printf("[����]��%d��	�ж�������\n", q->lineN);
                    err++;
                }
            }
            for (now = p; now->next != s; now = now->next);
            printf("[����]��%d��	ȱ�ٷֺ�\n", now->lineN);
            err++;
            break;
        }
        else {
            printf("[����]��%d��	�ж�������\n", s->lineN);
            err++;
        }
    }
    if (now->next == NULL) {
        printf("[����]ȱ�ٳ�����\n");
        err++;
        return;
    }
    now = now->next;
    block_syn(now);//ת��block����
    if (now->next != NULL || now->SYM != endS) {
        printf("[����]��%d���ж�������\n", now->lineN);
        err++;
        for (tabl s = now->next; s != NULL; s = s->next) {
            printf("[����]��%d���ж�������\n", s->lineN);
            err++;
        }
        return;
    }
    for (int k = 0; k < tx; k++)
    {
        if (ta[k].type == varS)
        {
            if (ta[k].num == 0xffffffff)
            {
                printf("[����]����%sδ����ֵ\n", ta[k].ID);
            }
        }
    }
}
//****************************************************************
int main() {
    lexical();		//��ʼ�ʷ�����
    tabl tb, p, s;
    tb = (tabl)malloc(sizeof(tabnode));//��ʼ������
    if (!tb)
    {
        printf("�ռ�����ʧ��\n");
        exit(0);
    }
    tb->next = NULL;
    FILE *fp = NULL;
    savelex();
    fp = fopen("lexical.txt", "rb");
    if (fp == NULL) {
        printf("�ļ���ʧ�ܣ�\n");
        return 0;
    }
    p = tb;
    while (!feof(fp)) {			//������������
        s = (tabl)malloc(sizeof(tabnode));
        if (!s)
        {
            printf("�ռ�����ʧ��\n");
            exit(0);
        }
        fread(&tab, sizeof(struct word), 1, fp);
        if (feof(fp))
        {
            free(s);
            break;
        }
        s->SYM = tab.SYM;
        strcpy(s->ID, tab.ID);
        s->NUM = tab.NUM;
        s->lineN = tab.lineN;
        s->next = NULL;
        p->next = s;
        p = s;
    }
    fclose(fp);
    p = tb->next;
    prog_syn(p);//��ʼ�﷨����
    printf("\n�﷨��������\n");
    printf("\n %d ����\n\n", err);
    if (!err) {
        getcode();
        run(cx, cod);
    }
    freelink(tb);

    return 0;
}
