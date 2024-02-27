#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

string kernel();
void CPU();
string RAM();
void GPU();

int main() {
  cout << "\nOperating System: Linux" << endl;
  cout << "OS version: " << kernel() << endl;
  CPU();
  cout << "Max RAM: " << RAM() << endl;
  cout << "\nGPUs: ";
  GPU();
  return 0;
}

string kernel() {
  string command;
  command = "uname -r";
  string kernelVersion;

  FILE *pipe = popen(command.c_str(), "r");
  if (pipe) {
    char buffer[128];
    while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != nullptr) {
        kernelVersion += buffer;
      }
    }
    pclose(pipe);
  }
  return kernelVersion;
}

void CPU() {
  string command;
  command = "cat /proc/cpuinfo";
  string cpuInfo;

  FILE *pipe = popen(command.c_str(), "r");
  if (pipe) {
    char buffer[128];
    while (!feof(pipe)) {
      if (fgets(buffer, 128, pipe) != nullptr) {
        cpuInfo += buffer;
      }
    }
    pclose(pipe);
  }
  istringstream iss(cpuInfo);
  string line;

  string vendorId;
  string modelName;
  int processorCount = 0;
  vector<double> cpuMHzList;
  while (getline(iss, line)) {
    if (line.find("vendor_id") != string::npos) {
      vendorId = line.substr(line.find(":") + 2);
    } else if (line.find("model name") != string::npos) {
      modelName = line.substr(line.find(":") + 2);
    } else if (line.find("processor") != string::npos) {
      processorCount++;
    } else if (line.find("cpu MHz") != string::npos) {
      double cpuMHz = stod(line.substr(line.find(":") + 2));
      cpuMHzList.push_back(cpuMHz);
    }
  }
  cout << "CPU Vendor ID: " << vendorId << endl;
  cout << "CPU Model Name: " << modelName << endl;
  cout << "Processor Count: " << processorCount << endl;
  for (int i = 0; i < cpuMHzList.size(); i++) {
    cout << "Processor " << i + 1 << " CPU MHz: " << cpuMHzList[i] << endl;
  }
  cout << endl;
}

string RAM() {
  ifstream meminfoFile("/proc/meminfo");
  string line;
  int memTotalMB;
  while (getline(meminfoFile, line)) {
    if (line.find("MemTotal:") != string::npos) {
      string memTotal = line.substr(line.find(":") + 2,
                                    line.find(" kB") - line.find(":") - 2);
      memTotalMB = stoi(memTotal) / 1024;
      break;
    }
  }
  return to_string(memTotalMB);
}

void GPU() {
  FILE *lspciOutput = popen("lspci -v", "r");
  if (!lspciOutput) {
    cerr << "Failed to execute command." << endl;
    return;
  }

  char buffer[256];
  string line;
  string keyword = "VGA compatible controller: ";
  cout << endl;
  while (fgets(buffer, sizeof(buffer), lspciOutput)) {
    line = buffer;
    size_t pos = line.find(keyword);
    if (pos != string::npos) {
      line.erase(0, pos + keyword.length());
      cout << line;
    }
  }
  cout << endl;
  pclose(lspciOutput);
}
