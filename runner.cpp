#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

void redistribute(vector <pair <string, pair <double, double>>> & v, double zonal)
{
    //function to redistribute farm level prediction
    //based in the total production prediction of the zone

    double currTotal = 0;
    for(auto it : v) currTotal += it.second.first;

    double diff = zonal-currTotal;

    //distribute the difference proportionally among the wind farms

    map <string, double> w; //redistribution proportionality weight of each farm
    double totalWeight = 0;

    for(auto it : v)
    {
        double weight = ((1.0*it.second.first)/it.second.second);
        totalWeight += weight;
        w[it.first] = weight;
    }

    for(int i=0; i<v.size(); i++)
    {
        v[i].second.first += (w[v[i].first]/totalWeight)*diff;
        //forecast cannot exceed capacity
        v[i].second.first = min(v[i].second.first, v[i].second.second);
    }

    //if some plant gets filled up, some energy will be leftover
    //re-run to distribute that energy in other plants
    double new_zonal = 0;
    for(auto it : v) new_zonal += it.second.first;
    if(new_zonal != zonal) redistribute(v, zonal);

}

int main()
{

    //Get input from the file
    
    string filename = "input.csv";

    ifstream inputFile;
    inputFile.open(filename);

    //zone wise data
    vector < vector <pair <string, pair <double, double>>> > zoneForecast (4);  //{e, w, n, s}

    string line = "";
    while(getline(inputFile, line))
    {
        string name;
        double forecast, capacity;
        string tempString;

        stringstream inputString(line);

        getline(inputString, name, ',');
        if(name[0] != 'E' and name[0] != 'W' and name[0] != 'N' and name[0] != 'S') continue;
        getline(inputString, tempString, ',');
        forecast = stod(tempString);
        getline(inputString, tempString, ',');
        capacity = stod(tempString);

        if(name[0] == 'E') zoneForecast[0].push_back({name, {forecast, capacity}});
        if(name[0] == 'W') zoneForecast[1].push_back({name, {forecast, capacity}});
        if(name[0] == 'N') zoneForecast[2].push_back({name, {forecast, capacity}});
        if(name[0] == 'S') zoneForecast[3].push_back({name, {forecast, capacity}});
    }


    //input complete

    //redistribute the zonal data
    int state_forecast = 12000;

    double zonal[] = {2800, 2000, 1500, 6500};  //{e, w, n, s}
    double currTotal = 0;   //current total of all the zones
    for(auto it : zonal) currTotal += it;

    double diff = state_forecast-currTotal;

    for(int i=0; i<4; i++)
    {
        zonal[i] += (zonal[i]/currTotal)*diff;
    }
    cout<<endl;

    //after we have the updated zonal data
    //redistribute the each wind farm based on the new zonal prediction

    for(int i=0; i<4; i++)
    {
        redistribute(zoneForecast[i], zonal[i]);
    }

    //printing the final answer in required format

    for(int i=0; i<4; i++)
    {
        for(auto it : zoneForecast[i])
        {
            cout<<it.first<<","<<it.second.first<<endl;
        }
    }

    return 0;
}
