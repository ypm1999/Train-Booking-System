#!/usr/bin/python
# -*- coding:utf-8 -*-
from database import *

class User(USER):
    id = USER.id
    name = USER.id
    password = USER.password
    email = USER.email
    phone = USER.phone
    def getuser(self, username):
        self.name = username
        temp = query_profile(username);
        if not temp.success:
            return None
        self.id = temp.id
        self.password = temp.password
        self.email = temp.email
        self.phone = temp.phone
        return self
    def is_authenticated(self):
        return True
    def is_active(self):
        return True
    def is_anonymous(self):
        return False
    def get_id(self):
        return unicode(self.id)
    def __repr__(self):
        return '<User %r>' % (self.name)
