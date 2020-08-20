from VacEnvWrapper import Environment
from VacEnvGym import VacEnvironment
import VacWindowWrapper as Window

from gym.envs.registration import register

register(
    id='VacEnv-v0',
    entry_point='VacEnv:VacEnvironment',
)