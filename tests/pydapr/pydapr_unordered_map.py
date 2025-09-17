#!/usr/bin/env python
import unittest
import pydapr


class UnorderedMapTestPydapr(unittest.TestCase):
    def test_polymorphic_value(self):
        dummy = pydapr.DerivedDummyProperty(5)

        dummy_map = pydapr.DummyUnorderedMapParent.PolymorphicValue()

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

        dummy_map_copy = pydapr.DummyUnorderedMapParent.PolymorphicValue(
            dummy_map)
        self.assertEqual(dummy_map, dummy_map_copy)

        dummy_map_copy.set(0, dummy)
        self.assertNotEqual(dummy_map, dummy_map_copy)

        with self.assertRaises(IndexError):
            dummy_map.get(3)

        dummy_map_copy = pydapr.DummyUnorderedMapParent.PolymorphicValue(
            dummy_map)
        dummy_map_merge = pydapr.DummyUnorderedMapParent.PolymorphicValue()
        dummy_map_merge.merge(dummy_map)
        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map_merge, dummy_map_copy)

    def test_polymorphic_key(self):
        dummy = pydapr.DerivedDummyProperty(5)

        dummy_map = pydapr.DummyUnorderedMapParent.PolymorphicKey()

        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 0)
        self.assertFalse(dummy_map.contains(dummy))

        dummy_map.set(dummy, 0)

        self.assertFalse(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 1)
        self.assertTrue(dummy_map.contains(dummy))
        self.assertEqual(dummy_map.get(dummy), 0)

        dummy_copy = pydapr.DerivedDummyProperty(dummy)
        dummy_copy.value = 7
        dummy_map.set(dummy_copy, 1)

        self.assertEqual(dummy_map.get(dummy_copy), 1)

        dummy_map.erase(dummy_copy)
        self.assertFalse(dummy_map.contains(dummy_copy))
        self.assertEqual(dummy_map.size(), 1)

        dummy_map_copy = pydapr.DummyUnorderedMapParent.PolymorphicKey(
            dummy_map)
        self.assertEqual(dummy_map, dummy_map_copy)

        dummy_map_copy.set(dummy, 1)
        self.assertNotEqual(dummy_map, dummy_map_copy)

        with self.assertRaises(IndexError):
            dummy_map.get(dummy_copy)

        dummy_map_copy = pydapr.DummyUnorderedMapParent.PolymorphicKey(
            dummy_map)
        dummy_map_merge = pydapr.DummyUnorderedMapParent.PolymorphicKey()
        dummy_map_merge.merge(dummy_map)
        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map_merge, dummy_map_copy)

    def test_polymorphic_key_value(self):
        dummy = pydapr.DerivedDummyProperty(5)

        dummy_map = pydapr.DummyUnorderedMapParent.PolymorphicKeyValue()

        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 0)
        self.assertFalse(dummy_map.contains(dummy))

        dummy_map.set(dummy, dummy)

        self.assertFalse(dummy_map.empty())
        self.assertEqual(dummy_map.size(), 1)
        self.assertTrue(dummy_map.contains(dummy))
        self.assertEqual(dummy_map.get(dummy), dummy)

        dummy_copy = pydapr.DerivedDummyProperty(dummy)
        dummy_copy.value = 7
        dummy_map.set(dummy_copy, dummy_copy)

        self.assertEqual(dummy_map.get(dummy_copy), dummy_copy)

        dummy_map.erase(dummy_copy)
        self.assertFalse(dummy_map.contains(dummy_copy))
        self.assertEqual(dummy_map.size(), 1)

        dummy_map_copy = pydapr.DummyUnorderedMapParent\
            .PolymorphicKeyValue(dummy_map)
        self.assertEqual(dummy_map, dummy_map_copy)

        dummy_map_copy.set(dummy, dummy_copy)
        self.assertNotEqual(dummy_map, dummy_map_copy)

        with self.assertRaises(IndexError):
            dummy_map.get(dummy_copy)

        dummy_map_copy = pydapr.DummyUnorderedMapParent.PolymorphicKeyValue(
            dummy_map)
        dummy_map_merge = pydapr.DummyUnorderedMapParent.PolymorphicKeyValue()
        dummy_map_merge.merge(dummy_map)
        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map_merge, dummy_map_copy)

    def test_not_polymorphic(self):
        dummy_map = pydapr.DummyUnorderedMapParent.NotPolymorphic()

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

        dummy_map_copy = pydapr.DummyUnorderedMapParent.NotPolymorphic(
            dummy_map)
        self.assertEqual(dummy_map, dummy_map_copy)

        dummy_map_copy.set(0, 0)
        self.assertNotEqual(dummy_map, dummy_map_copy)

        with self.assertRaises(IndexError):
            dummy_map.get(3)

        dummy_map_copy = pydapr.DummyUnorderedMapParent.NotPolymorphic(
            dummy_map)
        dummy_map_merge = pydapr.DummyUnorderedMapParent.NotPolymorphic()
        dummy_map_merge.merge(dummy_map)
        self.assertTrue(dummy_map.empty())
        self.assertEqual(dummy_map_merge, dummy_map_copy)


if __name__ == "__main__":
    unittest.main()
