import os
from flask import Flask
from flaskext.login import LoginManager
from flaskext.openid import OpenID
from config import basedir

lm = LoginManager()
lm.setup_app(app)
oid = OpenID(app, os.path.join(basedir, 'tmp'))
