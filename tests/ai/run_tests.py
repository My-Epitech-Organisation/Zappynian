import unittest
import sys
import os

# Add the project root to Python path
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../../")))

# Import all test classes
from test_command_queue import TestCommandQueue, TestWorldState, TestVision
from test_connection import TestConnection
from test_roles import TestRoles
from test_route_factory import TestRouteFactory
from test_utils_route_factory import TestRouteFactory as TestUtilsRouteFactory
from test_game_logic import TestGameLogic
from test_zappy_ai import TestZappyAI, TestParseArgs, TestMain
from test_incantation_data import TestIncantationData


def run_all_tests():
    """Run all AI tests with detailed output."""
    loader = unittest.TestLoader()
    suite = unittest.TestSuite()

    # Add all test cases
    test_cases = [
        TestCommandQueue,
        TestWorldState,
        TestVision,
        TestConnection,
        TestRoles,
        TestRouteFactory,
        TestUtilsRouteFactory,
        TestGameLogic,
        TestZappyAI,
        TestParseArgs,
        TestMain,
        TestIncantationData,
    ]

    print("Running AI tests...")
    print("=" * 50)
    
    for test_case in test_cases:
        suite.addTests(loader.loadTestsFromTestCase(test_case))

    runner = unittest.TextTestRunner(
        verbosity=2,
        descriptions=True,
        failfast=False
    )
    
    result = runner.run(suite)
    
    print("\n" + "=" * 50)
    print(f"Tests run: {result.testsRun}")
    print(f"Failures: {len(result.failures)}")
    print(f"Errors: {len(result.errors)}")
    
    if result.failures:
        print("\nFAILURES:")
        for test, traceback in result.failures:
            print(f"- {test}: {traceback}")
    
    if result.errors:
        print("\nERRORS:")
        for test, traceback in result.errors:
            print(f"- {test}: {traceback}")
    
    return result.wasSuccessful()


if __name__ == "__main__":
    success = run_all_tests()
    sys.exit(0 if success else 1)
