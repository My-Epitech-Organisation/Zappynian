import unittest
import sys
import os

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../../")))
from test_command_queue import TestCommandQueue
from test_role_selector import TestRoleSelector
from test_route_factory import TestRouteFactory


if __name__ == "__main__":
    loader = unittest.TestLoader()
    suite = unittest.TestSuite()

    suite.addTests(loader.loadTestsFromTestCase(TestCommandQueue))
    suite.addTests(loader.loadTestsFromTestCase(TestRoleSelector))
    suite.addTests(loader.loadTestsFromTestCase(TestRouteFactory))

    runner = unittest.TextTestRunner(verbosity=2)
    runner.run(suite)
