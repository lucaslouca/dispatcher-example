from typing import Dict, List

from strategy_finder import find_and_run_strategy
def do_work(strategy_name: str, parameters: str) -> tuple[bool, str]:
    return find_and_run_strategy(strategy_name, parameters)