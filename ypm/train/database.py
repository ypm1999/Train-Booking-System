#!/usr/bin/python3
# -*- coding:utf-8 -*-

from subprocess import Popen, PIPE, STDOUT

debug = 1

endl = '<br />'

db = Popen(['./db'], stdin = PIPE, stdout = PIPE)

def db_write(cmd):
    # print((cmd + '\n').encode())
    db.stdin.write((cmd + '\n').encode())
    db.stdin.flush()

def db_readline():
    return db.stdout.readline().decode().strip('\n')

# if success return id
# else return -1
def register(name, password, email, phone):
    if name == '' or password == '' or email == '' or phone == '':
        return None
    db_write(' '.join(['register', name, password, email, phone]))
    reply = db_readline()
    if reply == '-1':
        return None
    return reply

# if success return 1
# else return 0
def try_login(user_id, password):
    if user_id == '' or password == '':
        return None
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
    if user_id == '':
        return None
    db_write(' '.join(['query_profile', user_id]))
    reply = db_readline()
    if reply == '0':
        return None
    tmp = reply.split(' ')
    return {'name': tmp[0], 'email': tmp[1], 'phone': tmp[2], 'admin': int(tmp[3]) == 2}

# if success return 1
# else return 0
def modify_profile(user_id, name, password, email, phone):
    if user_id == '' or name == '' or password == '' or email == '' or phone == '':
        return None
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
    if id1 == '' or id2 == '' or privilege == '':
        return None
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
    
    key.append('startTime')
    value.append(value[2] + endl + value[3])
    key.append('arriveTime')
    value.append(value[5] + endl + value[6])

    sz = len(reply)
    for i in range(7, sz, 3):
    	if (reply[i] == '商务座'):
    		reply[i] = '特等座'
    	key.append(reply[i])
    	value.append(reply[i + 1] + '张' + endl + '￥' + reply[i + 2])
    
    train = dict(zip(key, value))

    trains.append(train)

def query_ticket(loc1, loc2, date, catalog):
    if loc1 == '' or loc2 == '' or date == '' or catalog == '':
        return None
    print('#############################query_ticket||||||||||||||', loc1, loc2, date, catalog)
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
def query_transfer(loc1, loc2, date, catalog):
    if loc1 == '' or loc2 == '' or date == '' or catalog == '':
        return None
    print('#############################query_transfer||||||||||||||', loc1, loc2, date, catalog)
    db_write(' '.join(['query_transfer', loc1, loc2, date, catalog]))
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
    if user_id == '' or num == '' or train_id == '' or loc1 == '' or loc2 == '' or date == '' or ticket_kind == '':
        return None
    db_write(' '.join(['buy_ticket', user_id, num, train_id, loc1, loc2, date, ticket_kind]))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

def _to_order(info):
    key = ['train_id', 'loc1', 'date1', 'time1', 'loc2', 'date2', 'time2']
    head = info[:7]

    print(info)

    key.append('start_time')
    head.append(head[2] + endl + head[3])
    key.append('arrive_time')
    head.append(head[5] + endl + head[6])

    key += ['ticket_kind', 'num', 'price']

    ticket_list = []
    for i in range(7, len(info), 3):
        print(info[i + 1])
        if int(info[i + 1]) > 0:
            ticket_list.append(dict(zip(key, head + [info[i], info[i + 1], info[i + 2]])))
    return ticket_list

# return a list contains tickets
# bought by user *id* with *catalog* at *date*
# return -1 if query is illegal
def query_order(user_id, date, catalog):
    if user_id == '' or date == '' or catalog == '':
        return None
    db_write(' '.join(['query_order', user_id, date, catalog]))
    reply_lines = int(db_readline())
    if reply_lines == -1:
        return None
    print(reply_lines)
    tickets = []
    for i in range(reply_lines):
        tickets += _to_order(db_readline().split(' '))
    return tickets

# user *id* refund *num* ticket(s) of *ticket_kind* in *train_id* from *loc1* to *loc2* at *date*
# return 1 if success
# return 0 if fail
# mirror of buy_ticket
def refund_ticket(user_id, num, train_id, loc1, loc2, date, ticket_kind):
    if user_id == '' or num == '' or train_id == '' or loc1 == '' or loc2 == '' or date == '' or ticket_kind == '':
        return None
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
    if train_id == '' or name == '' or catalog == '' or len(station_list) == 0 or len(ticket_kind_list) == 0:
        return None
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
    if train_id == '' or name == '' or catalog == '' or len(station_list) == 0 or len(ticket_kind_list) == 0:
        return None
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
    if train_id == '':
        return None
    db_write(' '.join(['sale_train', train_id]))
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

def _to_station(info, ticket):
    key = ['name', 'arrive', 'leave', 'stop'] + ticket
    return dict(zip(key, info))

# return info like add_train
# return 0 if fail (not exist or not on sale)
def query_train(train_id):
    if train_id == '':
        return None
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
    station = []
    for i in range(station_num):
        station.append(_to_station(db_readline().split(), ticket_kind_list))

    return {'train_id': train_id,
            'name': name,
            'saled': '否',
            'catalog': catalog,
            'station': station,
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

# 删库
def clean():
    db_write('clean')
    reply = db_readline()
    if reply == '1':
        return True
    else:
        return None

if __name__ == '__main__':
    clean()
    if debug:
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
        print(buy_ticket('2018', '2', 'xiaohuoche', '菜鸡站', '脑残站', '2018-06-01', 'VIP'))
        print(query_order('2018', '2018-06-01', 'C'))
        print(refund_ticket('2018', '1', 'xiaohuoche', '菜鸡站', '脑残站', '2018-06-01', 'VIP'))
        print(query_order('2018', '2018-06-01', 'C'))
        print(query_transfer('A', 'C', '2018-06-05', 'CD'))
