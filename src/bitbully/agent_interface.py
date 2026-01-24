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

    Example:
        Minimal agent compatible with the `Connect4Agent` protocol:

        ```python
        import random
        from bitbully import Board

        # Importing the Protocol is optional at runtime, but useful for:
        #  - static type checking (mypy / pyright)
        #  - documenting that this class satisfies the agent interface
        from bitbully.agent_protocols import Connect4Agent


        class RandomAgent:
            '''Agent that plays a random legal move.

            This class does NOT inherit from ``Connect4Agent``.
            It is compatible because it implements the required methods
            with matching signatures (structural typing).
            '''

            def score_all_moves(self, board: Board) -> dict[int, int]:
                # Only legal columns may appear in the result.
                # The GUI and other consumers rely on this contract.
                return {c: 0 for c in board.legal_moves()}

            def best_move(self, board: Board) -> int:
                # Consumers may call only ``best_move`` if they are
                # not interested in individual move scores.
                return random.choice(board.legal_moves())


        board = Board("332311")

        # The variable annotation enforces the protocol at type-check time.
        agent: Connect4Agent = RandomAgent()

        move = agent.best_move(board)
        board.play(move)
        ```
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
