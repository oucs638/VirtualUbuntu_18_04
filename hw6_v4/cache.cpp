#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<vector>
#include<map>
#include<string>
#include<cstring>
#include<math.h>
using namespace std;
long Atoi(string s){
	long ans=0;
	for(int i=2;i<s.size();i++){
		char t=s[i];
		if(t>='0'&&t<='9') 
			ans=ans*16+t-'0';
		else 
			ans=ans*16+t-'a'+10;
	}        
	return ans;
}
int main(int argc,char *argv[]){
	fstream source;
	source.open(argv[1],ios::in);
	fstream target;
	target.open(argv[2],ios::out);
	if(!source){
		cout<<"error, no file"<<endl;
		return 1;
	}
	int cachesize;
	int blocksize;
	int associativity;
	int replacement;
	long temp;
	int index;
	int tag,flag,same;
	int n,size,read;
	string temp1;
	source>>cachesize;
	source>>blocksize;
	source>>associativity;
	source>>replacement;
	int offset=log2(blocksize);
	int indexnum;
	if(associativity==0){
		n=1;
		indexnum=log(cachesize*1024/blocksize/n)/log(2);
		size=indexnum;
	}
	if(associativity==1){
		n=4;
		indexnum=log(cachesize*1024/blocksize/n)/log(2);
		size=indexnum;
	}
	if(associativity==2){
		n=cachesize*1024/blocksize;indexnum=0;size=0;
	}
	int s=2<<(size);
	int cache[s][n];
	int zero[s];
	int zero2=n;
	for(int i=0;i<s;i++)
		zero[i]=4;
	while(source>>temp1){
		read=read+1;
		temp=Atoi(temp1);
		tag=temp>>(offset+indexnum);
		index=(temp>>offset)-(tag<<(indexnum));
		if(replacement==0){
			if(associativity==0){
				if(cache[index][0]==0){
					cache[index][0]=tag;
					target<<"-1\n";
				}
				else{
					if(cache[index][0]==tag){
						target<<"-1\n";
					}
					else{
						target<<cache[index][0]<<"\n";
						cache[index][0]=tag;
					}
				}
			}
			if(associativity==1){
				if(zero[index]==0){
					if(cache[index][0]==tag||cache[index][1]==tag||cache[index][2]==tag||cache[index][3]==tag){
						target<<"-1\n";
					}
					else{
						target<<cache[index][0]<<"\n";
						for(int i=0;i<3;i++){
							cache[index][i]=cache[index][i+1];
						}
						cache[index][3]=tag;
					}
				}
				if(zero[index]>0){
					if(cache[index][0]==tag||cache[index][1]==tag||cache[index][2]==tag||cache[index][3]==tag){
						target<<"-1\n";
					}
					else{
						cache[index][4-zero[index]]=tag;
						zero[index]=zero[index]-1;
						target<<"-1\n";
					}
				}
			}
			if(associativity==2){
				if(zero2==0){
					for(int i=0;i<n;i++){
						if(cache[0][i]==tag)
							flag=1;
					}
					if(flag==1)
						target<<"-1\n";
					else{
						target<<cache[0][0]<<"\n";
						for(int i=0;i<n-1;i++){
							cache[0][i]=cache[0][i+1];
						}
						cache[0][n-1]=tag;
					}
					flag=0;
				}

				if(zero2>0){
					target<<"-1\n";
					for(int i=0;i<n;i++){
						if(cache[0][i]==tag)
							flag=1;
					}
					if(flag==1)
						flag=0;
					else{
						cache[0][n-zero2]=tag;
						zero2=zero2-1;
					}
				}
			}				
		}
		if(replacement==1){
			if(associativity==0){
				if(cache[index][0]==0){
					cache[index][0]=tag;
					target<<"-1\n";
				}
				else{
					if(cache[index][0]==tag){
						target<<"-1\n";
						cout<<"5151\n";
					}
					else{
						target<<cache[index][0]<<"\n";
						cache[index][0]=tag;
					}
				}
			}
			if(associativity==1){
				if(zero[index]==0){
					if(cache[index][0]==tag)
						same=0;
					if(cache[index][1]==tag)
						same=1;
					if(cache[index][2]==tag)
						same=2;
					if(cache[index][3]==tag)
						same=3;
					if(cache[index][0]==tag||cache[index][1]==tag||cache[index][2]==tag||cache[index][3]==tag){
						if(same==3)
							target<<"-1\n";
						else{
							target<<"-1\n";
							for(int i=same;i<3;i++){
								cache[index][i]=cache[index][i+1];
							}
							cache[index][3]=tag;
						}
						flag=0;
					}
					else{
						target<<cache[index][0]<<"\n";
						for(int i=0;i<3;i++){
							cache[index][i]=cache[index][i+1];
						}
						cache[index][3]=tag;
					}
				}

				if(zero[index]>0){
					if(cache[index][0]==tag)
						same=0;
					if(cache[index][1]==tag)
						same=1;
					if(cache[index][2]==tag)
						same=2;
					if(cache[index][3]==tag)
						same=3;
					if(cache[index][0]==tag||cache[index][1]==tag||cache[index][2]==tag||cache[index][3]==tag){
						if(same==4-zero[index]-1)
							target<<"-1\n";
						else{
							target<<"-1\n";
							for(int i=same;i<(4-zero[index]-1);i++){
								cache[index][i]=cache[index][i+1];
							}
							cache[index][4-zero[index]-1]=tag;
						}
					}
					else{
						target<<"-1\n";
						cache[index][4-zero[index]]=tag;
						zero[index]=zero[index]-1;
					}
				}
			}
			if(associativity==2){
				if(zero2==0){
					for(int i=0;i<n;i++){
						if(cache[0][i]==tag){
							flag=1;
							same=i;
						}
					}
					if(flag==1){
						if(same==n-1){
							target<<"-1\n";
						}
						else{
							target<<"-1\n";
							for(int i=same;i<n-1;i++){
								cache[0][i]=cache[0][i+1];
							}
							cache[0][n-1]=tag;
						}
						flag=0;
					}               
					else{
						target<<cache[0][0]<<"\n";
						for(int i=0;i<n-1;i++){
							cache[0][i]=cache[0][i+1];
						}
						cache[0][n-1]=tag;
					}
				}      
				if(zero2>0){
					for(int i=0;i<n;i++){
						if(cache[0][i]==tag){
							flag=1;
							same=i;
						}
					}
					if(flag==1){
						if(same==n-zero2-1){
							flag=0;
							target<<"-1\n"; 
						}
						else{
							target<<"-1\n"; 
							for(int i=same;i<n-zero2-1;i++){
								cache[0][i]=cache[0][i+1];
							}
							cache[0][n-zero2-1]=tag;
							flag=0;
						}
					}
					else{
						target<<"-1\n"; 
						cache[0][n-zero2]=tag;
						zero2=zero2-1;
					}
				}
			}
		}
	if(replacement==2){
		if(associativity==0){
			if(cache[index][0]==0){
				cache[index][0]=tag;
				target<<"-1\n";
			}
			else{
				if(cache[index][0]==tag){
					target<<"-1\n";
				}
				else{
					target<<cache[index][0]<<"\n";
					cache[index][0]=tag;
				}
			}
		}
		if(associativity==1){
			if(zero[index]==0){
				if(cache[index][0]==tag||cache[index][1]==tag||cache[index][2]==tag||cache[index][3]==tag){
					target<<"-1\n";
				}
				else{
					target<<cache[index][3]<<"\n";
					for(int i=3;i>0;i--){
						cache[index][i]=cache[index][i-1];
					}
					cache[index][0]=tag;
				}
			}
			if(zero[index]>0){
				if(cache[index][0]==tag||cache[index][1]==tag||cache[index][2]==tag||cache[index][3]==tag){
					target<<"-1\n";
				}
				else{
					cache[index][4-zero[index]]=tag;
					zero[index]=zero[index]-1;
					target<<"-1\n";
				}
			}
		}
		if(associativity==2){
			if(zero2==0){
				for(int i=0;i<n;i++){
					if(cache[0][i]==tag)
						flag=1;
				}
				if(flag==1)
					target<<"-1\n";
				else{
					target<<cache[0][n-1]<<"\n";
					for(int i=n-1;i>0;i--){
						cache[0][i]=cache[0][i-1];
					}
					cache[0][0]=tag;
				}
				flag=0;
			}

			if(zero2>0){
				target<<"-1\n";
				for(int i=0;i<n;i++){
					if(cache[0][i]==tag)
						flag=1;
				}
				if(flag==1)
					flag=0;
				else{
					cache[0][n-zero2]=tag;
					zero2=zero2-1;
				}
			}
		}     

	}
	}

	return 0;
}
