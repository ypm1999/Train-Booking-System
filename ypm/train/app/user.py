from flaskext.login import LoginManager
from flaskext.openid import OpenID

from config import basedir
lm = LoginMananger()
lm.steup_app(web)
