#include<iostream>
#include<math.h>
using namespace std;

// Very basic RSA implementation
// Doesn't fully work as I couldn't get it running with large prime numbers

int isprime(unsigned long long n) {
	/*if((n&1)==0) return n==2;*/
	if(n%3==0) return n==3;
	/*if(n<25) return n>1;*/
	unsigned long long p = 5;
	while (p*p <= n) {
		if (n%p==0) return 0;
		p += 2;
		if (n%p==0) return 0;
		p += 4;
	}
	return 1;
}

unsigned long long rand_prime(int lower, int upper) {
	unsigned long long spread = upper - lower + 1;
	while(1) {
		unsigned long long p = 1 | (rand() % spread + lower);
		if (isprime(p)) return p;
	}
}

int main() {
	//2 random prime numbers
	//unsigned long long lower = 4000000000;
	//unsigned long long upper = 4294967291;
	double p = 5; //rand_prime(lower, upper);
	double q = 7; //rand_prime(lower, upper);
	double n = p*q;
	//phi(n)
	double phi = (p-1)*(q-1);
	
	cout<<"p = "<<p<<"\n";
	cout<<"q = "<<q<<"\n";
	cout<<"n = pq = "<<n<<"\n";
	cout<<"phi = "<<phi<<"\n";
	
	//create public key
	double e = 11; //rand_prime(lower, upper);
	//create private key
	//choosing d such that it satisfies d*e = 1 mod phi
	double d1 = 1/e;
	double d = fmod(d1,phi);
	double message = 9;
	double c = pow(message,e); //encrypt the message
	double m = pow(c,d);
	c = fmod(c,n);
	m = fmod(m,n);
	
	cout<<"Original Message = "<<message<<"\n";
	cout<<"e = "<<e<<"\n";
	cout<<"d = "<<d<<"\n";
	cout<<"Encrypted message = "<<c<<"\n";
	cout<<"Decrypted message = "<<m<<"\n";
	return 0;
}