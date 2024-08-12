from typing import Dict, List

class LocationStrategy:
    def run(self, parameters: Dict[str, List[str]])-> bool:
        print(f"Python: Location with parameters={parameters}")
        result = {'loc': 'London', 'rating': 3.5}
        return True, result