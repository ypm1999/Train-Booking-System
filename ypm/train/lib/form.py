#!/usr/bin/python
# -*- coding:utf-8 -*-

from flask_wtf import FlaskForm
from wtforms import StringField, PasswordField, HiddenField, IntegerField, BooleanField
from wtforms.validators import *

class SigninForm(FlaskForm):
    name = StringField(validators = [DataRequired(u'用户名必填！'), Length(min = 3, max = 20, message=u"请输入用户名")])
    password = PasswordField(validators = [DataRequired(u'密码必填！'), Length(min = 5, max = 20, message = u"请输入密码")])

class SignupForm(FlaskForm):
    name = StringField(validators = [DataRequired(u'用户名必填！'),Length(min = 3, max = 20, message = u"请输入用户名，长度在3～20字符之间")])
    password = PasswordField(validators = [DataRequired(u'密码必填！'), Length(min = 5, max = 20, message = u"请输入密码，长度在5～20字符之间")])
    email = StringField([DataRequired(u'邮箱必填！'), Email(u"请输入邮箱")])
    phone = StringField([DataRequired(u'手机号必填！'), Length(min = 11, max = 11, message = u"请输入合法的手机号码")])

class UserForm(FlaskForm):
    name = StringField()
    email = StringField()
    phone = StringField()

class QueryOneTrain(FlaskForm):
    pass

class OneTrain(FlaskForm):
    pass

class QueryTwoTrain(FlaskForm):
    pass

class TwoTrain(FlaskForm):
    pass
