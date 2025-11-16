"""GUI module for the BitBully Connect-4 interactive widget."""

import importlib.resources
import logging
import time
from pathlib import Path

import matplotlib.backend_bases as mpl_backend_bases
import matplotlib.pyplot as plt
import numpy as np
import numpy.typing as npt
from IPython.display import Javascript, clear_output, display
from ipywidgets import AppLayout, Button, HBox, Layout, Output, VBox, widgets

from bitbully import bitbully_core


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

    def __init__(self) -> None:
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

        self.last_event_time = time.time()

        # Create board first
        self._create_board()

        # Generate buttons for inserting the tokens:
        self._create_buttons()

        # Create control buttons
        self._create_control_buttons()

        # Capture clicks on the field
        _ = self.m_fig.canvas.mpl_connect("button_press_event", self._on_field_click)

        # Movelist
        self.m_movelist: list[tuple[int, int, int]] = []

        # Redo list
        self.m_redolist: list[tuple[int, int, int]] = []

        # Gameover flag:
        self.m_gameover = False

        # C4 agent
        import bitbully_databases as bbd

        # TODO: allow choosing opening book
        db_path: str = bbd.BitBullyDatabases.get_database_path("12-ply-dist")
        self.bitbully_agent = bitbully_core.BitBullyCore(Path(db_path))

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
        self.m_logger.debug("Figure size: ", self._get_fig_size_px())

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
        self.m_control_buttons["evaluate"] = button

    def _computer_move(self) -> None:
        self.is_busy = True
        self._update_insert_buttons()
        b = bitbully_core.BoardCore()
        assert b.setBoard([mv[1] for mv in self.m_movelist])
        move_scores = self.bitbully_agent.scoreMoves(b)
        self.is_busy = False
        self._insert_token(int(np.argmax(move_scores)))

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

            fig.tight_layout()
            plt.subplots_adjust(wspace=0.05, hspace=0.05, left=0.0, right=1.0, top=1.0, bottom=0.0)
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

    def _is_legal_move(self, col: int) -> bool:
        return not self.m_height[col] >= self.m_n_row

    def _insert_token(self, col: int, reset_redo_list: bool = True) -> None:
        if self.is_busy:
            return
        self.is_busy = True

        for button in self.m_insert_buttons:
            button.disabled = True

        board = bitbully_core.BoardCore()
        board.setBoard([mv[1] for mv in self.m_movelist])
        if self.m_gameover or not board.play(col):
            self._update_insert_buttons()
            self.is_busy = False
            return

        try:
            # Get player
            player = 1 if not self.m_movelist else 3 - self.m_movelist[-1][0]
            self.m_movelist.append((player, col, self.m_height[col]))
            self._paint_token()
            self.m_height[col] += 1

            # Usually, after a move is performed, there is no possibility to
            # redo a move again
            if reset_redo_list:
                self.m_redolist = []

            self._check_winner(board)

        except Exception as e:
            self.m_logger.error("Error: %s", str(e))
            raise
        finally:
            time.sleep(0.5)  # debounce button
            # Re-enable all buttons (if columns not full)
            self.is_busy = False
            self._update_insert_buttons()

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
            if len(self.m_movelist) > 0:
                self._paint_token()
            else:
                self.m_fig.canvas.blit(self.ims[img_idx].get_clip_box())
                self.m_fig.canvas.flush_events()

            self.m_gameover = False

        except Exception as e:
            self.m_logger.error("Error: %s", str(e))
            raise
        finally:
            # Re-enable all buttons (if columns not full)
            self.is_busy = False
            self._update_insert_buttons()

            time.sleep(0.5)  # debounce button

    def _update_insert_buttons(self) -> None:
        for button, col in zip(self.m_insert_buttons, range(self.m_n_col)):
            button.disabled = bool(self.m_height[col] >= self.m_n_row) or self.m_gameover or self.is_busy

        self.m_control_buttons["undo"].disabled = len(self.m_movelist) < 1 or self.is_busy
        self.m_control_buttons["redo"].disabled = len(self.m_redolist) < 1 or self.is_busy
        self.m_control_buttons["move"].disabled = self.m_gameover or self.is_busy
        self.m_control_buttons["evaluate"].disabled = self.m_gameover or self.is_busy

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
        self.m_logger.debug("Figure size: ", self._get_fig_size_px())

        fig_size_px = self._get_fig_size_px()

        self.m_insert_buttons = []
        for col in range(self.m_n_col):
            button = Button(
                description="⏬",
                layout=Layout(width=f"{-3 + (fig_size_px[0] / self.m_n_col)}px", height="50px"),
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
            self.m_insert_buttons,
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
                justify_content="flex-end",  # Left alignment
                align_items="center",  # Top alignment
            ),
        )

        tb = self._create_column_labels()

        return AppLayout(
            header=None,
            left_sidebar=control_buttons_col,
            center=VBox(
                [insert_button_row, self.output, tb],
                layout=Layout(
                    display="flex",
                    flex_flow="column wrap",
                    justify_content="flex-start",  # Left alignment
                    align_items="flex-start",  # Top alignment
                ),
            ),
            footer=None,
            right_sidebar=None,
        )

    def _check_winner(self, board: bitbully_core.BoardCore) -> None:
        """Check for Win or draw."""
        if board.hasWin():
            winner = "Yellow" if board.movesLeft() % 2 else "Red"
            self._popup(f"Game over! {winner} wins!")
            self.m_gameover = True
        if board.movesLeft() == 0:
            self._popup("Game over! Draw!")
            self.m_gameover = True

    def destroy(self) -> None:
        """Destroy and release the acquired resources."""
        plt.close(self.m_fig)
        del self.bitbully_agent
        del self.m_axs
        del self.m_fig
        del self.output
