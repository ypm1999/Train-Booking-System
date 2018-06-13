#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
reload(sys)
sys.setdefaultencoding('utf-8')
from flask import *
from form import *
from database import *
from flask_login import (LoginManager, current_user, login_required, login_user, logout_user, confirm_login, fresh_login_required)
from user import LM, User

app = Flask(__name__)
app.jinja_env.variable_start_string = '{{ '
app.jinja_env.variable_end_string = ' }}'
app.config['SECRET_KEY'] = 'ASSUEIIJSUasdfdsfeIIJJLL'
LM.init_app(app)

@app.route('/test', methods = ['GET', 'POST'])
def test():
    return render_template('test.html')

@app.route('/', methods = ['GET', 'POST'])
def home():
    print current_user
    return render_template('index.html')

#用户管理，注册，登录，登出，profile，修改profile, 订单
@app.route('/register', methods = ['GET', 'POST'])
def user_register():
    user = UserForm(request.form)
    if request.method == 'POST':
        id = register(user.name.data, user.password.data, user.email.data, user.phone.data);
        if id:
            user = User()
            user.getuser(id)
            login_user(user, True)
            flash(message="Register successful! Your ID is %s" % id, category='success')
            if user.is_admin():
                flash(message="You registered as manager.", category='info')
            return redirect(url_for('home'))
        else:
            return render_template('register.html', note = '注册失败', form = user)
    return render_template('register.html', note = '注册为新用户', form = user)


@app.route('/login', methods = ['POST', 'GET'])
def user_login():
    if request.method == 'POST':
        userid = request.form["userid"]
        password = request.form["password"]
        if try_login(userid, password):
            user = User().getuser(userid)
            login_user(user, True)
            next = request.args.get('next')
            flash(message = "Login successfully!", category = "success")
            print current_user
            return redirect( next or url_for('home') )
    return render_template("login.html")

@app.route('/logout')
@login_required
def logout():
    logout_user();
    flash(message="You logout successfully.", category='success')
    return redirect(url_for('home'))


@app.route('/user/info', methods = ['POST', 'GET'])
@login_required
def user_info(id = None):
    user = User().getuser(current_user.id)
    success = False
    if id:
        user.getuser(id)
    else:
        if request.method == 'POST':
            form = request.form;
            if(modify_profile(user.id, form["name"], form["password"], form["email"], form["phone"])):
                flash(message="Change user profile successful!", category='success')
                success = True
            else:
                flash(message="Unfortunately, change user profile failed.", category='warning')
    return render_template("profile.html", user = user, succ = success)


@app.route('/user/modify', methods = ['POST', 'GET'])
@login_required
def modify_user():
    user_id = request.form['id']
    name = request.form['name']
    password = request.form['password']
    email = request.form['email']
    phone = request.form['phone']
    modify_profile(user_id, name, password, email, phone)
    return user_info(user_id);

@app.route('/user/tickets', methods = ['POST', 'GET'])#and manage_orders
@login_required
def user_orders(id = None):
    if id == None:
        id = current_user.id
    if request.method == 'POST':
        pass
    else:
        return render_template('user_tickets.html')


#管理员界面，管理用户，管理订单，管理车次。
@app.route('/manage', methods = ['GET', 'POST'])
@login_required
def manage():
    if not current_user.is_admin():
        flash(message='You are not manager !', category='warning')
        return redirect(url_for('home'))
    else:
        return render_template("manage.html")


@app.route('/manage/users', methods = ['GET', 'POST'])
@login_required
def manage_users():
    if not current_user.is_admin():
        flash(message='You are not manager !', category='warning')
        return redirect(url_for('home'))
    else:
        if request.method == 'POST':
            return user_info(request.form["id"])
        else:
            return redirect( url_for('user_info') )


@app.route('/manage/orders', methods = ['GET', 'POST'])
@login_required
def manage_orders():
    if not current_user.is_admin():
        flash('You are not manager !')
        return redirect(url_for('home'))
    else:
        if request.method == 'POST':
            return user_orders(request.form["userid"])
        else:
            return redirect( url_for('user_orders') )

#车次管理，增加，删除，修改，发售
@app.route('/Data/searchTrain', methods = ['GET', 'POST'])
def query_train_data():
    train_id = request.args['train_id']
    print train_id
    if train_id == '':
        return json.dumps({'train_id':'', 'name':'', 'station':[]})
    print "train_id: %s" % train_id
    station1 = {'name':'上海1', 'arrive': '10:30', 'leave':'10:40', '硬座': '￥1200'}
    station2 = {'name':'上海2', 'arrive': '10:31', 'leave':'10:40', '硬座': '￥1201'}
    station3 = {'name':'上海3', 'arrive': '10:32', 'leave':'10:40', '硬座': '￥1202'}
    train1 = {'train_id':train_id, 'name': 'C919', "saled" : '否', 'station':[station1, station2, station3]}
    print json.dumps([train1, train1])
    return json.dumps(train1)

@app.route('/manage/train', methods = ['GET', 'POST'])
@login_required
def manage_trains():
    if not current_user.is_admin():
        flash('You are not manager !')
        return redirect(url_for('home'))
    else:
        return render_template('manage_trains.html');


@app.route('/manage/add_train', methods = ['GET', 'POST'])
@login_required
def add_train():
    if not current_user.is_admin():
        flash(message='You are not manager !', category='warning')
        return redirect(url_for('home'))
    else:
        id = request.form['train_id'];
        name = request.form['name'];
        catelog = request.form['catelog'];
        m = int(request.form['seatNumber']);
        n = int(request.form['stationnumber']);

        seats = []
        for i in range(m):
            seats.append(request.form['head' + str(i)]);
            print 'head' + str(i)

        stations = []
        for i in range(n):
            tmp = ''
            tmp += request.form['station' + str(i) + '_name'] + ' ';
            tmp += request.form['station' + str(i) + '_arrive'] + ' ';
            tmp += request.form['station' + str(i) + '_leave'] + ' ';
            tmp += request.form['station' + str(i) + '_stop'] + ' ';
            for j in range(m):
                tmp += '￥' + request.form['station' + str(i) + '_' + str(j)] + ' ';
            stations.append(tmp);
        #TODO add_train(id, name, catelog, seats, stations)
        print (id, name, n, m, catelog)
        print seats
        return redirect(url_for('manage_trains'));


@app.route('/manage/del_train', methods = ['GET', 'POST'])
@login_required
def del_train():
    if not current_user.is_admin():
        flash(message='You are not manager !', category='warning')
        return redirect(url_for('home'))
    else:
        pass

@app.route('/manage/sale_train', methods = ['GET', 'POST'])
@login_required
def sale_train():
    if not current_user.is_admin():
        flash(message='You are not manager !', category='warning')
        return redirect(url_for('home'))
    else:
        pass


#查票，购票，退票
@app.route('/Data/trains/transfer', methods = ['GET', 'POST'])
def query_ticket_transfer_data():
    train1 = {'id':'G12306', 'startTime': '2018-06-01<br />06:30', 'arriveTime': '2018-06-01<br />10:50', '硬座': str(100) + '张<br />' + '￥1200', '软卧': str(100) + '张<br />' + '￥1200'}
    train2 = {'id':'G11007', 'startTime': '2018-06-01<br />06:30', 'arriveTime': '2018-06-01<br />10:50', '软卧': str(100) + '张<br />' + '￥1200'}
    loc1 = request.args['loc1']
#    loc2 = request.args['loc2']
#    date = request.args['date']
#    catelog = request.args['catelog']
    print loc1
#    TODO query_transform(loc1, loc2, date, catelog)
    return json.dumps([train1, train2])

@app.route('/Data/trains', methods = ['GET', 'POST'])
def query_ticket_data():
    train1 = {'id':'G12306', 'startTime': '2018-06-01<br />06:30', 'arriveTime': '2018-06-01<br />10:50', '硬座': str(100) + '张<br />' + '￥1200', '软卧': str(100) + '张<br />' + '￥1200'}
    train2 = {'id':'G11007', 'startTime': '2018-06-01<br />06:30', 'arriveTime': '2018-06-01<br />10:50', '软卧': str(100) + '张<br />' + '￥1200'}
    loc1 = request.args['loc1']
    loc2 = request.args['loc2']
    date = request.args['date']
    catelog = request.args['catelog']
    print (loc1, loc2, date, catelog)
    print json.dumps([train1, train2])
    #TODO query_ticket(loc1, loc2, date, catelog)
    return json.dumps([train1, train2])

@app.route('/tickets', methods = ['GET', 'POST'])
def query_tickets():
    return render_template("query_trains.html", form = request.form)

@app.route('/ticket/order', methods = ['GET', 'POST'])
@login_required
def ticket_order():
    form = request.form;
    if buy_ticket(current_user.id, form['number'], form['train_id'], form['loc1'], form['loc2'], form['date'], form['seat']):
        flash(message='购票成功！', category = 'success');
        redirect( url_for('home') );
    else:
        flash(message='购票失败！', category = 'danger');
        return render_template("query_trains.html", form = form)

@app.route('/tictek/refund', methods = ['GET', 'POST'])
@login_required
def tictek_refund():
    pass

if __name__ == '__main__':
    app.debug = True
    app.run(port=5000)
