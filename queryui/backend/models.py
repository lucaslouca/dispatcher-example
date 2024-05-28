from app import db
from sqlalchemy.ext.declarative import declarative_base

Base = declarative_base()


class Edge(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    from_id = db.Column(db.String(50))
    to_id = db.Column(db.String(50))
    context = db.Column(db.String(50))

    def __repr__(self):
        return f'Edge <{self.from_id} ={self.context}=> {self.to_id}>'
