#include "genlicense.cpp"
#include "genmasterpair.cpp"
#include "gensecondarypair.cpp"

#include <string>
#include <random>
#include <fstream>
#include <iostream>
using namespace std;
#include <crypto++/rsa.h>
#include <crypto++/osrng.h>
#include <crypto++/base64.h>
#include <crypto++/files.h>
#include <crypto++/aes.h>
#include <crypto++/modes.h>
#include <crypto++/ripemd.h>
using namespace CryptoPP;

int main(){
    // master key pair
    GenKeyPair();
    // secondary key pair
    ifstream readFile("master.key");
    char temp[1024] = {0};
    readFile >> temp;
	string pass = temp;

	random_device gen;
	string pass2 = to_string(gen());

	ofstream ofile;               
    ofile.open("secondary.key");
	ofile << pass2;
	ofile.close();

	AutoSeededRandomPool rng;
	string pubkey = GenSecondaryKeyPair(rng, pass2);
	SignSecondaryKey(rng, pubkey, pass);

    ifstream readFile2("secondary.key");
    char tempSec[1024] = {0};
    readFile2 >> tempSec;
	string passSec = tempSec;

	FILE *pf = popen("./getserial | grep serial | head -n 1", "r");
    char res[1024];
    fread(res, 1024, 1, pf);
    pclose(pf);
        string serial = res;
	printf("license %s",res);

	AutoSeededRandomPool rng2;
	SignLicense(rng2, serial, passSec);
}
