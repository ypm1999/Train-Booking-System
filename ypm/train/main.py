#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
from flask import *
from database import *
from flask_login import (LoginManager, current_user, login_required, login_user, logout_user, confirm_login, fresh_login_required)
from user import LM, User

reload(sys)
sys.setdefaultencoding('utf-8')
app = Flask(__name__)
app.jinja_env.variable_start_string = '{{ '
app.jinja_env.variable_end_string = ' }}'
app.config['SECRET_KEY'] = 'ASSUEIIJSUasdfdsfeIIJJLL'
LM.init_app(app)


@app.route('/', methods = ['GET', 'POST'])
def home():
    return render_template('index.html')

#用户管理，注册，登录，登出，profile，修改profile, 订单
@app.route('/register', methods = ['GET', 'POST'])
def user_register():
    form = request.form;
    if request.method == 'POST':
        id = register(form['name'], form['password'], form['email'], form['phone']);
        if id:
            user = User().getuser(id)
            login_user(user, False)
            flash(message="注册成功! 你的ID是%s，请牢记。" % id, category='success')
            if user.is_admin():
                flash(message="您注册为管理猿", category='info')
            elif user.is_root():
                flash(message="您注册为管理猿王", category='info')
            return redirect(url_for('home'))
        else:
            return render_template('register.html', note = '注册失败，请检查注册信息', form = form)
    return render_template('register.html', note = '注册为新用户', form = form)


@app.route('/login', methods = ['POST', 'GET'])
def user_login():
    if request.method == 'POST':
        userid = request.form["userid"]
        password = request.form["password"]
        remember = (request.form["remember"] == 'on')
        if try_login(userid, password):
            user = User().getuser(userid)
            login_user(user, remember)
            next = request.args.get('next')
            flash(message = "登录成功!", category = "success")
            return redirect( next or url_for('home') )
    return render_template("login.html")


@app.route('/logout')
@login_required
def logout():
    logout_user();
    flash(message="登出成功！", category='success')
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
            if(modify_profile(form['id'], form["name"], form["password"], form["email"], form["phone"])):
                flash(message="修改个人信息成功！", category='success')
            else:
                flash(message="修改个人信息失败.", category='warning')
    return render_template("profile.html", user = user)

@app.route('/Data/user/orders', methods = ['GET'])
@login_required
def get_order_data():
    user_id = request.args['user_id']
    date = request.args['date']
    catalog = 'CDGKOTZ'
    print (user_id, date, catalog)
    orders = query_order(user_id, date, catalog)
    print(orders)
    if (orders == None):
        orders = []
    return json.dumps(orders)
    # TODO query_order(user_id, date, catelog)
    # catelog 自动设置为全集

@app.route('/user/orders', methods = ['POST', 'GET'])#and manage_orders
@login_required
def query_orders(id = None):
    tmp = '';
    if(not current_user.is_admin()):
        tmp = "readonly";
    if id == None:
        id = current_user.id
    return render_template('query_orders.html', user_id = id, readonly = tmp)


#管理员界面，管理用户，管理订单，管理车次。
@app.route('/manage', methods = ['GET', 'POST'])
@login_required
def manage():
    if not current_user.is_admin():
        flash(message='您不是管理员！', category='warning')
        return redirect(url_for('home'))
    else:
        return render_template("manage.html")


@app.route('/manage/users', methods = ['GET', 'POST'])
@login_required
def manage_users():
    if not current_user.is_admin():
        flash(message='您不是管理员！', category='warning')
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
        flash('您不是管理员！')
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
        return json.dumps({'train_id':'', 'name':'', 'station':[], 'saled':''})
    print "train_id: %s" % train_id
    train = query_train(train_id)
    print train
    if train == None:
        return json.dumps({'train_id':'', 'name':'', 'station':[], 'saled':''})
    return json.dumps(train)

@app.route('/manage/trains', methods = ['GET', 'POST'])
@login_required
def manage_trains():
    if not current_user.is_admin():
        flash('您不是管理员！')
        return redirect(url_for('home'))
    else:
        return render_template('manage_trains.html');


@app.route('/manage/addTrain', methods = ['GET', 'POST'])
@login_required
def try_add_train():
    if not current_user.is_admin():
        flash(message='您不是管理员！', category='warning')
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
        add_train(id, name, catelog, seats, stations)
        print (id, name, n, m, catelog)
        print seats
        return redirect(url_for('manage_trains'));


@app.route('/manage/delTrain', methods = ['GET', 'POST'])
@login_required
def try_del_train():
    if not current_user.is_admin():
        flash(message='您不是管理员！', category='warning')
        return redirect(url_for('home'))
    else:
        train_id = request.form["search_id"];
        if(delete_train(train_id)):
            flash(message="删除车次成功!", category='success')
        else:
            flash(message="删除车次失败!", category='success')
        return redirect(url_for('manage_trains'));


@app.route('/manage/saleTrain', methods = ['GET', 'POST'])
@login_required
def try_sale_train():
    if not current_user.is_admin():
        flash(message='您不是管理员！', category='warning')
        return redirect(url_for('home'))
    else:
        train_id = request.form["search_id"];
        if(sale_train(train_id)):
            flash(message="发售车次成功!", category='success')
        else:
            flash(message="发售车次失败!", category='success')
        return redirect(url_for('manage_trains'));


#查票，购票，退票
@app.route('/Data/trains/transfer', methods = ['GET', 'POST'])
def query_tieket_transfer_data():
    loc1 = request.args['loc1']
    loc2 = request.args['loc2']
    date = request.args['date']
    catelog = request.args['catelog']
    trains = query_transfer(loc1, loc2, date, catelog)
    if trains == None:
        trains = []
    #TODO query_transform(loc1, loc2, date, catelog)
    return json.dumps(trains)

@app.route('/Data/trains', methods = ['GET', 'POST'])
def query_tieket_data():

    loc1 = request.args['loc1']
    loc2 = request.args['loc2']
    date = request.args['date']
    catelog = request.args['catelog']
    #TODO query_ticket(loc1, loc2, date, catelog)
    trains = query_ticket(loc1, loc2, date, catelog)
    if trains == None:
        trains = []
    return json.dumps(trains)

@app.route('/ticket/query', methods = ['GET', 'POST'])
def try_query_tickets():
    return render_template("query_tickets.html", form = request.form)

@app.route('/ticket/buy', methods = ['GET', 'POST'])
@login_required
def try_book_ticket():
    form = request.form;
    if buy_ticket(current_user.id, form['number'], form['train_id'], form['loc1'], form['loc2'], form['date'], form['seat']):
        flash(message='购票成功！', category = 'success');
        return redirect( url_for('home') );
    else:
        flash(message='购票失败！', category = 'danger');
        return render_template("query_tickets.html", form = form)


@app.route('/ticket/refund', methods = ['POST'])
@login_required
def try_refund_ticket():
    user_id = request.form['user_id']
    train_id = request.form['train_id']
    num = request.form['num']
    loc1 = request.form['loc1']
    loc2 = request.form['loc2']
    date = request.form['date']
    catalog = request.form['ticket_kind']
    print(user_id, num, train_id, loc1, loc2, date, catalog)
    if(refund_ticket(user_id, num, train_id, loc1, loc2, date, catalog)):
        flash(message="退票成功!", category='success')
    else:
        flash(message="退票失败，请重试！", category='warning')
    return query_orders(user_id)

@app.route('/manage/clean', methods = ['POST'])
@login_required
def try_clean():
    if current_user.is_root():
        pass
    return redirect(url_for('manage'));

@app.route('/manage/rollback', methods = ['POST'])
@login_required
def try_rollback():
    if current_user.is_root():
        pass
    return redirect(url_for('manage'));

@app.route('/manage/backup', methods = ['POST'])
@login_required
def try_clean():
    if current_user.is_root():
        pass
    return redirect(url_for('manage'));

if __name__ == '__main__':
    app.run(debug = True, port=5000)
