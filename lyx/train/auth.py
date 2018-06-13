#!/usr/bin/python
# -*- coding:utf-8 -*-
import os, datetime
from flask import current_app, Blueprint, render_template, abort, request, flash, redirect, url_for
from jinja2 import TemplateNotFound
from main import LM
from flask.ext.login import (current_user, login_required, login_user, logout_user, confirm_login, fresh_login_required)
