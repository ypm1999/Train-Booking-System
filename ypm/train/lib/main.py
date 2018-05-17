import os
from flask import Flask, request, render_template
from form import *
from user import loginManager

app = Flask(__name__)
loginManager.init_app(app)

app.config.update(dict(
    SECRET_KEY=os.urandom(24)
))

@app.route('/', methods = ['GET', 'POST'])
def index():
    return render_template('index.html')

def register(name, passwd, email, phone):
    print name
    print passwd
    print email
    print phone
    return True

@app.route('/register', methods = ['GET', 'POST'])
def register_form():
    user = SignupForm(request.form)
    if user.validate_on_submit():
        if register(user.name.data, user.password.data, user.email.data, user.phone.data):
            print "to /"
            return redirect('/')
        else:
            return render_template('register.html', note = 'something wrong', form = user)
    return render_template('register.html', note = '', form = user)

"""
@app.route('/register', methods = ['POST'])
def check_register():
    user = SignupForm(request.form)
    if register(user.name, user.password, user.email, user.phone):
        return render_template('register_successfully.html', username = user.name)
    else:
        return render_template('register.html', note = 'something wrong')
"""


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
