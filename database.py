#!/usr/bin/python3
# -*- coding:utf-8 -*-

import fileinput
import sys

nxt_id = 2018

# if success return id
# else return -1
def register(name, password, email, phone):
    f = open('user.tmp', "a");
    f.write('%d ' % nxt_id)
    f.write(name + ' ')
    f.write(password + ' ')
    f.write(email + ' ')
    f.write(phone + ' ')
    f.write('0\n')
    f.close();
    nxt_id += 1
    return True

# if success return 1
# else return 0
def try_login(username, password):
    f = open('user.tmp', "r")
    while(True):
        line = f.readline()
        if not line:
            break
    temp = line.split()
    print(temp[0], temp[1])
    print(username == temp[0])
    print(password == temp[1])
    if temp[0] == username and temp[1] == password:
        return True
    return False

# return user profiles:
#	*name*, *email*, *phone*, *privilege*
# if cannot find return 0
def query_profile(username):
    f = open('user.tmp', "r")
    while True:
        line = f.readline()
        if not line:
            break
    temp = line.split();
    if temp[0] == username:
        return {'name':temp[0], 'password':temp[1], 'email':temp[2], 'phone':temp[3], 'admin':(temp[4] == '1'), 'sucessed':True}
    return {'sucessed':False}

# if success return 1
# else return 0
def modify_profile(id, name, password, email, phone):
    for line in fileinput.input('user.tmp', inplace = True):
        num = int(line.split(' ')[0])
        pri = int(line.split(' ')[5])
        if num == id:
            print(id, name, password, email, phone, pri)
        else:
            print(line, end='')
    else:
        return False
    return True

# user *id1* want to change user *id2*'s privilege to *privilege*
# if success return 1
# else return 0
def modify_privilege(id1, id2, privilege):
    f = fopen('user.tmp', 'r')
    lines = f.readlines()
    f.close()
    for line in lines:
        num = int(line.split(' ')[0])
        pri = int(line.split(' ')[5])
        if num == id1:
            if pri == 0:
                return False # no privilege
            else:
                break
    else:
        return False # cannot find id1

    for line in fileinput.input('user.tmp', inplace = True):
        num = int(line.split(' ')[0])
        pri = int(line.split(' ')[5])
        if num == id2:
            print(id2, name, password, email, phone, privilege)
        else:
            print(line, end='')
    else:
        return False
    return True

# return a list contains trains
# from *loc1* to *loc2* at *date* with *catalog*
# without transfer
# return -1 if query is illegal
def query_ticket(loc1, loc2, date, catalog):
    pass

# return two trains can carry you
# from *loc1* to *loc2* at *date* with *catalog*
# total time (including waitng transfer) is shortest
# return -1 if query is illegal
def query_transfer(loc1, loc2, date, catelog):
    pass

# user *id* buy *num* ticket(s) of *ticket_kind* in *train_id* from *loc1* to *loc2* at *date*
# return 1 if success
# return 0 if fail
def buy_ticket(id, num, train_id, loc1, loc2, date, ticket_kind):
    pass

# return a list contains tickets
# bought by user *id* with *catalog* at *date*
# return -1 if query is illegal
def query_order(id, date, catalog):
    pass

# user *id* refund *num* ticket(s) of *ticket_kind* in *train_id* from *loc1* to *loc2* at *date*
# return 1 if success
# return 0 if fail
# mirror of buy_ticket
def refund_ticket(id, num, train_id, loc1, loc2, date, ticket_kind):
    pass

# too complicated...
# e.g. if you input
# >>> add_train abc123456 G123456 G 2 1 商务座
# >>> 北京 xx:xx 08:00 00:00 ￥0.0
# >>> 夏威夷 08:01 xx:xx 00:00 ￥1.5
# then train_id = 'abc123456'
# name = 'G123456'
# catalog = 'G'
# station_num = 2
# ticket_list = ['商务座']
# station_list = [['北京', time X 3, 0.0], ['夏威夷', time X 3, 1.5]]
# return 1 if success
# return 0 if fail
def add_train(train_id, name, catalog, station_list, ticket_kind_list):
    pass

# modify info of train_id like add_train
# return 1 if success
# return 0 if fail
def modify_train(train_id, name, catalog, station_num, ticket_num, station_list, ticket_kind_list):
    pass

# open sale for train_id
# and this train can on longer be modify or delete
# return 1 if success
# return 0 if fail (i.e. already on sale or doesn't exist)
def sale_train(train_id):
    pass

# return info like add_train
def query_train(train_id):
    pass

# delete *train_id*
# return 1 if success
# return 0 fail
# eg: delete_train("abc123456")
def delete_train(train_id):
    pass
