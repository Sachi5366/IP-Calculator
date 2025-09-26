#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Subnet {
    int requiredHosts;
    int totalIPs;
    int cidr;
    string subnetMask;
    string networkAddress;
    string broadcastAddress;
    string usableRange;
};

string calculateSubnetMask(int cidr) {
    int mask[4] = {0};
    for (int i = 0; i < cidr; i++) {
        mask[i / 8] += (1 << (7 - (i % 8)));
    }
    return to_string(mask[0]) + "." + to_string(mask[1]) + "." + to_string(mask[2]) + "." + to_string(mask[3]);
}

void incrementIP(vector<int> &ip, int increment) {
    ip[3] += increment;
    for (int i = 3; i > 0; i--) {
        if (ip[i] > 255) {
            ip[i] -= 256;
            ip[i - 1]++;
        }
    }
}

string ipToString(vector<int> ip) {
    return to_string(ip[0]) + "." + to_string(ip[1]) + "." + to_string(ip[2]) + "." + to_string(ip[3]);
}

int nextPowerOfTwo(int n) {
    int power = 1;
    while (power < n)
        power <<= 1;
    return power;
}

int calculateCIDR(int totalIPs) {
    int bits = 0;
    while ((1 << bits) < totalIPs) bits++;
    return 32 - bits;
}

void printSubnetInfo(const Subnet &s, int index) {
    cout << "\nSubnet " << index + 1 << ":\n";
    cout << "Required Hosts   : " << s.requiredHosts << endl;
    cout << "Allocated IPs    : " << s.totalIPs << endl;
    cout << "CIDR Notation    : /" << s.cidr << endl;
    cout << "Subnet Mask      : " << s.subnetMask << endl;
    cout << "Network Address  : " << s.networkAddress << endl;
    cout << "Usable IP Range  : " << s.usableRange << endl;
    cout << "Broadcast Address: " << s.broadcastAddress << endl;
}

int main() {
    vector<Subnet> subnets;
    vector<int> baseIP = {14, 24, 74, 0};

    vector<int> requiredHosts = {120, 60, 10}; // A, B, C

    // Sort descending for proper VLSM allocation
    sort(requiredHosts.begin(), requiredHosts.end(), greater<int>());

    for (int i = 0; i < requiredHosts.size(); i++) {
        Subnet s;
        s.requiredHosts = requiredHosts[i];
        s.totalIPs = nextPowerOfTwo(requiredHosts[i] + 2); // +2 for net and broadcast
        s.cidr = calculateCIDR(s.totalIPs);
        s.subnetMask = calculateSubnetMask(s.cidr);
        s.networkAddress = ipToString(baseIP);

        vector<int> broadcastIP = baseIP;
        incrementIP(broadcastIP, s.totalIPs - 1);
        s.broadcastAddress = ipToString(broadcastIP);

        vector<int> firstUsable = baseIP;
        incrementIP(firstUsable, 1);
        vector<int> lastUsable = broadcastIP;
        lastUsable[3] -= 1;
        s.usableRange = ipToString(firstUsable) + " - " + ipToString(lastUsable);

        incrementIP(baseIP, s.totalIPs); // Update baseIP for next subnet

        subnets.push_back(s);
    }

    cout << "\n--- IP Subnetting using VLSM ---\n";
    for (int i = 0; i < subnets.size(); i++) {
        printSubnetInfo(subnets[i], i);
    }

    return 0;
}
