#!/usr/bin/python
# -*- coding:utf-8 -*-

import os
from flask import *
from form import *
from database import *
from flask_login import (LoginManager, current_user, login_required, login_user, logout_user, confirm_login, fresh_login_required)
from user import LM, User

app = Flask(__name__)
app.config['SECRET_KEY'] = 'ASSUEIIJSUIIJJLL'
LM.init_app(app)

@app.route('/', methods = ['GET', 'POST'])
def home():
    return render_template('index.html')

@app.route('/register', methods = ['GET', 'POST'])
def user_register():
    user = SignupForm(request.form)
    if user.validate_on_submit():
        if register(user.name.data, user.password.data, user.email.data, user.phone.data):
            return redirect('/')
        else:
            return render_template('register.html', note = 'something wrong', form = user)
    return render_template('register.html', note = '', form = user,)

@app.route('/login', methods = ['POST', 'GET'])
def user_login():
    if request.method == 'POST':
        username = request.form["username"]
        password = request.form["password"]
        if login(username, password):
            usert = User()
            user = usert.getuser(username)
            login_user(user,True)
            next = request.args.get('next')
            return render_template(url_for(home))
        return render_template('login.html')
    else:
        return render_template("login.html")

@app.route('/logout')
@login_required
def logout():
    logout_user();
    return redirect('/')


@app.route('/user/info', methods = ['POST', 'GET'])
@login_required
def user_info():
    return render_template("profile.html")



@app.route('/user/change', methods = ['POST', 'GET'])
@login_required
def user_info_change():
    form = SignupForm(request.form);
    if request.method == 'POST':
        return render_template()
    else:
        return render_template("change_profile.html", form = form)




@app.route('/user/orders', methods = ['POST', 'GET'])
@login_required
def user_orders():
    pass

@app.route('/manage', methods = ['GET', 'POST'])
@login_required
def manage():
    if not current_user.is_admin():
        return redirect(url_for('home'))
    else:
        pass

@app.route('/manage/add_train', methods = ['GET', 'POST'])
@login_required
def add_train():
    if not current_user.is_admin():
        return redirect(url_for('home'))
    else:
        pass

@app.route('/manage/del_train', methods = ['GET', 'POST'])
@login_required
def del_train():
    if not current_user.is_admin():
        return redirect(url_for('home'))
    else:
        pass


@app.route('/manage/sale_train', methods = ['GET', 'POST'])
@login_required
def sale_train():
    if not current_user.is_admin():
        return redirect(url_for('home'))
    else:
        pass

@app.route('/manage/users', methods = ['GET', 'POST'])
@login_required
def users():
    if not current_user.is_admin():
        return redirect(url_for('home'))
    else:
        pass

@app.route('/manage/users/search', methods = ['GET', 'POST'])
@login_required
def search_user():
    if not current_user.is_admin():
        return redirect(url_for('home'))
    else:
        pass

@app.route('/manage/users/modify', methods = ['GET', 'POST'])
@login_required
def modify_user():
    if not current_user.is_admin():
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
    app.run(port = 5000, debug=True)
