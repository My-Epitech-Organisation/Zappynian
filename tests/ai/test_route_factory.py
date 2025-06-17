import unittest
from ai.src.utils.route_factory import route_to

class TestRouteFactory(unittest.TestCase):
    def test_route_to_known_tile(self):
        self.assertEqual(route_to(2), ["Forward"])
        self.assertEqual(route_to(4), ["Forward", "Forward", "Left", "Forward", "Forward"])

    def test_route_to_unknown_tile(self):
        self.assertEqual(route_to(99), ["Look"])

if __name__ == '__main__':
    unittest.main()
