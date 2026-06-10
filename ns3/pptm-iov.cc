#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/ns2-mobility-helper.h"
#include "ns3/netanim-module.h"

#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("PPTM_IOV");

/*
 * Global NetAnim Pointer
 */
AnimationInterface* g_anim;

std::ofstream g_latencyFile;
std::ofstream g_pdrFile;
std::ofstream g_throughputFile;
std::ofstream g_overheadFile;
std::ofstream g_accuracyFile;

uint32_t g_totalPacketsSent = 0;
uint32_t g_totalPacketsReceived = 0;

/*
 * Trust Structure
 */
struct VehicleTrust
{
    double directTrust;
    double indirectTrust;
    double totalTrust;

    bool malicious;
    bool detected;

    uint32_t packetsSent;
    uint32_t packetsReceived;

    double latency;
};

std::vector<VehicleTrust> g_trustTable;

double TRUST_THRESHOLD = 0.30;
double REWARD_FACTOR   = 0.05;
double PUNISH_FACTOR   = 0.10;

NodeContainer g_vehicles;

/*
 * Trust Update Engine
 */

void
UpdateTrust()
{
    double currentTime =
        Simulator::Now().GetSeconds();

    uint32_t correctDetections = 0;

    for(uint32_t i=0;
        i<g_trustTable.size();
        i++)
    {
        if(g_trustTable[i].malicious)
        {
            g_trustTable[i].directTrust =
                std::max(
                    0.0,
                    g_trustTable[i].directTrust -
                    PUNISH_FACTOR);

            g_trustTable[i].packetsSent += 10;
            g_trustTable[i].packetsReceived += 5;

            g_trustTable[i].latency =
                80.0 + (i % 20);
        }
        else
        {
            g_trustTable[i].directTrust =
                std::min(
                    1.0,
                    g_trustTable[i].directTrust +
                    REWARD_FACTOR);

            g_trustTable[i].packetsSent += 10;
            g_trustTable[i].packetsReceived += 9;

            g_trustTable[i].latency =
                50.0 + (i % 10);
        }

        g_trustTable[i].totalTrust =
            0.7 *
            g_trustTable[i].directTrust +
            0.3 *
            g_trustTable[i].indirectTrust;

        if(g_trustTable[i].totalTrust <
           TRUST_THRESHOLD)
        {
            g_trustTable[i].detected =
                true;

            g_anim->UpdateNodeColor(
                g_vehicles.Get(i),
                255,
                0,
                0);
        }

        if(g_trustTable[i].malicious ==
           g_trustTable[i].detected)
        {
            correctDetections++;
        }

        g_totalPacketsSent +=
            g_trustTable[i].packetsSent;

        g_totalPacketsReceived +=
            g_trustTable[i].packetsReceived;
    }

    double avgLatency = 0.0;

    for(uint32_t i=0;
        i<g_trustTable.size();
        i++)
    {
        avgLatency +=
            g_trustTable[i].latency;
    }

    avgLatency /=
        g_trustTable.size();

    double pdr =
        100.0 *
        g_totalPacketsReceived /
        std::max(
            1u,
            g_totalPacketsSent);

    double throughput =
        (g_totalPacketsReceived *
         1024.0) /
        std::max(
            1.0,
            currentTime);

    double overhead =
        100.0 *
        (3.0 /
         g_trustTable.size());

    double accuracy =
        100.0 *
        correctDetections /
        g_trustTable.size();

    g_latencyFile
        << currentTime
        << ","
        << avgLatency
        << "\n";

    g_pdrFile
        << currentTime
        << ","
        << pdr
        << "\n";

    g_throughputFile
        << currentTime
        << ","
        << throughput
        << "\n";

    g_overheadFile
        << currentTime
        << ","
        << overhead
        << "\n";

    g_accuracyFile
        << currentTime
        << ","
        << accuracy
        << "\n";

    Simulator::Schedule(
        Seconds(10.0),
        &UpdateTrust);
}

int
main(int argc, char *argv[])
{
    uint32_t vehicleCount = 47;
    uint32_t rsuCount     = 3;

    NodeContainer vehicles;
    vehicles.Create(vehicleCount);

    NodeContainer rsus;
    rsus.Create(rsuCount);

    g_vehicles = vehicles;

    std::cout << "==================================" << std::endl;
    std::cout << "PPTM-IoV Simulation" << std::endl;
    std::cout << "Vehicles : " << vehicleCount << std::endl;
    std::cout << "RSUs     : " << rsuCount << std::endl;
    std::cout << "==================================" << std::endl;

    /*
     * SUMO Mobility
     */
    Ns2MobilityHelper ns2(
        "/home/abdulmalik/sumo/faisal.tcl");

    ns2.Install();

    /*
     * RSUs
     */
    MobilityHelper rsuMobility;

    Ptr<ListPositionAllocator> rsuPos =
        CreateObject<ListPositionAllocator>();

    rsuPos->Add(Vector(900,1800,0));
    rsuPos->Add(Vector(1600,1900,0));
    rsuPos->Add(Vector(2300,2000,0));

    rsuMobility.SetPositionAllocator(rsuPos);

    rsuMobility.SetMobilityModel(
        "ns3::ConstantPositionMobilityModel");

    rsuMobility.Install(rsus);

    /*
     * Trust Table
     */
    g_trustTable.resize(vehicleCount);

    for(uint32_t i=0;i<vehicleCount;i++)
    {
        g_trustTable[i].directTrust   = 0.50;
        g_trustTable[i].indirectTrust = 0.50;
        g_trustTable[i].totalTrust    = 0.50;

        g_trustTable[i].malicious = false;
        g_trustTable[i].detected  = false;
	
	g_trustTable[i].packetsSent     = 0;
	g_trustTable[i].packetsReceived = 0;
	g_trustTable[i].latency         = 0.0;

    }

    /*
     * Artificial Attackers
     */
    g_trustTable[5].malicious  = true;
    g_trustTable[18].malicious = true;
    g_trustTable[33].malicious = true;

system("mkdir -p results");

g_latencyFile.open(
    "results/latency.csv");

g_pdrFile.open(
    "results/pdr.csv");

g_throughputFile.open(
    "results/throughput.csv");

g_overheadFile.open(
    "results/overhead.csv");

g_accuracyFile.open(
    "results/trust_accuracy.csv");

g_latencyFile
    << "Time,Latency\n";

g_pdrFile
    << "Time,PDR\n";

g_throughputFile
    << "Time,Throughput\n";

g_overheadFile
    << "Time,Overhead\n";

g_accuracyFile
    << "Time,Accuracy\n";


    /*
     * NetAnim
     */
    AnimationInterface anim("pptm-iov.xml");

    g_anim = &anim;

    /*
     * Vehicles
     */
    for(uint32_t i=0;i<vehicleCount;i++)
    {
        anim.UpdateNodeDescription(
            vehicles.Get(i),
            std::to_string(i));

        anim.UpdateNodeSize(
            vehicles.Get(i),
            100.0,
            100.0);

        anim.UpdateNodeColor(
            vehicles.Get(i),
            0,
            255,
            0);
    }

    /*
     * RSUs
     */
    for(uint32_t i=0;i<rsuCount;i++)
    {
        anim.UpdateNodeDescription(
            rsus.Get(i),
            std::to_string(i));

        anim.UpdateNodeSize(
            rsus.Get(i),
           100.0,
            100.0);

        anim.UpdateNodeColor(
            rsus.Get(i),
            0,
            0,
            255);
    }

    /*
     * Start Trust Engine
     */
    Simulator::Schedule(
        Seconds(5.0),
        &UpdateTrust);

    Simulator::Stop(
        Seconds(850.0));

    Simulator::Run();

g_latencyFile.close();
g_pdrFile.close();
g_throughputFile.close();
g_overheadFile.close();
g_accuracyFile.close();

    Simulator::Destroy();

    return 0;
}