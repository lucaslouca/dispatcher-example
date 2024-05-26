# cython: language_level=3
import sys
import os
dir_path = os.path.dirname(os.path.realpath(__file__))
sys.path.append(f"{dir_path}/PythonPlugins") # Update list of directories that the Python interpreter will search for when it tries to resolve a module name

from hello import hello_world

cdef public char* call_hello_world(char* message, list names):
    res = hello_world(message.decode('UTF-8'), names)
    return res.encode('UTF-8')