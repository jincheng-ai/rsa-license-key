all:
	g++ app.cpp -lcrypto++ -o app
	g++ -c verifylicense.cpp -lcrypto++ 
	g++ verify.cpp -lcrypto++ -o verify
