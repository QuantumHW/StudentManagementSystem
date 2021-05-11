#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include <time.h>
#include <graphics.h>

#define version 2.0
#define nameNum 10

typedef struct Students {
	char name[nameNum];
	char sex[10];
    char ID[40];
	int Chinese, Math, English;
	char password[20];
}STU;
typedef struct Node
{
	STU students;
	struct Node* next;
}Node;

Node* head = NULL;
Node* ptr = NULL;

int stucount = -1;
char power = '0';//Ȩ�޿���
char STUID[40] = {"\0"};//ѧ�����ʶ��
float ChineseAverage=0.0;//����ƽ����
float MathAverage=0.0;//��ѧƽ����
float EnglishAverage=0.0;//Ӣ��ƽ����

void WelcomeInterface();
void LoginInterface();
void TeacherLogin();
void StudentsLogin();
void TeachersMenu();
void StudentsMenu();
void InputInformation();
void DeleteInformation();
void ModifyInformation();
void FindInformation();
void ProcessingData();
void PrintResults();
void ExportAsWebPage();
void Rank();
void AverageScore();
void AnalysisResults();
void RankChinese(char* way);
void RankMath(char* way);
void RankEnglish(char* way);
void RankTotalScore(char* way);
int ClassificationByGrade(int min, int max, int subject);
void SetTEAPassword();
void ModifySTUPassword();
void ResetSTUPassword();
void WriteToFile();
void ReadFromFile();
void wcharTochar(const wchar_t* wchar, char* chr, int length);
void charTowchar(const char* chr, wchar_t* wchar, int size);

int main() {
	WelcomeInterface();
	LoginInterface();
	IMAGE img;
	initgraph(1200, 700);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	loadimage(&img, _T("PNG"), _T("�˳�����"), 1200, 700);
	putimage(0, 0, &img);
	system("Pause");
	return 0;
}

void WelcomeInterface() {//��ӭ����
	IMAGE img;
	initgraph(1200, 700);//����
	loadimage(&img, _T("PNG"),_T("��ӭ����"),1200,700);//����ͼƬ���ߣ�������ԭͼ��С
	putimage(0, 0, &img);//ͼƬ��ʾ��λ��
	HWND hwnd;// ���ô��ھ���������洰����Ϣ
	hwnd = GetHWnd();// ��ȡ���ھ��
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	Sleep(2000);
	closegraph();
}

void LoginInterface() {//��¼����
	IMAGE img;
	initgraph(1200, 700);
	HWND hwnd;// ���ô��ھ���������洰����Ϣ
	hwnd = GetHWnd();// ��ȡ���ھ��
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	loadimage(&img,_T("PNG"),_T("��¼��ʽ"), 1200, 700);
	putimage(0, 0, &img);
	//������¼�����
	MOUSEMSG msg;// ������������������Ϣ
	FlushMouseMsgBuffer();// ��������Ϣ������	
	while (1) { // ��ѭ�� ѭ�����������Ϣ
		while (MouseHit())// ���������Ϣ ���������Ϣ��ʱ��ִ�� �ɼ�������������Ϣ
		{
			msg = GetMouseMsg();// ��ȡ�����Ϣ
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x>=473 && msg.x<=730 && msg.y>=271 && msg.y<=340) {
				//��ʦ��¼
				TeacherLogin();
				goto label;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 473 && msg.x <= 730 && msg.y >= 379 && msg.y <= 449) {
				//ѧ����¼
				StudentsLogin();
				goto label;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 473 && msg.x <= 730 && msg.y >= 480 && msg.y <= 552) {
				//�˳�
				closegraph();
				goto label;
			}
		}
	}
label:return;
}

void TeacherLogin() {//��ʦ��¼
	ReadFromFile();
	AverageScore();
	HWND hwnd;
	hwnd = GetHWnd();
	FILE* fp;
	char accountTeachers[20];
	char passwordTeachers[20];
labelTEA:	fp = fopen("accountTeachers.txt", "r");
	if (fp == NULL) {
		int i;
		i = MessageBox(hwnd, L"������Ҫ����һ���µ��˻���", _T("����û�����κ��˻�"), MB_OKCANCEL | MB_ICONEXCLAMATION);
		if (i == true) {
			SetTEAPassword();
			goto labelTEA;
		}
		else {
			LoginInterface();
			goto labelTeacherLogin;
		}
	}
	else {
		fscanf(fp,"%s",accountTeachers);
		fscanf(fp, "%s", passwordTeachers);
		fclose(fp);
		IMAGE img;
		initgraph(1200, 700, NOCLOSE);
		loadimage(&img, _T("PNG"), _T("��ʦ��¼"), 1200, 700);
		putimage(0, 0, &img);
		hwnd = GetHWnd();
		SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	}
	wchar_t Tempaccount[20];
	wchar_t Temppassword[20];
	char account[20];
	char password[20];
	int i;
labelaccount:	i = InputBox(Tempaccount, 20, _T("�������˺�"), _T("��ʦ��¼"), NULL, 0, 0, false);
	if (i == true) {
		wcharTochar(Tempaccount,account , 20);
		InputBox(Temppassword, 20, _T("����������"), _T("��ʦ��¼"), NULL, 0, 0, true);
		wcharTochar(Temppassword, password, 20);
			if (strcmp(password, passwordTeachers) == 0 && strcmp(account, accountTeachers) == 0) {
				closegraph();
				power = 'T';
				TeachersMenu();
				goto labelTeacherLogin;
			}
			else {
				MessageBox(hwnd, L"����������˺Ż������������������룡", _T("�˺��������"), MB_OK | MB_ICONHAND);
				goto labelaccount;
			}
	}
	else if (i == false) {
		closegraph();
		LoginInterface();
		goto labelTeacherLogin;
	}
labelTeacherLogin:
	return;
}

void StudentsLogin() {//ѧ����¼
	ReadFromFile();
	AverageScore();
	int _bool = 0;
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("ѧ����¼"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	wchar_t Tempaccount[40];
	wchar_t Temppassword[40];
	char account[20];
	char password[20];
	char InputAccount[20];
	char InputPassword[20];
	int i;
labelaccount:	
	Node* ptrTemp = head;
	i = InputBox(Tempaccount, 20, _T("�������˺�"), _T("ѧ����¼"), NULL, 0, 0, false);
	if (i == true) {
		InputBox(Temppassword, 20, _T("����������"), _T("ѧ����¼"), NULL, 0, 0, true);
		wcharTochar(Tempaccount, account, 20);
		wcharTochar(Temppassword, password, 20);
		while (ptrTemp != NULL) {
			if (strcmp(account, ptrTemp->students.ID) == 0) {
				if (strcmp(password, ptrTemp->students.password) == 0) {
					strcat(STUID, ptrTemp->students.ID);
					_bool = 1;
					break;
				}
			}
			ptrTemp = ptrTemp->next;
		}
		if (_bool == 1) {
			_bool = 0;
			closegraph();
			power = 'S';
			StudentsMenu();
			goto labelStudentsLogin;
		}
		else if(_bool==0){
			hwnd = GetHWnd();
			MessageBox(hwnd, L"�ټ����������˺Ż������Ƿ���ȷ\n��ȷ����ʦ�Ѿ�¼�������Ϣ", _T("�˺Ż���������"), MB_OK | MB_ICONHAND);
			goto labelaccount;
		}
	}
	else if (i == false) {
		closegraph();
		LoginInterface();
	}
	
labelStudentsLogin:
	return;
}

void TeachersMenu() {//��ʦ���˵�
	IMAGE img;
	initgraph(1200, 700);//����
	loadimage(&img, _T("PNG"), _T("��ʦ���˵�"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	labelStart:
	MOUSEMSG msg;// ������������������Ϣ
	FlushMouseMsgBuffer();// ��������Ϣ������
	while (1) { 
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 220 && msg.y <= 259) {
				//¼��ѧ����Ϣ
				closegraph();
				InputInformation();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 287 && msg.y <= 324) {
				//ɾ��ѧ����Ϣ
				closegraph();
				DeleteInformation();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 354 && msg.y <= 394) {
				//�޸�ѧ����Ϣ
				closegraph();
				ModifyInformation();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 423 && msg.y <= 462) {
				//����ѧ����Ϣ
				closegraph();
				FindInformation();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 492 && msg.y <= 530) {
				//����ѧ����Ϣ
				closegraph();
				ProcessingData();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 1091 && msg.x <= 1192 && msg.y >= 632 && msg.y <= 652) {
				//�޸�����
				FILE* fp;
				char accountTeachers[20];
				char passwordTeachers[20];
				fp = fopen("accountTeachers.txt", "r");
				fscanf(fp, "%s", accountTeachers);
				fscanf(fp, "%s", passwordTeachers);
				fclose(fp);
				wchar_t TempID[20], TempPassword[20];
				char InputID[20], InputPassword[20];
				int j;
				j = InputBox(TempID, 20, _T("�����������˺ż����룬ȷ��Ϊ���˲���\n�������������˺š�"), _T("�����֤"), NULL, 0, 0, false);
				if (j == true) {
					int m;
					m = InputBox(TempPassword, 20, _T("�����������ľ����롿"), _T("�����֤"), NULL, 0, 0, false);
					if (m == true) {
						wcharTochar(TempID, InputID, 20);
						wcharTochar(TempPassword, InputPassword, 20);
						if (strcmp(InputID, accountTeachers) == 0 && strcmp(InputPassword, passwordTeachers) == 0) {
							InputBox(TempPassword, 20, _T("��֤ͨ��\n�����������������롿"), _T("�޸�����"), NULL, 0, 0, true);
							wcharTochar(TempPassword, InputPassword, 20);
							fp = fopen("accountTeachers.txt", "w");
							fprintf(fp, "%s\n", accountTeachers);
							fprintf(fp, "%s", InputPassword);
							fclose(fp);
							MessageBox(hwnd, _T("�޸ĳɹ��������Ʊ�������������"), _T("���"), MB_OK);
						}
						else {
							MessageBox(hwnd, _T("�˺Ż����������޷�ͨ����֤"), _T("��֤ʧ��"), MB_OK | MB_ICONWARNING);
							goto labelStart;
						}
					}
					else {
						goto labelStart;
					}
				}
				else {
					goto labelStart;
				}
				goto labelStart;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 1093 && msg.x <= 1192 && msg.y >= 667 && msg.y <= 689) {
				//ɾ����ʦ�˻�
				int i;
				i=MessageBox(hwnd, _T("ȷ��ɾ�������˻���\n��ᵼ�����Լ���ѧ����������Ϣ��ȫ��ɾ����"), _T("���������"), MB_OKCANCEL| MB_ICONWARNING);
				if (i == true) {
					FILE* fp;
					char accountTeachers[20];
					char passwordTeachers[20];
					fp = fopen("accountTeachers.txt", "r");
					fscanf(fp, "%s", accountTeachers);
					fscanf(fp, "%s", passwordTeachers);
					fclose(fp);
					wchar_t TempID[20], TempPassword[20];
					char InputID[20], InputPassword[20];
					int j;
					j=InputBox(TempID, 20, _T("�ڽ�����������֮ǰ\n��Ҫ����������˺ż����룬ȷ��Ϊ���˲���\n�������������˺š�"), _T("�����֤"), NULL, 0, 0, false);
					if (j == true) {
						int m;
						m=InputBox(TempPassword, 20, _T("���������������롿"), _T("�����֤"), NULL, 0, 0, false);
						if (m == true) {
							wcharTochar(TempID, InputID, 20);
							wcharTochar(TempPassword, InputPassword, 20);
							if (strcmp(InputID, accountTeachers) == 0 && strcmp(InputPassword, passwordTeachers) == 0) {
								remove("StuInformation.txt");
								remove("accountTeachers.txt");
								MessageBox(hwnd, _T("�����˻��ѱ�ɾ��"), _T("�����ɹ�"), MB_OK);
								LoginInterface();
							}
							else {
								MessageBox(hwnd, _T("�˺Ż����������޷�ͨ����֤"), _T("��֤ʧ��"), MB_OK);
								goto labelStart;
							}
						}
						else {
							goto labelStart;
						}
					}else{
						goto labelStart;
					}
				}
				else{
					goto labelStart;
				}
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 560 && msg.y <= 597) {
				//�˳�ϵͳ
				closegraph();
				goto labelTeachersMenu;
			}
		}
	}
labelTeachersMenu:
	return;
}

void StudentsMenu() {//ѧ�����˵�
	time_t timep;
	struct tm* p;
	time(&timep);
	p = gmtime(&timep);
	int SystemHour = 8 + p->tm_hour;
	labelStart:
	Node* ptrTemp = head;
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("ѧ�����˵�"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	wchar_t TempName[nameNum];
	wchar_t TempWelcome[20];//��ӭ��
	char Welcome[20] = {"\0"};
	wchar_t TempMessage[100];
	while (ptrTemp != NULL) {
		if (strcmp(STUID, ptrTemp->students.ID) == 0) {
			break;
		}
		ptrTemp = ptrTemp->next;
	}
	if (SystemHour >= 1 && SystemHour <= 3) {
		strcpy(Welcome, "��ҹ��");
	}
	else if (SystemHour >= 4 && SystemHour <= 8) {
		strcpy(Welcome, "���Ϻ�");
	}else if (SystemHour >= 8 && SystemHour <= 10) {
		strcpy(Welcome, "�����");
	}
	else if (SystemHour >= 11 && SystemHour <= 13) {
		strcpy(Welcome, "�����");
	}
	else if (SystemHour >= 14 && SystemHour <= 18) {
		strcpy(Welcome, "�����");
	}
	else if (SystemHour >= 19 && SystemHour <= 24) {
		strcpy(Welcome, "���Ϻ�");
	}
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0,191,255));
	settextstyle(20, 0, _T("����"));
	charTowchar(Welcome, TempWelcome,20);
	charTowchar(ptrTemp->students.name, TempName, 20);
	_stprintf_s(TempMessage, _T("��%sͬѧ��%s��^_^��"), TempName, TempWelcome);
	outtextxy(474, 196, TempMessage);
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 252 && msg.y <= 293) {
				//��ӡ���Ƴɼ�
				closegraph();
				PrintResults();
				goto labelStudentsMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 346 && msg.y <= 385) {
				//�޸�����
				ModifySTUPassword();
				goto labelStart;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 438 && msg.y <= 476) {
				//�˳�ϵͳ
				closegraph();
				goto labelStudentsMenu;
			}
		}
	}
labelStudentsMenu:
	return;
}

void InputInformation() {//1-¼��ѧ����Ϣ
	IMAGE img;
	initgraph(1200, 700, NOCLOSE);
	loadimage(&img, _T("PNG"), _T("¼��ѧ����Ϣ"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 250 && msg.y <= 289) {
				while(1) {
					Node* new_stu = (Node*)malloc(sizeof(Node));
					if (new_stu == NULL) {
						printf("�ڴ濪��ʧ�ܣ�");
						exit(1);
					}
					wchar_t TempName[nameNum];
					wchar_t TempSex[10];
					wchar_t TempID[40];
					wchar_t TempChinese[10];
					wchar_t TempMath[10];
					wchar_t TempEnglish[10];
					int i;
					wchar_t title[20];
					_stprintf_s(title, _T("����¼��� %d λͬѧ"), stucount + 1);
					i = InputBox(TempName, nameNum, _T("�����롾������"), title, NULL, 0, 0, false);
					if (i == true) {
						wchar_t reminder[20];
						_stprintf_s(reminder, _T("������%sͬѧ�ġ��Ա�"), TempName);
						InputBox(TempSex, 10, reminder, title, NULL, 0, 0, true);
						_stprintf_s(reminder, _T("������%sͬѧ�ġ�ѧ�š�"), TempName);
						InputBox(TempID, 40, reminder, title, NULL, 0, 0, true);
						_stprintf_s(reminder, _T("������%sͬѧ�ġ����ĳɼ���"), TempName);
						InputBox(TempChinese, 10, reminder, title, NULL, 0, 0, true);
						_stprintf_s(reminder, _T("������%sͬѧ�ġ���ѧ�ɼ���"), TempName);
						InputBox(TempMath, 10, reminder, title, NULL, 0, 0, true);
						_stprintf_s(reminder, _T("������%sͬѧ�ġ�Ӣ��ɼ���"), TempName);
						InputBox(TempEnglish, 10, reminder, title, NULL, 0, 0, true);
						
						wcharTochar(TempName, new_stu->students.name, nameNum);
						wcharTochar(TempSex, new_stu->students.sex, 10);
						wcharTochar(TempID, new_stu->students.ID, 40);
						wcharTochar(_T("123456"), new_stu->students.password, 40);
						new_stu->students.Chinese = _wtoi(TempChinese);
						new_stu->students.Math = _wtoi(TempMath);
						new_stu->students.English = _wtoi(TempEnglish);
						if (stucount == 0) {
							head = new_stu;
							new_stu->next = NULL;
							ptr = new_stu;
						}
						else {
							ptr->next = new_stu;
							ptr = new_stu;
							ptr->next = NULL;
						}
						stucount++;
					}
					else if (i == false) {
						WriteToFile();//д���ļ�
						closegraph();
						TeachersMenu();
						goto labelInputInformation;
					}
				}
				
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 336 && msg.y <= 376) {
				WriteToFile();//д���ļ�
				closegraph();
				TeachersMenu();
				goto labelInputInformation;
			}
		}
	}
labelInputInformation:
	return;
}

void DeleteInformation() {//2-ɾ��ѧ����Ϣ
	IMAGE img;
	initgraph(1200, 700, NOCLOSE);//����
	loadimage(&img, _T("PNG"), _T("ɾ��ѧ����Ϣ"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	Node* tempstu = NULL;
	wchar_t TempDelete[40];
	char Delete[40];
	int _bool = 0;
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
	labelDeleteStart:	while (MouseHit())
		{
			msg = GetMouseMsg();
			// ��ȡ�����Ϣ
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 250 && msg.y <= 290) {
				Node* _delete = head;
				Node* deleteTemp = head;
				int i;
				i = InputBox(TempDelete, 40, _T("��������Ӧͬѧ��ѧ��"), _T("ɾ��ѧ����Ϣ"), NULL, 0, 0, false);
				wcharTochar(TempDelete, Delete, 40);
				if (i == true) {
					int tempcount = 0;
					while (_delete != NULL) {
						tempcount++;
						if (strcmp(_delete->students.ID, Delete) == 0) {
							_bool = 1;
							wchar_t s[50];
							wchar_t TempID[50];
							wchar_t TempName[50];
							int n;
							charTowchar(_delete->students.ID, TempID, 50);
							charTowchar(_delete->students.name, TempName, 50);
							_stprintf_s(s, _T("�ҵ�ѧ��Ϊ%s��%sͬѧ"), TempID,TempName);
							n = MessageBox(hwnd, _T("ȷ��Ҫɾ����ͬѧ������Ϣ��"), s, MB_OKCANCEL | MB_ICONWARNING);
							if (n == 1) {
								if (tempcount > 1 && tempcount != stucount) {
									tempstu = _delete->next;
									free(_delete);
									deleteTemp->next = tempstu;
									stucount--;
								}
								else if (tempcount == 1) {
									head = head->next;
									free(_delete);
									stucount--;
								}
								else if (tempcount == stucount) {
									deleteTemp->next = NULL;
									free(_delete);
									stucount--;
								}
								MessageBox(hwnd, _T("�ɹ�ɾ��"), _T(" "), MB_OKCANCEL | MB_ICONWARNING);
								goto labelDeleteStart;
							}
							else {
								//ȡ��ɾ��
								//��ִ���κ�ָ��
							}
						}
						_delete = _delete->next;
						if (tempcount >= 2) {
							deleteTemp = deleteTemp->next;
						}
					}
					if (_bool == 0) {
						MessageBox(hwnd, _T("�����Ƿ���ȷ����ѧ��"), _T("δ�ҵ���Ӧͬѧ"), MB_OK| MB_ICONWARNING);
					}
					_bool = 0;
				}
				else if (i == false) {
					goto labelDelete;
				}
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 336 && msg.y <= 376) {
				WriteToFile();//д���ļ�
				goto labelDelete;
			}
		}
	}
labelDelete:
	closegraph();
	TeachersMenu();
}

void ModifyInformation() {//3-�޸�ѧ����Ϣ
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("�޸�ѧ����Ϣ"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	int _bool = 0;
	Node* modify = (Node*)malloc(sizeof(Node));
	if (modify == NULL) {
		exit(1);
	}
	modify = head;
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
	labelDeleteStart:	while (MouseHit()) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 250 && msg.y <= 290) {
			wchar_t TempID[40];
			char ID[40];
			int i;
			i = InputBox(TempID, 40, _T("��������Ӧͬѧ��ѧ��"), _T("�޸�ѧ����Ϣ"), NULL, 0, 0, false);
			wcharTochar(TempID, ID, 40);
			if (i == true) {
				while (modify != NULL) {
					if (strcmp(modify->students.ID, ID) == 0) {
						_bool = 1;
						wchar_t title[40];
						wchar_t temp[nameNum];
						charTowchar(modify->students.name, temp, nameNum);
						_stprintf_s(title, _T("���ҵ���%s��ͬѧ"), temp);//����
						int define;
						define=MessageBox(hwnd, _T("ȷ���޸Ĵ�ͬѧ����Ϣ��"), title, MB_OKCANCEL);
						if (define == true) {
							wchar_t NameTemp[nameNum];
							wchar_t SexTemp[nameNum];
							wchar_t IDTemp[nameNum];
							wchar_t ChineseTemp[nameNum];
							wchar_t MathTemp[nameNum];
							wchar_t EnglishTemp[nameNum];

							InputBox(NameTemp, nameNum, _T(" �������޸ĺ�ġ�������"));
							InputBox(SexTemp, 10, _T(" �������޸ĺ�ġ��Ա�"));
							InputBox(IDTemp, 40, _T(" �������޸ĺ�ġ�ѧ�š�"));
							InputBox(ChineseTemp, 10, _T(" �������޸ĺ�ġ����ĳɼ���"));
							InputBox(MathTemp, 10, _T(" �������޸ĺ�ġ���ѧ�ɼ���"));
							InputBox(EnglishTemp, 10, _T(" �������޸ĺ�ġ�Ӣ��ɼ���"));

							wcharTochar(NameTemp, modify->students.name, nameNum);
							wcharTochar(SexTemp, modify->students.sex, 10);
							wcharTochar(IDTemp, modify->students.ID, 40);
							modify->students.Chinese = _wtoi(ChineseTemp);
							modify->students.Math = _wtoi(MathTemp);
							modify->students.English = _wtoi(EnglishTemp);
							MessageBox(hwnd, _T("�޸ĳɹ�"), _T(" "), MB_OKCANCEL | MB_ICONWARNING);
							goto labelDeleteStart;
						}
					}
					else {
						modify = modify->next;
					}
				}
				if (_bool == 0) {
					MessageBox(hwnd, _T("����ѧ���Ƿ�������ȷ"), _T("δ�ҵ���ͬѧ"), MB_OK | MB_ICONWARNING);
				}
			}
		}
		else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 336 && msg.y <= 376) {
			WriteToFile();
			goto labelModify;
		}
	}
	}
labelModify:
	closegraph();
	TeachersMenu();
}

void FindInformation() {//4-����ѧ����Ϣ
	IMAGE img;
	initgraph(1200, 700);//����
	loadimage(&img, _T("PNG"), _T("����ѧ����Ϣ"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	int  _bool = 0;
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
		Node* ptrTemp = head;
	labelDeleteStart:	while (MouseHit()) {
		msg = GetMouseMsg();
		if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 250 && msg.y <= 290) {
			wchar_t _TempID[40];
			char ID[100];
			int i;
			i = InputBox(_TempID, 40, _T("��������Ӧͬѧ��ѧ��"), _T("����ѧ����Ϣ"), NULL, 0, 0, false);
			wcharTochar(_TempID, ID, 40);
			if (i == true) {
				while (ptrTemp != NULL) {
					if (strcmp(ID, ptrTemp->students.ID) == 0) {
						_bool = 1;
						wchar_t title[40], content[500];
						wchar_t TempName[nameNum];
						wchar_t TempSex[10];
						wchar_t TempID[40];
						wchar_t TempChinese[10];
						wchar_t TempMath[10];
						wchar_t TempEnglish[10];
						charTowchar(ptrTemp->students.name, TempName, nameNum);
						charTowchar(ptrTemp->students.sex, TempSex, 10);
						charTowchar(ptrTemp->students.ID, TempID, 40);
						_stprintf_s(title, _T("���ҵ�ѧ��Ϊ��%s����ͬѧ"), TempID);
						_stprintf_s(content, _T("����:%s\n�Ա�:%s\nѧ��:%s\n���ĳɼ�:%d\n��ѧ�ɼ�:%d\nӢ��ɼ�:%d"), TempName, TempSex, TempID, ptrTemp->students.Chinese, ptrTemp->students.Math, ptrTemp->students.English);
						MessageBox(hwnd, content, title, MB_OK);
					}
					ptrTemp = ptrTemp->next;
				}
				if (_bool == 0) {
					MessageBox(hwnd, _T("����ѧ���Ƿ�������ȷ"), _T("δ�ҵ���ͬѧ"), MB_OK | MB_ICONWARNING);
				}
				_bool = 0;
			}
		}
		else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 336 && msg.y <= 376) {
			WriteToFile();//д���ļ�
			goto labelFind;
		}
	}
	}
labelFind:
	closegraph();
	TeachersMenu();
}

void ProcessingData() {//5-����ѧ���ɼ�
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("����ѧ����Ϣ"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	labelStart:
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 220 && msg.y <= 259) {
				//��ӡ�ɼ���
				closegraph();
				PrintResults();
				goto labelProcessingData;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 287 && msg.y <= 326) {
				//����
				closegraph();
				Rank();
				goto labelProcessingData;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 354 && msg.y <= 394) {
				//�����ɼ�
				closegraph();
				AnalysisResults();
				goto labelProcessingData;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 423 && msg.y <= 460) {
				//����ѧ������
				ResetSTUPassword();
				goto labelStart;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 491 && msg.y <= 528) {
				//������һ��
				closegraph();
				TeachersMenu();
				goto labelProcessingData;
			}
		}
	}
labelProcessingData:
	return;
}

void PrintResults() {//��ӡ�ɼ���
	int _bool = 0;
	Node* ptrTemp = head;
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("�ɼ���"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	wchar_t TempInformation[100];
	setbkmode(TRANSPARENT);
	settextcolor(RGB(153,50,204));
	settextstyle(20, 0, _T("����"));
	if (power == 'T') {
		outtextxy(18, 113, _T("    ѧ��                   �� ��       �� ��  �� ��   �� ѧ     Ӣ ��      �� ��"));
		int x = 18;
		for (int y = 135;; y += 20) {
			if (ptrTemp == NULL) {
				break;
			}
			wchar_t TempName[nameNum];
			wchar_t TempSex[10];
			wchar_t TempID[40];
			charTowchar(ptrTemp->students.name, TempName, nameNum);
			charTowchar(ptrTemp->students.sex, TempSex, 10);
			charTowchar(ptrTemp->students.ID, TempID, 40);
			int sum = ptrTemp->students.Chinese + ptrTemp->students.Math + ptrTemp->students.English;
			_stprintf_s(TempInformation, _T("%-20s %-10s %-5s %-10d %-10d %-10d %-10d"), TempID, TempName, TempSex, ptrTemp->students.Chinese, ptrTemp->students.Math, ptrTemp->students.English, sum);
			outtextxy(x, y, TempInformation);
			ptrTemp = ptrTemp->next;
			if (y >= 620) {
				if (_bool == 0) {
					outtextxy(620, 113, _T("       ѧ��                �� ��       �� ��  �� ��   �� ѧ     Ӣ ��      �� ��"));
					_bool = 1;
				}
				x = 620;
				y = 115;
			}
		}
	}
	else if (power == 'S') {
		ptrTemp = head;
		while (ptrTemp!=NULL) {
			if (strcmp(STUID, ptrTemp->students.ID)==0) {
				break;
			}
			ptrTemp = ptrTemp->next;
		}
		wchar_t TempMessage[200];
		wchar_t TempScore[200];
		wchar_t TempName[nameNum];
		wchar_t TempSex[10];
		wchar_t TempID[40];
		charTowchar(ptrTemp->students.name, TempName, nameNum);
		charTowchar(ptrTemp->students.sex, TempSex, 10);
		charTowchar(ptrTemp->students.ID, TempID, 40);
		int sum = ptrTemp->students.Chinese + ptrTemp->students.Math + ptrTemp->students.English;
		_stprintf_s(TempMessage, _T("%sͬѧ����ĳɼ����£������κ����ʣ��뼰ʱ��ϵ��ʦ"),TempName );
		_stprintf_s(TempScore, _T("%-20s%-10s %-5s %-10d %-10d %-10d %-10d"), TempID, TempName, TempSex, ptrTemp->students.Chinese, ptrTemp->students.Math, ptrTemp->students.English, sum);
		outtextxy(200, 110, TempMessage);
		outtextxy(200,140, _T("    ѧ��                   �� ��       �� ��  �� ��   �� ѧ     Ӣ ��      �� ��"));
		outtextxy(200, 170, TempScore);
	}
labelStart:
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit()) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 780 && msg.x <= 995 && msg.y >= 650 && msg.y <= 688) {
				//����Ϊ��ҳ
				if (power == 'T') {
					_bool = 0;
					ExportAsWebPage();
					goto labelStart;
				}
				else if (power == 'S') {
					_bool = 0;
					MessageBox(hwnd, _T("ѧ���ݲ�֧�ֵ����ɼ�"), _T("��Ȩ��"), MB_OK | MB_ICONWARNING);
					goto labelStart;
				}
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 1019 && msg.x <= 1186 && msg.y >= 649 && msg.y <= 689) {
				//������һ��
				_bool = 0;
				closegraph();
				if (power == 'T') {
					ProcessingData();
				}
				else {
					StudentsMenu();
				}
				goto labelPrintResults;
			}
		}
	}
labelPrintResults:
	return;
}

void ExportAsWebPage() {//���ɼ�������Ϊ��ҳ
	HWND hwnd;
	hwnd = GetHWnd();
	time_t timep;
	struct tm* p;
	time(&timep);
	p = gmtime(&timep);
	int SystemSec = p->tm_sec;
	int SystemMin = p->tm_min;
	int SystemHour = 8 + p->tm_hour;
	int SystemDay = p->tm_mday;
	int SystemMonth = 1 + p->tm_mon;
	int SystemYear = 1900 + p->tm_year;
	Node* ptrTemp=head;
	FILE* fp;
	int countTemp = 1;
	fp = fopen("�ɼ���.html", "a+");
	fclose(fp);
	fp = fopen("�ɼ���.html", "w");
	fprintf(fp, "<!DOCTYPE html>\n");
	fprintf(fp, "<html lang=\"en\">\n");
	fprintf(fp, "<head>\n");
	fprintf(fp, "<meta charset=\"UTF - 8\">\n");
	fprintf(fp, "<title>�ɼ���</title>\n");
	fprintf(fp, "</head>\n");
	fprintf(fp, "<body>\n");
	fprintf(fp, "<hr/>\n");
	fprintf(fp, "<br/>\n");
	fprintf(fp, "<h1 align=\"center\" style=\"font - size:30pt\">ѧ���ɼ�����ϵͳ 2.0</h1>\n");
	fprintf(fp, "<hr/>\n");
	fprintf(fp, "<h2 align=\"center\" style=\"font - size:10pt\">����ʱ�䣺%d��%d��%d��%dʱ%d��%d��</h2>\n", SystemYear, SystemMonth, SystemDay, SystemHour, SystemMin, SystemSec);
	fprintf(fp, "<br/>\n");
	fprintf(fp, "<table border=\"1\" align=\"center\" cellpadding=\"8\" cellspacing=\"0\">\n");
	fprintf(fp, "<tr align=\"center\" bgcolor=\"CC0000\">\n");
	fprintf(fp, "<th colspan=\"8\" style=\"color:white\">�� �� ��</th>\n");
	fprintf(fp, "<tr/>\n");
	fprintf(fp, "<tr align=\"center\" bgcolor=\"#FAEBD7\">\n");
	fprintf(fp, "<th rowspan=\"2\">���</th>\n");
	fprintf(fp, "<th rowspan=\"2\">ѧ��</th>\n");
	fprintf(fp, "<th rowspan=\"2\">����</th>\n");
	fprintf(fp, "<th rowspan=\"2\">�Ա�</th>\n");
	fprintf(fp, "<th colspan=\"5\">��   ��   ��   ��</th>\n");
	fprintf(fp, "</tr>\n");
	fprintf(fp, "<tr align=\"center\" bgcolor=\"#FAEBD7\">\n");
	fprintf(fp, " <th width=\"60\">����</th>\n");
	fprintf(fp, "<th width=\"60\">��ѧ</th>\n");
	fprintf(fp, "<th width=\"60\">Ӣ��</th>\n");
	fprintf(fp, "<th width=\"60\">�ܷ�</th>\n");
	fprintf(fp, "</tr>\n");
	while (ptrTemp != NULL) {
		fprintf(fp, "<tr align=\"center\" bgcolor=\"#FAEBD7\">\n");
		fprintf(fp, "<td>%d</td>\n", countTemp);//���
		fprintf(fp, "<td>%s</td>\n",ptrTemp->students.ID);//ѧ��
		fprintf(fp, "<td>%s</td>\n", ptrTemp->students.name);//����
		fprintf(fp, "<td>%s</td>\n", ptrTemp->students.sex);//�Ա�
		fprintf(fp, "<td>%d</td>\n", ptrTemp->students.Chinese);//���ķ���
		fprintf(fp, "<td>%d</td>\n", ptrTemp->students.Math);//��ѧ����
		fprintf(fp, "<td>%d</td>\n", ptrTemp->students.English);//Ӣ�����
		fprintf(fp, "<td>%d</td>\n", ptrTemp->students.Chinese+ ptrTemp->students.Math+ ptrTemp->students.English);//�ܷ�
		fprintf(fp, "</tr>\n");
		ptrTemp = ptrTemp->next;
		countTemp++;
	}
	fprintf(fp, "</table>\n");
	fprintf(fp, "</body>\n");
	fprintf(fp, "</html>\n");
	fclose(fp);
	MessageBox(hwnd, _T("�ɼ��������ɹ���"), _T("���"), MB_OK);
}

void Rank() {//ѡ�������Ŀ
	IMAGE img;
	initgraph(1200, 700);//����
	loadimage(&img, _T("PNG"), _T("ѡ�������Ŀ"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	int i;
	wchar_t TempChinese[5];
	wchar_t TempMath[5];
	wchar_t TempEnglish[5];
	wchar_t TempTotalScore[5];
	char Chinese[5];
	char Math[5];
	char English[5];
	char TotalScore[5];
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit()){
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 220 && msg.y <= 259) {
				//����
				i = InputBox(TempChinese, 5, _T("������1��2\n1 ����[����]\n2 ����[����]"), _T("ѡ������ʽ��"), NULL, 0, 0, false);
				if (i == true) {
					wcharTochar(TempChinese, Chinese, 5);
					RankChinese(Chinese);
					MessageBox(hwnd, _T("�����ġ�����ɹ�"),_T("��Ϣ"), MB_OK);
				}
				else {
					//��ִ���κβ���
				}
				ProcessingData();
				goto labelRank;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 287 && msg.y <= 326) {
				//��ѧ
				i = InputBox(TempMath, 5, _T("������1��2\n1 ����[����]\n2 ����[����]"), _T("ѡ������ʽ��"), NULL, 0, 0, false);
				if (i == true) {
					wcharTochar(TempMath, Math, 5);
					RankMath(Math);
					MessageBox(hwnd, _T("����ѧ������ɹ�"), _T("��Ϣ"), MB_OK);
				}
				ProcessingData();
				goto labelRank;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 354 && msg.y <= 394) {
				//Ӣ��
				i = InputBox(TempEnglish, 5, _T("������1��2\n1 ����[����]\n2 ����[����]"), _T("ѡ������ʽ��"), NULL, 0, 0, false);
				if (i == true) {
					wcharTochar(TempEnglish, English, 5);
					RankEnglish(English);
					MessageBox(hwnd, _T("��Ӣ�����ɹ�"), _T("��Ϣ"), MB_OK);
				}
				ProcessingData();
				goto labelRank;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 422 && msg.y <= 460) {
				//�ܷ�
				i = InputBox(TempTotalScore, 5, _T("������1��2\n1 ����[����]\n2 ����[����]"), _T("ѡ������ʽ��"), NULL, 0, 0, false);
				if (i == true) {
					wcharTochar(TempTotalScore, TotalScore, 5);
					RankTotalScore(TotalScore);
					MessageBox(hwnd, _T("���ܷ֡�����ɹ�"), _T("��Ϣ"), MB_OK);
				}
				ProcessingData();
				goto labelRank;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 490 && msg.y <= 528) {
				//������һ��
				closegraph();
				ProcessingData();
				goto labelRank;
			}
		}
	}
labelRank:
	return;
}

void AverageScore() {//�����ƽ����
	Node* ptrTemp=head;
	int ChAva=0, MaAva=0, EnAva=0;
	while (ptrTemp != NULL) {
		ChAva += ptrTemp->students.Chinese;
		MaAva += ptrTemp->students.Math;
		EnAva += ptrTemp->students.English;
		ptrTemp = ptrTemp->next;
	}
	ChineseAverage = (float)ChAva / (float)stucount;
	MathAverage = (float)MaAva / (float)stucount;
	EnglishAverage = (float)EnAva / (float)stucount;
}

void AnalysisResults() {//�����ɼ�
	AverageScore();
	IMAGE img;
	HWND hwnd;
	MOUSEMSG msg;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("�����ɼ�"), 1200, 700);
	putimage(0, 0, &img);
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	setbkmode(TRANSPARENT);
	settextcolor(RGB(30, 144, 255));
	settextstyle(20, 0, _T("����"));
	//���ķֶ�
	int Ch_1 = ClassificationByGrade(0, 59, 1);
	int Ch_2 = ClassificationByGrade(60, 75, 1);
	int Ch_3 = ClassificationByGrade(76, 89, 1);
	int	Ch_4 = ClassificationByGrade(90, 1000, 1);
	//��ѧ�ֶ�
	int Ma_1 = ClassificationByGrade(0, 59, 2);
	int	Ma_2 = ClassificationByGrade(60, 75, 2);
	int	Ma_3 = ClassificationByGrade(76, 89, 2);
	int	Ma_4 = ClassificationByGrade(90, 1000, 2);
	//Ӣ��ֶ�
	int En_1 = ClassificationByGrade(0, 59, 3);
	int	En_2 = ClassificationByGrade(60, 75, 3);
	int En_3 = ClassificationByGrade(76, 89, 3);
	int En_4 = ClassificationByGrade(90, 1000, 3);
	//���û�ͼ����
	setfillstyle(BS_SOLID);// �����ʽ
	setfillcolor(RGB(30 ,144 ,255));//�����ɫ
	int parameter = 225;//�ı����ֵ�����Կ�����״ͼ�����߶�
	wchar_t numTemp[500];
	//�������ķ�����ͳ��ͼ
	_stprintf_s(numTemp, _T("%d��"), Ch_1);
	solidrectangle(92, 340-(parameter * Ch_1/stucount), 125, 340);// ���Ͻ����꣬���½�����
	outtextxy(92, 340 - (parameter * Ch_1 / stucount)-25, numTemp);//��ʾ����

	_stprintf_s(numTemp, _T("%d��"), Ch_2);
	solidrectangle(154, 340 - (parameter * Ch_2 / stucount), 185, 340);
	outtextxy(154, 340 - (parameter * Ch_2 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d��"), Ch_3);
	solidrectangle(216, 340 - (parameter * Ch_3 / stucount), 247, 340);
	outtextxy(216, 340 - (parameter * Ch_3 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d��"), Ch_4);
	solidrectangle(277, 340 - (parameter * Ch_4 / stucount), 307, 340);
	outtextxy(277, 340 - (parameter * Ch_4 / stucount) - 25, numTemp);

	//������ѧ������ͳ��ͼ
	_stprintf_s(numTemp, _T("%d��"), Ma_1);
	solidrectangle(440, 335 - (parameter * Ma_1 / stucount), 472, 335);
	outtextxy(440, 335 - (parameter * Ma_1 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d��"), Ma_2);
	solidrectangle(502, 335 - (parameter * Ma_2 / stucount), 534, 335);
	outtextxy(502, 335 - (parameter * Ma_2 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d��"), Ma_3);
	solidrectangle(564, 335 - (parameter * Ma_3 / stucount), 594, 335);
	outtextxy(564, 335 - (parameter * Ma_3 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d��"), Ma_4);
	solidrectangle(624, 335 - (parameter * Ma_4 / stucount), 656, 335);
	outtextxy(624, 335 - (parameter * Ma_4 / stucount) - 25, numTemp);

	//����Ӣ�������ͳ��ͼ
	_stprintf_s(numTemp, _T("%d��"), En_1);
	solidrectangle(777, 330 - (parameter * En_1 / stucount), 808, 330);
	outtextxy(777, 330 - (parameter * En_1 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d��"), En_2);
	solidrectangle(838, 330 - (parameter * En_2 / stucount), 870, 330);
	outtextxy(838, 330 - (parameter * En_2 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d��"), En_3);
	solidrectangle(901, 330 - (parameter * En_3 / stucount), 932, 330);
	outtextxy(901, 330 - (parameter * En_3 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d��"), En_4);
	solidrectangle(961, 330 - (parameter * En_4 / stucount), 993, 330);
	outtextxy(961, 330 - (parameter * En_4 / stucount) - 25, numTemp);

	settextcolor(RGB(0, 0, 0));//������ɫ
	_stprintf_s(numTemp, _T("��%d���гɼ���¼,���� ����ƽ����Ϊ%.2f  ��ѧƽ����Ϊ%.2f  Ӣ��ƽ����Ϊ%.2f"), Ch_1+ Ch_2+ Ch_3+ Ch_4, ChineseAverage,MathAverage,EnglishAverage);
	outtextxy(25, 460, numTemp);
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit()) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 1019 && msg.x <= 1186 && msg.y >= 649 && msg.y <= 689) {
				//������һ��
				closegraph();
				ProcessingData();
				goto labelAnalysisResults;
			}
		}
	}
	getchar();
labelAnalysisResults:
	return;
}

void RankChinese(char* way) {//�����ĳɼ�����
	Node* ChineseGrade = (Node*)malloc(sizeof(Node));
	ChineseGrade = head;
	STU* array = (STU*)malloc(stucount * sizeof(STU));
	for (int i = 0; ChineseGrade != NULL;i++) {
		array[i] = (ChineseGrade->students);
		ChineseGrade = ChineseGrade->next;
	}
	if (strcmp(way,"1")==0) {
		int flag=0;
		STU temp;
		for (int i = 0; i < stucount; i++,flag=i ) {
			for (int j = i + 1; j < stucount; j++) {
				if (array[flag].Chinese > array[j].Chinese) {
					flag = j;
				}
			}
			temp = array[i];
			array[i] = array[flag];
			array[flag] = temp;
		}
	}
	else if (strcmp(way, "2") == 0) {
		int flag = 0;
		STU temp;
		for (int i = 0; i < stucount; i++, flag = i) {
			for (int j = i + 1; j < stucount; j++) {
				if (array[flag].Chinese < array[j].Chinese) {
					flag = j;
				}
			}
			temp = array[i];
			array[i] = array[flag];
			array[flag] = temp;
		}
	}
	ChineseGrade = head;
	for (int i = 0; ChineseGrade != NULL; i++) {
		ChineseGrade->students = array[i];
		ChineseGrade = ChineseGrade->next;
	}
	WriteToFile();
}

void RankMath(char *way) {//����ѧ�ɼ�����
	Node* MathGrade = (Node*)malloc(sizeof(Node));
	MathGrade = head;
	STU* array = (STU*)malloc(stucount * sizeof(STU));
	for (int i = 0; MathGrade != NULL; i++) {
		array[i] = (MathGrade->students);
		MathGrade = MathGrade->next;
	}
	if (strcmp(way, "1") == 0) {
		int flag = 0;
		STU temp;
		for (int i = 0; i < stucount; i++, flag = i) {
			for (int j = i + 1; j < stucount; j++) {
				if (array[flag].Math > array[j].Math) {
					flag = j;
				}
			}
			temp = array[i];
			array[i] = array[flag];
			array[flag] = temp;
		}
	}
	else if (strcmp(way, "2") == 0) {
		int flag = 0;
		STU temp;
		for (int i = 0; i < stucount; i++, flag = i) {
			for (int j = i + 1; j < stucount; j++) {
				if (array[flag].Math < array[j].Math) {
					flag = j;
				}
			}
			temp = array[i];
			array[i] = array[flag];
			array[flag] = temp;
		}
	}
	MathGrade = head;
	for (int i = 0; MathGrade != NULL; i++) {
		MathGrade->students = array[i];
		MathGrade = MathGrade->next;
	}
	WriteToFile();
}

void RankEnglish(char* way) {//��Ӣ��ɼ�����
	Node* EnglishGrade = (Node*)malloc(sizeof(Node));
	EnglishGrade = head;
	STU* array = (STU*)malloc(stucount * sizeof(STU));
	for (int i = 0; EnglishGrade != NULL; i++) {
		array[i] = (EnglishGrade->students);
		EnglishGrade = EnglishGrade->next;
	}
	if (strcmp(way, "1") == 0) {
		int flag = 0;
		STU temp;
		for (int i = 0; i < stucount; i++, flag = i) {
			for (int j = i + 1; j < stucount; j++) {
				if (array[flag].English > array[j].English) {
					flag = j;
				}
			}
			temp = array[i];
			array[i] = array[flag];
			array[flag] = temp;
		}
	}
	else if (strcmp(way, "2") == 0) {
		int flag = 0;
		STU temp;
		for (int i = 0; i < stucount; i++, flag = i) {
			for (int j = i + 1; j < stucount; j++) {
				if (array[flag].English < array[j].English) {
					flag = j;
				}
			}
			temp = array[i];
			array[i] = array[flag];
			array[flag] = temp;
		}
	}
	EnglishGrade = head;
	for (int i = 0; EnglishGrade != NULL; i++) {
		EnglishGrade->students = array[i];
		EnglishGrade = EnglishGrade->next;
	}
	WriteToFile();
}

void RankTotalScore(char* way) {//�����ܳɼ�����
	Node* TotalScore = (Node*)malloc(sizeof(Node));
	TotalScore = head;
	STU* array = (STU*)malloc(stucount * sizeof(STU));
	for (int i = 0; TotalScore != NULL; i++) {
		array[i] = (TotalScore->students);
		TotalScore = TotalScore->next;
	}
	if (strcmp(way, "1") == 0) {
		int flag = 0;
		STU temp;
		for (int i = 0; i < stucount; i++, flag = i) {
			for (int j = i + 1; j < stucount; j++) {
				if ((array[flag].Chinese + array[flag].Math + array[flag].English) > (array[j].Chinese + array[j].Math + array[j].English)) {
					flag = j;
				}
			}
			temp = array[i];
			array[i] = array[flag];
			array[flag] = temp;
		}
	}
	else if (strcmp(way, "2") == 0) {
		int flag = 0;
		STU temp;
		for (int i = 0; i < stucount; i++, flag = i) {
			for (int j = i + 1; j < stucount; j++) {
				if ((array[flag].Chinese + array[flag].Math + array[flag].English) < (array[j].Chinese + array[j].Math + array[j].English)) {
					flag = j;
				}
			}
			temp = array[i];
			array[i] = array[flag];
			array[flag] = temp;
		}
	}
	TotalScore = head;
	for (int i = 0; TotalScore != NULL; i++) {
		TotalScore->students = array[i];
		TotalScore = TotalScore->next;
	}
	WriteToFile();
}

int ClassificationByGrade(int min,int max,int subject) {//���ɼ���ѧ�����з��ࣨͳ��ָ����Ŀ���������ε�ѧ��������1-���� 2-��ѧ 3-Ӣ�
	Node* ptrTemp = head;
	int count = 0;
	if (subject == 1) {//ͳ������
		while (ptrTemp != NULL) {
			if (ptrTemp->students.Chinese >= min && ptrTemp->students.Chinese <= max) {
				count++;
			}
			ptrTemp = ptrTemp->next;
		}
	}
	else if (subject == 2) {//ͳ����ѧ
		while (ptrTemp != NULL) {
			if (ptrTemp->students.Math >= min && ptrTemp->students.Math <= max) {
				count++;
			}
			ptrTemp = ptrTemp->next;
		}
	}
	else if (subject == 3) {//ͳ��Ӣ��
		while (ptrTemp != NULL) {
			if (ptrTemp->students.English >= min && ptrTemp->students.English <= max) {
				count++;
			}
			ptrTemp = ptrTemp->next;
		}
	}
	return count;
}

void SetTEAPassword() {//������ʦ�˻�
	IMAGE img;
	initgraph(1200, 700);//����
	loadimage(&img, _T("PNG"), _T("��ʦ��¼"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("ѧ���ɼ�����ϵͳ 2.0"));
	MessageBox(hwnd, _T("����Ϊ������һ�����õġ���ʦ���˻�"), _T("�����˻�"), MB_OK);
	wchar_t Temppassaccount[20];
	InputBox(Temppassaccount, 20, _T("�������˺�"), _T("��������ʦ�˻���"), NULL, 0, 0, true);
labelStart:	wchar_t Temppassword_one[20], Temppassword_two[20];
	InputBox(Temppassword_one, 20, _T("����������"), _T("��������ʦ�˻���"), NULL, 0, 0, true);
	InputBox(Temppassword_two, 20, _T("���ٴ���������"), _T("��������ʦ�˻���"), NULL, 0, 0, true);
	char temp_one[20], temp_two[20];
	wcharTochar(Temppassword_one, temp_one, 20);
	wcharTochar(Temppassword_two, temp_two, 20);
	if (strcmp(temp_one, temp_two) != 0) {
		MessageBox(hwnd, _T("������������벻һ�£�����������"), _T(""), MB_OK| MB_ICONEXCLAMATION);
		goto labelStart;
	}
	FILE* fp;
	char accountTeachers[20], passwordTeachers[20];
	fp = fopen("accountTeachers.txt", "w");
	wcharTochar(Temppassaccount, accountTeachers, 20);
	fputs(accountTeachers, fp);
	fputs("\n", fp);
	wcharTochar(Temppassword_two, passwordTeachers, 20);
	fputs(passwordTeachers, fp);
	fclose(fp);
	MessageBox(hwnd, _T("���ڣ�������ʹ������˻���¼����ʦ�ˡ��ˣ�"), _T("�����ɹ�"), MB_OK);
	return;
}

void ModifySTUPassword() {//��ѧ�����޸�ѧ����¼����
	HWND hwnd;
	Node* ptrTemp;
	hwnd = GetHWnd();
	wchar_t TempOld[20];
	wchar_t TempNew[20];
	char Old[20];
	char New[20];
	int i;
	labelModifySTUPassword:
	i=InputBox(TempOld, 20, _T("���������ľ�����"), _T("�޸ĵ�¼����"), NULL, 0, 0, false);
	wcharTochar(TempOld, Old, 20);
	ptrTemp = head;
	while (ptrTemp != NULL) {
		if (strcmp(STUID, ptrTemp->students.ID) == 0) {
			break;
		}
		ptrTemp = ptrTemp->next;
	}
	if (i == true) {
		if (strcmp(Old, ptrTemp->students.password) == 0) {
			int j;
			j=InputBox(TempNew, 20, _T("�������µĵ�¼����"), _T("����������"), NULL, 0, 0, false);
			wcharTochar(TempNew, New, 20);
			if (j == true) {
				strcpy(ptrTemp->students.password, New);
				MessageBox(hwnd, _T("    �޸ĳɹ���\n�����Ʊ����Լ�������"), _T("����޸�"), MB_OK);
				WriteToFile();
			}
		}
		else {
			MessageBox(hwnd, _T("������ľ�������������������"), _T("�������"), MB_OKCANCEL | MB_ICONERROR);
			goto labelModifySTUPassword;
		}
	}
}

void ResetSTUPassword() {//����ʦ������ѧ����¼����
	HWND hwnd;
	Node* ptrTemp=head;
	hwnd = GetHWnd();
	int i,j;
	wchar_t TempID[20];
	wchar_t TempName[20];
	wchar_t contentTemp[50];
	char ID[20];
	i = InputBox(TempID, 20, _T("��������Ҫ���������ѧ����ѧ��"),_T("����ѧ����¼����"), NULL, 0, 0, false);
	wcharTochar(TempID, ID, 20);
	if (i == true) {
		while (ptrTemp != NULL) {
			if (strcmp(ID, ptrTemp->students.ID) == 0) {
				break;
			}
			ptrTemp = ptrTemp->next;
		}
		charTowchar(ptrTemp->students.ID, TempID, 20);
		charTowchar(ptrTemp->students.name, TempName, nameNum);
		_stprintf_s(contentTemp, _T("  �����ٴκ˶���Ϣ��\nѧ�ţ�%s\n������%s"), TempID, TempName);
		j = MessageBox(hwnd, contentTemp, _T("ȷ��������"), MB_OKCANCEL);
		if (j == true) {
			strcpy(ptrTemp->students.password, "123456");
			WriteToFile();
			MessageBox(hwnd, _T("���óɹ������ú������Ϊ123456����֪ͨѧ����ʱ�޸�"), _T("�������"), MB_OK);
		}
	}
}

void WriteToFile() {//����Ϣд���ļ�
	FILE* fp;
	Node* temp = head;
	fp = fopen("StuInformation.txt", "w");
	fprintf(fp,"ѧ��������%d\n", stucount);
	while (temp != NULL) {
		fprintf(fp, "%s %s %s %d %d %d %s\n", temp->students.ID, temp->students.name, temp->students.sex, temp->students.Chinese, temp->students.Math, temp->students.English, temp->students.password);
		temp = temp->next;
	}
	fclose(fp);
}

void ReadFromFile() {//���ļ���ȡ��Ϣ
	FILE* fpTest;//��ʼ���ļ�����ֹ��һ��ʹ��ϵͳʱ�����ļ�ʧ�ܡ�
	fpTest = fopen("StuInformation.txt", "a+");
	fclose(fpTest);
	FILE* fp;
	int countTemp = 0,count=0;
	fp = fopen("StuInformation.txt", "r");
	fscanf(fp, "ѧ��������%d", &stucount);
	if (stucount == -1 || stucount==0) {
		stucount = 0;
		return;
	}
	Node* temp=0;
	while (1) {
		Node* temp = (Node*)malloc(sizeof(Node));
		if (temp == NULL) {
			exit(1);
		}
		fscanf(fp, "%s %s %s %d %d %d %s", temp->students.ID, temp->students.name, temp->students.sex, &temp->students.Chinese, &temp->students.Math, &temp->students.English, &temp->students.password);
		count++;
		if (countTemp == 0) {
			head = temp;
			temp->next = NULL;
			ptr = temp;
			countTemp = 1;
		}
		else {
			ptr->next = temp;
			ptr = temp;
			ptr->next = NULL;
		}
		if (count == stucount) {
			break;
		}
	}
	fclose(fp);
}

void wcharTochar(const wchar_t* wchar, char* chr, int length) {//��wcharת��Ϊchar
	WideCharToMultiByte(CP_ACP, 0, wchar, -1, chr, length, NULL, NULL);
}

void charTowchar(const char* chr, wchar_t* wchar, int size) {//��charת��Ϊwchar
	MultiByteToWideChar(CP_ACP, 0, chr, strlen(chr) + 1, wchar, size / sizeof(wchar[0]));
}