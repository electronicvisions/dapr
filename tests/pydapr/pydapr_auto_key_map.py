#!/usr/bin/env python
import unittest
import pydapr


class AutoKeyMapTestPydapr(unittest.TestCase):
    def test_polymorphic_value(self):
        dummy = pydapr.DerivedDummyProperty(5)

        dummy_map = pydapr.DummyAutoKeyMapParent.PolymorphicValue()

        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 0)
        self.assertFalse(dummy_map.contains(0))

        key_0 = dummy_map.insert(dummy)
        self.assertEqual(key_0, 0)

        self.assertFalse(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 1)
        self.assertTrue(dummy_map.contains(0))
        self.assertEqual(dummy_map.get(0), dummy)

        dummy_copy = pydapr.DerivedDummyProperty(dummy)
        dummy_copy.value = 7
        key_1 = dummy_map.insert(dummy_copy)
        self.assertEqual(key_1, 1)

        self.assertEqual(dummy_map.get(key_1), dummy_copy)

        dummy_map.erase(key_1)
        self.assertFalse(dummy_map.contains(key_1))
        self.assertEqual(dummy_map.size(), 1)

        dummy_map_copy = pydapr.DummyAutoKeyMapParent.PolymorphicValue(
            dummy_map)
        self.assertEqual(dummy_map, dummy_map_copy)

        dummy_map_copy.set(0, dummy_copy)
        self.assertNotEqual(dummy_map, dummy_map_copy)

        with self.assertRaises(IndexError):
            dummy_map.get(3)

        with self.assertRaises(IndexError):
            dummy_map.set(3, dummy)

        key_2 = dummy_map_copy.insert(dummy)
        self.assertEqual(key_2, 2)

    def test_not_polymorphic(self):
        dummy_map = pydapr.DummyAutoKeyMapParent.NotPolymorphic()

        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 0)
        self.assertFalse(dummy_map.contains(0))

        key_0 = dummy_map.insert(0)
        self.assertEqual(key_0, 0)

        self.assertFalse(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 1)
        self.assertTrue(dummy_map.contains(0))
        self.assertEqual(dummy_map.get(0), 0)

        key_1 = dummy_map.insert(7)
        self.assertEqual(key_1, 1)

        self.assertEqual(dummy_map.get(key_1), 7)

        dummy_map.erase(1)
        self.assertFalse(dummy_map.contains(1))
        self.assertEqual(dummy_map.size(), 1)

        dummy_map_copy = pydapr.DummyAutoKeyMapParent.NotPolymorphic(
            dummy_map)
        self.assertEqual(dummy_map, dummy_map_copy)

        dummy_map_copy.set(0, 7)
        self.assertNotEqual(dummy_map, dummy_map_copy)

        with self.assertRaises(IndexError):
            dummy_map.get(3)

        with self.assertRaises(IndexError):
            dummy_map.set(3, 0)

        key_2 = dummy_map_copy.insert(2)
        self.assertEqual(key_2, 2)


if __name__ == "__main__":
    unittest.main()
