#!/usr/bin/env python
# -*- coding: utf-8 -*-

import unittest
import subprocess

class TestRPNCalcInterface(unittest.TestCase):

    def setUp(self):
        self.p = subprocess.Popen("bin/RPNCalc", stdin=subprocess.PIPE, 
            stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    def _send_commands(self, cmdlist):
        cmd = "\n".join(cmdlist) + " q\n"
        return self.p.communicate(cmd)

    def _fmt_expect(self, double):
        return "%.6f" % double

    def test_handles_int_addition(self):
        r_out, r_err = self._send_commands(["5 4", "+"])
        expect = self._fmt_expect(9)
        self.assertIn(expect, r_out.split("\n"))

    def test_handles_double_operands(self):
        r_out, r_err = self._send_commands(["5.5 2", "+"])
        expect = self._fmt_expect(7.5)
        self.assertIn(expect, r_out.split("\n"))

    def test_handles_exponential_float_operands(self):
        r_out, r_err = self._send_commands(["125e-2 0", "+"])
        expect = self._fmt_expect(125e-2)
        self.assertIn(expect, r_out.split("\n"))

if __name__ == '__main__':
    unittest.main()