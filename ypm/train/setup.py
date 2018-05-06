from flask import Flask, request, render_template

web = Flask(__name__)

@web.route('/', methods = ['GET', 'POST'])
def index():
    return render_template('index.html')


@web.route('/register', methods = ['GET'])
def register_form():
    return render_template('register.html', note = '')

def userRegister(name, passwd, email, phone):
    return False

@web.route('/register', methods = ['POST'])
def check_register():
    name = request.form['userName']
    passwd = request.form['userPasswd']
    email = request.form['userEmail']
    phone = request.form['userPhone']
    if userRegister(name, passwd, email, phone):
        return render_template('register_successfully.html', username = name)
    else:
        return render_template('register.html', note = 'something wrong')

@web.route('/manage', methods = ['GET', 'POST'])
def manage():
    pass
@web.route('/query', methods = ['GET', 'POST'])
def query():
    pass

@web.route('/order', methods = ['GET', 'POST'])
def order():
    pass

if __name__ == '__main__':
    web.run(port = 5000, debug=True)
