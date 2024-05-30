
from flask import render_template, send_file, Blueprint
import os

web_api = Blueprint('web_api', __name__)


@web_api.route('/')
def home():
    return render_template('app.html')


# @web_api.route('/graph', methods=['GET'])
# def get_graph_query():
#     return render_template('app.html')


# @web_api.route('/graph/icon/<icon_name>', methods=['GET'])
# def get_icon(icon_name):
#     img_dir = "./templates/icons/"
#     img_path = os.path.join(img_dir, icon_name)
#     return send_file(img_path, mimetype='image/png')


# @web_api.route('/img/<img_name>')
# def flask_logo(img_name: str):
#     img_dir = "./ui/img/"
#     img_path = os.path.join(img_dir, img_name)
#     return send_file(img_path, mimetype='image/png')
