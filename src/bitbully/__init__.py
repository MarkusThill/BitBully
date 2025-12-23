"""BitBully package: A fast Python-based Connect-4 solver.

Homepage: https://github.com/MarkusThill/BitBully
"""

from .board import Board
from .solver import BitBully

__all__: list[str] = ["BitBully", "Board"]
__version__: str = "0.0.62"
