#!/usr/bin/python
# -*- coding:utf-8 -*-
from functools import wraps
from database import *
from flask_login import (LoginManager, current_user, login_required,
                            login_user, UserMixin)

LM = LoginManager()
LM.login_view = 'user_login'
LM.login_message = 'please login!'
LM.session_protection = 'strong'

class User(UserMixin):
    name = ''
    password = ''
    email = ''
    phone = ''
    admin = bool(False)
    def getuser(self, username):
        temp = query_profile(username);
        if temp == None:
            return None
        self.name = temp['name']
        self.password = temp['password']
        self.email = temp['email']
        self.phone = temp['phone']
        self.admin = temp['admin']
        return self

    def is_authenicated(self):
        return True

    def is_admin(self):
        return self.admin

    def get_id(self):
        return unicode(self.name)
    def __repr__(self):
        return '<User %r>' % (self.name)

@LM.user_loader
def load_user(name):
    user = User();
    return user.getuser(name)
