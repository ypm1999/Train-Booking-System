#ifndef "User_hpp"
#define "User_hpp"
#include<fstream>
#include<iostream>
#include<bptree.cpp>

class user_data
{
public:
    string Name;
    string Password;
    string Email;
    string Phone;
    int Privilege;
    user_data() = default;
    user_data(char* a, char* b, char* c, char* d, int e = 1): Name(a), Password(b), Email(c), Phone(d), Privilege(e) {}
    user_data(const user_data& o): Name(o.Name),Password(o.Password), Email(o.Email), Phone(o.Phone), Privilege(o.Privilege) {}
    user_data operator = (const user_data& o): Name(o.Name), Password(o.Password), Email(o.Email), Phone(o.Phone), Privilege(o.Privilege)
    {return *this;}
};
class user
{
private:
    BPtree<int, user_data> Root;
    static int _Cur_Id;
public:
    user()
    {
        Root = Bptree<int, user_data>("_User_Data");
    }
    int Register(char* a, char* b, char* c, char* d)
    {
        user_data u(a, b, c, d);
        Root.insert(_Cur_Id, u);
        return _Cur_Id++;
    }
    int login(int id, char* name)
    {
        if(Root.query().second == true) return 1;
        return 0;
    }
    user_data query_profile(int id)
    {
        return Root.query(id).first;
    }
    int modify_profile(int id, char* a, char* b, char* c, char* d)
    {
        user_data u(a, b, c, d, Root.query().first.Privilege);
        Root.modify(id, u);
        return 1;
    }
    int modify_privilege(int id1, int id2, int privilege)
    {
        user_data u(_Root.query(id2).first);
        u.Privilege = privilege;
        Root.modify(id, u);
        return 1;
    }
};
#endif
