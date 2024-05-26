from typing import List

def hello_world(text: str, names: List[str]) -> str:
    names_joined = ",".join(names)
    message = f"{text} {names_joined}!"
    print(f"In Python: {message}")
    return message+"done"