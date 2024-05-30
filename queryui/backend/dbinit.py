from app import db
from models import Favorite
from typing import List, Set
import json


def create_favorites(favs):
    for f in favs:
        favorite = Favorite(
            strategy_name=f["name"], parameters_json=json.dumps(f["parameters"], indent=4))
        db.session.add(favorite)
        try:
            db.session.commit()
            print(f"Created {favorite}")
        except Exception as e:
            db.session.rollback()
            print(str(e))


def init_db():
    db.drop_all()
    db.create_all()
    create_favorites([{"name": "Geofencing", "parameters": {
                     "latitude": "57", "longitude": "98", "radius": "100"}},
        {"name": "Locate", "parameters": {
            "username": "jack"}}])
