from turn_arr_to_path import *
import unittest

class TestTurnArrToPath(unittest.TestCase):

    def test_small_rect_path(self):
        I = [2,3,5]
        O = {(0,0,0):  1,  # 1
             (-1,0,0): 1,  # 2
             (-1,0,3): 1,  # 3
             (-1,0,2): 1,  # 4
             (0,0,2):  1,  # 5
             (0,0,1):  1}  # 6
        self.assertEqual(make_path(I),O)

    def test_larger_square_path(self):
        I = [2,5,8,11]
        O = {(0,0,0):   1,  # 1
             (-1,0,0):  1,  # 2
             (-1,0,3):  1,  # 3
             (-1,-1,3): 1,  # 4
             (-1,-2,3): 1,  # 5
             (-1,-2,2): 1,  # 6
             (0,-2,2):  1,  # 7
             (1,-2,2):  1,  # 8
             (1,-2,1):  1,  # 9
             (1,-1,1):  1,  # 10
             (1,0,1):   1,  # 11
             (1,0,0):   1}  # 12
        self.assertEqual(make_path(I),O)

    def test_L_overlap(self):
        I = [1,3,5,6,7,8,10,11,13]
        O = {(0,0,0):  1,  # 1
             (0,0,3):  1,  # 2
             (0,-1,3): 1,  # 3
             (0,-1,2): 1,  # 4
             (1,-1,2): 2,  # 5,9
             (1,-1,1): 1,  # 6
             (1,-1,0): 2,  # 7,13
             (1,-1,3): 2,  # 8,14
             (2,-1,2): 1,  # 10
             (2,-1,1): 1,  # 11
             (2,-1,0): 1}  # 12
        self.assertEqual(make_path(I),O)


if __name__ == "__main__":
    unittest.main()
