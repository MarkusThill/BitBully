import matplotlib.pyplot as plt
import numpy as np
import time
import logging
from ipywidgets import widgets
from IPython.display import Javascript, display, clear_output
from ipywidgets import Button, VBox, HBox, Output, Layout
from ipywidgets import AppLayout
import bitbully.bitbully_core as exp
import importlib.resources


class GuiC4:
    def __init__(self):
        # Create a logger with the class name
        self.m_logger = logging.getLogger(self.__class__.__name__)
        self.m_logger.setLevel(logging.DEBUG)  # Set the logging level

        # Create a console handler (optional)
        ch = logging.StreamHandler()
        ch.setLevel(logging.INFO)  # Set level for the handler

        # Create a formatter and add it to the handler
        formatter = logging.Formatter(
            "%(asctime)s - %(name)s - %(levelname)s - %(message)s"
        )
        ch.setFormatter(formatter)

        # Add the handler to the logger
        self.m_logger.addHandler(ch)

        # Avoid adding handlers multiple times
        self.m_logger.propagate = False
        with (
            importlib.resources.files("bitbully")
            .joinpath("assets/empty.png")
            .open("rb") as file
        ):
            png_empty = plt.imread(file, format=None)
        with (
            importlib.resources.files("bitbully")
            .joinpath("assets/empty_m.png")
            .open("rb") as file
        ):
            png_empty_m = plt.imread(file, format=None)
        with (
            importlib.resources.files("bitbully")
            .joinpath("assets/empty_r.png")
            .open("rb") as file
        ):
            png_empty_r = plt.imread(file, format=None)
        with (
            importlib.resources.files("bitbully")
            .joinpath("assets/red.png")
            .open("rb") as file
        ):
            png_red = plt.imread(file, format=None)
        with (
            importlib.resources.files("bitbully")
            .joinpath("assets/red_m.png")
            .open("rb") as file
        ):
            png_red_m = plt.imread(file, format=None)
        with (
            importlib.resources.files("bitbully")
            .joinpath("assets/yellow.png")
            .open("rb") as file
        ):
            png_yellow = plt.imread(file, format=None)
        with (
            importlib.resources.files("bitbully")
            .joinpath("assets/yellow_m.png")
            .open("rb") as file
        ):
            png_yellow_m = plt.imread(file, format=None)
        self.m_png = {
            0: {"plain": png_empty, "corner": png_empty_m, "underline": png_empty_r},
            1: {"plain": png_yellow, "corner": png_yellow_m},
            2: {"plain": png_red, "corner": png_red_m},
        }

        self.m_n_row, self.m_n_col = 6, 7
        # self.m_board = np.zeros( (7,6) )

        # TODO: probably not needed:
        self.m_height = np.zeros(7, dtype=np.int32)

        self.m_board_size = 3.5
        # self.m_player = 1
        self.is_busy = False

        self.last_event_time = time.time()

        # Create board first
        self.create_board()

        # Generate buttons for inserting the tokens:
        self.create_buttons()

        # Create control buttons
        self.create_control_buttons()

        # Capture clicks on the field
        _ = self.m_fig.canvas.mpl_connect("button_press_event", self.on_field_click)

        # Movelist
        self.m_movelist = []

        # Redo list
        self.m_redolist = []

        self.m_board = exp.Board()

    def reset(self):
        self.m_movelist = []
        self.m_redolist = []
        self.m_height = np.zeros(7, dtype=np.int32)

        for im in self.ims:
            im.set_data(self.m_png[0]["plain"])

        self.m_fig.canvas.draw_idle()
        self.m_fig.canvas.flush_events()
        self.update_insert_buttons()

    def get_fig_size_px(self):
        # Get the size in inches
        size_in_inches = self.m_fig.get_size_inches()
        self.m_logger.debug(f"Figure size in inches: {size_in_inches}")

        # Get the DPI
        dpi = self.m_fig.dpi
        self.m_logger.debug(f"Figure DPI: {dpi}")

        # Convert to pixels
        fig_size_in_pixels = size_in_inches * dpi

        # Alternatively:
        # self.m_logger.debug(f"Figure size in pixels: {fig_size_in_pixels}")
        # bbox = fig.get_window_extent().transformed(fig.dpi_scale_trans.inverted())
        # width, height = bbox.width*fig.dpi, bbox.height*fig.dpi
        # self.m_logger.debug(width, height)
        # return tuple(round(x) for x in fig_size_in_pixels)
        return fig_size_in_pixels

    def create_control_buttons(self):
        self.m_control_buttons = {}

        # Create buttons for each column
        self.m_logger.debug("Figure size: ", self.get_fig_size_px())

        fig_size_px = self.get_fig_size_px()
        wh = f"{-3 + (fig_size_px[1] / self.m_n_row)}px"
        btn_layout = Layout(height=wh, width=wh)

        button = Button(description="🔄", tooltip="Reset Game", layout=btn_layout)
        button.on_click(lambda b: self.reset())
        self.m_control_buttons["reset"] = button

        button = Button(description="↩️", tooltip="Undo Move", layout=btn_layout)
        button.disabled = True
        button.on_click(lambda b: self.undo_move())
        self.m_control_buttons["undo"] = button

        button = Button(description="↪️", tooltip="Redo Move", layout=btn_layout)
        button.disabled = True
        button.on_click(lambda b: self.redo_move())
        self.m_control_buttons["redo"] = button

        button = Button(description="🕹️", tooltip="Computer Move", layout=btn_layout)
        self.m_control_buttons["move"] = button

        button = Button(description="📊", tooltip="Evaluate Board", layout=btn_layout)
        self.m_control_buttons["evaluate"] = button

    def create_board(self):
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
            self.ims = list()
            for ax in axs:
                self.ims.append(ax.imshow(self.m_png[0]["plain"], animated=True))
                ax.axis("off")
                ax.set_xticklabels([])
                ax.set_yticklabels([])

            fig.tight_layout()
            plt.subplots_adjust(
                wspace=0.05, hspace=0.05, left=0.0, right=1.0, top=1.0, bottom=0.0
            )
            fig.suptitle("")
            fig.canvas.toolbar_visible = False
            fig.canvas.resizable = False
            fig.set_facecolor("darkgray")
            fig.canvas.toolbar_visible = False
            fig.canvas.header_visible = False
            fig.canvas.footer_visible = False
            fig.canvas.capture_scroll = True
            plt.show(block=False)

        self.m_fig = fig
        self.m_axs = axs

        # bacground does not appear to be necessary here
        # self.m_background = [fig.canvas.copy_from_bbox(im.get_clip_box()) for im in self.ims]
        # for b in self.m_background:
        #  fig.canvas.blit(b)

    notify_output = widgets.Output()
    display(notify_output)

    @notify_output.capture()
    def popup(self, text):
        clear_output()
        display(Javascript("alert('{}')".format(text)))

    def is_legal_move(self, col):
        if self.m_height[col] >= self.m_n_row:
            return False
        return True

    def insert_token(self, col, reset_redo_list=True):
        if self.is_busy:
            return
        self.is_busy = True

        for button in self.m_insert_buttons:
            button.disabled = True

        if not self.m_board.playMove(col):
            return

        self.check_winner()

        try:
            # Get player
            player = 1 if not self.m_movelist else 3 - self.m_movelist[-1][0]
            self.m_movelist.append((player, col, self.m_height[col]))
            self.paint_token()
            self.m_height[col] += 1

            # Usually, after a move is performed, there is no possibility to
            # redo a move again
            if reset_redo_list:
                self.m_redolist = []

        except Exception as e:
            self.m_logger.error(f"Error: {e}")
            raise
        finally:
            # Re-enable all buttons (if columns not full)
            self.update_insert_buttons()

            time.sleep(0.5)  # debounce button
            self.is_busy = False

    def redo_move(self):
        if len(self.m_redolist) < 1:
            return
        p, col, row = self.m_redolist.pop()
        self.insert_token(col, reset_redo_list=False)

    def undo_move(self):
        if len(self.m_movelist) < 1:
            return

        if self.is_busy:
            return
        self.is_busy = True

        try:
            p, col, row = mv = self.m_movelist.pop()
            self.m_redolist.append(mv)

            self.m_height[col] -= 1
            assert row == self.m_height[col]

            img_idx = self.get_img_idx(col, row)

            self.ims[img_idx].set_data(self.m_png[0]["plain"])
            self.m_axs[img_idx].draw_artist(self.ims[img_idx])
            if len(self.m_movelist) > 0:
                self.paint_token()
            else:
                self.m_fig.canvas.blit(self.ims[img_idx].get_clip_box())
                self.m_fig.canvas.flush_events()

        except Exception as e:
            self.m_logger.error(f"Error: {e}")
            raise
        finally:
            # Re-enable all buttons (if columns not full)
            self.update_insert_buttons()

            time.sleep(0.5)  # debounce button
            self.is_busy = False

    def update_insert_buttons(self):
        for button, col in zip(self.m_insert_buttons, range(self.m_n_col)):
            button.disabled = bool(self.m_height[col] >= self.m_n_row)

        #
        self.m_control_buttons["undo"].disabled = len(self.m_movelist) < 1

        #
        self.m_control_buttons["redo"].disabled = len(self.m_redolist) < 1

    def get_img_idx(self, col, row):
        """
        Get the index of the image to paint.

        This corresponds to the last token in the column
        """
        self.m_logger.debug(f"Got column: {col}")

        img_idx = col % self.m_n_col + (self.m_n_row - row - 1) * self.m_n_col
        self.m_logger.debug(f"{col}, {img_idx}")
        return img_idx

    def paint_token(self):
        if len(self.m_movelist) < 1:
            return

        p, col, row = self.m_movelist[-1]
        img_idx = self.get_img_idx(col, row)
        self.m_logger.debug(f"Paint token: {img_idx}")

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
            img_idx = self.get_img_idx(col, row)
            self.ims[img_idx].set_data(self.m_png[p]["plain"])
            self.m_axs[img_idx].draw_artist(self.ims[img_idx])
            blit_boxes.append(self.ims[img_idx].get_clip_box())

        self.m_fig.canvas.blit(blit_boxes[0])

        # self.m_fig.canvas.restore_region(self.m_background[img_idx])
        # self.m_fig.canvas.blit(self.ims[img_idx].get_clip_box())
        # self.m_fig.canvas.draw_idle()
        self.m_fig.canvas.flush_events()

    def create_buttons(self):
        # Create buttons for each column
        self.m_logger.debug("Figure size: ", self.get_fig_size_px())

        fig_size_px = self.get_fig_size_px()

        self.m_insert_buttons = []
        for col in range(self.m_n_col):
            button = Button(
                description="⏬",
                layout=Layout(
                    width=f"{-3 + (fig_size_px[0] / self.m_n_col)}px", height="50px"
                ),
            )
            button.on_click(lambda b, col=col: self.insert_token(col))
            self.m_insert_buttons.append(button)

    def create_column_labels(self):
        # col_labels = []

        fig_size_px = self.get_fig_size_px()
        width = f"{-3 + (fig_size_px[0] / self.m_n_col)}px"
        # textboxes = [Text("a", layout=Layout(width="55px", align="center")) for i in range(7)]
        textboxes = [
            widgets.Label(
                value=chr(ord("a") + i),
                layout=Layout(
                    justify_content="center", align_items="center", width=width
                ),
            )
            for i in range(self.m_n_col)
        ]
        tb = HBox(
            textboxes,
            layout=Layout(
                display="flex",
                flex_flow="row wrap",  # or "column" depending on your layout needs
                justify_content="center",  # Left alignment
                align_items="center",  # Top alignment
            ),
        )
        return tb

    def on_field_click(self, event):
        ix, iy = event.xdata, event.ydata
        self.m_logger.debug(f"click (x,y): {ix, iy}")
        idx = np.where(self.m_axs == event.inaxes)[0][0] % self.m_n_col
        # if self.is_legal_move(idx):
        self.insert_token(idx)

    def get_widget(self):
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

        tb = self.create_column_labels()

        return AppLayout(
            header=None,
            left_sidebar=control_buttons_col,
            center=VBox(
                [insert_button_row, self.output, tb],
                layout=Layout(
                    display="flex",
                    flex_flow="column wrap",  # or "column" depending on your layout needs
                    justify_content="flex-start",  # Left alignment
                    align_items="flex-start",  # Top alignment
                ),
            ),
            footer=None,
            right_sidebar=None,
        )

    def check_winner(self):
        """
        Placeholder for checking winner logic.
        """
        # self.m_logger.debug("Checking for winner...")
        # Add your Connect-4 winner checking logic here
        # self.popup("Checking for winner...")
        # print("Hubert!")
        if self.m_board.canWin():
            self.popup("Game over!")

    def destroy(self):
        plt.close(self.m_fig)
