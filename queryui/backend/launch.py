from app import app


def register():
    from web import web_api
    from strategy import strategy_api
    app.register_blueprint(web_api)
    app.register_blueprint(strategy_api, url_prefix='/api')


if __name__ == '__main__':
    register()
    from dbinit import init_db
    init_db()
    app.run(debug=True, host='0.0.0.0', port=8667)
else:
    register()
    gunicorn_app = app  # gunicorn launch:app --bind 0.0.0.0:8667 --daemon
