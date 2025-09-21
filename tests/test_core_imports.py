"""Test the imports from the package."""


def test_import_bitbully_core() -> None:
    """Verify that the `bitbully.bitbully_core` module exposes the expected API.

    Ensures:
        * The module can be successfully imported.
        * It defines the `Board` and `BitBullyCore` classes, which are core
          components required by the BitBullyCore library.

    """
    import bitbully.bitbully_core as bbc  # Local import to test importability

    assert hasattr(bbc, "BoardCore"), "bitbully_core should provide Board"
    assert hasattr(bbc, "BitBullyCore"), "bitbully_core should provide BitBullyCore"
    assert hasattr(bbc, "OpeningBookCore"), "bitbully_core should provide OpeningBookCore"
