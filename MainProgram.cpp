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
char power = '0';//权限控制
char STUID[40] = {"\0"};//学生身份识别
float ChineseAverage=0.0;//语文平均分
float MathAverage=0.0;//数学平均分
float EnglishAverage=0.0;//英语平均分

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
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	loadimage(&img, _T("PNG"), _T("退出界面"), 1200, 700);
	putimage(0, 0, &img);
	system("Pause");
	return 0;
}

void WelcomeInterface() {//欢迎界面
	IMAGE img;
	initgraph(1200, 700);//宽、高
	loadimage(&img, _T("PNG"),_T("欢迎界面"),1200,700);//设置图片宽、高，若加载原图大小
	putimage(0, 0, &img);//图片显示的位置
	HWND hwnd;// 设置窗口句柄变量保存窗口信息
	hwnd = GetHWnd();// 获取窗口句柄
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	Sleep(2000);
	closegraph();
}

void LoginInterface() {//登录界面
	IMAGE img;
	initgraph(1200, 700);
	HWND hwnd;// 设置窗口句柄变量保存窗口信息
	hwnd = GetHWnd();// 获取窗口句柄
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	loadimage(&img,_T("PNG"),_T("登录方式"), 1200, 700);
	putimage(0, 0, &img);
	//鼠标点击事件监听
	MOUSEMSG msg;// 定义变量，保存鼠标消息
	FlushMouseMsgBuffer();// 清空鼠标消息缓冲区	
	while (1) { // 主循环 循环监听鼠标信息
		while (MouseHit())// 监听鼠标信息 当有鼠标消息的时候执行 可检测连续的鼠标信息
		{
			msg = GetMouseMsg();// 获取鼠标消息
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x>=473 && msg.x<=730 && msg.y>=271 && msg.y<=340) {
				//教师登录
				TeacherLogin();
				goto label;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 473 && msg.x <= 730 && msg.y >= 379 && msg.y <= 449) {
				//学生登录
				StudentsLogin();
				goto label;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 473 && msg.x <= 730 && msg.y >= 480 && msg.y <= 552) {
				//退出
				closegraph();
				goto label;
			}
		}
	}
label:return;
}

void TeacherLogin() {//教师登录
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
		i = MessageBox(hwnd, L"现在想要创建一个新的账户吗？", _T("您还没创建任何账户"), MB_OKCANCEL | MB_ICONEXCLAMATION);
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
		loadimage(&img, _T("PNG"), _T("教师登录"), 1200, 700);
		putimage(0, 0, &img);
		hwnd = GetHWnd();
		SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	}
	wchar_t Tempaccount[20];
	wchar_t Temppassword[20];
	char account[20];
	char password[20];
	int i;
labelaccount:	i = InputBox(Tempaccount, 20, _T("请输入账号"), _T("教师登录"), NULL, 0, 0, false);
	if (i == true) {
		wcharTochar(Tempaccount,account , 20);
		InputBox(Temppassword, 20, _T("请输入密码"), _T("教师登录"), NULL, 0, 0, true);
		wcharTochar(Temppassword, password, 20);
			if (strcmp(password, passwordTeachers) == 0 && strcmp(account, accountTeachers) == 0) {
				closegraph();
				power = 'T';
				TeachersMenu();
				goto labelTeacherLogin;
			}
			else {
				MessageBox(hwnd, L"您所输入的账号或密码有误，请重新输入！", _T("账号密码错误"), MB_OK | MB_ICONHAND);
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

void StudentsLogin() {//学生登录
	ReadFromFile();
	AverageScore();
	int _bool = 0;
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("学生登录"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	wchar_t Tempaccount[40];
	wchar_t Temppassword[40];
	char account[20];
	char password[20];
	char InputAccount[20];
	char InputPassword[20];
	int i;
labelaccount:	
	Node* ptrTemp = head;
	i = InputBox(Tempaccount, 20, _T("请输入账号"), _T("学生登录"), NULL, 0, 0, false);
	if (i == true) {
		InputBox(Temppassword, 20, _T("请输入密码"), _T("学生登录"), NULL, 0, 0, true);
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
			MessageBox(hwnd, L"①检查你输入的账号或密码是否正确\n②确保老师已经录入你的信息", _T("账号或密码有误"), MB_OK | MB_ICONHAND);
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

void TeachersMenu() {//教师主菜单
	IMAGE img;
	initgraph(1200, 700);//宽、高
	loadimage(&img, _T("PNG"), _T("教师主菜单"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	labelStart:
	MOUSEMSG msg;// 定义变量，保存鼠标消息
	FlushMouseMsgBuffer();// 清空鼠标消息缓冲区
	while (1) { 
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 220 && msg.y <= 259) {
				//录入学生信息
				closegraph();
				InputInformation();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 287 && msg.y <= 324) {
				//删除学生信息
				closegraph();
				DeleteInformation();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 354 && msg.y <= 394) {
				//修改学生信息
				closegraph();
				ModifyInformation();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 423 && msg.y <= 462) {
				//查找学生信息
				closegraph();
				FindInformation();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 492 && msg.y <= 530) {
				//处理学生信息
				closegraph();
				ProcessingData();
				goto labelTeachersMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 1091 && msg.x <= 1192 && msg.y >= 632 && msg.y <= 652) {
				//修改密码
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
				j = InputBox(TempID, 20, _T("请输入您的账号及密码，确保为本人操作\n【请输入您的账号】"), _T("身份验证"), NULL, 0, 0, false);
				if (j == true) {
					int m;
					m = InputBox(TempPassword, 20, _T("【请输入您的旧密码】"), _T("身份验证"), NULL, 0, 0, false);
					if (m == true) {
						wcharTochar(TempID, InputID, 20);
						wcharTochar(TempPassword, InputPassword, 20);
						if (strcmp(InputID, accountTeachers) == 0 && strcmp(InputPassword, passwordTeachers) == 0) {
							InputBox(TempPassword, 20, _T("验证通过\n【请输入您的新密码】"), _T("修改密码"), NULL, 0, 0, true);
							wcharTochar(TempPassword, InputPassword, 20);
							fp = fopen("accountTeachers.txt", "w");
							fprintf(fp, "%s\n", accountTeachers);
							fprintf(fp, "%s", InputPassword);
							fclose(fp);
							MessageBox(hwnd, _T("修改成功，请妥善保管您的新密码"), _T("完成"), MB_OK);
						}
						else {
							MessageBox(hwnd, _T("账号或密码有误，无法通过验证"), _T("验证失败"), MB_OK | MB_ICONWARNING);
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
				//删除教师账户
				int i;
				i=MessageBox(hwnd, _T("确认删除您的账户吗？\n这会导致您以及您学生的所有信息完全被删除！"), _T("请谨慎操作"), MB_OKCANCEL| MB_ICONWARNING);
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
					j=InputBox(TempID, 20, _T("在进行销户操作之前\n需要您输入你的账号及密码，确保为本人操作\n【请输入您的账号】"), _T("身份验证"), NULL, 0, 0, false);
					if (j == true) {
						int m;
						m=InputBox(TempPassword, 20, _T("【请输入您的密码】"), _T("身份验证"), NULL, 0, 0, false);
						if (m == true) {
							wcharTochar(TempID, InputID, 20);
							wcharTochar(TempPassword, InputPassword, 20);
							if (strcmp(InputID, accountTeachers) == 0 && strcmp(InputPassword, passwordTeachers) == 0) {
								remove("StuInformation.txt");
								remove("accountTeachers.txt");
								MessageBox(hwnd, _T("您的账户已被删除"), _T("操作成功"), MB_OK);
								LoginInterface();
							}
							else {
								MessageBox(hwnd, _T("账号或密码有误，无法通过验证"), _T("验证失败"), MB_OK);
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
				//退出系统
				closegraph();
				goto labelTeachersMenu;
			}
		}
	}
labelTeachersMenu:
	return;
}

void StudentsMenu() {//学生主菜单
	time_t timep;
	struct tm* p;
	time(&timep);
	p = gmtime(&timep);
	int SystemHour = 8 + p->tm_hour;
	labelStart:
	Node* ptrTemp = head;
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("学生主菜单"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	wchar_t TempName[nameNum];
	wchar_t TempWelcome[20];//欢迎语
	char Welcome[20] = {"\0"};
	wchar_t TempMessage[100];
	while (ptrTemp != NULL) {
		if (strcmp(STUID, ptrTemp->students.ID) == 0) {
			break;
		}
		ptrTemp = ptrTemp->next;
	}
	if (SystemHour >= 1 && SystemHour <= 3) {
		strcpy(Welcome, "午夜好");
	}
	else if (SystemHour >= 4 && SystemHour <= 8) {
		strcpy(Welcome, "早上好");
	}else if (SystemHour >= 8 && SystemHour <= 10) {
		strcpy(Welcome, "上午好");
	}
	else if (SystemHour >= 11 && SystemHour <= 13) {
		strcpy(Welcome, "中午好");
	}
	else if (SystemHour >= 14 && SystemHour <= 18) {
		strcpy(Welcome, "下午好");
	}
	else if (SystemHour >= 19 && SystemHour <= 24) {
		strcpy(Welcome, "晚上好");
	}
	setbkmode(TRANSPARENT);
	settextcolor(RGB(0,191,255));
	settextstyle(20, 0, _T("等线"));
	charTowchar(Welcome, TempWelcome,20);
	charTowchar(ptrTemp->students.name, TempName, 20);
	_stprintf_s(TempMessage, _T("【%s同学，%s！^_^】"), TempName, TempWelcome);
	outtextxy(474, 196, TempMessage);
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 252 && msg.y <= 293) {
				//打印各科成绩
				closegraph();
				PrintResults();
				goto labelStudentsMenu;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 346 && msg.y <= 385) {
				//修改密码
				ModifySTUPassword();
				goto labelStart;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 723 && msg.y >= 438 && msg.y <= 476) {
				//退出系统
				closegraph();
				goto labelStudentsMenu;
			}
		}
	}
labelStudentsMenu:
	return;
}

void InputInformation() {//1-录入学生信息
	IMAGE img;
	initgraph(1200, 700, NOCLOSE);
	loadimage(&img, _T("PNG"), _T("录入学生信息"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
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
						printf("内存开辟失败！");
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
					_stprintf_s(title, _T("正在录入第 %d 位同学"), stucount + 1);
					i = InputBox(TempName, nameNum, _T("请输入【姓名】"), title, NULL, 0, 0, false);
					if (i == true) {
						wchar_t reminder[20];
						_stprintf_s(reminder, _T("请输入%s同学的【性别】"), TempName);
						InputBox(TempSex, 10, reminder, title, NULL, 0, 0, true);
						_stprintf_s(reminder, _T("请输入%s同学的【学号】"), TempName);
						InputBox(TempID, 40, reminder, title, NULL, 0, 0, true);
						_stprintf_s(reminder, _T("请输入%s同学的【语文成绩】"), TempName);
						InputBox(TempChinese, 10, reminder, title, NULL, 0, 0, true);
						_stprintf_s(reminder, _T("请输入%s同学的【数学成绩】"), TempName);
						InputBox(TempMath, 10, reminder, title, NULL, 0, 0, true);
						_stprintf_s(reminder, _T("请输入%s同学的【英语成绩】"), TempName);
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
						WriteToFile();//写入文件
						closegraph();
						TeachersMenu();
						goto labelInputInformation;
					}
				}
				
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 336 && msg.y <= 376) {
				WriteToFile();//写入文件
				closegraph();
				TeachersMenu();
				goto labelInputInformation;
			}
		}
	}
labelInputInformation:
	return;
}

void DeleteInformation() {//2-删除学生信息
	IMAGE img;
	initgraph(1200, 700, NOCLOSE);//宽、高
	loadimage(&img, _T("PNG"), _T("删除学生信息"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
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
			// 获取鼠标消息
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 250 && msg.y <= 290) {
				Node* _delete = head;
				Node* deleteTemp = head;
				int i;
				i = InputBox(TempDelete, 40, _T("请输入相应同学的学号"), _T("删除学生信息"), NULL, 0, 0, false);
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
							_stprintf_s(s, _T("找到学号为%s的%s同学"), TempID,TempName);
							n = MessageBox(hwnd, _T("确定要删除该同学所有信息吗？"), s, MB_OKCANCEL | MB_ICONWARNING);
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
								MessageBox(hwnd, _T("成功删除"), _T(" "), MB_OKCANCEL | MB_ICONWARNING);
								goto labelDeleteStart;
							}
							else {
								//取消删除
								//不执行任何指令
							}
						}
						_delete = _delete->next;
						if (tempcount >= 2) {
							deleteTemp = deleteTemp->next;
						}
					}
					if (_bool == 0) {
						MessageBox(hwnd, _T("请检查是否正确输入学号"), _T("未找到相应同学"), MB_OK| MB_ICONWARNING);
					}
					_bool = 0;
				}
				else if (i == false) {
					goto labelDelete;
				}
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 336 && msg.y <= 376) {
				WriteToFile();//写入文件
				goto labelDelete;
			}
		}
	}
labelDelete:
	closegraph();
	TeachersMenu();
}

void ModifyInformation() {//3-修改学生信息
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("修改学生信息"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
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
			i = InputBox(TempID, 40, _T("请输入相应同学的学号"), _T("修改学生信息"), NULL, 0, 0, false);
			wcharTochar(TempID, ID, 40);
			if (i == true) {
				while (modify != NULL) {
					if (strcmp(modify->students.ID, ID) == 0) {
						_bool = 1;
						wchar_t title[40];
						wchar_t temp[nameNum];
						charTowchar(modify->students.name, temp, nameNum);
						_stprintf_s(title, _T("已找到【%s】同学"), temp);//标题
						int define;
						define=MessageBox(hwnd, _T("确认修改此同学的信息吗？"), title, MB_OKCANCEL);
						if (define == true) {
							wchar_t NameTemp[nameNum];
							wchar_t SexTemp[nameNum];
							wchar_t IDTemp[nameNum];
							wchar_t ChineseTemp[nameNum];
							wchar_t MathTemp[nameNum];
							wchar_t EnglishTemp[nameNum];

							InputBox(NameTemp, nameNum, _T(" 请输入修改后的【姓名】"));
							InputBox(SexTemp, 10, _T(" 请输入修改后的【性别】"));
							InputBox(IDTemp, 40, _T(" 请输入修改后的【学号】"));
							InputBox(ChineseTemp, 10, _T(" 请输入修改后的【语文成绩】"));
							InputBox(MathTemp, 10, _T(" 请输入修改后的【数学成绩】"));
							InputBox(EnglishTemp, 10, _T(" 请输入修改后的【英语成绩】"));

							wcharTochar(NameTemp, modify->students.name, nameNum);
							wcharTochar(SexTemp, modify->students.sex, 10);
							wcharTochar(IDTemp, modify->students.ID, 40);
							modify->students.Chinese = _wtoi(ChineseTemp);
							modify->students.Math = _wtoi(MathTemp);
							modify->students.English = _wtoi(EnglishTemp);
							MessageBox(hwnd, _T("修改成功"), _T(" "), MB_OKCANCEL | MB_ICONWARNING);
							goto labelDeleteStart;
						}
					}
					else {
						modify = modify->next;
					}
				}
				if (_bool == 0) {
					MessageBox(hwnd, _T("请检查学号是否输入正确"), _T("未找到此同学"), MB_OK | MB_ICONWARNING);
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

void FindInformation() {//4-查找学生信息
	IMAGE img;
	initgraph(1200, 700);//宽、高
	loadimage(&img, _T("PNG"), _T("查找学生信息"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
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
			i = InputBox(_TempID, 40, _T("请输入相应同学的学号"), _T("查找学生信息"), NULL, 0, 0, false);
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
						_stprintf_s(title, _T("已找到学号为【%s】的同学"), TempID);
						_stprintf_s(content, _T("姓名:%s\n性别:%s\n学号:%s\n语文成绩:%d\n数学成绩:%d\n英语成绩:%d"), TempName, TempSex, TempID, ptrTemp->students.Chinese, ptrTemp->students.Math, ptrTemp->students.English);
						MessageBox(hwnd, content, title, MB_OK);
					}
					ptrTemp = ptrTemp->next;
				}
				if (_bool == 0) {
					MessageBox(hwnd, _T("请检查学号是否输入正确"), _T("未找到此同学"), MB_OK | MB_ICONWARNING);
				}
				_bool = 0;
			}
		}
		else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 475 && msg.x <= 724 && msg.y >= 336 && msg.y <= 376) {
			WriteToFile();//写入文件
			goto labelFind;
		}
	}
	}
labelFind:
	closegraph();
	TeachersMenu();
}

void ProcessingData() {//5-处理学生成绩
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("处理学生信息"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	labelStart:
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 220 && msg.y <= 259) {
				//打印成绩单
				closegraph();
				PrintResults();
				goto labelProcessingData;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 287 && msg.y <= 326) {
				//排序
				closegraph();
				Rank();
				goto labelProcessingData;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 354 && msg.y <= 394) {
				//分析成绩
				closegraph();
				AnalysisResults();
				goto labelProcessingData;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 423 && msg.y <= 460) {
				//重置学生密码
				ResetSTUPassword();
				goto labelStart;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 491 && msg.y <= 528) {
				//返回上一级
				closegraph();
				TeachersMenu();
				goto labelProcessingData;
			}
		}
	}
labelProcessingData:
	return;
}

void PrintResults() {//打印成绩单
	int _bool = 0;
	Node* ptrTemp = head;
	IMAGE img;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("成绩单"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	wchar_t TempInformation[100];
	setbkmode(TRANSPARENT);
	settextcolor(RGB(153,50,204));
	settextstyle(20, 0, _T("等线"));
	if (power == 'T') {
		outtextxy(18, 113, _T("    学号                   姓 名       性 别  语 文   数 学     英 语      总 分"));
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
					outtextxy(620, 113, _T("       学号                姓 名       性 别  语 文   数 学     英 语      总 分"));
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
		_stprintf_s(TempMessage, _T("%s同学，你的成绩如下，如有任何疑问，请及时联系老师"),TempName );
		_stprintf_s(TempScore, _T("%-20s%-10s %-5s %-10d %-10d %-10d %-10d"), TempID, TempName, TempSex, ptrTemp->students.Chinese, ptrTemp->students.Math, ptrTemp->students.English, sum);
		outtextxy(200, 110, TempMessage);
		outtextxy(200,140, _T("    学号                   姓 名       性 别  语 文   数 学     英 语      总 分"));
		outtextxy(200, 170, TempScore);
	}
labelStart:
	MOUSEMSG msg;
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit()) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 780 && msg.x <= 995 && msg.y >= 650 && msg.y <= 688) {
				//导出为网页
				if (power == 'T') {
					_bool = 0;
					ExportAsWebPage();
					goto labelStart;
				}
				else if (power == 'S') {
					_bool = 0;
					MessageBox(hwnd, _T("学生暂不支持导出成绩"), _T("无权限"), MB_OK | MB_ICONWARNING);
					goto labelStart;
				}
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 1019 && msg.x <= 1186 && msg.y >= 649 && msg.y <= 689) {
				//返回上一级
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

void ExportAsWebPage() {//将成绩单导出为网页
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
	fp = fopen("成绩单.html", "a+");
	fclose(fp);
	fp = fopen("成绩单.html", "w");
	fprintf(fp, "<!DOCTYPE html>\n");
	fprintf(fp, "<html lang=\"en\">\n");
	fprintf(fp, "<head>\n");
	fprintf(fp, "<meta charset=\"UTF - 8\">\n");
	fprintf(fp, "<title>成绩单</title>\n");
	fprintf(fp, "</head>\n");
	fprintf(fp, "<body>\n");
	fprintf(fp, "<hr/>\n");
	fprintf(fp, "<br/>\n");
	fprintf(fp, "<h1 align=\"center\" style=\"font - size:30pt\">学生成绩管理系统 2.0</h1>\n");
	fprintf(fp, "<hr/>\n");
	fprintf(fp, "<h2 align=\"center\" style=\"font - size:10pt\">导出时间：%d年%d月%d日%d时%d分%d秒</h2>\n", SystemYear, SystemMonth, SystemDay, SystemHour, SystemMin, SystemSec);
	fprintf(fp, "<br/>\n");
	fprintf(fp, "<table border=\"1\" align=\"center\" cellpadding=\"8\" cellspacing=\"0\">\n");
	fprintf(fp, "<tr align=\"center\" bgcolor=\"CC0000\">\n");
	fprintf(fp, "<th colspan=\"8\" style=\"color:white\">成 绩 单</th>\n");
	fprintf(fp, "<tr/>\n");
	fprintf(fp, "<tr align=\"center\" bgcolor=\"#FAEBD7\">\n");
	fprintf(fp, "<th rowspan=\"2\">序号</th>\n");
	fprintf(fp, "<th rowspan=\"2\">学号</th>\n");
	fprintf(fp, "<th rowspan=\"2\">姓名</th>\n");
	fprintf(fp, "<th rowspan=\"2\">性别</th>\n");
	fprintf(fp, "<th colspan=\"5\">各   科   分   数</th>\n");
	fprintf(fp, "</tr>\n");
	fprintf(fp, "<tr align=\"center\" bgcolor=\"#FAEBD7\">\n");
	fprintf(fp, " <th width=\"60\">语文</th>\n");
	fprintf(fp, "<th width=\"60\">数学</th>\n");
	fprintf(fp, "<th width=\"60\">英语</th>\n");
	fprintf(fp, "<th width=\"60\">总分</th>\n");
	fprintf(fp, "</tr>\n");
	while (ptrTemp != NULL) {
		fprintf(fp, "<tr align=\"center\" bgcolor=\"#FAEBD7\">\n");
		fprintf(fp, "<td>%d</td>\n", countTemp);//序号
		fprintf(fp, "<td>%s</td>\n",ptrTemp->students.ID);//学号
		fprintf(fp, "<td>%s</td>\n", ptrTemp->students.name);//姓名
		fprintf(fp, "<td>%s</td>\n", ptrTemp->students.sex);//性别
		fprintf(fp, "<td>%d</td>\n", ptrTemp->students.Chinese);//语文分数
		fprintf(fp, "<td>%d</td>\n", ptrTemp->students.Math);//数学分数
		fprintf(fp, "<td>%d</td>\n", ptrTemp->students.English);//英语分数
		fprintf(fp, "<td>%d</td>\n", ptrTemp->students.Chinese+ ptrTemp->students.Math+ ptrTemp->students.English);//总分
		fprintf(fp, "</tr>\n");
		ptrTemp = ptrTemp->next;
		countTemp++;
	}
	fprintf(fp, "</table>\n");
	fprintf(fp, "</body>\n");
	fprintf(fp, "</html>\n");
	fclose(fp);
	MessageBox(hwnd, _T("成绩单导出成功！"), _T("完成"), MB_OK);
}

void Rank() {//选择排序科目
	IMAGE img;
	initgraph(1200, 700);//宽、高
	loadimage(&img, _T("PNG"), _T("选择排序科目"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
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
				//语文
				i = InputBox(TempChinese, 5, _T("请输入1或2\n1 代表[升序]\n2 代表[降序]"), _T("选择【排序方式】"), NULL, 0, 0, false);
				if (i == true) {
					wcharTochar(TempChinese, Chinese, 5);
					RankChinese(Chinese);
					MessageBox(hwnd, _T("【语文】排序成功"),_T("消息"), MB_OK);
				}
				else {
					//不执行任何操作
				}
				ProcessingData();
				goto labelRank;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 287 && msg.y <= 326) {
				//数学
				i = InputBox(TempMath, 5, _T("请输入1或2\n1 代表[升序]\n2 代表[降序]"), _T("选择【排序方式】"), NULL, 0, 0, false);
				if (i == true) {
					wcharTochar(TempMath, Math, 5);
					RankMath(Math);
					MessageBox(hwnd, _T("【数学】排序成功"), _T("消息"), MB_OK);
				}
				ProcessingData();
				goto labelRank;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 354 && msg.y <= 394) {
				//英语
				i = InputBox(TempEnglish, 5, _T("请输入1或2\n1 代表[升序]\n2 代表[降序]"), _T("选择【排序方式】"), NULL, 0, 0, false);
				if (i == true) {
					wcharTochar(TempEnglish, English, 5);
					RankEnglish(English);
					MessageBox(hwnd, _T("【英语】排序成功"), _T("消息"), MB_OK);
				}
				ProcessingData();
				goto labelRank;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 422 && msg.y <= 460) {
				//总分
				i = InputBox(TempTotalScore, 5, _T("请输入1或2\n1 代表[升序]\n2 代表[降序]"), _T("选择【排序方式】"), NULL, 0, 0, false);
				if (i == true) {
					wcharTochar(TempTotalScore, TotalScore, 5);
					RankTotalScore(TotalScore);
					MessageBox(hwnd, _T("【总分】排序成功"), _T("消息"), MB_OK);
				}
				ProcessingData();
				goto labelRank;
			}
			else if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 476 && msg.x <= 724 && msg.y >= 490 && msg.y <= 528) {
				//返回上一级
				closegraph();
				ProcessingData();
				goto labelRank;
			}
		}
	}
labelRank:
	return;
}

void AverageScore() {//求各科平均分
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

void AnalysisResults() {//分析成绩
	AverageScore();
	IMAGE img;
	HWND hwnd;
	MOUSEMSG msg;
	initgraph(1200, 700);
	loadimage(&img, _T("PNG"), _T("分析成绩"), 1200, 700);
	putimage(0, 0, &img);
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	setbkmode(TRANSPARENT);
	settextcolor(RGB(30, 144, 255));
	settextstyle(20, 0, _T("等线"));
	//语文分段
	int Ch_1 = ClassificationByGrade(0, 59, 1);
	int Ch_2 = ClassificationByGrade(60, 75, 1);
	int Ch_3 = ClassificationByGrade(76, 89, 1);
	int	Ch_4 = ClassificationByGrade(90, 1000, 1);
	//数学分段
	int Ma_1 = ClassificationByGrade(0, 59, 2);
	int	Ma_2 = ClassificationByGrade(60, 75, 2);
	int	Ma_3 = ClassificationByGrade(76, 89, 2);
	int	Ma_4 = ClassificationByGrade(90, 1000, 2);
	//英语分段
	int En_1 = ClassificationByGrade(0, 59, 3);
	int	En_2 = ClassificationByGrade(60, 75, 3);
	int En_3 = ClassificationByGrade(76, 89, 3);
	int En_4 = ClassificationByGrade(90, 1000, 3);
	//设置绘图参数
	setfillstyle(BS_SOLID);// 填充样式
	setfillcolor(RGB(30 ,144 ,255));//填充颜色
	int parameter = 225;//改变此数值，可以控制柱状图的最大高度
	wchar_t numTemp[500];
	//绘制语文分数段统计图
	_stprintf_s(numTemp, _T("%d人"), Ch_1);
	solidrectangle(92, 340-(parameter * Ch_1/stucount), 125, 340);// 左上角坐标，右下角坐标
	outtextxy(92, 340 - (parameter * Ch_1 / stucount)-25, numTemp);//显示人数

	_stprintf_s(numTemp, _T("%d人"), Ch_2);
	solidrectangle(154, 340 - (parameter * Ch_2 / stucount), 185, 340);
	outtextxy(154, 340 - (parameter * Ch_2 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d人"), Ch_3);
	solidrectangle(216, 340 - (parameter * Ch_3 / stucount), 247, 340);
	outtextxy(216, 340 - (parameter * Ch_3 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d人"), Ch_4);
	solidrectangle(277, 340 - (parameter * Ch_4 / stucount), 307, 340);
	outtextxy(277, 340 - (parameter * Ch_4 / stucount) - 25, numTemp);

	//绘制数学分数段统计图
	_stprintf_s(numTemp, _T("%d人"), Ma_1);
	solidrectangle(440, 335 - (parameter * Ma_1 / stucount), 472, 335);
	outtextxy(440, 335 - (parameter * Ma_1 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d人"), Ma_2);
	solidrectangle(502, 335 - (parameter * Ma_2 / stucount), 534, 335);
	outtextxy(502, 335 - (parameter * Ma_2 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d人"), Ma_3);
	solidrectangle(564, 335 - (parameter * Ma_3 / stucount), 594, 335);
	outtextxy(564, 335 - (parameter * Ma_3 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d人"), Ma_4);
	solidrectangle(624, 335 - (parameter * Ma_4 / stucount), 656, 335);
	outtextxy(624, 335 - (parameter * Ma_4 / stucount) - 25, numTemp);

	//绘制英语分数段统计图
	_stprintf_s(numTemp, _T("%d人"), En_1);
	solidrectangle(777, 330 - (parameter * En_1 / stucount), 808, 330);
	outtextxy(777, 330 - (parameter * En_1 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d人"), En_2);
	solidrectangle(838, 330 - (parameter * En_2 / stucount), 870, 330);
	outtextxy(838, 330 - (parameter * En_2 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d人"), En_3);
	solidrectangle(901, 330 - (parameter * En_3 / stucount), 932, 330);
	outtextxy(901, 330 - (parameter * En_3 / stucount) - 25, numTemp);

	_stprintf_s(numTemp, _T("%d人"), En_4);
	solidrectangle(961, 330 - (parameter * En_4 / stucount), 993, 330);
	outtextxy(961, 330 - (parameter * En_4 / stucount) - 25, numTemp);

	settextcolor(RGB(0, 0, 0));//字体颜色
	_stprintf_s(numTemp, _T("共%d人有成绩记录,其中 语文平均分为%.2f  数学平均分为%.2f  英语平均分为%.2f"), Ch_1+ Ch_2+ Ch_3+ Ch_4, ChineseAverage,MathAverage,EnglishAverage);
	outtextxy(25, 460, numTemp);
	FlushMouseMsgBuffer();
	while (1) {
		while (MouseHit()) {
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN && msg.x >= 1019 && msg.x <= 1186 && msg.y >= 649 && msg.y <= 689) {
				//返回上一级
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

void RankChinese(char* way) {//按语文成绩排序
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

void RankMath(char *way) {//按数学成绩排序
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

void RankEnglish(char* way) {//按英语成绩排序
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

void RankTotalScore(char* way) {//按照总成绩排序
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

int ClassificationByGrade(int min,int max,int subject) {//按成绩对学生进行分类（统计指定科目各个分数段的学生个数，1-语文 2-数学 3-英语）
	Node* ptrTemp = head;
	int count = 0;
	if (subject == 1) {//统计语文
		while (ptrTemp != NULL) {
			if (ptrTemp->students.Chinese >= min && ptrTemp->students.Chinese <= max) {
				count++;
			}
			ptrTemp = ptrTemp->next;
		}
	}
	else if (subject == 2) {//统计数学
		while (ptrTemp != NULL) {
			if (ptrTemp->students.Math >= min && ptrTemp->students.Math <= max) {
				count++;
			}
			ptrTemp = ptrTemp->next;
		}
	}
	else if (subject == 3) {//统计英语
		while (ptrTemp != NULL) {
			if (ptrTemp->students.English >= min && ptrTemp->students.English <= max) {
				count++;
			}
			ptrTemp = ptrTemp->next;
		}
	}
	return count;
}

void SetTEAPassword() {//创建教师账户
	IMAGE img;
	initgraph(1200, 700);//宽、高
	loadimage(&img, _T("PNG"), _T("教师登录"), 1200, 700);
	putimage(0, 0, &img);
	HWND hwnd;
	hwnd = GetHWnd();
	SetWindowText(hwnd, _T("学生成绩管理系统 2.0"));
	MessageBox(hwnd, _T("现在为您创建一个可用的【教师】账户"), _T("创建账户"), MB_OK);
	wchar_t Temppassaccount[20];
	InputBox(Temppassaccount, 20, _T("请输入账号"), _T("创建【教师账户】"), NULL, 0, 0, true);
labelStart:	wchar_t Temppassword_one[20], Temppassword_two[20];
	InputBox(Temppassword_one, 20, _T("请输入密码"), _T("创建【教师账户】"), NULL, 0, 0, true);
	InputBox(Temppassword_two, 20, _T("请再次输入密码"), _T("创建【教师账户】"), NULL, 0, 0, true);
	char temp_one[20], temp_two[20];
	wcharTochar(Temppassword_one, temp_one, 20);
	wcharTochar(Temppassword_two, temp_two, 20);
	if (strcmp(temp_one, temp_two) != 0) {
		MessageBox(hwnd, _T("两次输入的密码不一致，请重新输入"), _T(""), MB_OK| MB_ICONEXCLAMATION);
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
	MessageBox(hwnd, _T("现在，您可以使用这个账户登录【教师端】了！"), _T("创建成功"), MB_OK);
	return;
}

void ModifySTUPassword() {//（学生）修改学生登录密码
	HWND hwnd;
	Node* ptrTemp;
	hwnd = GetHWnd();
	wchar_t TempOld[20];
	wchar_t TempNew[20];
	char Old[20];
	char New[20];
	int i;
	labelModifySTUPassword:
	i=InputBox(TempOld, 20, _T("请输入您的旧密码"), _T("修改登录密码"), NULL, 0, 0, false);
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
			j=InputBox(TempNew, 20, _T("请输入新的登录密码"), _T("设置新密码"), NULL, 0, 0, false);
			wcharTochar(TempNew, New, 20);
			if (j == true) {
				strcpy(ptrTemp->students.password, New);
				MessageBox(hwnd, _T("    修改成功√\n请妥善保管自己的密码"), _T("完成修改"), MB_OK);
				WriteToFile();
			}
		}
		else {
			MessageBox(hwnd, _T("您输入的旧密码有误，请重新输入"), _T("密码错误"), MB_OKCANCEL | MB_ICONERROR);
			goto labelModifySTUPassword;
		}
	}
}

void ResetSTUPassword() {//（教师）重置学生登录密码
	HWND hwnd;
	Node* ptrTemp=head;
	hwnd = GetHWnd();
	int i,j;
	wchar_t TempID[20];
	wchar_t TempName[20];
	wchar_t contentTemp[50];
	char ID[20];
	i = InputBox(TempID, 20, _T("请输入需要重置密码的学生的学号"),_T("重置学生登录密码"), NULL, 0, 0, false);
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
		_stprintf_s(contentTemp, _T("  【请再次核对信息】\n学号：%s\n姓名：%s"), TempID, TempName);
		j = MessageBox(hwnd, contentTemp, _T("确认重置吗？"), MB_OKCANCEL);
		if (j == true) {
			strcpy(ptrTemp->students.password, "123456");
			WriteToFile();
			MessageBox(hwnd, _T("重置成功，重置后的密码为123456，请通知学生及时修改"), _T("完成重置"), MB_OK);
		}
	}
}

void WriteToFile() {//将信息写入文件
	FILE* fp;
	Node* temp = head;
	fp = fopen("StuInformation.txt", "w");
	fprintf(fp,"学生个数：%d\n", stucount);
	while (temp != NULL) {
		fprintf(fp, "%s %s %s %d %d %d %s\n", temp->students.ID, temp->students.name, temp->students.sex, temp->students.Chinese, temp->students.Math, temp->students.English, temp->students.password);
		temp = temp->next;
	}
	fclose(fp);
}

void ReadFromFile() {//从文件读取信息
	FILE* fpTest;//初始化文件，防止第一次使用系统时，打开文件失败。
	fpTest = fopen("StuInformation.txt", "a+");
	fclose(fpTest);
	FILE* fp;
	int countTemp = 0,count=0;
	fp = fopen("StuInformation.txt", "r");
	fscanf(fp, "学生个数：%d", &stucount);
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

void wcharTochar(const wchar_t* wchar, char* chr, int length) {//将wchar转化为char
	WideCharToMultiByte(CP_ACP, 0, wchar, -1, chr, length, NULL, NULL);
}

void charTowchar(const char* chr, wchar_t* wchar, int size) {//将char转化为wchar
	MultiByteToWideChar(CP_ACP, 0, chr, strlen(chr) + 1, wchar, size / sizeof(wchar[0]));
}