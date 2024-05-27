# cython: language_level=3
import sys
import os
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(f"{dir_path}/PythonPlugins") # Update list of directories that the Python interpreter will search for when it tries to resolve a module name

from wrapper import run_query

cdef public char* call_hello_world(const char* query_name, dict parameters):
    res = run_query(query_name.decode('UTF-8'), parameters)
    return res.encode('UTF-8')