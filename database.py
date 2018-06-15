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
        return None
    return reply

# if success return 1
# else return 0
def try_login(user_id, password):
    db_write(' '.join(['login', user_id, password]))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

# return user profiles:
#   *name*, *email*, *phone*, *privilege*
# if cannot find return 0
def query_profile(user_id):
    db_write(' '.join(['query_profile', user_id]))
    reply = db_readline()
    if reply == '0':
        return None
    tmp = reply.split(' ')
    return {'name': tmp[0], 'email': tmp[1], 'phone': tmp[2], 'admin': int(tmp[3]) == 2}

# if success return 1
# else return 0
def modify_profile(user_id, name, password, email, phone):
    db_write(' '.join(['modify_profile', user_id, name, password, email, phone]))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

# user *id1* want to change user *id2*'s privilege to *privilege*
# if success return 1
# else return 0
def modify_privilege(id1, id2, privilege):
    db_write(' '.join(['modify_privilege', id1, id2, privilege]))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

# return a list contains trains
# from *loc1* to *loc2* at *date* with *catalog*
# without transfer
# return -1 if query is illegal
def _add(trains, reply):
    reply = reply.split(' ')
    key = ['train_id', 'loc1', 'date1', 'time1', 'loc2', 'date2', 'time2']
    value = reply[:7]

    sz = len(reply)
    for i in range(7, sz, 3):
    	if (reply[i] == '商务座'):
    		reply[i] = '特等座'
    	key.append(reply[i])
    	value.append((reply[i + 1], reply[i + 2]))

    train = dict(zip(key, value))

    trains.append(train)

def query_ticket(loc1, loc2, date, catalog):
    db_write(' '.join(['query_ticket', loc1, loc2, date, catalog]))
    reply_lines = int(db_readline())
    if reply_lines == -1:
        return None
    trains = []
    for i in range(reply_lines):
        _add(trains, db_readline())
    return trains

# return (a list of) two trains can carry you
# from *loc1* to *loc2* at *date* with *catalog*
# total time (including waitng transfer) is shortest
# return -1 if query is illegal
def query_transfer(loc1, loc2, date, catelog):
    db_write(' '.join(['query_transfer', loc1, loc2, date, catelog]))
    reply = db_readline()
    if reply == '-1':
        return None
    trains = []
    _add(trains, reply)
    _add(trains, db_readline())
    return trains

# user *id* buy *num* ticket(s) of *ticket_kind* in *train_id* from *loc1* to *loc2* at *date*
# return 1 if success
# return 0 if fail
def buy_ticket(user_id, num, train_id, loc1, loc2, date, ticket_kind):
    db_write(' '.join(['buy_ticket', user_id, num, train_id, loc1, loc2, date, ticket_kind]))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

# return a list contains tickets
# bought by user *id* with *catalog* at *date*
# return -1 if query is illegal
def query_order(user_id, date, catalog):
    db_write(' '.join(['query_order', user_id, date, catalog]))
    reply_lines = int(db_readline())
    if reply_lines == -1:
        return None
    tickets = []
    for i in range(reply_lines):
        tickets.append(db_readline().split(' '))
    return tickets

# user *id* refund *num* ticket(s) of *ticket_kind* in *train_id* from *loc1* to *loc2* at *date*
# return 1 if success
# return 0 if fail
# mirror of buy_ticket
def refund_ticket(user_id, num, train_id, loc1, loc2, date, ticket_kind):
    db_write(' '.join(['refund_ticket', user_id, num, train_id, loc1, loc2, date, ticket_kind]))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

# add a train but do not sale
# e.g.
# >>> add_train abc123456 G123456 G 2 1 商务座
# >>> 北京 xx:xx 08:00 00:00 ￥0.0
# >>> 夏威夷 08:01 xx:xx 00:00 ￥1.5

# *catalog* should be a single char

# return 1 if success
# return 0 if fail
def add_train(train_id, name, catalog, station_list, ticket_kind_list):
    db_write(' '.join(['add_train', train_id, name, catalog, str(len(station_list)), str(len(ticket_kind_list))] + ticket_kind_list))
    for x in station_list:
        db_write(' '.join(x))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

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
        return True
    else:
        return None

# open sale for train_id
# and this train can on longer be modify or delete
# return 1 if success
# return 0 if fail (i.e. already on sale or doesn't exist)
def sale_train(train_id):
    db_write(' '.join(['sale_train', train_id]))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

# return info like add_train
# return 0 if fail (not exist or not on sale)
def query_train(train_id):
    db_write(' '.join(['query_train', train_id]))
    reply = db_readline()
    if reply == '0':
        return None
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
            'ticket_kind_list': ticket_kind_list}

# delete *train_id*
# return 1 if success
# return 0 fail
# eg: delete_train("abc123456")
def delete_train(train_id):
    db_write(' '.join(['delete_train', train_id]))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

if __name__ == '__main__':
    print(register("lyx", "123", "qaq", "qqq"))
    print(register("lyc", "123", "qaq", "qqq"))
    print(register("lyc", "123", "qaq", "qqq"))
    print(add_train('xiaohuoche', 'littletrain', 'C', [['菜鸡站', '07:34', '08:00', '00:01', '￥0.0', '￥0.0'], ['脑残站', '08:02', '12:00', '00:00', '￥1.5', '￥3.0']], ['普通的票', 'VIP']))
    print(add_train('dahuoche', 'bigtrain', 'D', [['B', '12:34', '13:00', '00:00', '￥0.0', '￥0.0'], ['C', '13:01', '14:00', '00:00', '￥1.5', '￥3.0']], ['普通的票', 'VIP']))
    print(add_train('train', 'train', 'C', [['A', '07:00', '08:00', '00:10', '￥0.0'], ['B', '08:10', '12:00', '00:10', '￥3.0']], ['VIP']))
    print(sale_train('xiaohuoche'))
    print(sale_train('dahuoche'))
    print(sale_train('train'))
    print(query_ticket('菜鸡站', '脑残站', '2018-06-01', 'CD'))
    # print(buy_ticket('2018', '2', 'xiaohuoche', '菜鸡站', '脑残站', '2018-06-01', 'VIP'))
    # print(query_order('2018', '2018-06-01', 'C'))
    # print(refund_ticket('2018', '1', 'xiaohuoche', '菜鸡站', '脑残站', '2018-06-01', 'VIP'))
    # print(query_order('2018', '2018-06-01', 'C'))
    print(query_transfer('A', 'C', '2018-06-05', 'CD'))
