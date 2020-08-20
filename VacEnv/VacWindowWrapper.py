from ctypes import cdll
import ctypes

lib = cdll.LoadLibrary('./libVacEnvS.dll')

lib.win_init.argtypes = [ctypes.c_uint, ctypes.c_float, ctypes.c_float, ctypes.c_float, ctypes.c_float]
lib.win_init.restype = ctypes.c_void_p
init = lib.win_init

lib.win_close.argtypes = []
lib.win_close.restype = ctypes.c_void_p
close = lib.win_close

lib.win_run.argtypes = []
lib.win_run.restype = ctypes.c_int
show = lib.win_run

lib.win_add_world.argtypes = [ctypes.c_void_p]
lib.win_add_world.restype = ctypes.c_void_p
attachEnv = lib.win_add_world

lib.win_remove_world.argtypes = [ctypes.c_void_p]
lib.win_remove_world.restype = ctypes.c_void_p
detachEnv = lib.win_remove_world
