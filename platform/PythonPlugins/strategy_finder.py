from typing import Dict
import glob
import os
import importlib
from pathlib import Path

def instantiate(module_name):
    module = importlib.import_module(module_name)
    class_name = module_name
    try:
        my_class = getattr(module, class_name)
    except Exception as e:
        return None
    my_instance = my_class()
    return my_instance

def find_and_run_strategy(strategy_name: str, parameters: Dict[str, str]) -> bool:
    for p in glob.glob(glob.escape(os.getcwd()) + "/PythonPlugins/*Strategy.py"):
        file_name = os.path.basename(p)
        if file_name == f"{strategy_name}Strategy.py":
            class_name = Path(file_name).stem
            strategy_instance = instantiate(class_name)
            if strategy_instance:
                strategy_instance.run(parameters)
                return True
    return False

if __name__ == '__main__':
    find_and_run_strategy(strategy_name="Location", parameters={})