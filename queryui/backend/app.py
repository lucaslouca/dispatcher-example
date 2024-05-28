from flask import Flask
from flask_cors import CORS
from flask_sqlalchemy import SQLAlchemy
import os


class CustomFlask(Flask):
    jinja_options = Flask.jinja_options.copy()
    jinja_options.update(dict(
        variable_start_string='%%',  # Default is '{{', I'm changing this because Vue.js uses '{{' / '}}'
        variable_end_string='%%',
    ))


def create_app():
    app = CustomFlask(__name__, static_folder='./ui', template_folder='./templates')  # This replaces your existing "app = Flask(__name__)"
    basedir = os.path.abspath(os.path.dirname(__file__))
    CORS(app, resources={r"/*": {"origins": "*"}})
    app.config['SECRET_KEY'] = '004f2af45d3a4e161a7dd2d17fdae47f'
    app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///' + os.path.join(basedir, 'database.db')
    app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
    return app


app = create_app()
db = SQLAlchemy(app, session_options={"autoflush": False})
