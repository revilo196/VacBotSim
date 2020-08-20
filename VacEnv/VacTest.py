from VacEnvWrapper import Environment
import VacWindowWrapper as Window
from VacEnvGym import VacEnvironment


from stable_baselines.common.policies import MlpPolicy, MlpLstmPolicy
from stable_baselines.common.vec_env import DummyVecEnv
from stable_baselines.common.vec_env import VecFrameStack


from stable_baselines import A2C, PPO1


# TESTING
from random import randint
import threading
import multiprocessing
import time

from gym.envs.registration import register

import gym

register(
    id='VacEnv-v0',
    entry_point='VacEnvGym:VacEnvironment',
)


def callback(lcl, _glb):
    # stop training if reward exceeds 199
    is_solved = lcl['t'] > 100 and sum(lcl['episode_rewards'][-101:-1]) / 100 >= 199
    return is_solved


def test_gym():
    env = gym.make('VacEnv-v0')
    env2 = gym.make('VacEnv-v0')
    env3 = gym.make('VacEnv-v0')
    env4 = gym.make('VacEnv-v0')

    #envd = SubprocVecEnv([lambda: env])
    envd = DummyVecEnv([lambda: env,lambda: env2,lambda: env3,lambda: env4])

    Window.init(512, -50, 50, -50, 50)
    t = threading.Thread(target=Window.show)
    t.start()

    env.reset()
    env2.reset()
    env3.reset()
    env4.reset()

    Window.attachEnv(env.obj.obj)
    Window.attachEnv(env2.obj.obj)
    Window.attachEnv(env3.obj.obj)
    Window.attachEnv(env4.obj.obj)


    model = A2C(MlpLstmPolicy ,envd, verbose=1,tensorboard_log="./a2c_cartpole_tensorboard/",)
    model.learn(total_timesteps=10000000, log_interval=1000)


#    act = deepq.learn(
#        env,
#        network="mlp",
#        lr=1e-3,
#        total_timesteps=1000000,
#        buffer_size=50000,
#        exploration_fraction=0.1,
#        exploration_final_eps=0.02,
#        print_freq=10,
#        callback=callback,
#    )

    print("Saving model to cartpole_model.pkl")
    model.save("cartpole_model.pkl")


'''
    for i_episode in range(20):
        env.reset()
        for t in range(10000):
            #env.render()
            action = env.action_space.sample()
            observation, reward, done, info = env.step(action)
            print(observation)
            if done:
                print("Episode finished after {} timesteps".format(t+1))
                break
    Window.detachEnv(env.obj.obj)
    Window.close()
    t.join()
'''


def test_multi():

    def runloop(env):
        start = time.time()
        for x in range(10*20*60):
            env.step(randint(0, 4), 1 / 20)
            # if not env.running():
            #    return
        end = time.time()
        print(end - start)

    env1 = Environment()
    env2 = Environment()
    env3 = Environment()
    env4 = Environment()
    env5 = Environment()
    env6 = Environment()
    env7 = Environment()
    env8 = Environment()

    print(env1.running())
    print(env1.score())
    print(env1.world())

    Window.init(512, -50, 50, -50, 50)
    #Window.attachEnv(env1.obj)
    #Window.attachEnv(env2.obj)
    #Window.attachEnv(env3.obj)
    #Window.attachEnv(env4.obj)
    #Window.attachEnv(env5.obj)
    #Window.attachEnv(env6.obj)
    #Window.attachEnv(env7.obj)
    #Window.attachEnv(env8.obj)

    t = threading.Thread(target=Window.show)
    t.start()

    t1 = threading.Thread(target=runloop, args=[env1])
    t2 = threading.Thread(target=runloop, args=[env2])
    t3 = threading.Thread(target=runloop, args=[env3])
    t4 = threading.Thread(target=runloop, args=[env4])
    t5 = threading.Thread(target=runloop, args=[env5])
    t6 = threading.Thread(target=runloop, args=[env6])
    t7 = threading.Thread(target=runloop, args=[env7])
    t8 = threading.Thread(target=runloop, args=[env8])

    t1.start()
    t2.start()
    t3.start()
    t4.start()
    t5.start()
    t6.start()
    t7.start()
    t8.start()

    t1.join()
    t2.join()
    t3.join()
    t4.join()
    t5.join()
    t6.join()
    t7.join()
    t8.join()

    Window.detachEnv(env1.obj)
    Window.detachEnv(env2.obj)
    Window.detachEnv(env3.obj)
    Window.detachEnv(env4.obj)
    Window.detachEnv(env5.obj)
    Window.detachEnv(env6.obj)
    Window.detachEnv(env7.obj)
    Window.detachEnv(env8.obj)
    Window.close()
    t.join()



if __name__ == '__main__':
    #test_multi()
    test_gym()


