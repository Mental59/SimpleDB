import subprocess
import os
import tempfile

import pytest


def run_script(commands, database_filename):
    process = subprocess.Popen(
        ["./build/db", database_filename],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )

    input_data = "\n".join(commands) + "\n"
    raw_output, _ = process.communicate(input=input_data)
    return raw_output.split("\n")


@pytest.fixture
def database_filename():
    fd, filename = tempfile.mkstemp(prefix="simpledb_test_")
    os.close(fd)

    yield filename

    if os.path.exists(filename):
        os.remove(filename)


def test_inserts_and_retrieves_a_row(database_filename):
    result = run_script([
        "insert 1 user1 person1@example.com",
        "select",
        ".exit",
    ], database_filename)

    assert result == [
        "db > Executed.",
        "db > (1, user1, person1@example.com)",
        "Executed.",
        "db > ",
    ]


def test_keeps_data_after_closing_connection(database_filename):
    result1 = run_script([
        "insert 1 user1 person1@example.com",
        ".exit",
    ], database_filename)

    assert result1 == [
        "db > Executed.",
        "db > ",
    ]

    result2 = run_script([
        "select",
        ".exit",
    ], database_filename)

    assert result2 == [
        "db > (1, user1, person1@example.com)",
        "Executed.",
        "db > ",
    ]


def test_prints_error_message_when_table_is_full(database_filename):
    script = [
        f"insert {i} user{i} person{i}@example.com"
        for i in range(1, 1402)
    ]
    script.append(".exit")
    result = run_script(script, database_filename)

    assert result[-2] == "db > Error: Table full."


def test_allows_inserting_strings_that_are_the_maximum_length(database_filename):
    long_username = "a" * 32
    long_email = "a" * 255
    script = [
        f"insert 1 {long_username} {long_email}",
        "select",
        ".exit",
    ]
    result = run_script(script, database_filename)

    assert result == [
        "db > Executed.",
        f"db > (1, {long_username}, {long_email})",
        "Executed.",
        "db > ",
    ]


def test_prints_error_message_if_strings_are_too_long(database_filename):
    long_username = "a" * 33
    long_email = "a" * 256
    script = [
        f"insert 1 {long_username} {long_email}",
        "select",
        ".exit",
    ]
    result = run_script(script, database_filename)

    assert result == [
        "db > String is too long.",
        "db > Executed.",
        "db > ",
    ]


def test_prints_an_error_message_if_id_is_negative(database_filename):
    script = [
        "insert -1 cstack foo@bar.com",
        "select",
        ".exit",
    ]
    result = run_script(script, database_filename)

    assert result == [
        "db > Invalid uint32 value.",
        "db > Executed.",
        "db > ",
    ]


def test_prints_constants(database_filename):
    script = [
        ".constants",
        ".exit",
    ]
    result = run_script(script, database_filename)

    assert result == [
        "db > Constants:",
        "ROW_SIZE: 293",
        "COMMON_NODE_HEADER_SIZE: 6",
        "LEAF_NODE_HEADER_SIZE: 10",
        "LEAF_NODE_CELL_SIZE: 297",
        "LEAF_NODE_SPACE_FOR_CELLS: 4086",
        "LEAF_NODE_MAX_CELLS: 13",
        "db > ",
    ]


def test_allows_printing_out_the_structure_of_a_one_node_btree(database_filename):
    script = [
        f"insert {i} user{i} person{i}@example.com"
        for i in [3, 1, 2]
    ]
    script.append(".btree")
    script.append(".exit")
    result = run_script(script, database_filename)

    assert result == [
        "db > Executed.",
        "db > Executed.",
        "db > Executed.",
        "db > Tree:",
        "leaf (size 3)",
        "  - 0 : 3",
        "  - 1 : 1",
        "  - 2 : 2",
        "db > ",
    ]
