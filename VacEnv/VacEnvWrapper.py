from ctypes import cdll
import ctypes
import numpy as np

lib = cdll.LoadLibrary('./libVacEnvS.dll')
lib.VacEnv_new.restype = ctypes.c_void_p


class Environment(object):
    def __init__(self):
        # lib.VacEnv_new.argtypes = [ctypes.c_int]
        lib.VacEnv_new.restype = ctypes.c_void_p

        lib.VacEnv_del.argtypes = [ctypes.c_void_p]
        lib.VacEnv_del.restype = ctypes.c_void_p

        lib.VacEnv_score.argtypes = [ctypes.c_void_p]
        lib.VacEnv_score.restype = ctypes.c_float

        lib.VacEnv_running.argtypes = [ctypes.c_void_p]
        lib.VacEnv_running.restype = ctypes.c_bool

        lib.VacEnv_step.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_float, ctypes.c_void_p]
        lib.VacEnv_step.restype = ctypes.c_void_p

        lib.VacEnv_reset.argtypes = [ctypes.c_void_p]
        lib.VacEnv_reset.restype = ctypes.c_void_p

        lib.VacEnv_world.argtypes = [ctypes.c_void_p]
        lib.VacEnv_world.restype = ctypes.c_void_p

        self.obj = None

    def score(self):
        if self.obj:
            return lib.VacEnv_score(self.obj)
        else:
            return 0

    def running(self):
        if self.obj:
            return lib.VacEnv_running(self.obj)
        else:
            return False

    def world(self):
        if self.obj:
            return lib.VacEnv_world(self.obj)
        else:
            return None

    def step(self, value, dt):
        if self.obj:
            a = np.zeros((9,), dtype=np.double)
            lib.VacEnv_step(self.obj, value, dt, a.ctypes.data_as(ctypes.c_void_p))
            return a
        else:
            return np.zeros((9,), dtype=np.double)

    def reset(self):
        if self.obj:
            lib.VacEnv_reset(self.obj)
        else:
            self.obj = lib.VacEnv_new()

    def __del__(self):
        if self.obj:
            lib.VacEnv_del(self.obj)
            self.obj = None
