from typing import Dict

def hello_world(text: str, parameters: Dict[str, str]) -> str:
    print(f"In Python: {parameters}")
    return "ok"