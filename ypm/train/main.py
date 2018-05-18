#!/usr/bin/python
# -*- coding:utf-8 -*-

import os
from flask import *
from form import *
from database import *
from flask_login import LoginManager
from user import User

LM = LoginManager()
app = Flask(__name__)
app.config['SECRET_KEY'] = os.urandom(24)
LM.init_app(app)
LM.login_view = 'login'
LM.login_message = 'please login!'
LM.session_protection = 'strong'

@LM.user_loader
def load_user(name):
    return User.getuser(name)


@app.route('/', methods = ['GET', 'POST'])
def index():
    return render_template('index.html')

@app.route('/register', methods = ['GET', 'POST'])
def try_register():
    user = SignupForm(request.form)
    if user.validate_on_submit():
        if register(user.name.data, user.password.data, user.email.data, user.phone.data):
            return redirect('/')
        else:
            return render_template('register.html', note = 'something wrong', form = user)
    return render_template('register.html', note = '', form = user,)

@app.route('/login', methods = ['POST'])
def try_login():
    username = request.form["username"]
    password = request.form["password"]
    if login(username, password):
        usert = User()
        user = usert.getuser(username)
        login_user(user,True)
        flash('Logged in successfully.')
        next = request.args.get('next')
        if not next_is_valid(next):
            return abort(400)
        return redirect(next or '/')
    return render_template('login.html')

@app.route('/login', methods = ['GET'])
def login_page():
    return render_template("login.html")


@app.route('/manage', methods = ['GET', 'POST'])
def manage():
    pass
@app.route('/query', methods = ['GET', 'POST'])
def query():
    pass

@app.route('/order', methods = ['GET', 'POST'])
def order():
    pass

if __name__ == '__main__':
    app.run(port = 5000, debug=True)
