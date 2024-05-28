from app import db
from models import Edge
from typing import List, Set


def generate_graph(events: List[List[str]]):
    # Build tuples
    for row in events:
        tuples = []
        for i, from_id in enumerate(row):
            for to_id in row[i+1:]:
                if from_id != to_id:
                    tuples.append((from_id, to_id))
                    tuples.append((to_id, from_id))

        for t in tuples:
            edge = Edge(from_id=t[0], to_id=t[1], context=f'({t[0]},{t[1]})')
            db.session.add(edge)
            try:
                db.session.commit()
                # print(f"Created {edge}")
            except Exception as e:
                db.session.rollback()
                print(str(e))


def init_db():
    db.drop_all()
    db.create_all()
    generate_graph([
        ['A', 'B'],
        ['B'],
        ['B', 'C', 'D'],
        ['C', 'D'],
        ['D', 'E', 'F'],
        ['F', 'G'],
        ['G', 'H'],
        ['H', 'I'],
    ])
