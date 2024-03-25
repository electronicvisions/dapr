#!/usr/bin/env python
import unittest
import pydapr


class PropertyTestPydapr(unittest.TestCase):
    def test(self):
        property_0_0 = pydapr.DerivedDummyProperty(0)
        property_0_1 = pydapr.DerivedDummyProperty(0)
        property_1 = pydapr.DerivedDummyProperty(1)

        self.assertEqual(property_0_0, property_0_1)
        self.assertNotEqual(property_0_0, property_1)

        self.assertTrue(isinstance(property_0_0, pydapr.DummyProperty))

        self.assertEqual(str(property_0_0), "DerivedDummyProperty(0)")
        self.assertEqual(repr(property_0_0), "DerivedDummyProperty(0)")


if __name__ == "__main__":
    unittest.main()
