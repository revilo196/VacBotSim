#include <iostream>
#include <thread>
#include "MultiNEAT/Genome.h"
#include "MultiNEAT/Population.h"
#include "MultiNEAT/Genome.h"
#include "MultiNEAT/NeuralNetwork.h"
#include "MultiNEAT/PhenotypeBehavior.h"
#include "MultiNEAT/Parameters.h"


#include "VacEnv/VacEnv.h"
#include "VacEnv/VacEnvWindow.h"

using namespace NEAT;

#define CORES 14
#define SEED 35216958
#define POP_SIZE 512

void simulation();
void epoch(int generation );
void epochPart(double * fittnesses, int from, int to, int coreID) ;
double fittness(Genome * g,int coreID);

Parameters params;
Population * pop;

int main(int argv, char** args) {
    std::cout << "start main" << endl;
    //VacEnvWindow::init(256,-50,50,-50,50);
    //std::thread first (VacEnvWindow::run);
    std::cout << "window open?" << endl;
    params = Parameters();

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
    params.ActivationFunction_Tanh_Prob = 0.0;
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
    Genome g(0, 7, 8,4, true, NEAT::SOFTPLUS, NEAT::TANH, 0, params, 5);// ignored for seed_type == 0, specifies number of hidden units if seed_type == 1

    std::cout << "create pop" << endl;

    try {

        pop = new Population(g, params, true, 1.0, SEED);

    } catch (...) {
        std::cout << "ERROR" << endl;
    }
    std::cout << "test" << endl;
    //epoch(1);
    std::cout << "end" << endl;
    //first.join();
    //VacEnvWindow::close();
    return 0;

}

void simulation() {

    for(int generation = 0 ; generation < 2048 ; generation++) {
        std::cout << "Generation" << generation << endl;

        epoch(generation);
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

void epoch(int generation ){
    double fittnesses[POP_SIZE];
    std::thread* threads[CORES];


    int number = 512;//pop->NumGenomes();
    int part = number / CORES;

    for(int i = 0; i < CORES ; i++) {
        std::cout << "start core" << i << endl;
        threads[i] = new std::thread(epochPart,fittnesses,i*part,(i+1)*part,i);
    }

    for (auto &thread : threads) {
        std::cout << "stop core" << endl;
        thread->join();
        delete thread;
    }

    std::sort(fittnesses,fittnesses + POP_SIZE - 1);

    std::cout << generation <<" "<< fittnesses[POP_SIZE - 1] << " " << fittnesses[POP_SIZE/2] << " " <<fittnesses[0] << endl;
}

void epochPart(double * fittnesses ,int from, int to, int coreID)  {
    for(int i = from; i < to; i++) {
        fittnesses[i]  = i;
        std::cout << i  << endl;
    }
}

double fittness(Genome * g,int coreID)  {
    return  0.0;
}
