from flask import request, jsonify, send_file, Blueprint, make_response
from models import Favorite

favorite_api = Blueprint('favorite_api', __name__)

############################################################################################
# Strategy
############################################################################################


@favorite_api.route('/favorite', methods=['GET'])
def get_all():
    result = []
    return jsonify({'result': result})


@favorite_api.route('/favorite/<id>', methods=['GET'])
def get_by_id(id):
    result = []
    return jsonify({'result': result})


@favorite_api.route('/favorite', methods=['POST'])
def create():
    data = request.get_json()
    print(data)
    return make_response(jsonify({'success': 'created'}),  200)


@favorite_api.route('/favorite', methods=['PUT'])
def update():
    data = request.get_json()
    print(data)
    return make_response(jsonify({'success': 'updated'}),  200)


@favorite_api.route('/favorite/<id>', methods=['DELETE'])
def delete(id):
    data = request.get_json()
    print(data)
    return make_response(jsonify({'success': 'deleted'}),  200)

# @ favorite_api.route('/strategy/<from_id>', methods=['GET'])
# def get_strategy(from_id):
#     nodes = {}
#     edges = {}

#     level = 2
#     next_query_ids = {from_id}
#     ids_so_far = set()
#     ids_so_far.update(next_query_ids)
#     for lvl in range(level):
#         # in_clause = '('+', '.join([f"'{i}'" for i in next_query_ids]) + ')'
#         query_result = Edge.query.filter(
#             Edge.from_id.in_(next_query_ids)).all()
#         if not query_result:
#             break
#         next_query_ids = {e.to_id for e in query_result} - ids_so_far
#         ids_so_far.update({e.to_id for e in query_result})

#         for e in query_result:
#             nodes[e.from_id] = {
#                 'id': e.from_id, 'name': e.from_id, 'face': 'id.png', 'type': 'Identifier'}
#             nodes[e.to_id] = {'id': e.to_id, 'name': e.to_id,
#                               'face': 'id.png', 'type': 'Identifier'}
#             if e.from_id < e.to_id:
#                 edge_key = f"{e.from_id}-{e.to_id}"
#             else:
#                 edge_key = f"{e.to_id}-{e.from_id}"
#             if edge_key not in edges:
#                 edges[edge_key] = {'source': e.from_id,
#                                    'target': e.to_id, 'label': e.context}

#     return jsonify({'strategy': {'nodes': nodes, 'edges': edges}})
