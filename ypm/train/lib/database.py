#!/usr/bin/python3
# -*- coding:utf-8 -*-

from subprocess import Popen, PIPE, STDOUT

def db_init():
    db = Popen(['./train'], stdin = PIPE, stdout = PIPE)

db_init()

def db_write(cmd):
    db.stdin.write((cmd + '\n').encode())
    db.stdin.flush()

def db_readline():
    return db.stdout.readline().decode().strip('\n')

# if success return id
# else return -1
def register(name, password, email, phone):
    db.write(' '.join(['register', name, password, email, phone]))
    reply = db.readline()
    if reply == '-1':
        return {'userid': -1, 'success': False}
    userid = int(reply)
    return {'userid': userid, 'success': True}

# if success return 1
# else return 0
def try_login(userid, password):
    db.write(' '.join(['login', userid, password]))
    reply = db.readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# return user profiles:
#	*name*, *email*, *phone*, *privilege*
# if cannot find return 0
def query_profile(userid):
    db.write(' '.join(['query_profile', userid]))
    reply = db.readline()
    if reply == '0':
        return {'success': False}
    tmp = reply.split(' ')
    return {'name': tmp[0], 'email': tmp[2], 'phone': tmp[3], 'admin': (tmp[4] == '1'), 'success': True}

# if success return 1
# else return 0
def modify_profile(userid, name, password, email, phone):
    db.write(' '.join(['modify_profile', name, password, email, phone]))
    reply = db.readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# user *id1* want to change user *id2*'s privilege to *privilege*
# if success return 1
# else return 0
def modify_privilege(id1, id2, privilege):
    db.write(' '.join(['modify_privilege', id1, id2, privilege]))
    reply = db.readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

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
