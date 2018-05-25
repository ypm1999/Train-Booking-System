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
app.config['SECRET_KEY'] = 'ASSUEIIJSUasdfdsfeIIJJLL'
LM.init_app(app)

@app.route('/register', methods = ['GET', 'POST'])
def test():
    return render_template('test.html')

@app.route('/register', methods = ['GET', 'POST'])
def user_register():
    user = UserForm(request.form)
    if request.method == 'POST':
        if register(user.name.data, user.password.data, user.email.data, user.phone.data):
            return redirect('/')
        else:
            return render_template('register.html', note = '注册失败', form = user)
    return render_template('register.html', note = '注册', form = user)


@app.route('/', methods = ['GET', 'POST'])
def home():
    return render_template('index.html')

@app.route('/login', methods = ['POST', 'GET'])
def user_login():
    if request.method == 'POST':
        userid = request.form["userid"]
        password = request.form["password"]
        if try_login(userid, password):
            user = User().getuser(userid)
            login_user(user, True)
            next = request.args.get('next')
            print current_user
            return redirect( next or url_for('home') )
    return render_template("login.html")

@app.route('/logout')
@login_required
def logout():
    logout_user();
    return redirect(url_for('home'))


@app.route('/user/info', methods = ['POST', 'GET'])
@login_required
def user_info(id = None):
    user = User().getuser(current_user.id)
    sucess = False
    if id:
        user.getuser(id)
    else:
        if request.method == 'POST':
            form = request.form;
            if(modify_profile(user.id, form["name"], form["password"], form["email"], form["phone"])):
                falsh(message="Change user profile successful!", category='successful')
                sucess = True
            else:
                falsh(message="Unfortunately, change user profile file.", category='error')
    return render_template("profile.html", user = user, succ = sucess)


@app.route('/user/ticteks', methods = ['POST', 'GET'])
@login_required
def user_orders():
    pass

@app.route('/manage', methods = ['GET', 'POST'])
@login_required
def manage():
    if not current_user.is_admin():
        flash(message='You are not manager !', category='error')
        return redirect(url_for('home'))
    else:
        return render_template("manage.html")


@app.route('/manage/users', methods = ['GET', 'POST'])
@login_required
def manage_users():
    if not current_user.is_admin():
        flash(message='You are not manager !', category='error')
        return redirect(url_for('home'))
    else:
        if request.method == 'POST':
            return user_info(request.form["id"])
        else:
            return redirect( url_for('user_info') )


@app.route('/manage/train', methods = ['GET', 'POST'])
@login_required
def manage_trains():
    if not current_user.is_admin():
        flash('You are not manager !')
        return redirect(url_for('home'))
    else:
        pass

@app.route('/manage/add_train', methods = ['GET', 'POST'])
@login_required
def add_train():
    if not current_user.is_admin():
        flash('You are not manager !')
        return redirect(url_for('home'))
    else:
        pass



@app.route('/manage/del_train', methods = ['GET', 'POST'])
@login_required
def del_train():
    if not current_user.is_admin():
        flash('You are not manager !')
        return redirect(url_for('home'))
    else:
        pass


@app.route('/manage/sale_train', methods = ['GET', 'POST'])
@login_required
def sale_train():
    if not current_user.is_admin():
        flash('You are not manager !')
        return redirect(url_for('home'))
    else:
        pass



@app.route('/train/query', methods = ['GET', 'POST'])
def query():
    pass

@app.route('/tictek/order', methods = ['GET', 'POST'])
@login_required
def tictek_order():
    pass

@app.route('/tictek/refund', methods = ['GET', 'POST'])
@login_required
def tictek_refund():
    pass

if __name__ == '__main__':
    app.debug = True
    app.run(port=5000)
