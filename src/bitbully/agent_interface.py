"""Agent interface definitions for Connect-4.

This module defines the minimal, structural interface that Connect-4 agents must
implement in order to be compatible with the interactive GUI and other
high-level components. The interface is expressed using ``typing.Protocol`` to
enable static type checking without requiring inheritance or tight coupling
between agents and consumers.
"""

from __future__ import annotations

from typing import Protocol, runtime_checkable

from . import Board


@runtime_checkable
class Connect4Agent(Protocol):
    """Minimal interface a Connect-4 agent must implement to work with ``GuiC4``.

    This interface is intentionally aligned with the public ``BitBully`` API,
    but excludes BitBully-specific engine features such as opening-book handling,
    node counters, transposition tables, and specialized search entry points.

    Required methods:
        - ``score_all_moves``: Provide integer evaluations for all *legal* moves.
        - ``best_move``: Select one legal move using BitBully-compatible
          tie-breaking semantics.

    Notes on scoring:
        - Scores are integers where **larger values are better** for the side to move.
        - The absolute scale is agent-defined.
        - The GUI only relies on *relative ordering* and legality.
    """

    def score_all_moves(self, board: Board) -> dict[int, int]:
        """Score all legal moves for the given board state.

        Args:
            board (Board):
                Current Connect-4 board position.

        Returns:
            dict[int, int]:
                Mapping ``{column: score}`` for all *legal* columns (0..6).
                Columns that are full or illegal **must not** appear in the mapping.

        Notes:
            - Higher scores indicate better moves.
            - The returned dictionary may contain between 0 and 7 entries.
        """
        ...

    def best_move(
        self,
        board: Board,
    ) -> int:
        """Return the best legal move (column index) for the side to move.

        Args:
            board (Board):
                Current Connect-4 board position.

        Returns:
            int:
                Selected column index in the range ``0..6``.
        """
        ...

    def score_move(self, board: Board, column: int, first_guess: int = 0) -> int:
        """Evaluate a single legal move for the given board state.

        This method is optional and not required by the GUI, but can be useful
        for agents that support fine-grained move evaluation.

        Args:
            board (Board):
                Current Connect-4 board position.
            column (int):
                Column index (0..6) of the move to evaluate.
            first_guess (int):
                Optional initial guess for iterative or search-based agents.
                Implementations may safely ignore this parameter.

        Returns:
            int:
                Evaluation score for the given move.
        """
        ...
