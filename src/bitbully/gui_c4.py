"""GUI module for the BitBully Connect-4 interactive widget."""

import importlib.resources
import logging
import textwrap
import time
from collections.abc import Sequence
from pathlib import Path

import matplotlib.backend_bases as mpl_backend_bases
import matplotlib.pyplot as plt
import numpy as np
import numpy.typing as npt
from IPython.display import Javascript, clear_output, display
from ipywidgets import AppLayout, Button, HBox, Layout, Output, VBox, widgets

from . import Board
from .agent_interface import Connect4Agent  # adjust if needed


class GuiC4:
    """A class which allows to create an interactive Connect-4 widget.

    GuiC4 is an interactive Connect-4 graphical user interface (GUI) implemented using
    Matplotlib, IPython widgets, and a backend agent from the BitBully engine. It
    provides the following main features:

    - Interactive Game Board: Presents a dynamic 6-row by 7-column
        Connect-4 board with clickable board cells.
    - Matplotlib Integration: Utilizes Matplotlib figures
        to render high-quality game visuals directly within Jupyter notebook environments.
    - User Interaction: Captures and processes mouse clicks and button events, enabling
        intuitive gameplay via either direct board interaction or button controls.
    - Undo/Redo Moves: Supports undo and redo functionalities, allowing users to
        navigate through their move history during gameplay.
    - Automated Agent Moves: Incorporates BitBully, a Connect-4 backend engine, enabling
        computer-generated moves and board evaluations.
    - Game State Handling: Detects game-over scenarios, including win/draw conditions,
        and provides immediate user feedback through popup alerts.

    Attributes:
        notify_output (widgets.Output): Output widget for notifications and popups.

    Examples:
            Generally, you should this method to retreive and display the widget.

            ```pycon
            >>> %matplotlib ipympl
            >>> c4gui = GuiC4()
            >>> display(c4gui.get_widget())
            ```

    """

    def __init__(
        self,
        agents: dict[str, Connect4Agent] | Sequence[Connect4Agent] | None = None,
        *,
        autoplay: bool = False,
    ) -> None:
        """Init the GuiC4 widget."""
        # Create a logger with the class name
        self.m_logger = logging.getLogger(self.__class__.__name__)
        self.m_logger.setLevel(logging.DEBUG)  # Set the logging level

        # Create a console handler (optional)
        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)  # Set level for the handler

        # Create a formatter and add it to the handler
        formatter = logging.Formatter("%(asctime)s - %(name)s - %(levelname)s - %(message)s")
        ch.setFormatter(formatter)

        # Add the handler to the logger
        if not self.m_logger.handlers:
            self.m_logger.addHandler(ch)

        # Avoid adding handlers multiple times
        self.m_logger.propagate = False
        assets_pth = Path(str(importlib.resources.files("bitbully").joinpath("assets")))
        png_empty = plt.imread(assets_pth.joinpath("empty.png"), format=None)
        png_empty_m = plt.imread(assets_pth.joinpath("empty_m.png"), format=None)
        png_empty_r = plt.imread(assets_pth.joinpath("empty_r.png"), format=None)
        png_red = plt.imread(assets_pth.joinpath("red.png"), format=None)
        png_red_m = plt.imread(assets_pth.joinpath("red_m.png"), format=None)
        png_yellow = plt.imread(assets_pth.joinpath("yellow.png"), format=None)
        png_yellow_m = plt.imread(assets_pth.joinpath("yellow_m.png"), format=None)
        self.m_png = {
            0: {"plain": png_empty, "corner": png_empty_m, "underline": png_empty_r},
            1: {"plain": png_yellow, "corner": png_yellow_m},
            2: {"plain": png_red, "corner": png_red_m},
        }

        self.m_n_row, self.m_n_col = 6, 7

        # TODO: probably not needed:
        self.m_height = np.zeros(7, dtype=np.int32)

        self.m_board_size = 3.5
        # self.m_player = 1
        self.is_busy = False

        # ---------------- multi-agent support ----------------
        self.autoplay = bool(autoplay)

        # Normalize `agents` into a dict[str, Connect4Agent]
        if agents is None:
            self.agents: dict[str, Connect4Agent] = {}
        elif isinstance(agents, dict):
            self.agents = dict(agents)
        else:
            self.agents = {f"agent{i + 1}": a for i, a in enumerate(agents)}

        self._agent_names: list[str] = list(self.agents.keys())

        # Which controller plays which color
        # values are either "human" or one of self._agent_names
        self.yellow_player: str = "human"
        self.red_player: str = self._agent_names[0] if self._agent_names else "human"

        # Which agent should be used for the "Evaluate" button.
        # Values: "auto" or one of self._agent_names (if any exist)
        self.eval_agent_choice: str = "auto"

        # Create board first
        self._create_board()

        # timing row (must exist before get_widget())
        self._create_timing_row()

        # Generate buttons for inserting the tokens:
        self._create_buttons()

        # Create control buttons
        self._create_control_buttons()

        # player selection dropdowns (must exist before get_widget())
        self._create_player_selectors()

        # evaluation row widget (must exist before get_widget())
        self._create_eval_row()

        # Capture clicks on the field
        _ = self.m_fig.canvas.mpl_connect("button_press_event", self._on_field_click)

        # Movelist
        self.m_movelist: list[tuple[int, int, int]] = []

        # Redo list
        self.m_redolist: list[tuple[int, int, int]] = []

        # Gameover flag:
        self.m_gameover = False

        # NEW: move list + copy position UI
        self._create_move_list_ui()

    def _create_player_selectors(self) -> None:
        """Create UI controls for player assignment, autoplay, and evaluation agent."""
        agent_options = list(self._agent_names)
        player_options = ["human", *agent_options]
        eval_options = ["auto", *agent_options]  # "auto" = use agent for side-to-move, else fallback

        # --- Player assignment ---
        self.dd_yellow = widgets.Dropdown(
            options=player_options,
            value=self.yellow_player if self.yellow_player in player_options else "human",
            description="Yellow:",
            layout=Layout(width="200px"),
        )
        self.dd_red = widgets.Dropdown(
            options=player_options,
            value=self.red_player if self.red_player in player_options else "human",
            description="Red:",
            layout=Layout(width="200px"),
        )

        # --- Autoplay toggle ---
        self.cb_autoplay = widgets.Checkbox(
            value=bool(self.autoplay),
            description="Autoplay",
            indent=False,
            layout=Layout(width="auto"),  # width="200px"
        )

        # --- Eval agent selection ---
        self.dd_eval_agent = widgets.Dropdown(
            options=eval_options,
            value=self.eval_agent_choice if self.eval_agent_choice in eval_options else "auto",
            description="Eval:",
            layout=Layout(width="200px"),
        )

        def _on_players_change(_change: object) -> None:
            self.yellow_player = str(self.dd_yellow.value)
            self.red_player = str(self.dd_red.value)
            self._update_insert_buttons()
            if self.autoplay:
                self._maybe_autoplay()

        def _on_autoplay_change(_change: object) -> None:
            self.autoplay = bool(self.cb_autoplay.value)
            # If autoplay is turned on mid-game, maybe immediately make the next agent move
            if self.autoplay:
                self._maybe_autoplay()

        def _on_eval_agent_change(_change: object) -> None:
            self.eval_agent_choice = str(self.dd_eval_agent.value)

        self.dd_yellow.observe(_on_players_change, names="value")
        self.dd_red.observe(_on_players_change, names="value")
        self.cb_autoplay.observe(_on_autoplay_change, names="value")
        self.dd_eval_agent.observe(_on_eval_agent_change, names="value")

        row1 = HBox(
            [self.dd_yellow, self.dd_red],
            layout=Layout(
                display="flex",
                flex_flow="row",
                justify_content="flex-start",
                align_items="center",
            ),
        )

        row2 = HBox(
            [self.cb_autoplay, self.dd_eval_agent],
            layout=Layout(
                display="flex",
                flex_flow="row",
                justify_content="flex-end",
                align_items="flex-end",
            ),
        )

        self.player_select_row = VBox(
            [row1, row2],
            layout=Layout(
                display="flex",
                flex_flow="column",
                justify_content="flex-end",
                align_items="flex-end",
            ),
        )

    def _create_timing_row(self) -> None:
        """Create a row that shows the computation time of the last agent move."""
        self.m_time_label = widgets.Label(
            value="",
            layout=Layout(width="100%"),
        )
        self.m_time_row = HBox(
            [self.m_time_label],
            layout=Layout(
                display="flex",
                flex_flow="row",
                justify_content="flex-start",
                align_items="center",
                width="100%",
            ),
        )

    def _create_move_list_ui(self) -> None:
        """Create the move list display and clipboard buttons."""
        self.ta_moves = widgets.Textarea(
            value="",
            description="",
            disabled=True,
            layout=Layout(width="100%", height="100%"),  # was "110px"
        )

        self.btn_copy_pos = Button(
            description="📋 Copy move sequence",
            tooltip="Copy the position string used by Board(...), e.g. '3431'",
            layout=Layout(width="100%"),
        )
        self.btn_copy_moves_ag = Button(
            description="📋 Copy ASCII board",
            tooltip="Copy the ascii representation of the board",
            layout=Layout(width="100%"),
        )

        # buttons_row = VBox(
        #    [self.btn_copy_pos, self.btn_copy_moves_ag],
        #    layout=Layout(width="100%"),
        # )

        self.btn_copy_pos.on_click(lambda _b: self._copy_position_string())
        self.btn_copy_moves_ag.on_click(lambda _b: self._copy_moves_ag())

        self.move_list_row = VBox(
            [self.btn_copy_pos, self.btn_copy_moves_ag, self.ta_moves],
            layout=Layout(
                width="200px",
                height="100%",  # NEW: take all available height in sidebar
                align_items="stretch",
                flex="1 1 auto",  # NEW: allow growing
            ),
        )

        # Make the textarea take the remaining space below the buttons row
        # buttons_row.layout = Layout(width="100%", flex="0 0 auto")
        # self.ta_moves.layout = Layout(width="100%", height="100%", flex="1 1 auto")

        self._update_move_list_ui()  # initialize

    def _position_string(self) -> str:
        """Return the position encoding compatible with Board(...).

        BitBully's Board examples use strings like "341" (columns as digits),
        so we follow the same convention.
        """
        return "".join(str(col) for (_p, col, _row) in self.m_movelist)

    def _moves_ag_string(self) -> str:
        """Return board as ascii string.."""
        # column 0..6 -> a..g
        # return " ".join(chr(ord("a") + col) for (_p, col, _row) in self.m_movelist)
        return textwrap.dedent(self._board_from_history().to_string()).strip()

    def _update_move_list_ui(self) -> None:
        """Refresh the move list textarea."""
        pos = self._position_string()
        ag = self._moves_ag_string()

        lines: list[str] = []
        lines.extend(
            [
                f"moves: {pos or '—'}",  #
                f"\nplies:  {len(self.m_movelist)}",  #
                f"\nboard:\n{ag or '—'}",
            ]
        )

        self.ta_moves.value = "\n".join(lines)

    def _copy_to_clipboard(self, text: str) -> None:
        """Copy text to clipboard in Jupyter (best-effort)."""
        # Works in most modern Jupyter setups; if clipboard is blocked, it just won't copy.
        js = Javascript(
            f"""
            (async () => {{
            try {{
                await navigator.clipboard.writeText({text!r});
            }} catch (e) {{
                // Fallback for stricter environments
                const ta = document.createElement('textarea');
                ta.value = {text!r};
                document.body.appendChild(ta);
                ta.select();
                document.execCommand('copy');
                document.body.removeChild(ta);
            }}
            }})();
            """
        )
        display(js)

    def _copy_position_string(self) -> None:
        pos = self._position_string()
        self._copy_to_clipboard(pos)

    def _copy_moves_ag(self) -> None:
        ag = self._moves_ag_string()
        self._copy_to_clipboard(ag)

    # TODO: a bit hacky, use board instance instead?
    def _current_player(self) -> int:
        """Return player to move: 1 (Yellow) starts, then alternates."""
        return 1 if (len(self.m_movelist) % 2 == 0) else 2

    def _controller_for_player(self, player: int) -> str:
        return self.yellow_player if player == 1 else self.red_player

    def _agent_for_player(self, player: int) -> Connect4Agent | None:
        controller = self._controller_for_player(player)
        if controller == "human":
            return None
        return self.agents.get(controller)

    def _agent_for_evaluation(self) -> Connect4Agent | None:
        """Return the agent used for the Evaluate button based on dropdown selection."""
        if not self._agent_names:
            return None

        choice = getattr(self, "eval_agent_choice", "auto")
        if choice != "auto":
            return self.agents.get(choice)

        # "auto": prefer the agent controlling the side to move; fallback to first agent
        player = self._current_player()
        agent = self._agent_for_player(player)
        return agent or self.agents[self._agent_names[0]]

    def _reset(self) -> None:
        self.m_movelist = []
        self.m_redolist = []
        self.m_height = np.zeros(7, dtype=np.int32)
        self.m_gameover = False

        for im in self.ims:
            im.set_data(self.m_png[0]["plain"])

        self.m_fig.canvas.draw_idle()
        self.m_fig.canvas.flush_events()
        self._update_insert_buttons()
        self._clear_eval_row()
        self.m_time_label.value = ""
        self._update_move_list_ui()

    def _get_fig_size_px(self) -> npt.NDArray[np.float64]:
        # Get the size in inches
        size_in_inches = self.m_fig.get_size_inches()
        self.m_logger.debug("Figure size in inches: %f", size_in_inches)

        # Get the DPI
        dpi = self.m_fig.dpi
        self.m_logger.debug("Figure DPI: %d", dpi)

        # Convert to pixels
        return size_in_inches * dpi

    def _create_control_buttons(self) -> None:
        self.m_control_buttons = {}

        # Create buttons for each column
        self.m_logger.debug("Figure size: %s", self._get_fig_size_px())

        fig_size_px = self._get_fig_size_px()
        wh = f"{-3 + (fig_size_px[1] / self.m_n_row)}px"
        btn_layout = Layout(height=wh, width=wh)

        button = Button(description="🔄", tooltip="Reset Game", layout=btn_layout)
        button.on_click(lambda b: self._reset())
        self.m_control_buttons["reset"] = button

        button = Button(description="↩️", tooltip="Undo Move", layout=btn_layout)
        button.disabled = True
        button.on_click(lambda b: self._undo_move())
        self.m_control_buttons["undo"] = button

        button = Button(description="↪️", tooltip="Redo Move", layout=btn_layout)
        button.disabled = True
        button.on_click(lambda b: self._redo_move())
        self.m_control_buttons["redo"] = button

        button = Button(description="🕹️", tooltip="Computer Move", layout=btn_layout)
        button.on_click(lambda b: self._computer_move())
        self.m_control_buttons["move"] = button

        button = Button(description="📊", tooltip="Evaluate Board", layout=btn_layout)
        button.on_click(lambda b: self._evaluate_board())
        self.m_control_buttons["evaluate"] = button

        # ---------------- evaluation widgets ----------------

    def _create_eval_row(self) -> None:
        """Create a row of 7 labels to display per-column evaluation scores."""
        fig_size_px = self._get_fig_size_px()
        width = f"{-3 + (fig_size_px[0] / self.m_n_col)}px"

        self.m_eval_labels: list[widgets.Label] = [
            widgets.Label(
                value="",
                layout=Layout(justify_content="center", align_items="center", width=width),
            )
            for _ in range(self.m_n_col)
        ]
        self.m_eval_row = HBox(
            self.m_eval_labels,
            layout=Layout(
                display="flex",
                flex_flow="row wrap",
                justify_content="center",
                align_items="center",
            ),
        )

    def _clear_eval_row(self) -> None:
        """Clear all evaluation score labels."""
        for lbl in self.m_eval_labels:
            lbl.value = ""

    def _evaluate_board(self) -> None:
        """Compute and display per-column evaluation scores for the current position."""
        if self.is_busy:
            return

        self.is_busy = True
        self._update_insert_buttons()

        try:
            board = self._board_from_history()

            # If you want: show blanks for illegal moves.
            # Compute scores for all 7 columns.
            # scores = self.agent.score_all_moves(board)  # -> Sequence[int] of length 7
            agent = self._agent_for_evaluation()
            if agent is None:
                self._clear_eval_row()
                return

            t0 = time.perf_counter()
            scores = agent.score_all_moves(board)
            dt_ms = (time.perf_counter() - t0) * 1000.0

            # Update timing row
            # Identify agent name for display
            if self.eval_agent_choice != "auto":
                agent_name = self.eval_agent_choice
            else:
                # auto → agent for side-to-move or fallback
                player = self._current_player()
                agent_name = self._controller_for_player(player)
                if agent_name == "human":
                    agent_name = self._agent_names[0]
            self.m_time_label.value = f"📊 Evaluation: {agent_name} — ⏱️ {dt_ms:.1f} ms"

            # Fill the label row. (Optionally blank-out illegal moves)
            legal = set(board.legal_moves())
            for col in range(self.m_n_col):
                if col in legal:
                    self.m_eval_labels[col].value = str(int(scores[col]))
                else:
                    self.m_eval_labels[col].value = "—"

        except Exception as e:
            self.m_logger.error("Evaluation failed: %s", str(e))
            # Optional: popup on error
            # self._popup(f"Evaluation failed: {e}")
            self._clear_eval_row()
            raise
        finally:
            self.is_busy = False
            self._update_insert_buttons()

    def _computer_move(self) -> None:
        if self.is_busy or self.m_gameover:
            return

        player = self._current_player()
        agent = self._agent_for_player(player)
        if agent is None:
            # It's a human-controlled side
            return

        # Identify which agent name is playing this side (for display)
        agent_name = self._controller_for_player(player)

        self.is_busy = True
        self._update_insert_buttons()
        try:
            b = self._board_from_history()

            t0 = time.perf_counter()
            best_move = agent.best_move(b)
            dt_ms = (time.perf_counter() - t0) * 1000.0

            # Update timing row (only if it was an agent move)
            color = "Yellow" if player == 1 else "Red"
            self.m_time_label.value = f"🕹️ Last move: {color} ({agent_name}) — ⏱️ {dt_ms:.1f} ms"

        finally:
            self.is_busy = False

        # Perform move (this will re-disable/re-enable buttons as usual)
        self._insert_token(best_move)

    def _create_board(self) -> None:
        self.output = Output()

        with self.output:
            fig, axs = plt.subplots(
                self.m_n_row,
                self.m_n_col,
                figsize=(
                    self.m_board_size / self.m_n_row * self.m_n_col,
                    self.m_board_size,
                ),
            )
            axs = axs.flatten()
            self.ims = []
            for ax in axs:
                self.ims.append(ax.imshow(self.m_png[0]["plain"], animated=True))
                ax.axis("off")
                ax.set_xticklabels([])
                ax.set_yticklabels([])

            fig.tight_layout(pad=0.1)
            plt.subplots_adjust(wspace=0.05, hspace=0.05, left=0.01, right=0.99, top=0.99, bottom=0.01)
            fig.suptitle("")
            fig.set_facecolor("darkgray")
            fig.canvas.toolbar_visible = False  # type: ignore[attr-defined]
            fig.canvas.resizable = False  # type: ignore[attr-defined]
            fig.canvas.toolbar_visible = False  # type: ignore[attr-defined]
            fig.canvas.header_visible = False  # type: ignore[attr-defined]
            fig.canvas.footer_visible = False  # type: ignore[attr-defined]
            fig.canvas.capture_scroll = True  # type: ignore[attr-defined]
            plt.show(block=False)

        self.m_fig = fig
        self.m_axs = axs

    notify_output: widgets.Output = widgets.Output()
    display(notify_output)

    @notify_output.capture()
    def _popup(self, text: str) -> None:
        clear_output()
        display(Javascript(f"alert('{text}')"))

    def _board_from_history(self) -> Board:
        return Board([mv[1] for mv in self.m_movelist])

    def _insert_token(self, col: int, reset_redo_list: bool = True) -> None:
        if self.is_busy:
            return
        self.is_busy = True

        for button in self.m_insert_buttons:
            button.disabled = True

        board = self._board_from_history()
        if self.m_gameover or not board.play(int(col)):
            self.is_busy = False
            self._update_insert_buttons()
            return

        try:
            # Get player
            player = 1 if not self.m_movelist else 3 - self.m_movelist[-1][0]
            self.m_movelist.append((player, col, self.m_height[col]))
            self._update_move_list_ui()
            self._paint_token()
            self.m_height[col] += 1

            # Usually, after a move is performed, there is no possibility to
            # redo a move again
            if reset_redo_list:
                self.m_redolist = []

            self._check_winner(board)
            # clear eval row because the position changed
            self._clear_eval_row()

        except Exception as e:
            self.m_logger.error("Error: %s", str(e))
            raise
        finally:
            time.sleep(0.25)  # debounce button
            # Re-enable all buttons (if columns not full)
            self.is_busy = False
            self._update_insert_buttons()
            if self.autoplay:
                self._maybe_autoplay()

    def _redo_move(self) -> None:
        if len(self.m_redolist) < 1:
            return
        _p, col, _row = self.m_redolist.pop()
        self._insert_token(col, reset_redo_list=False)

    def _undo_move(self) -> None:
        if len(self.m_movelist) < 1:
            return

        if self.is_busy:
            return
        self.is_busy = True

        try:
            _p, col, row = mv = self.m_movelist.pop()
            self.m_redolist.append(mv)

            self.m_height[col] -= 1
            assert row == self.m_height[col]

            img_idx = self._get_img_idx(col, row)

            self.ims[img_idx].set_data(self.m_png[0]["plain"])
            self.m_axs[img_idx].draw_artist(self.ims[img_idx])

            self._update_move_list_ui()
            if len(self.m_movelist) > 0:
                self._paint_token()
            else:
                self.m_fig.canvas.blit(self.ims[img_idx].get_clip_box())
                self.m_fig.canvas.flush_events()

            self.m_gameover = False

            # clear eval row because the position changed
            self._clear_eval_row()

        except Exception as e:
            self.m_logger.error("Error: %s", str(e))
            raise
        finally:
            # Re-enable all buttons (if columns not full)
            self.is_busy = False
            self._update_insert_buttons()

            time.sleep(0.25)  # debounce button

    def _update_insert_buttons(self) -> None:
        player = self._current_player()
        human_turn = self._controller_for_player(player) == "human"

        # ⏬ buttons
        for button, col in zip(self.m_insert_buttons, range(self.m_n_col)):
            # disable if column full OR gameover/busy OR not a human turn
            button.disabled = (
                bool(self.m_height[col] >= self.m_n_row) or self.m_gameover or self.is_busy or (not human_turn)
            )

        # ↩️ button
        self.m_control_buttons["undo"].disabled = len(self.m_movelist) < 1 or self.is_busy

        # ↪️ button
        self.m_control_buttons["redo"].disabled = len(self.m_redolist) < 1 or self.is_busy

        # 🕹️ only makes sense if current side is agent-controlled
        self.m_control_buttons["move"].disabled = (
            self.m_gameover or self.is_busy or (self._agent_for_player(player) is None)
        )

        # 📊 enable only if we have at least one agent to evaluate with
        self.m_control_buttons["evaluate"].disabled = self.m_gameover or self.is_busy or (len(self.agents) == 0)

        if hasattr(self, "dd_eval_agent"):
            self.dd_eval_agent.disabled = (len(self.agents) == 0) or self.is_busy
        if hasattr(self, "cb_autoplay"):
            self.cb_autoplay.disabled = self.is_busy

    def _get_img_idx(self, col: int, row: int) -> int:
        """Translates a column and row ID into the corresponding image ID.

        Args:
            col (int): column (0-6) of the considered board cell.
            row (int): row (0-5) of the considered board cell.

        Returns:
            int: The corresponding image id (0-41).
        """
        self.m_logger.debug("Got column: %d", col)

        return col % self.m_n_col + (self.m_n_row - row - 1) * self.m_n_col

    def _paint_token(self) -> None:
        if len(self.m_movelist) < 1:
            return

        p, col, row = self.m_movelist[-1]
        img_idx = self._get_img_idx(col, row)
        self.m_logger.debug("Paint token: %d", img_idx)

        #
        # no need to reset background, since we anyhow overwrite it again
        # self.m_fig.canvas.restore_region(self.m_background[img_idx])
        self.ims[img_idx].set_data(self.m_png[p]["corner"])

        # see: https://matplotlib.org/3.4.3/Matplotlib.pdf
        #      2.3.1 Faster rendering by using blitting
        blit_boxes = []
        self.m_axs[img_idx].draw_artist(self.ims[img_idx])
        blit_boxes.append(self.ims[img_idx].get_clip_box())
        # self.m_fig.canvas.blit()

        if len(self.m_movelist) > 1:
            # Remove the white corners for the second-to-last move
            # TODO: redundant code above
            p, col, row = self.m_movelist[-2]
            img_idx = self._get_img_idx(col, row)
            self.ims[img_idx].set_data(self.m_png[p]["plain"])
            self.m_axs[img_idx].draw_artist(self.ims[img_idx])
            blit_boxes.append(self.ims[img_idx].get_clip_box())

        self.m_fig.canvas.blit(blit_boxes[0])

        # self.m_fig.canvas.restore_region(self.m_background[img_idx])
        # self.m_fig.canvas.blit(self.ims[img_idx].get_clip_box())
        # self.m_fig.canvas.draw_idle()
        self.m_fig.canvas.flush_events()

    def _create_buttons(self) -> None:
        # Create buttons for each column
        self.m_logger.debug("Figure size: %s", self._get_fig_size_px())

        fig_size_px = self._get_fig_size_px()

        self.m_insert_buttons = []
        for col in range(self.m_n_col):
            button = Button(
                description="⏬",
                layout=Layout(width=f"{-4 + (fig_size_px[0] / self.m_n_col)}px", height="50px"),
            )
            button.on_click(lambda b, col=col: self._insert_token(col))
            self.m_insert_buttons.append(button)

    def _create_column_labels(self) -> HBox:
        """Creates a row with the column labels 'a' to 'g'.

        Returns:
            HBox: A row of textboxes containing the columns labels 'a' to 'g'.
        """
        fig_size_px = self._get_fig_size_px()
        width = f"{-3 + (fig_size_px[0] / self.m_n_col)}px"
        textboxes = [
            widgets.Label(
                value=chr(ord("a") + i),
                layout=Layout(justify_content="center", align_items="center", width=width),
            )
            for i in range(self.m_n_col)
        ]
        return HBox(
            textboxes,
            layout=Layout(
                display="flex",
                flex_flow="row wrap",  # or "column" depending on your layout needs
                justify_content="center",  # Left alignment
                align_items="center",  # Top alignment
            ),
        )

    def _on_field_click(self, event: mpl_backend_bases.Event) -> None:
        """Based on the column where the click was detected, insert a token.

        Args:
            event (mpl_backend_bases.Event): A matplotlib mouse event.
        """
        if self._controller_for_player(self._current_player()) != "human":
            return

        if not isinstance(event, mpl_backend_bases.MouseEvent):
            return
        if event.inaxes is None or event.xdata is None:
            return
        if isinstance(event, mpl_backend_bases.MouseEvent):
            ix, iy = event.xdata, event.ydata
            self.m_logger.debug("click (x,y): %d, %d", ix, iy)
            idx = np.where(self.m_axs == event.inaxes)[0][0] % self.m_n_col
            self._insert_token(idx)

    def get_widget(self) -> AppLayout:
        """Get the widget.

        Examples:
            Generally, you should this method to retreive and display the widget.

            ```pycon
            >>> %matplotlib ipympl
            >>> c4gui = GuiC4()
            >>> display(c4gui.get_widget())
            ```

        Returns:
            AppLayout: the widget.
        """
        # Arrange buttons in a row
        insert_button_row = HBox(
            [VBox(layout=Layout(padding="0px 0px 0px 6px")), *self.m_insert_buttons],
            layout=Layout(
                display="flex",
                flex_flow="row wrap",  # or "column" depending on your layout needs
                justify_content="center",  # Left alignment
                align_items="center",  # Top alignment
            ),
        )
        control_buttons_col = HBox(
            [VBox(list(self.m_control_buttons.values()))],
            layout=Layout(
                display="flex",
                flex_flow="row wrap",  # or "column" depending on your layout needs
                justify_content="flex-end",
                align_items="center",  # bottom alignment
            ),
        )

        # deactivate for now
        # tb = self._create_column_labels()

        right = VBox(
            [self.move_list_row],
            layout=Layout(
                display="flex",
                flex_flow="column",
                justify_content="flex-start",
                align_items="stretch",
                width="200px",
                height="90%",  # NEW: fill AppLayout height
                flex="1 1 auto",  # NEW: allow it to grow
            ),
        )

        main = HBox(
            [
                VBox(
                    [
                        self.player_select_row,
                        insert_button_row,
                        self.output,
                        self.m_eval_row,
                        self.m_time_row,
                    ],
                    layout=Layout(
                        display="flex",
                        flex_flow="column",
                        align_items="flex-start",
                    ),
                ),
                right,
            ],
            layout=Layout(
                display="flex",
                flex_flow="row",
                align_items="flex-start",
                justify_content="flex-start",
                gap="5px",  # space between board and sidebar
                width="100%",
            ),
        )

        return AppLayout(
            header=None,
            left_sidebar=control_buttons_col,
            center=main,
            right_sidebar=None,  # <= important
            footer=None,
            layout=Layout(grid_gap="0px"),
        )

    def _maybe_autoplay(self) -> None:
        """If it's an agent-controlled turn, immediately play its move."""
        if self.is_busy or self.m_gameover:
            return
        if self._agent_for_player(self._current_player()) is None:
            return
        self._computer_move()

    def _check_winner(self, board: Board) -> None:
        """Check for Win or draw."""
        if board.has_win():
            winner = "Yellow" if board.winner() == 1 else "Red"
            self._popup(f"Game over! {winner} wins!")
            self.m_gameover = True
        elif board.is_full():
            self._popup("Game over! Draw!")
            self.m_gameover = True

    def destroy(self) -> None:
        """Destroy and release the acquired resources."""
        plt.close(self.m_fig)
        del self.agents
        del self.m_axs
        del self.m_fig
        del self.output
