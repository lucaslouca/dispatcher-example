from typing import Dict

def run_query(query_name: str, parameters: Dict[str, str]) -> str:
    print(f"In Python: {parameters}")
    return "ok"