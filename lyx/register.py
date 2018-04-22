from flask import Flask, request, render_template, abort

app = Flask(__name__)

class user():
    pass

users = []

@app.route('/', methods=['GET', 'POST'])
def home():
    return render_template('home.html', heading1='Welcome to TicketMall', heading2='a test site')

@app.route('/about', methods=['GET', 'POST'])
def about():
    return render_template('about.html')

@app.route('/register', methods=['GET'])
def register():
    return render_template('register.html', heading1='Register', heading2='but you cannot login')

@app.route('/register', methods=['POST'])
def add_user():
    new_user = user()
    new_user.username = request.form['username']
    new_user.password = request.form['password']
    repwd = request.form['repeatpassword']
    if new_user.username == '':
        return render_template('info.html', heading1='Failed', heading2='Invalid username')
    elif new_user.password != repwd:
        return render_template('info.html', heading1='Failed', heading2='Please repeat indentical Password')
    else:
        users.append(new_user)
        return render_template('info.html', heading1='Success', heading2='Successfully registered')

@app.errorhandler(404)
def page_not_found(error):
    return render_template('info.html', heading1='404 Error', heading2='Page Not Found'), 404

if __name__ == '__main__':
    app.run()
