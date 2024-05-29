#include "data.decl.h"
#include <string>
#include <fstream>

CProxy_Main mainproxy;

class Main : public CBase_Main {
	std::string _fname;
	CProxy_Testers testers;
public:
	Main(CkArgMsg* msg) {
		if(msg -> argc != 2){
			CkPrintf("usage: ./charmrun +pN data <filename>\n"); 
			CkExit();
		}
		_fname = std::string(msg -> argv[1]);
		testers = CProxy_Testers::ckNew(_fname);		
	}

	void finishProgram(){
		CkExit();
	}
};

class Testers : public CBase_Testers {
	char* data_buffer;
	size_t data_size;
	std::string _fname;
	double read_start;
	double read_end;
	double permutation_start;
	double permutation_end;
public:
	Testers(std::string fname) : _fname(fname) {
		std::ifstream ifs;
		ifs.open(_fname,std::ios_base::app);
		ifs.seekg(0, std::ios_base::end);
		data_size = ifs.tellg();
		ifs.seekg(0, std::ios_base::beg);
		ifs.close();
		data_buffer = new char[data_size];
		if(thisIndex == 1){
			CkPostBuffer(data_buffer, data_size, 0); //tag the buffer 0
			CkPrintf("Posting buffer of size %zu\n", data_size);
			thisProxy[0].readFile(0);
		}
	}

	void readFile(int buffer_tag){
		std::ifstream ifs;
		ifs.open(_fname,std::ios_base::app);
		read_start = CkWallTimer();
		ifs.read(data_buffer, data_size);
		read_end = CkWallTimer();
		CkPrintf("time for read: %f\n", (read_end - read_start));
		CkPrintf("Sending buffer of size %zu\n", data_size);
		permutation_start = CkWallTimer();
		thisProxy[1].permuteData(buffer_tag, permutation_start, data_size, CkSendBuffer(data_buffer));
	}

	void permuteData(int buffer_tag, double zero_start, size_t my_data_size, char* data){
		permutation_end = CkWallTimer();
		CkPrintf("total time for data permutation: %f\n", (permutation_end - zero_start));
		mainproxy.finishProgram();
	}

	void permuteData(int buffer_tag, double zero_start, size_t my_data_size, char* data, CkNcpyBufferPost* ncpyPost){
		ncpyPost[0].regMode = CK_BUFFER_REG;
    	ncpyPost[0].deregMode = CK_BUFFER_DEREG;
    	CkMatchBuffer(ncpyPost, 0, buffer_tag);
	}



};
#include "data.def.h"
