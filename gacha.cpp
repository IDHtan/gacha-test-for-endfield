#include<iostream>
#include<iomanip>
#include <string>
#include<vector>
#include<ctime>
#include<random>
#include <windows.h>
#define white 15
#define grey 8
#define trial 100000
#define Urate 0.5f
#define Wrate 0.25f
using namespace std;

//float rate6 = 0.008, rate5 = 0.08, rate4 = 0.912, rateU = 0.5, /*rate6W = 0.04,*/ rate6UW = 0.25;

HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

void setColor(int color) {
    SetConsoleTextAttribute(hConsoleOut, color);
}

bool boolRandom(float probability) {
    static default_random_engine e(random_device{}());
    bernoulli_distribution u(probability);      //生成1的概率
    return u(e);
}

int intRandom(int max) {
    static default_random_engine e(random_device{}());
    uniform_int_distribution<int> u(1, max);    //左闭右闭区间
    return u(e);
}

void gacha(int& Grate, int& N6count, int& N5count, int& Wcount) {
    int dice=intRandom(1000);
    if (N5count == 9) {
        if (dice <= Grate) {    //6
            N6count = 0; N5count = 0; Wcount += 2000; Grate = 8;
        }
        else {  //5
            N6count++; N5count = 0; Wcount += 200;
        }
        return;
    }
    if(N6count==79) {   //6
        N6count = 0; N5count = 0; Wcount += 2000; Grate = 8;
        return;
	}
    if (N6count >= 65) {    //概率提升
        Grate += 50;
    }
	if (dice <= Grate) {    //6
        N6count = 0; N5count = 0; Wcount += 2000; Grate = 8;
    }
	else if (dice <= Grate + 80) {  //5
        N6count++; N5count = 0; Wcount += 200;
    }
    else {  //4
        N6count++; N5count++; Wcount += 20;
    }
}

void gachaForGold(vector<int>& oneG, vector<int>& oneGW) {
    static int N6count = 0, N5count = 0, Grate=8;           //未金次数，未黄次数, 出率
    int count = 0, Wcount = 0;                              //抽卡次数，武库配额数
    static bool sixty = false;                              //寻访情报书
    bool thirty = true;
    bool reached = false;
    if (sixty) {    //寻访情报书
        for(int i=0;i<10;i++) {
            gacha(Grate, N6count, N5count, Wcount);
            if (N6count == 0) {
                reached = true;
            }
		}
        sixty = false;
    }
    while (!reached) {
        if (count == 30 && thirty) {
            for (int i = 0; i < 10; i++) {
                int _ = 8, __ = 0, ___ = 0;
                gacha(_, __, ___, Wcount);
                if (__ == 0) {
                    reached = true;
                }
            }
            thirty = false;
            continue;
        }
        gacha(Grate, N6count, N5count, Wcount);
        count++;
        if (N6count == 0) {
            reached = true;
        }
    }
    oneG.push_back(count);
    oneGW.push_back(Wcount); 
    if (count >= 60) sixty = true;
}

void gachaForUp(vector<int>& oneU, vector<int>& oneUW) {
    static int N6count = 0, N5count = 0, Grate = 8;           //未金次数，未黄次数, 出率
    int count = 0, Wcount = 0;                                //抽卡次数，武库配额数
    static bool sixty = false;                                //寻访情报书
    bool thirty = true;
    bool reached = false;
    if (sixty) {    //寻访情报书
        for (int i = 0; i < 10; i++) {
            gacha(Grate, N6count, N5count, Wcount);
            if (N6count == 0) {
                if (boolRandom(Urate)) {
                    reached = true;
                }
            }
        }
        sixty = false;
    }
    while (!reached && count < 119) {
        if (count == 30 && thirty) {
            int _ = 8, __ = 0, ___ = 0;
            for (int i = 0; i < 10; i++) {
                gacha(_, __, ___, Wcount);
                if (__ == 0) {
                    if (boolRandom(Urate)) {
                        reached = true;
                    }
                }
            }
            thirty = false;
            continue;
        }
        gacha(Grate, N6count, N5count, Wcount);
        count++;
        if (N6count == 0) {
            if (boolRandom(Urate)) {
                reached = true;
            }
        }
    }
    if (count == 119) {
        count++;
		N6count = 0; N5count = 0; Wcount += 2000; Grate = 8; reached = true;
    }
    oneU.push_back(count);
    oneUW.push_back(Wcount);
    if (count >= 60) sixty = true;
}

void gachaForFullUp(vector<int>& fullU, vector<int>& fullUW) {
    static int N6count = 0, N5count = 0, Grate = 8;           //未金次数，未黄次数, 出率
    int count = 0, Wcount = 0, Ucount=0;                      //抽卡次数，武库配额数，Up个数
    static bool sixty = false;                                //寻访情报书
    bool thirty = true;
    if (sixty) {    //寻访情报书
        for (int i = 0; i < 10; i++) {
            gacha(Grate, N6count, N5count, Wcount);
            if (N6count == 0) {
                if (boolRandom(Urate)) {
                    Ucount++;
                }
            }
        }
        sixty = false;
    }
    while (Ucount<6) {
        if(count==119 && Ucount==0) { 
            count++; Ucount++;
            N6count = 0; N5count = 0; Wcount += 2000; Grate = 8;
            continue;
		}
        if (count == 30 && thirty) {
            int _ = 8, __ = 0, ___ = 0;
            for (int i = 0; i < 10; i++) {               
                gacha(_, __, ___, Wcount);
                if (__ == 0) {
                    if (boolRandom(Urate)) {
                        Ucount++;
                    }
                }
            }
            thirty = false;
            continue;
        }
        gacha(Grate, N6count, N5count, Wcount);
        count++;
        if (N6count == 0) {
            if (boolRandom(Urate)) {
                Ucount++;
            }
        }
        if (count % 240 == 0) Ucount++;
    }
    fullU.push_back(count);
    fullUW.push_back(Wcount);
    if (count >= 60) sixty = true;
}

void gachaForWeapon(vector<int>& weaponG,vector<int>& weaponU, vector<int>& weaponFU) {//一次申领是十抽
    int N6count = 0, NUcount = 0, Ucount = 0, count = 0;//未金次数，未Up次数，Up个数，申领次数
    const static int Grate = 4;                         //出率
    int dice;
    bool G=false, U=false, gate1 = true, gate2 = true;
    while (Ucount < 6) {
        count++;
        for(int i=0;i<9;i++) {              //前九抽
            dice = intRandom(100);
            if (dice <= Grate) {
                G = true;
                if (boolRandom(Wrate)) {
                    Ucount++;
                    U = true;
                }
            }
		}
		if (NUcount == 7) {                 //最后一抽，受保底影响
            Ucount++;
			U = true; G = true;
        }
        else if (N6count == 3) {
            G = true;
            if (boolRandom(Wrate)) {
                Ucount++;
                U = true;
			}
        }
        else {
            dice = intRandom(100);
            if (dice <= Grate) {
                G = true;
                if (boolRandom(Wrate)) {
                    Ucount++;
                    U = true;
                }
            }
        }
        if (gate1 && G) {
            weaponG.push_back(count);
			gate1 = false;
        }
        if(gate2 && U){
            weaponU.push_back(count);
            gate2 = false;
		}
        if (U) {
            NUcount = 0;
            U = false;
        }
        else {
            NUcount++;
        }
        if (G) {
			N6count = 0;
			G = false;
        }
        else {
            N6count++;
        }
        if ((count - 18) % 16 == 0) Ucount++;
    }
    weaponFU.push_back(count);
}

int main() {
    cout << left;
    cout << "以下为明日方舟：终末地抽数模拟\n";
    cout << "假设有三个人以各自的目标条件进行抽卡，分别是：出金、出UP、满潜UP，条件达成后进入下一个卡池\n";
    cout << "第一次无寻访情报书，寻访情报书不计入抽数，最先抽，所以如果用了0抽那就是寻访情报书10连出的\n";
    cout << "免费十连不计入抽数\n";
    cout << "统计100000次的平均抽数\n";
	cout << "抽卡的同时统计武库配额数量，用灰色字体展示\n";
	cout << "抽卡规则参照游戏内\n\n";
	system("pause");

    cout << setw(10) << ' ' << setw(10) << "出金数" << setw(10) << "出UP数" << setw(10) << "满潜数" << endl;

    long long sumOneG = 0, sumOneU = 0, sumFullU = 0, sumOneGW = 0, sumOneUW = 0, sumFullUW = 0;
    vector<int> oneG, oneU, fullU, oneGW, oneUW, fullUW;
	long long sumWeaponG = 0, sumWeaponU = 0, sumWeaponFU = 0;
    vector<int> weaponG, weaponU, weaponFU;
    oneG.reserve(trial);
    oneU.reserve(trial);
    fullU.reserve(trial);
    oneGW.reserve(trial);
    oneUW.reserve(trial);
    fullUW.reserve(trial);
    weaponG.reserve(trial);
    weaponU.reserve(trial);
    weaponFU.reserve(trial);
    
    for (int i = 1; i <= trial; i++) {
        gachaForGold(oneG, oneGW);
        gachaForUp(oneU, oneUW);
        gachaForFullUp(fullU, fullUW);
		gachaForWeapon(weaponG, weaponU, weaponFU);
        sumOneG += oneG.back(), sumOneU += oneU.back(), sumFullU += fullU.back();
        sumOneGW += oneGW.back(), sumOneUW += oneUW.back(), sumFullUW += fullUW.back();
		sumWeaponG += weaponG.back(), sumWeaponU += weaponU.back(), sumWeaponFU += weaponFU.back();
        setColor(white);
        cout << setw(14) << ("第" + std::to_string(i) + "次:") << setw(10) << oneG.back() << setw(10) << oneU.back() << setw(10) << fullU.back() << endl;
        setColor(grey);
        cout << setw(14) << ' ' << setw(10) << oneGW.back() << setw(10) << oneUW.back() << setw(10) << fullUW.back() << endl;
    }

    double avgOneG = static_cast<double>(sumOneG) / trial;
    double avgOneU = static_cast<double>(sumOneU) / trial;
    double avgFullU = static_cast<double>(sumFullU) / trial;
    double avgOneGW = static_cast<double>(sumOneGW) / trial;
    double avgOneUW = static_cast<double>(sumOneUW) / trial;
    double avgFullUW = static_cast<double>(sumFullUW) / trial;
    double avgWeaponG = static_cast<double>(sumWeaponG) / trial;
    double avgWeaponU = static_cast<double>(sumWeaponU) / trial;
    double avgWeaponFU = static_cast<double>(sumWeaponFU) / trial;
    setColor(white);
    cout << fixed << setprecision(2);
    cout << "经过十万次试验\n平均出金抽数          " << setw(5) << avgOneG << setw(18) <<
        "，出UP抽数  " << setw(5) << avgOneU << setw(18) <<
        "，满潜抽数  " << setw(5) << avgFullU << endl;
    cout << "所获武库配额数        " << setw(5) << avgOneGW << setw(18) <<
        "，" << setw(5) << avgOneUW << setw(18) <<
        "，" << setw(5) << avgFullUW << endl;
    cout << "武器池出金申领次数    " << setw(5) << avgWeaponG << setw(18) <<
        "，出UP申领次数" << setw(5) << avgWeaponU << setw(18) <<
        "，满潜申领次数" << setw(5) << avgWeaponFU << endl;
    cout << "对应武库配额数        " << setw(5) << avgWeaponG * 1980 << setw(18) <<
        "，" << setw(5) << avgWeaponU * 1980 << setw(18) <<
        "，" << setw(5) << avgWeaponFU * 1980 << endl;
    system("pause");
    return 0;
}

//#include<iostream>
//#include<iomanip>
//using namespace std;
//
//int main() {
//    // 模拟平均数值（基于典型抽卡模拟结果）
//    const double avgOneG = 34.52;      // 角色池平均出金抽数
//    const double avgOneU = 67.83;      // 角色池平均出UP抽数  
//    const double avgFullU = 312.45;    // 角色池平均满潜抽数
//
//    const double avgOneGW = 3452.0;    // 出金所获武库配额（估算）
//    const double avgOneUW = 6783.0;    // 出UP所获武库配额（估算）
//    const double avgFullUW = 31245.0;  // 满潜所获武库配额（估算）
//
//    const double avgWeaponG = 2.50;    // 武器池平均出金申领次数（约25抽）
//    const double avgWeaponU = 4.20;    // 武器池平均出UP申领次数（约42抽）
//    const double avgWeaponFU = 18.35;  // 武器池平均满潜申领次数（约183抽）
//
//    cout << left;  // 保持与你原代码一致的对齐方式
//
//    // 测试输出格式
//    cout << "经过十万次试验\n平均出金抽数          " << setw(5) << avgOneG << setw(18) <<
//        "，出UP抽数  " << setw(5) << avgOneU << setw(18) <<
//        "，满潜抽数  " << setw(5) << avgFullU << endl;
//    cout << "所获武库配额数        " << setw(5) << avgOneGW << setw(18) <<
//        "，" << setw(5) << avgOneUW << setw(18) <<
//        "，" << setw(5) << avgFullUW << endl;
//    cout << "武器池出金申领次数    " << setw(5) << avgWeaponG << setw(18) <<
//        "，出UP申领次数" << setw(5) << avgWeaponU << setw(18) <<
//        "，满潜申领次数" << setw(5) << avgWeaponFU << endl;
//    cout << "对应武库配额数        " << setw(5) << avgWeaponG * 1980 << setw(18) <<
//        "，" << setw(5) << avgWeaponU * 1980 << setw(18) <<
//        "，" << setw(5) << avgWeaponFU * 1980 << endl;
//
//    return 0;
//}