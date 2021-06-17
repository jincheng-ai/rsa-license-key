//g++ genmasterpair.cpp -lcrypto++ -o genmasterpair

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

void GenKeyPair()
{
	// cout << "Enter new master key password" << endl;
	// string pass;
	// cin >> pass;
	random_device gen;
	string pass = to_string(gen());
	ofstream ofile;               
    ofile.open("master.key");
	ofile << pass;
	ofile.close();

	// InvertibleRSAFunction is used directly only because the private key
	// won't actually be used to perform any cryptographic operation;
	// otherwise, an appropriate typedef'ed type from rsa.h would have been used.
	AutoSeededRandomPool rng;
	InvertibleRSAFunction privkey;
	privkey.Initialize(rng, 1024*8);

	// With the current version of Crypto++, MessageEnd() needs to be called
	// explicitly because Base64Encoder doesn't flush its buffer on destruction.
	string privKeyDer;
	StringSink privKeyDerSink(privKeyDer);
	privkey.DEREncode(privKeyDerSink);

	//Hash the pass phrase to create 128 bit key
	string hashedPass;
	RIPEMD128 hash;
	StringSource(pass, true, new HashFilter(hash, new StringSink(hashedPass)));
	
	// Generate a random IV
	byte iv[AES::BLOCKSIZE];
	rng.GenerateBlock(iv, AES::BLOCKSIZE);

	//Encrypt private key
	CFB_Mode<AES>::Encryption cfbEncryption((const unsigned char*)hashedPass.c_str(), hashedPass.length(), iv);
	byte encPrivKey[privKeyDer.length()];
	cfbEncryption.ProcessData(encPrivKey, (const byte*)privKeyDer.c_str(), privKeyDer.length());
	string encPrivKeyStr((char *)encPrivKey, privKeyDer.length());

	//Save private key to file
	StringSource encPrivKeySrc(encPrivKeyStr, true);
	Base64Encoder sink(new FileSink("master-privkey-enc.bin"));
	encPrivKeySrc.CopyTo(sink);
	sink.MessageEnd();

	//Save initialization vector key to file
	StringSource ivStr(iv, AES::BLOCKSIZE, true);
	Base64Encoder sink2(new FileSink("master-privkey-iv.bin"));
	ivStr.CopyTo(sink2);
	sink2.MessageEnd();

	// Suppose we want to store the public key separately,
	// possibly because we will be sending the public key to a third party.
	RSAFunction pubkey(privkey);
	
	Base64Encoder pubkeysink(new FileSink("master-pubkey.bin"));
	pubkey.DEREncode(pubkeysink);
	pubkeysink.MessageEnd();



}

// int main()
// {
// 	GenKeyPair();
// }


