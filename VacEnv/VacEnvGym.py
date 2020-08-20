import gym
import numpy as np
from VacEnvWrapper import Environment
from gym import error, spaces, utils
from gym.utils import seeding


class VacEnvironment(gym.Env):
    metadata = {'render.modes': ['human']}

    def __init__(self):

        self.obj = Environment()
        self.score = 0.0

        self.dt = 1/20
        self.action_space = spaces.Discrete(5)
        self.observation_space = spaces.Box(np.array([0,-1,-1,-1,-1,-1,0,0,0]), np.array([+1,+1,+1,+1,+1,+1,+1,+1,+1]),
                                            dtype=np.double)

        self.states = np.array([0,0,0,0,0,0,0,0,0],  dtype=np.double)

    def step(self, action):
        """

        Parameters
        ----------
        action :

        Returns
        -------
        ob, reward, episode_over, info : tuple
            ob (object) :
                an environment-specific object representing your observation of
                the environment.
            reward (float) :
                amount of reward achieved by the previous action. The scale
                varies between environments, but the goal is always to increase
                your total reward.
            episode_over (bool) :
                whether it's time to reset the environment again. Most (but not
                all) tasks are divided up into well-defined episodes, and done
                being True indicates the episode has terminated. (For example,
                perhaps the pole tipped too far, or you lost your last life.)
            info (dict) :
                 diagnostic information useful for debugging. It can sometimes
                 be useful for learning (for example, it might contain the raw
                 probabilities behind the environment's last state change).
                 However, official evaluations of your agent are not allowed to
                 use this for learning.
        """
        self.states = self.obj.step(action, self.dt)
        newScore = self.obj.score()
        reward = (newScore - self.score) * 1000
        self.score = newScore

        ob = self.states
        episode_over = not self.obj.running()
        if episode_over :
            reward = -5
        info = {'score': self.score}
        return ob, reward, episode_over, info

    def reset(self):
        self.obj.reset()
        self.score = 0.0
        self.states = np.array([0,0,0,0,0,0,0,0,0],  dtype=np.double)
        self.dt = 1/20
        return self.states

    def render(self, mode='human'):
        pass

