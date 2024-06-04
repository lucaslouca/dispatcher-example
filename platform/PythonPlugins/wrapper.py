from typing import Dict

from strategy_finder import find_and_run_strategy
def run_query(strategy_name: str, parameters: Dict[str, str]) -> bool:
    if (find_and_run_strategy(strategy_name, parameters)):
        return "ok"
    else:
        return "fail"