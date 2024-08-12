from typing import Dict, List
import glob
import os
import importlib
from pathlib import Path
import sqlite3
from datetime import datetime, timezone
import json
import sys

def get_strategy(strategy_name: str):
    result = []
    path_list = Path(".").glob("**/*Strategy.py")
    for path in path_list:
        module_name = str(path).rsplit(".", 1)[0]
        class_name = module_name.rsplit("/",1)[-1]
        if class_name == strategy_name:
            try:
                module = importlib.import_module(class_name)
                importlib.reload(module)
            except Exception as e:
                print(f"Error: could not import module '{class_name}': {e}")
                return []
            try:
                clazz = getattr(module, class_name)
                i = clazz()
                result.append(i)
            except Exception as e:
                print(f"Error: could not instantiate class '{class_name}': {e}")
                return []
    return result

def persist(params, result_json: str, strategy_name: str) -> str:
    user = params["parameters"]["user"][0]
    case_id = params["parameters"]["caseId"][0]
    con = sqlite3.connect("result.db")
    cur = con.cursor()
    cur.execute("CREATE TABLE IF NOT EXISTS result (id INTEGER PRIMARY KEY AUTOINCREMENT, user TEXT, strategy TEXT, user_id TEXT, timestamp DATETIME, result_json TEXT, parameters_json TEXT)")
    con.commit()
    del params["parameters"]["user"]
    stmt = "INSERT INTO result VALUES (?,?,?,?,?,?,?)"
    cur.execute(stmt, (None, user, strategy_name, case_id, datetime.now(timezone.utc), result_json, json.dumps(params)))
    con.commit()

def do_work_recursive(strategy_name, params, original_strategy_name, original_forwards) -> tuple[bool, str]:
    is_final_recursive_call = False
    next_strategy = f"{strategy_name}Strategy"
    
    forwards = params["forwards"]
    del params["forwards"]

    user = params["parameters"]["user"][0]
    case_id = params["parameters"]["caseId"][0]

    strategies = get_strategy(next_strategy)
    if len(strategies) == 0:
        msg = f"Error: no strategy '{next_strategy}' found!"
        return False, msg
    
    strategy_to_run = strategies[0]
    strategy_success, result = strategy_to_run.run(params["parameters"])
    if not strategy_success:
        msg = f"Error: something went wrong running strategy '{next_strategy}'"
        return False, msg
    elif len(forwards) > 0:
        next_strategy = forwards[0]["name"]
        next_params = {"forwards": forwards[1:], "parameters": {"user": [user], "caseId": [case_id]}}
        forward_mappings = forwards[0]["parameters"]
        for mapping in forward_mappings:
            from_param = mapping["from"]
            to_param = mapping["to"]
            if len(to_param) > 0:
                next_params["parameters"][to_param] = [result[from_param]]

        strategy_success, msg = do_work_recursive(next_strategy, next_params, original_strategy_name, original_forwards)
        if not strategy_success:
            msg = f"Error: something went wrong running strategy '{next_strategy}': {msg}\n"
            return False, msg
    else:
        is_final_recursive_call = True
    
    if is_final_recursive_call:
        params["forwards"] = original_forwards
        persist(params, json.dumps(result), strategy_name)
    return True, "All Good!"

def find_and_run_strategy(strategy_name: str, params: str) -> tuple[bool, str]:
    params_dict = json.loads(params)
    params_as_list = {}
    for k, v in params_dict["parameters"].items():
        params_as_list[k] = [v]
    if "repeated" in params_dict:
        repeated = params_dict["repeated"]
        for k, v in repeated.items():
            params_as_list[k] = v
        del params_dict["repeated"]
    params_dict["parameters"] = params_as_list

    if "forwards" in params_dict:
        forwards = params_dict["forwards"]
    else:
        return False, "No forwards provided!"

    return do_work_recursive(strategy_name = strategy_name,
                             params= params_dict, 
                             original_strategy_name = strategy_name, 
                             original_forwards = forwards)


if __name__ == '__main__':
    pass