#!/usr/bin/python3
# -*- coding:utf-8 -*-

from subprocess import Popen, PIPE, STDOUT

db = Popen(['./train'], stdin = PIPE, stdout = PIPE)

def db_write(cmd):
    db.stdin.write((cmd + '\n').encode())
    db.stdin.flush()

def db_readline():
    return db.stdout.readline().decode().strip('\n')

# if success return id
# else return -1
def register(name, password, email, phone):
    db_write(' '.join(['register', name, password, email, phone]))
    reply = db_readline()
    if reply == '-1':
        return {'user_id': -1, 'success': False}
    user_id = int(reply)
    return {'user_id': user_id, 'success': True}

# if success return 1
# else return 0
def try_login(user_id, password):
    db_write(' '.join(['login', user_id, password]))
    reply = db_readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# return user profiles:
#   *name*, *email*, *phone*, *privilege*
# if cannot find return 0
def query_profile(user_id):
    db_write(' '.join(['query_profile', user_id]))
    reply = db_readline()
    if reply == '0':
        return {'success': False}
    tmp = reply.split(' ')
    return {'name': tmp[0], 'email': tmp[1], 'phone': tmp[2], 'admin': int(tmp[3]), 'success': True}

# if success return 1
# else return 0
def modify_profile(user_id, name, password, email, phone):
    db_write(' '.join(['modify_profile', user_id, name, password, email, phone]))
    reply = db_readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# user *id1* want to change user *id2*'s privilege to *privilege*
# if success return 1
# else return 0
def modify_privilege(id1, id2, privilege):
    db_write(' '.join(['modify_privilege', id1, id2, privilege]))
    reply = db_readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# TODO unknown bug
# cannot find train
# return a list contains trains
# from *loc1* to *loc2* at *date* with *catalog*
# without transfer
# return -1 if query is illegal
def query_ticket(loc1, loc2, date, catalog):
    # print(' '.join(['query_ticket', loc1, loc2, date, catalog]))
    db_write(' '.join(['query_ticket', loc1, loc2, date, catalog]))
    reply_lines = int(db_readline())
    if reply_lines == -1:
        return {'success': False}
    print(reply_lines)
    trains = []
    for i in range(reply_lines):
        reply = db_readline()
        trains.append(reply.split(' '))
    return {'trains': trains, 'success': True}

# TODO test functions below

# return (a list of) two trains can carry you
# from *loc1* to *loc2* at *date* with *catalog*
# total time (including waitng transfer) is shortest
# return -1 if query is illegal
def query_transfer(loc1, loc2, date, catelog):
    db_write(' '.join(['query_transfer', loc1, loc2, date, catelog]))
    reply = db_readline()
    if reply == '-1':
        return {'success': False}
    trains = [reply.split(' ')]
    trains.append(db_readline().split(' '))
    return {'trains': trains, 'success': True}

# user *id* buy *num* ticket(s) of *ticket_kind* in *train_id* from *loc1* to *loc2* at *date*
# return 1 if success
# return 0 if fail
def buy_ticket(user_id, num, train_id, loc1, loc2, date, ticket_kind):
    db_write(' '.join(['buy_ticket', user_id, num, train_id, loc1, loc2, date, ticket_kind]))
    reply = db_readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# return a list contains tickets
# bought by user *id* with *catalog* at *date*
# return -1 if query is illegal
def query_order(user_id, date, catalog):
    db_write(' ',join(['query_order', user_id, date, catalog]))
    reply_lines = int(db_readline())
    if reply_lines == -1:
        return {'success': False}
    tickets = []
    for i in range(reply_lines):
        tickets.append(db_readline().split(' '))
    return {'tickets': tickets, 'success': True}

# user *id* refund *num* ticket(s) of *ticket_kind* in *train_id* from *loc1* to *loc2* at *date*
# return 1 if success
# return 0 if fail
# mirror of buy_ticket
def refund_ticket(user_id, num, train_id, loc1, loc2, date, ticket_kind):
    db_write(' '.join(['refund_ticket', user_id, num, train_id, loc1, loc2, date, ticket_kind]))
    reply = db_readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# add a train but do not sale
# e.g.
# >>> add_train abc123456 G123456 G 2 1 商务座
# >>> 北京 xx:xx 08:00 00:00 ￥0.0
# >>> 夏威夷 08:01 xx:xx 00:00 ￥1.5

# return 1 if success
# return 0 if fail
def add_train(train_id, name, catalog, station_list, ticket_kind_list):
    db_write(' '.join(['add_train', train_id, name, catalog, str(len(station_list)), str(len(ticket_kind_list))] + ticket_kind_list))
    for x in station_list:
        db_write(' '.join(x))
    reply = db_readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# modify info of train_id like add_train
# return 1 if success
# return 0 if fail
# copy from add_train
def modify_train(train_id, name, catalog, station_list, ticket_kind_list):
    db_write(' '.join(['modify_train', train_id, name, catalog, len(station_list), len(ticket_kind_listk)] + [ticket_kind_list]))
    for x in station_list:
        db_write(' '.join(x))
    reply = db_readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# open sale for train_id
# and this train can on longer be modify or delete
# return 1 if success
# return 0 if fail (i.e. already on sale or doesn't exist)
def sale_train(train_id):
    db_write(' '.join(['sale_train', train_id]))
    reply = db_readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

# return info like add_train
# return 0 if fail (not exist or not on sale)
def query_train(train_id):
    db_write(' '.join(['query_train', train_id]))
    reply = db_readline()
    if reply == '0':
        return {'success': False}
    print(reply)
    train_id = reply.split()[0]
    name = reply.split()[1]
    catalog = reply.split()[2]
    station_num = int(reply.split()[3])
    ticket_kind_num = int(reply.split()[4])
    ticket_kind_list = reply.split()[5:]
    station_list = []
    for i in range(station_num):
        station_list.append(db_readline().split())

    return {'train_id': train_id,
            'name': name,
            'catalog': catalog,
            'station_list': station_list,
            'ticket_kind_list': ticket_kind_list,
            'success': True}

# delete *train_id*
# return 1 if success
# return 0 fail
# eg: delete_train("abc123456")
def delete_train(train_id):
    db_write(' '.join(['delete_train', train_id]))
    reply = db_readline()
    if reply == '1':
        return {'success': True}
    else:
        return {'success': False}

if __name__ == '__main__':
    print(register("lyx", "123", "qaq", "qqq"))
    print(register("lyc", "123", "qaq", "qqq"))
    print(register("lyc", "123", "qaq", "qqq"))
    print(add_train('xiaohuoche', 'littletrain', 'CDEF', [['菜鸡站', '7:34', '08:00', '00:01', '￥0.0', '￥0.0'], ['脑残站', '08:02', '12:00', '00:00', '￥1.5', '￥3.0']], ['普通的票', 'VIP']))
    print(add_train('dahuoche', 'bigtrain', 'CDEF', [['A', '7:34', '08:00', '00:00', '￥0.0', '￥0.0'], ['B', '08:01', '12:00', '00:00', '￥1.5', '￥3.0']], ['普通的票', 'VIP']))
    print(sale_train('xiaohuoche'))
    print(sale_train('littletrain'))
    print(sale_train('dahuoche'))
    print(query_train('xiaohuoche'))
    print(query_train('dahuoche'))
    print(query_ticket('菜鸡站', '脑残站', '2018-06-01', 'C'))
    print(buy_ticket('2018', '1', 'xiaohuoche', '菜鸡站', '脑残站', '2018-06-01', 'C'))
    print(query_ticket('A', 'B', '2018-06-05', 'CD'))
    #print(query_transfer())

