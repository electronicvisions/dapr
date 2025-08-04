#!/usr/bin/env python
import unittest
import pydapr


class MapTestPydapr(unittest.TestCase):
    def test_polymorphic_value(self):
        dummy = pydapr.DerivedDummyProperty(5)

        dummy_map = pydapr.DummyMapParent.PolymorphicValue()

        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 0)
        self.assertFalse(dummy_map.contains(0))

        dummy_map.set(0, dummy)

        self.assertFalse(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 1)
        self.assertTrue(dummy_map.contains(0))
        self.assertEqual(dummy_map.get(0), dummy)

        dummy_copy = pydapr.DerivedDummyProperty(dummy)
        dummy_copy.value = 7
        dummy_map.set(0, dummy_copy)

        self.assertEqual(dummy_map.get(0), dummy_copy)

        dummy_map.erase(1)
        self.assertFalse(dummy_map.contains(1))
        self.assertEqual(dummy_map.size(), 1)

        dummy_map_copy = pydapr.DummyMapParent.PolymorphicValue(
            dummy_map)
        self.assertEqual(dummy_map, dummy_map_copy)

        dummy_map_copy.set(0, dummy)
        self.assertNotEqual(dummy_map, dummy_map_copy)

        with self.assertRaises(IndexError):
            dummy_map.get(3)

    def test_not_polymorphic(self):
        dummy_map = pydapr.DummyMapParent.NotPolymorphic()

        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 0)
        self.assertFalse(dummy_map.contains(0))

        dummy_map.set(0, 0)

        self.assertFalse(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 1)
        self.assertTrue(dummy_map.contains(0))
        self.assertEqual(dummy_map.get(0), 0)

        dummy_map.set(0, 7)

        self.assertEqual(dummy_map.get(0), 7)

        dummy_map.erase(1)
        self.assertFalse(dummy_map.contains(1))
        self.assertEqual(dummy_map.size(), 1)

        dummy_map_copy = pydapr.DummyMapParent.NotPolymorphic(
            dummy_map)
        self.assertEqual(dummy_map, dummy_map_copy)

        dummy_map_copy.set(0, 0)
        self.assertNotEqual(dummy_map, dummy_map_copy)

        with self.assertRaises(IndexError):
            dummy_map.get(3)


if __name__ == "__main__":
    unittest.main()
