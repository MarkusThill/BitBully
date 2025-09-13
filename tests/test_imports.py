def test_import_bitbully_core():
    import bitbully.bitbully_core as bbc
    assert hasattr(bbc, "Board"), "bitbully_core should provide Board"
    assert hasattr(bbc, "BitBully"), "bitbully_core should provide BitBully"
