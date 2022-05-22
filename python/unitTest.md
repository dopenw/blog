<!-- Unit test -->


<!-- @import "[TOC]" {cmd="toc" depthFrom=1 depthTo=6 orderedList=false} -->

<!-- code_chunk_output -->

- [Unit testing framework](#unit-testing-framework)
  - [basic example](#basic-example)
  - [Command-Line Interface](#command-line-interface)
  - [assert methods](#assert-methods)
  - [Skipping tests and expected failures](#skipping-tests-and-expected-failures)

<!-- /code_chunk_output -->

## Unit testing framework

[Unit testing framework](https://docs.python.org/3/library/unittest.html)


### basic example
test.py:
```py
import unittest

class TestStringMethods(unittest.TestCase):

    def test_upper(self):
        self.assertEqual('foo'.upper(), 'FOO')

    def test_isupper(self):
        self.assertTrue('FOO'.isupper())
        self.assertFalse('Foo'.isupper())

    def test_split(self):
        s = 'hello world'
        self.assertEqual(s.split(), ['hello', 'world'])
        # check that s.split fails when the separator is not a string
        with self.assertRaises(TypeError):
            s.split(2)

if __name__ == '__main__':
    unittest.main()
```
Execute `python test.py` command,show
```py
...
----------------------------------------------------------------------
Ran 3 tests in 0.001s

OK
```

Passing the `-v` option to your test script will instruct unittest.main() to enable a higher level of verbosity:
```py
test_isupper (__main__.TestStringMethods) ... ok
test_split (__main__.TestStringMethods) ... ok
test_upper (__main__.TestStringMethods) ... ok

----------------------------------------------------------------------
Ran 3 tests in 0.001s

OK
```

### Command-Line Interface
The unittest module can be used from the command line to run tests from modules, classes or even individual test methods:

Take the above test.py as an example, it can be used like this:
```cmd
python -m unittest -v test 
python -m unittest -v test.TestStringMethods
python -m unittest -v test.TestStringMethods.test_split
```
Of course, it can also be like this:
```cmd
python -m unittest -v test.py
```
When executed without arguments [Test Discovery](https://docs.python.org/3/library/unittest.html#unittest-test-discovery) is started:
```cmd
python -m unittest
```
For a list of all the command-line options:
```cmd
python -m unittest -h
```

### assert methods

The following table lists the most commonly used methods :
|           Method          |      Checks that     | New in |
|:-------------------------:|:--------------------:|:------:|
|     assertEqual(a, b)     |        a == b        |        |
|    assertNotEqual(a, b)   |        a != b        |        |
|       assertTrue(x)       |    bool(x) is True   |        |
|       assertFalse(x)      |   bool(x) is False   |        |
|       assertIs(a, b)      |        a is b        |   3.1  |
|     assertIsNot(a, b)     |      a is not b      |   3.1  |
|      assertIsNone(x)      |       x is None      |   3.1  |
|     assertIsNotNone(x)    |     x is not None    |   3.1  |
|       assertIn(a, b)      |        a in b        |   3.1  |
|     assertNotIn(a, b)     |      a not in b      |   3.1  |
|   [assertIsInstance(a, b)](https://docs.python.org/3/library/unittest.html#unittest.TestCase.assertIsInstance)  |   isinstance(a, b)   |   3.2  |
| assertNotIsInstance(a, b) | not isinstance(a, b) |   3.2  |


It is also possible to check the production of exceptions, warnings, and log messages using the following methods:
|                     Method                    |                           Checks that                          | New in |
|:---------------------------------------------:|:--------------------------------------------------------------:|:------:|
|     assertRaises(exc, fun, *args, **kwds)     |                  fun(*args, **kwds) raises exc                 |        |
| assertRaisesRegex(exc, r, fun, *args, **kwds) |  fun(*args, **kwds) raises exc and the message matches regex r |   3.1  |
|     assertWarns(warn, fun, *args, **kwds)     |                 fun(*args, **kwds) raises warn                 |   3.2  |
| assertWarnsRegex(warn, r, fun, *args, **kwds) | fun(*args, **kwds) raises warn and the message matches regex r |   3.2  |
|           assertLogs(logger, level)           |        The with block logs on logger with minimum level        |   3.4  |

There are also other methods used to perform more specific checks, such as:
|           Method           |                                  Checks that                                  | New in |
|:--------------------------:|:-----------------------------------------------------------------------------:|:------:|
|   assertAlmostEqual(a, b)  |                               round(a-b, 7) == 0                              |        |
| assertNotAlmostEqual(a, b) |                               round(a-b, 7) != 0                              |        |
|     assertGreater(a, b)    |                                     a > b                                     |   3.1  |
|  assertGreaterEqual(a, b)  |                                     a >= b                                    |   3.1  |
|      assertLess(a, b)      |                                     a < b                                     |   3.1  |
|    assertLessEqual(a, b)   |                                     a <= b                                    |   3.1  |
|      assertRegex(s, r)     |                                  r.search(s)                                  |   3.1  |
|    assertNotRegex(s, r)    |                                not r.search(s)                                |   3.2  |
|   assertCountEqual(a, b)   | a and b have the same elements in the same number, regardless of their order. |   3.2  |

The list of type-specific methods automatically used by assertEqual() are summarized in the following table. Note that it’s usually not necessary to invoke these methods directly.
|           Method           |   Used to compare  | New in |
|:--------------------------:|:------------------:|:------:|
| assertMultiLineEqual(a, b) |       strings      |   3.1  |
|  assertSequenceEqual(a, b) |      sequences     |   3.1  |
|    assertListEqual(a, b)   |        lists       |   3.1  |
|   assertTupleEqual(a, b)   |       tuples       |   3.1  |
|    assertSetEqual(a, b)    | sets or frozensets |   3.1  |
|    assertDictEqual(a, b)   |        dicts       |   3.1  |

### Skipping tests and expected failures

[unittest - Skipping tests and expected failures](https://docs.python.org/3/library/unittest.html#skipping-tests-and-expected-failures)

eg: test.py:
```py
import unittest
import sys

def external_resource_available():
    return False

def my_lib_version():
    return (1,2)

class MyTestCase(unittest.TestCase):

    @unittest.skip("demonstrating skipping")
    def test_nothing(self):
        self.fail("shouldn't happen")

    @unittest.skipIf(my_lib_version() < (1, 3),
                     "not supported in this library version")
    def test_format(self):
        # Tests that work for only a certain version of the library.
        pass

    @unittest.skipUnless(sys.platform.startswith("win"), "requires Windows")
    def test_windows_support(self):
        # windows specific testing code
        pass

    def test_maybe_skipped(self):
        if not external_resource_available():
            self.skipTest("external resource not available")
        # test code that depends on the external resource
        pass

if __name__ == "__main__":
    unittest.main()
```

Execute `python test.py -v`,show:
```cmd
test_format (__main__.MyTestCase) ... skipped 'not supported in this library version'
test_maybe_skipped (__main__.MyTestCase) ... skipped 'external resource not available'
test_nothing (__main__.MyTestCase) ... skipped 'demonstrating skipping'
test_windows_support (__main__.MyTestCase) ... ok

----------------------------------------------------------------------
Ran 4 tests in 0.001s

OK (skipped=3)
```

---
- [上一级](README.md)
- 上一篇 -> [翻译小工具](translateTool.md)
