#ifndef _USER
#define _USER

#include <iostream>;
#include <cstring>;
#include <cmath>;
#include <cstdio>;
#include "String.h"
#include "station.h"
using namespace std;

//��ѯ��Ʊ
String Query_ticket(Station s1, Station s2, String cata) {};
//��ѯ��ת��Ʊ
String Query_tranfer(Station s1, Station s2, String cata) {};

//������
class Order {
private:
	String tr_id;
	Station loc1;
	Station loc2;
	String Catalog;
};

//ע���û�
size_t Register(String s1, String s2, String s3, String s4) {};
//��½
bool Login(String s1, String s2) {};

//�û���
class User {
private:
	String name;
	String password;
	String email;
	String phone;
	size_t id;
	size_t privilege;
	Order order;
public:
	User() {};
	~User() {};
	//��ѯ�û���Ϣ
	String Query_profile() {};
	//�޸��û���Ϣ
	bool Modify_profile(String s1, String s2, String s3, String s4) {};
	//�޸��û�Ȩ��
	bool Modify_privilege(size_t i, size_t p) {};
	//����Ʊ
	bool Buy_ticket(size_t n, String s1, Station s2, Station s3, String s4) {};
	//��ѯ��Ʊ��Ϣ
	void Query_order(String cata) {};
	//�˶���Ʊ
	bool Refund_ticket(size_t n, String s1, Station s2, Station s3, String s4) {};
};

#endif
