# cython: language_level=3
import sys
import os
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(f"{dir_path}/PythonPlugins") # Update list of directories that the Python interpreter will search for when it tries to resolve a module name

from wrapper import do_work

cdef public char* call_do_work(const char* strategy_name, const char* params):
    try:
        success, msg = do_work(strategy_name.decode("UTF-8"), params.decode("UTF-8"))
        return msg.encode("UTF-8")
    except Exception as e:
        msg = f"Error: {e}"
        return msg.encode("UTF-8")