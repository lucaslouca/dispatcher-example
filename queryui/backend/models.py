from app import db
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()


class Favorite(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    strategy_name = db.Column(db.String(50))
    parameters_json = db.Column(db.String(500))

    def __repr__(self):
        return f'Favorite <name={self.strategy_name} parameters_json={self.parameters_json}>'
