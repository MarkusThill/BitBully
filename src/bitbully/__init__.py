"""BitBully package: A fast Python-based Connect-4 solver.

Homepage: https://github.com/MarkusThill/BitBully
"""

from .board import Board
from .solver import BitBully

__all__: list[str] = ["BitBully", "Board"]
__version__: str = "0.0.70"

# bitbully/__init__.py
__all__ = ["BitBully", "Board"]


def __getattr__(name: str) -> object:
    if name == "BitBully":
        from .solver import BitBully

        return BitBully
    if name == "Board":
        from .board import Board

        return Board
    raise AttributeError(name)
