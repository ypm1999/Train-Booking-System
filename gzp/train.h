#ifndef _TRAIN
#define _TRAIN

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
#include "String.h"
#include "station.h"
using namespace std;

bool Add_train(String s1, String s2, size_t n1, size_t n2, String s3) {};
//��Ʊ��
class Train {
private:
	String trainid;
	size_t num;
	String name;
	Station th[10];
	size_t ticket_type;
	size_t left_ticket[3];
	size_t price;
	bool pub;
public:
	Train() {};
	~Train() {};
	//��������
	bool Sale_train() {};
	//ɾ������
	bool Delete_train(String s) {};
	//��ѯ������Ϣ
	String Query_train(String s) {};
	//�޸ĳ���
	bool Modefy_train(String s1, size_t n1, size_t n2, String s2) {};
};

#endif
