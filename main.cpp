#include <iostream>
#include <thread>
#include "MultiNEAT/Genome.h"
#include "MultiNEAT/Population.h"
#include "MultiNEAT/Genome.h"
#include "MultiNEAT/NeuralNetwork.h"
#include "MultiNEAT/PhenotypeBehavior.h"
#include "MultiNEAT/Parameters.h"

    #include <cstdio>
#include "VacEnv/VacEnv.h"
#include "VacEnv/VacEnvWindow.h"

using namespace NEAT;

#define CORES 16
#define SEED 352169585

void simulation(Population * pop);
void epoch(unsigned int generation, Population * pop );
void epochPart(double * fittnesses,unsigned int from, unsigned int to,unsigned int coreID, Population * pop) ;
double fittness(Genome &g,unsigned int coreID);

Population * pop;

int main(int argv, char** args) {
    std::cout << "start main" << endl;
    VacEnvWindow::init(256,-50,50,-50,50);
    std::thread first (VacEnvWindow::run);
    std::cout << "window open?" << endl;
    std::freopen("output.txt","w",stdout);

    Parameters params;

    params.GeometrySeed = true;

    params.PopulationSize = 512;
    params.DynamicCompatibility = true;
    params.WeightDiffCoeff = 4.0;
    params.CompatTreshold = 2.0;
    params.YoungAgeTreshold = 15;
    params.SpeciesMaxStagnation = 15;
    params.OldAgeTreshold = 35;
    params.MinSpecies = 5;
    params.MaxSpecies = 10;
    params.RouletteWheelSelection = false;
    params.RecurrentProb = 0.0;
    params.OverallMutationRate = 0.8;
    params.MutateWeightsProb = 0.90;

    params.WeightMutationMaxPower = 2.5;
    params.WeightReplacementMaxPower = 5.0;
    params.MutateWeightsSevereProb = 0.5;
    params.WeightMutationRate = 0.25;

    params.MaxWeight = 8;

    params.MutateAddNeuronProb = 0.03;
    params.MutateAddLinkProb = 0.05;
    params.MutateRemLinkProb = 0.0;

    params.MinActivationA = 4.9;
    params.MaxActivationA = 4.9;

    params.ActivationFunction_SignedSigmoid_Prob = 0.0;
    params.ActivationFunction_UnsignedSigmoid_Prob = 1.0;
    params.ActivationFunction_Tanh_Prob = 0.5;
    params.ActivationFunction_SignedStep_Prob = 0.0;

    params.CrossoverRate = 0.75 ;
    params.MultipointCrossoverRate = 0.4;
    params.SurvivalRate = 0.2;

    params.MutateNeuronTraitsProb = 0;
    params.MutateLinkTraitsProb = 0;

    params.AllowLoops = true;

    params.DivisionThreshold = 0.5;
    params.VarianceThreshold = 0.03;
    params.BandThreshold = 0.3;
    params.InitialDepth = 2;
    params.MaxDepth = 3;
    params.IterationLevel = 1;
    params.Leo = false;
    params.GeometrySeed = false;
    params.LeoSeed = false;
    params.LeoThreshold = 0.3;
    params.CPPN_Bias = -1.0;
    params.Qtree_X = 0.0;
    params.Qtree_Y = 0.0;
    params.Width = 10.;
    params.Height = 10.;

    //params.CustomConstraints = constraints;

    std::cout << "create genom" << endl;
    Genome g(0,8, 8,5, true, NEAT::SOFTPLUS, NEAT::RELU, 0, params);// ignored for seed_type == 0, specifies number of hidden units if seed_type == 1

    std::cout << "create pop" << endl;



    pop = new  Population (g, params, true, 1.0, SEED);


    std::cout << "simulation" << endl;
    simulation(pop);
    std::cout << "end" << endl;
    first.join();
    VacEnvWindow::close();
    delete pop;
    pop = nullptr;
    return 0;

}

void simulation(Population * pop) {

    for(unsigned int generation = 0 ; generation < 15000 ; generation++) {
        std::cout << ";Generation;" << generation << ";";

        epoch(generation, pop);

        Genome g = pop->GetBestGenome();
        g.HasLoops();
        g.CalculateDepth();
        std::cout << ";Genome; Depth:; " << g.GetDepth();
        std::cout << ";Loops:; " <<  g.HasLoops() ;
        std::cout << ";NumNeurons:; " <<  g.NumNeurons() << std::endl;
        // std::cout << "Traits:"  << std::endl;
        // g.PrintAllTraits();
        pop->Epoch();

        if(generation % 5 == 4) {
            pop->Save(R"(E:\Oliver\Documents\Population.pop)");
            pop->GetBestGenome().Save(R"(E:\Oliver\Documents\BestGenome.gen)");
            NeuralNetwork net;
            pop->GetBestGenome().BuildPhenotype(net);
            net.Save(R"(E:\Oliver\Documents\BestNet.net)");
        }
    }
}

void epoch( unsigned int generation, Population * pop ){

    std::vector<std::thread> threads;

    int number = pop->NumGenomes();
    int part = number / CORES;
    auto * fittnesses = new double[number];


    for(int i = 0; i < CORES-1 ; i++) {
        threads.emplace_back(std::thread(epochPart,fittnesses,i*part,(i+1)*part,generation,pop));
    }
    threads.emplace_back(std::thread(epochPart,fittnesses,(CORES-1)*part,number,generation,pop));

    for (auto &thread : threads) {
        thread.join();

    }

    std::sort(fittnesses,fittnesses + number);

    std::cout << generation <<"; "<< fittnesses[number - 1] << "; " << fittnesses[number/2] << "; " <<fittnesses[0];
    delete[] fittnesses;
}

void epochPart(double * fittnesses ,unsigned int from, unsigned int to,unsigned int generation, Population * pop)  {


    for(unsigned int i = from; i < to; i++) {

        double fitt = fittness(pop->AccessGenomeByIndex(i),generation);
        fitt += fittness(pop->AccessGenomeByIndex(i),generation);
        pop->AccessGenomeByIndex(i).SetFitness(fitt);
        pop->AccessGenomeByIndex(i).SetEvaluated();

        fittnesses[i]= fitt ;
    }
}

double fittness(Genome & g,unsigned int generation)  {
    double fitt = 0;
    NeuralNetwork net ;
    g.BuildPhenotype(net);

    VacEnv env;

    VacEnvWindow::acc.lock();
    VacEnvWindow::worlds->push_back(&env);
    VacEnvWindow::acc.unlock();

    unsigned long time = 20 * 360 + 20 * 2 * generation;

    std::vector<double > state = env.step(0,1.0f/20.0f);
    std::vector<double > action;

    for(unsigned long i = 0; i < time  && env.running; i++) {

        net.Flush();
        net.Input(state);
        net.Activate();
        action = net.Output();

        int actionEncode = 0;
        double max = 0;
        for(int j = 0; j < action.size(); j++) {
            if(action[j] > max) {
                actionEncode = j;
                max = action[j];
            }
        }

        state = env.step(actionEncode,1.0f/20.0f);
    }

    fitt = env.score;

    VacEnvWindow::acc.lock();
    auto pos = std::find(VacEnvWindow::worlds->begin(), VacEnvWindow::worlds->end(), &env);
    VacEnvWindow::worlds->erase(pos);
    VacEnvWindow::acc.unlock();

    return  fitt;
}
